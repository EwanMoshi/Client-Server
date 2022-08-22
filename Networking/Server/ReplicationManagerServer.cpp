#include "pch.h"
#include "ReplicationManagerServer.h"

void ReplicationManagerServer::replicateCreate(int networkId) {
	networkIdtoReplicationCommand[networkId] = ReplicationCommand();
}

void ReplicationManagerServer::replicateDestroy(int networkId) {
	networkIdtoReplicationCommand[networkId].setDestroy();
}

void ReplicationManagerServer::write(OutputBitStream& outputStream) {

}
