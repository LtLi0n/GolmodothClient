#include "Player.h"

Player::Player(olcConsoleGameEngineGLOOP& engine)
{
	_engine = &engine;
}

void Player::Update()
{
	//NORTH
	if ((_engine->GetKey(VK_UP).bPressed || _engine->GetKey(0x57).bPressed) && position.y > 0)
	{
		if (scene->GetTile(position.x, position.y - 1).Walkable())
		{
			position.y--;
		}
	}
	//EAST
	else if (_engine->GetKey(VK_RIGHT).bPressed || _engine->GetKey(0x44).bPressed)
	{
		if (scene->GetWidth() - 1 > position.x)
		{
			if (scene->GetTile(position.x + 1, position.y).Walkable())
			{
				position.x++;
			}
		}
	}
	//SOUTH
	else if (_engine->GetKey(VK_DOWN).bPressed || _engine->GetKey(0x53).bPressed)
	{
		if (scene->GetHeight() - 1 > position.y)
		{
			if (scene->GetTile(position.x, position.y + 1).Walkable())
			{
				position.y++;
			}
		}
	}
	//WEST
	else if ((_engine->GetKey(VK_LEFT).bPressed || _engine->GetKey(0x41).bPressed) && position.x > 0)
	{
		if (scene->GetTile(position.x - 1, position.y).Walkable())
		{
			position.x--;
		}
	}

	scene->Update(*this);
}