#include "pch.h"
#include "NetworkManagerServer.h"
#include "ReliableCharacterTransmissionData.h"
#include "ReliableWelcomeTransmissionData.h"
#include "Server.h"
#include <MoveList.h>

NetworkManagerServer* NetworkManagerServer::Instance = nullptr;

NetworkManagerServer::NetworkManagerServer() : newPlayerIdCounter(1), newNetworkId(1) { }

bool NetworkManagerServer::staticInit(uint16_t inPort) {
	if (Instance == nullptr) {
		Instance = new NetworkManagerServer();

		// call parent init
		return Instance->init(inPort);
	}

	return true;
}

void NetworkManagerServer::sendOutgoingPackets() {
	for (const auto& iter : addressToClient) {
		if (iter.second->isStateDirty()) {
			std::cout << "[NetworkManagerServer::sendOutgoingPackets] State dirty. Sending state packet to client" << std::endl;
			sendStatePacketToClient(iter.second);
		}
	}
	//for (auto iter = addressToClient.begin(), end = addressToClient.end(); iter != end; ++iter) {
	//	std::shared_ptr<ClientProxy> clientProxy = iter->second;
	//	
	//}
}

void NetworkManagerServer::sendStatePacketToClient(const std::shared_ptr<ClientProxy>& clientProxy) {
	OutputBitStream statePacket;

	statePacket.write(stateMessage);

	InFlightPacket* inFlightPacket = clientProxy->getPacketDeliveryNotificationManager().writeStatePacket(statePacket);

	std::shared_ptr<ReliableCharacterTransmissionData> reliableCharacterTransmissionData =
		std::make_shared<ReliableCharacterTransmissionData>(&clientProxy->getReplicationManagerServer(), clientProxy);

	
	clientProxy->getReplicationManagerServer().write(statePacket, reliableCharacterTransmissionData);

	// RPCH = Replication Character
	// TODO: Maybe change RPCH to stateMessage 
	inFlightPacket->setTransmissionData('RPCH', reliableCharacterTransmissionData);

	sendPacket(statePacket, clientProxy->getSocketAddress());

	// now that state packet has been sent, we can mark state as not dirty
	clientProxy->setStateDirty(false);
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
			// if (clientProxy->getPacketDeliveryNotificationManager().readAndProcessWelcomePacket(inputStream)) {
			//	sendWelcomePacket(clientProxy);
			// }
			sendWelcomePacket(clientProxy);
			break;
		}
		case inputMessage: {
			if (clientProxy->getPacketDeliveryNotificationManager().readAndProcessStatePacket(inputStream)) {
				handleInputPacket(clientProxy, inputStream);
			}
			break;
		}
		default: {
			std::cout << "[NetworkManagerServer::processPacket]: Unknown packet type received" << std::endl;
			LOG("[NetworkManagerServer::processPacket]: Unknown packet type received from socket address %s", clientProxy->getSocketAddress().toString().c_str());
			break;
		}
	}
}

void NetworkManagerServer::sendWelcomePacket(const std::shared_ptr<ClientProxy>& clientProxy) {
	OutputBitStream welcomePacket;

	welcomePacket.write(welcomeMessage);

	// InFlightPacket* inFlightPacket = clientProxy->getPacketDeliveryNotificationManager().writeWelcomePacket(welcomePacket);

	welcomePacket.write(clientProxy->getPlayerId());

	LOG("[NetworkManagerServer::sendWelcomePacket]: New client '%s' as player %d", clientProxy->getName().c_str(), clientProxy->getPlayerId());
	std::cout << "[NetworkManagerServer::sendWelcomePacket]: New client " << clientProxy->getName().c_str() << " as player " << clientProxy->getPlayerId() << std::endl;

	// inFlightPacket->setTransmissionData(welcomeMessage, std::make_shared<ReliableWelcomeTransmissionData>(clientProxy));

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
		playerIdToClientProxy[newClientProxy->getPlayerId()] = newClientProxy;

		Server::Instance->handleNewClient(newClientProxy->getPlayerId());

		sendWelcomePacket(newClientProxy);

		for (const auto& pair : networkIdToGameObject) {
			newClientProxy->getReplicationManagerServer().replicateCreate(pair.first);
		}
	}
	else {
		LOG("[NetworkManagerServer::handlePacketFromNewClient]: Received packet from new client that isn't hello from socket address %s", fromAddress.toString().c_str());
	}
}

void NetworkManagerServer::handleInputPacket(const std::shared_ptr<ClientProxy>& clientProxy, InputBitStream& inputStream) {
	uint32_t moveCount = 0;
	Move move;
	inputStream.read(moveCount);
	std::cout << "[NetworkManagerServer::handleInputPacket]: Move Count = " << moveCount << std::endl;

	for (; moveCount > 0; moveCount--) {
		if (move.read(inputStream)) {
			clientProxy->getUnprocessedMoveList().addMove(move);
			clientProxy->setStateDirty(true);
		}
	}
}

std::shared_ptr<ClientProxy> NetworkManagerServer::getClientProxy(int playerId) {
	auto pair = playerIdToClientProxy.find(playerId);
	if (pair != playerIdToClientProxy.end()) {
		return pair->second;
	}

	return nullptr;
}

int NetworkManagerServer::getNewNetworkId() {
	int networkIdToReturn = newNetworkId++;

	if (newNetworkId < networkIdToReturn) {
		LOG("[NetworkManagerServer::getNewNetworkId] wrap around");
		std::cout << "[NetworkManagerServer::getNewNetworkId] wrap around" << std::endl;
	}

	return networkIdToReturn;
}

void NetworkManagerServer::registerGameObject(std::shared_ptr<GameObject> gameObject) {
	int newNetworkId = getNewNetworkId();
	gameObject->setNetworkId(newNetworkId);

	networkIdToGameObject[newNetworkId] = gameObject;

	for (const auto& pair : addressToClient) {
		pair.second->getReplicationManagerServer().replicateCreate(newNetworkId);
	}
}