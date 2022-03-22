#pragma once
class SocketAddressFactory
{
public:
	static std::shared_ptr<SocketAddress> createIPv4FromString(const std::string& ipString);
};