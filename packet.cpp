#include "packet.h"

Packet::Packet(char type)
{
    this->type = type;
}

Packet::~Packet() {}

Packet* Packet::parsePacket(QString string)
{
    switch (string.toStdString()[0]) {
        case ERROR:
            return new ErrorPacket(string);
        case MEASUREMENT_RESULT:
            return new MeasuremetResultPacket(string);
        case CURRENT_POSITION:
            return new CurrentPositionPacker(string);
        case ENDSTOP_STATE:
            return new EndstopStatePacket(string);
        default:
            return new ErrorPacket("E Unrecognized packet: " + string);
    }
}
