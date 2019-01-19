#pragma once

#include "Vector2.h"

class OtherPlayer
{
public:
	OtherPlayer(const int& id);
	~OtherPlayer();

	Vector2 position;
	int GetID() const { return _id; }

private:
	int _id;
};

