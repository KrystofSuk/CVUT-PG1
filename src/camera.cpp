#include "camera.h"

Camera::Camera(): width(600), height(600), fov(0.5)
{
}

Camera::Camera(const Vec3& _position, const Vec3& _up, const Vec3& _front, const double& _fov, const int& _width, const int& _height) : position(_position), up(_up), front(_front), fov(_fov), width(_width), height(_height)
{
	side = Vec3::Cross(front, up).normalized();

	double w = 2.0 * tan(fov / 2.0);
	double h = w * _width / _height;

	d0 = front - (side * w / 2) + (up * h / 2);

	m = side * (w / (_width - 1.0));
	n = -up * (h / (_height - 1.0));
}

const Ray Camera::PrimaryRayForPixel(const double& _x, const double& _y) const
{
	Vec3 dir = (d0 + m * _x + n * _y).normalized();

	return Ray(position, dir);
}
