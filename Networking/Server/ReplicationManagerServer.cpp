#include "pch.h"
#include "ReplicationManagerServer.h"

void ReplicationManagerServer::replicateCreate(int networkId) {
	networkIdtoReplicationCommand[networkId] = ReplicationCommand();
}

void ReplicationManagerServer::replicateDestroy(int networkId) {
	networkIdtoReplicationCommand[networkId].setDestroy();
}

void ReplicationManagerServer::write(OutputBitStream& outputStream) {
	for (auto& pair : networkIdtoReplicationCommand) {
		ReplicationCommand& repCommand = pair.second;

		int networkId = pair.first;
		outputStream.write(networkId);

		ReplicationAction action = repCommand.getAction();
		outputStream.write(action, 2);

		uint32_t writtenState = 0;
		// TODO: implement dirty state

		switch (action) {
		case ReplicationAction::RA_Create:
			writtenState = writeCreateAction(outputStream, networkId);
			break;
		case ReplicationAction::RA_Update:
			break;
		case ReplicationAction::RA_Destroy:
			break;
		}
	}
}

uint32_t ReplicationManagerServer::writeCreateAction(OutputBitStream& outputStream, int networkId, uint32_t dirtyState) {
	std::shared_ptr<GameObject> gameObject = NetworkManagerServer::Instance->getGameObject(networkId);

	outputStream.write(gameObject->getClassId());
	return gameObject->write(outputStream);

}

uint32_t ReplicationManagerServer::writeUpdateAction(OutputBitStream& outputStream, int networkId, uint32_t dirtyState) {
	return 0;
}

uint32_t ReplicationManagerServer::writeDestroyAction(OutputBitStream& outputStream, int networkId, uint32_t dirtyState) {
	return 0;
}