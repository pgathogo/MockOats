#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "scheduleitem.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->vlSchedule->setSpacing(0);
    ui->vlSchedule->setContentsMargins(0,0,0,0);

    std::unique_ptr<OATS::ScheduleItem> schedule_item1 = std::make_unique<OATS::ScheduleItem>("background-color:green");
    std::unique_ptr<OATS::ScheduleItem> schedule_item2 = std::make_unique<OATS::ScheduleItem>("background-color:yellow");
    std::unique_ptr<OATS::ScheduleItem> schedule_item3 = std::make_unique<OATS::ScheduleItem>("background-color:blue");

    ui->vlSchedule->addWidget(schedule_item1.get());
    ui->vlSchedule->addWidget(schedule_item2.get());
    ui->vlSchedule->addWidget(schedule_item3.get());
    ui->vlSchedule->insertStretch(-1,1);

    m_schedule_items.push_back(std::move(schedule_item1));
    m_schedule_items.push_back(std::move(schedule_item2));
    m_schedule_items.push_back(std::move(schedule_item3));

}

MainWindow::~MainWindow()
{
    delete ui;
}

