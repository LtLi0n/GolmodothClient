#pragma once

#include <map>
#include "..\Network\TlsClient.h"
#include "..\Dependencies\json.hpp"
#include "Player.h"
#include "OtherPlayer.h"
#include "Tile.h"
#include "TransportTile.h"

class TransportNode;
class Player;

class Scene
{
public:
	Scene
	(
		ConsoleEngine& engine,
		const int& width,
		const int& height,
		std::shared_ptr<std::map<const int, std::shared_ptr<Tile>>> tileInfo,
		int* tileIds
	);
	~Scene();

	void Update(TlsClient& tls, Player& player);

	std::map<const int, std::unique_ptr<TransportTile>> transport;
	std::map<const int, std::unique_ptr<OtherPlayer>> players;

	void DownloadPlayers(TlsClient& tls, bool request);

	//Set
	void SetTileInfo(const int& id, const Tile& tile);
	void SetTileID(const int& x, const int& y, const int& id);
	//Get
	shared_ptr<Tile> GetTileInfo(const int& x, const int& y);
	shared_ptr<Tile> GetTileInfo(const int& id);
	const int& GetTileID(const int& x, const int& y);
	const int& GetWidth();
	const int& GetHeight();

private:
	ConsoleEngine& _engine;
	int _width;
	int _height;
	std::shared_ptr<std::map<const int, std::shared_ptr<Tile>>> _tileInfo;
	int* _tileIds;
};

