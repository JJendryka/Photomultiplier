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

    fileMenu->addAction("Save", measurementDock, &MeasurementDock::save);

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
    fileMenu = menuBar()->addMenu("File");
}

void MainWindow::createPlot() {
    chart = new QChart();
    chart->legend()->hide();
    xAxis = new QValueAxis();
    yAxis = new QValueAxis();
    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);

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

    connect(&serial, &SerialPort::newPacket, measurementDock, &MeasurementDock::onPacketReceived);
    connect(measurementDock, &MeasurementDock::sendPacket, &serial, &SerialPort::sendPacket);
    connect(measurementDock, &MeasurementDock::removeSeries, this, &MainWindow::removeSeries);
    connect(measurementDock, &MeasurementDock::addSeries, this, &MainWindow::addSeries);
    connect(measurementDock, &MeasurementDock::setAxes, this, &MainWindow::setAxes);
}

void MainWindow::removeSeries(QList<QAbstractSeries *> series) {
    foreach (QAbstractSeries *s, series) {
        chart->removeSeries(s);
    }
}
void MainWindow::addSeries(QList<QAbstractSeries *> series) {
    foreach (QAbstractSeries *s, series) {
        chart->addSeries(s);
        s->attachAxis(xAxis);
        s->attachAxis(yAxis);
    }

}
void MainWindow::setAxes(AxesRange range) {
    xAxis->setMin(range.minX);
    xAxis->setMax(range.maxX);
    yAxis->setMin(range.minY);
    yAxis->setMax(range.maxY);
}
