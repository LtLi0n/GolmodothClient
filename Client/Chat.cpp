#include "Chat.h"
#include <chrono>

Chat::Chat(ConsoleEngine* engine)
{
	_engine = engine;
	_off_i = 0;
	width = 40;
	height = 15;

	auto chrono_time = std::chrono::system_clock::now();
	time_t time = std::chrono::system_clock::to_time_t(chrono_time);

	for (int i = 0; i < 5; i++)
	{
		AddMessage(Message(L"LtLi0n", time, L"test1"));
		AddMessage(Message(L"LtLi0n", time, L"test2"));
		AddMessage(Message(L"mild assburger", time, L"test3"));
		AddMessage(Message(L"LtLi0n", time, L"test4"));
		AddMessage(Message(L"LtLi0n", time, L"test5"));
	}
}

void Chat::AddMessage(const Message& message)
{
	_messages.push_back(message);
	if (_messages.size() - _off_i > height) _off_i++;
}

void Chat::Update()
{
	if (_engine->mouseWheelRotation != 0)
	{
		int tmp_off_i = _off_i + (_engine->mouseWheelRotation > 0 ? -1 : 1);

		//offset message display position
		if (tmp_off_i < _messages.size() && tmp_off_i > -1)
		{
			_off_i = tmp_off_i;
		}

		_engine->mouseWheelRotation = 0;
	}
}

void Chat::Render()
{
	int chat_y = _engine->ScreenHeight() - height - 5;

	_engine->Fill(0, chat_y - 2, width + 3, chat_y + height, L' ', FG_BLACK);

	//display messages
	for (int y = 0; y < height; y++)
	{
		int off_ii = _off_i + y;
		if (off_ii == _messages.size()) break;

		int author_l = _messages[off_ii].Author().length();
		int content_l = _messages[off_ii].Content().length();

		time_t time = _messages[off_ii].TimeStamp();
		tm* time_converted = gmtime(&time);

		wstring time_str = L'[' + (time_converted->tm_hour < 10 ? (L"0" + to_wstring(time_converted->tm_hour)) : to_wstring(time_converted->tm_hour)) +
			L':' + (time_converted->tm_min < 10 ? (L"0" + to_wstring(time_converted->tm_min)) : to_wstring(time_converted->tm_min)) + L']';

		_engine->DrawString(1, chat_y + y, time_str, FG_DARK_GREY);
		_engine->DrawString(9, chat_y + y, _messages[off_ii].Author() + L':', FG_RED);
		_engine->DrawString(11 + author_l, chat_y + y, _messages[off_ii].Content().substr(0, width - (11 + author_l)), FG_GREY);
	}

	int max_vertical = _messages.size() - height;
	double closest_value = 999;
	int closest_at_y = 0;

	int vertical_bar_pos = _messages.size() - (_off_i + height);
	if (vertical_bar_pos < 0) vertical_bar_pos = 0;

	for (int y = 0; y < height + 1; y++)
	{
		double delta = abs((max_vertical * (y + 1)) / (height + 1)  - vertical_bar_pos);

		if (delta < closest_value)
		{
			closest_at_y = y;
			closest_value = delta;
		}

		_engine->Draw(width + 1, chat_y + y - 1, L'|', FG_WHITE);
	}

	_engine->Draw(width + 1, chat_y + height - closest_at_y - 1, L'O', FG_WHITE);
}