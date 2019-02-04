#pragma once
#define _CRT_SECURE_NO_WARNINGS 1 

#include "..\Network\TlsClient.h"
#include "..\Engine\ConsoleEngine.h"
#include "..\Engine\KeyboardInputManager.h"
#include "Message.h"

#include <vector>

class Chat
{
public:
	Chat(ConsoleEngine& engine, TlsClient& tls);

	void Update();
	void Render();

	int width;
	int height;

	bool InputMode() const;
	void EnterInputMode();
	void ExitInputMode(const bool& sendInput);

private:
	void SendMessage(const std::wstring& content) const;
	void AddMessage(const Message& message);
	ConsoleEngine& m_engine;
	TlsClient& m_tls;
	std::vector<Message> m_messages;
	int m_off_i;

	std::wstring ConvertString(std::string& content) const;
};

