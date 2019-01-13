#pragma once

#include "Scene.h"
#include "Vector2.h"

class Scene;

enum TransportDirection { TRANSPORT_NORTH, TRANSPORT_EAST, TRANSPORT_SOUTH, TRANSPORT_WEST, TRANSPORT_PORTAL };

class TransportNode
{
public:
	TransportNode(Scene& origin, Scene& target, const Vector3& targetLocation, const TransportDirection& direction);
	wstring GetDisplay() const;
	TransportDirection GetDirection() const { return _direction; }
	Scene* GetTarget() const { return _target; }
	Vector3 GetTargetLocation() const { return _targetLocation; }
private:
	Scene* _origin;
	Scene* _target;
	Vector3 _targetLocation;
	TransportDirection _direction;
};

