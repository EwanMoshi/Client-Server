#include "pch.h"
#include "ClientCharacter.h"

ClientCharacter::ClientCharacter() {

}

void ClientCharacter::update() {

}

void ClientCharacter::read(InputBitStream& inputStream) {
	uint32_t playerId;
	inputStream.read(playerId);
	setPlayerId(playerId);
	std::cout << "Read player ID " << playerId << std::endl;
}