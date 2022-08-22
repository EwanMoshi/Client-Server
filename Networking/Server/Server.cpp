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

	window = glfwCreateWindow(640, 480, "Server", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, Server::keyCallback);

	while (!glfwWindowShouldClose(window)) {
		Timing::instance.Update();

		processInputs(window);

		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);

		glfwPollEvents();

		// TODO: turn this into a getInstance function that initializes the Instance if it's nullptr
		NetworkManagerServer::Instance->processIncomingPackets();

		// NetworkManagerServer::Instance->checkForDisconnects();

		// NetworkManagerServer::Instance->sendOutgoingPackets();

		// temp for testing
		NetworkManagerServer::Instance->processTimedOutPackets();
	}

	glfwTerminate();
	return 0;
}

void Server::processInputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void Server::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		std::cout << "aaaaaaaa" << std::endl;
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
