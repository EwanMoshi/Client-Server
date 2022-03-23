#pragma once

class UDPSocket;

#define LOG( ... ) StringUtils::Log( __VA_ARGS__ );

namespace StringUtils {
	void Log(const char* inFormat, ...);
}

enum SocketAddressFamily
{
	INET = AF_INET,
	INET6 = AF_INET6
};

class SocketUtil
{
public:	
	static std::shared_ptr<UDPSocket> CreateUDPSocket(SocketAddressFamily addressFamily);

	static void ReportError(const char* error);
	static int GetLastError();
};

