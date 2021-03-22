#pragma once

#include "triangle.h"

struct HitResult
{
	float distance = INFINITY;
	Triangle* triangle;
	float b1, b2;
	Vec3 normal;
	Vec3 position;

	HitResult();
};