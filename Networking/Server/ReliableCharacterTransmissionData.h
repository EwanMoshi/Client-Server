#pragma once

class PacketDeliveryNotificationManager;
class ReplicationManagerServer;

class ReliableCharacterTransmissionData : public TransmissionData {

public:
	ReliableCharacterTransmissionData(ReplicationManagerServer* replicationManager, const std::shared_ptr<ClientProxy>& clientProxy);

	virtual void addTransmission(int networkId, ReplicationAction replicationAction) override;

	virtual void handleDeliverySuccess(PacketDeliveryNotificationManager* packetDeliveryNotificationManager) const override;
	virtual void handleDeliveryFailure(PacketDeliveryNotificationManager* packetDeliveryNotificationManager) const override;

	class CharacterReplicationTransmission 
	{
		public:
			CharacterReplicationTransmission(int networkId, ReplicationAction replicationAction) :
				networkId(networkId), replicationAction(replicationAction) { }

			int networkId;
			ReplicationAction replicationAction;
	};

private:
	void handleCreateDeliverySuccess(int networkId) const;
	void handleDestroyDeliverySuccess(int networkId) const;

	void handleCreateDeliveryFailure(int networkId) const;
	void handleUpdateDeliveryFailure(int networkId, PacketDeliveryNotificationManager* packetDeliveryNotificationManager) const;
	void handleDestroyDeliveryFailure(int networkId) const;

	ReplicationManagerServer* replicationManager;
	std::vector<CharacterReplicationTransmission> transmissions;
	std::shared_ptr<ClientProxy> clientProxy;
};

