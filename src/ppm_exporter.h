#pragma once

#include "exporter.h"

struct PPM_Exporter : public Exporter {
	void Export(const std::string& _path, int* _img, std::pair<int, int> _size) const override;
};