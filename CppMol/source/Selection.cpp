#include "Selection.h"

Selection::Selection() :
	residueRange(
		std::pair<std::optional<int>, std::optional<int>>()
	) {}

void Selection::reset() {
	residue.reset();
	residueRange.reset();
	element.reset();
	chain.reset();
}

void Selection::print() const {
	if (residue) {
		std::cout << "Residue: " << *residue << "    ";
	}
	else if (residueRange) {
		int firstStr = residueRange->first ?
			*(residueRange->first) : 0;
		std::string secondStr = residueRange->second ?
			std::to_string(*(residueRange->second)) : "end";
		std::cout << "Residues: " << firstStr << ":" << secondStr << "    ";
	}

	if (element) {
		std::cout << "Element: " << *element << "    ";
	}
	if (chain) {
		std::cout << "Chain: " << *chain;
	}

	if (!residue && !residueRange && !element && !chain) {
		std::cout << "Residues: 0:end";
	}

	std::cout << "\n\n";
}

void Selection::parseQuery(const std::vector<std::string> &query) {
	std::optional<int> newResidue;
	std::optional<std::pair<std::optional<int>, std::optional<int>>> newResidueRange;
	std::optional<std::string> newElement;
	std::optional<char> newChain;

	const unsigned int NUM_PARAMS = 3;
	std::string paramNames[NUM_PARAMS] = { "r=", "e=", "c=" };

	//Check for invalid input
	for (size_t i = 0; i < query.size(); ++i) {
		bool paramNameIsValid = false;
		for (unsigned int j = 0; j < NUM_PARAMS; ++j) {
			if (query[i].size() > paramNames[j].size()) {
				std::string queryParamName = query[i].substr(0, paramNames[j].size());
				if (queryParamName == paramNames[j]) {
					paramNameIsValid = true;
					break;
				}
			}
		}

		if (!paramNameIsValid) {
			std::cerr << "ERROR > Invalid argument: " << query[i] << "\n\n";
			return;
		}
	}

	//Parse residue selection
	std::string residueArg = Parser::getArg("r", query);
	if (residueArg.size() > 0) {
		for (size_t i = 0; i < residueArg.size(); ++i) {
			if (residueArg[i] != ':' && 
				residueArg[i] < '0' &&
				residueArg[i] > '9') {

				std::cerr << "ERROR > Invalid argument: r=" << residueArg << "\n\n";
				return;
			}
		}

		size_t colonPos = residueArg.find(":");

		//Residue range provided
		if (colonPos != std::string::npos) {
			/*
			num1:num2
			:num2 -> 0:num2
			num1: -> num1:end
			: -> 0:end
			*/

			std::optional<int> start, end;

			if (colonPos != 0) {
				std::string startStr = residueArg.substr(0, colonPos);
				try {
					start = std::stoi(startStr);
				}
				catch (...) {
					std::cerr << "ERROR > Invalid start residue\n\n";
					return;
				}
			}

			if (colonPos != residueArg.size() - 1) {
				std::string endStr = residueArg.substr(colonPos + 1);
				try {
					end = std::stoi(endStr);
				}
				catch (...) {
					std::cerr << "ERROR > Invalid end residue\n\n";
					return;
				}
			}
			
			newResidueRange = std::make_pair(start, end);
		}

		//Single residue provided
		else {
			try {
				int residue = std::stoi(residueArg);
				newResidue = residue;
			}
			catch (...) {
				std::cerr << "ERROR > Invalid residue\n\n";
				return;
			}
		}
	}

	//Parse element selection
	std::string elementArg = Parser::getArg("e", query);
	if (elementArg.size() > 0) {
		newElement = elementArg;
	}

	//Parse chain selection
	std::string chainArg = Parser::getArg("c", query);
	if (chainArg.size() > 0) {
		if (chainArg.size() > 1) {
			std::cerr << "ERROR > Invalid chain: must be a single character\n\n";
			return;
		}
		newChain = chainArg[0];
	}

	residue = newResidue;
	residueRange = newResidueRange;
	element = newElement;
	chain = newChain;
}

bool Selection::isMatch(const Atom *atom, bool reversed) const {
	if (residue) {
		int residueNum = atom->residueNum;
		bool match = residueNum == residue;
		if (!match) {
			return reversed;
		}
	}
	else if (residueRange) {
		int residueNum = atom->residueNum;
		bool match =
			!((residueRange->first && residueNum < *(residueRange->first)) ||
			(residueRange->second && residueNum > *(residueRange->second)));
		if (!match) {
			return reversed;
		}
	}

	if (element) {
		std::string element = atom->element;
		bool match =
			Parser::lowercase(element) ==
			Parser::lowercase(*(this->element));
		if (!match) {
			return reversed;
		}
	}

	if (chain) {
		char chain = atom->chain;
		bool match = std::tolower(chain) == std::tolower(*(this->chain));
		if (!match) {
			return reversed;
		}
	}

	return !reversed;
}
