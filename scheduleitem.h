#pragma once

#include <memory>

class QWidget;
class QBoxLayout;
class QFrame;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;

namespace OATS{

    class Panel : public QFrame
    {
    public:
        Panel();
        virtual ~Panel();
        QVBoxLayout* main_layout();
    private:
        QVBoxLayout* m_layout;

    };

    class TimePanel: public Panel
    {
        public:
            TimePanel();
            ~TimePanel() override;
            void set_label(QString);
        private:
            QLabel* m_label;
    };

    class TrackPanel : public Panel
    {
    public:
        TrackPanel();
        ~TrackPanel() override;
    };

    class StatusPanel : public Panel
    {
    public:
        StatusPanel();
        ~StatusPanel() override;
    };

    class PlayModePanel : public Panel
    {
    public:
        PlayModePanel();
        ~PlayModePanel() override;
    };


    class ScheduleItem : public QWidget
    {
    public:
        ScheduleItem(QString);
        ~ScheduleItem();

        QHBoxLayout* main_layout();
    private:
        QHBoxLayout* m_layout;
        std::unique_ptr<TimePanel> m_time_panel;
        std::unique_ptr<TrackPanel> m_track_panel;
        std::unique_ptr<StatusPanel> m_status_panel;
        std::unique_ptr<PlayModePanel> m_play_mode_panel;
    };
}

