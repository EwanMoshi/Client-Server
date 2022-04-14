#pragma once

class PacketDeliveryNotificationManager;

class ReliableWelcomeTransmissionData : public TransmissionData {
public:
	ReliableWelcomeTransmissionData(std::shared_ptr<ClientProxy> clientProxy);

	virtual void handleDeliverySuccess(PacketDeliveryNotificationManager* packetDeliveryNotificationManager) const override;
	virtual void handleDeliveryFailure(PacketDeliveryNotificationManager* packetDeliveryNotificationManager) const override;

private:
	std::shared_ptr<ClientProxy> clientProxy;
};

