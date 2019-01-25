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

class Game : public ConsoleEngine
{
public:
	Player* player;

	Scene* city;
	Scene* outdoors;
	TcpClient* tcp;

	Game()
	{
		tcp = new TcpClient("127.0.0.1", 5000);
		tcp->Start();

		player = new Player(*this, tcp);
		player->DownloadScene(true);
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

		player->Update();

		return true;
	}
};

int main()
{	
	Game game;

	game.ConstructConsole(4 * 31, 2 * 25, 9, 15);
	game.Start();
}
