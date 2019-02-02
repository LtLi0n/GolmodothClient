#include "Packet.h"
#include <cstring>
#include <string>
#include <iostream>

Packet::Packet(const PacketType& type) :
	m_id(0),
	m_type(type) { }

Packet::Packet(const PacketType& type, const unsigned int& id) :
	m_id(id),
	m_type(type) { }

void Packet::AddContent(const char* content)
{
	m_content = content;
	m_content_n = strlen(content);
}

//redo to streams later.
//this approach is too expensive
const char* Packet::GenerateBuffer() const
{	
	int buffer_l = m_content_n + 1;
	char* buffer = new char[buffer_l + 4];

	std::memcpy(buffer, &buffer_l, 4);

	//type
	buffer[4] = (char)m_type;

	//set data
	for (int i = 0; i < m_content_n; i++) buffer[i + 5] = m_content[i];

	return (const char*)buffer;
}

PacketType Packet::GetType() const { return m_type; }
unsigned int Packet::GetID() const { return m_id; }
const char* Packet::Content() const { return m_content; }
const int& Packet::ContentLength() const { return m_content_n; }