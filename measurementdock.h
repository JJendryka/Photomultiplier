#ifndef MEASUREMENTDOCK_H
#define MEASUREMENTDOCK_H

#include <QDockWidget>
#include <ui_measurementdock.h>

#include "packet.h"
#include "experiment.h"

class MeasurementDock : public QDockWidget, private Ui::MeasurementDock
{
    Q_OBJECT

public:
    explicit MeasurementDock(QWidget *parent = nullptr);

private:
    void connectObjects();
    void addNewExperiment(Experiment* experiment);

    QList<Experiment *> experiments;
    int currentExperimentIndex = -1;

    bool isExperimentRunning = false;

    void executeNextMeasurement();

public slots:
    void save();
    void onPacketReceived(std::shared_ptr<Packet>);

signals:
    void sendPacket(std::shared_ptr<SendablePacket>);

    void removeSeries(QList<QAbstractSeries *> series);
    void addSeries(QList<QAbstractSeries *> series);
    void setAxes(AxesRange range);

private slots:
    void onNewClicked();
    void onCurrentExperimentChanged(int index);
    void onCellChanged(int row, int column);
    void onLoadClicked();
    void onSaveLocationClicked();
    void onStartClicked();
};

#endif // MEASUREMENTDOCK_H
