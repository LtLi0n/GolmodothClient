#pragma once
#define _CRT_SECURE_NO_WARNINGS 1 

#include <vector>

#include "..\Engine\ConsoleEngine.h"
#include "..\Engine\KeyboardInputManager.h"
#include "..\Network\TlsClient.h"
#include "Message.h"

class Chat
{
public:
	Chat(ConsoleEngine& engine, TlsClient& tls);

	void AddMessage(const Message& message);
	void Update();
	void Render();

	int width;
	int height;

	bool InputMode() const;
	void EnterInputMode();
	void ExitInputMode(const bool& sendInput);

private:
	ConsoleEngine& m_engine;
	TlsClient& m_tls;
	std::vector<Message> m_messages;
	int m_off_i;
};

