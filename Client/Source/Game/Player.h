#pragma once

#include "..\Network\TcpClient.h"
#include "..\Engine\ConsoleEngine.h"
#include "..\Vector3.h"
#include "..\Interface\Interface.h"

#include "Scene.h"

class TcpClient;
class Scene;
class Menu;

class Player
{
public:
	Player(ConsoleEngine& engine, TcpClient& tcp);
	Vector3 position;
	std::unique_ptr<Scene> scene;
	void Update();
	void DownloadScene(const bool& sendPackets);

	Interface& GetInterface();

private:
	ConsoleEngine& _engine;
	TcpClient& _tcp;
	Interface _interface;
};

