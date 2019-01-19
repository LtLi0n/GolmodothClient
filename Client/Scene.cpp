#include "Scene.h"

using json = nlohmann::json;

Scene::Scene(olcConsoleGameEngineOOP& engine, const int& width, const int& height, std::map<const int, Tile*>* tileInfo, int tileIds[])
{
	_engine = &engine;
	_width = width;
	_height = height;
	_tileInfo = tileInfo;
	_tileIds = tileIds;
}

Scene::~Scene()
{
	//free up memory
	for (auto const& pair : *_tileInfo) delete pair.second;
	for (auto const& pair : transport) delete pair.second;
	for (auto const& pair : players) delete pair.second;

	delete[] _tileIds;
	delete _tileInfo;
}

const int Scene::GetTileID(const int& x, const int& y) const
{
	return _tileIds[y * _width + x];
}

void Scene::SetTileID(const int& x, const int& y, const int& id)
{
	_tileIds[y * _width + x] = id;
}

Tile* Scene::GetTileInfo(const int& id) const
{
	return (*_tileInfo)[id];
}

Tile* Scene::GetTileInfo(const int& x, const int& y) const
{
	return (*_tileInfo)[_tileIds[y * _width + x]];
}

void Scene::SetTileInfo(const int& id, Tile* tile)
{
	delete (*_tileInfo)[id];
	(*_tileInfo)[id] = tile;
}

void Scene::Update(TcpClient* tcp, Player& player)
{
	if (tcp->GetByHeader("map.request->players") != nullptr)
	{
		DownloadPlayers(tcp, false);
	}

	int render_height = _engine->ScreenHeight() > _height ? _height : _engine->ScreenHeight();
	int render_width = _engine->ScreenWidth() > _width ? _width : _engine->ScreenWidth();

	//render tiles
	for (int y = 0; y < render_height; y++)
	{
		for (int x = 0; x < render_width; x++)
		{
			Tile* t = GetTileInfo(x, y);
			if (t == nullptr) continue;

			_engine->DrawString(x * 3 + x, y * 2, t->GetDisplay(), t->GetColor());

			//horizontal connections
			if (x < render_width - 1)
			{
				if (GetTileID(x, y) == GetTileID(x + 1, y))
				{
					_engine->DrawString((x + 1) * 3 + x, y * 2, t->GetConnectionHorizontal(), t->GetColor());
				}
			}

			//vertical connections
			if (y < render_height - 1)
			{
				if (GetTileID(x, y) == GetTileID(x, y + 1) || (GetTileID(x, y) == 2 || GetTileID(x, y) == 3))
				{
					_engine->DrawString(x * 3 + x, (y + 1) * 2 - 1, t->GetConnectionVertical(), t->GetColor());
				}
				else if (GetTileID(x, y + 1) == 2 || GetTileID(x, y + 1) == 3)
				{
					Tile* tOff = GetTileInfo(x, y + 1);

					_engine->DrawString(x * 3 + x, y * 2 + 1, tOff->GetConnectionVertical(), tOff->GetColor());
				}
			}
		}
	}

	//render transport nodes
	for (pair<const int, TransportTile*>& pair : transport)
	{
		int x = pair.first % _width;
		int y = pair.first / _width;

		_engine->DrawString(x * 3 + x, y * 2, pair.second->GetDisplay(), FG_RED);
	}

	//render players
	for (auto const& pair : players)
	{
		_engine->DrawString(pair.second->position.x * 3 + pair.second->position.x, pair.second->position.y * 2, L"[o]", FG_YELLOW);
	}
}

void Scene::DownloadPlayers(TcpClient* tcp, bool request)
{
	if (request) tcp->SendRequest("map.request->players\n");

	const Packet* packet_players = tcp->WaitHeader("map.request->players");

	json json = json::parse(packet_players->content + 21);

	for (auto const& pair : players) delete pair.second;
	players.clear();

	for (int i = 0; i < json.size(); i++)
	{
		OtherPlayer* op = new OtherPlayer(json[i]["id"].get<int>());
		op->position = Vector2(json[i]["x"].get<int>(), json[i]["y"].get<int>());

		players[op->GetID()] = op;
	}

	tcp->DeletePacket(packet_players);
}