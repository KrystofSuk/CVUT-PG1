#pragma once

#include "pch.h"
#include "scene.h"
#include "utils.h"
#include "hit_result.h"

struct RenderPass {
	//color ; depth buffers
	virtual void Render(Scene& _scene, std::pair<int*, int*> _buffers, const int& _bounces = 1) = 0;
};