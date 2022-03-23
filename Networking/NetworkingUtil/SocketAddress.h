#pragma once

#include "pch.h"

class SocketAddress {
public:
	SocketAddress(uint32_t address, uint16_t port) {
		// AF_INET means use IPv4 address
		getAsSockAddrIn()->sin_family = AF_INET;

		// htonl converts 32 bit integer from host native byte-order to network's native byte-order
		getAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(address);

		// same as htonl but on 16 bit integer
		getAsSockAddrIn()->sin_port = htons(port);
	}

	SocketAddress(const sockaddr& sockAddr) {
		memcpy(&sockAddress, &sockAddr, sizeof(sockaddr));
	}

	SocketAddress() {
		getAsSockAddrIn()->sin_family = AF_INET;
		getIP4Ref() = INADDR_ANY;
		getAsSockAddrIn()->sin_port = 0;
	}

	sockaddr_in* getAsSockAddrIn() {
		return reinterpret_cast<sockaddr_in*>(&sockAddress);
	}

	const sockaddr_in* getAsSockAddrIn() const { 
		return reinterpret_cast<const sockaddr_in*>(&sockAddress);
	}

	size_t GetSize() const { return sizeof(sockaddr); }

private:
	friend class UDPSocket;

	sockaddr sockAddress;

#if _WIN32
	uint32_t& getIP4Ref() { 
		return *reinterpret_cast<uint32_t*>(&getAsSockAddrIn()->sin_addr.S_un.S_addr); 
	}

	const uint32_t& getIP4Ref() const { 
		return *reinterpret_cast<const uint32_t*>(&getAsSockAddrIn()->sin_addr.S_un.S_addr); 
	}
#else
	uint32_t& getIP4Ref() { 
		return getAsSockAddrIn()->sin_addr.s_addr; 
	}
	const uint32_t& getIP4Ref()	const { 
		return getAsSockAddrIn()->sin_addr.s_addr; 
	}
#endif
};