#include <QDebug>
#include <QScrollBar>
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

    ui->hlMain->setContentsMargins(80,10,0,0);
    ui->vlSchedule->setContentsMargins(10,10,0,0);

    connect(ui->gridScroll, &QScrollBar::valueChanged, this, &MainWindow::scroll_changed);
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
    audio.set_title("Let It Go");
    audio.set_artist("Frozen");
    audio.set_duration(3200);

    auto sched_item = std::make_unique<OATS::ScheduleItem>();
    sched_item->set_schedule_ref(s_sched_ref++);
    sched_item->set_play_status("");
    sched_item->set_play_channel(play_channel());
    sched_item->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item->set_hour(hr);
    sched_item->set_time("15:38:09");
    sched_item->set_audio(audio);
    m_schedule_items.push_back(std::move(sched_item));

    OATS::Audio audio1;
    audio1.set_title("Waiting on the World to Change");
    audio1.set_artist("John Mayer");
    audio1.set_duration(3000);
    auto sched_item1 = std::make_unique<OATS::ScheduleItem>();
    sched_item1->set_schedule_ref(s_sched_ref++);
    sched_item1->set_play_status("");
    sched_item1->set_play_channel("B");
    sched_item1->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item1->set_hour(hr);
    sched_item1->set_time("15:40:02");
    sched_item1->set_audio(audio1);
    m_schedule_items.push_back(std::move(sched_item1));

    OATS::Audio audio2;
    audio2.set_title("Tuonela");
    audio2.set_artist("Therion");
    audio2.set_duration(3500);
    auto sched_item2 = std::make_unique<OATS::ScheduleItem>();
    sched_item2->set_schedule_ref(s_sched_ref++);
    sched_item2->set_play_status("");
    sched_item2->set_play_channel("A");
    sched_item2->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item2->set_hour(hr);
    sched_item2->set_time("15:55:02");
    sched_item2->set_audio(audio2);
    m_schedule_items.push_back(std::move(sched_item2));

    OATS::Audio audio3;
    audio3.set_title("Ever Dream");
    audio3.set_artist("Nightwish");
    audio3.set_duration(4000);
    auto sched_item3 = std::make_unique<OATS::ScheduleItem>();
    sched_item3->set_schedule_ref(s_sched_ref++);
    sched_item3->set_play_status("");
    sched_item3->set_play_channel("B");
    sched_item3->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item3->set_hour(hr);
    sched_item3->set_time("16:05:02");
    sched_item3->set_audio(audio3);
    m_schedule_items.push_back(std::move(sched_item3));

    OATS::Audio audio4;
    audio4.set_title("The 13th Floor");
    audio4.set_artist("Sirenia");
    audio4.set_duration(2800);
    auto sched_item4 = std::make_unique<OATS::ScheduleItem>();
    sched_item4->set_schedule_ref(s_sched_ref++);
    sched_item4->set_play_status("");
    sched_item4->set_play_channel("A");
    sched_item4->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item4->set_hour(hr);
    sched_item4->set_time("16:15:02");
    sched_item4->set_audio(audio4);
    m_schedule_items.push_back(std::move(sched_item4));

    OATS::Audio audio5;
    audio5.set_title("Christina");
    audio5.set_artist("Theatres Des Vampires");
    audio5.set_duration(3200);
    auto sched_item5 = std::make_unique<OATS::ScheduleItem>();
    sched_item5->set_schedule_ref(s_sched_ref++);
    sched_item5->set_play_status("");
    sched_item5->set_play_channel("B");
    sched_item5->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item5->set_hour(hr);
    sched_item5->set_time("16:15:02");
    sched_item5->set_audio(audio5);
    m_schedule_items.push_back(std::move(sched_item5));

    OATS::Audio audio6;
    audio6.set_title("No Way Out");
    audio6.set_artist("Dust in Mind");
    audio6.set_duration(2900);
    auto sched_item6 = std::make_unique<OATS::ScheduleItem>();
    sched_item6->set_schedule_ref(s_sched_ref++);
    sched_item6->set_play_status("");
    sched_item6->set_play_channel("A");
    sched_item6->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item6->set_hour(hr);
    sched_item6->set_time("16:18:02");
    sched_item6->set_audio(audio6);
    m_schedule_items.push_back(std::move(sched_item6));

    OATS::Audio audio7;
    audio7.set_title("Lying From You");
    audio7.set_artist("Linkin Park");
    audio7.set_duration(2955);
    auto sched_item7 = std::make_unique<OATS::ScheduleItem>();
    sched_item7->set_schedule_ref(s_sched_ref++);
    sched_item7->set_play_status("");
    sched_item7->set_play_channel("B");
    sched_item7->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item7->set_hour(hr);
    sched_item7->set_time("16:21:02");
    sched_item7->set_audio(audio7);
    m_schedule_items.push_back(std::move(sched_item7));

    OATS::Audio audio8;
    audio8.set_title("Float On");
    audio8.set_artist("Modest Mouse");
    audio8.set_duration(3100);
    auto sched_item8 = std::make_unique<OATS::ScheduleItem>();
    sched_item8->set_schedule_ref(s_sched_ref++);
    sched_item8->set_play_status("");
    sched_item8->set_play_channel("A");
    sched_item8->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item8->set_hour(hr);
    sched_item8->set_time("16:26:00");
    sched_item8->set_audio(audio8);
    m_schedule_items.push_back(std::move(sched_item8));

    OATS::Audio audio9;
    audio9.set_title("Maniac - From Flashdance");
    audio9.set_artist("Micheal Sembello");
    audio9.set_duration(29000);
    auto sched_item9 = std::make_unique<OATS::ScheduleItem>();
    sched_item9->set_schedule_ref(s_sched_ref++);
    sched_item9->set_play_status("");
    sched_item9->set_play_channel("B");
    sched_item9->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item9->set_hour(hr);
    sched_item9->set_time("16:30:02");
    sched_item9->set_audio(audio9);
    m_schedule_items.push_back(std::move(sched_item9));

    OATS::Audio audio10;
    audio10.set_title("Sugar, We're Going Down");
    audio10.set_artist("Fall Out Boy");
    audio10.set_duration(3400);
    auto sched_item10 = std::make_unique<OATS::ScheduleItem>();
    sched_item10->set_schedule_ref(s_sched_ref++);
    sched_item10->set_play_status("");
    sched_item10->set_play_channel("A");
    sched_item10->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item10->set_hour(hr);
    sched_item10->set_time("16:32:02");
    sched_item10->set_audio(audio10);
    m_schedule_items.push_back(std::move(sched_item10));

}

