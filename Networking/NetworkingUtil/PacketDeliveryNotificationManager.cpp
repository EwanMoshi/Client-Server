#include "pch.h"
#include "PacketDeliveryNotificationManager.h"

PacketDeliveryNotificationManager::PacketDeliveryNotificationManager(bool shouldSendAcks, bool shouldProcessAcks) :
	nextSequenceNumber(0), nextExpectedSequenceNumber(0), shouldSendAcks(shouldSendAcks), shouldProcessAcks(shouldProcessAcks) { 

}

PacketDeliveryNotificationManager::~PacketDeliveryNotificationManager() { }

void PacketDeliveryNotificationManager::processTimedOutPackets() {
	float timeoutTime = Timing::instance.GetTimeAsFloat() - packetAckTimeoutDelay_Sec;

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
	std::cout << "[PacketDeliveryNotificationManager::handlePacketDeliveryFailure]: Packet with sequence number " << inFlightPacket.getSequenceNubmer() << " successfully sent" << std::endl;

	inFlightPacket.handleDeliverySuccess(this);

	// can incremenet a counter here to keep track of sent packets
}

void PacketDeliveryNotificationManager::handlePacketDeliveryFailure(const InFlightPacket& inFlightPacket) {
	std::cout << "[PacketDeliveryNotificationManager::handlePacketDeliveryFailure]: Packet with sequence number " << inFlightPacket.getSequenceNubmer() << " detected as dropped" << std::endl;

	inFlightPacket.handleDeliveryFailure(this);

	// can incremenet a counter here to keep track of dropped packets
}

bool PacketDeliveryNotificationManager::readAndProcessWelcomePacket(InputBitStream& inputStream) {
	bool shouldProcessPacket = processSequenceNumber(inputStream);

	if (shouldProcessAcks) {
		processAcks(inputStream);
	}

	return shouldProcessPacket;
}

InFlightPacket* PacketDeliveryNotificationManager::writeWelcomePacket(OutputBitStream& outputStream) {
	InFlightPacket* newInFlightPacket = writeSequenceNumber(outputStream);

	if (shouldSendAcks) {
		writeAckData(outputStream);
	}

	return newInFlightPacket;
}

InFlightPacket* PacketDeliveryNotificationManager::writeStatePacket(OutputBitStream& statePacket) {
	InFlightPacket* inFlightPacket = writeSequenceNumber(statePacket);

	if (shouldSendAcks) {
		writeAckData(statePacket);
	}
	
	return inFlightPacket;
}

bool PacketDeliveryNotificationManager::readAndProcessStatePacket(InputBitStream& inputStream) {
	bool result = processSequenceNumber(inputStream);

	if (shouldProcessAcks) {
		processAcks(inputStream);
	}

	return result;
}

InFlightPacket* PacketDeliveryNotificationManager::writeSequenceNumber(OutputBitStream& outputStream) {
	// get the current sequenceNumber and increment in preparation for the next packet
	PacketSequenceNumber currentSequenceNumber = nextSequenceNumber++;

	outputStream.write(currentSequenceNumber);

	std::cout << " -------    WRITING sequenceNumber " << currentSequenceNumber << std::endl;

	std::cout << "[PacketDeliveryNotificationManager::writeSequenceNumber]: Writing sequence number " << currentSequenceNumber << " into the packet" << std::endl;

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
	std::cout << " -------    READING  sequenceNumber " << sequenceNumber << std::endl;

	std::cout << "[PacketDeliveryNotificationManager::processSequenceNumber]: Reading sequence number " << sequenceNumber << " from the packet" << std::endl;

	// this handles the case when sequence number is == expected and when it's > expected
	// Note: if the sequenceNumber is > than what we expected, it means we missed some packets
	if (sequenceNumber >= nextExpectedSequenceNumber) {
		nextExpectedSequenceNumber = sequenceNumber + 1;

		std::cout << "[PacketDeliveryNotificationManager::processSequenceNumber]: Next expected sequence number is " << nextExpectedSequenceNumber << std::endl;
		
		if (shouldSendAcks) {
			std::cout << "[PacketDeliveryNotificationManager::processSequenceNumber]: adding pending ack into pendingAckRanges" << std::endl;
			addPendingAck(sequenceNumber);
		}

		return true;
	}
	else if (sequenceNumber < nextExpectedSequenceNumber) {
		// silently drop old packets 
		// NOTE: wrapping around the PacketSequenceNumber can trigger this
		std::cout << "[PacketDeliveryNotificationManager::processSequenceNumber]: sequence number " << sequenceNumber << " is < " << nextExpectedSequenceNumber << ". Silently dropping old packet" << std::endl;
		return false;
	}

	return false;
}

void PacketDeliveryNotificationManager::writeAckData(OutputBitStream& outputStream) {
	bool hasAcks = !pendingAckRanges.empty();

	outputStream.write(hasAcks);

	std::cout << " -------    WRITING hasAcks " << hasAcks << std::endl;

	if (hasAcks) {
		auto ackRange = pendingAckRanges.front();
		ackRange.write(outputStream);

		auto rangeTo = ackRange.getStart() + ackRange.getCount();
		std::cout << "[PacketDeliveryNotificationManager::writeAckData]: Writing AckRange " << ackRange.getStart() << " to " << rangeTo << std::endl;

		pendingAckRanges.pop_front();
	}
	else {
		std::cout << "[PacketDeliveryNotificationManager::writeAckData]: pendingAckRanges is empty. Not writing any AckRange into packet. " << std::endl;
	}
}

void PacketDeliveryNotificationManager::processAcks(InputBitStream& inputStream) {
	bool hasAcks;

	inputStream.read(hasAcks);
	std::cout << " -------    READING  hasAcks " << hasAcks << std::endl;

	// if the incoming packet has acks
	if (hasAcks) {
		std::cout << "[PacketDeliveryNotificationManager::processAcks]: Ack found in packet" << std::endl;

		AckRange ackRange;
		ackRange.read(inputStream);

		auto rangeTo = ackRange.getStart() + ackRange.getCount();
		std::cout << "[PacketDeliveryNotificationManager::processAcks]: Reading AckRange from " << ackRange.getStart() << "to" << rangeTo << std::endl;

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
				std::cout << "[PacketDeliveryNotificationManager::processAcks]: inFlightPacketSequenceNumber > nextAckdSequenceNumber" << std::endl;
				nextAckdSequenceNumber++;
			}
		}
	}
	else {
		std::cout << "[PacketDeliveryNotificationManager::processAcks]: Ack not found in packet" << std::endl;
	}
}

void PacketDeliveryNotificationManager::addPendingAck(PacketSequenceNumber sequenceNumberToAck) {
	// we only need to add a new AckRange if there is a break in the AckRange sequence (i.e. they're no longer consecutive)
	if (pendingAckRanges.size() == 0 || !pendingAckRanges.back().extend(sequenceNumberToAck)) {
		pendingAckRanges.emplace_back(sequenceNumberToAck);
	}
}

const std::deque<InFlightPacket>& PacketDeliveryNotificationManager::getInFlightPackets() const {
	return inFlightPackets;
}

bool PacketDeliveryNotificationManager::needsToAck() const {
	return pendingAckRanges.size() != 0;
}