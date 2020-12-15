#ifndef PACKET_H
#define PACKET_H

#include <memory>
#include <QString>
#include <QStringList>
#include <QMetaType>

#define ERROR_PACKET 'E'
#define MEASUREMENT_RESULT_PACKET 'M'
#define CURRENT_POSITION_PACKET 'P'
#define ENDSTOP_STATE_PACKET 'S'
#define HOMED_PACKET 'H'

#define MEASURE_PACKET 'm'
#define LASER_STATE_PACKET 'l'
#define VOLTAGE_PACKET 'v'
#define THRESHOLD_PACKET 't'
#define PHOTOMULTIPLIER_STATE_PACKET 'a'
#define TARGET_POSITION_PACKET 'g'

class Packet
{
public:
    static std::shared_ptr<Packet> parsePacket(QString);
    virtual ~Packet() = default;

    char type;

protected:
    Packet(char type);
};

Q_DECLARE_METATYPE(std::shared_ptr<Packet>);

class SendablePacket : public Packet
{
public:
    virtual QString serialize() = 0;
    QString appendType(QString);

protected:
     SendablePacket(char type);
};

Q_DECLARE_METATYPE(std::shared_ptr<SendablePacket>);

class ErrorPacket : public Packet {
public:
    ErrorPacket(QString string) : Packet(ERROR_PACKET) {
        error = string.right(string.size()-2);
    }

    QString error;
};

class MeasuremetResultPacket : public Packet {
public:
    MeasuremetResultPacket(QString string) : Packet(MEASUREMENT_RESULT_PACKET) {
        value = string.split(" ")[1].toInt();
    }

    unsigned long value;
};

class CurrentPositionPacket : public Packet {
public:
    CurrentPositionPacket(QString string) : Packet(CURRENT_POSITION_PACKET) {
        position = string.split(" ")[1].toInt();
    }

    unsigned long position;
};

class EndstopStatePacket : public Packet {
public:
    EndstopStatePacket(QString string) : Packet(ENDSTOP_STATE_PACKET) {
        state = (string.split(" ")[1].toInt() == 1);
    }

    bool state;
};

class TargetPositionPacket : public SendablePacket {
public:
    TargetPositionPacket(int position) : SendablePacket(TARGET_POSITION_PACKET) {
        this->position = position;
    }

    QString serialize() override {
        return appendType(QString::number(position));
    }

    int position;
};

class VoltagePacket : public SendablePacket {
public:
    VoltagePacket(int voltage) : SendablePacket(VOLTAGE_PACKET) {
        this->voltage = voltage;
    }

    QString serialize() override {
        return appendType(QString::number(voltage));
    }

    int voltage;
};

class ThresholdPacket : public SendablePacket {
public:
    ThresholdPacket(int threshold) : SendablePacket(THRESHOLD_PACKET) {
        this->threshold = threshold;
    }

    QString serialize() override {
        return appendType(QString::number(threshold));
    }

    int threshold;
};

class LaserPacket : public SendablePacket {
public:
    LaserPacket(bool state) : SendablePacket(LASER_STATE_PACKET) {
        this->state = state;
    }

    QString serialize() override {
        return appendType(state ? "1" : "0");
    }

    bool state;
};

class AmplifierPacket : public SendablePacket {
public:
    AmplifierPacket(bool state) : SendablePacket(PHOTOMULTIPLIER_STATE_PACKET) {
        this->state = state;
    }

    QString serialize() override {
        return appendType(state ? "1" : "0");
    }

    bool state;
};

class MeasurePacket : public SendablePacket {
public:
    MeasurePacket(int miliseconds) : SendablePacket(MEASURE_PACKET) {
        this->miliseconds = miliseconds;
    }

    QString serialize() override {
        return appendType(QString::number(miliseconds));
    }

    int miliseconds;
};

#endif // PACKET_H
