#pragma once

#include "PacketDeliveryNotificationManager.h"

class ClientProxy {
public:
	ClientProxy(const SocketAddress& socketAddress, const std::string& name, int playerId);

	const SocketAddress& getSocketAddress()	const { 
		return socketAddress; 
	}

	int getPlayerId() const { 
		return playerId; 
	}

	const std::string& getName() const { 
		return name; 
	}

	PacketDeliveryNotificationManager& getPacketDeliveryNotificationManager();

private:
	PacketDeliveryNotificationManager packetDeliveryNotificationManager;
	SocketAddress socketAddress;
	std::string name;
	int playerId;
};

