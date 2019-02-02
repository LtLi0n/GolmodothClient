#include "Player.h"

#include "..\Dependencies\json.hpp"
#include <map>
#include <string>
#include <locale>
#include <codecvt>

using json = nlohmann::json;

Player::Player(ConsoleEngine& engine, TlsClient& tls) :
	_engine(engine),
	_tls(tls),
	_interface(engine) { }

void Player::Update()
{
	if (scene == nullptr) return;

	bool isOnTransport = scene->transport.count(position.y * scene->GetWidth() + position.x) == 1;

	bool position_change = false;
	bool transport_request = false;

	//input code
	if (_engine.IsFocused())
	{
		if(!_interface.GetMenu().MenuActive())
		{
			if (_engine.GetKey(VK_RETURN).bPressed)
			{
				if (_interface.GetChat().InputMode()) _interface.GetChat().ExitInputMode(true);
				else _interface.GetChat().EnterInputMode();
			}

			if (!_interface.GetChat().InputMode())
			{
				//NORTH
				if ((_engine.GetKey(VK_UP).bPressed || _engine.GetKey(0x57).bPressed))
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
				else if (_engine.GetKey(VK_RIGHT).bPressed || _engine.GetKey(0x44).bPressed)
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
				else if (_engine.GetKey(VK_DOWN).bPressed || _engine.GetKey(0x53).bPressed)
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
				else if ((_engine.GetKey(VK_LEFT).bPressed || _engine.GetKey(0x41).bPressed))
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
			}

			if (position_change)
			{
				Packet pos_update = Packet(PACKET_SEND);
				std::string content = "map.sync->position\nx: " + std::to_string(position.x) + "\ny: " + std::to_string(position.y);
				pos_update.AddContent(content.c_str());
				_tls.Send(pos_update);
			}

			if (transport_request)
			{
				_tls.SendRequest("map.request->transport");
				DownloadScene(false);
			}
		}
	}

	scene->Update(_tls, *this);

	_interface.GetChat().Update();
	_interface.GetChat().Render();

	_interface.GetMenu().Update();
}

void Player::DownloadScene(const bool& sendPackets)
{
	if (sendPackets)
	{
		_tls.SendRequest("map.request->tile_info");
		_tls.SendRequest("map.request->tiles");
		_tls.SendRequest("map.request->transport_nodes");
	}

	std::shared_ptr<std::map<const int, std::shared_ptr<Tile>>> tile_info = std::make_shared<std::map<const int, std::shared_ptr<Tile>>>();

	//load tile info of the scene
	std::shared_ptr<Packet> response_tileInfo = _tls.WaitHeader("map.request->tile_info");
	{	
		json json = json::parse(response_tileInfo->Content() + 23);

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		for (int i = 0; i < json.size(); i++)
		{
			(*tile_info)[json[i]["id"].get<const int>()] = std::make_shared<Tile>(
					converter.from_bytes(json[i]["display"].get<std::string>()),
					json[i]["color"].get<short>(),
					json[i]["walkable"].get<bool>(),
					converter.from_bytes(json[i]["connection_horizontal"].get<std::string>()),
					converter.from_bytes(json[i]["connection_vertical"].get<std::string>()));
		}
	}

	int* tiles = NULL;
	int map_size_x, map_size_y;

	//load corresponding tile ids of the scene
	std::shared_ptr<Packet> response_tiles = _tls.WaitHeader("map.request->tiles");
	{
		std::string map_size_x_str;
		std::string map_size_y_str;

		std::string player_pos_x;
		std::string player_pos_y;

		int offsetter = 0;

		//width
		for (int i = (offsetter = 19); ; i++)
		{
			if (response_tiles->Content()[i] == 'x') break;
			map_size_x_str += response_tiles->Content()[i];
		}
		map_size_x = std::stoi(map_size_x_str);

		//height
		for (int i = (offsetter += 1 + map_size_x_str.size()); ; i++)
		{
			if (response_tiles->Content()[i] == '\n') break;
			map_size_y_str += response_tiles->Content()[i];
		}
		map_size_y = std::stoi(map_size_y_str);

		//pos_x
		for (int i = (offsetter += 1 + map_size_y_str.size()); ; i++)
		{
			if (response_tiles->Content()[i] == ' ') break;
			player_pos_x += response_tiles->Content()[i];
		}

		//pos_y
		for (int i = (offsetter += 1 + player_pos_x.size()); ; i++)
		{
			if (response_tiles->Content()[i] == '\n') break;
			player_pos_y += response_tiles->Content()[i];
		}

		position.x = stoi(player_pos_x);
		position.y = stoi(player_pos_y);

		tiles = new int[map_size_x * map_size_y];

		int current_x = 0;
		int current_y = 0;

		int header_length = strlen(response_tiles->Content());

		for (int i = (offsetter += 1 + player_pos_y.size()); i < header_length; i++)
		{
			if (response_tiles->Content()[i] == '\n')
			{
				current_y++;
				current_x = 0;
			}
			else if (response_tiles->Content()[i] == ',')
			{
				current_x++;
			}
			else
			{
				tiles[current_y * map_size_x + current_x] = (int)response_tiles->Content()[i] - 48;
			}
		}
	}

	scene = std::make_unique<Scene>(_engine, map_size_x, map_size_y, tile_info, tiles);

	//load transport tiles
	std::shared_ptr<Packet> response_transportNodes = _tls.WaitHeader("map.request->transport_nodes");
	{
		json json = json::parse(response_transportNodes->Content() + 29);

		for (int i = 0; i < json.size(); i++)
		{
			scene->transport[json[i]["y"].get<int>() * map_size_x + json[i]["x"].get<int>()] = std::make_unique<TransportTile>((TransportDirection)json[i]["direction"].get<int>());
		}

		bool stopHere = true;
	}

	scene->DownloadPlayers(_tls, true);

	_tls.DeletePacket(response_tileInfo);
	_tls.DeletePacket(response_tiles);
	_tls.DeletePacket(response_transportNodes);
}

Interface& Player::GetInterface() { return _interface; }