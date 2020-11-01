#include <QtCharts>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serial(this)
{
    ui->setupUi(this);

    createMenu();

    createDocks();
    createPlot();

    connectObjects();

    serial.openPort();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createDocks() {
    createControlDock();
    createMeasurementDock();
    createTerminalDock();
}

void MainWindow::createControlDock() {
    controlDock = new ControlDock(this);
    addDockWidget(Qt::RightDockWidgetArea, controlDock);
    viewMenu->addAction(controlDock->toggleViewAction());
}

void MainWindow::createMeasurementDock() {
    measurementDock = new MeasurementDock(this);
    addDockWidget(Qt::BottomDockWidgetArea, measurementDock);
    viewMenu->addAction(measurementDock->toggleViewAction());
}

void MainWindow::createTerminalDock() {
    terminalDock = new TerminalDock(this);
    addDockWidget(Qt::BottomDockWidgetArea, terminalDock);
    viewMenu->addAction(terminalDock->toggleViewAction());
}

void MainWindow::createMenu() {
    viewMenu = menuBar()->addMenu("View");
}

void MainWindow::createPlot() {
    QLineSeries *series = new QLineSeries();
    series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);
    *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Simple line chart example");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    setCentralWidget(chartView);
}

void MainWindow::connectObjects() {
    connect(&serial, &SerialPort::newRawIncomingPacket, terminalDock, &TerminalDock::rawIncomingPacket);
    connect(&serial, &SerialPort::newRawOutgoingPacket, terminalDock, &TerminalDock::rawOutgoingPacket);
    connect(terminalDock, &TerminalDock::sendRaw, &serial, &SerialPort::sendRawText);

    connect(&serial, &SerialPort::newPacket, controlDock, &ControlDock::onPacketReceived);
    connect(controlDock, &ControlDock::sendPacket, &serial, &SerialPort::sendPacket);
}
