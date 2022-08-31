#include "pch.h"
#include "ReplicationManagerClient.h"

void ReplicationManagerClient::read(InputBitStream& inputStream) {
	while (inputStream.getRemainingBitCount() >= 32) {
		int networkId;
		inputStream.read(networkId);

		ReplicationAction action;
		inputStream.read(action, 2);

		switch (action) {
		case ReplicationAction::RA_Create:
			std::cout << "[ReplicationManagerClient::read] RA_Create" << std::endl;
			readAndPerformCreateAction(inputStream, networkId);
			break;
		case ReplicationAction::RA_Update:
			std::cout << "[ReplicationManagerClient::read] RA_Update" << std::endl;
			readAndPerformUpdateAction(inputStream, networkId);
			break;
		case ReplicationAction::RA_Destroy:
			std::cout << "[ReplicationManagerClient::read] RA_Destroy" << std::endl;
			break;
		}
	}
}


void ReplicationManagerClient::readAndPerformCreateAction(InputBitStream& inputStream, int networkId) {
	uint32_t classId;
	inputStream.read(classId);

	// if the ack for object creation from Client->Server was dropped, we might receive another create action from the server
	// so it's possible that this object already exists in that case
	std::shared_ptr<GameObject> gameObject = NetworkManagerClient::Instance->getGameObject(networkId);
	if (!gameObject) {
		gameObject = GameObjectRegistry::instance->createGameObject(classId);
		gameObject->setNetworkId(networkId);
		NetworkManagerClient::Instance->addNetworkIdToGameObjectMap(gameObject);
	}

	gameObject->read(inputStream);

}

void ReplicationManagerClient::readAndPerformUpdateAction(InputBitStream& inputStream, int networkId) {
	std::shared_ptr<GameObject> gameObject = NetworkManagerClient::Instance->getGameObject(networkId);
	gameObject->read(inputStream);
}

void ReplicationManagerClient::readAndPerformDestroyAction(InputBitStream& inputStream, int networkId) {

}