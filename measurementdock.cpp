#include "measurementdock.h"
#include "ui_measurementdock.h"
#include "newexperimentdialog.h"
#include "basicexperiment.h"
#include "controldock.h"
#include "mainwindow.h"

MeasurementDock::MeasurementDock(QWidget *parent) :
    QDockWidget(parent)
{
    setupUi(this);
    connectObjects();
}

void MeasurementDock::connectObjects() {
    connect(newMeasurementButton, &QPushButton::clicked, this, &MeasurementDock::onNewClicked);
    connect(measurementComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MeasurementDock::onCurrentExperimentChanged);
    connect(parameterTable, QOverload<int, int>::of(&QTableWidget::cellChanged), this, &MeasurementDock::onCellChanged);
    connect(loadMeasurementButton, &QPushButton::clicked, this, &MeasurementDock::onLoadClicked);
    connect(saveMeasurementButton, &QPushButton::clicked, this, &MeasurementDock::onSaveLocationClicked);
    connect(startButton, &QPushButton::clicked, this, &MeasurementDock::onStartClicked);
}

void MeasurementDock::onNewClicked() {
    QStringList experimentTypes({"Basic Experiment"});
    NewExperimentDialog dialog(experimentTypes);
    if (dialog.exec() == QDialog::Accepted)
    {
        addNewExperiment(new BasicExperiment);
    }
}

void MeasurementDock::addNewExperiment(Experiment *experiment) {
    this->experiments.append(experiment);
    measurementComboBox->addItem(experiment->name);
}

void MeasurementDock::onCurrentExperimentChanged(int index) {
    if (currentExperimentIndex > 0) {
        auto previousExperiment = experiments[currentExperimentIndex];
        removeSeries(previousExperiment->series);
    }

    currentExperimentIndex = index;
    auto parameters = experiments[index]->getParameters();
    parameterTable->clear();
    parameterTable->setColumnCount(2);
    parameterTable->setRowCount(parameters.size());
    int i=0;
    for (const auto& pair : parameters) {
        auto item = new QTableWidgetItem(pair.first, QTableWidgetItem::Type);
        item->setFlags(Qt::ItemFlag::ItemIsEnabled);
        parameterTable->setItem(i, 0, item);
        parameterTable->setItem(i, 1, new QTableWidgetItem(pair.second, QTableWidgetItem::Type));
        i++;
    }

    auto currentExperiment = experiments[currentExperimentIndex];
    progressBar->setValue(currentExperiment->getProgress() * 100);
    setAxes(currentExperiment->getAxesRange());


    addSeries(currentExperiment->series);

    if (currentExperiment->hasStarted && !currentExperiment->hasFinished) {
        startButton->setText("Resume");
        startButton->setDisabled(false);
    }
    else if (currentExperiment->hasFinished) {
        startButton->setText("Finished");
        startButton->setDisabled(true);
    }
    else {
        startButton->setText("Start");
        startButton->setDisabled(false);
    }
}

void MeasurementDock::onCellChanged(int row, int column) {
    if (column == 1) {
        QString key = parameterTable->item(row, 0)->data(Qt::ItemDataRole::DisplayRole).toString();
        QString value = parameterTable->item(row, 1)->data(Qt::ItemDataRole::DisplayRole).toString();
        experiments[currentExperimentIndex]->setParameter(key, value);

        if (key == "Title") {
            auto index = measurementComboBox->currentIndex();
            measurementComboBox->setItemText(index, value);
        }

        progressBar->setValue(experiments[currentExperimentIndex]->getProgress());
    }
}

