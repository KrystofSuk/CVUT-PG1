#include "scene.h"

Scene::Scene()
{
}

std::vector<Light*> Scene::GenerateLightsInTriangle(Triangle* _triangle) {
	std::srand(std::time(NULL));
	std::vector<Light*> lights;
	for (int i = 0; i < lightSampleRate; i++)
	{
		double b1 = (float)std::rand() / RAND_MAX;
		double b2 = (float)std::rand() / RAND_MAX;
		if (b1 + b2 > 1)
		{
			b1 = 1 - b1;
			b2 = 1 - b2;
		}

		Vec3 pos = _triangle->a.position + (_triangle->b.position - _triangle->a.position) * b1 + (_triangle->c.position - _triangle->a.position) * b2;
		Vec3 norm = (_triangle->Normal(b1, b2)).normalized();

		Light* l = new Light();

		l->color = _triangle->material->emission;
		l->area = _triangle->area;
		l->position = pos + norm * epsilon;
		l->normal = norm;
		lights.push_back(l);
	}
	return lights;
}

void Scene::LoadMetaFile(const std::string& _path)
{
	std::string line;
	std::ifstream config(_path);

	if (config.is_open())
	{
		Vec3 pos;
		Vec3 up;
		Vec3 dir;
		float fov;

		while (getline(config, line))
		{
			std::stringstream ss;
			ss.str(line);
			std::string name;
			double x, y, z, a, b, c;
			ss >> name;
			if (name == "pos") {
				ss >> x >> y >> z;
				pos = Vec3(x, y, z);
			}
			if (name == "up") {
				ss >> x >> y >> z;
				up = Vec3(x, y, z);
			}
			if (name == "dir") {
				ss >> x >> y >> z;
				dir = Vec3(x, y, z);
			}
			if (name == "fov") {
				ss >> x;
				fov = x;
			}
			if (name == "light") {
				ss >> x >> y >> z >> a >> b >> c;

				Light* l = new Light();
				l->color = Vec3(a, b, c);
				l->position = Vec3(x, y, z);

				lights.push_back(l);
			}
		}

		camera = Camera(pos, up, dir, fov, 600, 600);
		config.close();
	}
}

void Scene::LoadGeometry(const std::vector<std::string>& _paths, const Loader& _loader)
{
	for (auto& path : _paths) {
		std::vector<Triangle *> loaded = _loader.Load(path, materials);
		triangles.insert(triangles.end(), loaded.begin(), loaded.end());
	}

	bvh = BVHTree(triangles);

	for (auto triangle : triangles) {
		if (triangle->material->emission.length() > 0) {
			auto generatedLights = GenerateLightsInTriangle(triangle);
			for (auto light : generatedLights) {
				areaLights.push_back(light);
			}
		}
	}


	/*
	
	*/
}

