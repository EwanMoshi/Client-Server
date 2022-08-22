#include "pch.h"
#include "BaseCharacter.h"

BaseCharacter::BaseCharacter() : playerId(0) {
}

void BaseCharacter::setPlayerId(uint32_t newPlayerId) {
	playerId = newPlayerId;
}

uint32_t BaseCharacter::write(OutputBitStream& outputStream) const {
	outputStream.write(playerId);
	outputStream.write(getX());
	outputStream.write(getY());

	return 0;
}