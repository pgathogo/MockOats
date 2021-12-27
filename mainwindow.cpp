#include <QDebug>
#include <QScrollBar>
#include <QTime>
#include <QTimer>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "schedulegriditem.h"
#include "scheduleitem.h"

int MainWindow::s_sched_ref{0};
std::string MainWindow::s_channel{"A"};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_current_play_item{nullptr}
{
    ui->setupUi(this);

    load_schedule(8);

    make_schedule_grid();

    ui->hlMain->setContentsMargins(80,10,0,0);
    ui->vlSchedule->setContentsMargins(10,10,0,0);

    connect(ui->gridScroll, &QScrollBar::valueChanged, this, &MainWindow::scroll_changed);
    connect(ui->btnCurrent, &QPushButton::clicked, this, &MainWindow::go_current);

    fill_hour_strings();

    auto curr_time = QTime::currentTime();
    make_str(curr_time.hour(), curr_time.minute());

    m_time_timer = std::make_unique<QTimer>(this);
    connect(m_time_timer.get(), &QTimer::timeout, this, &MainWindow::show_time);
    m_time_timer->start(100);
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
    sched_item->set_item_status(OATS::ScheduleItem::ItemStatus::WAITING);
    sched_item->set_play_channel(play_channel());
    sched_item->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item->set_hour(hr);
    sched_item->set_time("15:38:09");
    sched_item->set_audio(audio);
    sched_item->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item));

    OATS::Audio audio1;
    audio1.set_title("Waiting on the World to Change");
    audio1.set_artist("John Mayer");
    audio1.set_duration(3000);
    auto sched_item1 = std::make_unique<OATS::ScheduleItem>();
    sched_item1->set_schedule_ref(s_sched_ref++);
    sched_item1->set_item_status(OATS::ScheduleItem::ItemStatus::WAITING);
    sched_item1->set_play_channel("B");
    sched_item1->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item1->set_hour(hr);
    sched_item1->set_time("15:40:02");
    sched_item1->set_audio(audio1);
    sched_item1->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item1));

    OATS::Audio audio2;
    audio2.set_title("Tuonela");
    audio2.set_artist("Therion");
    audio2.set_duration(3500);
    auto sched_item2 = std::make_unique<OATS::ScheduleItem>();
    sched_item2->set_schedule_ref(s_sched_ref++);
    sched_item2->set_item_status(OATS::ScheduleItem::ItemStatus::WAITING);
    sched_item2->set_play_channel("A");
    sched_item2->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item2->set_hour(hr);
    sched_item2->set_time("15:55:02");
    sched_item2->set_audio(audio2);
    sched_item2->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item2));

    OATS::Audio audio3;
    audio3.set_title("Ever Dream");
    audio3.set_artist("Nightwish");
    audio3.set_duration(4000);
    auto sched_item3 = std::make_unique<OATS::ScheduleItem>();
    sched_item3->set_schedule_ref(s_sched_ref++);
    sched_item3->set_item_status(OATS::ScheduleItem::ItemStatus::WAITING);
    sched_item3->set_play_channel("B");
    sched_item3->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item3->set_hour(hr);
    sched_item3->set_time("16:05:02");
    sched_item3->set_audio(audio3);
    sched_item3->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item3));

    OATS::Audio audio4;
    audio4.set_title("The 13th Floor");
    audio4.set_artist("Sirenia");
    audio4.set_duration(2800);
    auto sched_item4 = std::make_unique<OATS::ScheduleItem>();
    sched_item4->set_schedule_ref(s_sched_ref++);
    sched_item4->set_item_status(OATS::ScheduleItem::ItemStatus::WAITING);
    sched_item4->set_play_channel("A");
    sched_item4->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item4->set_hour(hr);
    sched_item4->set_time("16:15:02");
    sched_item4->set_audio(audio4);
    sched_item4->set_index(m_schedule_items.size());
    sched_item4->set_schedule_type(OATS::ScheduleItem::SchedType::COMM);
    m_schedule_items.push_back(std::move(sched_item4));

    OATS::Audio audio5;
    audio5.set_title("Christina");
    audio5.set_artist("Theatres Des Vampires");
    audio5.set_duration(3200);
    auto sched_item5 = std::make_unique<OATS::ScheduleItem>();
    sched_item5->set_schedule_ref(s_sched_ref++);
    sched_item5->set_item_status(OATS::ScheduleItem::ItemStatus::WAITING);
    sched_item5->set_play_channel("B");
    sched_item5->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item5->set_hour(hr);
    sched_item5->set_time("16:15:02");
    sched_item5->set_audio(audio5);
    sched_item5->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item5));

    OATS::Audio audio6;
    audio6.set_title("No Way Out");
    audio6.set_artist("Dust in Mind");
    audio6.set_duration(2900);
    auto sched_item6 = std::make_unique<OATS::ScheduleItem>();
    sched_item6->set_schedule_ref(s_sched_ref++);
    sched_item6->set_item_status(OATS::ScheduleItem::ItemStatus::WAITING);
    sched_item6->set_play_channel("A");
    sched_item6->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item6->set_hour(hr);
    sched_item6->set_time("16:18:02");
    sched_item6->set_audio(audio6);
    sched_item6->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item6));

    OATS::Audio audio7;
    audio7.set_title("Lying From You");
    audio7.set_artist("Linkin Park");
    audio7.set_duration(2955);
    auto sched_item7 = std::make_unique<OATS::ScheduleItem>();
    sched_item7->set_schedule_ref(s_sched_ref++);
    sched_item7->set_item_status(OATS::ScheduleItem::ItemStatus::WAITING);
    sched_item7->set_play_channel("B");
    sched_item7->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item7->set_hour(hr);
    sched_item7->set_time("16:21:02");
    sched_item7->set_audio(audio7);
    sched_item7->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item7));

    OATS::Audio audio8;
    audio8.set_title("Float On");
    audio8.set_artist("Modest Mouse");
    audio8.set_duration(3100);
    auto sched_item8 = std::make_unique<OATS::ScheduleItem>();
    sched_item8->set_schedule_ref(s_sched_ref++);
    sched_item8->set_item_status(OATS::ScheduleItem::ItemStatus::WAITING);
    sched_item8->set_play_channel("A");
    sched_item8->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item8->set_hour(hr);
    sched_item8->set_time("16:26:00");
    sched_item8->set_audio(audio8);
    sched_item8->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item8));

    OATS::Audio audio9;
    audio9.set_title("Maniac - From Flashdance");
    audio9.set_artist("Micheal Sembello");
    audio9.set_duration(29000);
    auto sched_item9 = std::make_unique<OATS::ScheduleItem>();
    sched_item9->set_schedule_ref(s_sched_ref++);
    sched_item9->set_item_status(OATS::ScheduleItem::ItemStatus::WAITING);
    sched_item9->set_play_channel("B");
    sched_item9->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item9->set_hour(hr);
    sched_item9->set_time("16:30:02");
    sched_item9->set_audio(audio9);
    sched_item9->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item9));

    OATS::Audio audio10;
    audio10.set_title("Sugar, We're Going Down");
    audio10.set_artist("Fall Out Boy");
    audio10.set_duration(3400);
    auto sched_item10 = std::make_unique<OATS::ScheduleItem>();
    sched_item10->set_schedule_ref(s_sched_ref++);
    sched_item10->set_item_status(OATS::ScheduleItem::ItemStatus::WAITING);
    sched_item10->set_play_channel("A");
    sched_item10->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item10->set_hour(hr);
    sched_item10->set_time("16:32:02");
    sched_item10->set_audio(audio10);
    sched_item10->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item10));

    ui->gridScroll->setMaximum(m_schedule_items.size() - MAX_GRID_ITEMS );

}

