#include "pch.h"
#include "AckRange.h"

AckRange::AckRange() : start(0), count(0) {}

AckRange::AckRange(PacketSequenceNumber start) : start(start), count(1) {}

void AckRange::write(OutputBitStream& outputStream) const {
	outputStream.write(start);
	std::cout << " -------    WRITING ackStart " << start << std::endl;

	bool hasCount = count > 1;

	outputStream.write(hasCount);
	std::cout << " -------    WRITING hasCount " << hasCount << std::endl;

	if (hasCount) {
		// max ack is 255
		uint32_t countMinusOne = count - 1;
		uint8_t countToAck = countMinusOne > 255 ? 255 : static_cast<uint8_t>(countMinusOne);
		outputStream.write(countToAck);
		std::cout << " -------    WRITING countToAck " << countToAck << std::endl;
	}
}

void AckRange::read(InputBitStream& inputStream) {
	inputStream.read(start);
	std::cout << " -------    READING ackStart " << start << std::endl;

	bool hasCount;
	inputStream.read(hasCount);
	std::cout << " -------    READING  hasCount " << hasCount << std::endl;

	if (hasCount) {
		uint8_t countMinusOne;
		inputStream.read(countMinusOne);
		std::cout << " -------    READING countMinusOne " << countMinusOne << std::endl;
		count = countMinusOne + 1;
	}
	else {
		// default to 1
		std::cout << "[AckRange::read]: Defaulting count to 1" << std::endl;
		count = 1;
	}
}

bool AckRange::extend(PacketSequenceNumber newSequenceNumber) {
	if (newSequenceNumber == start + count) {
		count++;
		return true;
	}

	return false;
}

PacketSequenceNumber AckRange::getStart() const {
	return start;
}

uint32_t AckRange::getCount() const {
	return count;
}

