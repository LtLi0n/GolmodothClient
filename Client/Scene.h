#pragma once

#include "Player.h"
#include "Tile.h"
#include "TransportTile.h"

#include <map>

class TransportNode;

class Scene
{
public:
	Scene(olcConsoleGameEngineOOP& engine, const int& width, const int& height, std::map<const int, Tile*>* tileInfo, int tileIds[]);
	~Scene();

	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }

	const int GetTileID(const int& x, const int& y) const;
	void SetTileID(const int& x, const int& y, const int& id);

	Tile* GetTileInfo(const int& id) const;
	Tile* GetTileInfo(const int& x, const int& y) const;
	void SetTileInfo(const int& id, Tile* tile);

	void Update(class Player& player);

	std::map<const int, TransportTile*> transport;

private:
	olcConsoleGameEngineOOP* _engine;
	int* _tileIds;
	std::map<const int, Tile*>* _tileInfo;

	int _width;
	int _height;
};

