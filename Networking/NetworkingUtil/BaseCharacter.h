#pragma once

#include "GameObject.h"
#include "MoveList.h"

class BaseCharacter : public GameObject
{
public:
	CLASS_IDENTIFICATION('CHAR', GameObject)

	virtual ~BaseCharacter();

	static GameObject* staticCreate() {
		std::cout << " Creating base character" << std::endl;
		return new BaseCharacter();
	}

	void processInput(const InputState& inputState, float deltaTime);
	void simulateMovement(float deltaTime);

	unsigned int getPlayerId();
	void setPlayerId(uint32_t newPlayerId);

	virtual uint32_t write(OutputBitStream& outputStream) const override;

protected:
	BaseCharacter();

	// rendering
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	unsigned int shaderProgram;

	// movement
	float movementSpeed;
	float moveDirHorizontal;
	float moveDirVertical;

private:
	uint32_t playerId;

};

