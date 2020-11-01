#include "controldock.h"
#include "ui_controldock.h"

ControlDock::ControlDock(QWidget *parent) :
    QDockWidget(parent)
{
    setupUi(this);
    connectObjects();
    setupSlider();
}

void ControlDock::connectObjects() {
    connect(positionFineLeftButton, &QPushButton::clicked, this, &ControlDock::onFineLeftClicked);
    connect(positionCoarseLeftButton, &QPushButton::clicked, this, &ControlDock::onCoarseLeftClicked);
    connect(positionFineRightButton, &QPushButton::clicked, this, &ControlDock::onFineRightClicked);
    connect(positionCoarseRightButton, &QPushButton::clicked, this, &ControlDock::onCoarseRightClicked);

    connect(positionHomeButton, &QPushButton::clicked, this, &ControlDock::onHomeClicked);

    connect(positionSlider, &QSlider::valueChanged, this, &ControlDock::onSliderMoved);
    connect(positionSlider, &QSlider::sliderReleased, this, &ControlDock::onSliderReleased);

    connect(positionLineEdit, &QLineEdit::returnPressed, this, &ControlDock::onValueEntered);
}

void ControlDock::setupSlider() {
    positionSlider->setMinimum(0);
    positionSlider->setMaximum(MAX_TARGET_POSITION);
    positionSlider->setSingleStep(1);
    positionSlider->setPageStep(COARSE_STEP);
    positionSlider->setTickPosition(QSlider::TicksAbove);
    positionSlider->setTickInterval(2000);
}

void ControlDock::onFineLeftClicked() {
    targetPosition -= 1;
    if (targetPosition < 0) {
        targetPosition = 0;
    }
    updateTargetPosition();
    sendTargetPosition();
}

void ControlDock::onCoarseLeftClicked() {
    targetPosition -= COARSE_STEP;
    if (targetPosition < 0) {
        targetPosition = 0;
    }
    updateTargetPosition();
    sendTargetPosition();
}

void ControlDock::onFineRightClicked() {
    targetPosition += 1;
    if (targetPosition > MAX_TARGET_POSITION) {
        targetPosition = MAX_TARGET_POSITION;
    }
    updateTargetPosition();
    sendTargetPosition();
}

void ControlDock::onCoarseRightClicked() {
    targetPosition += COARSE_STEP;
    if (targetPosition > MAX_TARGET_POSITION) {
        targetPosition = MAX_TARGET_POSITION;
    }
    updateTargetPosition();
    sendTargetPosition();
}

void ControlDock::onHomeClicked() {
    positionSlider->setValue(0);
    positionLineEdit->setText(QString::number(0));
    targetPositionLabel->setText("Target position: " + QString::number(0) + "mm");
}

void ControlDock::onSliderMoved() {
    targetPosition = positionSlider->value();
    updateTargetPosition();
}

void ControlDock::onSliderReleased() {
    updateTargetPosition();
    sendTargetPosition();
}

void ControlDock::onValueEntered() {
    targetPosition = positionLineEdit->text().toFloat() / MM_PER_STEP;
    updateTargetPosition();
    sendTargetPosition();
}

void ControlDock::updateTargetPosition() {
    positionSlider->setValue(targetPosition);
    positionLineEdit->setText(QString::number(targetPosition * MM_PER_STEP));
    targetPositionLabel->setText("Target position: " + QString::number((float)targetPosition*MM_PER_STEP, 'f', 3) + "mm");
}

void ControlDock::sendTargetPosition() {
    sendPacket(std::make_shared<TargetPositionPacket>(targetPosition));
}

void ControlDock::onPacketReceived(std::shared_ptr<Packet> packet) {
    if (packet->type == CURRENT_POSITION_PACKET) {
        int currentPosition = std::dynamic_pointer_cast<CurrentPositionPacket>(packet)->position;
        currentPositionLabel->setText("Current position: " + QString::number((float)currentPosition*MM_PER_STEP, 'f', 3) + "mm");
    }
}
