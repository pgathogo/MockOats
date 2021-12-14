#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>


#include "scheduleitem.h"


namespace OATS{

    Panel::Panel()
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

    QVBoxLayout* Panel::main_layout()
    {
        return m_layout;
    }

    TimePanel::TimePanel()
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

    void set_label(QString text)
    {
        //m_label->setText(text);
    }

    TrackPanel::TrackPanel()
    {
        setFixedWidth(385);
    }

    TrackPanel::~TrackPanel()
    {
    }

    StatusPanel::StatusPanel()
    {
        setFixedWidth(70);
    }

    StatusPanel::~StatusPanel()
    {
    }

    PlayModePanel::PlayModePanel()
    {
        setFixedWidth(60);
    }

    PlayModePanel::~PlayModePanel()
    {
    }

    ScheduleItem::ScheduleItem(QString style)
    {
        setStyleSheet(style);

        m_time_panel = std::make_unique<TimePanel>();
        m_track_panel = std::make_unique<TrackPanel>();
        m_status_panel = std::make_unique<StatusPanel>();
        m_play_mode_panel = std::make_unique<PlayModePanel>();

        m_layout = new QHBoxLayout();
        m_layout->addWidget(m_time_panel.get());
        m_layout->addWidget(m_track_panel.get());
        m_layout->addWidget(m_status_panel.get());
        m_layout->addWidget(m_play_mode_panel.get());

        m_layout->setSpacing(0);
        m_layout->setContentsMargins(0,0,0,0);
        m_layout->insertStretch(-1,1);

        setLayout(m_layout);
    }

    ScheduleItem::~ScheduleItem()
    {
    }

//    QHBoxLayout* ScheduleItem::main_layout()
//    {
//        return m_layout;
//    }

}
