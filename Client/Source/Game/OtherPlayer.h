#pragma once

#include "..\Vector2.h"

class OtherPlayer
{
public:
	OtherPlayer(const int& id);

	Vector2 position;
	const int& GetID();

private:
	int _id;
};

