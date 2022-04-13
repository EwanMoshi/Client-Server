#pragma once

class DeliveryNotificationManager;

class TransmissionData {
public:
	virtual void handleDeliverySuccess(DeliveryNotificationManager* deliveryNotificationManager) const = 0;
	virtual void handleDeliveryFailure(DeliveryNotificationManager* deliveryNotificationManager) const = 0;
};