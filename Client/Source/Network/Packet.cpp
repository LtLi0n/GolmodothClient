#include "Packet.h"
#include <cstring>

Packet::Packet(const PacketType& type) :
	_id(0),
	_hintId(0),
	_type(type) { }

Packet::Packet(const PacketType& type, const unsigned int& id) :
	_id(id),
	_hintId(0),
	_type(type) { }

Packet::Packet(const PacketType& type, const unsigned int& id, const unsigned int& hintId) :
	_id(id),
	_hintId(hintId),
	_type(type) { }

const char* Packet::GenerateBuffer() const
{	
	char* buffer = new char[2040];
	
	//type
	buffer[0] = (char)_type;

	//0 everything out
	for (int i = 0; i < 2039; i++)buffer[i + 1] = 0;

	//set data
	for (int i = 0; i < strlen(content); i++) buffer[i + 1] = content[i];

	return (const char*)buffer;
}

PacketType Packet::GetType() const { return _type; }
unsigned int Packet::GetID() const { return _id; }
unsigned int Packet::GetHintID() const { return _hintId; }
