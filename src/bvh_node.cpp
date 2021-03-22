#include "bvh_node.h"

BVHNode::BVHNode()
{

}

bool BVHNode::sortByAxisX(AABB& lhs, AABB& rhs)
{
	return lhs.Midpoint(0) < rhs.Midpoint(0);
}

bool BVHNode::sortByAxisY(AABB& lhs, AABB& rhs)
{
	return lhs.Midpoint(1) < rhs.Midpoint(1);
}

bool BVHNode::sortByAxisZ(AABB& lhs, AABB& rhs)
{
	return lhs.Midpoint(2) < rhs.Midpoint(2);
}

bool BVHNode::Hit(Ray& _ray, HitResult& _hit, float _minimum, bool _culling)
{
	if (!box.Hit(_ray))
		return false;

	if (!leaf) {
		bool l = left->Hit(_ray, _hit, _minimum, _culling);
		bool r = right->Hit(_ray, _hit, _minimum, _culling);
		if (l || r)
			return true;
		return false;
	}
	else {
		float b1, b2;
		float res = box.triangle->Intersect(_ray, _culling, b1, b2);
		if (res == INFINITY || res < _minimum)
			return false;

		if (_hit.distance > res) {
			_hit.distance = res;
			_hit.triangle = box.triangle;
			_hit.b1 = b1;
			_hit.b2 = b2;
			_hit.normal = box.triangle->Normal(b1, b2);
			_hit.position = _ray.origin + _ray.direction * _hit.distance;
		}
		return true;
	}
}
