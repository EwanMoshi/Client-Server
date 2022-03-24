#pragma once

#include <memory>
#include "NetworkManagerBase.h"
#include <unordered_map>
#include "ClientProxy.h"

class ClientProxy;

class NetworkManagerServer : public NetworkManagerBase {

public:
	static NetworkManagerServer* Instance;

	static bool staticInit(uint16_t port);

	virtual void processPacket(InputBitStream& inputStream, const SocketAddress& fromAddress) override;

	std::shared_ptr<ClientProxy> getClientProxy(int playerId);

private:
	// constructor is private - create instance by calling static init function
	NetworkManagerServer();

	void processPacket(std::shared_ptr<ClientProxy> clientProxy, InputBitStream& inputStream);
	void handlePacketFromNewClient(InputBitStream& inputStream, const SocketAddress& fromAddress);
	void sendWelcomePacket(std::shared_ptr<ClientProxy> clientProxy);

	std::unordered_map<int, std::shared_ptr<ClientProxy>> playerIdToClient;
	std::unordered_map<SocketAddress, std::shared_ptr<ClientProxy>> addressToClient;

	int newPlayerIdCounter;
};

