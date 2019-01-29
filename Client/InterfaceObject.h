#pragma once

#include "ConsoleEngine.h"
#include "Vector2.h"
#include <functional>

class InterfaceObject
{
public:
	InterfaceObject(ConsoleEngine& engine, const int& width, const int& height);

	const wchar_t* texture;
	Vector2 position;
	bool MouseOver() const;
	void Update();
	virtual void Render();

	int width;
	int height;

	std::function<void()> OnClick;
	std::function<void(ConsoleEngine& engine, InterfaceObject& obj)> OnRender;

protected:
	ConsoleEngine& _engine;
	virtual void OnUpdate();

private:
	bool _mouseOver;

	void _InternalUpdate();
};

