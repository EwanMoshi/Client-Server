#include "pch.h"
#include "NetworkBitStream.h"

OutputBitStream::OutputBitStream() : bitHead(0), buffer(nullptr) {
	reallocBuffer(1500 * 8);
}

OutputBitStream::~OutputBitStream() {
	std::free(buffer);
}

void OutputBitStream::writeBits(const void* data, uint32_t bitCount) {
	const char* sourceByte = static_cast<const char*>(data);

	// call writeBits on each byte until we no longer have a byte to write
	while (bitCount > 8) {
		writeBits(*sourceByte, 8);
		sourceByte++;
		bitCount -= 8;
	}

	// write remainder of bits
	if (bitCount > 0) {
		writeBits(*sourceByte, bitCount);
	}
}

void OutputBitStream::writeBits(uint8_t data, uint32_t bitCount) {
	uint32_t nextBitHead = bitHead + static_cast<uint32_t>(bitCount);

	// increase size of buffer if we're out of space
	if (nextBitHead > bitCapacity) {
		reallocBuffer(std::max(bitCapacity * 2, nextBitHead));
	}

	// find the current byte offset which can be found by dividing by 8 (same as right shift by 3, i.e. >> 3)
	uint32_t currentByteOffset = bitHead >> 3;

	// find bit offset within that byte by examining last 3 digits that were shifted
	// Note: We can AND with 0x7 which is 111 in binary
	uint32_t currentBitOffset = bitHead & 0x7;

	// find bits to preserve within current byte
	uint8_t currentMask = ~(0xff << currentBitOffset);
	buffer[currentByteOffset] = (buffer[currentByteOffset] & currentMask) | (data << currentBitOffset);

	// find how many bits were not used in current byte in the buffer
	uint32_t bitsFreeInByte = 8 - currentBitOffset;

	// if we needed more bits than were free, we carry over into the next byte in our buffer
	if (bitsFreeInByte < bitCount) {
		buffer[currentByteOffset + 1] = data >> bitsFreeInByte;
	}

	bitHead = nextBitHead;
}

void OutputBitStream::write(bool inData) {
	writeBits(&inData, 1);
}

void OutputBitStream::reallocBuffer(uint32_t newBitLength) {
	if (buffer == nullptr) {
		buffer = static_cast<char*>(std::malloc(newBitLength >> 3));
		memset(buffer, 0, newBitLength >> 3);
	}
	else {
		// create new temp buffer
		char* tempBuffer = static_cast<char*>(std::malloc(newBitLength >> 3));
		memset(tempBuffer, 0, newBitLength >> 3);

		// copy the buffer into tempBuffer
		memcpy(tempBuffer, buffer, bitCapacity >> 3);

		// free current buffer and assign to tempBuffer
		std::free(buffer);
		buffer = tempBuffer;
	}

	bitCapacity = newBitLength;
}

// InputBitStream Class ---------------------------------------

InputBitStream::InputBitStream(char* newBuffer, uint32_t bitCount) : buffer(newBuffer), bitCapacity(bitCount), bitHead(0), ownsBuffer(false) {}

InputBitStream::InputBitStream(const InputBitStream& other) : bitCapacity(other.bitCapacity), bitHead(other.bitHead), ownsBuffer(true) {
	int byteCount = bitCapacity / 8;
	buffer = static_cast<char*>(std::malloc(byteCount));

	memcpy(buffer, other.buffer, byteCount);
}

InputBitStream::~InputBitStream() {
	if (ownsBuffer) {
		std::free(buffer);
	}
}

void InputBitStream::readBits(void* data, uint32_t bitCount) {
	uint8_t* destByte = reinterpret_cast<uint8_t*>(data);

	while (bitCount > 8) {
		readBits(*destByte, 8);
		destByte++;
		bitCount -= 8;
	}

	if (bitCount > 0) {
		readBits(*destByte, bitCount);
	}
}

void InputBitStream::readBits(uint8_t& data, uint32_t bitCount) {
	uint32_t currentByteOffset = bitHead >> 3;
	uint32_t currentBitOffset = bitHead & 0x7;

	data = static_cast<uint8_t>(buffer[currentByteOffset]) >> currentBitOffset;

	uint32_t bitsFreeInByte = 8 - currentBitOffset;
	if (bitsFreeInByte < bitCount)
	{
		data |= static_cast<uint8_t>(buffer[currentByteOffset + 1]) << bitsFreeInByte;
	}

	// mask so that we only read the bit we need
	data &= (~(0x00ff << bitCount));

	bitHead += bitCount;
}