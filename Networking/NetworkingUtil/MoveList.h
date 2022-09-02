#pragma once

struct InputState {
	InputState() :
		forward(0),
		left(0),
		back(0),
		right(0) 
	{
	}

	InputState(float forward, float left, float back, float right) :
		forward(forward),
		left(left),
		back(back),
		right(right)
	{
	}

	float getHorizontalDelta() const {
		return right - left;
	}

	float getVerticalDelta() const {
		return forward - back;
	}

	bool write(OutputBitStream& outputStream) const {
		writeSignedBinaryValue(outputStream, getHorizontalDelta());
		writeSignedBinaryValue(outputStream, getVerticalDelta());

		return true;
	}

	bool read(InputBitStream& inputStream) {
		readSignedBinaryValue(inputStream, right);
		readSignedBinaryValue(inputStream, forward);

		return true;
	}

private:
	void writeSignedBinaryValue(OutputBitStream& outputStream, float value) const {
		bool isNonZero = value != 0.0f;
		outputStream.write(isNonZero);
		if (isNonZero) {
			outputStream.write(value > 0.0f);
		}
	}

	void readSignedBinaryValue(InputBitStream& inputStream, float& outValue) {
		bool isNonZero;
		inputStream.read(isNonZero);

		if (isNonZero) {
			bool isPositive;
			inputStream.read(isPositive);
			outValue = isPositive ? 1.0f : -1.0f;
		}
		else {
			outValue = 0.0f;
		}
	}

	// TODO: we may not need left and back. Maybe just rename forward and right to be horizontal and vertical
	float forward, left, back, right;
};

struct Move {
	Move() {}

	Move(const InputState& inputState, float timestamp, float deltaTime, int frame) :
		inputState(inputState),
		timestamp(timestamp),
		deltaTime(deltaTime),
		frame(frame) 
	{
	}

	const InputState& getInputState() const { return inputState; }
	float getTimestamp() const { return timestamp; }
	float getDeltaTime() const { return deltaTime; }
	float getFrame() const { return frame; }

	bool write(OutputBitStream& outputStream) const {
		inputState.write(outputStream);
		outputStream.write(timestamp);
		outputStream.write(frame);

		return true;
	}

	bool read(InputBitStream& inputStream) {
		inputState.read(inputStream);
		inputStream.read(timestamp);
		inputStream.read(frame);

		return true;
	}

private:
	InputState inputState;
	float timestamp;
	float deltaTime;
	int frame;
};

class MoveList
{
public:
	MoveList();

	const Move& addMove(const InputState& inputState, float timestamp, int frame);
	void addMove(const Move& move);

	void removeProcessedMoves(float lastMoveProcessedOnServerTimestamp, int frame = 0);
	float getLastMoveTimestamp() const;
	const Move& getLatestMove() const;

	void clear();
	bool hasMoves() const;
	int getMoveCount() const;

	std::deque<Move>::const_iterator begin() const { return moves.begin(); }
	std::deque<Move>::const_iterator end() const { return moves.end(); }

	const Move& operator[](size_t i) const { return moves[i]; }
private:
	float lastMoveTimestamp;
	std::deque<Move> moves;
};

