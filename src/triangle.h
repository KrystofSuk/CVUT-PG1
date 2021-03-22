#pragma once

#include "vertex.h"
#include "ray.h"
#include "material.h"

struct Triangle{
	Vertex a, b, c, center;
	Material* material = nullptr;
	double area = 0;

	Triangle();
	Triangle(const Triangle& _triangle);
	Triangle(const Vertex& _a, const Vertex& _b, const Vertex& _c, const bool& _recalculate = true);

	float Intersect(const Ray& ray, const bool& cullback, float& b1, float& b2) const;

	Vec3 Normal(const float& b1, const float& b2);
	Vec3 Refract(const Ray& _ray, const Vec3& _normal);

	static Vec3 Reflect(const Ray& _ray, const Vec3& _normal);
};