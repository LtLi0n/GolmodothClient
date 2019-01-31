#include "Tile.h"

Tile::Tile(
	const std::wstring& display,
	const short& color,
	const bool& walkable,
	const std::wstring& connection_horizontal,
	const std::wstring& connection_vertical)
{
	_display = display;
	_color = color;
	_walkable = walkable;
	_connection_horizontal = connection_horizontal;
	_connection_vertical = connection_vertical;
}

const std::wstring& Tile::GetDisplay() { return _display; }
const short& Tile::GetColor() { return _color; }
const bool& Tile::Walkable() { return _walkable; }
const std::wstring& Tile::GetConnectionHorizontal() { return _connection_horizontal; }
const std::wstring& Tile::GetConnectionVertical() { return _connection_vertical; }
