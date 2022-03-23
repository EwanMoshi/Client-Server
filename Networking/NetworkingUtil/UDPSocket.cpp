#include "pch.h"
#include "UDPSocket.h"
#include "SocketAddress.h"
#include "SocketUtil.h"

UDPSocket::~UDPSocket() {
#if _WIN32
	closesocket(socket);
#else
	close(socket);
#endif
}

int UDPSocket::Bind(const SocketAddress& addressToBind) {
	int result = bind(socket, &addressToBind.sockAddress, addressToBind.GetSize());
	if (result == 0) {
		return NO_ERROR;
	}

	SocketUtil::ReportError("UDPSocket::Bind: Failed to bind socket");
	return SocketUtil::GetLastError();
}

int UDPSocket::SendTo(const void* bufferAddress, int length, const SocketAddress& addressTo) {
	int bytesSent = sendto(socket, static_cast<const char*>(bufferAddress), length, 0, &addressTo.sockAddress, addressTo.GetSize());

	if (bytesSent <= 0) {
		SocketUtil::ReportError("UDPSocket::SendTo: Failed to send data");
		return SocketUtil::GetLastError();
	}

	return bytesSent;
}

int UDPSocket::ReceiveFrom(void* receiveBuffer, int maxLength, SocketAddress& addressFrom) {
	socklen_t fromLength = addressFrom.GetSize();

	int bytesRead = recvfrom(socket, static_cast<char*>(receiveBuffer), maxLength, 0, &addressFrom.sockAddress, &fromLength);

	if (bytesRead >= 0) {
		return bytesRead;
	}

	int error = SocketUtil::GetLastError();

	if (error == WSAEWOULDBLOCK) {
		return 0;
	}
	else if (error == WSAECONNRESET) {
		SocketUtil::ReportError("UDPSocket::ReceiveFrom: Connection reset");
		return WSAECONNRESET;
	}
	else {
		SocketUtil::ReportError("UDPSocket::ReceiveFrom: Failed");
		return error;
	}
}

int UDPSocket::SetNonBlockingMode(bool enableNonBlockingMode) {
#if _WIN32
	u_long arg = enableNonBlockingMode ? 1 : 0;
	int result = ioctlsocket(socket, FIONBIO, &arg);
#else
	int flags = fcntl(socket, F_GETFL, 0);
	flags = enableNonBlockingMode ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
	int result = fcntl(socket, F_SETFL, flags);
#endif

	if (result == SOCKET_ERROR)
	{
		SocketUtil::ReportError("UDPSocket::SetNonBlockingMode");
		return SocketUtil::GetLastError();
	}

	return NO_ERROR;
}
