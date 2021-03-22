#pragma once

#include "triangle.h"
#include "vec3.h"
#include "light.h"
#include "pch.h"
#include "utils.h"

struct Phong
{
	static Vec3 CalculateAmbient(Triangle* _triangle) {
		Vec3 result;
		result += _triangle->material->ambient;
		return result;
	}

	static Vec3 CalculateDiffuse(Triangle* _triangle, Light* _light, const Vec3& _lightVector, const Vec3& _normalVector) {
		Vec3 result;
		result += _light->color * _triangle->material->diffuse * std::max(Vec3::Dot(_lightVector, _normalVector), 0.0) + _triangle->material->emission;
		return result;
	}

	static Vec3 CalculateSpecular(Triangle* _triangle, Light* _light, const Vec3& _viewVector, const Ray& _lightRay, const Vec3& _normalVector) {
		Vec3 result;
		result += _light->color * _triangle->material->specular * std::pow(std::max(Vec3::Dot(_viewVector, Triangle::Reflect(_lightRay, _normalVector)), 0.0), _triangle->material->shininess);
		return result;
	}
};