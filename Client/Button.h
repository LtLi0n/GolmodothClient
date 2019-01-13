#pragma once

#include "InterfaceObject.h"

class Button : public InterfaceObject
{
public:
	Button(olcConsoleGameEngineOOP& engine, const int& widht, const int& height);

protected:
	void OnUpdate() override;
};

