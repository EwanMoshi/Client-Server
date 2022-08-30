#pragma once
#include <ReplicationCommand.h>

class ReplicationManagerServer
{
public:
	void replicateCreate(int networkId);
	void replicateDestroy(int networkId);

	void write(OutputBitStream& outputStream);
private:
	uint32_t writeCreateAction(OutputBitStream& outputStream, int networkId, uint32_t dirtyState = 0);
	uint32_t writeUpdateAction(OutputBitStream& outputStream, int networkId, uint32_t dirtyState = 0);
	uint32_t writeDestroyAction(OutputBitStream& outputStream, int networkId, uint32_t dirtyState = 0);

	std::unordered_map<int, ReplicationCommand> networkIdtoReplicationCommand;
};

