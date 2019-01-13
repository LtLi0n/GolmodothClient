#include "TcpClient.h"
#include <chrono>
#include <iostream>


TcpClient::TcpClient(const char* address, const unsigned short& port)
{
	_address = address;
	_port = port;

	_listening = false;
}

TcpClient::~TcpClient()
{
	delete _address;
	if(_listeningThread != NULL) delete _listeningThread;	
}

int TcpClient::Start()
{
	//Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0) return -1;

	_socket = socket(AF_INET, SOCK_STREAM, NULL);
	if (_socket == INVALID_SOCKET)
	{
		WSACleanup();
		return -1;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(_port);
	inet_pton(AF_INET, _address, &hint.sin_addr);

	int connResult = connect(_socket, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << WSAGetLastError() << std::endl;
		closesocket(_socket);
		WSACleanup();
		return -1;
	}

	_listening = true;
	_listeningThread = new std::thread(&TcpClient::Listen, this);

	return 0;
}

int TcpClient::Send(const Packet& packet)
{
	std::vector<unsigned char> data = packet.GenerateBuffer();
	return send(_socket, (char*)data.data(), data.size(), NULL);
}

void TcpClient::Listen()
{
	while (_listening)
	{
		unsigned char* data = new unsigned char[4096];
		ZeroMemory(data, 4096);

		unsigned char* content = new unsigned char[4087];
		ZeroMemory(content, 4087);

		recv(_socket, (char*)data, 4096, NULL);

		unsigned int packet_id = *((unsigned int*)data);
		PacketType packet_type = (PacketType)data[4];
		for (int i = 0; i < 4087; i++) content[i] = data[i + 5];
		unsigned int packet_hintId = *((unsigned int*)data[4092]);

		if (packet_type != PACKET_NULL)
		{
			Packet packet(packet_type, packet_id, packet_hintId);
			packet.content = content;

			receivedPackets[packet_id] = packet;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}