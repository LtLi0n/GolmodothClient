#include "InterfaceObject.h"

InterfaceObject::InterfaceObject(ConsoleEngine& engine, const int& width, const int& height) :
	width(width),
	height(height),
	_engine(engine) { }

bool InterfaceObject::MouseOver() const { return _mouseOver; }

void InterfaceObject::Update()
{
	_InternalUpdate();
	OnUpdate();
}

void InterfaceObject::Render()
{
	if (OnRender == nullptr)
	{
		//render
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				_engine.Draw(x + position.x, y + position.y, texture[y * width + x], _mouseOver ? FG_WHITE : FG_GREY);
			}
		}
	}
	else
	{
		OnRender(_engine, *this);
	}
}

//protected
void InterfaceObject::OnUpdate() { }

//private

void InterfaceObject::_InternalUpdate()
{
	if ((_engine.GetMouseX() >= position.x && _engine.GetMouseX() - width  < position.x) &&
		(_engine.GetMouseY() >= position.y && _engine.GetMouseY() - height < position.y))
	{
		_mouseOver = true;

		//click event
		if (_engine.GetMouse(0).bPressed)
		{
			if (OnClick != NULL)
			{
				OnClick();
			}
		}
	}
	else _mouseOver = false;
}