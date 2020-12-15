#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialport.h"
#include "controldock.h"
#include "measurementdock.h"
#include "terminaldock.h"
#include "experiment.h"


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

    QMenu *fileMenu;
    QMenu *viewMenu;
    Ui::MainWindow *ui;
    SerialPort serial;
    TerminalDock *terminalDock;
    ControlDock *controlDock;
    MeasurementDock *measurementDock;
    QChart *chart;
    QValueAxis *xAxis;
    QValueAxis *yAxis;

private slots:
    void removeSeries(QList<QAbstractSeries *> series);
    void addSeries(QList<QAbstractSeries *> series);
    void setAxes(AxesRange range);
};
#endif // MAINWINDOW_H
