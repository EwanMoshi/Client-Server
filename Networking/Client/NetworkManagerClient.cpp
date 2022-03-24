#include "pch.h"
#include "NetworkManagerClient.h"

NetworkManagerClient* NetworkManagerClient::Instance = nullptr;

NetworkManagerClient::NetworkManagerClient() : clientState(NCS_Uninitialized) { }

void NetworkManagerClient::staticInit(const SocketAddress& serverAddress, const std::string& name) {
	if (Instance == nullptr) {
		Instance = new NetworkManagerClient();

		// call parent init
		return Instance->init(serverAddress, name);
	}
}

void NetworkManagerClient::init(const SocketAddress& serverAddress, const std::string& name)
{
	NetworkManagerBase::init(0);

	this->serverAddress = serverAddress;
	this->name = name;
	clientState = NCS_SayingHello;
}

void NetworkManagerClient::processPacket(InputBitStream& inputStream, const SocketAddress& fromAddress) {
	uint32_t packetType;
	inputStream.read(packetType);

	switch (packetType)	{
	case welcomeMessage: {
		handleWelcomePacket(inputStream);
		break;
	}
	}
}


void NetworkManagerClient::sendOutgoingPackets() {
	switch (clientState)
	{
		case NCS_SayingHello: {
			OutputBitStream helloPacket;

			helloPacket.write(helloMessage);
			helloPacket.write(name);

			std::cout << "sending hello packet" << std::endl;

			sendPacket(helloPacket, serverAddress);

			break;
		}
		case NCS_Welcomed: {
			// send something else
			int a = 5;
			break;
		}
	}
}

void NetworkManagerClient::handleWelcomePacket(InputBitStream& inputStream) {
	if (clientState == NCS_SayingHello) {
		// probably don't need this local variable
		int playerId;
		inputStream.read(playerId);

		this->playerId = playerId;
		clientState = NCS_Welcomed;

		LOG("Client '%s' was welcomed as player %d", name.c_str(), this->playerId);
		std::cout << "Client with name: " << name.c_str() << " welcomed by server as player with ID: " << this->playerId << std::endl;
	}
}
