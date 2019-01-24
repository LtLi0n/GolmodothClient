#pragma once

#include "TcpClient.h"
#include "olcConsoleGameEngineOOP.h"
#include "Vector3.h"
#include "Scene.h"

class TcpClient;
class Scene;

class Player
{
public:
	Player(olcConsoleGameEngineOOP& engine, TcpClient* tcp);
	Vector3 position;
	Scene* scene;

	void Update();

	void DownloadScene(const bool& sendPackets);

private:
	olcConsoleGameEngineOOP* _engine;
	TcpClient* _tcp;
};

