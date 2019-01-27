#pragma once

#include "TcpClient.h"
#include "ConsoleEngine.h"
#include "Vector3.h"
#include "Scene.h"
#include "Menu.h"

class TcpClient;
class Scene;
class Menu;

class Player
{
public:
	Player(ConsoleEngine& engine, TcpClient* tcp);
	Vector3 position;
	Scene* scene;

	void Update();

	void DownloadScene(const bool& sendPackets);
	void ToggleMenu(bool on = true);

private:
	ConsoleEngine* _engine;
	TcpClient* _tcp;
	Menu* _menu;
	bool _isInMenu;
};

