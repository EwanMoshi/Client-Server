#include "pch.h"
#include "NetworkManagerServer.h"
#include "ReliableWelcomeTransmissionData.h"

NetworkManagerServer* NetworkManagerServer::Instance = nullptr;

NetworkManagerServer::NetworkManagerServer() : newPlayerIdCounter(1) { }

bool NetworkManagerServer::staticInit(uint16_t inPort) {
	if (Instance == nullptr) {
		Instance = new NetworkManagerServer();

		// call parent init
		return Instance->init(inPort);
	}

	return true;
}

void NetworkManagerServer::processTimedOutPackets() {
	for (const auto& pair : addressToClient) {
		pair.second->getPacketDeliveryNotificationManager().processTimedOutPackets();
	}
}

void NetworkManagerServer::processPacket(InputBitStream& inputStream, const SocketAddress& fromAddress) {
	auto pair = addressToClient.find(fromAddress);
	if (pair == addressToClient.end()) {
		std::cout << "[NetworkManagerServer::processPacket]: packet from new client using socket address: %s" << fromAddress.toString().c_str() << std::endl;

		handlePacketFromNewClient(inputStream, fromAddress);
	}
	else {
		processPacket((*pair).second, inputStream);
	}
}


void NetworkManagerServer::processPacket(std::shared_ptr<ClientProxy> clientProxy, InputBitStream& inputStream) {
	std::cout << "SERVER DEBUG OUTPUT incoming packet  " << inputStream.getBufferPtr() << std::endl;

	uint32_t packetType;
	inputStream.read(packetType);
	std::cout << " -------    READING  " << packetType << std::endl;

	switch (packetType) {
	case helloMessage: {
		// NOTE: This is temporary
		if (clientProxy->getPacketDeliveryNotificationManager().readAndProcessWelcomePacket(inputStream)) {
			sendWelcomePacket(clientProxy);
		}
		break;
	}
	default: {
		LOG("[NetworkManagerServer::processPacket]: Unknown packet type received from socket address %s", clientProxy->getSocketAddress().toString().c_str());
		break;
	}
	}
}

void NetworkManagerServer::sendWelcomePacket(std::shared_ptr<ClientProxy> clientProxy) {
	OutputBitStream welcomePacket;

	welcomePacket.write(welcomeMessage);

	InFlightPacket* inFlightPacket = clientProxy->getPacketDeliveryNotificationManager().writeWelcomePacket(welcomePacket);

	welcomePacket.write(clientProxy->getPlayerId());

	LOG("[NetworkManagerServer::sendWelcomePacket]: New client '%s' as player %d", clientProxy->getName().c_str(), clientProxy->getPlayerId());
	std::cout << "[NetworkManagerServer::sendWelcomePacket]: New client " << clientProxy->getName().c_str() << " as player " << clientProxy->getPlayerId() << std::endl;

	inFlightPacket->setTransmissionData(welcomeMessage, std::make_shared<ReliableWelcomeTransmissionData>(clientProxy));

	sendPacket(welcomePacket, clientProxy->getSocketAddress());
}

void NetworkManagerServer::handlePacketFromNewClient(InputBitStream& inputStream, const SocketAddress& fromAddress) {
	uint32_t packetType;
	inputStream.read(packetType);

	if (packetType == helloMessage) {
		std::string name;
		inputStream.read(name);

		auto newClientProxy = std::make_shared<ClientProxy>(fromAddress, name, newPlayerIdCounter++);
		addressToClient[fromAddress] = newClientProxy;
		playerIdToClient[newClientProxy->getPlayerId()] = newClientProxy;

		if (newClientProxy->getPacketDeliveryNotificationManager().readAndProcessWelcomePacket(inputStream)) {
			sendWelcomePacket(newClientProxy);
		}

		// can spawn object for player like player controller, or whatever


		// TODO: init replication manager
	}
	else {
		LOG("[NetworkManagerServer::handlePacketFromNewClient]: Received packet from new client that isn't hello from socket address %s", fromAddress.toString().c_str());
	}
}

std::shared_ptr<ClientProxy> NetworkManagerServer::getClientProxy(int playerId) {
	auto pair = playerIdToClient.find(playerId);
	if (pair != playerIdToClient.end()) {
		return pair->second;
	}

	return nullptr;
}