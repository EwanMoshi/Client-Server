#pragma once

typedef uint16_t PacketSequenceNumber;

class DeliveryNotificationManager;
class TransmissionData;

class InFlightPacket
{
public:
	InFlightPacket(PacketSequenceNumber sequenceNumber);

	PacketSequenceNumber getSequenceNubmer() const;
	float getTimeDispatched() const;

private:
	PacketSequenceNumber sequenceNumber;
	float timeDispatched;

	std::unordered_map<int, std::shared_ptr<TransmissionData>> transmissionData;
};

