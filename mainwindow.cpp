#include <math.h>
#include <chrono>
#include <QDebug>
#include <QScrollBar>
#include <QToolButton>
#include <QTime>
#include <QTimer>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "schedulegriditem.h"
#include "scheduleitem.h"

int MainWindow::s_sched_ref{0};
std::string MainWindow::s_channel{"A"};

QString stoq(const std::string s){
    return QString::fromStdString(s);
}

void print(QString s){
    qDebug() << " ** " << s << " ** ";
}

using namespace std::chrono_literals;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_current_playing_item{nullptr}
    , m_outputA{nullptr}
    , m_outputB{nullptr}
{
    ui->setupUi(this);

    load_schedule(8);

    make_schedule_grid();

    ui->hlMain->setContentsMargins(80,10,0,0);
    ui->vlSchedule->setContentsMargins(10,10,0,0);

    connect(ui->gridScroll, &QScrollBar::valueChanged, this, &MainWindow::scroll_changed);
    connect(ui->btnCurrent, &QPushButton::clicked, this, &MainWindow::go_current);
    connect(ui->btnTest, &QPushButton::clicked, this, &MainWindow::test);

    connect(ui->btnSlowFlash, &QPushButton::clicked, this, &MainWindow::test_slow_flash);
    connect(ui->btnFastFlash, &QPushButton::clicked, this, &MainWindow::test_fast_flash);
    connect(ui->btnMainTimer, &QPushButton::clicked, this, &MainWindow::test_main_timer);

    fill_hour_strings();

    make_output_panels();

    set_current_play_item();

    auto curr_time = QTime::currentTime();
    make_str(curr_time.hour(), curr_time.minute());

    compute_schedule_time();

    // Timer for the wall clock
    m_time_timer = std::make_unique<QTimer>(this);
    connect(m_time_timer.get(), &QTimer::timeout, this, &MainWindow::show_time);
    m_time_timer->start(100);

    // Slow Flash Timer
    auto slow_flash_interval = 400ms;
    m_slow_flash_timer = std::make_unique<QTimer>(this);
    connect(m_slow_flash_timer.get(), &QTimer::timeout, this, &MainWindow::slow_flash);
    //m_slow_flash_timer->start(slow_flash_interval.count());

    auto fast_flash_interval = 250ms;
    m_fast_flash_timer = std::make_unique<QTimer>(this);
    connect(m_fast_flash_timer.get(), &QTimer::timeout, this, &MainWindow::fast_flash);
    //m_fast_flash_timer->start(fast_flash_interval.count());

    auto countdown_interval = 50ms;
    m_countdown_timer = std::make_unique<QTimer>(this);
    connect(m_countdown_timer.get(), &QTimer::timeout, this, &MainWindow::count_down);

    // Main Player Timer
    auto main_timer_interval = 25ms;
    m_main_player_timer = std::make_unique<QTimer>(this);
    connect(m_main_player_timer.get(), &QTimer::timeout, this, &MainWindow::status_timer);
    //m_main_player_timer->start(main_timer_interval);

}

void MainWindow::set_current_play_item()
{

    auto s_item = schedule_item(0);
    m_current_playing_item.item = s_item;
    m_current_playing_item.schedule_index = s_item->index();
    m_current_playing_item.grid_index = 0;

    make_item_current(s_item->schedule_ref(), 0);
}

