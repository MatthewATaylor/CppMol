#pragma once

#include <bitset>
#include <string>
#include <cctype>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <iomanip>

class AminoAcid {
private:
	static std::unordered_map<std::string, std::shared_ptr<const AminoAcid>> dict;
	
	/*
	0: isPolar
	1: isBasic
	2: isAcidic
	*/
	const std::bitset<3> properties;

	//Warn about overwriting amino acid if name is a key in types
	static void overwriteWarn(const std::string &name);

public:
	const std::string name; //Full name
	const std::string abbr1; //One-letter abbreviation
	const std::string abbr3; //Three-letter abbreviation
	const std::string formula;

	/*
	Arguments:
	- name: full amino acid name (or "" to ignore)
	- abbr1: 1-letter abbreviation (or "" to ignore)
	- abbr3: 3-letter abbreviation (or "" to ignore)
	- formula: chemical formula
	- properties: string of three bits ('0'/'1'), where bit 0 is '1' if the
		amino acid is polar, bit 1 is '1' if the amino acid is basic, and
		bit 2 is '1' if the amino acid is acidic
	*/
	AminoAcid(
		std::string name, std::string abbr1, std::string abbr3,
		std::string formula, std::string properties
	);

	bool isPolar() const;
	bool isBasic() const;
	bool isAcidic() const;
	bool containsSulfur() const;

	/*
	Returns an AminoAcid pointer for the given name (full or abbreviated).
	Returns nullptr if no amino acid matches given name
	*/
	static const AminoAcid *get(const std::string &name);

	/*
	Constructs a new amino acid (returned) and adds it to the dictionary of existing
	amino acids

	Arguments:
		- name: full amino acid name (or "" to ignore)
		- abbr1: 1-letter abbreviation (or "" to ignore)
		- abbr3: 3-letter abbreviation (or "" to ignore)
		- formula: chemical formula
		- properties: string of three bits ('0'/'1'), where bit 0 is '1' if the
			amino acid is polar, bit 1 is '1' if the amino acid is basic, and
			bit 2 is '1' if the amino acid is acidic
	*/
	static const AminoAcid *set(
		std::string name, std::string abbr1, std::string abbr3,
		std::string formula, std::string properties
	);

	//Erases amino acid from dictionary
	static void erase(const AminoAcid *aminoAcid);

	//Erases key from dictionary
	static void erase(const std::string &name);

	//Display the amino acid dictionary
	static void showDict();
};
