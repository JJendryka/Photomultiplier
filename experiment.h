#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <QString>
#include <QtCharts>
#include <QDateTime>
#include <memory>

struct AxesRange {
    float minX;
    float maxX;
    float minY;
    float maxY;
};

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

    // Returns experiment type identifier
    virtual QString getIdentifier() = 0;

    // Returns parameters to measure next data point
    virtual std::unique_ptr<struct DataPointParameters> getNextParameters() = 0;
    // Adds new data point
    virtual void addNewData(long result) = 0;
    // Returns progress (0 to 1)
    virtual float getProgress() = 0;

    ////TODO: Make a method to prepare experiment

    // Returns list of parameter names
    virtual std::vector<std::pair<QString, QString>> getParameters();
    // Sets parameters according to list from getParameters()
    virtual void setParameter(QString name, QString value);

    // Returns all recorded data in table format [row][column]
    virtual std::vector<std::vector<float>> getData();
    // Sets data (same format as getData())
    virtual void setData(std::vector<std::vector<float>> data);

    virtual AxesRange getAxesRange() = 0;

    QString name;
    QString description;
    QDateTime startTime;

    QList<QAbstractSeries *> series;

    bool hasStarted = false;
    bool hasFinished = false;

    QString saveFileName = nullptr;

protected:
    std::vector<std::vector<float>> data;
};

#endif // EXPERIMENT_H
