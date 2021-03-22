#pragma once

#include "vec3.h"

struct Vertex
{
	Vec3 position, normal;

	Vertex();
	Vertex(const Vec3& _position);
	Vertex(const Vec3& _position, const Vec3& _normal);

	Vec3 RecalculateNormal(const Vertex& _vertex1, const Vertex& _vertex2);
};