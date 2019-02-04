#pragma once

#include "Chat.h"
#include "Menu.h"
#include "..\Engine\ConsoleEngine.h"

class Interface
{
public:
	Interface(ConsoleEngine& engine, TlsClient& tls);
	Menu& GetMenu();
	Chat& GetChat();

private:
	const ConsoleEngine& _engine;
	Menu _menu;
	Chat _chat;
};

