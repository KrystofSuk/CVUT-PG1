#include "depth_pass.h"

void DepthPass::Render(Scene& _scene, std::pair<int*, int*> _buffers, const int& _bounces)
{
	std::cout << "Rendering Pass(Depth): STARTED" << std::endl;

	double res = INFINITY;

	for (int y = 0; y < _scene.camera.height; y++)
	{
		for (int x = 0; x < _scene.camera.width; x++)
		{
			Ray r = _scene.camera.PrimaryRayForPixel(x, y);

			res = INFINITY;

			HitResult hit;
			bool bvhHit = _scene.bvh.Hit(r, hit, true);
			if (bvhHit == true) {
				res = hit.distance;
			}
			else {
				res = 255;
			}

			if (res != INFINITY)
			{
				for (int k = 0; k < 3; ++k)
				{
					_buffers.second[(y * (int)_scene.camera.width + x) * 3 + k] = Clamp(255 - (int)(res * 30), 0, 255);
				}
			}
			else {
				for (int k = 0; k < 3; ++k)
				{
					_buffers.second[(y * (int)_scene.camera.width + x) * 3 + k] = 0;
				}
			}
		}

		std::cout << "Progress [";
		for (int i = 0; i < std::round((y / _scene.camera.height) * 20); i++) {
			std::cout << "#";
		}
		for (int i = 20; i >= std::round((y / _scene.camera.height) * 20); i--) {
			std::cout << "-";
		}
		std::cout << "]" << "\r";
	}

	std::cout << "Progress [#####################]" << std::endl << "Rendering Pass(Depth): FINISHED" << std::endl;
}
