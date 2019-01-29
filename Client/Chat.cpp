#include "Chat.h"
#include <chrono>

Chat::Chat(ConsoleEngine& engine) : _engine(engine)
{
	_off_i = 0;
	width = 20;
	height = 8;
}

void Chat::AddMessage(const Message& message)
{
	_messages.push_back(message);
	if (_messages.size() - _off_i > height) _off_i++;
}

void Chat::Update()
{
	width = _engine.ScreenWidth() - 2;

	if (_engine.mouseWheelRotation != 0)
	{
		int tmp_off_i = _off_i + (_engine.mouseWheelRotation > 0 ? -1 : 1);

		//offset message display position
		if (tmp_off_i < _messages.size() && tmp_off_i > -1)
		{
			_off_i = tmp_off_i;
		}

		_engine.mouseWheelRotation = 0;
	}
}

void Chat::Render()
{
	int chat_y = _engine.ScreenHeight() - height - 5;

	_engine.Fill(0, chat_y - 2, width + 3, chat_y + height, L' ', FG_BLACK);

	int offY = 0;

	//display messages
	for (int y = 0; y + offY < height; y++)
	{
		int off_ii = _off_i + y;
		if (off_ii == _messages.size()) break;

		int author_l = _messages[off_ii].Author().length();
		int content_l = _messages[off_ii].Content().length();

		time_t time = _messages[off_ii].TimeStamp();
		tm* time_converted = gmtime(&time);

		wstring time_str = L'[' + (time_converted->tm_hour < 10 ? (L"0" + to_wstring(time_converted->tm_hour)) : to_wstring(time_converted->tm_hour)) +
			L':' + (time_converted->tm_min < 10 ? (L"0" + to_wstring(time_converted->tm_min)) : to_wstring(time_converted->tm_min)) + L']';

		wstring content = _messages[off_ii].Content();

		//render message
		_engine.DrawString(1, chat_y + y + offY, time_str, FG_DARK_GREY);
		_engine.DrawString(9, chat_y + y + offY, _messages[off_ii].Author() + L':', FG_RED);
		for (int ln = 0; content.size() > 0; ln++)
		{
			wstring output = ln == 0 ? content.substr(0, width - (11 + author_l)) : content.substr(0, width);
			content = content.erase(0, ln == 0 ? width - (11 + author_l) : width);

			if (ln > 0) offY++;
			if (y + offY >= height) break;

			_engine.DrawString(ln == 0 ? (11 + author_l) : 1, chat_y + y + offY, output, FG_GREY);
		}
	}

	//scrollbar
	{
		int max_vertical = _messages.size() - height;
		double closest_value = 999;
		int closest_at_y = 0;

		int vertical_bar_pos = _messages.size() - (_off_i + height);
		if (vertical_bar_pos < 0) vertical_bar_pos = 0;

		for (int y = 0; y < height + 1; y++)
		{
			double delta = abs((max_vertical * (y + 1)) / (height + 1) - vertical_bar_pos);

			if (delta < closest_value)
			{
				closest_at_y = y;
				closest_value = delta;
			}

			_engine.Draw(width + 1, chat_y + y - 1, L'|', FG_WHITE);
		}

		_engine.Draw(width + 1, chat_y + height - closest_at_y - 1, L'O', FG_WHITE);
	}

	if (_engine.keyboard.receive_input)
	{
		_engine.DrawString(
			1, 
			chat_y + height + 3, 
			_engine.keyboard.input.size() > width ? _engine.keyboard.input.substr(_engine.keyboard.input.size() - width) : _engine.keyboard.input,
			FG_GREY);

		auto current = std::chrono::system_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(current - _engine.keyboard.last_blink);

		if (ms.count() < 500)
		{
			_engine.Draw(1 + (_engine.keyboard.input.size() > width ? width : _engine.keyboard.input.size()), chat_y + height + 3, L'│', FG_WHITE);
		}
		else if (ms.count() >= 1000)
		{
			_engine.keyboard.last_blink = std::chrono::system_clock::now();
		}
	}

}

void Chat::EnterInputMode()
{
	_engine.keyboard.input.clear();
	_engine.keyboard.last_blink = std::chrono::system_clock::now();
	_engine.keyboard.receive_input = true;
}

void Chat::ExitInputMode(const bool& sendInput)
{
	if (_engine.keyboard.input.size() > 0 && sendInput)
	{
		auto chrono_time = std::chrono::system_clock::now();
		time_t time = std::chrono::system_clock::to_time_t(chrono_time);
		AddMessage(Message(L"LtLi0n", time, _engine.keyboard.input));
	}

	_engine.keyboard.input.clear();
	_engine.keyboard.receive_input = false;
}