#include "InterfaceObject.h"

InterfaceObject::InterfaceObject(ConsoleEngine& engine, const int& width, const int& height)
{ 
	_engine = &engine;
	_width = width;
	_height = height;
	texture = new wchar_t[_width * _height];
	
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			texture[y * _width + x] = U'o';
		}
	}
}

bool InterfaceObject::MouseOver() const { return _mouseOver; }

void InterfaceObject::Update()
{
	_InternalUpdate();
	OnUpdate();
}

//protected

void InterfaceObject::OnUpdate() { }

//private

void InterfaceObject::_InternalUpdate()
{
	if ((_engine->GetMouseX() >= position.x && _engine->GetMouseX() - _width  < position.x) &&
		(_engine->GetMouseY() >= position.y && _engine->GetMouseY() - _height < position.y))
	{
		_mouseOver = true;

		//click event
		if (_engine->GetMouse(0).bPressed)
		{
			if (OnClick != NULL)
			{
				OnClick();
			}
		}
	}
	else _mouseOver = false;

	//render
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_engine->Draw(x + position.x, y + position.y, texture[y * _width + x], _mouseOver ? FG_WHITE : FG_GREY);
		}
	}
}