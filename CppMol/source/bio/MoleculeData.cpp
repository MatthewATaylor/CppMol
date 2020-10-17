#include "bio/MoleculeData.h"

void MoleculeData::printSequence() {
	if (sequence.empty()) {
		std::cout << "Empty\n\n";
	}
	else {
		for (size_t i = 0; i < sequence.size(); ++i) {
			std::cout << i + 1 << ") " << sequence[i].name << "\tr=" <<
				sequence[i].residueNum << "\tc=" << sequence[i].chain << "\n";
		}
		std::cout << "\n";
	}
}

void MoleculeData::printHelices() {
	if (helices.empty()) {
		std::cout << "Empty\n\n";
	}
	else {
		for (size_t i = 0; i < helices.size(); ++i) {
			std::cout << i + 1 << ") " << helices[i].getName() << "\tr=" <<
				helices[i].residueStart << ":" << helices[i].residueEnd <<
				"\tc=" << helices[i].chain << "\n";
		}
		std::cout << "\n";
	}
}

void MoleculeData::printSheets() {
	if (sheets.empty()) {
		std::cout << "Empty\n\n";
	}
	else {
		for (size_t i = 0; i < sheets.size(); ++i) {
			std::cout << i + 1 << ") " << "r=" << sheets[i].residueStart << ":" <<
				sheets[i].residueEnd << "\tc=" << sheets[i].chain << "\n";
		}
		std::cout << "\n";
	}
}

void MoleculeData::printDisulfideBonds() {
	if (disulfideBonds.empty()) {
		std::cout << "Empty\n\n";
	}
	else {
		for (size_t i = 0; i < disulfideBonds.size(); ++i) {
			std::cout << i + 1 << ") " << disulfideBonds[i].residue1 << 
				disulfideBonds[i].chain1 << ":" << disulfideBonds[i].residue2 << 
				disulfideBonds[i].chain2 << "\n";
		}
		std::cout << "\n";
	}
}

void MoleculeData::printAtoms() {
	if (atoms.empty()) {
		std::cout << "Empty\n\n";
	}
	else {
		for (size_t i = 0; i < atoms.size(); ++i) {
			std::cout << i + 1 << ") " << atoms[i].name << "\tr=" <<
				atoms[i].residueNum << " (" << atoms[i].residueName <<
				")\te=" << atoms[i].element << "\tc=" << atoms[i].chain << "\n";
		}
		std::cout << "\n";
	}
}

void MoleculeData::printChains() {
	if (atoms.empty()) {
		std::cout << "Empty\n\n";
	}
	else {
		for (size_t i = 0; i < chains.size(); ++i) {
			std::cout << i + 1 << ") " << chains[i].identifier <<
				" (" << chains[i].length << " residues)" << "\n";
		}
		std::cout << "\n";
	}
}
