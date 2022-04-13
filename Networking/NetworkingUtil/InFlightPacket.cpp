#include "pch.h"
#include "InFlightPacket.h"

InFlightPacket::InFlightPacket(PacketSequenceNumber sequenceNumber) :	sequenceNumber(sequenceNumber),	timeDispatched(Timing::instance.GetTimeAsFloat()) { }

PacketSequenceNumber InFlightPacket::getSequenceNubmer() const {
	return sequenceNumber;
}

float InFlightPacket::getTimeDispatched() const {
	return timeDispatched;
}