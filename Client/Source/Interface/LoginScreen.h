#pragma once

#include "../Network/TlsClient.h"
#include "../Engine/ConsoleEngine.h"
#include "Button.h"


class LoginScreen
{
public:
	LoginScreen(ConsoleEngine& engine, TlsClient& tls);
	void Update();
	void Logout();
	const bool LoggedIn() const;
	const std::wstring Username() const;

private:
	ConsoleEngine& m_engine;
	TlsClient& m_tls;
	bool m_logged_in;
	int m_focused;
	Button m_login_btn;

	std::wstring m_username;
	std::wstring m_password;
	std::wstring m_server_err;

	void Login();
	//void RenderLoginButton(ConsoleEngine& engine, InterfaceObject& obj);
};