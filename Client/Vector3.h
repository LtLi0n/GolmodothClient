#pragma once
struct Vector3
{
	int x;
	int y;
	int z;

	Vector3() : x(0), y(0), z(0) { }
	Vector3(int x, int y, int z) : x(x), y(y), z(z) { }

	Vector3 operator+(const Vector3& other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	Vector3 operator-(const Vector3& other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	bool operator==(const Vector3& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}
	bool operator!=(const Vector3& other) const 
	{ 
		return !(*this == other); 
	}
};

