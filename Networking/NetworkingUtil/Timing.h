#pragma once
class Timing {
public:
	static Timing instance;

	Timing();

	void Update();
	float GetDeltaTime() const;
	double GetTimeAsDouble() const;
	float GetTimeAsFloat() const;
	float GetFrameStartTime() const;

private:
	float deltaTime;
	uint64_t deltaTick;

	double lastFrameStartTime;
	float frameStartTime;
	double perfCountDuration;
};

