#pragma once

class Client {
public:
	static Client* Instance;

	static bool staticInit();
	int mainGameLoop();

private:
	Client();
	void processInputs(class GLFWwindow* window);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};