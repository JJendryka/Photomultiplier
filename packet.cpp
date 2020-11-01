#include "packet.h"

Packet::Packet(char type)
{
    this->type = type;
}

Packet::~Packet() {}

std::shared_ptr<Packet> Packet::parsePacket(QString string)
{
    switch (string.toStdString()[0]) {
        case ERROR:
            return std::make_shared<ErrorPacket>(string);
        case MEASUREMENT_RESULT:
            return std::make_shared<MeasuremetResultPacket>(string);
        case CURRENT_POSITION:
            return std::make_shared<CurrentPositionPacker>(string);
        case ENDSTOP_STATE:
            return std::make_shared<EndstopStatePacket>(string);
        default:
            return std::make_shared<ErrorPacket>("E Unrecognized packet: " + string);
    }
}
