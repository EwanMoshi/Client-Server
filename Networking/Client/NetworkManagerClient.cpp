#include "NetworkManagerClient.h"
#include "NetworkBitStream.h"

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

void NetworkManagerClient::sendOutgoingPacket() {
	switch (clientState)
	{
		case NCS_SayingHello: {
			OutputBitStream helloPacket;

			helloPacket.write(helloMessage);
			helloPacket.write(name);

			sendPacket(helloPacket, serverAddress);

			break;
		}
		case NCS_Welcomed: {
			// send something else
			int a = 5;
			break;
		}
	}
}