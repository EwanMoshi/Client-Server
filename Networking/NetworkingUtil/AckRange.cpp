#include "pch.h"
#include "AckRange.h"

AckRange::AckRange() : start(0), count(0) {}

AckRange::AckRange(PacketSequenceNumber start) : start(start), count(1) {}

void AckRange::write(OutputBitStream& outputStream) const {
	outputStream.write(start);

	bool hasCount = count > 1;
	outputStream.write(hasCount);

	if (hasCount) {
		// max ack is 255
		uint32_t countMinusOne = count - 1;
		uint8_t countToAck = countMinusOne > 255 ? 255 : static_cast<uint8_t>(countMinusOne);
		outputStream.write(countToAck);
	}
}

void AckRange::read(InputBitStream& inputStream) {
	inputStream.read(start);

	bool hasCount;
	inputStream.read(hasCount);

	if (hasCount) {
		uint8_t countMinusOne;
		inputStream.read(countMinusOne);
		count = countMinusOne + 1;
	}
	else {
		// default to 1
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

