#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "schedulegriditem.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->vlSchedule->setSpacing(1);
    ui->vlSchedule->setContentsMargins(0,0,0,2);

    for (int i=0; i < MAX_GRID_ITEMS; ++i){
        std::unique_ptr<OATS::ScheduleGridItem> grid_item = std::make_unique<OATS::ScheduleGridItem>("background-color:green");
        ui->vlSchedule->addWidget(grid_item.get());
        m_schedule_grid.push_back(std::move(grid_item));
    }

    ui->vlSchedule->insertStretch(-1,1);

}

MainWindow::~MainWindow()
{
    delete ui;
}

