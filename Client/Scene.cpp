#include "Scene.h"

Scene::Scene(olcConsoleGameEngineOOP& engine, const int& width, const int& height)
{
	_engine = &engine;
	_width = width;
	_height = height;
	_tiles = new Tile[_width * _height];

	for (int x = 0; x < _width; x++) _tiles[x] = Tile(TILE_PATH);
	for (int x = 0; x < _width; x++) _tiles[(_height - 1) * _width + x] = Tile(TILE_PATH);
	for (int y = 0; y < _height; y++) _tiles[y * _width] = Tile(TILE_PATH);
	for (int y = 0; y < _height; y++) _tiles[y * _width + _width - 1] = Tile(TILE_PATH);
}

Scene::Scene(olcConsoleGameEngineOOP& engine, const int& width, const int& height, int tileTypeArr[])
{
	_engine = &engine;
	_width = width;
	_height = height;
	_tiles = new Tile[_width * _height];

	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tiles[y * _width + x] = Tile((TileType)tileTypeArr[y * _width + x]);
		}
	}
}

Scene::~Scene()
{
	delete[] _tiles;
}

const Tile Scene::GetTile(const int& x, const int& y) const
{
	return _tiles[y * _width + x];
}

void Scene::SetTile(const int& x, const int& y, const Tile& tile)
{
	_tiles[y * _width + x] = tile;
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
			Tile t = _tiles[y * _width + x];
			_engine->DrawString(x * 3 + x, y * 2, t.GetTileDisplay(), t.GetColor());
			
			TileType type1 = t.GetType();

			//horizontal connections
			if (x < render_width - 1)
			{
				wchar_t connecting_char;
				bool contains_connection = false;

				TileType type2 = _tiles[y * _width + x + 1].GetType();

				if (type1 == TILE_PATH && type2 == TILE_PATH)
				{
					connecting_char = U'-';
					contains_connection = true;
				}
				else if (type1 == TILE_WATER && type2 == TILE_WATER)
				{
					connecting_char = U'o';
					contains_connection = true;
				}

				if (contains_connection)
				{
					_engine->Draw((x + 1) * 3 + x, y * 2, connecting_char, t.GetColor());
				}
			}

			//vertical connections
			if (y < render_height - 1)
			{
				wstring connecting_string;
				bool contains_connection = false;

				TileType type2 = _tiles[(y + 1) * _width + x].GetType();

				if (type1 == TILE_PATH && type2 == TILE_PATH)
				{
					connecting_string = L" | ";
					contains_connection = true;
				}
				else if (type1 == TILE_GRASS && type2 == TILE_GRASS)
				{
					connecting_string = L" x ";
					contains_connection = true;
				}
				else if (type1 == TILE_WATER && type2 == TILE_WATER)
				{
					connecting_string = L"ooo";
					contains_connection = true;
				}

				if (contains_connection)
				{
					_engine->DrawString(x * 3 + x, (y + 1) * 2 - 1, connecting_string, t.GetColor());
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