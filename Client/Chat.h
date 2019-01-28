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

private:
	std::vector<Message> _messages;
	ConsoleEngine* _engine;
	int _off_i;
};

