#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialport.h"
#include "controldock.h"
#include "measurementdock.h"
#include "terminaldock.h"


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
    void createMenu();

    void createDocks();
    void createControlDock();
    void createMeasurementDock();
    void createTerminalDock();

    void createPlot();

    void connectObjects();

    QMenu *viewMenu;
    Ui::MainWindow *ui;
    SerialPort serial;
    TerminalDock *terminalDock;
    ControlDock *controlDock;
    MeasurementDock *measurementDock;
};
#endif // MAINWINDOW_H
