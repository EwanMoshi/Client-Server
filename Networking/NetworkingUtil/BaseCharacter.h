#pragma once

#include "GameObject.h"

class BaseCharacter : public GameObject
{
public:
	CLASS_IDENTIFICATION('CHAR', GameObject)

	virtual ~BaseCharacter();

	static GameObject* staticCreate() {
		std::cout << " Creating base character" << std::endl;
		return new BaseCharacter();
	}

	void setPlayerId(uint32_t newPlayerId);

	virtual uint32_t write(OutputBitStream& outputStream) const override;

protected:
	BaseCharacter();

	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	unsigned int shaderProgram;

private:
	uint32_t playerId;

};

