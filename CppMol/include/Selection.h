#pragma once

#include <string>
#include <utility>

struct Selection {
	enum {
		RESIDUE, RESIDUE_RANGE, ELEMENT, CHAIN, 
		ALL, NONE
	} tag;

	union {
		int residue;
		std::pair<int, int> residueRange;
		const char *element;
		char chain;
	};
};
