#pragma once

#include "BaseCharacter.h"

class ClientCharacter : public BaseCharacter
{
public:
	static std::shared_ptr<GameObject> staticCreate() {
		return std::shared_ptr<GameObject>(new ClientCharacter());
	}

	virtual void update() override;

	virtual void read(InputBitStream& inputStream) override;

protected:
	ClientCharacter();
};

