#pragma once


class SocketAddress {
public:
	SocketAddress(uint32_t address, uint16_t port) {
		// AF_INET means use IPv4 address
		GetAsSockAddrIn()->sin_family = AF_INET;

		// htonl converts 32 bit integer from host native byte-order to network's native byte-order
		GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(address);

		// same as htonl but on 16 bit integer
		GetAsSockAddrIn()->sin_port = htons(port);
	}

	SocketAddress(const sockaddr& sockAddr) {
		memcpy(&sockAddress, &sockAddr, sizeof(sockaddr));
	}

	sockaddr_in* GetAsSockAddrIn() {
		return reinterpret_cast<sockaddr_in*>(&sockAddress);
	}

	const sockaddr_in* GetAsSockAddrIn() const { 
		return reinterpret_cast<const sockaddr_in*>(&sockAddress);
	}

	size_t GetSize() const { return sizeof(sockaddr); }

private:
	sockaddr sockAddress;

	sockaddr_in* getAsSockAddrIn() {
		return reinterpret_cast<sockaddr_in*>(&sockAddress);
	}
};