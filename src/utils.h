#pragma once

#include "pch.h"

template <typename T>
inline T Clamp(T _value, T _low, T _high)
{
	return std::max(std::min(_value, _high), _low);
}