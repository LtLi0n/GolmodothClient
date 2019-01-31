#include "TransportTile.h"

TransportTile::TransportTile(const TransportDirection& direction)
{
	_direction = direction;
}

std::wstring TransportTile::GetDisplay() const
{
	switch (_direction)
	{

	case TRANSPORT_NORTH: return L"[↑]";
	case TRANSPORT_EAST: return L"[→]";
	case TRANSPORT_SOUTH: return L"[↓]";
	case TRANSPORT_WEST: return L"[←]";
	case TRANSPORT_PORTAL: return L"[?]";

	}
}

