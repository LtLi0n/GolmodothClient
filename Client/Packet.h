#pragma once

#include <vector>

enum PacketType { PACKET_NULL, PACKET_SEND, PACKET_REQUEST };

class Packet
{
public:
	Packet(const PacketType& type, const unsigned int& id);
	Packet(const PacketType& type, const unsigned int& id, const unsigned int& hintId);
	Packet() = default;

	unsigned char* content; //4087
	std::vector<unsigned char> GenerateBuffer() const;

private:
	unsigned int _id;
	PacketType _type;
	int _hintId; //if data that is bigger than 1 packet
};

