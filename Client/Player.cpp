#include "Player.h"
#include "json.hpp"
#include <map>
#include <string>
#include <locale>
#include <codecvt>

using json = nlohmann::json;

Player::Player(ConsoleEngine& engine, TcpClient* tcp)
{
	_engine = &engine;
	_tcp = tcp;

	_menu = new Menu(engine, *this);
	_isInMenu = false;
}

void Player::Update()
{
	if (scene == nullptr) return;

	bool isOnTransport = scene->transport.count(position.y * scene->GetWidth() + position.x) == 1;

	bool position_change = false;
	bool transport_request = false;

	//input code
	if (_engine->IsFocused())
	{
		if (_engine->GetKey(VK_ESCAPE).bPressed)
		{
			_isInMenu = !_isInMenu;
		}

		if(!_isInMenu)
		{
			//NORTH
			if ((_engine->GetKey(VK_UP).bPressed || _engine->GetKey(0x57).bPressed))
			{
				bool skip = false;

				//change scenes
				if (isOnTransport)
				{
					if (scene->transport[position.y * scene->GetWidth() + position.x]->GetDirection() == TRANSPORT_NORTH)
					{
						transport_request = true;
						skip = true;
					}
				}

				//move north
				if (!skip && position.y > 0)
				{
					if (scene->GetTileInfo(position.x, position.y - 1)->Walkable())
					{
						position.y--;
						position_change = true;
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
					if (scene->transport[position.y * scene->GetWidth() + position.x]->GetDirection() == TRANSPORT_EAST)
					{
						transport_request = true;
						skip = true;
					}
				}

				//move east
				if (!skip && scene->GetWidth() - 1 > position.x)
				{
					if (scene->GetTileInfo(position.x + 1, position.y)->Walkable())
					{
						position.x++;
						position_change = true;
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
					if (scene->transport[position.y * scene->GetWidth() + position.x]->GetDirection() == TRANSPORT_SOUTH)
					{
						transport_request = true;
						skip = true;
					}
				}

				//move south
				if (!skip && scene->GetHeight() - 1 > position.y)
				{
					if (scene->GetTileInfo(position.x, position.y + 1)->Walkable())
					{
						position.y++;
						position_change = true;
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
					if (scene->transport[position.y * scene->GetWidth() + position.x]->GetDirection() == TRANSPORT_WEST)
					{
						transport_request = true;
						skip = true;
					}
				}

				//move west
				if (!skip && position.x > 0)
				{
					if (scene->GetTileInfo(position.x - 1, position.y)->Walkable())
					{
						position.x--;
						position_change = true;
					}
				}
			}

			if (position_change)
			{
				Packet pos_update = Packet(PACKET_SEND);
				std::string content = "map.sync->position\nx: " + std::to_string(position.x) + "\ny: " + std::to_string(position.y);
				pos_update.content = content.c_str();
				_tcp->Send(pos_update);
			}

			if (transport_request)
			{
				_tcp->SendRequest("map.request->transport\n");
				DownloadScene(false);
			}
		}
	}

	scene->Update(_tcp, *this);

	if (_isInMenu)
	{
		_menu->Update();
	}
}

void Player::DownloadScene(const bool& sendPackets)
{
	if (sendPackets)
	{
		_tcp->SendRequest("map.request->tile_info\n");
		_tcp->SendRequest("map.request->tiles\n");
		_tcp->SendRequest("map.request->transport_nodes\n");
	}

	std::map<const int, Tile*>* tile_info = new std::map<const int, Tile*>();

	//load tile info of the scene
	const Packet* response_tileInfo = _tcp->WaitHeader("map.request->tile_info");
	{	
		json json = json::parse(response_tileInfo->content + 23);

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

		std::string player_pos_x;
		std::string player_pos_y;

		int offsetter = 0;

		//width
		for (int i = (offsetter = 19); ; i++)
		{
			if (response_tiles->content[i] == 'x') break;
			map_size_x_str += response_tiles->content[i];
		}
		map_size_x = std::stoi(map_size_x_str);

		//height
		for (int i = (offsetter += 1 + map_size_x_str.size()); ; i++)
		{
			if (response_tiles->content[i] == '\n') break;
			map_size_y_str += response_tiles->content[i];
		}
		map_size_y = std::stoi(map_size_y_str);

		//pos_x
		for (int i = (offsetter += 1 + map_size_y_str.size()); ; i++)
		{
			if (response_tiles->content[i] == ' ') break;
			player_pos_x += response_tiles->content[i];
		}

		//pos_y
		for (int i = (offsetter += 1 + player_pos_x.size()); ; i++)
		{
			if (response_tiles->content[i] == '\n') break;
			player_pos_y += response_tiles->content[i];
		}

		position.x = stoi(player_pos_x);
		position.y = stoi(player_pos_y);

		tiles = new int[map_size_x * map_size_y];

		int current_x = 0;
		int current_y = 0;

		int header_length = strlen(response_tiles->content);

		for (int i = (offsetter += 1 + player_pos_y.size()); i < header_length; i++)
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
				tiles[current_y * map_size_x + current_x] = (int)response_tiles->content[i] - 48;
			}
		}
	}

	delete scene;
	scene = new Scene(*_engine, map_size_x, map_size_y, tile_info, tiles);

	//load transport tiles
	const Packet* response_transportNodes = _tcp->WaitHeader("map.request->transport_nodes");
	{
		json json = json::parse(response_transportNodes->content + 29);

		for (int i = 0; i < json.size(); i++)
		{
			scene->transport[json[i]["y"].get<int>() * map_size_x + json[i]["x"].get<int>()] = new TransportTile((TransportDirection)json[i]["direction"].get<int>());
		}

		bool stopHere = true;
	}

	scene->DownloadPlayers(_tcp, true);

	_tcp->DeletePacket(response_tileInfo);
	_tcp->DeletePacket(response_tiles);
	_tcp->DeletePacket(response_transportNodes);
}

void Player::ToggleMenu(bool on)
{
	_isInMenu = on;
}