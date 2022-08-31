// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include "Server.h"
#include <GLFW/glfw3.h>
#include "ServerCharacter.h"

Server* Server::Instance = nullptr;

bool Server::staticInit() {
	if (Instance == nullptr) {
		Instance = new Server();
	}

	return true;
}

Server::Server() {
	GameObjectRegistry::staticInit();
	World::staticInit();

	GameObjectRegistry::instance->registerCreationFunction('CHAR', ServerCharacter::staticCreate);

	initNetworkManager();
}

bool Server::initNetworkManager() {
	std::string portString = "8989";
	uint16_t port = stoi(portString);

	return NetworkManagerServer::staticInit(port);
}

int Server::run() {
	GLFWwindow* window;

	if (!glfwInit()) {
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "Server", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, Server::keyCallback);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	while (!glfwWindowShouldClose(window)) {
		Timing::instance.Update();
	
		// simulate
		World::instance->update();

		// NOTE: This is temporary, server won't have processInputs
		processInputs(window);

		NetworkManagerServer::Instance->processIncomingPackets();

		// render
		glClear(GL_COLOR_BUFFER_BIT);

		for (const auto& go : World::instance->getGameObjects()) {
			go->render();
		}

		glfwSwapBuffers(window);

		glfwPollEvents();

		// NetworkManagerServer::Instance->checkForDisconnects();

		NetworkManagerServer::Instance->sendOutgoingPackets();

		// temp for testing
		NetworkManagerServer::Instance->processTimedOutPackets();
	}

	glfwTerminate();
	return 0;
}

void Server::handleNewClient(int playerId) {
	spawnCharacterForPlayer(playerId);
}

void Server::spawnCharacterForPlayer(int playerId) {
	std::shared_ptr<BaseCharacter> character = std::static_pointer_cast<BaseCharacter>(GameObjectRegistry::instance->createGameObject('CHAR'));
	character->setPlayerId(playerId);
}

void Server::processInputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		auto firstGameObject = World::instance->getGameObjects()[0];
		firstGameObject->setLocation(firstGameObject->getX(), firstGameObject->getY() + 5);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		auto firstGameObject = World::instance->getGameObjects()[0];
		firstGameObject->setLocation(firstGameObject->getX() - 5, firstGameObject->getY());
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		auto firstGameObject = World::instance->getGameObjects()[0];
		firstGameObject->setLocation(firstGameObject->getX(), firstGameObject->getY() - 5);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		auto firstGameObject = World::instance->getGameObjects()[0];
		firstGameObject->setLocation(firstGameObject->getX() + 5, firstGameObject->getY());
	}
}

void Server::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		std::cout << "aaaaaaaa" << std::endl;
		auto firstGameObject = World::instance->getGameObjects()[0];
		firstGameObject->setLocation(firstGameObject->getX() + 5, firstGameObject->getY() + 5);
	}
}

int main()
{
	if (Server::staticInit()) {
		return Server::Instance->run();
	}
	else {
		return -1;
	}

    // create UDP socket using AF_INET (IPv4)
  //  auto udpSocket = SocketUtil::CreateUDPSocket(INET);
  //  udpSocket->SetNonBlockingMode(true);

  //  while (true) {
		//char data[1500];

		//std::shared_ptr<SocketAddress> serverAddress = SocketAddressFactory::createIPv4FromString("127.0.0.1:8989");

		//int bytesReceived = udpSocket->ReceiveFrom(data, sizeof(data), *serverAddress);

		//if (bytesReceived > 0) {

		//}
  //  }
}
