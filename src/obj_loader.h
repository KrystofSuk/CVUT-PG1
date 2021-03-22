#pragma once

#include "loader.h"

struct ObjLoader : public Loader
{
	std::vector<Triangle*> Load(const std::string& _path, std::vector<Material*>& _materials) const override;
};

