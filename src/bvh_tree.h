#pragma once

#include "pch.h"
#include "vec3.h"
#include "triangle.h"
#include "ray.h"
#include "aabb.h"
#include "bvh_node.h"

struct BVHTree {
	BVHNode* root;

	BVHTree();
	BVHTree(std::vector<Triangle *>& _triangles);

	void RecursiveBuildup(std::vector<AABB>& _boxes, BVHNode* _parent);
	bool Hit(Ray& _ray, HitResult& _hit, bool _culling, float _minimum = 0);
};
