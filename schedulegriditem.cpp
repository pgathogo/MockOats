#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
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
      setFrameShadow(QFrame::Raised);
      setFixedHeight(60);
      m_layout = new QVBoxLayout();
      m_layout->setSpacing(0);
      m_layout->setContentsMargins(0,0,0,0);
      setLayout(main_layout());
    }

    Panel::~Panel()
    {
    }

    int Panel::index()
    {
        return dynamic_cast<ScheduleGridItem*>(m_parent)->index();
    }

    QVBoxLayout* Panel::main_layout()
    {
        return m_layout;
    }

    /* -------------- Panel -------------- */

    TimePanel::TimePanel(QWidget* parent)
        :Panel(parent)
    {
        setFixedWidth(80);
        QFont f;
        f.setBold(true);
        f.setPointSize(10);

        m_label = new QLabel("00:00:00");
        m_label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_label->setFont(f);

        main_layout()->addWidget(m_label);
    }

    TimePanel::~TimePanel()
    {
    }

    void TimePanel::set_label(QString text)
    {
        //m_label->setText(text);
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

    /* -------------- TimePanel -------------- */

    TrackPanel::TrackPanel(QWidget* parent)
        :Panel(parent)
    {
        setFixedWidth(385);
    }


    TrackPanel::~TrackPanel()
    {
    }

    void TrackPanel::contextMenuEvent(QContextMenuEvent* event)
    {
        QMenu menu(this);
        auto act_move_up = new QAction("Move Up");
        auto act_move_down = new QAction("Move Down");
        auto act_make_current = new QAction("Make Current");

        connect(act_move_up, &QAction::triggered, this, TrackPanel::move_up);
        connect(act_move_down, &QAction::triggered, this, TrackPanel::move_down);
        connect(act_make_current, &QAction::triggered, this, TrackPanel::make_audio_current);

        menu.addAction(act_move_up);
        menu.addAction(act_move_down);
        menu.addAction(act_make_current);

        menu.exec(event->globalPos());

    }

    void TrackPanel::move_up()
    {
    }

    void TrackPanel::move_down()
    {
    }

    void TrackPanel::make_audio_current()
    {
    }

    void TrackPanel::update(ScheduleItem* schedule_item)
    {
    }

    /* -------------- StatusPanel -------------- */

    StatusPanel::StatusPanel(QWidget* parent)
        :Panel(parent)
    {
        setFixedWidth(70);
    }

    StatusPanel::~StatusPanel()
    {
    }

    void StatusPanel::update(ScheduleItem* schedule_item)
    {
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

        m_item_index = ++g_index;

        //setStyleSheet(style);

        m_time_panel = std::make_unique<TimePanel>(this);
        m_track_panel = std::make_unique<TrackPanel>(this);
        m_status_panel = std::make_unique<StatusPanel>(this);
        m_play_mode_panel = std::make_unique<PlayModePanel>(this);

        m_layout = new QHBoxLayout();
        m_layout->setSpacing(0);
        m_layout->setContentsMargins(5,0,0,0);

        m_layout->addWidget(m_time_panel.get());
        m_layout->addWidget(m_track_panel.get());
        m_layout->addWidget(m_status_panel.get());
        m_layout->addWidget(m_play_mode_panel.get());

        m_layout->insertStretch(-1,1);

        setLayout(m_layout);
    }

    ScheduleGridItem::~ScheduleGridItem()
    {
    }

    int ScheduleGridItem::index()
    {
        return m_item_index;
    }

    void ScheduleGridItem::update(Subject* changed_subject)
    {
        auto si = dynamic_cast<ScheduleItem*>(changed_subject);
        qDebug() << "Schedule Ref: "<< si->schedule_ref() << QString::fromStdString(si->audio().title());
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
