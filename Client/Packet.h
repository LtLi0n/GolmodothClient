#pragma once

enum PacketType { PACKET_NULL, PACKET_SEND, PACKET_REQUEST };

class Packet
{
public:
	Packet(const PacketType& type);
	Packet(const PacketType& type, const unsigned int& id);
	Packet(const PacketType& type, const unsigned int& id, const unsigned int& hintId);
	Packet() = default;

	const char* content; //2039
	const char* GenerateBuffer() const;
	PacketType GetType() const;
	unsigned int GetID() const;
	unsigned int GetHintID() const;

private:
	PacketType _type;
	unsigned int _id;
	int _hintId; //next packet id to wait for a complete content, if the sent data is bigger than 2039 bytes.
};

