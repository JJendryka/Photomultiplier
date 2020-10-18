#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QSerialPort>

#include "packet.h"

class SerialPort : QObject
{
    Q_OBJECT

private:
    QWidget* parent;
    QSerialPort serialPort;

public:
    SerialPort(QWidget *parent);
    ~SerialPort();

public slots:
    void openPort();
    void serialReadReady();

signals:
    void portOpen();
    void newPacket(Packet*);
};

#endif // SERIALPORT_H
