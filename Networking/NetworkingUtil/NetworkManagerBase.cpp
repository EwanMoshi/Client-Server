#include "pch.h"
#include "NetworkManagerBase.h"
#include "SocketAddress.h"
#include "UDPSocket.h"

NetworkManagerBase::NetworkManagerBase() {
	// temporarily here - calls WSAStartup
	SocketUtil::init();
}

NetworkManagerBase::~NetworkManagerBase() {
	// temporarily here - calls WSACleanup
	SocketUtil::cleanUp();
}

bool NetworkManagerBase::init(uint16_t port) {
	SocketAddress ownAddress(INADDR_ANY, port);

	udpSocket = SocketUtil::CreateUDPSocket(INET);
	udpSocket->Bind(ownAddress);

	LOG("NetworkManager::init: binding socket at port %d", port);

	if (udpSocket == nullptr) {
		return false;
	}

	// set udp socket to non-blocking mode
	if (udpSocket->SetNonBlockingMode(true) != NO_ERROR) {
		return false;
	}

	return true;
}

void NetworkManagerBase::sendPacket(const OutputBitStream& outputStream, const SocketAddress& fromAddress)
{
	int sentByteCount = udpSocket->SendTo(outputStream.getBufferPtr(), outputStream.getByteLength(), fromAddress);

	if (sentByteCount > 0) {
		//
	}
}