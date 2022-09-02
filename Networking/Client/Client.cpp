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

	// disable vsync
	// glfwSwapInterval(0);

	float updateAccumulator = 0.0f;
	float packetSendAccumulator = 0.0f;

	const double tickRate = 1.0 / 64.0;

	// how often we send input packet to server in MS
	const double packetTransmissionRate_MS = 1.0 / 20.0;

	float timer = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		Timing::instance.Update();

		processInputs(window, 0.0f, 0);

		//std::cout << "Delta time = " << Timing::instance.GetDeltaTime() << std::endl;
		//std::cout << "FPS = " << 1.0f / Timing::instance.GetDeltaTime() << std::endl;

		float deltaTime = Timing::instance.GetDeltaTime();
		updateAccumulator += deltaTime;
		packetSendAccumulator += deltaTime;
		timer += deltaTime;

		// simulate
		while (updateAccumulator >= tickRate) {
			World::instance->update();
			updateAccumulator -= tickRate;
			std::cout << "simulate" << std::endl;
		}

		NetworkManagerClient::Instance->processIncomingPackets();

		// render
		glClear(GL_COLOR_BUFFER_BIT);

		for (const auto& go : World::instance->getGameObjects()) {
			go->render();
		}

		glfwSwapBuffers(window);

		glfwPollEvents();

		// send input packets
		while (packetSendAccumulator >= packetTransmissionRate_MS) {
			packetSendAccumulator -= packetTransmissionRate_MS;
			NetworkManagerClient::Instance->sendOutgoingPackets();
			std::cout << "sendOutgoingPackets" << std::endl;
		}

		frameCount++;
	}

	return 0;
}

void Client::processInputs(GLFWwindow* window, float timestamp, int frame) {
	std::shared_ptr<GameObject> firstGameObject = nullptr;

	if (!World::instance->getGameObjects().empty()) {
		firstGameObject = World::instance->getGameObjects()[0];
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	//if (!firstGameObject) {
	//	std::cout << "no object found" << std::endl;
	//	return;
	//}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		moveList.addMove(InputState(1.0f, 0.0f, 0.0f, 0.0f), timestamp, frame);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		moveList.addMove(InputState(0.0f, 1.0f, 0.0f, 0.0f), timestamp, frame);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		moveList.addMove(InputState(0.0f, 0.0f, 1.0f, 0.0f), timestamp, frame);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		moveList.addMove(InputState(0.0f, 0.0f, 0.0f, 1.0f), timestamp, frame);
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
