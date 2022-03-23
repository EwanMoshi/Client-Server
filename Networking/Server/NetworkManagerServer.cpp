#include "pch.h"
#include "NetworkManagerServer.h"

NetworkManagerServer* NetworkManagerServer::Instance = nullptr;

NetworkManagerServer::NetworkManagerServer() { }

bool NetworkManagerServer::staticInit(uint16_t inPort) {
	if (Instance == nullptr) {
		Instance = new NetworkManagerServer();

		// call parent init
		return Instance->init(inPort);
	}

	return true;
}
