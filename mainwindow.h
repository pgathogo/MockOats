#pragma once

#include <memory>

#include <QMainWindow>
#include "scheduleitem.h"
#include "schedulegriditem.h"

namespace OATS{
    class ScheduleItem;
    class OutputPanel;
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct CurrentPlayItem{
    OATS::ScheduleItem* item;
    int schedule_index{-1};
    int grid_index{-1};
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    constexpr static int MAX_GRID_ITEMS = 5;
    constexpr static int MAX_SCHEDULE_ITEMS = 10;

    constexpr static int YIELD_FADE_DELAY = 3000;
    constexpr static int YIELD_FADE_OUT = 7000;


    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void load_schedule(int);
    void make_schedule_grid();
    void display_schedule(int start_index=0);

    std::string play_channel();
    OATS::ScheduleItem* schedule_item(int);
    OATS::ScheduleItem* subject_item(int);
    int index_of(int);
    int grid_index_of(int);

    void print_schedule_items();
    void print_grid_items();
    std::unique_ptr<OATS::ScheduleItem> make_schedule_item();

    void refresh_schedule(int,int);
    void go_current();

    std::string minute_to_str(int);
    void fill_hour_strings();
    void make_str(int, int);
    OATS::ScheduleItem* find_next_schedule_item(OATS::ScheduleItem*);
    bool is_schedule_item(OATS::ScheduleItem*);

    void make_output_panels();
    void cue_next_schedule(OATS::ScheduleItem*, OATS::OutputPanel*);

    OATS::OutputPanel* create_output_panel(const QString);
    OATS::OutputPanel* find_output_panel(int);
    int next_output_panel_id(int);
    void compute_schedule_time();

    void calculate_trigger_times();
    int calculate_yield_contribution(OATS::ScheduleItem*);
    void set_current_play_item();
    long get_tick_count();

    void test_setup();
    void test_set_current_playing_item();

private slots:
    void item_move_up(int,int);
    void item_move_down(int,int);
    void delete_schedule_item(int);
    void insert_schedule_item(int, int);
    void make_item_current(int, int);
    void play_current_item(int, int);
//    void play_audio(int, OATS::ScheduleItem*);
//    void stop_audio(int, OATS::ScheduleItem*);
    void play_audio(OATS::OutputPanel*);
    void stop_audio(OATS::OutputPanel*);

    void scroll_changed(int);
    void show_time();
    void slow_flash();
    void fast_flash();
    void status_timer();

    void test();
    void test_slow_flash();
    void test_fast_flash();
    void test_main_timer();

    void test_change_outputA_status(int);
    void test_change_outputB_status(int);

    void test_set_triggerA();
    void test_set_triggerB();

    void test_set_fadeA();
    void test_set_fadeB();

    void count_down();

public slots:
    void test_set_RT_outputA();
    void test_set_RT_outputB();

private:
    Ui::MainWindow *ui;
    std::vector<std::unique_ptr<OATS::ScheduleItem>> m_grid_subjects;
    std::vector<std::unique_ptr<OATS::ScheduleGridItem>> m_schedule_grid;
    std::vector<std::unique_ptr<OATS::ScheduleItem>> m_schedule_items;

    OATS::OutputPanel* m_outputA;
    OATS::OutputPanel* m_outputB;
    std::unique_ptr<OATS::OutputPanel> m_outputC;


    std::vector<std::unique_ptr<OATS::OutputPanel>> m_output_panels;

    std::unique_ptr<QTimer> m_time_timer;
    std::map<int, std::string> m_hour_strings;

    std::unique_ptr<QTimer> m_slow_flash_timer;
    std::unique_ptr<QTimer> m_fast_flash_timer;
    std::unique_ptr<QTimer> m_countdown_timer;
    std::unique_ptr<QTimer> m_main_player_timer;

    static int s_sched_ref;
    static std::string s_channel;
    CurrentPlayItem m_current_playing_item;


};

struct FindByRef{
    FindByRef(int ref)
        :m_ref{ref}
    {
    }
    bool operator()(std::unique_ptr<OATS::ScheduleItem> const& item){
        return (item->schedule_ref() == m_ref);
    }
 private:
    int m_ref;
};

struct FindGridItemByRef{
    FindGridItemByRef(int ref)
        :m_ref{ref}
    {}
    bool operator()(std::unique_ptr<OATS::ScheduleGridItem>const& item){
        return (item->schedule_item()->schedule_ref()==m_ref);
    }
private:
    int m_ref;
};

