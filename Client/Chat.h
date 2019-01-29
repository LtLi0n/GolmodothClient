#pragma once
#define _CRT_SECURE_NO_WARNINGS 1 

#include <vector>
#include "Message.h"
#include "ConsoleEngine.h"
#include "KeyboardInputManager.h"

class Chat
{
public:
	Chat(ConsoleEngine& engine);

	void AddMessage(const Message& message);
	void Update();
	void Render();

	int width;
	int height;

	bool InputMode() const { return _engine.keyboard.receive_input; }
	void EnterInputMode();
	void ExitInputMode(const bool& sendInput);

private:
	int _off_i;
	std::vector<Message> _messages;
	ConsoleEngine& _engine;
};

