#include "Menu.h"

void RenderResumeButton(ConsoleEngine& engine, InterfaceObject& obj)
{
	engine.DrawString(
		obj.position.x,
		obj.position.y + 1,
		obj.MouseOver() ? L"     → Resume ←         " : L"       Resume           ",
		BG_BLACK + obj.MouseOver() ? FG_GREEN : FG_DARK_GREEN);
}

void RenderLogoutButton(ConsoleEngine& engine, InterfaceObject& obj)
{
	engine.DrawString(
		obj.position.x,
		obj.position.y + 1,
		obj.MouseOver() ? L"     → Logout ←         " : L"       Logout           ",
		BG_BLACK + obj.MouseOver() ? FG_GREEN : FG_DARK_GREEN);
}

void RenderExitButton(ConsoleEngine& engine, InterfaceObject& obj)
{
	engine.DrawString(
		obj.position.x,
		obj.position.y + 1,
		obj.MouseOver() ? L"     → Exit ←           " : L"       Exit             ",
		BG_BLACK + obj.MouseOver() ? FG_GREEN : FG_DARK_GREEN);
}

Menu::Menu(ConsoleEngine& engine) :
	m_engine(engine),
	m_button_resume(std::make_unique<Button>(engine, 24, 3)),
	m_button_logout(std::make_unique<Button>(engine, 24, 3)),
	m_button_exit(std::make_unique<Button>(engine, 24, 3)),
	m_menuActive(false),
	m_logout_request(false)
{
	m_button_resume->OnRender = RenderResumeButton;
	m_button_resume->OnClick = [&]() { m_menuActive = false; };

	m_button_logout->OnRender = RenderLogoutButton;
	m_button_logout->OnClick = [&]() { m_logout_request = true; };


	m_button_exit->OnRender = RenderExitButton;
	m_button_exit->OnClick = []() { exit(0); };
}

void Menu::Update()
{
	if (m_engine.GetKey(VK_ESCAPE).bPressed) m_menuActive = !m_menuActive;

	if (m_menuActive)
	{
		int x1 = m_engine.ScreenWidth() / 4;
		int x2 = m_engine.ScreenWidth() - x1;

		int y1 = m_engine.ScreenHeight() / 6;
		int y2 = m_engine.ScreenHeight() - y1;

		int center_x = (x1 + x2) / 2;

		m_engine.Fill(center_x - 12, y1, center_x + 12, y2, L' ', FG_BLACK);

		m_button_resume->Update();
		m_button_resume->position.x = center_x - 12;
		m_button_resume->position.y = y1 + 4;

		m_button_logout->Update();
		m_button_logout->position.x = center_x - 12;
		m_button_logout->position.y = y2 - 9;

		m_button_exit->Update();
		m_button_exit->position.x = center_x - 12;
		m_button_exit->position.y = y2 - 6;

		m_button_resume->Render();
		m_button_logout->Render();
		m_button_exit->Render();

		m_engine.DrawString(center_x - 12, y1 + 1, L"x--------[Menu]--------x", BG_WHITE + FG_BLACK);
		m_engine.DrawString(center_x - 12, y2 - 2, L"x----------------------x", BG_WHITE + FG_BLACK);

		for (int y = y1 + 2; y < y2 - 2; y++)
		{
			m_engine.Draw(center_x - 12, y, L'|', BG_WHITE + FG_BLACK);
			m_engine.Draw(center_x + 11, y, L'|', BG_WHITE + FG_BLACK);
		}
	}
}
