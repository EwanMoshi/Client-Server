#pragma once

#include "ReplicationCommand.h"

class PacketDeliveryNotificationManager;

class TransmissionData {
public:
	virtual void addTransmission(int networkId, ReplicationAction replicationAction) = 0;
	virtual void handleDeliverySuccess(PacketDeliveryNotificationManager* deliveryNotificationManager) const = 0;
	virtual void handleDeliveryFailure(PacketDeliveryNotificationManager* deliveryNotificationManager) const = 0;
};