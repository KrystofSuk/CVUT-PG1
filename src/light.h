#pragma once

#include "vec3.h"

struct Light
{
	Vec3 color;
	Vec3 position;
	Vec3 normal;
	double area;
};