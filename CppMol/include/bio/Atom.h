#pragma once

#include <string>

#include "math/Vec.h"
#include "AminoAcid.h"

struct Atom {
	const std::string name;
	const AminoAcid *aminoAcid;
	const char chain;
	const int residueNum;
	const Vec3 coords;
	const std::string element;
};
