#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFont>
#include <QMenu>
#include <QContextMenuEvent>
#include <QPushButton>
#include <QDebug>

#include <chrono>
#include <sstream>

#include "schedulegriditem.h"

QString stoq(const std::string& s){
    return QString::fromStdString(s);
}

namespace OATS{

    Panel::Panel(QWidget* parent)
        :m_parent{parent}
    {
      setFrameShape(QFrame::Panel);
      setFrameShadow(QFrame::Plain);
      setLineWidth(1);
      setFixedHeight(60);
    }

    Panel::~Panel()
    {
    }

    int Panel::index()
    {
        return dynamic_cast<ScheduleGridItem*>(m_parent)->grid_index();
    }

    int Panel::parent_index()
    {
        return dynamic_cast<ScheduleGridItem*>(m_parent)->subject_index();
    }

    int Panel::parent_ref()
    {
        return dynamic_cast<ScheduleGridItem*>(m_parent)->subject_ref();
    }

    QWidget* Panel::parent()
    {
        return m_parent;
    }

    QTime Panel::msec_to_time(int h, int ms)
    {
        auto msecs = std::chrono::milliseconds(ms);
        auto hr = std::chrono::duration_cast<std::chrono::hours>(msecs);
        msecs -= hr;
        auto mins = std::chrono::duration_cast<std::chrono::minutes>(msecs);
        msecs -= mins;
        auto secs = std::chrono::duration_cast<std::chrono::seconds>(msecs);
        QTime t(h, mins.count(), secs.count());
        return t;
    }

    OATS::TimeFmt Panel::dur_to_tfmt(long ms)
    {
        long h = ms / 3600000;
        ms = ms - 3600000 * h;

        long m = ms / 60000;
        ms = ms - 60000 * m;

        long s = ms / 1000;
        ms = ms - 1000 * s;

        TimeFmt tf;
        tf.hr = h;
        tf.min = m;
        tf.sec = s;
        tf.msec = ms;

        return tf;

    }

    /* -------------- TimePanel -------------- */

    TimePanel::TimePanel(QWidget* parent)
        :Panel(parent)
    {
        m_layout = new QVBoxLayout();
        m_layout->setSpacing(0);
        m_layout->setContentsMargins(0,0,0,0);

        setFixedWidth(80);
        QFont f;
        f.setBold(true);
        f.setPointSize(10);

        m_label = new QLabel("00:00:00");
        m_label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_label->setFont(f);

        m_layout->addWidget(m_label);
        setLayout(m_layout);
    }

    TimePanel::~TimePanel()
    {
        delete m_layout;
    }

    void TimePanel::set_label(QString text)
    {
        m_label->setText(text);
    }

    void TimePanel::time_menu()
    {
        qDebug() << "Time Panel... "<< index();
    }

    void TimePanel::contextMenuEvent(QContextMenuEvent* event)
    {
        QMenu menu(this);
        auto act_time = new QAction("Time Menu");
        connect(act_time, &QAction::triggered, this, &TimePanel::time_menu);
        menu.addAction(act_time);
        menu.exec(event->globalPos());
    }


    void TimePanel::update(ScheduleItem* schedule_item)
    {
        QTime t = msec_to_time(schedule_item->hour(), schedule_item->current_time());
        set_label(t.toString("HH:mm:ss"));
    }

    QVBoxLayout* TimePanel::main_layout()
    {
        return m_layout;
    }

    /* -------------- TrackPanel -------------- */

    TrackPanel::TrackPanel(QWidget* parent)
        :Panel(parent)
    {
        constexpr static int COLUMN_0 = 0;
        constexpr static int COLUMN_1 = 1;
        constexpr static int ROW_0 = 0;
        constexpr static int ROW_1 = 1;

        setFixedWidth(385);

        m_layout = new QGridLayout();

        QFont f;
        f.setBold(true);
        f.setPointSize(12);
        m_track_label = new QLabel("Track Title");
        m_track_label->setFont(f);

        m_artist_label = new QLabel("Artist");
        m_duration_label = new QLabel("03:02:07");

        m_layout->addWidget(m_track_label, ROW_0, COLUMN_0);
        m_layout->addWidget(m_duration_label, ROW_0, COLUMN_1);
        m_layout->addWidget(m_artist_label, ROW_1, COLUMN_0);
        m_layout->setColumnStretch(COLUMN_0,1);
        m_layout->setContentsMargins(5,0,5,0);

        setLayout(m_layout);

    }


