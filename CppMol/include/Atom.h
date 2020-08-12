#pragma once

#include <string>

#include "math/Vec.h"

struct Atom {
	std::string name;
	char chain;
	int residueNum;
	Vec3 coords;
	std::string element;
};
