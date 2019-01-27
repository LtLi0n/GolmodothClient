#include "Tile.h"
#include "ConsoleEngine.h"

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