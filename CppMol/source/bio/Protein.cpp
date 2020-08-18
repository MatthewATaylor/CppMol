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
				"Consider creating a dictionary entry yourself.\n\n";
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

Protein::Protein(const MoleculeData *moleculeData) {
	for (size_t i = 0; i < moleculeData->sequence.size(); ++i) {
		std::string name = moleculeData->sequence[i].name;
		const AminoAcid *aminoAcid = AminoAcid::get(name);
		if (aminoAcid) {
			sequence.push_back(aminoAcid);
		}
		else {
			std::cout << "INFO > Adding amino acid \"" << name << "\" to dictionary. " <<
				"Consider creating a dictionary entry yourself.\n\n";
			const AminoAcid *newAminoAcid = AminoAcid::set(name, "", "", "", "");
			if (newAminoAcid) {
				this->sequence.push_back(newAminoAcid);
			}
		}
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
