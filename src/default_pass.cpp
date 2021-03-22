#include "default_pass.h"

void DefaultPass::Render(Scene& _scene, std::pair<int*, int*> _buffers, const int& _bounces)
{
	time_t timestamp = time(0);

	std::cout << "Rendering Pass(Default): STARTED" << std::endl;

	double res = INFINITY;


	for (int y = 0; y < _scene.camera.height; y++)
	{
		for (int x = 0; x < _scene.camera.width; x++)
		{
			Ray primaryRay = _scene.camera.PrimaryRayForPixel(x, y);
			Vec3 color = LightRay(primaryRay, _bounces, _scene);
			color *= 255.0;

			_buffers.first[(y * (int)_scene.camera.width + x) * 3 + 0] = Clamp((int)color.x, 0, 255);
			_buffers.first[(y * (int)_scene.camera.width + x) * 3 + 1] = Clamp((int)color.y, 0, 255);
			_buffers.first[(y * (int)_scene.camera.width + x) * 3 + 2] = Clamp((int)color.z, 0, 255);

		}

		std::cout << "Progress [";
		for (int i = 0; i < std::round((y / _scene.camera.height) * 20); i++) {
			std::cout << "#";
		}
		for (int i = 20; i >= std::round((y / _scene.camera.height) * 20); i--) {
			std::cout << "-";
		}
		std::cout << "] (" << y << ") ";

		auto secs = difftime(time(0), timestamp);
		std::cout << (int)(secs / 60.0) << ":" << (int)secs % 60 << "    " << "\r";

	}

	std::cout << "Progress [#####################]" << std::endl << "Rendering Pass(Default): FINISHED" << std::endl;
}

Vec3 DefaultPass::LightRay(Ray& _ray, int _bounce, Scene& _scene, bool _culling)
{
	if (_bounce == 0)
		return Vec3(0);

	Vec3 result = Vec3(0);

	HitResult hit;
	_scene.bvh.Hit(_ray, hit, _culling, 1e-5);

	if (hit.distance == INFINITY)
		return result;

	//point lights
	for (auto light : _scene.lights) {
		Vec3 lightV = (light->position - hit.position).normalized();
		Vec3 viewV = (-_ray.direction).normalized();

		Ray shadowRay(hit.position, lightV);
		HitResult hitShadow;
		bool bvhShadowHit = _scene.bvh.Hit(shadowRay, hitShadow, 1e-5);
		if (bvhShadowHit == true) {
			float d = hitShadow.distance - 1e-5;
			float b = (light->position - hit.position).length();
			if (d > b) {
				result += Phong::CalculateDiffuse(hit.triangle, light, lightV, hit.normal);
				result += Phong::CalculateSpecular(hit.triangle, light, viewV, Ray(hit.position, -lightV), hit.normal);
			}
		}
		else {
			result += Phong::CalculateDiffuse(hit.triangle, light, lightV, hit.normal);
			result += Phong::CalculateSpecular(hit.triangle, light, viewV, Ray(hit.position, -lightV), hit.normal);
		}
	}

	Light* tmpLight = new Light();
	//area lights
	for (auto light : _scene.areaLights) {
		Vec3 lightV = (light->position - hit.position).normalized();
		Vec3 viewV = (-_ray.direction).normalized();

		tmpLight->normal = light->normal;
		tmpLight->area = light->area;
		double w = tmpLight->area * Vec3::Dot(tmpLight->normal, -lightV) / lightSampleRate / lightV.length() / lightV.length();
		if (w < 0)
			w = 0;
		tmpLight->color = light->color * w;
		tmpLight->position = light->position;


		Ray shadowRay(hit.position, lightV);
		HitResult hitShadow;
		bool bvhShadowHit = _scene.bvh.Hit(shadowRay, hitShadow, 1e-5);
		if (bvhShadowHit == true) {
			float d = hitShadow.distance + 1e-5;
			float b = (tmpLight->position - hit.position).length();
			if (d > b) {
				result += Phong::CalculateDiffuse(hit.triangle, tmpLight, lightV, hit.normal);
				result += Phong::CalculateSpecular(hit.triangle, tmpLight, viewV, Ray(hit.position, -lightV), hit.normal);
			}
		}
		else {
			result += Phong::CalculateDiffuse(hit.triangle, tmpLight, lightV, hit.normal);
			result += Phong::CalculateSpecular(hit.triangle, tmpLight, viewV, Ray(hit.position, -lightV), hit.normal);
		}
	}

	delete tmpLight;

	if (hit.triangle->material->specular.length() != 0.0) {
		Ray reflectionRay = Ray(hit.position, Triangle::Reflect(_ray, hit.normal));
		result += hit.triangle->material->specular * LightRay(reflectionRay, _bounce - 1, _scene, _culling);
	}

	if (hit.triangle->material->transmittance.length() != 0.0) {
		Ray refractedRay = Ray(hit.position, hit.triangle->Refract(_ray, hit.normal));
		if (refractedRay.direction.length() > 0)
			result += hit.triangle->material->transmittance * LightRay(refractedRay, _bounce - 1, _scene, !_culling);
	}

	return result;
}
