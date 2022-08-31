#pragma once

#include "PacketDeliveryNotificationManager.h"
#include "ReplicationManagerServer.h"

class ClientProxy {
public:
	ClientProxy(const SocketAddress& socketAddress, const std::string& name, int playerId);

	bool isStateDirty() const;
	void setStateDirty(bool isDirty);

	const SocketAddress& getSocketAddress()	const { 
		return socketAddress; 
	}

	int getPlayerId() const { 
		return playerId; 
	}

	const std::string& getName() const { 
		return name; 
	}

	ReplicationManagerServer& getReplicationManagerServer();
	PacketDeliveryNotificationManager& getPacketDeliveryNotificationManager();

private:
	ReplicationManagerServer replicationManagerServer;
	PacketDeliveryNotificationManager packetDeliveryNotificationManager;
	SocketAddress socketAddress;
	std::string name;
	int playerId;

	bool stateDirty;
};

