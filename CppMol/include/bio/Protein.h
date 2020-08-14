#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <cctype>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <stdexcept>

#include "AminoAcid.h"
#include "Atom.h"
#include "HTTPConnection.h"
#include "PDBFile.h"
#include "graphics/SphereTemplate.h"
#include "graphics/Model.h"

class Protein {
public:
	std::vector<const AminoAcid*> sequence;

	Protein();
	Protein(std::string sequence);
	Protein(const std::vector<const AminoAcid*> &sequence);
	Protein(const PDBFile *pdbFile);

	//Clear vectors
	void reset();

	friend std::ostream &operator<<(std::ostream &os, Protein &protein);
};
