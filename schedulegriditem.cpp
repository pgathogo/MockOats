#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFont>
#include <QMenu>
#include <QContextMenuEvent>
#include <QDebug>

#include "schedulegriditem.h"
#include "scheduleitem.h"

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


    /* -------------- Panel -------------- */

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
        set_label(QString::fromStdString(schedule_item->time()));
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
        auto act_play = new QAction("Play Item");

        connect(act_move_up, &QAction::triggered, this, TrackPanel::move_up);
        connect(act_move_down, &QAction::triggered, this, TrackPanel::move_down);
        connect(act_make_current, &QAction::triggered, this, TrackPanel::make_audio_current);
        connect(act_delete_item, &QAction::triggered, this, TrackPanel::delete_item);
        connect(act_play, &QAction::triggered, this, TrackPanel::play_item);

        menu.addAction(act_move_up);
        menu.addAction(act_move_down);
        menu.addAction(act_make_current);
        menu.addAction(act_delete_item);
        menu.addAction(act_play);

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

    void TrackPanel::play_item()
    {
        emit dynamic_cast<ScheduleGridItem*>(parent())->play_item(parent_ref(), index());
    }

    void TrackPanel::update(ScheduleItem* schedule_item)
    {
        m_track_label->setText(QString::fromStdString(schedule_item->audio().title()));
        m_artist_label->setText(QString::fromStdString(schedule_item->audio().artist()));

        setStyleSheet("");
        if (schedule_item->schedule_type() == ScheduleItem::SchedType::COMM){
            setStyleSheet("background-color: rgb(255, 255, 127)");
        }

        if (schedule_item->item_status() == ScheduleItem::ItemStatus::PLAYING){
            setStyleSheet("background-color: rgb(78, 207, 42)");
        }
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

        if (schedule_item->item_status() == ScheduleItem::ItemStatus::CUED){
            setStyleSheet("background-color: rgb(255, 0, 0)");
        }

        if (schedule_item->item_status() == ScheduleItem::ItemStatus::PLAYING){
            setStyleSheet("background-color: rgb(78, 207, 42)");
        }


    }

    /* -------------- PlayModePanel -------------- */

    PlayModePanel::PlayModePanel(QWidget* parent)
        :Panel(parent)
    {
        setFixedWidth(60);
    }

    PlayModePanel::~PlayModePanel()
    {
    }

    void PlayModePanel::update(ScheduleItem* schedule_item)
    {
    }


    /* -------------- ScheduleGridItem -------------- */

    int ScheduleGridItem::g_index{0};

    ScheduleGridItem::ScheduleGridItem(OATS::ScheduleItem* sched_item)
        :m_schedule_item{sched_item}
    {
        m_schedule_item->attach(this);

        m_grid_index = g_index++;

        //setStyleSheet(style);

        m_time_panel = std::make_unique<TimePanel>(this);
        m_track_panel = std::make_unique<TrackPanel>(this);
        m_status_panel = std::make_unique<StatusPanel>(this);
        m_play_mode_panel = std::make_unique<PlayModePanel>(this);

        m_layout = new QHBoxLayout();
        m_layout->setSpacing(0);
        m_layout->setContentsMargins(0,0,0,0);

        m_layout->addWidget(m_time_panel.get());
        m_layout->addWidget(m_track_panel.get());
        m_layout->addWidget(m_status_panel.get());
        m_layout->addWidget(m_play_mode_panel.get());

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

}
