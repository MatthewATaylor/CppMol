#pragma once

#include <string>

#include "math/Vec.h"

struct Atom {
	const std::string name;
	const std::string residueName;
	const char chain;
	const int residueNum;
	const Vec3 coords;
	const std::string element;
};
