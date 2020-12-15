#include "basicexperiment.h"

BasicExperiment::BasicExperiment() : Experiment() {
    this->begining = 0;
    this->end = 0;
    this->acceleration_voltage = 0;
    this->threshold_voltage = 0;
    this->period = 0;
    this->pitch = 0;
    series.append(new QLineSeries);
    this->name = "No name basic experiment";
}

std::unique_ptr<DataPointParameters> BasicExperiment::getNextParameters() {
    hasStarted = true;
    float position = (data.size() != 0) ? data[data.size()-1][0] + pitch : begining;
    if (position > end) {
        hasFinished = true;
        return nullptr;
    }

    auto params = std::make_unique<struct DataPointParameters>();

    params->accelerationVoltage = acceleration_voltage;
    params->measurementTimeMs = period;
    params->position = position;
    params->laserState = true;
    params->thresholdVoltage = threshold_voltage;

    return params;
}

void BasicExperiment::addNewData(long result) {
    qDebug(name.toUtf8() + " %d", result);
    float position = (data.size() != 0) ? data[data.size()-1][0] + pitch : begining;
    data.push_back({position, result/period});
    ((QLineSeries *)series[0])->append(position, result/period);
}

float BasicExperiment::getProgress() {
    float position = (data.size() != 0) ? data[data.size()-1][0] + pitch : begining;
    return (position - begining)/(end + pitch - begining);
}

std::vector<std::pair<QString, QString>> BasicExperiment::getParameters() {
    auto list = this->Experiment::getParameters();
    list.push_back(std::make_pair("Beginning", QString::number(begining)));
    list.push_back(std::make_pair("End", QString::number(end)));
    list.push_back(std::make_pair("Acceleration voltage", QString::number(acceleration_voltage)));
    list.push_back(std::make_pair("Threshold voltage", QString::number(threshold_voltage)));
    list.push_back(std::make_pair("Period", QString::number(period)));
    list.push_back(std::make_pair("Pitch", QString::number(pitch)));
    return list;
}

void BasicExperiment::setParameter(QString name, QString value) {
    if (name == "Beginning") {
        begining = value.toFloat();
    }
    else if (name == "End") {
        end = value.toFloat();
    }
    else if (name == "Acceleration voltage") {
        acceleration_voltage = value.toFloat();
    }
    else if (name == "Threshold voltage") {
        threshold_voltage = value.toFloat();
    }
    else if (name == "Period") {
        period = value.toFloat();
    }
    else if (name == "Pitch") {
        pitch = value.toFloat();
    }
    else {
        this->Experiment::setParameter(name, value);
    }
}

AxesRange BasicExperiment::getAxesRange() {
    float max = 0;
    for (int i=0; i<data.size(); i++) {
        if (data[i][1] > max) {
            max = data[i][1];
        }
    }
    return {begining, end, 0, max};
}
