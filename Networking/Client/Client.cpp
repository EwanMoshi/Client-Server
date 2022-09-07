#include "pch.h"
#include "Client.h"
#include <GLFW/glfw3.h>
#include "ClientCharacter.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


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

	const char* glslVersion = "#version 130";

	// set up imgui
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glslVersion);

	float updateAccumulator = 0.0f;
	float packetSendAccumulator = 0.0f;

	const double tickRate = 1.0 / 64.0;

	// how often we send input packet to server in MS
	static float packetTransmissionRate_MS = (1.0f / 20.0f) * 1000.0f;

	float timer = 0.0f;
	float c = 0;

	while (!glfwWindowShouldClose(window)) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		Timing::instance.Update();

		processInputs(window, 0.0f, 0);

		float deltaTime = Timing::instance.GetDeltaTime();
		updateAccumulator += deltaTime;
		packetSendAccumulator += deltaTime;
		timer += deltaTime;

		if (timer >= 1.0) {
			std::cout << "Input packets sent in the last second " << c << std::endl;
			timer = 0.0f;
			c = 0;
		}

		// simulate
		while (updateAccumulator >= tickRate) {
			World::instance->update();
			updateAccumulator -= tickRate;
			//std::cout << "simulate" << std::endl;
		}

		NetworkManagerClient::Instance->processIncomingPackets();

		// render
		glClear(GL_COLOR_BUFFER_BIT);

		for (const auto& go : World::instance->getGameObjects()) {
			go->render();
		}

		// imgui
		{
			ImGui::Begin("Network configuration");
			
			ImGui::SliderFloat("Simulate Latency RTT (sec)", &NetworkManagerBase::simulatedLatency_SEC, 0.0f, 1.0f);
			ImGui::SliderFloat("Packet loss %", &NetworkManagerClient::simulatedPacketLoss, 0.0f, 1.0f);
			ImGui::SliderFloat("Transmission Rate (ms)", &packetTransmissionRate_MS, (1.0f / 20.0f) * 1000.0f, 1000.0f);

			ImGui::Text("Sending %.3f input packets per second", 1000.0f / packetTransmissionRate_MS);
			ImGui::Text("Current ping (ms) %.0f", NetworkManagerBase::simulatedLatency_SEC * 1000.0f);
			ImGui::Text("Packet loss %.0f%%", NetworkManagerClient::simulatedPacketLoss * 100.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();

		// send input packets
		while (packetSendAccumulator >= packetTransmissionRate_MS / 1000.0f) {
			c++;
			packetSendAccumulator -= packetTransmissionRate_MS / 1000.0f;
			NetworkManagerClient::Instance->sendOutgoingPackets();
			//std::cout << "sendOutgoingPackets" << std::endl;
		}

		frameCount++;
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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
