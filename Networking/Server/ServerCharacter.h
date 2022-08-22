#pragma once

#include "BaseCharacter.h"

class ServerCharacter : public BaseCharacter
{
public:
	static std::shared_ptr<GameObject> staticCreate() {
		return NetworkManagerServer::Instance->registerAndReturn(new ServerCharacter());
	}

	virtual void update() override;

protected:
	ServerCharacter();
};

