#pragma once

#include "SocketAddress.h"
#include "NetworkBitStream.h"
#include <memory>

namespace Math {
	float GetRandomFloat();
}

class NetworkManagerBase {
public:
	NetworkManagerBase();
	virtual ~NetworkManagerBase();

	bool init(uint16_t port);

	void processIncomingPackets();
	void sendPacket(const OutputBitStream& outputStream, const SocketAddress& fromAddress);

	virtual void processPacket(InputBitStream& inputStream, const SocketAddress& fromAddress) = 0;

	void addNetworkIdToGameObjectMap(std::shared_ptr<GameObject> newGameObject);

	std::shared_ptr<GameObject> getGameObject(int networkId) const;

	static const uint32_t helloMessage = 'HELO';
	static const uint32_t welcomeMessage = 'WLCM';
	static const uint32_t stateMessage = 'STAT';
	static const uint32_t inputMessage = 'INPT';

	static float simulatedLatency_SEC;

protected:
	std::unordered_map<int, std::shared_ptr<GameObject>> networkIdToGameObject;

	virtual float getPacketLossChance();

private:
	class ReceivedPacket {
	public:
		ReceivedPacket(InputBitStream& inputMemoryBitStream, const SocketAddress& address, float receivedTime);

		const SocketAddress& getFromAddress() const {
			return fromAddress;
		}

		InputBitStream& getPacketBuffer() { 
			return packetBuffer;
		}

		float getReceivedTime() const {
			return receivedTime;
		}

	private:
		InputBitStream packetBuffer;
		SocketAddress fromAddress;
		float receivedTime;
	};

	void readIncomingPacketsIntoQueue();
	void processQueuedPackets();

	std::shared_ptr<class UDPSocket> udpSocket;

	std::queue<ReceivedPacket, std::list<ReceivedPacket>> packetQueue;

};

