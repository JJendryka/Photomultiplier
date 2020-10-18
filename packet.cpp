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
            return new Error(string);
        case MEASUREMENT_RESULT:
            return new MeasuremetResult(string);
        case CURRENT_POSITION:
            return new CurrentPosition(string);
        case ENDSTOP_STATE:
            return new EndstopState(string);
        default:
            return new Error("E Unrecognized packet: " + string);
    }
}