void MainWindow::make_schedule_grid()
{
    ui->vlSchedule->setSpacing(1);
    ui->vlSchedule->setContentsMargins(0,0,0,2);

    std::string ch = "A";

    for (int i=0; i < MAX_GRID_ITEMS; ++i){

        auto si = schedule_item(i);
        si->set_play_channel(ch);

        std::unique_ptr<OATS::ScheduleGridItem> grid_item = std::make_unique<OATS::ScheduleGridItem>(m_schedule_items[i].get());
        m_schedule_items[i]->notify();

        connect(grid_item.get(), &OATS::ScheduleGridItem::move_up, this, &MainWindow::item_move_up);
        connect(grid_item.get(), &OATS::ScheduleGridItem::move_down, this, &MainWindow::item_move_down);
        connect(grid_item.get(), &OATS::ScheduleGridItem::delete_item, this, &MainWindow::delete_schedule_item);
        connect(grid_item.get(), &OATS::ScheduleGridItem::insert_item, this, &MainWindow::insert_schedule_item);
        connect(grid_item.get(), &OATS::ScheduleGridItem::make_current, this, &MainWindow::make_item_current);
        connect(grid_item.get(), &OATS::ScheduleGridItem::play_item, this, &MainWindow::play_current_item);

        ui->vlSchedule->addWidget(grid_item.get());
        m_schedule_grid.push_back(std::move(grid_item));

        if (ch == "A"){
            ch = "B";
        }else{
            ch = "A";
        }
    }

    ui->vlSchedule->insertStretch(-1,1);
}

