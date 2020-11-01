#ifndef CONTROLDOCK_H
#define CONTROLDOCK_H

#include <QDockWidget>
#include <QTimer>

#include "packet.h"
#include "ui_controldock.h"

#define MAX_TARGET_POSITION 20000
#define MM_PER_STEP 0.005
#define COARSE_STEP 5

#define MAX_THRESHOLD 1023
#define THRESHOLD_PER_STEP  0.00477028347

#define MAX_VOLTAGE 1007
#define VOLTAGE_PER_STEP 1.19165839126

#define TIMEOUT_MS 200

class ControlDock : public QDockWidget, private Ui::ControlDock
{
    Q_OBJECT

private:
    int targetPosition;

    bool amplifierState = false;
    bool laserState = false;

    QTimer positionTimer;
    QTimer voltageTimer;
    QTimer thresholdTimer;

    void connectObjects();
    void setupSlider();
    void updateTargetPosition();

private slots:
    void onFineLeftClicked();
    void onCoarseLeftClicked();
    void onFineRightClicked();
    void onCoarseRightClicked();

    void onHomeClicked();

    void onPositionSliderMoved();
    void onValueEntered();

    void onVoltageSliderMoved();

    void onThresholdSliderMoved();

    void onAmplifierButtonClicked();
    void onLaserButtonClicked();

    void sendTargetPosition();
    void sendVoltage();
    void sendThreshold();

public slots:
    void onPacketReceived(std::shared_ptr<Packet>);


signals:
    void sendPacket(std::shared_ptr<SendablePacket>);

public:
    explicit ControlDock(QWidget *parent = nullptr);
};

#endif // ControlDock_H
