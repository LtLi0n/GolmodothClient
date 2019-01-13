#pragma once

#include "InterfaceObject.h"

class Button : public InterfaceObject
{
public:
	Button(olcConsoleGameEngineGLOOP& engine, const int& widht, const int& height);

protected:
	void OnUpdate() override;
};

