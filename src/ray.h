#include "utils.h"

struct Ray
{
	Ray(vec3 o_, vec3 d_) : o(o_), d(d_){};
	vec3 o, d;
};