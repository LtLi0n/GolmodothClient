#include "TcpClient.h" //redefition madness, need to move up
#include <iostream>
#include <thread>
#include <vector>
#include <string>

#include <stdio.h>
#include <Windows.h>

#include "olcConsoleGameEngineOOP.h"
#include "Player.h"
#include "Button.h"
#include "Scene.h"

class Game : public olcConsoleGameEngineOOP
{
public:
	Player* player;
	Button* button;

	Scene* city;
	Scene* outdoors;

	Game()
	{
		player = new Player(*this);

		button = new Button(*this, 10, 2);
		button->position = { 5, 5 };

		int city_tiles[25 * 25] = 
		{
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1,
			1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1,
			1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1,
			1, 0, 0, 1, 0, 0, 1, 2, 2, 2, 1, 3, 1, 3, 1, 2, 2, 2, 1, 0, 0, 1, 0, 0, 1,
			1, 0, 1, 1, 1, 0, 1, 2, 2, 2, 1, 3, 1, 3, 1, 2, 2, 2, 1, 0, 1, 1, 1, 0, 1,
			1, 0, 1, 0, 1, 0, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 0, 1, 0, 1, 0, 1,
			1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 3, 3, 3, 3, 3, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1,
			1, 0, 0, 1, 0, 0, 1, 3, 3, 1, 3, 1, 1, 1, 3, 1, 3, 3, 1, 0, 0, 1, 0, 0, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 1, 0, 0, 1, 3, 3, 1, 3, 1, 1, 1, 3, 1, 3, 3, 1, 0, 0, 1, 0, 0, 1,
			1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 3, 3, 3, 3, 3, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1,
			1, 0, 1, 0, 1, 0, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 0, 1, 0, 1, 0, 1,
			1, 0, 1, 1, 1, 0, 1, 2, 2, 2, 1, 3, 1, 3, 1, 2, 2, 2, 1, 0, 1, 1, 1, 0, 1,
			1, 0, 0, 1, 0, 0, 1, 2, 2, 2, 1, 3, 1, 3, 1, 2, 2, 2, 1, 0, 0, 1, 0, 0, 1,
			1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1,
			1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1,
			1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
		};
		city = new Scene(*this, 25, 25, city_tiles);

		int outdoor_tiles[11*11] = 
		{
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		};
		outdoors = new Scene(*this, 10, 10, outdoor_tiles);

		city->transport[12] = new TransportNode(*city, *outdoors, Vector3(5, 9, 0), TRANSPORT_NORTH);
		outdoors->transport[95] = new TransportNode(*outdoors, *city, Vector3(12, 0, 0), TRANSPORT_SOUTH);

		player->scene = city;
	}

protected:

	virtual bool OnUserCreate()
	{
		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime)
	{
		m_sAppName = L"X: "s + to_wstring(player->position.x) + L" Y: "s + to_wstring(player->position.y);

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
	TcpClient tcp("127.0.0.1", 5000);
	tcp.Start();

	system("pause");

	//Game game;

	//game.ConstructConsole(110, 50, 8, 16);
	//game.Start();
}
