#include "triangle.h"

Triangle::Triangle()
{
}

Triangle::Triangle(const Vertex& _a, const Vertex& _b, const Vertex& _c, const bool& _recalculate) : a(_a), b(_b), c(_c)
{
	center = Vec3((_a.position.x + _b.position.x + _c.position.x) / 3.0, (_a.position.y + _b.position.y + _c.position.y) / 3.0, (_a.position.y + _b.position.y + _c.position.y) / 3.0);

	if (_recalculate) {
		Vec3 n = a.RecalculateNormal(b, c);
		b.normal = n;
		c.normal = n;
	}

	area = Vec3::Cross(_b.position - a.position, _c.position - a.position).length() / 2.0;
}

Triangle::Triangle(const Triangle& _triangle)
{
	a = _triangle.a;
	b = _triangle.b;
	c = _triangle.c;
	center = _triangle.center;
	area = _triangle.area;
}

float Triangle::Intersect(const Ray& ray, const bool& cullback, float& b1, float& b2) const
{
	Vec3 e1(b.position - a.position), e2(c.position - a.position);
	Vec3 pvec = Vec3::Cross(ray.direction, e2);
	float det = Vec3::Dot(e1, pvec);

	if (cullback)
	{
		if (det < epsilon) // ray is parallel to triangle
			return INFINITY;
	}
	else
	{
		if (fabs(det) < epsilon) // ray is parallel to triangle
			return INFINITY;
	}

	float invDet = 1.0f / det;

	// Compute first barycentric coordinate
	Vec3 tvec = ray.origin - a.position;
	b1 = Vec3::Dot(tvec, pvec) * invDet;

	if (b1 < 0.0f || b1 > 1.0f)
		return INFINITY;

	// Compute second barycentric coordinate
	Vec3 qvec = Vec3::Cross(tvec, e1);
	b2 = Vec3::Dot(ray.direction, qvec) * invDet;

	if (b2 < 0.0f || b1 + b2 > 1.0f)
		return INFINITY;

	// Compute t to intersection point
	float t = Vec3::Dot(e2, qvec) * invDet;
	return t;
}

Vec3 Triangle::Normal(const float& b1, const float& b2)
{
	return a.normal * (1 - b1 - b2) + b.normal * b1 + c.normal * b2;
}

Vec3 Triangle::Reflect(const Ray& _ray, const Vec3& _normal)
{
	Vec3 v = -_ray.direction.normalized();
	float nv = std::max(Vec3::Dot(_normal, v), 0.0);
	return (_normal * 2.0 * nv - v).normalized();
}

Vec3 Triangle::Refract(const Ray& _ray, const Vec3& _normal)
{
	Vec3 r = _ray.direction.normalized();
	Vec3 n = _normal.normalized();

	float ndotr = Vec3::Dot(r, n);

	double ior = 1.0 / material->ior;

	if (ndotr < 0) {
		ndotr = -ndotr;
	}
	else {
		n = -n;
		ior = 1.0 / ior;
	}

	double minN = std::min(std::fabs(ndotr), 1.0f);
	double k = 1 - ior * ior * (1.0 - minN * minN);
	if (k < 0.0)
		return Vec3(0);

	return Vec3(r * ior + n * (ior * minN - std::sqrt(k))).normalized();
}
