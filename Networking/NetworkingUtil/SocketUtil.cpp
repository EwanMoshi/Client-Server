#include "pch.h"
#include "UDPSocket.h"

bool SocketUtil::init() {
#if _WIN32
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		ReportError("SocketUtil::init");
		return false;
	}
#endif
	return true;
}

void SocketUtil::cleanUp() {
#if _WIN32
	WSACleanup();
#endif
}

std::shared_ptr<UDPSocket> SocketUtil::CreateUDPSocket(SocketAddressFamily addressFamily) {
	// address family will either be AF_INET (IPv4) or AF_INET6 (IPv6)
	SOCKET sock = socket(addressFamily, SOCK_DGRAM, IPPROTO_UDP);

	if (sock == INVALID_SOCKET) {
		ReportError("SocketUtil::CreateUDPSocket: Failed to create socket");
		return nullptr;
	}

	return std::shared_ptr<UDPSocket>(new UDPSocket(sock));
}

void SocketUtil::ReportError(const char* error)
{
#if _WIN32
	LPVOID lpMsgBuf;
	DWORD errorNum = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);


	LOG("Error %s: %d- %s", error, errorNum, lpMsgBuf);
#else
	LOG("Error: %hs", inOperationDesc);
#endif
}

int SocketUtil::GetLastError()
{
#if _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}

void StringUtils::Log(const char* inFormat, ...) {
	//not thread safe...
	static char temp[4096];

	va_list args;
	va_start(args, inFormat);

#if _WIN32
	_vsnprintf_s(temp, 4096, 4096, inFormat, args);
#else
	vsnprintf(temp, 4096, inFormat, args);
#endif
	OutputDebugString(temp);
	OutputDebugString("\n");
}