#pragma once

class ReplicationManagerClient
{
public:
	void read(InputBitStream& inputStream);

private:
	void readAndPerformCreateAction(InputBitStream& inputStream, int networkId);
	void readAndPerformUpdateAction(InputBitStream& inputStream, int networkId);
	void readAndPerformDestroyAction(InputBitStream& inputStream, int networkId);
};

