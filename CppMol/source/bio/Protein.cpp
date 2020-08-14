#include "bio/Protein.h"

Protein::Protein() {}

Protein::Protein(std::string sequence) {
	std::stringstream sequenceStream(sequence);
	std::string abbr;
	while (sequenceStream >> abbr) {
		const AminoAcid *aminoAcid = AminoAcid::get(abbr);
		if (aminoAcid) {
			this->sequence.push_back(aminoAcid);
		}
		else {
			std::cout << "INFO > Adding amino acid \"" << abbr << "\" to dictionary. " <<
				"Consider creating a dictionary entry yourself with AminoAcid::set.\n\n";
			const AminoAcid *newAminoAcid = AminoAcid::set(abbr, "", "", "", "");
			if (newAminoAcid) {
				this->sequence.push_back(newAminoAcid);
			}
		}
	}
}

Protein::Protein(const std::vector<const AminoAcid*> &sequence) {
	for (size_t i = 0; i < sequence.size(); ++i) {
		this->sequence.push_back(sequence[i]);
	}
}

Protein::Protein(const PDBFile *pdbFile) {
	for (size_t i = 0; i < pdbFile->sequence.size(); ++i) {
		sequence.push_back(pdbFile->sequence[i]);
	}
}

void Protein::reset() {
	sequence.clear();
}

std::ostream &operator<<(std::ostream &os, Protein &protein) {
	if (protein.sequence.size() == 0) {
		os << "Empty";
	}
	else {
		for (size_t i = 0; i < protein.sequence.size(); ++i) {
			os << protein.sequence[i]->abbr3 << " ";
		}
	}
	return os;
}
