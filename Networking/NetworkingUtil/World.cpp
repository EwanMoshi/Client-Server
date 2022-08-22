#include "pch.h"
#include "World.h"

std::unique_ptr<World> World::instance;

World::World() { }

void World::staticInit() {
	instance.reset(new World());
}

void World::addGameObject(std::shared_ptr<GameObject> gameObject) {
	gameObjects.push_back(gameObject);
	gameObject->setIndexInWorld(gameObjects.size() - 1);
}

void World::update() {

}

int World::getIndexOfGameObject(std::shared_ptr<GameObject> gameObject)
{
	return 0;
}
