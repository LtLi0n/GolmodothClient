#pragma once

#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <string>
#include <map>
#include <thread>
#include <atomic>

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
	std::shared_ptr<Packet> WaitHeader(const char* header) const;
	std::shared_ptr<Packet> GetByHeader(const char* header) const;
	void DeletePacket(const std::shared_ptr<Packet>& packet);

private:
    const char* _address;
	unsigned short _port;
	std::atomic<bool> _listening;
	
	SOCKET _socket;
	std::unique_ptr<std::thread> _listeningThread;
	void Listen();
};

