#pragma once

#include <QFrame>
#include <memory>
#include "subjectobserver.h"

class QWidget;
class QGridLayout;
class QBoxLayout;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QContextMenuEvent;

namespace OATS{
    class ScheduleItem;
}

namespace OATS{

    class Panel : public QFrame
    {
    public:
        Panel(QWidget*);
        virtual ~Panel();
        virtual void update(ScheduleItem* schedule_item) = 0;
        int index();
        int parent_index();
        int parent_ref();

        QWidget* parent();
    private:
        QWidget* m_parent;
    };

    class TimePanel: public Panel
    {
        Q_OBJECT;
        public:
            TimePanel(QWidget*);
            ~TimePanel() override;
            void set_label(QString);
            void update(ScheduleItem* schedule_item) override;
            QVBoxLayout* main_layout();
        protected:
            void contextMenuEvent(QContextMenuEvent* event) override;
        private slots:
            void time_menu();
        private:
            QVBoxLayout* m_layout;
            QLabel* m_label;
    };

    class TrackPanel : public Panel
    {
        Q_OBJECT
    public:
        TrackPanel(QWidget*);
        ~TrackPanel() override;
        void update(ScheduleItem* schedule_item) override;
    protected:
        void contextMenuEvent(QContextMenuEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
    private slots:
        void move_up();
        void move_down();
        void make_audio_current();
        void delete_item();
        void play_item();
    private:
        QGridLayout* m_layout;
        QLabel* m_track_label;
        QLabel* m_artist_label;
        QLabel* m_duration_label;
    };

    class StatusPanel : public Panel
    {
    public:
        StatusPanel(QWidget*);
        ~StatusPanel() override;
        void update(ScheduleItem* schedule_item) override;
    private:
        QVBoxLayout* m_layout;
        QLabel* m_status1;
        QLabel* m_status2;

    };

    class PlayModePanel : public Panel
    {
    public:
        PlayModePanel(QWidget*);
        ~PlayModePanel() override;
        void update(ScheduleItem* schedule_item) override;
    };


    class ScheduleGridItem : public QWidget, public Observer
    {
        Q_OBJECT

    public:
        ScheduleGridItem(ScheduleItem*);
        ~ScheduleGridItem();

        QHBoxLayout* main_layout();
        int grid_index();
        int subject_index();
        void set_subject(Subject*);
        void update(Subject* changed_subject) override;
        std::string format_message(Message message) override;
        std::string name() override;

        void set_item_index(int);
        ScheduleItem* schedule_item();
        void refresh();
        int subject_ref();

    signals:
        void move_up(int, int);
        void move_down(int, int);
        void delete_item(int);
        void insert_item(int, int);
        void make_current(int, int);
        void play_item(int, int);

    private:
        QHBoxLayout* m_layout;
        std::unique_ptr<TimePanel> m_time_panel;
        std::unique_ptr<TrackPanel> m_track_panel;
        std::unique_ptr<StatusPanel> m_status_panel;
        std::unique_ptr<PlayModePanel> m_play_mode_panel;

        ScheduleItem* m_schedule_item;

        int m_grid_index;
        int m_subject_index;

        static int g_index;
    };
}

