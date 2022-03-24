#pragma once
#include "pch.h"

class OutputBitStream {
public:
	OutputBitStream();
	~OutputBitStream();

	// template function supporting primitive types
	// Note: This only supports little-endian. Consider byte swapping if needed
	template<typename T>
	void write(T data, uint32_t bitCount = sizeof(T) * 8) {
		static_assert(std::is_arithmetic<T>::value ||
			std::is_enum<T>::value,
			"OutputBitStream::write only supports primitive data types");

		writeBits(&data, bitCount);
	}

	// a bool version of write so that we can pass 1 bit as the bitCount
	// Note: Using template version above would result in 8 bits being used for a bool
	void write(bool inData);
	void write(const std::string& stringToWrite);

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
	InputBitStream(char* inBuffer, uint32_t inBitCount);
	InputBitStream(const InputBitStream& other);
	~InputBitStream();

	const char* getBufferPtr() const { return buffer; }
	uint32_t getRemainingBitCount() 	const { return bitCapacity - bitHead; }
	void readBytes(void* data, uint32_t byteCount) { readBits(data, byteCount << 3); }

	template<typename T>
	void read(T& data, uint32_t bitCount = sizeof(T) * 8)
	{
		static_assert(std::is_arithmetic<T>::value ||
			std::is_enum<T>::value,
			"InputBitStream::read only supports primitive data types");
		readBits(&data, bitCount);
	}


	void read(std::string& stringToRead);

	void readBits(uint8_t& data, uint32_t bitCount);
	void readBits(void* data, uint32_t bitCount);

	void resetToCapacity(uint32_t byteCapacity);


private:
	char* buffer;
	uint32_t bitHead;
	uint32_t bitCapacity;
	bool ownsBuffer;
};