void MainWindow::compute_schedule_time()
{
    int min = m_current_playing_item.item->schedule_time().minute();
    int sec = m_current_playing_item.item->schedule_time().second();
    int msec = m_current_playing_item.item->schedule_time().msec();

    int time_counter = (min * 60000) + (sec * 1000) + msec;

    m_current_playing_item.item->set_current_time(time_counter);

    m_current_playing_item.item->notify();

    time_counter = time_counter + m_current_playing_item.item->audio().duration();

//	if (m_current_play_item.current_item->live_assist_transition == ScheduleItem::AssistTransition::TransMix)
//		time_counter = time_counter - m_current_play_item.current_item.audio().fade_delay();

    for (int i=m_current_playing_item.schedule_index+1; i < m_schedule_items.size()-1; ++i){
        auto si = schedule_item(i);
        si->set_current_time(time_counter);
        time_counter = time_counter + si->audio().duration();

//		if (si->live_assist_transition == ScheduleItem::TransitionAssist::TransMix){
//			time_counter = time_counter - si->audio().fade_delay();
//		}

        si->notify();
    }



    qDebug() << "Time Counter: "<< time_counter;
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
    audio.set_duration(15000); // 15 seconds
    audio.set_intro(3000);
    //audio.set_duration(150000); // 2.5 minutes

    auto sched_item = std::make_unique<OATS::ScheduleItem>();
    sched_item->set_schedule_ref(s_sched_ref++);
    sched_item->set_item_status(OATS::ItemStatus::WAITING);
    sched_item->set_play_channel(play_channel());
    sched_item->set_schedule_type(OATS::ScheduleType::SONG);
    sched_item->set_hour(hr);
    sched_item->set_schedule_time((QTime::fromString("08:08:09", "HH:mm:ss")));
    sched_item->set_audio(audio);
    sched_item->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item));

    OATS::Audio audio1;
    audio1.set_title("Waiting on the World to Change");
    audio1.set_artist("John Mayer");
    audio1.set_duration(15000);
    audio1.set_intro(5000);
    //audio1.set_duration(204000); // 3.4 minutes
    auto sched_item1 = std::make_unique<OATS::ScheduleItem>();
    sched_item1->set_schedule_ref(s_sched_ref++);
    sched_item1->set_item_status(OATS::ItemStatus::WAITING);
    sched_item1->set_play_channel("B");
    sched_item1->set_schedule_type(OATS::ScheduleType::SONG);
    sched_item1->set_hour(hr);
    sched_item1->set_schedule_time(QTime::fromString("08:12:02", "HH:mm:ss"));
    sched_item1->set_audio(audio1);
    sched_item1->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item1));

    OATS::Audio audio2;
    audio2.set_title("Tuonela");
    audio2.set_artist("Therion");
    //audio2.set_duration(192000); // 3.2
    audio2.set_duration(15000);
    audio2.set_intro(2000);
    auto sched_item2 = std::make_unique<OATS::ScheduleItem>();
    sched_item2->set_schedule_ref(s_sched_ref++);
    sched_item2->set_item_status(OATS::ItemStatus::WAITING);
    sched_item2->set_play_channel("A");
    sched_item2->set_schedule_type(OATS::ScheduleType::SONG);
    sched_item2->set_hour(hr);
    sched_item2->set_schedule_time(QTime::fromString("08:15:02", "HH:mm:ss"));
    sched_item2->set_audio(audio2);
    sched_item2->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item2));

    OATS::Audio audio3;
    audio3.set_title("Ever Dream");
    audio3.set_artist("Nightwish");
    audio3.set_duration(15000);
    audio3.set_intro(3000);
    //audio3.set_duration(120000); //2 minutes
    auto sched_item3 = std::make_unique<OATS::ScheduleItem>();
    sched_item3->set_schedule_ref(s_sched_ref++);
    sched_item3->set_item_status(OATS::ItemStatus::WAITING);
    sched_item3->set_play_channel("B");
    sched_item3->set_schedule_type(OATS::ScheduleType::SONG);
    sched_item3->set_hour(hr);
    sched_item3->set_schedule_time(QTime::fromString("08:17:05", "HH:mm:ss"));
    sched_item3->set_audio(audio3);
    sched_item3->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item3));

    OATS::Audio audio4;
    audio4.set_title("The 13th Floor");
    audio4.set_artist("Sirenia");
    audio4.set_duration(15000); //2.6
    //audio4.set_duration(156000); //2.6
    auto sched_item4 = std::make_unique<OATS::ScheduleItem>();
    sched_item4->set_schedule_ref(s_sched_ref++);
    sched_item4->set_item_status(OATS::ItemStatus::WAITING);
    sched_item4->set_play_channel("A");
    sched_item4->set_schedule_type(OATS::ScheduleType::SONG);
    sched_item4->set_hour(hr);
    sched_item4->set_schedule_time(QTime::fromString("08:21:11", "HH:mm:ss"));
    sched_item4->set_audio(audio4);
    sched_item4->set_index(m_schedule_items.size());
    sched_item4->set_schedule_type(OATS::ScheduleType::COMM);
    m_schedule_items.push_back(std::move(sched_item4));

    OATS::Audio audio5;
    audio5.set_title("Christina");
    audio5.set_artist("Theatres Des Vampires");
    //audio5.set_duration(204000); //3.4
    audio5.set_duration(15000); //3.4
    auto sched_item5 = std::make_unique<OATS::ScheduleItem>();
    sched_item5->set_schedule_ref(s_sched_ref++);
    sched_item5->set_item_status(OATS::ItemStatus::WAITING);
    sched_item5->set_play_channel("B");
    sched_item5->set_schedule_type(OATS::ScheduleType::SONG);
    sched_item5->set_hour(hr);
    sched_item5->set_schedule_time(QTime::fromString("08:25:32", "HH:mm:ss"));
    sched_item5->set_audio(audio5);
    sched_item5->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item5));

    OATS::Audio audio6;
    audio6.set_title("No Way Out");
    audio6.set_artist("Dust in Mind");
    //audio6.set_duration(186000); // 3.1
    audio6.set_duration(15000); // 3.1
    auto sched_item6 = std::make_unique<OATS::ScheduleItem>();
    sched_item6->set_schedule_ref(s_sched_ref++);
    sched_item6->set_item_status(OATS::ItemStatus::WAITING);
    sched_item6->set_play_channel("A");
    sched_item6->set_schedule_type(OATS::ScheduleType::SONG);
    sched_item6->set_hour(hr);
    sched_item6->set_schedule_time(QTime::fromString("08:27:37", "HH:mm:ss"));
    sched_item6->set_audio(audio6);
    sched_item6->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item6));

    OATS::Audio audio7;
    audio7.set_title("Lying From You");
    audio7.set_artist("Linkin Park");
    //audio7.set_duration(174000); // 2.9
    audio7.set_duration(15000); // 2.9
    auto sched_item7 = std::make_unique<OATS::ScheduleItem>();
    sched_item7->set_schedule_ref(s_sched_ref++);
    sched_item7->set_item_status(OATS::ItemStatus::WAITING);
    sched_item7->set_play_channel("B");
    sched_item7->set_schedule_type(OATS::ScheduleType::SONG);
    sched_item7->set_hour(hr);
    sched_item7->set_schedule_time(QTime::fromString("08:31:30", "HH:mm:ss"));
    sched_item7->set_audio(audio7);
    sched_item7->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item7));

    OATS::Audio audio8;
    audio8.set_title("Float On");
    audio8.set_artist("Modest Mouse");
    //audio8.set_duration(210000); //3.5
    audio8.set_duration(15000); //3.5
    auto sched_item8 = std::make_unique<OATS::ScheduleItem>();
    sched_item8->set_schedule_ref(s_sched_ref++);
    sched_item8->set_item_status(OATS::ItemStatus::WAITING);
    sched_item8->set_play_channel("A");
    sched_item8->set_schedule_type(OATS::ScheduleType::SONG);
    sched_item8->set_hour(hr);
    sched_item8->set_schedule_time(QTime::fromString("08:35:40", "HH:mm:ss"));
    sched_item8->set_audio(audio8);
    sched_item8->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item8));

    OATS::Audio audio9;
    audio9.set_title("Maniac - From Flashdance");
    audio9.set_artist("Micheal Sembello");
    //audio9.set_duration(180000); //3
    audio9.set_duration(15000); //3
    auto sched_item9 = std::make_unique<OATS::ScheduleItem>();
    sched_item9->set_schedule_ref(s_sched_ref++);
    sched_item9->set_item_status(OATS::ItemStatus::WAITING);
    sched_item9->set_play_channel("B");
    sched_item9->set_schedule_type(OATS::ScheduleType::SONG);
    sched_item9->set_hour(hr);
    sched_item9->set_schedule_time(QTime::fromString("08:38:22", "HH:mm:ss"));
    sched_item9->set_audio(audio9);
    sched_item9->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item9));

    OATS::Audio audio10;
    audio10.set_title("Sugar, We're Going Down");
    audio10.set_artist("Fall Out Boy");
    //audio10.set_duration(132000); //2.2
    audio10.set_duration(15000); //2.2
    auto sched_item10 = std::make_unique<OATS::ScheduleItem>();
    sched_item10->set_schedule_ref(s_sched_ref++);
    sched_item10->set_item_status(OATS::ItemStatus::WAITING);
    sched_item10->set_play_channel("A");
    sched_item10->set_schedule_type(OATS::ScheduleType::SONG);
    sched_item10->set_hour(hr);
    sched_item10->set_schedule_time(QTime::fromString("08:42:55", "HH:mm:ss"));
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

        ui->vlSchedule->addWidget(grid_item.get());
        m_schedule_grid.push_back(std::move(grid_item));

        ch = (ch == "A") ? "B" : "A";

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
    assert(m_outputA != nullptr);

    m_outputA->set_panel_status(OATS::PanelStatus::CUED);

    int index = index_of(item_ref);
    auto si = schedule_item(index);

    si->set_item_status(OATS::ItemStatus::CUED);

    if (si->play_channel() == "A"){
        m_outputA->cue_item(si);
    } else {
        m_outputB->cue_item(si);
    }

    for (int i=index+1; i < m_schedule_items.size()-1; ++i){
        auto sched_item = schedule_item(i);
        sched_item->set_item_status(OATS::ItemStatus::WAITING);
        sched_item->notify();
    }

    display_schedule(ui->gridScroll->value());

}

