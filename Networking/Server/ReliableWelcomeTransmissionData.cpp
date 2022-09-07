#include "pch.h"
#include "ReliableWelcomeTransmissionData.h"

ReliableWelcomeTransmissionData::ReliableWelcomeTransmissionData(std::shared_ptr<ClientProxy> clientProxy) :
	clientProxy(std::move(clientProxy))
{

}

void ReliableWelcomeTransmissionData::handleDeliverySuccess(PacketDeliveryNotificationManager* packetDeliveryNotificationManager) const {
	std::cout << "[ReliableWelcomeTransmissionData::handleDeliveryFailure]: WelcomeTransmissionData successfull sent" << std::endl;
}

void ReliableWelcomeTransmissionData::handleDeliveryFailure(PacketDeliveryNotificationManager* packetDeliveryNotificationManager) const {
	std::cout << "[ReliableWelcomeTransmissionData::handleDeliveryFailure]: Attempting to resend packet" << std::endl;
	//NetworkManagerServer::Instance->sendWelcomePacket(std::move(clientProxy));
}
