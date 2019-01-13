#include "TransportNode.h"

TransportNode::TransportNode(Scene& origin, Scene& target, const Vector3& targetLocation, const TransportDirection& direction)
{
	_origin = &origin;
	_target = &target;
	_targetLocation = targetLocation;
	_direction = direction;
}

wstring TransportNode::GetDisplay() const
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

