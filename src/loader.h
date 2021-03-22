#pragma once

#include "pch.h"
#include "triangle.h"

struct Loader
{
	virtual std::vector<Triangle*> Load(const std::string& _path, std::vector<Material*>& _materials) const = 0;
};