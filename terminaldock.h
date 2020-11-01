#ifndef TERMINALDOCK_H
#define TERMINALDOCK_H

#include <QDockWidget>
#include <memory>

#include "ui_terminaldock.h"

class TerminalDock : public QDockWidget, private Ui::TerminalDock
{
    Q_OBJECT

public:
    explicit TerminalDock(QWidget *parent = nullptr);

signals:
    void sendRaw(QString);

public slots:
    void rawIncomingPacket(QString);
    void rawOutgoingPacket(QString);

    void buttonClicked();
};

#endif // TERMINALDOCK_H
