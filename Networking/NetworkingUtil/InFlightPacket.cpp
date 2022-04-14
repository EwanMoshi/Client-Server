#include "pch.h"
#include "InFlightPacket.h"

InFlightPacket::InFlightPacket(PacketSequenceNumber sequenceNumber) : sequenceNumber(sequenceNumber), timeDispatched(Timing::instance.GetTimeAsFloat()) { }

void InFlightPacket::setTransmissionData(int key, std::shared_ptr<TransmissionData> transmissionData) {
	this->transmissionData[key] = transmissionData;
}

const std::shared_ptr<TransmissionData> InFlightPacket::getTransmissionData(int key) const {
	auto it = transmissionData.find(key);

	if (it != transmissionData.end()) {
		return it->second;
	}

	return nullptr;
}

void InFlightPacket::handleDeliverySuccess(PacketDeliveryNotificationManager* deliveryNotificationManager) const {
	for (const auto& pair : transmissionData) {
		pair.second->handleDeliverySuccess(deliveryNotificationManager);
	}
}

void InFlightPacket::handleDeliveryFailure(PacketDeliveryNotificationManager* deliveryNotificationManager) const {
	for (const auto& pair : transmissionData) {
		pair.second->handleDeliveryFailure(deliveryNotificationManager);
	}
}

PacketSequenceNumber InFlightPacket::getSequenceNubmer() const {
	return sequenceNumber;
}

float InFlightPacket::getTimeDispatched() const {
	return timeDispatched;
}