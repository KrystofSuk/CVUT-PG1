#pragma once

#include "pch.h"

struct Exporter {
	virtual void Export(const std::string & _path, int* _img, std::pair<int, int> _size) const = 0;
};