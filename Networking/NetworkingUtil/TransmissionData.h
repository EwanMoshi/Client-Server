#pragma once

class PacketDeliveryNotificationManager;

class TransmissionData {
public:
	virtual void handleDeliverySuccess(PacketDeliveryNotificationManager* deliveryNotificationManager) const = 0;
	virtual void handleDeliveryFailure(PacketDeliveryNotificationManager* deliveryNotificationManager) const = 0;
};