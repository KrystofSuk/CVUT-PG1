#pragma once

#include "vec3.h"

struct Ray {
	Vec3 origin, direction;

	Ray(const Vec3& _origin, const Vec3& _direction);
};