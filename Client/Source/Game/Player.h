#pragma once

#include "..\Network\TlsClient.h"
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
	Player(ConsoleEngine& engine, TlsClient& tls);
	Vector3 position;
	std::unique_ptr<Scene> scene;
	void Update();
	void DownloadScene(const bool& sendPackets);

	Interface& GetInterface();

private:
	ConsoleEngine& _engine;
	TlsClient& _tls;
	Interface _interface;
};

