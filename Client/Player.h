#pragma once

#include "TcpClient.h"
#include "olcConsoleGameEngineOOP.h"
#include "Vector3.h"
#include "Scene.h"

class TcpClient;

class Player
{
public:
	Player(olcConsoleGameEngineOOP& engine, TcpClient* tcp);
	Vector3 position;
	class Scene* scene;

	void Update();

	void DownloadScene();

private:
	olcConsoleGameEngineOOP* _engine;
	TcpClient* _tcp;
};

