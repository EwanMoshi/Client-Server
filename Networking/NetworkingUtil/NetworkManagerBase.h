#pragma once

#include <memory>

class NetworkManagerBase {
public:
	NetworkManagerBase();
	~NetworkManagerBase();

	bool init(uint16_t port);

private:
	std::shared_ptr<class UDPSocket> udpSocket;
};

