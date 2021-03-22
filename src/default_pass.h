#pragma once

#include "render_pass.h"
#include "phong.h"

struct DefaultPass : public RenderPass {
	void Render(Scene& _scene, std::pair<int*, int*> _buffers, const int& _bounces = 1) override;

	Vec3 LightRay(Ray& _ray, int _bounce, Scene& _scene, bool _culling = true);
};