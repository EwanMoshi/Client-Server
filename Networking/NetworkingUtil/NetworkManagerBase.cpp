#include "pch.h"
#include "NetworkManagerBase.h"
#include "SocketAddress.h"
#include "UDPSocket.h"
#include "../Client/Client.h"

float NetworkManagerBase::simulatedLatency_SEC = 0.0f;

float Math::GetRandomFloat() {
	static std::random_device randomDevice;
	static std::mt19937 gen(randomDevice());
	static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
	return dist(gen);
}


NetworkManagerBase::ReceivedPacket::ReceivedPacket(InputBitStream& inputBitStream, const SocketAddress& fromAddress, float receivedTime) :
	fromAddress(fromAddress),
	packetBuffer(inputBitStream),
	receivedTime(receivedTime)
{
}

NetworkManagerBase::NetworkManagerBase() {
	// temporarily here - calls WSAStartup
	SocketUtil::init();
}

NetworkManagerBase::~NetworkManagerBase() {
	// temporarily here - calls WSACleanup
	SocketUtil::cleanUp();
}

bool NetworkManagerBase::init(uint16_t port) {
	// std::shared_ptr<SocketAddress> serverAddress = SocketAddressFactory::createIPv4FromString("127.0.0.1:8989");

	SocketAddress ownAddress(INADDR_ANY, port);

	udpSocket = SocketUtil::CreateUDPSocket(INET);
	udpSocket->Bind(ownAddress);

	LOG("[NetworkManager::init]: binding socket at port %d", port);
	std::cout << "[NetworkManager::init]: binding socket at port " <<  port << std::endl;

	if (udpSocket == nullptr) {
		return false;
	}

	// set udp socket to non-blocking mode
	if (udpSocket->SetNonBlockingMode(true) != NO_ERROR) {
		return false;
	}

	//std::cout << "NetworkManager::init: Socket bound at port " << ownAddress.toString().c_str() << std::endl;
	return true;
}

void NetworkManagerBase::processIncomingPackets() {
	readIncomingPacketsIntoQueue();

	processQueuedPackets();
}

void NetworkManagerBase::readIncomingPacketsIntoQueue() {
	char packetBuffer[1500];
	int packetSize = sizeof(packetBuffer);

	InputBitStream inputStream(packetBuffer, packetSize * 8);
	SocketAddress fromAddress;

	int receivedPackedCount = 0;
	int totalReadByteCount = 0;

	// temp here
	int maxPacketsToReadPerFrame = 1;

	while (receivedPackedCount < maxPacketsToReadPerFrame) {
		int readByteCount = udpSocket->ReceiveFrom(packetBuffer, packetSize, fromAddress);

		if (readByteCount == 0) {
			break;
		}
		else if (readByteCount == WSAECONNRESET) {
			// handle disconnect
		}
		else if (readByteCount > 0) {
			inputStream.resetToCapacity(readByteCount);

			receivedPackedCount++;
			totalReadByteCount += readByteCount;
			
			auto randFloat = Math::GetRandomFloat();
			std::cout << randFloat << std::endl;

			// Note: can simulate lag/packet loss here by not inserting into queue
			if (randFloat >= getPacketLossChance()) {
				std::cout << "[NetworkManagerBase::readIncomingPacketsIntoQueue]: Placing packet into packet queue" << std::endl;
				std::cout << "[NetworkManagerBase::readIncomingPacketsIntoQueue]: --------------------------------- " << simulatedLatency_SEC  << std::endl;

				float receivedTime = Timing::instance.GetTimeAsFloat() + simulatedLatency_SEC;
				packetQueue.emplace(inputStream, fromAddress, receivedTime);
			}
			else {
				std::cout << "[NetworkManagerBase::readIncomingPacketsIntoQueue]: Simulating packet loss - Dropping packet " << std::endl;
			}
		}
	}
}

void NetworkManagerBase::processQueuedPackets() {
	while (!packetQueue.empty()) {
		ReceivedPacket& nextPacket = packetQueue.front();

		std::cout << "simulated time ===== " << Timing::instance.GetTimeAsFloat() << std::endl;
		std::cout << "received time ===== " << nextPacket.getReceivedTime() << std::endl;

		// simulate latency
		if (Timing::instance.GetTimeAsFloat() > nextPacket.getReceivedTime()) {
			processPacket(nextPacket.getPacketBuffer(), nextPacket.getFromAddress());
			packetQueue.pop();
		}
		else {
			std::cout << "[NetworkManagerBase::processQueuedPackets]: Simulating lag - will not process packet right away" << std::endl;
			break;
		}
	}
}

void NetworkManagerBase::sendPacket(const OutputBitStream& outputStream, const SocketAddress& fromAddress)
{
	std::cout << "[NetworkManagerBase::sendPacket]: Sending packet to address" << fromAddress.toString() << std::endl;

	int sentByteCount = udpSocket->SendTo(outputStream.getBufferPtr(), outputStream.getByteLength(), fromAddress);

	if (sentByteCount > 0) {
		
	}
}

float NetworkManagerBase::getPacketLossChance() {
	return 0.0f;
}

std::shared_ptr<GameObject> NetworkManagerBase::getGameObject(int networkId) const {
	auto gameObjectIterator = networkIdToGameObject.find(networkId);

	if (gameObjectIterator != networkIdToGameObject.end()) {
		return gameObjectIterator->second;
	}

	return nullptr;
}

void NetworkManagerBase::addNetworkIdToGameObjectMap(std::shared_ptr<GameObject> newGameObject) {
	networkIdToGameObject[newGameObject->getNetworkId()] = newGameObject;
}
