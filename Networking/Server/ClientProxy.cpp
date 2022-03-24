#include "pch.h"
#include "ClientProxy.h"

ClientProxy::ClientProxy(const SocketAddress& socketAddress, const std::string& name, int playerId) :
	socketAddress(socketAddress),
	name(name),
	playerId(playerId)
{
	
}
