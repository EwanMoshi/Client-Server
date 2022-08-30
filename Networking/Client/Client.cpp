#include "pch.h"
#include "Client.h"
#include <GLFW/glfw3.h>
#include "ClientCharacter.h"

Client* Client::Instance = nullptr;

bool Client::staticInit() {
	if (Instance == nullptr) {
		Instance = new Client();
	}

	return true;
}

Client::Client() {
	GameObjectRegistry::staticInit();
	World::staticInit();

	GameObjectRegistry::instance->registerCreationFunction('CHAR', ClientCharacter::staticCreate);

	SocketUtil::init();

	std::shared_ptr<SocketAddress> serverAddress = SocketAddressFactory::createIPv4FromString("127.0.0.1:8989");

	// name should be unique
	NetworkManagerClient::staticInit(*serverAddress, "e_moshi");
}
	
int Client::mainGameLoop() {
	GLFWwindow* window;

	if (!glfwInit()) {
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "Client", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, Client::keyCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	while (!glfwWindowShouldClose(window)) {
		Timing::instance.Update();

		// simulate
		World::instance->update();

		processInputs(window);

		NetworkManagerClient::Instance->processIncomingPackets();

		// render
		glClear(GL_COLOR_BUFFER_BIT);

		for (const auto& go : World::instance->getGameObjects()) {
			go->render();
		}

		glfwSwapBuffers(window);

		glfwPollEvents();

		NetworkManagerClient::Instance->sendOutgoingPackets();
	}

	return 0;
}

void Client::processInputs(GLFWwindow* window) {
	std::shared_ptr<GameObject> firstGameObject = nullptr;

	if (!World::instance->getGameObjects().empty()) {
		firstGameObject = World::instance->getGameObjects()[0];
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (!firstGameObject) {
		std::cout << "no object found" << std::endl;
		return;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		firstGameObject->setLocation(firstGameObject->getX(), firstGameObject->getY() + 5);
		std::cout << "WWWW" << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		firstGameObject->setLocation(firstGameObject->getX() - 5, firstGameObject->getY());
		std::cout << "AAAAAAA" << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		std::cout << "SSSSSSSS" << std::endl;
		firstGameObject->setLocation(firstGameObject->getX(), firstGameObject->getY() - 5);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		std::cout << "DDDD" << std::endl;
		firstGameObject->setLocation(firstGameObject->getX() + 5, firstGameObject->getY());
	}
}

void Client::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}


int main()
{
	if (Client::staticInit()) {
		return Client::Instance->mainGameLoop();
	}
	else {
		return -1;
	}
}
