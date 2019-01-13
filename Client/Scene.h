#pragma once

#include "Player.h"
#include "Tile.h"

class Scene
{
public:
	Scene(olcConsoleGameEngineGLOOP& engine, const int& width, const int& height);
	Scene(olcConsoleGameEngineGLOOP& engine, const int& width, const int& height, int tileTypeArr[]);
	~Scene();

	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }

	const Tile GetTile(const int& x, const int& y) const;
	void SetTile(const int& x, const int& y, const Tile& tile);

	void Update(class Player& player);

private:
	olcConsoleGameEngineGLOOP* _engine;
	Tile* _tiles;

	int _width;
	int _height;
};

