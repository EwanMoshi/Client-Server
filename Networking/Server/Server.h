#pragma once

class Server {
public:
	static Server* Instance;

	static bool staticInit();
	int run();

	void handleNewClient(int playerId);

private:
	Server();
	bool initNetworkManager();
	void processInputs(class GLFWwindow* window);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	
	void spawnCharacterForPlayer(int playerId);
};