#pragma once

#include <string>
#include "..\Vector2.h"

enum TransportDirection { TRANSPORT_NORTH, TRANSPORT_EAST, TRANSPORT_SOUTH, TRANSPORT_WEST, TRANSPORT_PORTAL };

class TransportTile
{
public:
	TransportTile(const TransportDirection& direction);
	std::wstring GetDisplay() const;
	TransportDirection GetDirection() const { return _direction; }
private:
	TransportDirection _direction;
};

