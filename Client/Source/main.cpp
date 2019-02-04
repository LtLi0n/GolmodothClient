#include "Interface/LoginScreen.h" //redefition madness, need to move up
#include "Engine/ConsoleEngine.h"
#include "Dependencies/json.hpp"
#include "Game/Player.h"
#include "Interface/Button.h"

#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <stdio.h>
#include <Windows.h>

using json = nlohmann::json;

#define RENDER_WIDTH 75
#define RENDER_HEIGHT 35

class Game : public ConsoleEngine
{
public:
	Player player;
	TlsClient tls;

	Game() : 
		tls(),
		player(*this, tls)
	{
		tls.Connect("192.168.1.233", 5000);
		player.DownloadScene(true);
	}

protected:

	virtual bool OnUserCreate()
	{
		this->m_sAppName = L"Golmodoth Online";

		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime)
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), U' ');
		
		player.Update();

		return true;
	}
};

int main()
{	
	Game game;

	ConsoleSettings cs(100, 50, 9, 15);

	if (game.ConstructConsole(cs) == 1) game.Start();
	else system("pause");
}
