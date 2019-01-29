#pragma once

#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <string>
#include <map>
#include <thread>

#include "Packet.h"

class Packet;

class TcpClient
{
public:
	TcpClient(const char* address, const unsigned short& port);
	~TcpClient();

	std::map<const unsigned int, std::shared_ptr<Packet>> receivedPackets;

	int Start();
	int Send(const Packet& packet);
	int SendRequest(const char* content) const;
	const std::shared_ptr<Packet> WaitHeader(const char* header) const;
	const std::shared_ptr<Packet> GetByHeader(const char* header) const;
	void DeletePacket(const std::shared_ptr<Packet>& packet);

private:
    const char* _address;
	unsigned short _port;
	SOCKET _socket;
	std::thread* _listeningThread;
	bool _listening;
	void Listen();
};