void MainWindow::play_current_item(int item_ref, int grid_index)
{
    int index = index_of(item_ref);
    auto si = schedule_item(index);

    m_current_playing_item.item = si;
    m_current_playing_item.schedule_index = index;
    m_current_playing_item.grid_index = grid_index;

    si->set_item_status(OATS::ItemStatus::PLAYING);
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
    sched_item->set_item_status(OATS::ItemStatus::WAITING);
    sched_item->set_play_channel(play_channel());
    sched_item->set_schedule_type(OATS::ScheduleType::SONG);
    sched_item->set_hour(8);
    sched_item->set_schedule_time(QTime(QTime::fromString("08:40:10", "HH:mm:ss")));
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
    if (index <= m_schedule_items.size()-1){
        return m_schedule_items[index].get();
    }else{
        return nullptr;
    }

}

OATS::ScheduleItem* MainWindow::find_next_schedule_item(OATS::ScheduleItem* current_item)
{
    OATS::ScheduleItem* si{nullptr};

    auto index = index_of(current_item->schedule_ref());
    auto next_item = schedule_item(index+1);
    return next_item;

    /*
    for (int i=index+1; i < m_schedule_items.size()-1; ++i){
        si = schedule_item(index);
        if (is_schedule_item(si)){
            break;
        }
    }
    */

}

