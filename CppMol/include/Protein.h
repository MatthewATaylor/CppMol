#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <cctype>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "AminoAcid.h"
#include "Atom.h"
#include "HTTPConnection.h"
#include "graphics/SphereTemplate.h"
#include "graphics/Model.h"

class Protein {
private:
	void removeSpaces(std::string &str);

public:
	std::vector<const AminoAcid*> sequence;
	std::vector<Atom> atoms;

	Protein();
	Protein(std::string sequence);
	Protein(const std::vector<const AminoAcid*> &sequence);

	//Load protein sequence from an online PDB file
	void loadPDB(std::string url);

	//Create graphical model of protein
	void genModel();

	//Clear vectors
	void reset();

	friend std::ostream &operator<<(std::ostream &os, Protein &protein);
};
