#include "LoginScreen.h"

#include <chrono>
#include <memory>

void RenderLoginButton(ConsoleEngine& engine, InterfaceObject& obj)
{
	engine.DrawString(obj.position.x + 1, obj.position.y, std::wstring(obj.width - 2 , U'-'), FG_GREEN);
	engine.Draw(obj.position.x, obj.position.y, U'*', FG_GREEN); engine.Draw(obj.position.x + obj.width - 1, obj.position.y, U'*', FG_GREEN);
	engine.Draw(obj.position.x, obj.position.y + 1, U'│', FG_GREEN); engine.Draw(obj.position.x + obj.width - 1, obj.position.y + 1, U'│', FG_GREEN);
	engine.DrawString(obj.position.x + 5, obj.position.y + 1, L"Login", obj.MouseOver() ? FG_WHITE : obj.focused ? FG_WHITE : FG_DARK_GREY);
	engine.DrawString(obj.position.x + 1, obj.position.y + obj.height - 1, std::wstring(obj.width - 2, U'-'), FG_GREEN);
	engine.Draw(obj.position.x, obj.position.y + obj.height - 1, U'*', FG_GREEN); engine.Draw(obj.position.x + obj.width - 1, obj.position.y + obj.height - 1, U'*', FG_GREEN);
}

LoginScreen::LoginScreen(ConsoleEngine& engine, TlsClient& tls) :
	m_engine(engine),
	m_tls(tls),
	m_logged_in(false),
	m_focused(0),
	m_login_btn(engine, 15, 3)
{
	//m_login_btn.OnClick = std::bind(&LoginScreen::Login, *this);
	m_login_btn.OnClick = [this]() 
	{
		m_tls.SendRequest((
			std::string("user.login\n") + 
			std::string(m_username.begin(), m_username.end()) + '\n' +
			std::string(m_password.begin(), m_password.end())).c_str());

		std::shared_ptr<Packet> response_packet = m_tls.WaitHeader("user.login");
		std::string response_str = response_packet->Content() + 10;
		int return_code = std::stoi(response_str);

		switch (return_code)
		{
			case 0: m_server_err = L"Username and password fields cannot be empty."; break;
			case 1: 
				m_logged_in = true; 
				m_engine.keyboard.input = std::wstring();
				m_engine.keyboard.receive_input = false;
				break;
			case 404: m_server_err = L"Account credentials were incorrect."; break;
		}

		m_tls.DeletePacket(response_packet);
	};

	m_login_btn.OnRender = RenderLoginButton;
}

void LoginScreen::Update()
{
	m_engine.keyboard.receive_input = true;

	wstring title = L"Golmodoth # Online";
	m_engine.DrawString(m_engine.ScreenWidth() / 2 - title.length() / 2, 2, title, FG_CYAN);

	m_login_btn.position.x = m_engine.ScreenWidth() / 2 - 7;
	m_login_btn.position.y = m_engine.ScreenHeight() - 6;

	int username_y = m_engine.ScreenHeight() / 2 - 5;
	int password_y = m_engine.ScreenHeight() / 2 - 3;

	//input
	{
		bool input_changed = false;

		if (m_engine.m_keys[VK_RETURN].bPressed)
		{
			m_focused++;
			if (m_focused == 3) m_focused = 0;
			m_login_btn.focused = m_focused == 2;

			input_changed = true;

		}
		else if (m_engine.GetMouse(0).bPressed)
		{
			if (m_engine.GetMouseY() == username_y)
			{
				m_focused = 0;
				input_changed = true;
				m_login_btn.focused = false;
			}
			else if (m_engine.GetMouseY() == password_y)
			{
				m_focused = 1;
				input_changed = true;
				m_login_btn.focused = false;
			}
		}

		if (input_changed)
		{
			switch (m_focused)
			{
				case 0: m_engine.keyboard.input = m_username; break;
				case 1: m_engine.keyboard.input = m_password; break;
			}
		}
	}
	

	switch (m_focused)
	{
		case 0: m_username = m_engine.keyboard.input; break;
		case 1: m_password = m_engine.keyboard.input; break;
	}

	m_engine.DrawString(3, username_y, L"Username:", FG_GREEN);
	m_engine.DrawString(13, username_y, m_username, FG_GREY);

	m_engine.DrawString(3, password_y, L"Password:", FG_GREEN);
	m_engine.DrawString(13, password_y, std::wstring(m_password.length(), '*'));

	//blink
	if (m_focused < 2)
	{
		auto current = std::chrono::system_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(current - m_engine.keyboard.last_blink);

		int x = 13 + (m_focused == 0 ? m_username.length() : m_password.length());
		int y = m_focused == 0 ? username_y : password_y;

		if (ms.count() < 500)
		{
			m_engine.Draw(x, y, U'│', FG_WHITE);
		}
		else if (ms.count() >= 1000)
		{
			m_engine.keyboard.last_blink = std::chrono::system_clock::now();
		}
	}

	m_engine.DrawString(m_engine.ScreenWidth() / 2 - m_server_err.length() / 2, m_login_btn.position.y - 2, m_server_err, FG_RED);

	m_login_btn.Update();
	m_login_btn.Render();
}

void LoginScreen::Logout()
{
	m_logged_in = false;
}

const bool LoginScreen::LoggedIn() const { return m_logged_in; }
const std::wstring LoginScreen::Username() const { return m_username; }
