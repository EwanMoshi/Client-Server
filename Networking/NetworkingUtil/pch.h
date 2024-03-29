#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "Windows.h"
#include "WinSock2.h"
#include "Ws2tcpip.h"
typedef int socklen_t;

#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
typedef int SOCKET;
const int NO_ERROR = 0;
const int INVALID_SOCKET = -1;
const int WSAECONNRESET = ECONNRESET;
const int WSAEWOULDBLOCK = EAGAIN;
const int SOCKET_ERROR = -1;
#endif

#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <string>
#include <list>
#include <queue>
#include <deque>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include "SocketUtil.h"
#include "SocketAddressFactory.h"
#include "SocketAddress.h"
#include "Timing.h"
#include "NetworkBitStream.h"
#include "InFlightPacket.h"
#include "AckRange.h"
#include "TransmissionData.h"
#include "GameObjectRegistry.h"
#include "GameObject.h"
#include "BaseCharacter.h"
#include "World.h"
#include "glad/glad.h"
