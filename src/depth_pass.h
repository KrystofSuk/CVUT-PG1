#pragma once

#include "render_pass.h"

struct DepthPass : public RenderPass {
	void Render(Scene& _scene, std::pair<int*, int*> _buffers, const int& _bounces = 1) override;
};
