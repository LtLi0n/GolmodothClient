#pragma once

#include "olcConsoleGameEngineGLOOP.h"
#include "Vector3.h"
#include "Scene.h"

class Player
{
public:
	Player(olcConsoleGameEngineGLOOP& engine);
	Vector3 position;
	class Scene* scene;

	void Update();

private:
	olcConsoleGameEngineGLOOP* _engine;
};

