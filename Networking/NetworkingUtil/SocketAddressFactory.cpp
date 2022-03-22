#include "pch.h"
#include "SocketAddressFactory.h"
#include "SocketAddress.h"

std::shared_ptr<SocketAddress> SocketAddressFactory::createIPv4FromString(const std::string& ipString) {
	auto pos = ipString.find_last_of(':');

	std::string host, port;

	if (pos != std::string::npos)
	{
		host = ipString.substr(0, pos);
		port = ipString.substr(pos + 1);
	}
	else
	{
		host = ipString;
		port = "0";
	}

	addrinfo hint;
	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;

	addrinfo* result;
	int error = getaddrinfo(host.c_str(), port.c_str(), &hint, &result);

	if (error != 0 && result != nullptr)
	{
		SocketUtil::ReportError("SocketAddressFactory::CreateIPv4FromString: Failed to getaddrinfo");
		return nullptr;
	}

	while (!result->ai_addr && result->ai_next)
	{
		result = result->ai_next;
	}

	if (!result->ai_addr)
	{
		return nullptr;
	}

	std::shared_ptr<SocketAddress> socketAddress = std::make_shared<SocketAddress>(*result->ai_addr);

	freeaddrinfo(result);

	return socketAddress;
	
	
	//sockaddr_in newAddr;

	//// use IPv4
	//newAddr.sin_family = AF_INET;

	//newAddr.sin_port = htons(8989);

	//// InetPton converts string to binary IP address
	//InetPton(AF_INET, (PCWSTR)"127.0.0.1", &newAddr.sin_addr);

	//// cast the sockaddr_in newAddr to sockaddr
	//std::shared_ptr<SocketAddress> socketAddress = std::make_shared<SocketAddress>((struct sockaddr*) &newAddr);

	//return socketAddress;
}
