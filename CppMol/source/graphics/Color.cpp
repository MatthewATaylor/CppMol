#include "graphics/Color.h"

const Color Color::HELIX_COLOR_ALPHA(1.0f, 0.0f, 0.5f);
const Color Color::HELIX_COLOR_3_10(0.63f, 0.0f, 0.5f);
const Color Color::HELIX_COLOR_PI(0.38f, 0.0f, 0.5f);
const Color Color::HELIX_COLOR_UNDEFINED(0.7f, 0.7f, 0.7f);
const Color Color::SHEET_COLOR(1.0f, 0.78f, 0.0f);

Color::Color() {}

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

	throw std::invalid_argument("ERROR > Unknown color for element: " + element);
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

bool Color::operator==(const Color &color) const {
	return r == color.r && g == color.g && b == color.b;
}
