#pragma once

class Server {
public:
	static Server* Instance;

	static bool staticInit();
	int run();

private:
	Server();
	bool initNetworkManager();
};