#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <QString>
#include <QtCharts>
#include <QDateTime>
#include <memory>

struct DataPointParameters {
    bool laserState;
    int measurementTimeMs;
    float accelerationVoltage;
    float thresholdVoltage;
    float position;
};

class Experiment
{
public:
    Experiment();

    // Returns parameters to measure next data point
    virtual std::unique_ptr<DataPointParameters> getNextParameters() = 0;
    // Adds new data point
    virtual void addNewData(long result) = 0;
    // Draws collected data
    virtual void drawData(QChart *chart) = 0;

    // Prepares device for the experiment
    void startExperiment();
    // Returns device to
    void stopExperiment();

    //Saves data to file
    virtual QString save() = 0;
    //Loads data from file
    virtual void load(QString data) = 0;

    QString name;
    QString description;
    QDateTime startTime;

};

#endif // EXPERIMENT_H
