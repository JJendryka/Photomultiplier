#include "packet.h"

Packet::Packet(char type)
{
    this->type = type;
}

std::shared_ptr<Packet> Packet::parsePacket(QString string)
{
    switch (string.toStdString()[0]) {
        case ERROR_PACKET:
            return std::make_shared<ErrorPacket>(string);
        case MEASUREMENT_RESULT_PACKET:
            return std::make_shared<MeasuremetResultPacket>(string);
        case CURRENT_POSITION_PACKET:
            return std::make_shared<CurrentPositionPacket>(string);
        case ENDSTOP_STATE_PACKET:
            return std::make_shared<EndstopStatePacket>(string);
        default:
            return std::make_shared<ErrorPacket>("E Unrecognized packet: " + string);
    }
}

SendablePacket::SendablePacket(char type) : Packet(type) {}

QString SendablePacket::appendType(QString string) {
    return (QString)type + " " + string + "\n";
}
