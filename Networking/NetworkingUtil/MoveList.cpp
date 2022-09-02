#include "pch.h"
#include "MoveList.h"

MoveList::MoveList() : lastMoveTimestamp(-1.0f) {

}

const Move& MoveList::addMove(const InputState& inputState, float timestamp, int frame) {
	float deltaTime = lastMoveTimestamp >= 0.0f ? timestamp - lastMoveTimestamp : 0.0f;

	moves.emplace_back(inputState, timestamp, deltaTime, frame);

	lastMoveTimestamp = timestamp;

	return moves.back();
}

void MoveList::addMove(const Move& move) {
	float timestamp = move.getTimestamp();
	float deltaTime = lastMoveTimestamp >= 0.0f ? timestamp - lastMoveTimestamp : 0.0f;
	lastMoveTimestamp = timestamp;
	moves.emplace_back(move.getInputState(), timestamp, deltaTime, move.getFrame());
}

void MoveList::removeProcessedMoves(float lastMoveProcessedOnServerTimestamp, int frame) {
	while (!moves.empty() && moves.front().getTimestamp() <= lastMoveProcessedOnServerTimestamp) {
		moves.pop_front();
	}
}

const Move& MoveList::getLatestMove() const {
	return moves.back();
}


float MoveList::getLastMoveTimestamp() const {
	return lastMoveTimestamp;
}

void MoveList::clear() {
	moves.clear();
}

bool MoveList::hasMoves() const {
	return !moves.empty();
}

int MoveList::getMoveCount() const {
	return moves.size();
}