void MainWindow::make_schedule_grid()
{
    ui->vlSchedule->setSpacing(1);
    ui->vlSchedule->setContentsMargins(0,0,0,2);

    for (int i=0; i < MAX_GRID_ITEMS; ++i){
        auto subject = std::make_unique<OATS::ScheduleItem>();
        std::unique_ptr<OATS::ScheduleGridItem> grid_item = std::make_unique<OATS::ScheduleGridItem>(subject.get());
        connect(grid_item.get(), &OATS::ScheduleGridItem::move_up, this, &MainWindow::audio_move_up);
        connect(grid_item.get(), &OATS::ScheduleGridItem::move_down, this, &MainWindow::audio_move_down);
        connect(grid_item.get(), &OATS::ScheduleGridItem::delete_item, this, &MainWindow::delete_schedule_item);

        ui->vlSchedule->addWidget(grid_item.get());
        m_schedule_grid.push_back(std::move(grid_item));
        m_grid_subjects.push_back(std::move(subject));
    }

    ui->vlSchedule->insertStretch(-1,1);
}

void MainWindow::audio_move_up(int pos)
{
    if (pos-1 < 0)
        return;

    std::iter_swap(m_schedule_items.begin()+pos, m_schedule_items.begin()+(pos-1));

    auto current_subject = subject_item(pos);
    auto subject_above_current = subject_item(pos-1);

    current_subject->notify(schedule_item(pos));
    subject_above_current->notify(schedule_item(pos-1));

}

void MainWindow::audio_move_down(int pos)
{
    if (pos+1 > MAX_GRID_ITEMS-1)
        return;

    std::iter_swap(m_schedule_items.begin()+pos, m_schedule_items.begin()+(pos+1));
    auto current_subject = subject_item(pos);
    auto subject_below_current = subject_item(pos+1);

    current_subject->notify(schedule_item(pos));
    subject_below_current->notify(schedule_item(pos+1));

}

void MainWindow::delete_schedule_item(int pos)
{
    m_schedule_items.erase(m_schedule_items.begin()+pos);
    display_schedule(1);
}

void MainWindow::display_schedule(int start_index)
{
    for(int i=0; i < MAX_GRID_ITEMS; ++i) {

        int schedule_index = (i-1)+start_index;
        if (schedule_index >= m_schedule_items.size())
            break;

        auto schedule = schedule_item(schedule_index);

        if (schedule_index >= m_grid_subjects.size()){
            auto subject = std::make_unique<OATS::ScheduleItem>();
            m_grid_subjects.push_back(std::move(subject));
        }

        auto subject  = subject_item(i);

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

void MainWindow::scroll_changed(int new_pos)
{
    qDebug() << "New Pos: "<< new_pos;

    display_schedule(new_pos);
}

void MainWindow::print_schedule_items()
{
    for (auto const& item : m_schedule_items){
        qDebug() << item->schedule_ref() << ":" << QString::fromStdString(item->audio().title());
    }
}

void MainWindow::print_grid_items()
{
    for (auto const& item : m_schedule_grid){
        qDebug() << "Index: "<< item->index();
    }
}
