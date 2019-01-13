#include "Packet.h"

Packet::Packet(const PacketType& type, const unsigned int& id)
{
	_id = id;
	_type = type;
	_hintId = _id;
}

Packet::Packet(const PacketType& type, const unsigned int& id, const unsigned int& hintId)
{
	_id = id;
	_type = type;
	_hintId = hintId;
}

std::vector<unsigned char> Packet::GenerateBuffer() const
{
	std::vector<unsigned char> buffer = std::vector<unsigned char>(4096);
	
	//id
	for (int i = 0; i < 4; i++) buffer[3 - i] = (_id >> (i * 8));

	//type
	buffer[4] = (unsigned char)_type;

	//data
	for (int i = 0; i < 4087; i++) buffer[i + 5] = content[i];

	//hint id
	for (int i = 4092; i < 4096; i++) buffer[3 - (i - 4092)] = (_hintId >> ((i - 4092) * 8));

	return buffer;
}

