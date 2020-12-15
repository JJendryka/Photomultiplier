#ifndef BASICEXPERIMENT_H
#define BASICEXPERIMENT_H

#include "experiment.h"

class BasicExperiment : public Experiment
{
public:
    BasicExperiment();

    QString getIdentifier() override {
        return "basic";
    }

    std::unique_ptr<struct DataPointParameters> getNextParameters() override;
    void addNewData(long result) override;
    float getProgress() override;

    std::vector<std::pair<QString, QString>> getParameters() override;
    void setParameter(QString name, QString value) override;

    AxesRange getAxesRange() override;

    float begining;
    float end;
    float acceleration_voltage;
    float threshold_voltage;
    float period;
    float pitch;
};

#endif // BASICEXPERIMENT_H
