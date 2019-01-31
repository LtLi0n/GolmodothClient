#pragma once
#define _CRT_SECURE_NO_WARNINGS 1 

#include <vector>

#include "..\Engine\ConsoleEngine.h"
#include "..\Engine\KeyboardInputManager.h"
#include "Message.h"

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
	ConsoleEngine& _engine;
	std::vector<Message> _messages;
	int _off_i;
};

