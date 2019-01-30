#pragma once

#include "ConsoleEngine.h"
#include "Menu.h"
#include "Chat.h"

class Interface
{
public:
	Interface(ConsoleEngine& engine);
	Menu& GetMenu();
	Chat& GetChat();

private:
	const ConsoleEngine& _engine;
	Menu _menu;
	Chat _chat;
};

