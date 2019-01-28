#pragma once
#define _CRT_SECURE_NO_WARNINGS 1 

#include <vector>
#include "Message.h"
#include "ConsoleEngine.h"

class Chat
{
public:
	Chat(ConsoleEngine* engine);

	void AddMessage(const Message& message);
	void Update();
	void Render();

	int width;
	int height;

	bool InputMode() const { return _inputMode; }
	void EnterInputMode();
	void ExitInputMode(const bool& sendInput);

private:
	std::chrono::time_point<std::chrono::system_clock> _last_blink;
	bool _inputMode;
	std::vector<Message> _messages;
	ConsoleEngine* _engine;
	int _off_i;
	std::wstring _input;
};

