#include <QMessageBox>

#include "controldock.h"
#include "ui_controldock.h"

ControlDock::ControlDock(QWidget *parent) :
    QDockWidget(parent)
{
    setupUi(this);
    connectObjects();
    setupSlider();

    positionTimer.setSingleShot(true);
    voltageTimer.setSingleShot(true);
    thresholdTimer.setSingleShot(true);
}

void ControlDock::connectObjects() {
    connect(positionFineLeftButton, &QPushButton::clicked, this, &ControlDock::onFineLeftClicked);
    connect(positionCoarseLeftButton, &QPushButton::clicked, this, &ControlDock::onCoarseLeftClicked);
    connect(positionFineRightButton, &QPushButton::clicked, this, &ControlDock::onFineRightClicked);
    connect(positionCoarseRightButton, &QPushButton::clicked, this, &ControlDock::onCoarseRightClicked);

    connect(positionHomeButton, &QPushButton::clicked, this, &ControlDock::onHomeClicked);

    connect(positionSlider, &QSlider::valueChanged, this, &ControlDock::onPositionSliderMoved);

    connect(positionLineEdit, &QLineEdit::returnPressed, this, &ControlDock::onValueEntered);

    connect(thresholdSlider, &QSlider::valueChanged, this, &ControlDock::onThresholdSliderMoved);
    connect(voltageSlider, &QSlider::valueChanged, this, &ControlDock::onVoltageSliderMoved);

    connect(&positionTimer, &QTimer::timeout, this, &ControlDock::sendTargetPosition);
    connect(&thresholdTimer, &QTimer::timeout, this, &ControlDock::sendThreshold);
    connect(&voltageTimer, &QTimer::timeout, this, &ControlDock::sendVoltage);

    connect(photomultiplierStatusButton, &QPushButton::clicked, this, &ControlDock::onAmplifierButtonClicked);

    connect(laserStatusButton, &QPushButton::clicked, this, &ControlDock::onLaserButtonClicked);
}

void ControlDock::setupSlider() {
    positionSlider->setMaximum(MAX_TARGET_POSITION);
    positionSlider->setPageStep(COARSE_STEP);
    positionSlider->setTickPosition(QSlider::TicksAbove);
    positionSlider->setTickInterval(2000);

    thresholdSlider->setMaximum(MAX_THRESHOLD);
    thresholdSlider->setPageStep(COARSE_STEP);
    thresholdSlider->setTickPosition(QSlider::TicksBelow);
    thresholdSlider->setTickInterval(1/THRESHOLD_PER_STEP);

    voltageSlider->setMaximum(MAX_VOLTAGE);
    voltageSlider->setPageStep(COARSE_STEP);
    voltageSlider->setTickPosition(QSlider::TicksBelow);
    voltageSlider->setTickInterval(100/VOLTAGE_PER_STEP);
}

void ControlDock::onFineLeftClicked() {
    targetPosition -= 1;
    if (targetPosition < 0) {
        targetPosition = 0;
    }
    updateTargetPosition();
    positionTimer.start(TIMEOUT_MS);
}

void ControlDock::onCoarseLeftClicked() {
    targetPosition -= COARSE_STEP;
    if (targetPosition < 0) {
        targetPosition = 0;
    }
    updateTargetPosition();
    positionTimer.start(TIMEOUT_MS);
}

void ControlDock::onFineRightClicked() {
    targetPosition += 1;
    if (targetPosition > MAX_TARGET_POSITION) {
        targetPosition = MAX_TARGET_POSITION;
    }
    updateTargetPosition();
    positionTimer.start(TIMEOUT_MS);
}

void ControlDock::onCoarseRightClicked() {
    targetPosition += COARSE_STEP;
    if (targetPosition > MAX_TARGET_POSITION) {
        targetPosition = MAX_TARGET_POSITION;
    }
    updateTargetPosition();
    positionTimer.start(TIMEOUT_MS);
}

void ControlDock::onHomeClicked() {
    positionSlider->setValue(0);
    positionLineEdit->setText(QString::number(0));
    targetPositionLabel->setText("Target position: " + QString::number(0) + "mm");
}

void ControlDock::onPositionSliderMoved() {
    targetPosition = positionSlider->value();
    updateTargetPosition();
    positionTimer.start(TIMEOUT_MS);
}

void ControlDock::onValueEntered() {
    targetPosition = positionLineEdit->text().toFloat() / MM_PER_STEP;
    updateTargetPosition();
    positionTimer.start(TIMEOUT_MS);
}

void ControlDock::updateTargetPosition() {
    positionSlider->setValue(targetPosition);
    positionLineEdit->setText(QString::number(targetPosition * MM_PER_STEP));
    targetPositionLabel->setText("Target position: " + QString::number((float)targetPosition*MM_PER_STEP, 'f', 3) + "mm");
}

void ControlDock::sendTargetPosition() {
    sendPacket(std::make_shared<TargetPositionPacket>(targetPosition));
}

void ControlDock::onVoltageSliderMoved() {
    voltageLabel->setText("Acceleration voltage: " + QString::number(voltageSlider->value() * VOLTAGE_PER_STEP) + "V");
    voltageTimer.start(TIMEOUT_MS);
}

void ControlDock::sendVoltage() {
    sendPacket(std::make_shared<VoltagePacket>(voltageSlider->value()));
}

void ControlDock::onThresholdSliderMoved() {
    thresholdLabel->setText("Threshold voltage: " + QString::number(thresholdSlider->value() * THRESHOLD_PER_STEP) + "V");
    thresholdTimer.start(TIMEOUT_MS);
}

void ControlDock::sendThreshold() {
    sendPacket(std::make_shared<ThresholdPacket>(thresholdSlider->value()));
}

void ControlDock::onAmplifierButtonClicked() {
    if (!amplifierState) {
        QMessageBox msgBox;
        msgBox.setText("Amplifier state change");
        msgBox.setInformativeText("Are you sure you want to turn on the amplifier?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Ok) {
            amplifierState = true;
            photomultiplierStatusButton->setText("On");
            photomultiplierStatusButton->setStyleSheet("QPushButton {color: green;}");
            sendPacket(std::make_shared<AmplifierPacket>(true));
        }
    } else {
        amplifierState = false;
        photomultiplierStatusButton->setText("Off");
        photomultiplierStatusButton->setStyleSheet("QPushButton {color: red;}");
        sendPacket(std::make_shared<AmplifierPacket>(false));
    }
}

void ControlDock::onLaserButtonClicked() {
    if (!laserState) {
        laserState = true;
        laserStatusButton->setText("On");
        laserStatusButton->setStyleSheet("QPushButton {color: green;}");
        sendPacket(std::make_shared<LaserPacket>(true));
    } else {
        laserState = false;
        laserStatusButton->setText("Off");
        laserStatusButton->setStyleSheet("QPushButton {color: red;}");
        sendPacket(std::make_shared<LaserPacket>(false));
    }
}

void ControlDock::onPacketReceived(std::shared_ptr<Packet> packet) {
    if (packet->type == CURRENT_POSITION_PACKET) {
        int currentPosition = std::dynamic_pointer_cast<CurrentPositionPacket>(packet)->position;
        currentPositionLabel->setText("Current position: " + QString::number((float)currentPosition*MM_PER_STEP, 'f', 3) + "mm");
    }
}
