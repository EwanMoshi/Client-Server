#pragma once

#include <cstdint>

enum class ReplicationAction {
	RA_Create,
	RA_Update,
	RA_Destroy,
	RA_RPC,
	RA_MAX
};

struct ReplicationCommand {
public:
	ReplicationCommand() {}

	// TODO: partial rep/dirty states
	ReplicationCommand(uint32_t initialDirtyState) : action(ReplicationAction::RA_Create) {}

	void setDestroy() {
		action = ReplicationAction::RA_Destroy;
	}

private:
	ReplicationAction action;

};