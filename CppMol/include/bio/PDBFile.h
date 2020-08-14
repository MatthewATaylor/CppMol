#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <cctype>
#include <vector>
#include <stdexcept>

#include "HTTPConnection.h"
#include "AminoAcid.h"
#include "Helix.h"
#include "Sheet.h"
#include "DisulfideBond.h"
#include "Atom.h"
#include "math/Vec.h"

class PDBFile {
private:
	static void removeSpaces(std::string &str);

public:
	std::vector<const AminoAcid*> sequence;
	std::vector<Helix> helices;
	std::vector<Sheet> sheets;
	std::vector<DisulfideBond> disulfideBonds;
	std::vector<Atom> atoms;

	PDBFile(const std::string &url);
};
