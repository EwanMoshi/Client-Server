#pragma once

#include "GameObject.h"

typedef std::shared_ptr<GameObject>(*GameObjectCreationFunc)();

class GameObjectRegistry
{
public:
	static void staticInit();

	static std::unique_ptr<GameObjectRegistry> instance;

	void registerCreationFunction(uint32_t className, GameObjectCreationFunc creationFunction);
	std::shared_ptr<GameObject> createGameObject(uint32_t className);

private:
	GameObjectRegistry();
	std::unordered_map<uint32_t, GameObjectCreationFunc> nameToGameObjectCreationFunction;
};

