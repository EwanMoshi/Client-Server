#pragma once
#include "GameObject.h"

class World
{
public:
	static void staticInit();

	static std::unique_ptr<World> instance;

	void addGameObject(std::shared_ptr<GameObject> gameObject);

	void update();

	const std::vector<std::shared_ptr<GameObject>>& getGameObjects() const {
		return gameObjects;
	}

private:
	World();
	World(World const&) = delete;
	World& operator=(World const&) = delete;
	World(World&&) = delete;
	World& operator=(World&&) = delete;

	int getIndexOfGameObject(std::shared_ptr<GameObject> gameObject);

	std::vector<std::shared_ptr<GameObject>> gameObjects;
};

