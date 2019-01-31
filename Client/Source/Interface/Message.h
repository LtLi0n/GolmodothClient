#pragma once

#include <string>
#include <time.h>

class Message
{
public:
	Message(const std::wstring& author, const time_t& timeStamp, const std::wstring& content);

	const std::wstring Author() const { return _author; }
	const std::wstring Content() const { return _content; }
	const time_t TimeStamp() const { return _timeStamp; }

private:
	const std::wstring _author;
	const time_t _timeStamp;
	const std::wstring _content;
};

