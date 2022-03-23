#pragma once

#include "NetworkManagerBase.h"

class NetworkManagerServer : public NetworkManagerBase {

public:
	static NetworkManagerServer* Instance;

	static bool init(uint16_t port);

private:
	// constructor is private - create instance by calling static init function
	NetworkManagerServer();
};

