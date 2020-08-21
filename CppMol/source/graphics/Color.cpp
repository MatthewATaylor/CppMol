#include "graphics/Color.h"

const Color Color::HELIX_COLOR_ALPHA(1.0f, 0.0f, 0.5f);
const Color Color::HELIX_COLOR_3_10(0.63f, 0.0f, 0.5f);
const Color Color::HELIX_COLOR_PI(0.38f, 0.0f, 0.5f);
const Color Color::HELIX_COLOR_UNDEFINED(0.7f, 0.7f, 0.7f);
const Color Color::SHEET_COLOR(1.0f, 0.78f, 0.0f);

Color::Color(float r, float g, float b) :
	r(r), g(g), b(b) {}

Color Color::fromByte(unsigned char r, unsigned char g, unsigned char b) {
	return Color(r / 255.0f, g / 255.0f, b / 255.0f);
}

Color Color::fromName(const std::string &name) {
	if (name == "red") {
		return Color(1.0f, 0.0f, 0.0f);
	}
	if (name == "green") {
		return Color(0.0f, 1.0f, 0.0f);
	}
	if (name == "blue") {
		return Color(0.0f, 0.0f, 1.0f);
	}
	if (name == "orange") {
		return Color(1.0f, 0.5f, 0.0f);
	}
	if (name == "yellow") {
		return Color(1.0f, 1.0f, 0.0f);
	}
	if (name == "purple") {
		return Color(1.0f, 0.0f, 1.0f);
	}
	if (name == "white") {
		return Color(1.0f, 1.0f, 1.0f);
	}
	if (name == "light-gray") {
		return Color(0.7f, 0.7f, 0.7f);
	}
	if (name == "dark-gray") {
		return Color(0.3f, 0.3f, 0.3f);
	}
	if (name == "brown") {
		return Color(0.57f, 0.36f, 0.19f);
	}
	if (name == "black") {
		return Color(0.0f, 0.0f, 0.0f);
	}
	throw std::invalid_argument("Invalid color name");
}

Color Color::fromElement(const std::string &element) {
	if (element == "C") {
		return Color(0.39f, 0.39f, 0.39f);
	}
	if (element == "H") {
		return Color(1.0f, 1.0f, 1.0f);
	}
	if (element == "N") {
		return Color(0.0f, 0.0f, 1.0f);
	}
	if (element == "O") {
		return Color(1.0f, 0.0f, 0.0f);
	}
	if (element == "P") {
		return Color(1.0f, 0.5f, 0.0f);
	}
	if (element == "S") {
		return Color(1.0f, 1.0f, 0.0f);
	}

	throw std::invalid_argument("Unknown color for element: " + element);
}

Color Color::fromStructure(const Atom *atom, const MoleculeData *moleculeData) {
	//Check if atom is in a helix
	for (size_t i = 0; i < moleculeData->helices.size(); ++i) {
		if (atom->chain == moleculeData->helices[i].chain &&
			atom->residueNum >= moleculeData->helices[i].residueStart &&
			atom->residueNum <= moleculeData->helices[i].residueEnd) {

			switch (moleculeData->helices[i].type) {
			//Alpha
			case 1:
			case 6:
				return HELIX_COLOR_ALPHA;
			//3/10
			case 5:
				return HELIX_COLOR_3_10;
			//Pi
			case 3:
				return HELIX_COLOR_PI;
			default:
				return HELIX_COLOR_UNDEFINED;
			}

			break;
		}
	}

	//Check if atom is in a sheet
	for (size_t i = 0; i < moleculeData->sheets.size(); ++i) {
		if (atom->chain == moleculeData->sheets[i].chain &&
			atom->residueNum >= moleculeData->sheets[i].residueStart &&
			atom->residueNum <= moleculeData->sheets[i].residueEnd) {

			return SHEET_COLOR;
		}
	}

	return Color(1.0f, 1.0f, 1.0f);
}

Color Color::fromConnectorStructure(
	const Atom *atom1, const Atom *atom2, const MoleculeData *moleculeData
) {
	/*
	//Atoms can have up to 2 different helix types
	int atom1HelixTypes[] = { -1, -1 };
	int atom2HelixTypes[] = { -1, -1 };

	std::pair<const Atom*, int*> atomsToHelixTypes[] = {
		std::make_pair(atom1, atom1HelixTypes),
		std::make_pair(atom2, atom2HelixTypes)
	};

	//Get atom helix types
	for (size_t i = 0; i < moleculeData->helices.size(); ++i) {
		for (unsigned int j = 0; j < 2; ++j) {
			if (atomsToHelixTypes[j].first->chain == moleculeData->helices[i].chain &&
				atomsToHelixTypes[j].first->residueNum >= moleculeData->helices[i].residueStart &&
				atomsToHelixTypes[j].first->residueNum <= moleculeData->helices[i].residueEnd) {

				if (atomsToHelixTypes[j].second[0] == -1) {
					atomsToHelixTypes[j].second[0] = moleculeData->helices[i].type;
				}
				else {
					atomsToHelixTypes[j].second[1] = moleculeData->helices[i].type;
				}
			}
		}
	}

	//Check if atoms are in a sheet
	bool atom1InSheet = false;
	bool atom2InSheet = false;

	for (size_t i = 0; i < moleculeData->sheets.size(); ++i) {
		if (atom1->chain == moleculeData->sheets[i].chain &&
			atom1->residueNum >= moleculeData->sheets[i].residueStart &&
			atom1->residueNum <= moleculeData->sheets[i].residueEnd) {

			atom1InSheet = true;
		}
		if (atom2->chain == moleculeData->sheets[i].chain &&
			atom2->residueNum >= moleculeData->sheets[i].residueStart &&
			atom2->residueNum <= moleculeData->sheets[i].residueEnd) {

			atom2InSheet = true;
		}
	}

	return Color(1.0f, 1.0f, 1.0f);
	*/

	Color atom1Color = fromStructure(atom1, moleculeData);
	Color atom2Color = fromStructure(atom2, moleculeData);

	if (atom1Color == atom2Color) {
		return atom1Color;
	}
	return Color(1.0f, 1.0f, 1.0f);
}

bool Color::operator==(const Color &color) const {
	return r == color.r && g == color.g && b == color.b;
}
