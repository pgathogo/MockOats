#pragma once

#include <memory>

#include <QMainWindow>

namespace OATS{
    class ScheduleItem;
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::vector<std::unique_ptr<OATS::ScheduleItem>> m_schedule_items;

};