    TrackPanel::~TrackPanel()
    {
        delete m_layout;
    }

    void TrackPanel::contextMenuEvent(QContextMenuEvent* event)
    {
        QMenu menu(this);
        auto act_move_up = new QAction("Move Up");
        auto act_move_down = new QAction("Move Down");
        auto act_make_current = new QAction("Make Current");
        auto act_delete_item = new QAction("Delete Item");

        connect(act_move_up, &QAction::triggered, this, TrackPanel::move_up);
        connect(act_move_down, &QAction::triggered, this, TrackPanel::move_down);
        connect(act_make_current, &QAction::triggered, this, TrackPanel::make_audio_current);
        connect(act_delete_item, &QAction::triggered, this, TrackPanel::delete_item);

        menu.addAction(act_move_up);
        menu.addAction(act_move_down);
        menu.addAction(act_make_current);
        menu.addAction(act_delete_item);

        menu.exec(event->globalPos());

    }

    void TrackPanel::mousePressEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton){
            emit dynamic_cast<ScheduleGridItem*>(parent())->insert_item(parent_index(), index());
        }
    }

    void TrackPanel::move_up()
    {
        emit dynamic_cast<ScheduleGridItem*>(parent())->move_up(parent_ref(), index());
    }

    void TrackPanel::move_down()
    {
        emit dynamic_cast<ScheduleGridItem*>(parent())->move_down(parent_ref(), index());
    }

    void TrackPanel::make_audio_current()
    {
        emit dynamic_cast<ScheduleGridItem*>(parent())->make_current(parent_ref(), index());
    }

    void TrackPanel::delete_item()
    {
        emit dynamic_cast<ScheduleGridItem*>(parent())->delete_item(parent_ref());
    }

