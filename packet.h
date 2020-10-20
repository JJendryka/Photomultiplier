#ifndef PACKET_H
#define PACKET_H

#include <QString>
#include <QStringList>
#include <QMetaType>

#define ERROR 'E'
#define MEASUREMENT_RESULT 'M'
#define CURRENT_POSITION 'P'
#define ENDSTOP_STATE 'S'
#define HOMED 'H'

#define MEASURE 'm'
#define LASER_STATE 'l'
#define VOLTAGE 'v'
#define PHOTOMULTIPLIER_STATE 'a'

class Packet
{
public:
    static Packet* parsePacket(QString);
    virtual ~Packet();

    char type;

protected:
    Packet(char type);
};

Q_DECLARE_METATYPE(Packet*);

class SendablePacket : public Packet
{
public:
    virtual QString serialize() = 0;
};

Q_DECLARE_METATYPE(SendablePacket*);

class ErrorPacket : public Packet {
public:
    ErrorPacket(QString string) : Packet(ERROR) {
        error = string.right(string.size()-2);
    }

    QString error;
};

class MeasuremetResultPacket : public Packet {
public:
    MeasuremetResultPacket(QString string) : Packet(MEASUREMENT_RESULT) {
        value = string.split(" ")[1].toInt();
    }

    unsigned long value;
};

class CurrentPositionPacker : public Packet {
public:
    CurrentPositionPacker(QString string) : Packet(CURRENT_POSITION) {
        position = string.split(" ")[1].toInt();
    }

    unsigned long position;
};

class EndstopStatePacket : public Packet {
public:
    EndstopStatePacket(QString string) : Packet(ENDSTOP_STATE) {
        state = (string.split(" ")[1].toInt() == 1);
    }

    bool state;
};

#endif // PACKET_H
