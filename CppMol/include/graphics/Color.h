#pragma once

#include <string>
#include <stdexcept>
#include <utility>

#include "bio/Atom.h"
#include "bio/MoleculeData.h"

class Color {
private:
	static const Color HELIX_COLOR_ALPHA;
	static const Color HELIX_COLOR_3_10;
	static const Color HELIX_COLOR_PI;
	static const Color HELIX_COLOR_UNDEFINED;
	static const Color SHEET_COLOR;

public:
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;

	Color(float r, float g, float b);

	static Color fromByte(unsigned char r, unsigned char g, unsigned char b);
	static Color fromName(const std::string &name);
	static Color fromElement(const std::string &element);
	static Color fromStructure(const Atom *atom, const MoleculeData *moleculeData);
	static Color fromConnectorStructure(
		const Atom *atom1, const Atom *atom2, const MoleculeData *moleculeData
	);

	bool operator==(const Color &color) const;
};
