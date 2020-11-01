#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <memory>
#include <QSerialPort>

#include "packet.h"

Q_DECLARE_METATYPE(std::shared_ptr<QString>);

class SerialPort : public QObject
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

    void sendRawText(QString);
    void sendPacket(std::shared_ptr<SendablePacket>);

signals:
    void portOpen();
    void newPacket(std::shared_ptr<Packet>);
    void newRawIncomingPacket(QString);
    void newRawOutgoingPacket(QString);
};

#endif // SERIALPORT_H
