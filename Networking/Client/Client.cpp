#include "pch.h"
#include "Client.h"

Client* Client::Instance = nullptr;

bool Client::staticInit() {
	if (Instance == nullptr) {
		Instance = new Client();
	}

	return true;
}

Client::Client() {
	SocketUtil::init();

	std::shared_ptr<SocketAddress> serverAddress = SocketAddressFactory::createIPv4FromString("127.0.0.1:8989");

	// name should be unique
	NetworkManagerClient::staticInit(*serverAddress, "client");
}
	
int Client::mainGameLoop() {
	while (true) {
		NetworkManagerClient::Instance->processIncomingPackets();
		NetworkManagerClient::Instance->sendOutgoingPackets();
	}

	return 0;
}

int main()
{
	if (Client::staticInit()) {
		return Client::Instance->mainGameLoop();
	}
	else {
		return -1;
	}
}