//    void TrackPanel::play_item()
//    {
//        emit dynamic_cast<ScheduleGridItem*>(parent())->play_item(parent_ref(), index());
//    }

    void TrackPanel::update(ScheduleItem* schedule_item)
    {
        m_track_label->setText(QString::fromStdString(schedule_item->audio().title()));
        m_artist_label->setText(QString::fromStdString(schedule_item->audio().artist()));

        QTime t = msec_to_time(schedule_item->hour(), schedule_item->audio().duration());
//        TimeFmt tf = dur_to_tfmt(schedule_item->audio().duration());
//        std::stringstream ss;
//        ss << tf.min << ":" << tf.sec << ":" << tf.msec;
//        m_duration_label->setText(QString::fromStdString(ss.str()));

        m_duration_label->setText(t.toString("mm:ss"));

        setStyleSheet("");

        if (schedule_item->schedule_type() == OATS::ScheduleType::COMM){
            setStyleSheet("background-color: rgb(255, 255, 127)");
        }

//        if (schedule_item->item_status() == OATS::ItemStatus::PLAYING){
//            setStyleSheet("background-color: rgb(78, 207, 42)");
//        }
    }

    /* -------------- StatusPanel -------------- */

    StatusPanel::StatusPanel(QWidget* parent)
        :Panel(parent)
    {
        setFixedWidth(70);

        m_layout = new QVBoxLayout();
        m_layout->setSpacing(0);
        m_layout->setContentsMargins(0,0,0,0);

        setFixedWidth(80);
        QFont f;
        f.setBold(true);
        f.setPointSize(10);

        m_status1 = new QLabel("");
        m_status1->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_status1->setFont(f);

        QFont f2;
        f2.setBold(true);
        f2.setPointSize(12);

        m_status2 = new QLabel("");
        m_status2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_status2->setFont(f2);


        m_layout->addWidget(m_status1);
        m_layout->addWidget(m_status2);
        setLayout(m_layout);

    }

    StatusPanel::~StatusPanel()
    {
        delete m_layout;
    }

    void StatusPanel::update(ScheduleItem* schedule_item)
    {
        m_status1->setText(QString::fromStdString(schedule_item->item_status_text()));
        m_status2->setText(QString::fromStdString(schedule_item->play_channel()));

        setStyleSheet("");

        if (schedule_item->item_status() == OATS::ItemStatus::CUED){
            setStyleSheet("background-color: rgb(255, 0, 0)");
        }

        if (schedule_item->item_status() == OATS::ItemStatus::PLAYING){
            setStyleSheet("background-color: rgb(78, 207, 42)");
        }


    }

    /* -------------- TransitionModePanel -------------- */

    TransitionModePanel::TransitionModePanel(QWidget* parent)
        :Panel(parent)
        ,m_layout{nullptr}
        ,m_transition_label{nullptr}
    {
        setFixedWidth(60);

        m_layout = new QVBoxLayout();
        m_layout->setSpacing(0);
        m_layout->setContentsMargins(0,0,0,0);

        QFont f;
        f.setBold(true);
        f.setPointSize(10);

        m_transition_label = new QLabel("MIX");
        m_transition_label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_transition_label->setFont(f);

        m_layout->addWidget(m_transition_label);
        setLayout(m_layout);
    }

    TransitionModePanel::~TransitionModePanel()
    {
        delete m_layout;
    }

    void TransitionModePanel::update(ScheduleItem* schedule_item)
    {
    }


    /* -------------- ScheduleGridItem -------------- */

    int ScheduleGridItem::g_index{0};

    ScheduleGridItem::ScheduleGridItem(OATS::ScheduleItem* sched_item)
        :m_schedule_item{sched_item}
        ,m_item_status{ScheduleGridItem::ItemStatus::WAITING}
    {
        m_schedule_item->attach(this);

        m_grid_index = g_index++;

        //setStyleSheet(style);

        m_time_panel = std::make_unique<TimePanel>(this);
        m_track_panel = std::make_unique<TrackPanel>(this);
        m_status_panel = std::make_unique<StatusPanel>(this);
        m_transition_mode_panel = std::make_unique<TransitionModePanel>(this);

        m_layout = new QHBoxLayout();
        m_layout->setSpacing(0);
        m_layout->setContentsMargins(0,0,0,0);

        m_layout->addWidget(m_time_panel.get());
        m_layout->addWidget(m_track_panel.get());
        m_layout->addWidget(m_status_panel.get());
        m_layout->addWidget(m_transition_mode_panel.get());

        m_layout->insertStretch(-1,1);

        setLayout(m_layout);
    }

    ScheduleGridItem::~ScheduleGridItem()
    {
        delete m_layout;
    }

    int ScheduleGridItem::grid_index()
    {
        return m_grid_index;
    }

    int ScheduleGridItem::subject_index()
    {
        return m_schedule_item->index();
    }

    int ScheduleGridItem::subject_ref()
    {
        return m_schedule_item->schedule_ref();
    }

    void ScheduleGridItem::set_subject(Subject* subject)
    {
        update(subject);
    }

    void ScheduleGridItem::refresh()
    {
        update(m_schedule_item);
    }

    void ScheduleGridItem::set_item_index(int idx)
    {
        m_grid_index = idx;
    }

    ScheduleItem* ScheduleGridItem::schedule_item()
    {
        return m_schedule_item;
    }

    void ScheduleGridItem::update(Subject* changed_subject)
    {
        auto si = dynamic_cast<ScheduleItem*>(changed_subject);

        m_time_panel->update(si);
        m_track_panel->update(si);
        m_status_panel->update(si);
        m_transition_mode_panel->update(si);

        m_schedule_item = si;
    }

    std::string ScheduleGridItem::format_message(Message message)
    {
        return "";
    }

    std::string ScheduleGridItem::name()
    {
        return "";
    }

    ScheduleGridItem::ItemStatus ScheduleGridItem::item_status()
    {
        return m_item_status;
    }

    void ScheduleGridItem::set_item_status(OATS::ItemStatus i_status)
    {
//        m_item_status = i_status;
        m_schedule_item->set_item_status(i_status);
    }

    /* ------------------------------------------ */

    OutputPanel::OutputPanel(QString name)
        :m_panel_name{name}
        ,m_status{OATS::PanelStatus::WAITING}
    {
        setFrameShape(QFrame::Panel);
        setFrameShadow(QFrame::Raised);

        m_play_button = new QPushButton("PLAY "+name);
        connect(m_play_button, &QPushButton::clicked, this, &OutputPanel::play);
        m_play_button->setStyleSheet("background-color: rgb(78, 207, 42)");

        m_stop_button = new QPushButton("STOP "+name);
        connect(m_stop_button, &QPushButton::clicked, this, &OutputPanel::stop);

        m_status_image = new QLabel("img");
        m_layout_buttons = new QHBoxLayout();

        m_layout_buttons->addWidget(m_play_button);
        m_layout_buttons->addWidget(m_stop_button);
        m_layout_buttons->addWidget(m_status_image);

        QFont f_title;
        f_title.setBold(true);
        f_title.setPointSize(12);

        m_title = new QLabel("Title "+name);
        m_title->setFont(f_title);
        m_title->setAlignment(Qt::AlignHCenter);
        m_layout_title = new QHBoxLayout();
        m_layout_title->addWidget(m_title);

        QFont f_artist;
        f_artist.setBold(true);
        f_artist.setPointSize(10);

        m_artist = new QLabel("Artist "+name);
        m_artist->setFont(f_artist);
        m_artist->setAlignment(Qt::AlignHCenter);
        m_layout_artist = new QHBoxLayout();
        m_layout_artist->addWidget(m_artist);

        m_time = new QLabel("00:00 / 00:00 / 00:00");
        m_time->setAlignment(Qt::AlignHCenter);
        m_layout_time = new QHBoxLayout();
        m_layout_time->addWidget(m_time);

        m_main_panel = new QVBoxLayout();
        m_main_panel->addLayout(m_layout_buttons);
        m_main_panel->addLayout(m_layout_title);
        m_main_panel->addLayout(m_layout_artist);
        m_main_panel->addLayout(m_layout_time);

        setLayout(m_main_panel);

    }

    OutputPanel::~OutputPanel()
    {
        delete m_title;
        delete m_artist;
        delete m_time;

        delete m_play_button;
        delete m_stop_button;
        delete m_status_image;

        delete m_layout_buttons;
        delete m_layout_title;
        delete m_layout_artist;
        delete m_layout_time;

        delete m_main_panel;
    }

    QLabel* OutputPanel::title()
    {
        return m_title;
    }

    QLabel* OutputPanel::artist()
    {
        return m_artist;
    }

    QLabel* OutputPanel::time()
    {
        return m_time;
    }

    OATS::PanelStatus OutputPanel::panel_status()
    {
        return m_status;
    }

    void OutputPanel::set_panel_status(OATS::PanelStatus p_status)
    {
        m_status = p_status;
    }

    QString OutputPanel::panel_status_text()
    {
        QString text{""};

        switch(m_status)
        {
         case OATS::PanelStatus::WAITING:
            text = "WAITING";
            break;
         case OATS::PanelStatus::CUED:
            text = "CUED";
            break;
         case OATS::PanelStatus::PLAYING:
            text = "PLAYING";
            break;
         case OATS::PanelStatus::STOP:
            text = "STOP";
            break;
         default:
            text = "NONE";
            break;
        }

        return text;
    }

    void OutputPanel::cue_item(ScheduleItem* s_item)
    {
        m_title->setText(QString::fromStdString(s_item->audio().title()));
        m_artist->setText(QString::fromStdString(s_item->audio().artist()));
        m_schedule_item = s_item;
    }

    void OutputPanel::play()
    {
        // play cued item
        //emit play_item(m_id, m_schedule_item);
        emit play_item(this);
        m_status = OATS::PanelStatus::PLAYING;
        set_panel_style();
    }

    void OutputPanel::stop()
    {
        // stop audio play
        //emit stop_play(m_id, m_schedule_item);
        emit stop_play(this);
        m_status = OATS::PanelStatus::STOP;
        set_panel_style();
    }

    int OutputPanel::id() const
    {
        return m_id;
    }

    void OutputPanel::set_id(int i)
    {
        m_id = i;
    }

    void OutputPanel::set_panel_style()
    {
        setStyleSheet("");

        if (m_status == OATS::PanelStatus::PLAYING){
            setStyleSheet("background-color: rgb(78, 207, 42)");
        }
    }

    QTime OutputPanel::play_start_time()
    {
        return m_play_start_time;
    }

    void OutputPanel::set_play_start_time(QTime time)
    {
        m_play_start_time = time;
    }

    long OutputPanel::time_remaining()
    {
        return m_time_remaining;
    }
    void OutputPanel::set_time_remaining(long time)
    {
        m_time_remaining = time;
    }

    long OutputPanel::start_tick_stamp()
    {
        return m_start_tick_stamp;
    }

    void OutputPanel::set_start_tick_stamp(long stamp)
    {
        m_start_tick_stamp = stamp;
    }

    long OutputPanel::start_trigger_tick_stamp()
    {
        return m_start_trigger_tick_stamp;
    }
    void OutputPanel::set_start_trigger_tick_stamp(long time)
    {
        m_start_trigger_tick_stamp = time;
    }

    long OutputPanel::fade_trigger_tick_stamp()
    {
        return m_fade_trigger_tick_stamp;
    }

    void OutputPanel::set_fade_trigger_tick_stamp(long time)
    {
        m_fade_trigger_tick_stamp = time;
    }

    OATS::ScheduleItem* OutputPanel::schedule_item()
    {
        return m_schedule_item;
    }

    void OutputPanel::slow_flash_play_button()
    {
        if (!m_slow_flash_bit){
            m_play_button->setStyleSheet("background-color: rgb(255, 170, 255)");
            m_slow_flash_bit = true;
        }else{
            m_play_button->setStyleSheet("");
            m_slow_flash_bit = false;
        }
    }

    void OutputPanel::slow_flash_stop_button()
    {
        if (!m_stop_slow_flash_bit){
            m_stop_button->setStyleSheet("background-color: rgb(255, 170, 255)");
            m_stop_slow_flash_bit = true;
        } else {
            m_stop_button->setStyleSheet("");
            m_stop_slow_flash_bit = false;
        }
    }

    void OutputPanel::fast_flash_play_button(OATS::ButtonFlashColor bfc)
    {
        QString flash_color{""};

        switch(bfc)
        {
        case OATS::ButtonFlashColor::NONE:
            break;
        case OATS::ButtonFlashColor::GREEN:
            flash_color = "background-color: rgb(170, 255, 0)";
            break;
        case OATS::ButtonFlashColor::ORANGE:
            flash_color =  "background-color: rgb(255, 195, 0)";
            break;
        case OATS::ButtonFlashColor::RED:
            flash_color =  "background-color: rgb(210, 4, 45)";
            break;
        default:
            flash_color = "";
        }

        if (!m_fast_flash_on){
            m_play_button->setStyleSheet(flash_color);
            m_fast_flash_on = true;
        }else {
            m_play_button->setStyleSheet("");
            m_fast_flash_on = false;
        }
    }

    void OutputPanel::fast_flash_stop_button()
    {
        if (!m_stop_fast_flash_bit){
            m_stop_button->setStyleSheet(color_red);
            m_stop_fast_flash_bit = true;
        } else {
            m_stop_button->setStyleSheet("");
            m_stop_fast_flash_bit = false;
        }
    }

    TimeStamp OutputPanel::time_remaining_ts()
    {
        return m_ts;
    }

    /* --- TimeStamp  --- */

    long TimeStamp::time_remaining_ticks(OutputPanel* op)
    {
        return op->m_time_remaining;
    }


}
