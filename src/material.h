#pragma once

#include "pch.h"
#include "vec3.h"

struct Material {
	std::string name;
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
	Vec3 transmittance;
	Vec3 emission;
	float shininess;
	float ior;
	float dissolve;
};