#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <cctype>
#include <vector>

#include "HTTPConnection.h"
#include "MoleculeData.h"
#include "AminoAcid.h"
#include "Helix.h"
#include "Sheet.h"
#include "DisulfideBond.h"
#include "Atom.h"
#include "math/Vec.h"

class PDBFile : public MoleculeData {
private:
	static void removeSpaces(std::string &str);

public:
	PDBFile(const std::string &url);
};
