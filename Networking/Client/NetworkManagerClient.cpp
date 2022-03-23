#include "NetworkManagerClient.h"

NetworkManagerClient* NetworkManagerClient::Instance = nullptr;

NetworkManagerClient::NetworkManagerClient() : clientState(NCS_Uninitialized) { }

void NetworkManagerClient::staticInit(const SocketAddress& serverAddress, const std::string& name) {
	if (Instance == nullptr) {
		Instance = new NetworkManagerClient();

		// call parent init
		return Instance->init(serverAddress, name);
	}
}

void NetworkManagerClient::init(const SocketAddress& serverAddress, const std::string& name)
{
	NetworkManagerBase::init(0);

	this->serverAddress = serverAddress;
	clientState = NCS_SayingHello;
	// timeOfLastHello = 0.f;
	this->name = name;
}