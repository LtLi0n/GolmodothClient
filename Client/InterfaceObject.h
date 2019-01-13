#pragma once

#include "olcConsoleGameEngineGLOOP.h"
#include "Vector2.h"
#include <functional>

class InterfaceObject
{
public:
	InterfaceObject(olcConsoleGameEngineGLOOP& engine, const int& width, const int& height);

	wchar_t* texture;
	Vector2 position;
	bool MouseOver() const;
	void Update();
	function<void()> OnClick;

protected:
	olcConsoleGameEngineGLOOP* _engine;
	virtual void OnUpdate();

private:
	int _width;
	int _height;
	bool _mouseOver;

	void _InternalUpdate();
};

