#pragma once

#include "SocketAddress.h"
#include "NetworkBitStream.h"
#include <memory>

class NetworkManagerBase {
public:
	NetworkManagerBase();
	virtual ~NetworkManagerBase();

	bool init(uint16_t port);

	void sendPacket(const OutputBitStream& outputStream, const SocketAddress& fromAddress);


	static const uint32_t helloMessage = 'HELO';
	static const uint32_t welcomeMessage = 'WLCM';
	static const uint32_t stateMessage = 'STAT';
	static const uint32_t inputMessage = 'INPT';
private:
	std::shared_ptr<class UDPSocket> udpSocket;
};

