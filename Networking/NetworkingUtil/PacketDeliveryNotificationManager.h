#pragma once

#include "InFlightPacket.h"


class OutputBitStream;
class InputBitStream;
class AckRange;

class PacketDeliveryNotificationManager {
public:
	PacketDeliveryNotificationManager(bool shouldSendAcks, bool shouldProcessAcks);
	~PacketDeliveryNotificationManager();

	void processTimedOutPackets();

	InFlightPacket* writeWelcomePacket(OutputBitStream& outputStream);
	bool readAndProcessWelcomePacket(InputBitStream& inputStream);

private:
	void handlePacketDeliverySuccess(const InFlightPacket& inFlightPacket);
	void handlePacketDeliveryFailure(const InFlightPacket& inFlightPacket);

	InFlightPacket* writeSequenceNumber(OutputBitStream& outputStream);
	bool processSequenceNumber(InputBitStream& inputStream);
	void processAcks(InputBitStream& inputStream);

	void addPendingAck(PacketSequenceNumber sequenceNumberToAck);
	void writeAckData(OutputBitStream& outputStream);

	PacketSequenceNumber nextSequenceNumber;
	PacketSequenceNumber nextExpectedSequenceNumber;
	bool shouldSendAcks; 
	bool shouldProcessAcks;

	std::deque<InFlightPacket> inFlightPackets;
	std::deque<AckRange> pendingAckRanges;

	const float packetAckTimeoutDelay = 3.0f;
};