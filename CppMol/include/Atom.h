#pragma once

#include <string>

#include "math/Vec.h"

struct Atom {
	std::string name;
	Vec3 coords;
	std::string element;
};
