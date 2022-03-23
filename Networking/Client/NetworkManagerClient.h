#pragma once

#include <string>
#include "NetworkManagerBase.h"
#include "SocketAddress.h"

class NetworkManagerClient : public NetworkManagerBase {
	enum NetworkClientState {
		NCS_Uninitialized,
		NCS_SayingHello,
		NCS_Welcomed
	};

public:
	static NetworkManagerClient* Instance;

	static void	staticInit(const SocketAddress& serverAddress, const std::string& name);

	void sendOutgoingPacket();

private:
	NetworkManagerClient();
	void init(const SocketAddress& serverAddress, const std::string& name);

	SocketAddress serverAddress;
	NetworkClientState clientState;
	std::string name;
};

