#include "pch.h"
#include "GameObjectRegistry.h"
#include "World.h"

std::unique_ptr<GameObjectRegistry>	GameObjectRegistry::instance;

GameObjectRegistry::GameObjectRegistry() { }

void GameObjectRegistry::staticInit() {
	instance.reset(new GameObjectRegistry());
}

void GameObjectRegistry::registerCreationFunction(uint32_t className, GameObjectCreationFunc creationFunction) {
	nameToGameObjectCreationFunction[className] = creationFunction;
}

std::shared_ptr<GameObject> GameObjectRegistry::createGameObject(uint32_t className) {
	GameObjectCreationFunc creationFunction = nameToGameObjectCreationFunction[className];

	std::shared_ptr<GameObject> gameObject = creationFunction();

	World::instance->addGameObject(gameObject);

	return gameObject;
}
