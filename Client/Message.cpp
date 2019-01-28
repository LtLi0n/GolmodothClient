#include "Message.h"

Message::Message(const std::wstring& author, const time_t& timeStamp, const std::wstring& content)
{
	_author = author;
	_timeStamp = timeStamp;
	_content = content;
}
