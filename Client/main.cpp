#include "TcpClient.h" //redefition madness, need to move up
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <stdio.h>
#include <Windows.h>

#include "ConsoleEngine.h"
#include "json.hpp"

#include "Player.h"
#include "Button.h"
#include "Scene.h"

using json = nlohmann::json;

#define RENDER_WIDTH 75
#define RENDER_HEIGHT 35

class Game : public ConsoleEngine
{
public:
	Player player;
	TcpClient tcp;

	Game() : 
		tcp("192.168.1.233", 5000),
		player(*this, tcp)
	{
		tcp.Start();
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
