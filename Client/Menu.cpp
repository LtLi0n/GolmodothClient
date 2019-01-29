#include "Menu.h"

void RenderResumeButton(ConsoleEngine& engine, InterfaceObject& obj)
{
	engine.DrawString(
		obj.position.x,
		obj.position.y + 1,
		obj.MouseOver() ? L"       → Resume ←       " : L"         Resume         ",
		BG_BLACK + obj.MouseOver() ? FG_GREEN : FG_DARK_GREEN);
}

void RenderExitButton(ConsoleEngine& engine, InterfaceObject& obj)
{
	engine.DrawString(
		obj.position.x,
		obj.position.y + 1,
		obj.MouseOver() ? L"        → Exit ←        " : L"          Exit          ",
		BG_BLACK + obj.MouseOver() ? FG_GREEN : FG_DARK_GREEN);
}

Menu::Menu(ConsoleEngine& engine, Player& player) : _engine(engine), _player(player)
{
	_button_resume = new Button(engine, 24, 3);
	_button_resume->OnRender = RenderResumeButton;
	_button_resume->OnClick = [&]() { player.ToggleMenu(false); };

	_button_exit = new Button(engine, 24, 3);
	_button_exit->OnRender = RenderExitButton;
	_button_exit->OnClick = []() { exit(0); };
}

Menu::~Menu()
{
	delete _button_resume;
	delete _button_exit;
}

void Menu::Update()
{
	int x1 = _engine.ScreenWidth() / 4;
	int x2 = _engine.ScreenWidth() - x1;

	int y1 = _engine.ScreenHeight() / 6;
	int y2 = _engine.ScreenHeight() - y1;

	int center_x = (x1 + x2) / 2;

	_engine.Fill(center_x - 12, y1, center_x + 12, y2, L' ', FG_BLACK);

	_button_resume->Update();
	_button_resume->position.x = center_x - 12;
	_button_resume->position.y = y1 + 4;

	_button_exit->Update();
	_button_exit->position.x = center_x - 12;
	_button_exit->position.y = y2 - 6;

	_button_resume->Render();
	_button_exit->Render();

	_engine.DrawString(center_x - 12, y1 + 1, L"x--------[Menu]--------x", BG_WHITE + FG_BLACK);
	_engine.DrawString(center_x - 12, y2 - 2, L"x----------------------x", BG_WHITE + FG_BLACK);

	for (int y = y1 + 2; y < y2 - 2; y++)
	{
		_engine.Draw(center_x - 12, y, L'|', BG_WHITE + FG_BLACK);
		_engine.Draw(center_x + 11, y, L'|', BG_WHITE + FG_BLACK);
	}
}
