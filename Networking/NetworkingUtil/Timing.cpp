#include "pch.h"
#include "Timing.h"

#if !_WIN32
#include <chrono>
using namespace std::chrono;
#endif

namespace
{
#if _WIN32
	LARGE_INTEGER startTime = { 0 };
#else
	high_resolution_clock::time_point startTime;
#endif
}

Timing Timing::instance;

Timing::Timing() {
#if _WIN32
	LARGE_INTEGER perfFreq;
	QueryPerformanceFrequency(&perfFreq);
	perfCountDuration = 1.0 / perfFreq.QuadPart;

	QueryPerformanceCounter(&startTime);

	lastFrameStartTime = GetTimeAsDouble();
#else
	startTime = high_resolution_clock::now();
#endif
}

void Timing::Update() {
	double currentTime = GetTimeAsDouble();

	deltaTime = (float)(currentTime - lastFrameStartTime);

	lastFrameStartTime = currentTime;
	frameStartTime = static_cast<float> (lastFrameStartTime);
}

float Timing::GetDeltaTime() const { 
	return deltaTime; 
}

double Timing::GetTimeAsDouble() const {
#if _WIN32
	LARGE_INTEGER curTime, timeSinceStart;
	QueryPerformanceCounter(&curTime);

	timeSinceStart.QuadPart = curTime.QuadPart - startTime.QuadPart;

	return timeSinceStart.QuadPart * perfCountDuration;
#else
	auto now = high_resolution_clock::now();
	auto ms = duration_cast<milliseconds>(now - startTime).count();
	return static_cast<double>(ms) / 1000;
#endif
}

float Timing::GetTimeAsFloat() const {
	return static_cast<float>(GetTimeAsDouble());
}

float Timing::GetFrameStartTime() const {
	return frameStartTime; 
}
