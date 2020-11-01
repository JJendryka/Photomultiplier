#include <QMessageBox>

#include "serialport.h"

SerialPort::SerialPort(QWidget *parent) : QObject(parent), serialPort(parent) {}

void SerialPort::openPort() {
    serialPort.setPortName("/dev/tnt0");
    serialPort.setBaudRate(9600);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort.open(QIODevice::ReadWrite)) {
        portOpen();
        QObject::connect(&serialPort, SIGNAL(readyRead()), this, SLOT(serialReadReady()));
    }
    else {
        QMessageBox::critical(parent, tr("Error"), serialPort.errorString());
    }
}

SerialPort::~SerialPort() {
    if(serialPort.isOpen()) {
        serialPort.close();
    }
}

void SerialPort::serialReadReady() {
    while (serialPort.canReadLine()) {
        QString string = serialPort.readLine();
        auto packet = Packet::parsePacket(string);
        newPacket(packet);
        newRawIncomingPacket(string);
    }
}

void SerialPort::sendRawText(QString string) {
    newRawOutgoingPacket(string);
    serialPort.write(string.toUtf8());
}

void SerialPort::sendPacket(std::shared_ptr<SendablePacket> packet) {
    QString string = packet->serialize();
    sendRawText(string);
}
