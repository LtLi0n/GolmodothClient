#include "Scene.h"

using json = nlohmann::json;

Scene::Scene
(
	ConsoleEngine& engine,
	const int& width,
	const int& height,
	std::shared_ptr<std::map<const int, std::shared_ptr<Tile>>> tileInfo,
	int* tileIds
) :
	_engine(engine),
	_width(width),
	_height(height),
	_tileInfo(tileInfo),
	_tileIds(tileIds) { }

Scene::~Scene()
{
	delete[] _tileIds;
}

void Scene::Update(TlsClient& tls, Player& player)
{
	if (tls.GetByHeader("map.request->players") != nullptr)
	{
		DownloadPlayers(tls, false);
	}

	int max_render_height = (_engine.ScreenHeight() - player.GetInterface().GetChat().height - 3) / 2;
	int max_render_width = (_engine.ScreenWidth() + 1) / 4;

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

			std::shared_ptr<Tile> t = GetTileInfo(x, y);
			if (t == nullptr) continue;

			_engine.DrawString(engine_x * 4, engine_y * 2, t->GetDisplay(), t->GetColor());

			//horizontal connections
			if (engine_x < render_width + engine_offset_x - 1)
			{
				if (GetTileID(x, y) == GetTileID(x + 1, y))
				{
					_engine.DrawString(engine_x * 4 + 3, engine_y * 2, t->GetConnectionHorizontal(), t->GetColor());
				}
			}

			//vertical connections
			if (engine_y < render_height + engine_offset_y - 1)
			{
				if (GetTileID(x, y) == GetTileID(x, y + 1) || (GetTileID(x, y) == 2 || GetTileID(x, y) == 3))
				{
					_engine.DrawString(engine_x * 4, engine_y * 2 + 1, t->GetConnectionVertical(), t->GetColor());
				}
				else if (GetTileID(x, y + 1) == 2 || GetTileID(x, y + 1) == 3)
				{
					std::shared_ptr<Tile> tOff = GetTileInfo(x, y + 1);

					_engine.DrawString(engine_x * 4, engine_y * 2 + 1, tOff->GetConnectionVertical(), tOff->GetColor());
				}
			}
		}
	}

	//render transport nodes
	for (pair<const int, std::unique_ptr<TransportTile>>& pair : transport)
	{
		int x = pair.first % _width;
		int y = pair.first / _width;

		if (x > x1 || x < x0) continue;
		if (y > y1 || y < y0) continue;

		_engine.DrawString((x - x0 + engine_offset_x) * 4, (y - y0 + engine_offset_y) * 2, pair.second->GetDisplay(), FG_RED);
	}

	//render players
	for (auto const& pair : players)
	{
		_engine.DrawString((pair.second->position.x - x0 + engine_offset_x) * 4, (pair.second->position.y - y0 + engine_offset_y) * 2, L"[o]", FG_YELLOW);
	}

	//render player
	_engine.DrawString(
		(player.position.x - x0 + engine_offset_x) * 4,
		(player.position.y - y0 + engine_offset_y) * 2,
		L"[@]",
		FG_RED);
}

void Scene::DownloadPlayers(TlsClient& tls, bool request)
{
	if (request) tls.SendRequest("map.request->players");

	std::shared_ptr<Packet> packet_players = tls.WaitHeader("map.request->players");

	json json = json::parse(packet_players->Content() + 21);

	players.clear();
	for (int i = 0; i < json.size(); i++)
	{
		int id = json[i]["id"].get<int>();

		players[id] = std::make_unique<OtherPlayer>(id);
		players[id]->position = Vector2(json[i]["x"].get<int>(), json[i]["y"].get<int>());
	}

	tls.DeletePacket(packet_players);
}


//Set
void Scene::SetTileInfo(const int& id, const Tile& tile) { (*_tileInfo)[id] = std::make_shared<Tile>(tile); }
void Scene::SetTileID(const int& x, const int& y, const int& id) { _tileIds[y * _width + x] = id; }
//Get
shared_ptr<Tile> Scene::GetTileInfo(const int& x, const int& y) { int index = _tileIds[y * _width + x]; return _tileInfo->count(index) ? (*_tileInfo)[index] : nullptr; }
shared_ptr<Tile> Scene::GetTileInfo(const int& id) { return _tileInfo->count(id) ? (*_tileInfo)[id] : nullptr; }
const int& Scene::GetTileID(const int& x, const int& y) { return _tileIds[y * _width + x]; }
const int& Scene::GetWidth() { return _width; }
const int& Scene::GetHeight() { return _height; }