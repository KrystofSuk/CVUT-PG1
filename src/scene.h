#pragma once

#include "pch.h"
#include "loader.h"
#include "camera.h"
#include "bvh_tree.h"
#include "light.h"
#include "material.h"

struct Scene {
	std::vector<Triangle*> triangles;
	std::vector<Light*> lights;
	std::vector<Light*> areaLights;
	std::vector<Material*> materials;

	BVHTree bvh;
	Camera camera;

	Scene();

	std::vector<Light*> GenerateLightsInTriangle(Triangle* _triangle);

	void LoadMetaFile(const std::string& _path);
	void LoadGeometry(const std::vector<std::string>& _paths, const Loader& _loader);
};