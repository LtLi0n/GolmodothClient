#include "Tile.h"
#include "olcConsoleGameEngineOOP.h"

Tile::Tile(const TileType& type, const short& color)
{
	_type = type;
	_color = color;
}

Tile::Tile(const TileType& type)
{
	_type = type;

	switch (type)
	{

	case TILE_NULL: _color = FG_BLACK; break;
	case TILE_PATH: _color = FG_GREY; break;
	case TILE_GRASS: _color = FG_DARK_GREEN; break;
	case TILE_WATER: _color = FG_CYAN; break;
	default: _color = FG_BLACK; break;

	}
}

wstring Tile::GetTileDisplay() const
{
	switch (_type)
	{

	case TILE_NULL: return L"   ";
	case TILE_PATH: return L"( )";
	case TILE_GRASS: return L"x x";
	case TILE_WATER: return L"ooo";
	default: return L"   ";

	}
}

bool Tile::Walkable() const
{
	return _type != TILE_NULL && _type != TILE_WATER;
}