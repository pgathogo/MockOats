#pragma once

#include <memory>

#include <QMainWindow>

namespace OATS{
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

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::vector<std::unique_ptr<OATS::ScheduleGridItem>> m_schedule_grid;

};
