#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <optional>

#include "Parser.h"
#include "bio/Atom.h"

class Selection {
public:
	std::optional<int> residue;

	//First has no value: starts at 0, second has no value: ends at last residue
	std::optional<std::pair<std::optional<int>, std::optional<int>>> residueRange;
	
	std::optional<std::string> element;
	std::optional<char> chain;

	Selection();

	void reset();
	void print() const;

	//Parse vector of selection query parameters
	void parseQuery(const std::vector<std::string> &query);

	bool isMatch(const Atom *atom, bool reversed = false) const;
};
