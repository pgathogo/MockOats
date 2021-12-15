#pragma once

#include <memory>

class QWidget;
class QBoxLayout;
class QFrame;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QContextMenuEvent;

namespace OATS{

    class Panel : public QFrame
    {
    public:
        Panel(QWidget*);
        virtual ~Panel();
        QVBoxLayout* main_layout();
        int index();
    private:
        QVBoxLayout* m_layout;
        QWidget* m_parent;
    };

    class TimePanel: public Panel
    {
        public:
            TimePanel(QWidget*);
            ~TimePanel() override;
            void set_label(QString);
        protected:
            void contextMenuEvent(QContextMenuEvent* event) override;
        private slots:
            void time_menu();
        private:
            QLabel* m_label;
    };

    class TrackPanel : public Panel
    {
    public:
        TrackPanel(QWidget*);
        ~TrackPanel() override;
    protected:
        void contextMenuEvent(QContextMenuEvent* event) override;
    private slots:
        void move_up();
        void move_down();
        void make_audio_current();
    };

    class StatusPanel : public Panel
    {
    public:
        StatusPanel(QWidget*);
        ~StatusPanel() override;
    };

    class PlayModePanel : public Panel
    {
    public:
        PlayModePanel(QWidget*);
        ~PlayModePanel() override;
    };


    class ScheduleGridItem : public QWidget
    {
    public:
        ScheduleGridItem(QString);
        ~ScheduleGridItem();

        QHBoxLayout* main_layout();
        int index();
    private:
        QHBoxLayout* m_layout;
        std::unique_ptr<TimePanel> m_time_panel;
        std::unique_ptr<TrackPanel> m_track_panel;
        std::unique_ptr<StatusPanel> m_status_panel;
        std::unique_ptr<PlayModePanel> m_play_mode_panel;

        int m_item_index;
        static int g_index;
    };
}

