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
#include <sstream>

class Game : public olcConsoleGameEngineOOP
{
public:
	Player* player;
	Button* button;

	Scene* city;
	Scene* outdoors;
	TcpClient* tcp;

	Game()
	{
		tcp = new TcpClient("127.0.0.1", 5000);
		tcp->Start();

		Packet p(PACKET_REQUEST);
		p.content = "map.request->tiles\n";
		tcp->Send(p);

		player = new Player(*this);

		button = new Button(*this, 10, 2);
		button->position = { 5, 5 };

		Packet response = tcp->WaitHeader("map.request->tiles");
		std::string map_size_x_str;
		std::string map_size_y_str;

		for (int i = 19; ; i++)
		{
			if (response.content[i] == 'x') break;
			map_size_x_str += response.content[i];
		}

		for (int i = 20 + map_size_x_str.size(); ; i++)
		{
			if (response.content[i] == '\n') break;
			map_size_y_str += response.content[i];
		}

		int map_size_x = std::stoi(map_size_x_str);
		int map_size_y = std::stoi(map_size_y_str);

	    int city_size = map_size_x * map_size_y;

		int* city_tiles = new int[city_size];

		int current_x = 0;
		int current_y = 0;

		for (int i = 21 + map_size_x_str.size() + map_size_y_str.size(); i < strlen(response.content); i++)
		{
			if (response.content[i] == '\n')
			{
				current_y++;
				current_x = 0;
			}
			else if (response.content[i] == ',') current_x++;
			else
			{
				city_tiles[current_y * map_size_y + current_x] = (int)response.content[i] - 48;
			}
		}

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
	Game game;

	game.ConstructConsole(110, 50, 8, 16);
	game.Start();
}
