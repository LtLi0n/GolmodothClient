#include "Player.h"

#include "..\Dependencies\json.hpp"
#include <map>
#include <string>
#include <locale>
#include <codecvt>

using json = nlohmann::json;

Player::Player(ConsoleEngine& engine, TlsClient& tls) :
	m_engine(engine),
	m_tls(tls),
	m_interface(engine, tls),
	m_loginScreen(engine, tls) { }

void Player::Update()
{
	if (!m_loginScreen.LoggedIn())
	{
		m_loginScreen.Update();
		return;
	}

	if (scene == nullptr) return;

	bool isOnTransport = scene->transport.count(position.y * scene->GetWidth() + position.x) == 1;

	bool position_change = false;
	bool transport_request = false;

	//input code
	if (m_engine.IsFocused())
	{
		if(!m_interface.GetMenu().m_menuActive)
		{
			if (m_engine.GetKey(VK_RETURN).bPressed)
			{
				if (m_interface.GetChat().InputMode()) m_interface.GetChat().ExitInputMode(true);
				else m_interface.GetChat().EnterInputMode();
			}

			if (!m_interface.GetChat().InputMode())
			{
				//NORTH
				if ((m_engine.GetKey(VK_UP).bPressed || m_engine.GetKey(0x57).bPressed))
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
				else if (m_engine.GetKey(VK_RIGHT).bPressed || m_engine.GetKey(0x44).bPressed)
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
				else if (m_engine.GetKey(VK_DOWN).bPressed || m_engine.GetKey(0x53).bPressed)
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
				else if ((m_engine.GetKey(VK_LEFT).bPressed || m_engine.GetKey(0x41).bPressed))
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
				m_tls.Send(pos_update);
			}

			if (transport_request)
			{
				m_tls.SendRequest("map.request->transport");
				DownloadScene(false);
			}
		}
	}

	scene->Update(m_tls, *this);

	m_interface.GetChat().Update();
	m_interface.GetChat().Render();

	m_interface.GetMenu().Update();
	if (m_interface.GetMenu().m_logout_request)
	{
		m_interface.GetMenu().m_logout_request = false;
		m_interface.GetMenu().m_menuActive = false;
		m_loginScreen.Logout();
	}
}

void Player::DownloadScene(const bool& sendPackets)
{
	if (sendPackets)
	{
		m_tls.SendRequest("map.request->tile_info");
		m_tls.SendRequest("map.request->tiles");
		m_tls.SendRequest("map.request->transport_nodes");
	}

	std::shared_ptr<std::map<const int, std::shared_ptr<Tile>>> tile_info = std::make_shared<std::map<const int, std::shared_ptr<Tile>>>();

	//load tile info of the scene
	std::shared_ptr<Packet> response_tileInfo = m_tls.WaitHeader("map.request->tile_info");
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
	std::shared_ptr<Packet> response_tiles = m_tls.WaitHeader("map.request->tiles");
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

	scene = std::make_unique<Scene>(m_engine, map_size_x, map_size_y, tile_info, tiles);

	//load transport tiles
	std::shared_ptr<Packet> response_transportNodes = m_tls.WaitHeader("map.request->transport_nodes");
	{
		json json = json::parse(response_transportNodes->Content() + 29);

		for (int i = 0; i < json.size(); i++)
		{
			scene->transport[json[i]["y"].get<int>() * map_size_x + json[i]["x"].get<int>()] = std::make_unique<TransportTile>((TransportDirection)json[i]["direction"].get<int>());
		}

		bool stopHere = true;
	}

	scene->DownloadPlayers(m_tls, true);

	m_tls.DeletePacket(response_tileInfo);
	m_tls.DeletePacket(response_tiles);
	m_tls.DeletePacket(response_transportNodes);
}

Interface& Player::GetInterface() { return m_interface; }