#include "pch.h"

std::shared_ptr<SocketAddress> SocketAddressFactory::createIPv4FromString(const string& ipString) {
	sockaddr_in newAddr;

	// use IPv4
	newAddr.sin_family = AF_INET;

	newAddr.sin_port = htons(8989);

	// InetPton converts string to binary IP address
	InetPton(AF_INET, "127.0.0.1", &newAddr.sin_addr);

	// cast the sockaddr_in newAddr to sockaddr
	auto socketAddress = std::make_shared<SocketAddress>((struct sockaddr *) newAddr);

	return socketAddress;
}
