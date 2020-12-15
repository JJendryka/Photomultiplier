#include "experiment.h"

Experiment::Experiment() : series(), data()
{
    startTime = QDateTime::currentDateTime();
}

std::vector<std::pair<QString, QString>> Experiment::getParameters() {
    auto list = std::vector<std::pair<QString, QString>>();
    list.push_back(std::make_pair("Title", name));
    list.push_back(std::make_pair("Description", description));
    list.push_back(std::make_pair("Start time", startTime.toString()));
    return list;
}

void Experiment::setParameter(QString name, QString value) {
    if (name == "Title") {
        this->name = value;
    }
    else if (name == "Description") {
        this->description = value;
    }
}

void Experiment::setData(std::vector<std::vector<float>> data) {
    this->data = std::vector<std::vector<float>>();
    for (ulong i=0; i<data.size(); i++) {
        this->data.push_back(std::vector<float>());
        for (ulong j=0; j<data[i].size(); j++) {
            this->data[i].push_back(data[i][j]);
        }
    }
}

std::vector<std::vector<float>> Experiment::getData() {
    return data;
}
