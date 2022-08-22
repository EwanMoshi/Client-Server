#pragma once
#include <ReplicationCommand.h>

class ReplicationManagerServer
{
public:
	void replicateCreate(int networkId);
	void replicateDestroy(int networkId);

	void write(OutputBitStream& outputStream);
private:
	std::unordered_map<int, ReplicationCommand> networkIdtoReplicationCommand;
};

