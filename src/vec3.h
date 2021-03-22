#pragma once

#include "pch.h"
#include "const.h"

struct Vec3
{
	double x, y, z;

	Vec3();
	Vec3(double _vector);
	Vec3(double _x, double _y, double _z = 0);
	Vec3 operator*(double _a) const;
	Vec3 operator*(float _a) const;
	Vec3 operator*(const Vec3 _vector) const;
	Vec3 operator/(const double _r) const;
	Vec3 operator+(const Vec3& _vector) const;
	Vec3 operator-(const Vec3& _vector) const;
	Vec3 operator-() const;
	void operator+=(const Vec3& _vector);
	void operator*=(double _a);
	void operator*=(const Vec3& _vector);
	double& operator[](const int& _index);
	double length() const;
	double average();
	Vec3 normalized() const;

	static double Dot(const Vec3& _vector1, const Vec3& _vector2);
	static Vec3 Cross(const Vec3& _vector1, const Vec3& _vector2);

	friend Vec3 operator/(const double& _r, const Vec3& _vector);

	friend std::ostream& operator<<(std::ostream& _os, Vec3 _vector) {
		_os << _vector.x << " " << _vector.y << " " << _vector.z;
		return _os;
	};
};

