#include "TcpClient.h" //redefition madness, need to move up
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <stdio.h>
#include <Windows.h>

#include "olcConsoleGameEngineOOP.h"
#include "json.hpp"

#include "Player.h"
#include "Button.h"
#include "Scene.h"

using json = nlohmann::json;

class Game : public olcConsoleGameEngineOOP
{
public:
	Player* player;

	Scene* city;
	Scene* outdoors;
	TcpClient* tcp;

	Game()
	{
		tcp = new TcpClient("192.168.1.233", 5000);
		tcp->Start();

		player = new Player(*this, tcp);
		player->DownloadScene();
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

		DrawString(
			player->position.x * 4,
			player->position.y * 2,
			L"[@]",
			FG_RED);

		return true;
	}
};

int main()
{	
	Game game;

	game.ConstructConsole(110, 50, 8, 16);
	game.Start();
}
