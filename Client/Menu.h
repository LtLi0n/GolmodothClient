#pragma once

#include "Player.h"
#include "ConsoleEngine.h"
#include "Button.h"

class Player;

class Menu
{
public:
	Menu(ConsoleEngine* engine, Player& player);
	~Menu();
	void Update();

private:
	ConsoleEngine* _engine;
	Player* _player;
	Button* _button_resume;
	Button* _button_exit;
};

