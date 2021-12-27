#pragma once

#include <memory>

#include <QMainWindow>
#include "scheduleitem.h"

namespace OATS{
    class ScheduleItem;
    class ScheduleGridItem;
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct CurrentPlayItem{
    OATS::ScheduleItem* current_item;
    int schedule_index{-1};
    int grid_index{-1};
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    constexpr static int MAX_GRID_ITEMS = 5;
    constexpr static int MAX_SCHEDULE_ITEMS = 10;


    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void load_schedule(int);
    void make_schedule_grid();
    void display_schedule(int start_index=0);

    std::string play_channel();
    OATS::ScheduleItem* schedule_item(int);
    OATS::ScheduleItem* subject_item(int);
    int index_of(int);

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


private slots:
    void item_move_up(int,int);
    void item_move_down(int,int);
    void delete_schedule_item(int);
    void insert_schedule_item(int, int);
    void make_item_current(int, int);
    void play_current_item(int, int);

private:
    Ui::MainWindow *ui;
    std::vector<std::unique_ptr<OATS::ScheduleItem>> m_grid_subjects;
    std::vector<std::unique_ptr<OATS::ScheduleGridItem>> m_schedule_grid;
    std::vector<std::unique_ptr<OATS::ScheduleItem>> m_schedule_items;

    std::unique_ptr<QTimer> m_time_timer;
    std::map<int, std::string> m_hour_strings;

    static int s_sched_ref;
    static std::string s_channel;
    CurrentPlayItem m_current_play_item;

private slots:
    void scroll_changed(int);
    void show_time();

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

