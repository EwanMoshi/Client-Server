#include "pch.h"
#include "ReliableCharacterTransmissionData.h"

ReliableCharacterTransmissionData::ReliableCharacterTransmissionData(ReplicationManagerServer* replicationManagerServer, const std::shared_ptr<ClientProxy>& clientProxy) :
	replicationManager(replicationManagerServer), clientProxy(clientProxy) {

}

void ReliableCharacterTransmissionData::addTransmission(int networkId, ReplicationAction replicationAction) {
	transmissions.emplace_back(networkId, replicationAction);
}

void ReliableCharacterTransmissionData::handleDeliverySuccess(PacketDeliveryNotificationManager* packetDeliveryNotificationManager) const {
	for (const CharacterReplicationTransmission& charReplicationTransmission : transmissions) {
		int networkId = charReplicationTransmission.networkId;

		switch (charReplicationTransmission.replicationAction) {
			case ReplicationAction::RA_Create:
				handleCreateDeliverySuccess(networkId);
				break;
			case ReplicationAction::RA_Destroy:
				handleDestroyDeliverySuccess(networkId);
				break;
		}
	}
}

void ReliableCharacterTransmissionData::handleDeliveryFailure(PacketDeliveryNotificationManager* packetDeliveryNotificationManager) const {
	for (const CharacterReplicationTransmission& charReplicationTransmission: transmissions) {
		int networkId = charReplicationTransmission.networkId;

		switch (charReplicationTransmission.replicationAction) {
			case ReplicationAction::RA_Create:
				handleCreateDeliveryFailure(networkId);
				break;
			case ReplicationAction::RA_Update:
				handleUpdateDeliveryFailure(networkId, packetDeliveryNotificationManager);
				break;
			case ReplicationAction::RA_Destroy:
				handleDestroyDeliveryFailure(networkId);
				break;
		}
	}
}

void ReliableCharacterTransmissionData::handleCreateDeliverySuccess(int networkId) const {
	std::cout << "[ReliableCharacterTransmissionData::handleCreateDeliverySuccess]: Create Character Transmission successfull sent" << std::endl;

	replicationManager->handleCreateAckd(networkId);
}

void ReliableCharacterTransmissionData::handleDestroyDeliverySuccess(int networkId) const {
	std::cout << "[ReliableCharacterTransmissionData::handleCreateDeliverySuccess]: Destroy Character Transmission successfull sent" << std::endl;
	// NOT IMPLEMENTED YET
}

void ReliableCharacterTransmissionData::handleCreateDeliveryFailure(int networkId) const {
	std::cout << "[ReliableCharacterTransmissionData::handleCreateDeliveryFailure]: Create Character Transmission failed to send" << std::endl;

	// resend create command on failure
	std::shared_ptr<GameObject> gameObject = NetworkManagerServer::Instance->getGameObject(networkId);

	if (gameObject) {
		replicationManager->replicateCreate(networkId);
	}

}

void ReliableCharacterTransmissionData::handleUpdateDeliveryFailure(int networkId, PacketDeliveryNotificationManager* packetDeliveryNotificationManager) const {
	std::cout << "[ReliableCharacterTransmissionData::handleUpdateDeliveryFailure]: Update Character Transmission failed to send" << std::endl;

	if (NetworkManagerServer::Instance->getGameObject(networkId)) {
		for (const auto& inflightPacket : packetDeliveryNotificationManager->getInFlightPackets()) {
			
			std::shared_ptr<ReliableCharacterTransmissionData> reliableCharacterTransmissionData = 
				std::static_pointer_cast<ReliableCharacterTransmissionData>(inflightPacket.getTransmissionData(NetworkManagerServer::Instance->stateMessage));

			// if there already exists a packet for this networkId then don't bother resending because we know another update is in flight
			for (const CharacterReplicationTransmission& characterReplicationTransmission : reliableCharacterTransmissionData->transmissions) {
				if (characterReplicationTransmission.networkId == networkId) {
					std::cout << "[ReliableCharacterTransmissionData::handleUpdateDeliveryFailure]: An existing packet found in flight, will not attempt to resend." << std::endl;
					return;
				}
			}
		}

		std::cout << "[ReliableCharacterTransmissionData::handleUpdateDeliveryFailure]: Setting client proxy state to dirty. Attempting to resend packet." << std::endl;
		clientProxy->setStateDirty(true);
	}
}

void ReliableCharacterTransmissionData::handleDestroyDeliveryFailure(int networkId) const {
	std::cout << "[ReliableCharacterTransmissionData::handleDestroyDeliveryFailure]: Destroy Character Transmission failed to send" << std::endl;
	// NOT IMPLEMENTED YET
}