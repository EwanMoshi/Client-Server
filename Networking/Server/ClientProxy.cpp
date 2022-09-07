#include "pch.h"
#include "ClientProxy.h"

ClientProxy::ClientProxy(const SocketAddress& socketAddress, const std::string& name, int playerId) :
	socketAddress(socketAddress),
	name(name),
	playerId(playerId),
	packetDeliveryNotificationManager(false, true), // Note: We set shouldSendAcks to false because this is on the server
	stateDirty(true) {
	
}

bool ClientProxy::isStateDirty() const {
	return stateDirty;
}

void ClientProxy::setStateDirty(bool isDirty) {
	stateDirty = isDirty;
}

ReplicationManagerServer& ClientProxy::getReplicationManagerServer() {
	return replicationManagerServer;
}

PacketDeliveryNotificationManager& ClientProxy::getPacketDeliveryNotificationManager() {
	return packetDeliveryNotificationManager;
}

MoveList& ClientProxy::getUnprocessedMoveList() {
	return unprocessedMoves;
}