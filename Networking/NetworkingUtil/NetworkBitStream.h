#pragma once
#include "pch.h"

class OutputBitStream {
public:
	OutputBitStream();
	~OutputBitStream();

	void writeBits(uint8_t data, uint32_t bitCount);
	void writeBits(const void* data, uint32_t bitCount);

	const char* getBufferPtr() const { return buffer; }
	uint32_t getBitLength() const { return bitHead; }
	uint32_t getByteLength() const { return (bitHead + 7) >> 3; }

private:
	void reallocBuffer(uint32_t newBitLength);

	char* buffer;
	uint32_t	bitHead;
	uint32_t	bitCapacity;
};


class InputBitStream {
public:

};