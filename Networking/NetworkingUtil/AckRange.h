#pragma once

/**
 * Represents a collection of consecutive sequences numbers to acknowledge. Start contains the first 
 * sequence number to acknowledge and count is how many consecutive sequence numbers starting from start.
 */
class AckRange {
public:
	AckRange();
	AckRange(PacketSequenceNumber start);

	/**
	 * This function only extends if the new sequence number is consecutive. If not,
	 * it returns false which notifies the caller that a new AckRange must be constructed.
	 */
	bool extend(PacketSequenceNumber newSequenceNumber);

	PacketSequenceNumber getStart() const;
	uint32_t getCount() const;

	void write(OutputBitStream& outputStream) const;
	void read(InputBitStream& inputStream);

private:
	PacketSequenceNumber start;
	uint32_t count;

};