void MainWindow::item_move_up(int schedule_ref, int grid_pos)
{
    int index = index_of(schedule_ref);

    std::iter_swap(m_schedule_items.begin()+index, m_schedule_items.begin()+(index-1));
    m_schedule_grid[grid_pos]->set_subject(schedule_item(index));
    m_schedule_grid[grid_pos-1]->set_subject(schedule_item(index-1));

}

void MainWindow::item_move_down(int schedule_pos, int grid_pos)
{
    if (grid_pos+1 > MAX_GRID_ITEMS-1)
        return;

    std::iter_swap(m_schedule_items.begin()+schedule_pos, m_schedule_items.begin()+(schedule_pos+1));
    m_schedule_grid[grid_pos]->set_subject(schedule_item(schedule_pos));
    m_schedule_grid[grid_pos+1]->set_subject(schedule_item(schedule_pos+1));
}

void MainWindow::delete_schedule_item(int pos)
{
    m_schedule_items.erase(m_schedule_items.begin()+pos);
    display_schedule();
}

void MainWindow::insert_schedule_item(int parent_index, int grid_index)
{
    auto item = make_schedule_item();
    m_schedule_grid[grid_index]->set_subject(item.get());
    m_schedule_items.insert(m_schedule_items.begin()+parent_index, std::move(item));
    refresh_schedule(grid_index, parent_index);
}

void MainWindow::make_item_current(int item_ref, int grid_index)
{
    int index = index_of(item_ref);
    auto si = schedule_item(index);
    si->set_item_status(OATS::ScheduleItem::ItemStatus::CUED);
    m_schedule_grid[grid_index]->refresh();

    for (int i=index+1; i < m_schedule_items.size()-1; ++i){
        si = schedule_item(i);
        si->set_item_status(OATS::ScheduleItem::ItemStatus::WAITING);
        int gi = ++grid_index;
        if (gi < MAX_GRID_ITEMS)
            m_schedule_grid[gi]->refresh();
    }
}

void MainWindow::play_current_item(int item_ref, int grid_index)
{
    int index = index_of(item_ref);
    auto si = schedule_item(index);

    m_current_play_item.current_item = si;
    m_current_play_item.schedule_index = index;
    m_current_play_item.grid_index = grid_index;

    si->set_item_status(OATS::ScheduleItem::ItemStatus::PLAYING);
    m_schedule_grid[grid_index]->refresh();
}

void MainWindow::refresh_schedule(int from_pos, int schedule_index)
{
    if (from_pos < 0)
        from_pos = 0;

    for (int i=from_pos; i < MAX_GRID_ITEMS; ++i){
        auto si = schedule_item(schedule_index++);
        m_schedule_grid[i]->set_subject(si);
        if (i > MAX_GRID_ITEMS-1)
            break;

    }
}

