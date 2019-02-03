#include "Chat.h"
#include <chrono>

Chat::Chat(ConsoleEngine& engine, TlsClient& tls) : width(20), height(5), m_engine(engine), m_tls(tls), m_off_i(0) { }

void Chat::AddMessage(const Message& message)
{
	m_messages.push_back(message);
	if (m_messages.size() - m_off_i > height) m_off_i++;
}

void Chat::Update()
{
	width = m_engine.ScreenWidth() - 2;

	if (m_engine.mouseWheelRotation != 0)
	{
		int tmp_off_i = m_off_i + (m_engine.mouseWheelRotation > 0 ? -1 : 1);

		//offset message display position
		if (tmp_off_i < m_messages.size() && tmp_off_i > -1)
		{
			m_off_i = tmp_off_i;
		}

		m_engine.mouseWheelRotation = 0;
	}
}

void Chat::Render()
{
	int chat_y = m_engine.ScreenHeight() - height - 2;

	m_engine.Fill(0, chat_y - 2, width + 3, chat_y - 1, L'-', FG_GREY);
	m_engine.Fill(0, chat_y - 1, width + 3, chat_y + height + 1, L' ', FG_BLACK);
	
	int offY = 0;

	//display messages
	for (int y = 0; y + offY < height; y++)
	{
		int off_ii = m_off_i + y;
		if (off_ii == m_messages.size()) break;

		int author_l = m_messages[off_ii].Author().length();
		int content_l = m_messages[off_ii].Content().length();

		time_t time = m_messages[off_ii].TimeStamp();
		tm* time_converted = gmtime(&time);

		wstring time_str = L'[' + (time_converted->tm_hour < 10 ? (L"0" + to_wstring(time_converted->tm_hour)) : to_wstring(time_converted->tm_hour)) +
			L':' + (time_converted->tm_min < 10 ? (L"0" + to_wstring(time_converted->tm_min)) : to_wstring(time_converted->tm_min)) + L']';

		wstring content = m_messages[off_ii].Content();

		//render message
		m_engine.DrawString(1, chat_y + y + offY - 1, time_str, FG_DARK_GREY);
		m_engine.DrawString(9, chat_y + y + offY - 1, m_messages[off_ii].Author() + L':', FG_RED);
		for (int ln = 0; content.size() > 0; ln++)
		{
			wstring output = ln == 0 ? content.substr(0, width - (11 + author_l)) : content.substr(0, width);
			content = content.erase(0, ln == 0 ? width - (11 + author_l) : width);

			if (ln > 0) offY++;
			if (y + offY >= height) break;

			m_engine.DrawString(ln == 0 ? (11 + author_l) : 1, chat_y + y + offY - 1, output, FG_GREY);
		}
	}

	//scrollbar
	{
		int max_vertical = m_messages.size() - height;
		double closest_value = 999;
		int closest_at_y = 0;

		int vertical_bar_pos = m_messages.size() - (m_off_i + height);
		if (vertical_bar_pos < 0) vertical_bar_pos = 0;

		for (int y = 0; y < height; y++)
		{
			double delta = abs((max_vertical * (y + 1)) / height - vertical_bar_pos);

			if (delta < closest_value)
			{
				closest_at_y = y;
				closest_value = delta;
			}

			m_engine.Draw(width + 1, chat_y + y - 1, L'|', FG_WHITE);
		}

		m_engine.Draw(width + 1, chat_y + height - closest_at_y - 2, L'O', FG_WHITE);
	}

	m_engine.Fill(0, chat_y + height - 1, m_engine.ScreenWidth(), chat_y + height, L'-', FG_GREY);

	if (m_engine.keyboard.receive_input)
	{
		m_engine.DrawString(
			1,
			chat_y + height,
			m_engine.keyboard.input.size() > width ? m_engine.keyboard.input.substr(m_engine.keyboard.input.size() - width) : m_engine.keyboard.input,
			FG_GREY);
	}

	auto current = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(current - m_engine.keyboard.last_blink);

	if (m_engine.keyboard.receive_input)
	{
		if (ms.count() < 500)
		{
			m_engine.Draw(1 + (m_engine.keyboard.input.size() > width ? width : m_engine.keyboard.input.size()), chat_y + height, L'│', FG_WHITE);
		}
		else if (ms.count() >= 1000)
		{
			m_engine.keyboard.last_blink = std::chrono::system_clock::now();
		}
	}
	else
	{
		m_engine.Draw(1 + (m_engine.keyboard.input.size() > width ? width : m_engine.keyboard.input.size()), chat_y + height, L'_', FG_WHITE);
	}
}

bool Chat::InputMode() const { return m_engine.keyboard.receive_input; }

void Chat::EnterInputMode()
{
	m_engine.keyboard.input.clear();
	m_engine.keyboard.last_blink = std::chrono::system_clock::now();
	m_engine.keyboard.receive_input = true;
}

void Chat::ExitInputMode(const bool& sendInput)
{
	if (m_engine.keyboard.input.size() > 0 && sendInput)
	{
		auto chrono_time = std::chrono::system_clock::now();
		time_t time = std::chrono::system_clock::to_time_t(chrono_time);
		AddMessage(Message(L"LtLi0n", time, m_engine.keyboard.input));
	}

	m_engine.keyboard.input.clear();
	m_engine.keyboard.receive_input = false;
}