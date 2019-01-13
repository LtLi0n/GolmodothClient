#pragma once

#include "olcConsoleGameEngineOOP.h"
#include "Vector3.h"
#include "Scene.h"

class Player
{
public:
	Player(olcConsoleGameEngineOOP& engine);
	Vector3 position;
	class Scene* scene;

	void Update();

private:
	olcConsoleGameEngineOOP* _engine;
};

