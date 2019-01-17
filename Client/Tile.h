#pragma once

#include <string>

class Tile
{
public:
	Tile(const std::wstring& display,
		const short& color, 
		const bool& walkable, 
		const std::wstring& connection_horizontal,
		const std::wstring& connection_vertical);

	const std::wstring GetDisplay() const { return _display; }
	const short GetColor() const { return _color; }
	const bool Walkable() const { return _walkable; };
	const std::wstring GetConnectionHorizontal() const { return _connection_horizontal; }
	const std::wstring GetConnectionVertical() const { return _connection_vertical; }

private:
	std::wstring _display;
	short _color;
	bool _walkable;
	std::wstring _connection_horizontal;
	std::wstring _connection_vertical;
};

