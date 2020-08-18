#pragma once

#include <vector>
#include <iostream>

#include "Residue.h"
#include "Chain.h"
#include "Helix.h"
#include "Sheet.h"
#include "DisulfideBond.h"
#include "Atom.h"

class MoleculeData {
public:
	std::vector<Residue> sequence;
	std::vector<Chain> chains;
	std::vector<Helix> helices;
	std::vector<Sheet> sheets;
	std::vector<DisulfideBond> disulfideBonds;
	std::vector<Atom> atoms;

	void printSequence();
	void printHelices();
	void printSheets();
	void printDisulfideBonds();
	void printAtoms();
	void printChains();
};
