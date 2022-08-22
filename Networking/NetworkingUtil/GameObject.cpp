#include "pch.h"
#include "GameObject.h"

GameObject::GameObject() :
	x(0),
	y(0),
	networkId(0),
	indexInWorld(-1) 
{

}

void GameObject::update() {

}

void GameObject::setNetworkId(int newNetworkId) {
	networkId = newNetworkId;
}

int GameObject::getNetworkId() const
{
	return networkId;
}

void GameObject::setIndexInWorld(int newIndex) {
	indexInWorld = newIndex;
}

int GameObject::getIndexInWorld() const {
	return indexInWorld;
}

uint32_t GameObject::write(OutputBitStream& outputStream) const {
	return 0;
}

void GameObject::read(InputBitStream& inputStream) {

}

uint32_t GameObject::getX() const {
	return x;
}

uint32_t GameObject::getY() const {
	return y;
}