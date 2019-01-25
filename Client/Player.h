#pragma once

#include "TcpClient.h"
#include "ConsoleEngine.h"
#include "Vector3.h"
#include "Scene.h"

class TcpClient;
class Scene;

class Player
{
public:
	Player(ConsoleEngine& engine, TcpClient* tcp);
	Vector3 position;
	Scene* scene;

	void Update();

	void DownloadScene(const bool& sendPackets);

private:
	ConsoleEngine* _engine;
	TcpClient* _tcp;
};

