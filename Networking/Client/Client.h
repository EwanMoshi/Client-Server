#pragma once

class Client {
public:
	static Client* Instance;

	static bool staticInit();
	int mainGameLoop();

private:
	Client();
};