void MeasurementDock::onLoadClicked() {
    auto dialog = QFileDialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setDirectory(QDir::currentPath());
    dialog.setNameFilter("*.json *.csv");
    if (dialog.exec()) {
        auto fileNames = dialog.selectedFiles();

        auto files = QSet<QString>();
        foreach (QString name, fileNames) {
            files.insert(name.split(".").at(0));
        }

        foreach (QString name, files) {

            // Load json file
            QFile jsonFile(name + ".json");
            if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qWarning("Couldn't open json file");
                continue;
            }
            QString jsonContent = jsonFile.readAll();
            jsonFile.close();
            qWarning() << jsonContent;
            QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonContent.toUtf8());
            QJsonObject json = jsonDocument.object();

            Experiment* loadedExperiment;

            // Determine experiment type
            auto experimentType = json["type"].toString();
            if (experimentType == "basic") {
                loadedExperiment = new BasicExperiment();
            }
            else {
                qWarning("Invalid experiment type");
                continue;
            }

            // Set file to save to
            loadedExperiment->saveFileName = name;

            // Load parameters
            foreach(const QString& key, json.keys()) {
                if (key != "type") {
                    QJsonValue value = json.value(key);
                    loadedExperiment->setParameter(key, value.toString());
                }
            }

            // Load csv file
            QFile csvFile(name + ".csv");
            if (!csvFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qWarning("Couldn't open csv file");
            }

            auto data = std::vector<std::vector<float>>();
            while (!csvFile.atEnd()) {
                QString row = csvFile.readLine();
                data.push_back(std::vector<float>());
                auto columns = row.split("\t");
                foreach (QString column, columns) {
                    data[data.size()-1].push_back(column.toFloat());
                }
                csvFile.close();
            }
            loadedExperiment->setData(data);

            // Add loaded experiment to other experiments
            addNewExperiment(loadedExperiment);
        }
    }
}

void MeasurementDock::onSaveLocationClicked() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory to which file will be saved"), QDir::currentPath(), QFileDialog::ShowDirsOnly);
    auto currentExperiment = experiments[currentExperimentIndex];
    currentExperiment->saveFileName = dir + "/" + currentExperiment->name + "_" + QString::number(currentExperiment->startTime.toSecsSinceEpoch());
}

void MeasurementDock::save() {
    for (const auto experiment : experiments) {

        if (experiment->saveFileName == nullptr) {
            continue;
        }

        // Create json
        auto json = QJsonObject();
        auto identifier = experiment->getIdentifier();
        json["type"] = identifier;
        auto parameters = experiment->getParameters();
        for (const auto& parameter : parameters) {
            json[parameter.first] = parameter.second;
        }

        // Save json to appropriate file
        QFile jsonFile(experiment->saveFileName + ".json");
        if (!jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning("Couldn't open json file");
            continue;
        }
        QJsonDocument jsonDocument(json);
        jsonFile.write(jsonDocument.toJson());
        jsonFile.close();

        // Get data from experiment
        auto data = experiment->getData();

        QFile csvFile(experiment->saveFileName + ".csv");
        if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning("Couldn't open csv file");
            continue;
        }
        for (const auto& row : data) {
            for (const auto& column : row) {
                csvFile.write(QString::number(column).toUtf8());
                csvFile.write("\t");
            }
            csvFile.write("\n");
        }
        csvFile.close();
    }
}

void MeasurementDock::onStartClicked() {
    if (isExperimentRunning) {
        isExperimentRunning = false;
    }
    else {
        isExperimentRunning = true;
        measurementComboBox->setDisabled(true);
        startButton->setText("Stop");

        executeNextMeasurement();
    }
}

void MeasurementDock::onPacketReceived(std::shared_ptr<Packet> packet) {
    if (packet->type == MEASUREMENT_RESULT_PACKET) {
        auto result = std::dynamic_pointer_cast<MeasuremetResultPacket>(packet)->value;
        auto experiment = experiments[currentExperimentIndex];
        experiment->addNewData(result);
        progressBar->setValue(experiment->getProgress()*100);
        setAxes(experiment->getAxesRange());
        if (isExperimentRunning) {
            executeNextMeasurement();
        }
        else {
            startButton->setText("Paused");
            measurementComboBox->setDisabled(false);
        }
    }
}

void MeasurementDock::executeNextMeasurement() {
    auto experiment = experiments[currentExperimentIndex];
    auto parameters = experiment->getNextParameters();
    if (parameters != nullptr) {
        sendPacket(std::make_shared<LaserPacket>(parameters->laserState));
        sendPacket(std::make_shared<TargetPositionPacket>(parameters->position * MM_PER_STEP));
        sendPacket(std::make_shared<AmplifierPacket>(true));
        sendPacket(std::make_shared<VoltagePacket>(parameters->accelerationVoltage * VOLTAGE_PER_STEP));
        sendPacket(std::make_shared<ThresholdPacket>(parameters->thresholdVoltage * THRESHOLD_PER_STEP));
        sendPacket(std::make_shared<MeasurePacket>(parameters->measurementTimeMs));
    }
    else {
        isExperimentRunning = false;
        startButton->setText("Finished");
        measurementComboBox->setDisabled(false);
        startButton->setDisabled(true);
    }
}
