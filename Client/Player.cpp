#include "Player.h"
#include "json.hpp"
#include <map>
#include <string>
#include <locale>
#include <codecvt>

using json = nlohmann::json;

Player::Player(olcConsoleGameEngineOOP& engine, TcpClient* tcp)
{
	_engine = &engine;
	_tcp = tcp;
}

void Player::Update()
{
	if (scene == nullptr) return;

	bool isOnTransport = scene->transport.count(position.y * scene->GetWidth() + position.x) == 1;

	//input code
	if (_engine->IsFocused())
	{
		//NORTH
		if ((_engine->GetKey(VK_UP).bPressed || _engine->GetKey(0x57).bPressed))
		{
			bool skip = false;

			//change scenes
			if (isOnTransport)
			{
				TransportNode* tn = scene->transport[position.y * scene->GetWidth() + position.x];
				if (tn->GetDirection() == TRANSPORT_NORTH)
				{
					scene = tn->GetTarget();
					position = tn->GetTargetLocation();
					skip = true;
				}
			}

			//move north
			if (!skip && position.y > 0)
			{
				if (scene->GetTileInfo(position.x, position.y - 1)->Walkable())
				{
					position.y--;
				}
			}
		}
		//EAST
		else if (_engine->GetKey(VK_RIGHT).bPressed || _engine->GetKey(0x44).bPressed)
		{
			bool skip = false;

			//change scenes
			if (isOnTransport)
			{
				TransportNode* tn = scene->transport[position.y * scene->GetWidth() + position.x];
				if (tn->GetDirection() == TRANSPORT_EAST)
				{
					scene = tn->GetTarget();
					position = tn->GetTargetLocation();
					skip = true;
				}
			}

			//move east
			if (!skip && scene->GetWidth() - 1 > position.x)
			{
				if (scene->GetTileInfo(position.x + 1, position.y)->Walkable())
				{
					position.x++;
				}
			}
		}
		//SOUTH
		else if (_engine->GetKey(VK_DOWN).bPressed || _engine->GetKey(0x53).bPressed)
		{
			bool skip = false;

			//change scenes
			if (isOnTransport)
			{
				TransportNode* tn = scene->transport[position.y * scene->GetWidth() + position.x];
				if (tn->GetDirection() == TRANSPORT_SOUTH)
				{
					scene = tn->GetTarget();
					position = tn->GetTargetLocation();
					skip = true;
				}
			}

			//move south
			if (!skip && scene->GetHeight() - 1 > position.y)
			{
				if (scene->GetTileInfo(position.x, position.y + 1)->Walkable())
				{
					position.y++;
				}
			}
		}
		//WEST
		else if ((_engine->GetKey(VK_LEFT).bPressed || _engine->GetKey(0x41).bPressed))
		{
			bool skip = false;

			//change scenes
			if (isOnTransport)
			{
				TransportNode* tn = scene->transport[position.y * scene->GetWidth() + position.x];
				if (tn->GetDirection() == TRANSPORT_WEST)
				{
					scene = tn->GetTarget();
					position = tn->GetTargetLocation();
					skip = true;
				}
			}

			//move west
			if (!skip && position.x > 0)
			{
				if (scene->GetTileInfo(position.x - 1, position.y)->Walkable())
				{
					position.x--;
				}
			}
		}
	}

	scene->Update(*this);
}

void Player::DownloadScene()
{
	_tcp->SendRequest("map.request->tile_info\n");
	_tcp->SendRequest("map.request->tiles\n");

	std::map<const int, Tile*>* tile_info = new std::map<const int, Tile*>();

	//load tile info of the scene
	const Packet* response_tileInfo = _tcp->WaitHeader("map.request->tile_info");
	{	
		json json = json::parse(response_tileInfo->content + 23);

		bool test = json.is_array();

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		for (int i = 0; i < json.size(); i++)
		{
			(*tile_info)[json[i]["id"].get<const int>()] =
				new Tile(
					converter.from_bytes(json[i]["display"].get<std::string>()),
					json[i]["color"].get<short>(),
					json[i]["walkable"].get<bool>(),
					converter.from_bytes(json[i]["connection_horizontal"].get<std::string>()),
					converter.from_bytes(json[i]["connection_vertical"].get<std::string>())
				);
		}
	}

	int* tiles = NULL;
	int map_size_x, map_size_y;

	//load corresponding tile ids of the scene
	const Packet* response_tiles = _tcp->WaitHeader("map.request->tiles");
	{
		std::string map_size_x_str;
		std::string map_size_y_str;

		for (int i = 19; ; i++)
		{
			if (response_tiles->content[i] == 'x') break;
			map_size_x_str += response_tiles->content[i];
		}

		for (int i = 20 + map_size_x_str.size(); ; i++)
		{
			if (response_tiles->content[i] == '\n') break;
			map_size_y_str += response_tiles->content[i];
		}

		map_size_x = std::stoi(map_size_x_str);
		map_size_y = std::stoi(map_size_y_str);

		int map_size = map_size_x * map_size_y;

		tiles = new int[map_size];

		int current_x = 0;
		int current_y = 0;

		for (int i = 21 + map_size_x_str.size() + map_size_y_str.size(); i < strlen(response_tiles->content); i++)
		{
			if (response_tiles->content[i] == '\n')
			{
				current_y++;
				current_x = 0;
			}
			else if (response_tiles->content[i] == ',')
			{
				current_x++;
			}
			else
			{
				tiles[current_y * map_size_y + current_x] = (int)response_tiles->content[i] - 48;
			}
		}
	}

	delete response_tileInfo;
	delete response_tiles;
	delete scene;

	scene = new Scene(*_engine, map_size_x, map_size_y, tile_info, tiles);
}