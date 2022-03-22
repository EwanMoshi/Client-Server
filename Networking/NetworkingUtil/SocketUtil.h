#pragma once

namespace StringUtils {
	void Log(const char* inFormat, ...);
}

class SocketUtil
{
public:
	static void ReportError(const char* error);
};

