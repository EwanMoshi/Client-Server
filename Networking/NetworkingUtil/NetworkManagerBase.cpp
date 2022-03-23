#include "pch.h"
#include "NetworkManagerBase.h"
#include "SocketAddress.h"
#include "UDPSocket.h"

NetworkManagerBase::NetworkManagerBase() {

}

NetworkManagerBase::~NetworkManagerBase() {

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