#pragma once

#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <string>
#include <map>
#include <thread>

#include "Packet.h"

class TcpClient
{
public:
	TcpClient(const char* address, const unsigned short& port);
	~TcpClient();

	std::map<const unsigned int, Packet> receivedPackets;

	int Start();
	int Send(const Packet& packet);

private:
	const char* _address;
	unsigned short _port;
	SOCKET _socket;
	std::thread* _listeningThread;
	bool _listening;
	void Listen();
};

