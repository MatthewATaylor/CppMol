#pragma once

#include <vector>

#include "AminoAcid.h"
#include "Helix.h"
#include "Sheet.h"
#include "DisulfideBond.h"
#include "Atom.h"

class MoleculeData {
public:
	std::vector<const AminoAcid*> sequence;
	std::vector<Helix> helices;
	std::vector<Sheet> sheets;
	std::vector<DisulfideBond> disulfideBonds;
	std::vector<Atom> atoms;
};
