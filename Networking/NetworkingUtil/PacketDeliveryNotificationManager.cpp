#include "pch.h"
#include "PacketDeliveryNotificationManager.h"

PacketDeliveryNotificationManager::PacketDeliveryNotificationManager(bool shouldSendAcks, bool shouldProcessAcks) :
	nextSequenceNumber(0), nextExpectedSequenceNumber(0), shouldSendAcks(shouldSendAcks), shouldProcessAcks(shouldProcessAcks) { }

PacketDeliveryNotificationManager::~PacketDeliveryNotificationManager() { }

void PacketDeliveryNotificationManager::processTimedOutPackets() {
	float timeoutTime = Timing::instance.GetTimeAsFloat() - packetAckTimeoutDelay;

	while (!inFlightPackets.empty()) {
		const InFlightPacket inFlightPacket = inFlightPackets.front();

		if (inFlightPacket.getTimeDispatched() < timeoutTime) {
			handlePacketDeliveryFailure(inFlightPacket);
			inFlightPackets.pop_front();
		}
		else {
			// because inFlightsPackets are ordered, we can break as soon as we hit the first packet that hasn't timed out
			break;
		}
	}
}

void PacketDeliveryNotificationManager::handlePacketDeliverySuccess(const InFlightPacket& inFlightPacket) {
	inFlightPacket.handleDeliverySuccess(this);

	// can incremenet a counter here to keep track of sent packets
}

void PacketDeliveryNotificationManager::handlePacketDeliveryFailure(const InFlightPacket& inFlightPacket) {
	inFlightPacket.handleDeliveryFailure(this);

	// can incremenet a counter here to keep track of dropped packets
}

InFlightPacket* PacketDeliveryNotificationManager::writeSequenceNumber(OutputBitStream& outputStream) {
	// get the current sequenceNumber and increment in preparation for the next packet
	PacketSequenceNumber currentSequenceNumber = nextSequenceNumber++;

	outputStream.write(currentSequenceNumber);

	if (shouldProcessAcks) {
		// Note: This constructs the InFlightPacket object with the currentSequenceNumber
		inFlightPackets.emplace_back(currentSequenceNumber);

		return &inFlightPackets.back();
	}
	
	return nullptr;
}

bool PacketDeliveryNotificationManager::processSequenceNumber(InputBitStream& inputStream) {
	PacketSequenceNumber sequenceNumber;

	inputStream.read(sequenceNumber);

	// this handles the case when sequence number is == expected and when it's > expected
	// Note: if the sequenceNumber is > than what we expected, it means we missed some packets
	if (sequenceNumber >= nextExpectedSequenceNumber) {
		nextExpectedSequenceNumber = sequenceNumber + 1;

		if (shouldSendAcks) {
			addPendingAck(sequenceNumber);
		}

		return true;
	}
	else if (sequenceNumber < nextExpectedSequenceNumber) {
		// silently drop old packets 
		// NOTE: wrapping around the PacketSequenceNumber can trigger this
		return false;
	}

	return false;
}

void PacketDeliveryNotificationManager::writeAckData(OutputBitStream& outputStream) {
	bool hasAcks = !pendingAckRanges.empty();

	outputStream.write(hasAcks);

	if (hasAcks) {
		pendingAckRanges.front().write(outputStream);
		pendingAckRanges.pop_front();
	}
}

void PacketDeliveryNotificationManager::processAcks(InputBitStream& inputStream) {
	bool hasAcks;

	inputStream.read(hasAcks);

	// if the incoming packet has acks
	if (hasAcks) {
		AckRange ackRange;
		ackRange.read(inputStream);

		PacketSequenceNumber nextAckdSequenceNumber = ackRange.getStart();
		uint32_t onePastAckdSequenceNumber = nextAckdSequenceNumber + ackRange.getCount();

		while (nextAckdSequenceNumber < onePastAckdSequenceNumber && !inFlightPackets.empty()) {
			const InFlightPacket& inFlightPacket = inFlightPackets.front();
			PacketSequenceNumber inFlightPacketSequenceNumber = inFlightPacket.getSequenceNubmer();

			if (inFlightPacketSequenceNumber < nextAckdSequenceNumber) {
				InFlightPacket inFlightPacketCopy = inFlightPacket;
				inFlightPackets.pop_front();
				handlePacketDeliveryFailure(inFlightPacketCopy);
			}
			else if (inFlightPacketSequenceNumber == nextAckdSequenceNumber) {
				handlePacketDeliverySuccess(inFlightPacket);
				inFlightPackets.pop_front();
				nextAckdSequenceNumber++;
			}
			else if (inFlightPacketSequenceNumber > nextAckdSequenceNumber) {
				// the inFlightPacket's sequnce number is within the range. This can happen if 
				// a packet was marked as dropped due to a late ack
				nextAckdSequenceNumber++;
			}
		}

	}
}

void PacketDeliveryNotificationManager::addPendingAck(PacketSequenceNumber sequenceNumberToAck) {
	// we only need to add a new AckRange is there is a break in the AckRange sequence (i.e. they're no longer consecutive)
	if (pendingAckRanges.size() == 0 || !pendingAckRanges.back().extend(sequenceNumberToAck)) {
		pendingAckRanges.emplace_back(sequenceNumberToAck);
	}
}