std::unique_ptr<OATS::ScheduleItem> MainWindow::make_schedule_item()
{
    OATS::Audio audio;
    audio.set_title("Billie Jean");
    audio.set_artist("Micheal Jackson");
    audio.set_duration(3280);
    auto sched_item = std::make_unique<OATS::ScheduleItem>();
    sched_item->set_schedule_ref(s_sched_ref++);
    sched_item->set_item_status(OATS::ScheduleItem::ItemStatus::WAITING);
    sched_item->set_play_channel(play_channel());
    sched_item->set_schedule_type(OATS::ScheduleItem::SONG);
    sched_item->set_hour(8);
    sched_item->set_time("17:12:42");
    sched_item->set_audio(audio);

    return std::move(sched_item);
}

void MainWindow::display_schedule(int start_index)
{
    for(int i=0; i < MAX_GRID_ITEMS; ++i) {

        int schedule_index = i+start_index;

        if (schedule_index >= m_schedule_items.size())
            break;

        auto schedule = schedule_item(schedule_index);

        m_schedule_grid[i]->set_subject(schedule);
    }
}


OATS::ScheduleItem* MainWindow::schedule_item(int index)
{
    return m_schedule_items[index].get();
}

OATS::ScheduleItem* MainWindow::find_next_schedule_item(OATS::ScheduleItem* current_item)
{
    OATS::ScheduleItem* si{nullptr};

    auto index = index_of(current_item->schedule_ref());
    for (int i=index+1; i < m_schedule_items.size()-1; ++i){
        si = schedule_item(index);
        if (is_schedule_item(si)){
            break;
        }
    }

    return si;
}

bool MainWindow::is_schedule_item(OATS::ScheduleItem* schedule_item)
{
    return (schedule_item->schedule_type() == OATS::ScheduleItem::SchedType::COMM ||
            schedule_item->schedule_type() == OATS::ScheduleItem::SchedType::SONG ||
            schedule_item->schedule_type() == OATS::ScheduleItem::SchedType::JINGLE ||
            schedule_item->schedule_type() == OATS::ScheduleItem::SchedType::DROP) ? true : false;
}

int MainWindow::index_of(int item_ref)
{
    auto it = std::find_if(m_schedule_items.begin(), m_schedule_items.end(), FindByRef(item_ref));
    if (it != m_schedule_items.end()){
        return std::distance(m_schedule_items.begin(), it);
    }else{
        return -1;
    }
}

OATS::ScheduleItem* MainWindow::subject_item(int index)
{
    return m_grid_subjects[index].get();
}

void MainWindow::scroll_changed(int new_pos)
{
    display_schedule(new_pos);
}

void MainWindow::go_current()
{
    if (m_current_play_item.schedule_index > -1 ){
        int idx = m_current_play_item.schedule_index;
        for (int i=0; i < MAX_GRID_ITEMS; ++i){
            if (i > MAX_GRID_ITEMS)
                break;
            auto si = schedule_item(idx++);
            m_schedule_grid[i]->set_subject(si);
        }
    }

    ui->gridScroll->setValue(m_current_play_item.grid_index);
}

void MainWindow::print_schedule_items()
{
    for (auto const& item : m_schedule_items){
        qDebug() << item->schedule_ref() << ":" <<item->index() << "=" <<QString::fromStdString(item->audio().title());
    }
}

void MainWindow::print_grid_items()
{
    for (auto const& item : m_schedule_grid){
        qDebug() << "Index: "<< item->grid_index();
    }
}


void MainWindow::show_time()
{
    auto curr_time = QTime::currentTime();
    ui->lblDigitalTime->setText(curr_time.toString("HH:mm:ss"));

    if (curr_time.second() < 2 ){
        make_str(curr_time.hour(), curr_time.minute());
    }

}

