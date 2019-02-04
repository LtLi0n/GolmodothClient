#pragma once

#include <string>
#include <time.h>

class Message
{
public:
	Message(const std::wstring& author, const time_t& timeStamp, const std::wstring& content, const short& author_color);

	const std::wstring& Author() const;
	const std::wstring& Content() const;
	const time_t& TimeStamp() const;
	const short& AuthorColor() const;

private:
	const short m_author_color;
	const std::wstring m_author;
	const time_t m_timeStamp;
	const std::wstring m_content;
};

