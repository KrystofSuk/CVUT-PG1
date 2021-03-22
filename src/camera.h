#pragma once

#include "vec3.h"
#include "ray.h"

struct Camera 
{
	Vec3 position, up, front, side, d0, m, n;
	double fov, width, height;

	Camera();
	Camera(const Vec3& _position, const Vec3& _up, const Vec3& _front, const double& _fov, const int& _width, const int& _height);

	const Ray PrimaryRayForPixel(const double& _x, const double& _y) const;
};