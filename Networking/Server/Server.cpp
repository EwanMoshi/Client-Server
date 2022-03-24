// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include "Server.h"

Server* Server::Instance = nullptr;

bool Server::staticInit() {
	if (Instance == nullptr) {
		Instance = new Server();
	}

	return true;
}

Server::Server() {
	initNetworkManager();
}

bool Server::initNetworkManager() {
	std::string portString = "8989";
	uint16_t port = stoi(portString);

	return NetworkManagerServer::staticInit(port);
}

int Server::run() {
	while (true) {
		// TODO: turn this into a getInstance function that initializes the Instance if it's nullptr
		NetworkManagerServer::Instance->processIncomingPackets();

		// NetworkManagerServer::Instance->checkForDisconnects();

		// NetworkManagerServer::Instance->sendOutgoingPackets();
	}

	return 0;
}

int main()
{
	if (Server::staticInit()) {
		return Server::Instance->run();
	}
	else {
		return -1;
	}

    // create UDP socket using AF_INET (IPv4)
  //  auto udpSocket = SocketUtil::CreateUDPSocket(INET);
  //  udpSocket->SetNonBlockingMode(true);

  //  while (true) {
		//char data[1500];

		//std::shared_ptr<SocketAddress> serverAddress = SocketAddressFactory::createIPv4FromString("127.0.0.1:8989");

		//int bytesReceived = udpSocket->ReceiveFrom(data, sizeof(data), *serverAddress);

		//if (bytesReceived > 0) {

		//}
  //  }
}
