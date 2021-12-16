#include <QDebug>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "schedulegriditem.h"
#include "scheduleitem.h"

int MainWindow::s_sched_ref{0};
std::string MainWindow::s_channel{"A"};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    make_schedule_grid();
    load_schedule(8);
    display_schedule(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::string MainWindow::play_channel()
{
    return (s_channel == "A") ? "B" : "A";
}

void MainWindow::load_schedule(int hr)
{
    OATS::Audio audio;
    audio.set_artist("Frozen");
    audio.set_duration(3200);

    for(int i=0; i < MAX_SCHEDULE_ITEMS; ++i){
        auto sched_item = std::make_unique<OATS::ScheduleItem>();
        sched_item->set_schedule_ref(s_sched_ref++);
        sched_item->set_play_status("");
        sched_item->set_play_channel(play_channel());
        sched_item->set_schedule_type(OATS::ScheduleItem::SONG);
        sched_item->set_hour(hr);
        sched_item->set_time("15:38:09");
        sched_item->set_audio(audio);

        audio.set_title("Let It Go: "+std::to_string(sched_item->schedule_ref()));

        m_schedule_items.push_back(std::move(sched_item));
    }
}

void MainWindow::make_schedule_grid()
{
    ui->vlSchedule->setSpacing(1);
    ui->vlSchedule->setContentsMargins(0,0,0,2);

    for (int i=0; i < MAX_GRID_ITEMS; ++i){
        auto subject = std::make_unique<OATS::ScheduleItem>();
        std::unique_ptr<OATS::ScheduleGridItem> grid_item = std::make_unique<OATS::ScheduleGridItem>(subject.get());
        ui->vlSchedule->addWidget(grid_item.get());
        m_schedule_grid.push_back(std::move(grid_item));
        m_grid_subjects.push_back(std::move(subject));
    }

    ui->vlSchedule->insertStretch(-1,1);
}

void MainWindow::display_schedule(int start_index)
{
    for(int i=0; i < MAX_GRID_ITEMS; ++i) {
        int schedule_index = (i-1)+start_index;
        if (schedule_index >= m_schedule_items.size())
            break;

        auto schedule = schedule_item(schedule_index);
        auto subject  = subject_item(schedule_index);

        subject->notify(schedule);
    }
}

OATS::ScheduleItem* MainWindow::schedule_item(int index)
{
    return m_schedule_items[index].get();
}

OATS::ScheduleItem* MainWindow::subject_item(int index)
{
    return m_grid_subjects[index].get();
}
