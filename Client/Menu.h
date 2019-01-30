#pragma once

#include "ConsoleEngine.h"
#include "Button.h"

class Menu
{
public:
	Menu(ConsoleEngine& engine);
	bool MenuActive() const;
	void Update();

private:
	ConsoleEngine& _engine;
	std::unique_ptr<Button> _button_resume;
	std::unique_ptr<Button> _button_exit;
	bool _menuActive;
};

