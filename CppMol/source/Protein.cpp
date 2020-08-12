#include "Protein.h"

void Protein::removeSpaces(std::string &str) {
	str.erase(
		std::remove(str.begin(), str.end(), ' '),
		str.end()
	);
}

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

void Protein::loadPDB(std::string url) {
	sequence.clear();
	atoms.clear();

	//Valid extension
	if (url.size() > 4) {
		std::string extension = url.substr(url.size() - 4);
		for (unsigned int i = 0; i < extension.size(); ++i) {
			extension[i] = std::tolower(extension[i]);
		}
		if (extension != ".pdb") {
			std::cerr << "ERROR > Invalid webpage: expecting a PDB file.\n\n";
			return;
		}
	}
	
	HTTPConnection connection;
	if (!connection.get(url)) {
		return;
	}
	
	std::stringstream responseStream(connection.response);
	std::string fileLine;
	const char *sequenceIndicator = "SEQRES";
	const char *atomIndicator = "ATOM  ";
	while (std::getline(responseStream, fileLine)) {
		if (fileLine.size() != 80) {
			reset();
			std::cerr << "ERROR > Invalid PDB file: lines must contain 80 columns.\n\n";
			return;
		}

		std::string firstWord = fileLine.substr(0, 6); //Read first 6 characters of line
		
		//Append current line's residues to protein sequence
		if (firstWord == sequenceIndicator) {
			for (unsigned int i = 19; i < 80; i += 4) {
				if (fileLine[i] == ' ') { //Residue list ended
					break;
				}
				std::string abbr3 = fileLine.substr(i, 3); //Read 3-letter residue abbreviation
				const AminoAcid *residue = AminoAcid::get(abbr3);
				if (residue) {
					sequence.push_back(residue);
				}
				else {
					std::cout << "INFO > Adding amino acid \"" << abbr3 << "\" to dictionary. " <<
						"Consider creating a dictionary entry yourself.\n\n";
					const AminoAcid *newAminoAcid = AminoAcid::set("", "", abbr3, "", "");
					if (newAminoAcid) {
						sequence.push_back(newAminoAcid);
					}
				}
			}
		}
		//Append current line's atom to atoms vector
		else if (firstWord == atomIndicator) {
			std::string name = fileLine.substr(12, 4);
			removeSpaces(name);

			char chain = fileLine[21];

			std::string residueStr = fileLine.substr(22, 4);
			int residueNum;
			try {
				residueNum = std::stoi(residueStr);
			}
			catch (std::invalid_argument) {
				std::cerr << "ERROR > Invalid residue number: " << residueStr << ". " <<
					"Skipping atom...\n\n";
				continue;
			}
			
			std::string xStr = fileLine.substr(30, 8);
			std::string yStr = fileLine.substr(38, 8);
			std::string zStr = fileLine.substr(46, 8);

			removeSpaces(xStr);
			removeSpaces(yStr);
			removeSpaces(zStr);
			
			Vec3 coords(std::stof(xStr), std::stof(yStr), std::stof(zStr));

			std::string element = fileLine.substr(76, 2);
			removeSpaces(element);

			atoms.push_back({ name, chain, residueNum, coords, element });
		}
	}
}

void Protein::genModel() {
	Model::reset();

	if (atoms.size() == 0) {
		return;
	}

	//Gather coordinate information to adjust model size and position
	Vec3 coordSums;
	float maxCoord = 0.0f;
	for (size_t i = 0; i < atoms.size(); ++i) {
		coordSums += atoms[i].coords;
		for (unsigned int j = 0; j < 3; j++) {
			float absCoord = std::abs(atoms[i].coords.get(j));
			if (absCoord > maxCoord) {
				maxCoord = absCoord;
			}
		}
	}
	float coordScale = 8.0f / maxCoord;
	Vec3 coordAverages = coordSums * coordScale / (float)atoms.size();

	struct AlphaCarbonData {
		char chain;
		int residueNum;
		Vec3 coords;
	};

	//Pair of alpha carbon chain identifier and coordinates
	std::vector<AlphaCarbonData> alphaCarbonData;

	for (size_t i = 0; i < atoms.size(); ++i) {
		float r = 0.0f, g = 0.0f, b = 0.0f;
		if (atoms[i].element == "C") {
			r = 0.39f;
			g = 0.39f;
			b = 0.39f;
		}
		else if (atoms[i].element == "N") {
			b = 1.0f;
		}
		else if (atoms[i].element == "O") {
			r = 1.0f;
		}
		else if (atoms[i].element == "S") {
			r = 1.0f;
			g = 1.0f;
		}
		else if (atoms[i].element == "P") {
			r = 1.0f;
			g = 0.5f;
		}
		else {
			std::cerr << "ERROR > Unknown color for element: " << atoms[i].element << "\n\n";
			return;
		}

		Vec3 coords = atoms[i].coords * coordScale - coordAverages;

		if (atoms[i].name == "CA") {
			alphaCarbonData.push_back({
				atoms[i].chain, atoms[i].residueNum, coords
			});
		}

		Model::addSphere(
			coords,
			SphereTemplate::DEFAULT_RADIUS,
			r, g, b
		);
	}

	for (size_t i = 0; i < alphaCarbonData.size() - 1; ++i) {
		//Same chain, no residues skipped
		if (alphaCarbonData[i].chain == alphaCarbonData[i + 1].chain &&
		    (alphaCarbonData[i].residueNum == alphaCarbonData[i + 1].residueNum || 
			alphaCarbonData[i].residueNum + 1 == alphaCarbonData[i + 1].residueNum)) {

			Model::addConnector(
				ConnectorTemplate::DEFAULT_RADIUS,
				0.39f, 0.39f, 0.39f,
				alphaCarbonData[i].coords, alphaCarbonData[i + 1].coords
			);
		}
	}

	Model::genSphereBuffers(false, true, true);
}

void Protein::reset() {
	sequence.clear();
	atoms.clear();
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
