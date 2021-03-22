#include "vec3.h"

Vec3::Vec3(double _x, double _y, double _z /*= 0*/) : x(_x), y(_y), z(_z)
{

}

Vec3::Vec3(double _vector) : x(_vector), y(_vector), z(_vector)
{

}

Vec3::Vec3() : x(0), y(0), z(0)
{

}

void Vec3::operator*=(const Vec3& _vector)
{
	x *= _vector.x, y *= _vector.y, z *= _vector.z;
}

void Vec3::operator*=(double _a)
{
	x *= _a, y *= _a, z *= _a;
}

Vec3 Vec3::operator*(const Vec3 _vector) const
{
	return Vec3(x * _vector.x, y * _vector.y, z * _vector.z);
}

Vec3 Vec3::operator*(double _a) const
{
	return Vec3(x * _a, y * _a, z * _a);
}

Vec3 Vec3::operator*(float _a) const
{
	return Vec3(x * _a, y * _a, z * _a);
}

Vec3 Vec3::operator/(const double _r) const
{
	return fabs(_r) > epsilon ? Vec3(x / _r, y / _r, z / _r) : Vec3(0, 0, 0);
}

void Vec3::operator+=(const Vec3& _vector)
{
	x += _vector.x, y += _vector.y, z += _vector.z;
}

Vec3 Vec3::operator+(const Vec3& _vector) const
{
	return Vec3(x + _vector.x, y + _vector.y, z + _vector.z);
}

Vec3 Vec3::operator-() const
{
	return Vec3(-x, -y, -z);
}

Vec3 Vec3::operator-(const Vec3& _vector) const
{
	return Vec3(x - _vector.x, y - _vector.y, z - _vector.z);
}

double& Vec3::operator[](const int& _index)
{
	if (_index == 0)
		return x;
	if (_index == 1)
		return y;
	if (_index == 2)
		return z;
	return x;
}

double Vec3::length() const
{
	return sqrt(x * x + y * y + z * z);
}

double Vec3::average()
{
	return (x + y + z) / 3;
}

Vec3 Vec3::normalized() const
{
	return (*this) / length();
}

double Vec3::Dot(const Vec3& _vector1, const Vec3& _vector2)
{
	return (_vector1.x * _vector2.x + _vector1.y * _vector2.y + _vector1.z * _vector2.z);
}

Vec3 Vec3::Cross(const Vec3& _vector1, const Vec3& _vector2)
{
	return Vec3(_vector1.y * _vector2.z - _vector1.z * _vector2.y, _vector1.z * _vector2.x - _vector1.x * _vector2.z, _vector1.x * _vector2.y - _vector1.y * _vector2.x);
}

Vec3 operator/(const double& _r, const Vec3& _vector)
{
	return Vec3(_r / _vector.x, _r / _vector.y, _r / _vector.z);
}
