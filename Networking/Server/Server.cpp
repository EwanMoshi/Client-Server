// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include "SocketUtil.h"
#include <iostream>

int main()
{
    // create UDP socket using AF_INET (IPv4)
    auto udpSocket = SocketUtil::CreateUDPSocket(INET);
    udpSocket->SetNonBlockingMode(true);

    while (true) {
		char data[1500];

		std::shared_ptr<SocketAddress> serverAddress = SocketAddressFactory::createIPv4FromString("127.0.0.1:8989");

		int bytesReceived = udpSocket->ReceiveFrom(data, sizeof(data), *serverAddress);

		if (bytesReceived > 0) {
			// debug
			int a = 5;
		}
    }
}
