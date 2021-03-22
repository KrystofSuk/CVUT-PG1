#include "aabb.h"

AABB::AABB(const AABB& _aabb) : min(Vec3(INFINITY)), max(Vec3(-INFINITY))
{
	min = _aabb.min;
	max = _aabb.max;
	triangle = _aabb.triangle;
}

AABB::AABB(std::vector<AABB>& _boxes) : min(Vec3(INFINITY)), max(Vec3(-INFINITY))
{
	for (auto& box : _boxes)
	{
		for (int i = 0; i < 3; i++)
		{
			min[i] = std::min(min[i], box.min[i]);
			max[i] = std::max(max[i], box.max[i]);
		}
	}
}

AABB::AABB(AABB& _aabb) : min(Vec3(INFINITY)), max(Vec3(-INFINITY))
{
	min = _aabb.min;
	max = _aabb.max;
	triangle = _aabb.triangle;
}

AABB::AABB(Triangle* _triangle) : min(Vec3(INFINITY)), max(Vec3(-INFINITY))
{
	for (int i = 0; i < 3; i++)
	{
		min[i] = std::min(min[i], _triangle->a.position[i]);
		min[i] = std::min(min[i], _triangle->b.position[i]);
		min[i] = std::min(min[i], _triangle->c.position[i]);
		max[i] = std::max(max[i], _triangle->a.position[i]);
		max[i] = std::max(max[i], _triangle->b.position[i]);
		max[i] = std::max(max[i], _triangle->c.position[i]);
	}
	triangle = _triangle;
}

AABB::AABB() : min(Vec3(INFINITY)), max(Vec3(-INFINITY))
{

}

float AABB::Midpoint(int axis)
{
	return (min[axis] + max[axis]) / 2.0;
}

bool AABB::Hit(Ray& _ray)
{
	float tMin = -INFINITY, tMax = INFINITY;
	Vec3 inv = 1.0 / _ray.direction;

	for (int i = 0; i < 3; i++)
	{
		float mi = (min[i] - _ray.origin[i]) * inv[i];
		float mx = (max[i] - _ray.origin[i]) * inv[i];

		if (mi > mx)
			std::swap(mi, mx);

		tMin = std::max(mi, tMin);
		tMax = std::min(mx, tMax);

		if (tMax < tMin) {

			return false;
		}
	}
	return true;
}
