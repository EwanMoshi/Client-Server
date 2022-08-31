#pragma once

#include <string.h>
#include "NetworkManagerBase.h"
#include "PacketDeliveryNotificationManager.h"
#include "ReplicationManagerClient.h"

class NetworkManagerClient : public NetworkManagerBase {
	enum NetworkClientState {
		NCS_Uninitialized,
		NCS_SayingHello,
		NCS_Welcomed
	};

public:
	static NetworkManagerClient* Instance;

	static void	staticInit(const SocketAddress& serverAddress, const std::string& name);

	void sendOutgoingPackets();

	void handleWelcomePacket(InputBitStream& inputStream);
	void handleStatePacket(InputBitStream& inputStream);

	virtual void processPacket(InputBitStream& inputStream, const SocketAddress& fromAddress) override;

protected:
	float getPacketLossChance() override;
	
private:
	NetworkManagerClient();
	void init(const SocketAddress& serverAddress, const std::string& name);

	PacketDeliveryNotificationManager packetDeliveryNotificationManager;
	ReplicationManagerClient replicationManager;

	SocketAddress serverAddress;
	NetworkClientState clientState;
	std::string name;
	int playerId;

	// this is temp to stop spamming the server with hello packets
	bool helloPacketSent;
};