bool MainWindow::is_schedule_item(OATS::ScheduleItem* schedule_item)
{
    return (schedule_item->schedule_type() == OATS::ScheduleType::COMM ||
            schedule_item->schedule_type() == OATS::ScheduleType::SONG ||
            schedule_item->schedule_type() == OATS::ScheduleType::JINGLE ||
            schedule_item->schedule_type() == OATS::ScheduleType::DROP) ? true : false;
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

int MainWindow::grid_index_of(int item_ref)
{
    auto it = std::find_if(m_schedule_grid.begin(), m_schedule_grid.end(), FindGridItemByRef(item_ref));
    if (it != m_schedule_grid.end()){
        return std::distance(m_schedule_grid.begin(), it);
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
    qDebug() << "Size: "<< m_schedule_grid.size();


    m_schedule_grid[0]->set_subject(m_current_playing_item.item);
    m_current_playing_item.item->notify();

    if (m_current_playing_item.schedule_index > -1 ){
        int idx = m_current_playing_item.schedule_index;
        for (int i=0; i < MAX_GRID_ITEMS; ++i){

            if (i > MAX_GRID_ITEMS)
                break;

            auto si = schedule_item(idx++);
            if (si == nullptr)
                break;

            m_schedule_grid[i]->set_subject(si);
        }
    }

    ui->gridScroll->setValue(m_current_playing_item.grid_index);
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

void MainWindow::slow_flash()
{
    auto TEN_SECONDS  = 10000ms;
    auto FIVE_SECONDS =  5000ms;

    long current_time = get_tick_count();

    // Output A

    if (m_outputA->panel_status() == OATS::PanelStatus::CUED ){
        auto tick_count_diff_OPA = m_outputA->start_trigger_tick_stamp() - current_time;
        if ( tick_count_diff_OPA < TEN_SECONDS.count() && tick_count_diff_OPA > FIVE_SECONDS.count() ) {
            m_outputA->slow_flash_play_button();
        }
    } else if (m_outputA->panel_status() == OATS::PanelStatus::PLAYING) {

        if ((m_outputA->time_remaining() < TEN_SECONDS.count()) &&
            (m_outputA->time_remaining() > FIVE_SECONDS.count()) ){
            m_outputA->slow_flash_stop_button();
        }
    }

    // Output B

    if (m_outputB->panel_status() == OATS::PanelStatus::CUED){
        auto tick_count_diff_OPB = m_outputB->start_trigger_tick_stamp() - current_time;
        if ( tick_count_diff_OPB < TEN_SECONDS.count() && tick_count_diff_OPB > FIVE_SECONDS.count() ){
            m_outputB->slow_flash_play_button();
        }
    } else if (m_outputB->panel_status() == OATS::PanelStatus::PLAYING){
        if ((m_outputB->time_remaining() < TEN_SECONDS.count()) &&
            (m_outputB->time_remaining() > FIVE_SECONDS.count()) ){
            m_outputB->slow_flash_stop_button();
        }
    }

}


void MainWindow::fast_flash()
{
    auto FIVE_SECONDS = 5000ms;

    long current_time = get_tick_count();

    // Output A
    if (m_outputA->panel_status() == OATS::PanelStatus::CUED) {
        auto tick_count_diff_OPA = m_outputA->start_trigger_tick_stamp() - current_time;
        if ( (tick_count_diff_OPA < FIVE_SECONDS.count()) && (tick_count_diff_OPA > 0) ) {
            m_outputA->fast_flash_play_button(OATS::ButtonFlashColor::GREEN);
        }
    } else {
        if (m_outputA->panel_status() == OATS::PanelStatus::PLAYING){
            if ( (m_outputA->time_remaining() < FIVE_SECONDS.count() ) &&
                 ( m_outputA->time_remaining() > 0 ) ) {
                m_outputA->fast_flash_stop_button();
            }
        }
    }

    // Output B
    if (m_outputB->panel_status() == OATS::PanelStatus::CUED){
        auto tick_count_diff_OPB = m_outputB->start_trigger_tick_stamp() - current_time;
        if ( (tick_count_diff_OPB < FIVE_SECONDS.count()) && (tick_count_diff_OPB > 0) ){
            m_outputB->fast_flash_play_button(OATS::ButtonFlashColor::GREEN);
        }
    } else {
        if (m_outputB->panel_status() == OATS::PanelStatus::PLAYING){
            if ((m_outputB->time_remaining() < FIVE_SECONDS.count()) &&
                (m_outputB->time_remaining() > 0) ){
                m_outputB->fast_flash_stop_button();
            }
        }
    }
}

void MainWindow::status_timer()
{
    int elapsed{0};
    int remaining{0};
    long trigger_ticker = get_tick_count();

    switch (m_outputA->panel_status())
    {
     case OATS::PanelStatus::WAITING:
        m_outputA->set_start_trigger_tick_stamp(-1);
        break;
     case OATS::PanelStatus::CUED:
        if ( (m_outputA->start_trigger_tick_stamp() > -1) &&
             (m_outputA->start_trigger_tick_stamp() <= (trigger_ticker+0)))
        {
            qDebug() << "status_timer::CUED=>OP_A";
            qDebug() << "play_audio(m_outputA)";
            if (m_outputB->panel_status() == OATS::PanelStatus::PLAYING)
                stop_audio(m_outputB);

            play_audio(m_outputA);
        }
        // Do some outputC stuff
        break;
    case OATS::PanelStatus::PLAYING:
        if ( (m_outputA->fade_trigger_tick_stamp() > 0) &&
             (m_outputA->fade_trigger_tick_stamp()<= trigger_ticker))
        {
            //fade_audio(m_outputA);

            elapsed = trigger_ticker - m_outputA->start_tick_stamp();

            remaining = m_outputA->schedule_item()->audio().duration() - elapsed;
            m_outputA->set_time_remaining(remaining);

            if ( remaining <= 0 ){
                // protect C output
                stop_audio(m_outputA);
            }
        }


    }

    switch(m_outputB->panel_status())
    {
     case OATS::PanelStatus::WAITING:
        m_outputB->set_start_trigger_tick_stamp(-1);
        break;
     case OATS::PanelStatus::CUED:
        if ((m_outputB->start_trigger_tick_stamp() > -1 ) &&
            (m_outputB->start_trigger_tick_stamp() <= (trigger_ticker+0)))
        {
            play_audio(m_outputB);
        }
        break;
    case OATS::PanelStatus::PLAYING:
        if ( (m_outputB->fade_trigger_tick_stamp() > 0) &&
             (m_outputB->fade_trigger_tick_stamp() <= trigger_ticker))
        {
            elapsed = trigger_ticker - m_outputB->start_tick_stamp();
            remaining = m_outputB->schedule_item()->audio().duration()-elapsed;
            m_outputB->set_time_remaining(remaining);


            if (remaining <= 0){
                stop_audio(m_outputB);
            }
        }
    }
}

void MainWindow::count_down()
{
    auto trigger_ticker = get_tick_count();

    switch (m_outputA->panel_status())
    {
    case OATS::PanelStatus::WAITING:
        break;
    case OATS::PanelStatus::CUED:
        break;

    case OATS::PanelStatus::PLAYING:
        int elapsed = trigger_ticker - m_outputA->start_tick_stamp();
        int remaining = m_outputA->schedule_item()->audio().duration() - elapsed;

        if (elapsed < m_outputA->schedule_item()->audio().intro()){
            // intro is still in progress
            auto duration_seconds = (int)(m_outputA->schedule_item()->audio().intro() - elapsed)/100;
            int ds_u1 = (int)duration_seconds/10;
            int ds_u2 = duration_seconds % 10;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                                 .arg(ds_u2, 2, 10, QChar('0'));
            m_outputA->set_cue_time_string(cue_string);
            m_outputA->set_progress_bar_background(OATS::ProgressBarBGColor::RED);

            m_outputA->update_progress_bar( 100 - round(( ((float)(m_outputA->schedule_item()->audio().intro() - elapsed ) /
                                     m_outputA->schedule_item()->audio().intro()) * 100 )
                    ) );


        } else if ( remaining < m_outputA->schedule_item()->audio().fade_out()) {
            int duration_seconds = (int)remaining / 1000;
            int ds_u1 = (int)duration_seconds / 60;
            int ds_u2 = duration_seconds % 60;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                                 .arg(ds_u2, 2, 10, QChar('0'));
            m_outputA->set_cue_time_string(cue_string);
            m_outputA->set_progress_bar_background(OATS::ProgressBarBGColor::GREEN);

            if (m_outputA->schedule_item()->audio().fade_out() > 0) {
                m_outputA->update_progress_bar(
                            100 - round( ((float)m_outputA->time_remaining() /
                                          m_outputA->schedule_item()->audio().fade_out()
                                          ) * 100 )
                            );
            }
        } else {
            int duration_seconds = (int)remaining / 1000;
            int ds_u1 = (int)duration_seconds / 60;
            int ds_u2 = duration_seconds % 60;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                 .arg(ds_u2, 2, 10, QChar('0'));
            m_outputA->set_cue_time_string(cue_string);

            m_outputA->set_progress_bar_background(OATS::ProgressBarBGColor::BLUE);

            // Chunck Color!!!!!
//			if (m_outputA->schedule_item()->audio().fade_out() > 0) {
//				m_outputA->set_progress_bar_background();
//			} else {
//				m_outputA->set_progress_bar_background();
//			}

            m_outputA->update_progress_bar(100 - round(  ((float)m_outputA->time_remaining() /
                                 (m_outputA->schedule_item()->audio().duration() -
                                  m_outputA->schedule_item()->audio().intro()) ) * 100 )
                    );

        }
    }

    // Output B
    switch (m_outputB->panel_status())
    {
     case OATS::PanelStatus::WAITING:
        break;
     case OATS::PanelStatus::CUED:
        break;
     case OATS::PanelStatus::PLAYING:
        int elapsed = trigger_ticker - m_outputB->start_tick_stamp();
        int remaining = m_outputB->schedule_item()->audio().duration() - elapsed;

        if (elapsed < m_outputB->schedule_item()->audio().intro()){
            // intro is still in progress
            auto duration_seconds = (int)(m_outputB->schedule_item()->audio().intro() - elapsed)/100;
            int ds_u1 = (int)duration_seconds/10;
            int ds_u2 = duration_seconds % 10;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                                 .arg(ds_u2, 2, 10, QChar('0'));
            m_outputB->set_cue_time_string(cue_string);
            m_outputB->set_progress_bar_background(OATS::ProgressBarBGColor::RED);

            m_outputB->update_progress_bar(
                        100 - round(((float)(m_outputB->schedule_item()->audio().intro() - elapsed ) /
                                     m_outputB->schedule_item()->audio().intro()) * 100 )
                    );
        } else if ( remaining < m_outputB->schedule_item()->audio().fade_out()) {
            int duration_seconds = (int)remaining / 1000;
            int ds_u1 = (int)duration_seconds / 60;
            int ds_u2 = duration_seconds % 60;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                                 .arg(ds_u2, 2, 10, QChar('0'));
            m_outputB->set_cue_time_string(cue_string);
            m_outputB->set_progress_bar_background(OATS::ProgressBarBGColor::GREEN);

            if (m_outputB->schedule_item()->audio().fade_out() > 0) {
                m_outputB->update_progress_bar(
                            100 - round( ((float)m_outputB->time_remaining() /
                                          m_outputB->schedule_item()->audio().fade_out()
                                          ) * 100 )
                            );
            }
        } else {
            int duration_seconds = (int)remaining / 1000;
            int ds_u1 = (int)duration_seconds / 60;
            int ds_u2 = duration_seconds % 60;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                 .arg(ds_u2, 2, 10, QChar('0'));
            m_outputB->set_cue_time_string(cue_string);

            m_outputB->set_progress_bar_background(OATS::ProgressBarBGColor::BLUE);

            // Chunck Color!!!!!
//			if (m_outputA->schedule_item()->audio().fade_out() > 0) {
//				m_outputA->set_progress_bar_background();
//			} else {
//				m_outputA->set_progress_bar_background();
//			}
            m_outputB->update_progress_bar(100 - round( ((float)m_outputB->time_remaining() /
                                      (m_outputB->schedule_item()->audio().duration() -
                                       m_outputB->schedule_item()->audio().intro())
                                      ) * 100 ) );

        }
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

void MainWindow::make_output_panels()
{
    m_outputA = create_output_panel("A");
    m_outputB = create_output_panel("B");

    m_outputC = std::make_unique<OATS::OutputPanel>("C");
    ui->hlOutput->addWidget(m_outputC.get());
}

void MainWindow::cue_next_schedule(OATS::ScheduleItem* si, OATS::OutputPanel* op)
{
    si->set_item_status(OATS::ItemStatus::CUED);
    op->cue_item(si);
    op->set_panel_status(OATS::PanelStatus::CUED);

    auto duration_seconds = (int)si->audio().duration()/1000;
    auto intro_seconds = (int)si->audio().intro()/1000;
    auto fade_seconds = (int)si->audio().fade_delay()/1000;

    int is_u1 = (int)intro_seconds/60;
    int is_u2 = intro_seconds % 60;

    int ds_u1 = (int)duration_seconds/60;
    int ds_u2 = duration_seconds % 60;

    int fs_u1 = (int)fade_seconds/60;
    int fs_u2 = fade_seconds % 60;

    QString cue_string = QString("%1:%2 / %3:%4 / %5:%6").arg(is_u1, 2, 10, QChar('0'))
                                                         .arg(is_u2, 2, 10, QChar('0'))
                                                         .arg(ds_u1, 2, 10, QChar('0'))
                                                         .arg(ds_u2, 2, 10, QChar('0'))
                                                         .arg(fs_u1, 2, 10, QChar('0'))
                                                         .arg(fs_u2, 2, 10, QChar('0'));
    op->set_cue_time_string(cue_string);

    op->update_progress_bar(0);

    if (si->audio().intro() > 0) {
        op->set_progress_bar_background(OATS::ProgressBarBGColor::RED);
    } else {
        op->set_progress_bar_background(OATS::ProgressBarBGColor::BLUE);
    }

    if (op->panel_name() == "A"){
        qDebug() << "CUE_NEXT_SCHEDULE::* A *";
        test_set_triggerA();
        test_set_fadeA();
    } else {
        qDebug() << "CUE_NEXT_SCHEDULE::* B *";
        test_set_triggerB();
        test_set_fadeB();
    }
}


OATS::OutputPanel* MainWindow::create_output_panel(const QString panel_name)
{
    auto panel = std::make_unique<OATS::OutputPanel>(panel_name);
    panel->set_id(m_output_panels.size());
    ui->hlOutput->addWidget(panel.get());

    connect(panel.get(), &OATS::OutputPanel::play_item, this, &MainWindow::play_audio);
    connect(panel.get(), &OATS::OutputPanel::stop_play, this, &MainWindow::stop_audio);

    auto rPtr = panel.get();
    m_output_panels.push_back(std::move(panel));

    return rPtr;
}

long MainWindow::get_tick_count()
{
    auto time_point = std::chrono::steady_clock::now();
    auto msec = std::chrono::time_point_cast<std::chrono::milliseconds>(time_point);
    return msec.time_since_epoch().count();
}

//void MainWindow::play_audio(int panel_id, OATS::ScheduleItem* s_item)
void MainWindow::play_audio(OATS::OutputPanel* op)
{
    op->set_panel_status(OATS::PanelStatus::PLAYING);
    op->schedule_item()->set_item_status(OATS::ItemStatus::PLAYING);
    op->schedule_item()->set_play_start_time(QTime::currentTime());

    op->set_start_tick_stamp(get_tick_count());

    op->schedule_item()->notify();

    int grid_index = index_of(op->schedule_item()->schedule_ref());

    m_current_playing_item.item = op->schedule_item();
    m_current_playing_item.schedule_index = op->schedule_item()->index();
    m_current_playing_item.grid_index = grid_index;

    int next_id = next_output_panel_id(op->id());
    auto next_output_panel = find_output_panel(next_id);
    auto next_schedule_item = find_next_schedule_item(op->schedule_item());
    if (next_schedule_item != nullptr)
        cue_next_schedule(next_schedule_item, next_output_panel);

    display_schedule(ui->gridScroll->value());

    //calculate_trigger_times();

//    auto main_timer_interval = 25ms;
//    m_main_player_timer->start(main_timer_interval);
}

//void MainWindow::stop_audio(int panel_id, OATS::ScheduleItem* s_item)
void MainWindow::stop_audio(OATS::OutputPanel* op)
{
//	auto active_panel = find_output_panel(panel_id);
    if (op->schedule_item()->item_status() == OATS::ItemStatus::PLAYING){
        op->schedule_item()->set_item_status(OATS::ItemStatus::STOP);
        op->schedule_item()->notify();
        op->set_panel_status(OATS::PanelStatus::WAITING);
    }

    display_schedule(ui->gridScroll->value());

    op->reset_play_button();
    op->reset_stop_button();

    op->update_progress_bar(100);

    if (op->panel_name() == "A"){
        test_set_triggerA();
        test_set_fadeA();
    } else {
        test_set_triggerB();
        test_set_fadeB();
    }
}

OATS::OutputPanel* MainWindow::find_output_panel(int panel_id)
{
    for (auto const& panel : m_output_panels){
        if (panel->id() == panel_id)
            return panel.get();
    }
    return nullptr;

}

int MainWindow::next_output_panel_id(int current_panel_id)
{
    return ((current_panel_id+1) <= m_output_panels.size()-1) ? ++current_panel_id : --current_panel_id;

}

void MainWindow::test()
{
    test_setup();

    auto slow_flash_interval = 400ms;
    m_slow_flash_timer->start(slow_flash_interval);

    auto fast_flash_interval = 250ms;
    m_fast_flash_timer->start(fast_flash_interval);

    auto main_timer_interval = 25ms;
    m_main_player_timer->start(main_timer_interval);

    auto countdown_interval = 50ms;
    m_countdown_timer->start(countdown_interval);

    ui->cbStatusA->setCurrentIndex(1);


}

void MainWindow::calculate_trigger_times()
{
    assert(m_outputA != nullptr);
    assert(m_outputB != nullptr);

    int current_play_start_time{0};

    if (m_current_playing_item.item->item_status() == OATS::ItemStatus::PLAYING)
    {
        if (m_current_playing_item.item->play_channel() == "A" ) {
            current_play_start_time = m_outputA->start_tick_stamp();
        }else{
            current_play_start_time = m_outputB->start_tick_stamp();
        }

        // Check if current playing item need to contribute to the Time Mark Yield
        int time_yield = calculate_yield_contribution(m_current_playing_item.item);

        if (time_yield > 0){

            if (time_yield > 500) {
                m_current_playing_item.item->audio().set_duration(
                            (m_current_playing_item.item->audio().duration() - time_yield) +
                            (YIELD_FADE_DELAY - m_current_playing_item.item->audio().fade_delay())
                            );
                m_current_playing_item.item->audio().set_fade_delay(YIELD_FADE_DELAY);
                m_current_playing_item.item->audio().set_fade_out(YIELD_FADE_OUT);
            }else{
                m_current_playing_item.item->audio().set_duration(
                            m_current_playing_item.item->audio().duration() - time_yield
                            );

            }

        } // time_yield > 0

        compute_schedule_time();

        auto next_schedule_item = find_next_schedule_item(m_current_playing_item.item);

        int drop_start_mark{-1};
        int fade_start_mark{-1};
        int track_start_mark{-1};
        int segue_interval{-1};

        if (next_schedule_item != nullptr){

            if (next_schedule_item->transition_type() == OATS::TransitionType::STOP){
                drop_start_mark = -1;
                fade_start_mark = -1;
                track_start_mark= -1;
            }else if(m_current_playing_item.item->transition_type() == OATS::TransitionType::CUT){
                segue_interval = m_current_playing_item.item->audio().extro() - next_schedule_item->audio().intro();

                track_start_mark = (current_play_start_time + m_current_playing_item.item->audio().duration()) - 500;
                fade_start_mark = -1; // Don't fade

            } else if (m_current_playing_item.item->transition_type() == OATS::TransitionType::MIX){
                segue_interval = (m_current_playing_item.item->audio().extro() + next_schedule_item->audio().intro()) -
                                  m_current_playing_item.item->audio().fade_delay();

                if (m_current_playing_item.item->audio().fade_delay() > (next_schedule_item->audio().duration()-
                                                                         next_schedule_item->audio().fade_delay()) ){

                        m_current_playing_item.item->audio().set_fade_delay((next_schedule_item->audio().duration() -
                                                                     next_schedule_item->audio().fade_delay()) - 500);
                }

                track_start_mark = (current_play_start_time + m_current_playing_item.item->audio().duration()) -
                                   m_current_playing_item.item->audio().fade_delay();

                fade_start_mark = (current_play_start_time + m_current_playing_item.item->audio().duration())-
                                   m_current_playing_item.item->audio().fade_out();

              }

             // Do some drop item calculations
             drop_start_mark = -1;

            }

        if (m_current_playing_item.item->play_channel() == "A"){
            m_outputB->set_start_trigger_tick_stamp(track_start_mark);
            m_outputB->set_fade_trigger_tick_stamp(fade_start_mark);
        }else{
            m_outputA->set_start_trigger_tick_stamp(track_start_mark);
            m_outputA->set_fade_trigger_tick_stamp(fade_start_mark);
        }


    }
}

int MainWindow::calculate_yield_contribution(OATS::ScheduleItem* item)
{
    return 0;
}

void MainWindow::test_setup()
{
// sbA
// tbTRA

    QStringList status = {"WAITING", "CUED", "PLAYING", "STOP"};
    ui->cbStatusA->addItems(status);
    ui->cbStatusB->addItems(status);

    connect(ui->cbStatusA, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::test_change_outputA_status);
    connect(ui->cbStatusB, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::test_change_outputB_status);

    //connect(ui->tbTRA, &QToolButton::clicked, this, &MainWindow::test_set_RT_outputA);
    //connect(ui->tbTRB, &QToolButton::clicked, this, &MainWindow::test_set_RT_outputB);

    //connect(ui->tbTriggerA, &QToolButton::clicked, this, &MainWindow::test_set_triggerA);
    //connect(ui->tbTriggerB, &QToolButton::clicked, this, &MainWindow::test_set_triggerB);

    //connect(ui->tbFadeA, &QToolButton::clicked, this, &MainWindow::test_set_fadeA);
    //connect(ui->tbFadeB, &QToolButton::clicked, this, &MainWindow::test_set_fadeB);
}

void MainWindow::test_set_current_playing_item()
{
    m_current_playing_item.item = schedule_item(0);
    m_current_playing_item.schedule_index = 0;
    m_current_playing_item.grid_index = 0;
    calculate_trigger_times();
}
void MainWindow::test_slow_flash()
{
    int duration = 15000;
    int d_secs = (int)duration/1000;
    int d_sec1 = (int)d_secs/60;
    int d_sec2 = d_secs % 60;
    QString test = QString("Time: %1:%2").arg(d_sec1, 2, 10, QChar('0')).arg(d_sec2, 2, 10, QChar('0'));
    qDebug() << test;
/*
    qDebug() << "test_slow_flash...";
    test_set_current_playing_item();
    if (!ui->btnSlowFlash->isChecked()){
        qDebug() << "test_slow_flash...STOP";
        m_slow_flash_timer->stop();
    }else {
        qDebug() << "test_slow_flash...START";
        auto slow_flash_interval = 400ms;
        m_slow_flash_timer->start(slow_flash_interval.count());
    }
    */
}

void MainWindow::test_fast_flash()
{
    test_set_current_playing_item();
    if (!ui->btnFastFlash->isChecked()){
        qDebug() << "test_fast_flash...STOP";
        m_fast_flash_timer->stop();
    }else {
        qDebug() << "test_fast_flash...START";
        auto fast_flash_interval = 250ms;
        m_fast_flash_timer->start(fast_flash_interval);
    }
}

void MainWindow::test_main_timer()
{
    test_set_current_playing_item();
    if (!ui->btnMainTimer->isChecked()){
        m_main_player_timer->stop();
    } else {
        auto main_timer_interval = 25ms;
        m_main_player_timer->start(main_timer_interval);
    }
}
void MainWindow::test_change_outputA_status(int index)
{
    m_outputA->set_panel_status((OATS::PanelStatus)index);
    m_outputA->schedule_item()->set_item_status((OATS::ItemStatus)index);

    if (m_outputA->panel_status() == OATS::PanelStatus::CUED){
        test_set_triggerA();
        test_set_fadeA();
    }

    m_outputA->update_progress_bar(0);

    //calculate_trigger_times();
    qDebug() << " ** A ** ";
    qDebug() << "OPA->Status: "<< m_outputA->panel_status_text();
    qDebug() << "OPA->Trigger Tick Stamp: "<< m_outputA->start_trigger_tick_stamp();
    qDebug() << "OPA->Fade Trigger Stamp: "<< m_outputA->fade_trigger_tick_stamp();
    qDebug() << "OPA->Timer Remaining: "<< m_outputA->time_remaining();
    qDebug() << "OPA->Status: "<< m_outputA->panel_status_text();


}
void MainWindow::test_change_outputB_status(int index)
{
    assert(m_outputB != nullptr);

    auto next_si = find_next_schedule_item(m_current_playing_item.item);
    m_outputB->cue_item(next_si);

    m_outputB->set_panel_status((OATS::PanelStatus)index);
    m_outputB->schedule_item()->set_item_status((OATS::ItemStatus)index);
    //calculate_trigger_times();
    if (m_outputB->panel_status() == OATS::PanelStatus::CUED){
        test_set_triggerB();
        test_set_fadeB();
    }
}

void MainWindow::test_set_RT_outputA()
{
    //m_outputA->set_time_remaining(ui->sbA->value());
    qDebug() << "OPA->Timer Remaining: "<< m_outputA->time_remaining();
}

void MainWindow::test_set_RT_outputB()
{
    //m_outputB->set_time_remaining(ui->sbB->value());
    qDebug() << "OPB->Timer Remaining: "<< m_outputB->time_remaining();
}

void MainWindow::test_set_triggerA()
{
    m_outputA->set_start_trigger_tick_stamp(get_tick_count() + m_current_playing_item.item->audio().duration());
    qDebug() << "OP_A->Start Trigger Stamp: "<< m_outputA->start_trigger_tick_stamp();
}

void MainWindow::test_set_triggerB()
{
    m_outputB->set_start_trigger_tick_stamp(get_tick_count() + m_current_playing_item.item->audio().duration()); // - fade_delay
    qDebug() <<"OP_B->Start Trigger Stamp: " << m_outputB->start_trigger_tick_stamp();
}

void MainWindow::test_set_fadeA()
{
    m_outputA->set_fade_trigger_tick_stamp(2000);
    //m_outputA->set_fade_trigger_tick_stamp(ui->sbFadeA->value());
    qDebug() << "OP_A->fade trigger stamp: " << m_outputA->fade_trigger_tick_stamp();
}

void MainWindow::test_set_fadeB()
{
    m_outputB->set_fade_trigger_tick_stamp(2000);
    //m_outputB->set_fade_trigger_tick_stamp(ui->sbFadeB->value());
    qDebug() << "OP_B->fade trigger stamp: " << m_outputB->fade_trigger_tick_stamp();
}
