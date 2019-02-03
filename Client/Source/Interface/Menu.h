#pragma once

#include "..\Engine\ConsoleEngine.h"
#include "Button.h"

class Menu
{
public:
	Menu(ConsoleEngine& engine);
	void Update();

	bool m_logout_request;
	bool m_menuActive;

private:
	ConsoleEngine& m_engine;
	std::unique_ptr<Button> m_button_resume;
	std::unique_ptr<Button> m_button_logout;
	std::unique_ptr<Button> m_button_exit;
};

