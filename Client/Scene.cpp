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

	int max_render_height = _engine->ScreenHeight() / 2;
	int max_render_width = (_engine->ScreenWidth() + 1) / 4;

	int render_height = max_render_height > _height ? _height : max_render_height;
	int render_width = max_render_width > _width ? _width : max_render_width;

	int map_width = _width;

	int x0 = player.position.x - render_width / 2;
	int x1 = player.position.x + render_width / 2 + render_width % 2;
	int y0 = player.position.y - render_height / 2;
	int y1 = player.position.y + render_height / 2 + render_height % 2;

	//x axis
	if (x0 < 0)
	{
		x1 += abs(x0);
		x0 = 0;
	}
	if (x1 > _width)
	{
		x0 -= x1 - _width;
		x1 = _width;
	}

	//y axis
	if (y0 < 0)
	{
		y1 += abs(y0);
		y0 = 0;
	}
	if (y1 > _height)
	{
		y0 -= y1 - _height;
		y1 = _height;
	}

	int engine_offset_x = (max_render_width - render_width) / 2 + (max_render_width - render_width) % 2;
	int engine_offset_y = (max_render_height - render_height) / 2 + (max_render_height - render_height) % 2;

	//render tiles
	for (int y = y0; y < y1; y++)
	{
		if (y == y1 - 1)
		{
			bool test = true;
		}

		int engine_y = y - y0 + engine_offset_y;

		for (int x = x0; x < x1; x++)
		{
			int engine_x = x - x0 + engine_offset_x;

			Tile* t = GetTileInfo(x, y);
			if (t == nullptr) continue;

			_engine->DrawString(engine_x * 4, engine_y * 2, t->GetDisplay(), t->GetColor());

			//horizontal connections
			if (engine_x < render_width + engine_offset_x - 1)
			{
				if (GetTileID(x, y) == GetTileID(x + 1, y))
				{
					_engine->DrawString(engine_x * 4 + 3, engine_y * 2, t->GetConnectionHorizontal(), t->GetColor());
				}
			}

			//vertical connections
			if (engine_y < render_height + engine_offset_y - 1)
			{
				if (GetTileID(x, y) == GetTileID(x, y + 1) || (GetTileID(x, y) == 2 || GetTileID(x, y) == 3))
				{
					_engine->DrawString(engine_x * 4, engine_y * 2 + 1, t->GetConnectionVertical(), t->GetColor());
				}
				else if (GetTileID(x, y + 1) == 2 || GetTileID(x, y + 1) == 3)
				{
					Tile* tOff = GetTileInfo(x, y + 1);

					_engine->DrawString(engine_x * 4, engine_y * 2 + 1, tOff->GetConnectionVertical(), tOff->GetColor());
				}
			}
		}
	}

	//render transport nodes
	for (pair<const int, TransportTile*>& pair : transport)
	{
		int x = pair.first % _width;
		int y = pair.first / _width;

		if (x > x1 || x < x0) continue;
		if (y > y1 || y < y0) continue;

		_engine->DrawString((x - x0 + engine_offset_x) * 4, (y - y0 + engine_offset_y) * 2, pair.second->GetDisplay(), FG_RED);
	}

	//render players
	for (auto const& pair : players)
	{
		_engine->DrawString((pair.second->position.x - x0 + engine_offset_x) * 4, (pair.second->position.y - y0 + engine_offset_y) * 2, L"[o]", FG_YELLOW);
	}

	//render player
	_engine->DrawString(
		(player.position.x - x0 + engine_offset_x) * 4,
		(player.position.y - y0 + engine_offset_y) * 2,
		L"[@]",
		FG_RED);
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