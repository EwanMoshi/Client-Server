#pragma once

#include <string.h>
#include "NetworkManagerBase.h"

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
	virtual void processPacket(InputBitStream& inputStream, const SocketAddress& fromAddress) override;

private:
	NetworkManagerClient();
	void init(const SocketAddress& serverAddress, const std::string& name);

	SocketAddress serverAddress;
	NetworkClientState clientState;
	std::string name;
	int playerId;
};

