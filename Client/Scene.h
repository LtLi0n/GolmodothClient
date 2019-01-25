#pragma once

#include "Player.h"
#include "TcpClient.h"
#include "OtherPlayer.h"
#include "Tile.h"
#include "TransportTile.h"
#include "json.hpp"

#include <map>

class TransportNode;
class Player;

class Scene
{
public:
	Scene(ConsoleEngine& engine, const int& width, const int& height, std::map<const int, Tile*>* tileInfo, int tileIds[]);
	~Scene();

	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }

	const int GetTileID(const int& x, const int& y) const;
	void SetTileID(const int& x, const int& y, const int& id);

	Tile* GetTileInfo(const int& id) const;
	Tile* GetTileInfo(const int& x, const int& y) const;
	void SetTileInfo(const int& id, Tile* tile);

	void Update(TcpClient* tcp, Player& player);

	std::map<const int, TransportTile*> transport;
	std::map<const int, OtherPlayer*> players;

	void DownloadPlayers(TcpClient* tcp, bool request);

private:
	ConsoleEngine* _engine;
	int* _tileIds;
	std::map<const int, Tile*>* _tileInfo;

	int _width;
	int _height;
};

