#include "TcpClient.h"
#include <chrono>
#include <iostream>
#include <string>


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
	return send(_socket, packet.GenerateBuffer(), 2040, NULL);
}

Packet TcpClient::WaitHeader(const char* header) const
{
	while (true)
	{
		for (auto const& x : receivedPackets)
		{
			std::string headerStr = "";
			bool header_found = false;

			for (int i = 0; i < 2039; i++)
			{
				if (x.second.content[i] == '\0') break;
				if (x.second.content[i] == '\n')
				{
					header_found = true;
					break;
				}
				headerStr += x.second.content[i];
			}

			if (strcmp(headerStr.c_str(), header) == 0) return x.second;
		}
	}
}

Packet TcpClient::GetByHeader(const char* header) const
{
	for (auto const& x : receivedPackets)
	{
		const char* content = x.second.content;
		std::string headerStr = "";
		bool header_found = false;

		for (int i = 0; i < 2039; i++)
		{
			if (content[i] == 0) break;
			if (content[i] == '\n')
			{
				header_found = true;
				break;
			}
			headerStr += content[i];
		}

		if (strcmp(headerStr.c_str(), header) == 0) return x.second;
	}
}

void TcpClient::Listen()
{
	while (_listening)
	{
		char* data = new char[2048];
		ZeroMemory(data, 2048);

		char* content = new char[2039];
		ZeroMemory(content, 2039);

		recv(_socket, (char*)data, 2048, NULL);

		unsigned int packet_id = *((unsigned int*)data);
		PacketType packet_type = (PacketType)data[4];
		for (int i = 0; i < 2039; i++) content[i] = data[i + 5];

		//Offsetting the adress by 2044 bytes. At this point "I'm the smartest programmer that has ever lived."
		unsigned int packet_hintId = *(unsigned int*)(data + 2044);

		if (packet_type != PACKET_NULL)
		{
			Packet packet(packet_type, packet_id, packet_hintId);
			packet.content = content;

			receivedPackets[packet_id] = packet;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}