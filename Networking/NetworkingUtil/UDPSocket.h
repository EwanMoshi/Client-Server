#pragma once

class SocketAddress;

class UDPSocket {
public:
	~UDPSocket();

	int Bind(const SocketAddress& addressToBind);
	int SendTo(const void* bufferAddress, int length, const SocketAddress& addressTo);
	int ReceiveFrom(void* receiveBuffer, int maxLength, SocketAddress& addressFrom);

	int SetNonBlockingMode(bool enableNonBlockingMode);

private:
	friend class SocketUtil;

	UDPSocket(SOCKET newSocket) : socket(newSocket) { }

	SOCKET socket;
};