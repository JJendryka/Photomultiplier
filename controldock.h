#ifndef CONTROLDOCK_H
#define CONTROLDOCK_H

#include <QDockWidget>

#include "packet.h"
#include "ui_controldock.h"

#define MAX_TARGET_POSITION 20000
#define MM_PER_STEP 0.005
#define COARSE_STEP 5

class ControlDock : public QDockWidget, private Ui::ControlDock
{
    Q_OBJECT

private:
    int targetPosition;

    void connectObjects();
    void setupSlider();
    void updateTargetPosition();
    void sendTargetPosition();

public slots:
    void onFineLeftClicked();
    void onCoarseLeftClicked();
    void onFineRightClicked();
    void onCoarseRightClicked();

    void onHomeClicked();

    void onSliderMoved();
    void onSliderReleased();
    void onValueEntered();

    void onPacketReceived(std::shared_ptr<Packet>);

signals:
    void sendPacket(std::shared_ptr<SendablePacket>);

public:
    explicit ControlDock(QWidget *parent = nullptr);
};

#endif // ControlDock_H
