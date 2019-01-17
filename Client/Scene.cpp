#include "Scene.h"

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
	for (auto const& pair : *_tileInfo)
	{
		delete pair.second;
	}

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

void Scene::Update(class Player& player)
{
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
				if (GetTileID(x, y) == GetTileID(x, y + 1))
				{
					_engine->DrawString(x * 3 + x, (y + 1) * 2 - 1, t->GetConnectionVertical(), t->GetColor());
				}
			}
		}
	}

	//render transport nodes
	for (pair<const int, TransportNode*>& pair : transport)
	{
		int x = pair.first % _width;
		int y = pair.first / _height;

		_engine->DrawString(x * 3 + x, y * 2, pair.second->GetDisplay(), FG_RED);
	}
}