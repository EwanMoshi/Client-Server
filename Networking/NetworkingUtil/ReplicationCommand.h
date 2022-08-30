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
	ReplicationCommand() : action(ReplicationAction::RA_Create) {}

	// TODO: partial rep/dirty states
	ReplicationCommand(uint32_t initialDirtyState) : action(ReplicationAction::RA_Create) {}

	void handleCreateAckd() {
		if (action == ReplicationAction::RA_Create) {
			action = ReplicationAction::RA_Update;
		}
	}

	void setDestroy() {
		action = ReplicationAction::RA_Destroy;
	}

	ReplicationAction getAction() const {
		return action;
	}

private:
	ReplicationAction action;

};