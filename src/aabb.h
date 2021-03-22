#pragma once

#include "pch.h"
#include "triangle.h"
#include "hit_result.h"

struct AABB {
	Vec3 min, max;
	Triangle* triangle = nullptr;

	AABB();
	AABB(Triangle* _triangle);
	AABB(AABB& _aabb);
	AABB(std::vector<AABB>& _boxes);
	AABB(const AABB& _aabb);

	float Midpoint(int axis);

	bool Hit(Ray& _ray);
};