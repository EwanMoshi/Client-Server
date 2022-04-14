#pragma once

typedef uint16_t PacketSequenceNumber;

class PacketDeliveryNotificationManager;
class TransmissionData;

class InFlightPacket
{
public:
	InFlightPacket(PacketSequenceNumber sequenceNumber);

	void setTransmissionData(int key, std::shared_ptr<TransmissionData> transmissionData);
	const std::shared_ptr<TransmissionData> getTransmissionData(int key) const;

	void handleDeliverySuccess(PacketDeliveryNotificationManager* deliveryNotificationManager) const;
	void handleDeliveryFailure(PacketDeliveryNotificationManager* deliveryNotificationManager) const;

	PacketSequenceNumber getSequenceNubmer() const;
	float getTimeDispatched() const;

private:
	PacketSequenceNumber sequenceNumber;
	float timeDispatched;

	std::unordered_map<int, std::shared_ptr<TransmissionData>> transmissionData;
};

