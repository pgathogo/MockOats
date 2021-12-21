#pragma once

#include <memory>

#include <QMainWindow>

namespace OATS{
    class ScheduleItem;
    class ScheduleGridItem;
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    void display_schedule(int);

    std::string play_channel();
    OATS::ScheduleItem* schedule_item(int);
    OATS::ScheduleItem* subject_item(int);

    void print_schedule_items();
    void print_grid_items();

private slots:
    void audio_move_up(int);
    void audio_move_down(int);
    void delete_schedule_item(int);

private:
    Ui::MainWindow *ui;
    std::vector<std::unique_ptr<OATS::ScheduleItem>> m_grid_subjects;
    std::vector<std::unique_ptr<OATS::ScheduleGridItem>> m_schedule_grid;
    std::vector<std::unique_ptr<OATS::ScheduleItem>> m_schedule_items;

    static int s_sched_ref;
    static std::string s_channel;

private slots:
    void scroll_changed(int);

};
