// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include "SocketUtil.h"
#include "UDPSocket.h"
#include <iostream>

int main()
{
    // create UDP socket using AF_INET (IPv4)
    auto udpSocket = SocketUtil::CreateUDPSocket(INET);

    std::cout << "Hello World!\n";
}
