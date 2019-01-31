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

	const std::wstring& GetDisplay();
	const short& GetColor();
	const bool& Walkable();
	const std::wstring& GetConnectionHorizontal();
	const std::wstring& GetConnectionVertical();

private:
	std::wstring _display;
	short _color;
	bool _walkable;
	std::wstring _connection_horizontal;
	std::wstring _connection_vertical;
};

