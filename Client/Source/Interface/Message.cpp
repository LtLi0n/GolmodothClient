#include "Message.h"

Message::Message(const std::wstring& author, const time_t& timeStamp, const std::wstring& content, const short& author_color) :
	m_author(author),
	m_timeStamp(timeStamp),
	m_content(content),
	m_author_color(author_color) { }

const std::wstring& Message::Author() const { return m_author; }
const std::wstring& Message::Content() const { return m_content; }
const time_t& Message::TimeStamp() const { return m_timeStamp; }
const short& Message::AuthorColor() const { return m_author_color; }