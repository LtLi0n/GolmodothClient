#include "Player.h"

Player::Player(olcConsoleGameEngineOOP& engine)
{
	_engine = &engine;
}

void Player::Update()
{
	bool isOnTransport = scene->transport.count(position.y * scene->GetWidth() + position.x) == 1;

	//input code
	if (_engine->IsFocused())
	{
		//NORTH
		if ((_engine->GetKey(VK_UP).bPressed || _engine->GetKey(0x57).bPressed))
		{
			bool skip = false;

			//change scenes
			if (isOnTransport)
			{
				TransportNode* tn = scene->transport[position.y * scene->GetWidth() + position.x];
				if (tn->GetDirection() == TRANSPORT_NORTH)
				{
					scene = tn->GetTarget();
					position = tn->GetTargetLocation();
					skip = true;
				}
			}

			//move north
			if (!skip && position.y > 0)
			{
				if (scene->GetTile(position.x, position.y - 1).Walkable())
				{
					position.y--;
				}
			}
		}
		//EAST
		else if (_engine->GetKey(VK_RIGHT).bPressed || _engine->GetKey(0x44).bPressed)
		{
			bool skip = false;

			//change scenes
			if (isOnTransport)
			{
				TransportNode* tn = scene->transport[position.y * scene->GetWidth() + position.x];
				if (tn->GetDirection() == TRANSPORT_EAST)
				{
					scene = tn->GetTarget();
					position = tn->GetTargetLocation();
					skip = true;
				}
			}

			//move east
			if (!skip && scene->GetWidth() - 1 > position.x)
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
			bool skip = false;

			//change scenes
			if (isOnTransport)
			{
				TransportNode* tn = scene->transport[position.y * scene->GetWidth() + position.x];
				if (tn->GetDirection() == TRANSPORT_SOUTH)
				{
					scene = tn->GetTarget();
					position = tn->GetTargetLocation();
					skip = true;
				}
			}

			//move south
			if (!skip && scene->GetHeight() - 1 > position.y)
			{
				if (scene->GetTile(position.x, position.y + 1).Walkable())
				{
					position.y++;
				}
			}
		}
		//WEST
		else if ((_engine->GetKey(VK_LEFT).bPressed || _engine->GetKey(0x41).bPressed))
		{
			bool skip = false;

			//change scenes
			if (isOnTransport)
			{
				TransportNode* tn = scene->transport[position.y * scene->GetWidth() + position.x];
				if (tn->GetDirection() == TRANSPORT_WEST)
				{
					scene = tn->GetTarget();
					position = tn->GetTargetLocation();
					skip = true;
				}
			}

			//move west
			if (!skip && position.x > 0)
			{
				if (scene->GetTile(position.x - 1, position.y).Walkable())
				{
					position.x--;
				}
			}
		}
	}

	scene->Update(*this);
}