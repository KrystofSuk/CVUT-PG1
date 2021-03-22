#pragma once

#include "pch.h"
#include "aabb.h"

struct BVHNode {
	AABB box;
	BVHNode* left = nullptr;
	BVHNode* right = nullptr;
	bool leaf = false;

	BVHNode();

	static bool sortByAxisX(AABB& lhs, AABB& rhs);
	static bool sortByAxisY(AABB& lhs, AABB& rhs);
	static bool sortByAxisZ(AABB& lhs, AABB& rhs);

	bool Hit(Ray& _ray, HitResult& _hit, float _minimum, bool _culling);
};