void MainWindow::make_str(int hours, int minutes)
{
    std::string str_link{""};
    std::string str_hours{""};
    std::string time_str{""};

    if (minutes > 30)
        ++hours;

    str_hours = m_hour_strings[hours];

    std::string str_minutes = minute_to_str(minutes);

    if (minutes > 30){
        str_link = " to ";
    } else if (minutes == 0) {
        str_link = " ";
    } else {
        str_link = " past ";
    }

    if (minutes == 0)
        str_hours = str_hours + " o ''clock";

    time_str = str_minutes + str_link + str_hours;

    ui->lblTextTime->setText(QString::fromStdString(time_str));

}

void MainWindow::fill_hour_strings()
{
    m_hour_strings[0] = "Twelve";
    m_hour_strings[1] = "One";
    m_hour_strings[2] = "Two";
    m_hour_strings[3] = "Three";
    m_hour_strings[4] = "Four";
    m_hour_strings[5] = "Five";
    m_hour_strings[6] = "Six";
    m_hour_strings[7] = "Seven";
    m_hour_strings[8] = "Eight";
    m_hour_strings[9] = "Nine";
    m_hour_strings[10] = "Ten";
    m_hour_strings[11] = "Eleven";
    m_hour_strings[12] = "Twelve";
    m_hour_strings[13] = "One";
    m_hour_strings[14] = "Two";
    m_hour_strings[15] = "Three";
    m_hour_strings[16] = "Four";
    m_hour_strings[17] = "Five";
    m_hour_strings[18] = "Six";
    m_hour_strings[19] = "Seven";
    m_hour_strings[20] = "Eight";
    m_hour_strings[21] = "Nine";
    m_hour_strings[22] = "Ten";
    m_hour_strings[23] = "Eleven";
    m_hour_strings[24] = "Twelve";
}

std::string MainWindow::minute_to_str(int minutes)
{
    if (minutes == 0) return "Exactly";
    if (minutes == 1 || minutes == 59) return "One minute";
    if (minutes == 2 || minutes == 58) return "Two minutes";
    if (minutes == 3 || minutes == 57) return "Three minutes";
    if (minutes == 4 || minutes == 56) return "Four minutes";
    if (minutes == 5 || minutes == 55) return "Five minutes";
    if (minutes == 6 || minutes == 54) return "Six minutes";
    if (minutes == 7 || minutes == 53) return "Seven minutes";
    if (minutes == 8 || minutes == 52) return "Eight minutes";
    if (minutes == 9 || minutes == 51) return "Nine minutes";
    if (minutes == 10 || minutes == 50) return "Ten minutes";
    if (minutes == 11 || minutes == 49) return "Eleven minutes";
    if (minutes == 12 || minutes == 48) return "Twelve minutes";
    if (minutes == 13 || minutes == 47) return "Thirteen minutes";
    if (minutes == 14 || minutes == 46) return "Fourteen minutes";
    if (minutes == 15 || minutes == 45) return "Quarter";
    if (minutes == 16 || minutes == 44) return "Sixteen minutes";
    if (minutes == 17 || minutes == 43) return "Seventeen minutes";
    if (minutes == 18 || minutes == 42) return "Eighteen minutes";
    if (minutes == 19 || minutes == 41) return "Nineteen minutes";
    if (minutes == 20 || minutes == 40) return "Twenty minutes";
    if (minutes == 21 || minutes == 39) return "Twenty-one minutes";
    if (minutes == 22 || minutes == 38) return "Twenty-two minutes";
    if (minutes == 23 || minutes == 37) return "Twenty-three minutes";
    if (minutes == 24 || minutes == 36) return "Twenty-four minutes";
    if (minutes == 25 || minutes == 35) return "Twenty-five minutes";
    if (minutes == 26 || minutes == 34) return "Twenty-six minutes";
    if (minutes == 27 || minutes == 33) return "Twenty-seven minutes";
    if (minutes == 28 || minutes == 32) return "Twenty-eight minutes";
    if (minutes == 29 || minutes == 31) return "Twenty-nine minutes";
    if (minutes == 30) return "Half";

    return "";

}
