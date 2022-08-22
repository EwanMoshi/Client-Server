#pragma once

#include "GameObject.h"

class BaseCharacter : public GameObject
{
public:
	CLASS_IDENTIFICATION('CHAR', GameObject)

	static GameObject* staticCreate() {
		return new BaseCharacter();
	}

	void setPlayerId(uint32_t newPlayerId);

	virtual uint32_t write(OutputBitStream& outputStream) const override;

protected:
	BaseCharacter();

private:
	uint32_t playerId;
};

