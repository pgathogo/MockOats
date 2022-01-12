#pragma once

#include <QFrame>
#include <memory>
#include "subjectobserver.h"
#include "scheduleitem.h"

class QWidget;
class QGridLayout;
class QBoxLayout;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QContextMenuEvent;
class QPushButton;


namespace OATS{

    struct TimeFmt{
        long hr;
        long min;
        long sec;
        long msec;
    };

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

        QTime msec_to_time(int, int);
        TimeFmt dur_to_tfmt(long);

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

    class TransitionModePanel : public Panel
    {
    public:
        TransitionModePanel(QWidget*);
        ~TransitionModePanel() override;
        void update(ScheduleItem* schedule_item) override;
    private:
        QVBoxLayout* m_layout;
        QLabel* m_transition_label;
    };


    class ScheduleGridItem : public QWidget, public Observer
    {
        Q_OBJECT

    public:
        enum ItemStatus{WAITING, CUED, PLAYING, PLAYED, STOP};

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

        ItemStatus item_status();
        void set_item_status(OATS::ItemStatus);

    signals:
        void move_up(int, int);
        void move_down(int, int);
        void delete_item(int);
        void insert_item(int, int);
        void make_current(int, int);

    private:
        ItemStatus m_item_status;
        QHBoxLayout* m_layout;
        std::unique_ptr<TimePanel> m_time_panel;
        std::unique_ptr<TrackPanel> m_track_panel;
        std::unique_ptr<StatusPanel> m_status_panel;
        std::unique_ptr<TransitionModePanel> m_transition_mode_panel;

        ScheduleItem* m_schedule_item;

        int m_grid_index;
        int m_subject_index;

        static int g_index;
    };

    class OutputPanel;

    class TimeStamp{
    public:
        long time_remaining_ticks(OutputPanel*);
    };

    enum class PanelStatus{WAITING, CUED, PLAYING, STOP};
    enum class ButtonFlashColor{NONE, GREEN, ORANGE, RED};

    class OutputPanel : public QFrame
    {
        Q_OBJECT
    public:
        const QString color_red =  "background-color: rgb(210, 4, 45)";

        friend class TimeStamp;

        OutputPanel(QString name);
        ~OutputPanel();

        int id() const;
        void set_id(int);

        QLabel* title();
        QLabel* artist();
        QLabel* time();

        void play();
        void stop();
        void cue_item(ScheduleItem*);

        PanelStatus panel_status();
        void set_panel_status(PanelStatus);
        QString panel_status_text();

        void set_panel_style();

        QTime play_start_time();
        void set_play_start_time(QTime);

        long time_remaining();
        void set_time_remaining(long);
        long start_tick_stamp();
        void set_start_tick_stamp(long);
        long start_trigger_tick_stamp();
        void set_start_trigger_tick_stamp(long);
        long fade_trigger_tick_stamp();
        void set_fade_trigger_tick_stamp(long);

        void slow_flash_play_button();
        void fast_flash_play_button(ButtonFlashColor);

        void slow_flash_stop_button();
        void fast_flash_stop_button();

        OATS::ScheduleItem* schedule_item();

        TimeStamp time_remaining_ts();
    signals:
//        void play_item(int panel_id, ScheduleItem* si);
//        void stop_play(int panel_id, ScheduleItem* si);
        void play_item(OutputPanel*);
        void stop_play(OutputPanel*);

    private:
        int m_id;
        QString m_panel_name;
        QPushButton* m_play_button;
        QPushButton* m_stop_button;
        QLabel* m_status_image;
        QLabel* m_title;
        QLabel* m_artist;
        QLabel* m_time;

        QHBoxLayout* m_layout_buttons;
        QHBoxLayout* m_layout_title;
        QHBoxLayout* m_layout_artist;
        QHBoxLayout* m_layout_time;

        QVBoxLayout* m_main_panel;

        OATS::ScheduleItem* m_schedule_item;
        PanelStatus m_status;
        QTime m_play_start_time;
        long m_time_remaining{0};
        long m_start_tick_stamp{0};
        long m_start_trigger_tick_stamp{0};
        long m_fade_trigger_tick_stamp{0};

        bool m_slow_flash_bit{false};
        bool m_fast_flash_on{false};

        bool m_stop_slow_flash_bit{false};
        bool m_stop_fast_flash_bit{false};

        TimeStamp m_ts;
    };


}

