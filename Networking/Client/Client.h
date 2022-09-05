#pragma once

#include "MoveList.h"

class Client {
public:
	static Client* Instance;

	static bool staticInit();
	int mainGameLoop();

	// TODO: Temp. Find a better place to store moveList
	MoveList& getMoveList() { return moveList; }

private:
	Client();
	void processInputs(struct GLFWwindow* window, float timestamp, int frame);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	int frameCount = 0;
	MoveList moveList;
};