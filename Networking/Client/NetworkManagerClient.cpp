#include "pch.h"
#include "NetworkManagerClient.h"
#include "Client.h"

NetworkManagerClient* NetworkManagerClient::Instance = nullptr;

NetworkManagerClient::NetworkManagerClient() :
	clientState(NCS_Uninitialized),
	helloPacketSent(false),
	packetDeliveryNotificationManager(true, false) // client needs to send acks but not process them
{ }

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
			//if (packetDeliveryNotificationManager.readAndProcessWelcomePacket(inputStream)) {
			//	handleWelcomePacket(inputStream);
			//}
			handleWelcomePacket(inputStream);
			break;
		}
		case stateMessage: {
			handleStatePacket(inputStream);
			break;
		}
	}
}

void NetworkManagerClient::sendOutgoingPackets() {
	switch (clientState)
	{
		case NCS_SayingHello: {

			if (!helloPacketSent) {
				OutputBitStream helloPacket;

				helloPacket.write(helloMessage);
				helloPacket.write(name);

				//packetDeliveryNotificationManager.writeWelcomePacket(helloPacket);

				std::cout << "sending hello packet" << std::endl;
				sendPacket(helloPacket, serverAddress);
				helloPacketSent = true;
			}

			break;
		}
		case NCS_Welcomed: {
			sendInputPacket();
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

		/*OutputBitStream ackPacket;
		ackPacket.write(helloMessage);

		std::cout << " -------    WRITING  " << helloMessage << std::endl;

		packetDeliveryNotificationManager.writeWelcomePacket(ackPacket);
		sendPacket(ackPacket, serverAddress);
		std::cout << "DEBUG OUTPUT ackPacket " << ackPacket.getBufferPtr() << std::endl;*/
	}
}

void NetworkManagerClient::sendInputPacket() {
	MoveList& moveList = Client::Instance->getMoveList();

	if (moveList.hasMoves()) {
		std::cout << "[NetworkManagerClient::sendInputPacket] MoveList not empty, sending moves." << std::endl;

		OutputBitStream inputPacket;
		inputPacket.write(inputMessage);

		// send last three moves
		int moveCount = moveList.getMoveCount();
		int startIndex = moveCount > 3 ? moveCount - 3 - 1 : 0;
		inputPacket.write(moveCount - startIndex);
		
		for (int i = startIndex; i < moveCount; i++) {
			moveList[i].write(inputPacket);
		}

		sendPacket(inputPacket, serverAddress);
		moveList.clear();
	}
}

void NetworkManagerClient::handleStatePacket(InputBitStream& inputStream) {
	if (clientState == NCS_Welcomed) {
		replicationManager.read(inputStream);
	}
}

float NetworkManagerClient::getPacketLossChance() {
	return 0.0f;
}