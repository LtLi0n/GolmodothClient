#pragma once

#include <string>
using namespace std;

enum TileType
{
	TILE_NULL, TILE_PATH, TILE_GRASS, TILE_WATER
};

class Tile
{
public:
	Tile(const TileType& type, const short& color);
	Tile() : _type(TILE_NULL), _color(15) { }
	Tile(const TileType& type);

	TileType GetType() const { return _type; }
	wstring GetTileDisplay() const;
	short GetColor() const { return _color; }
	bool Walkable() const;

private:
	TileType _type;
	short _color;
};

