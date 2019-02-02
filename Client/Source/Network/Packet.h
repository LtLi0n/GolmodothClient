#pragma once

enum PacketType { PACKET_NULL, PACKET_SEND, PACKET_REQUEST };

class Packet
{
public:
	Packet(const PacketType& type);
	Packet(const PacketType& type, const unsigned int& id);
	Packet() = default;

	void AddContent(const char* content);
	const char* GenerateBuffer() const;
	PacketType GetType() const;
	unsigned int GetID() const;

    const char* Content() const;
	const int& ContentLength() const;
private:
	const char* m_content;
	int m_content_n;
	const unsigned int& m_id;
	const PacketType& m_type;
};

