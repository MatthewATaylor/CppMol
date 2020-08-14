#include "bio/AminoAcid.h"

//These shared pointers won't ever reach a use count of 0
std::shared_ptr<const AminoAcid> alanine       (new AminoAcid("ALANINE",       "A", "ALA", "C3H7NO2",    "000"));
std::shared_ptr<const AminoAcid> arginine      (new AminoAcid("ARGININE",      "R", "ARG", "C6H14N4O2",  "110"));
std::shared_ptr<const AminoAcid> asparagine    (new AminoAcid("ASPARAGINE",    "N", "ASN", "C4H8N2O3",   "100"));
std::shared_ptr<const AminoAcid> asparticAcid  (new AminoAcid("ASPARTIC_ACID", "D", "ASP", "C4H7NO4",    "101"));
std::shared_ptr<const AminoAcid> cysteine      (new AminoAcid("CYSTEINE",      "C", "CYS", "C3H7NO2S",   "100"));
std::shared_ptr<const AminoAcid> glutamine     (new AminoAcid("GLUTAMINE",     "Q", "GLN", "C5H10N2O3",  "100"));
std::shared_ptr<const AminoAcid> glutamicAcid  (new AminoAcid("GLUTAMIC_ACID", "E", "GLU", "C5H9NO4",    "101"));
std::shared_ptr<const AminoAcid> glycine       (new AminoAcid("GLYCINE",       "G", "GLY", "C2H5NO2",    "000"));
std::shared_ptr<const AminoAcid> histidine     (new AminoAcid("HISTIDINE",     "H", "HIS", "C6H9N3O2",   "110"));
std::shared_ptr<const AminoAcid> isoleucine    (new AminoAcid("ISOLEUCINE",    "I", "ILE", "C6H13NO2",   "000"));
std::shared_ptr<const AminoAcid> leucine       (new AminoAcid("LEUCINE",       "L", "LEU", "C6H13NO2",   "000"));
std::shared_ptr<const AminoAcid> lysine        (new AminoAcid("LYSINE",        "K", "LYS", "C6H14N2O2",  "110"));
std::shared_ptr<const AminoAcid> methionine    (new AminoAcid("METHIONINE",    "M", "MET", "C5H11NO2S",  "000"));
std::shared_ptr<const AminoAcid> phenylalanine (new AminoAcid("PHENYLALANINE", "F", "PHE", "C9H11NO2",   "000"));
std::shared_ptr<const AminoAcid> proline       (new AminoAcid("PROLINE",       "P", "PRO", "C5H9NO2",    "000"));
std::shared_ptr<const AminoAcid> serine        (new AminoAcid("SERINE",        "S", "SER", "C3H7NO3",    "100"));
std::shared_ptr<const AminoAcid> threonine     (new AminoAcid("THREONINE",     "T", "THR", "C4H9NO3",    "100"));
std::shared_ptr<const AminoAcid> tryptophan    (new AminoAcid("TRYPTOPHAN",    "W", "TRP", "C11H12N2O2", "000"));
std::shared_ptr<const AminoAcid> tyrosine      (new AminoAcid("TYROSINE",      "Y", "TYR", "C9H11NO3",   "100"));
std::shared_ptr<const AminoAcid> valine        (new AminoAcid("VALINE",        "V", "VAL", "C5H11NO2",   "000"));

std::unordered_map<std::string, std::shared_ptr<const AminoAcid>> AminoAcid::dict = {
	{ "ALANINE",       alanine       }, { "ALA", alanine       }, { "A", alanine       },
	{ "ARGININE",      arginine      }, { "ARG", arginine      }, { "R", arginine      },
	{ "ASPARAGINE",    asparagine    }, { "ASN", asparagine    }, { "N", asparagine    },
	{ "ASPARTIC_ACID", asparticAcid  }, { "ASP", asparticAcid  }, { "D", asparticAcid  },
	{ "CYSTEINE",      cysteine      }, { "CYS", cysteine      }, { "C", cysteine      },
	{ "GLUTAMINE",     glutamine     }, { "GLN", glutamine     }, { "Q", glutamine     },
	{ "GLUTAMIC_ACID", glutamicAcid  }, { "GLU", glutamicAcid  }, { "E", glutamicAcid  },
	{ "GLYCINE",       glycine       }, { "GLY", glycine       }, { "G", glycine       },
	{ "HISTIDINE",     histidine     }, { "HIS", histidine     }, { "H", histidine     },
	{ "ISOLEUCINE",    isoleucine    }, { "ILE", isoleucine    }, { "I", isoleucine    },
	{ "LEUCINE",       leucine       }, { "LEU", leucine       }, { "L", leucine       },
	{ "LYSINE",        lysine        }, { "LYS", lysine        }, { "K", lysine        },
	{ "METHIONINE",    methionine    }, { "MET", methionine    }, { "M", methionine    },
	{ "PHENYLALANINE", phenylalanine }, { "PHE", phenylalanine }, { "F", phenylalanine },
	{ "PROLINE",       proline       }, { "PRO", proline       }, { "P", proline       },
	{ "SERINE",        serine        }, { "SER", serine        }, { "S", serine        },
	{ "THREONINE",     threonine     }, { "THR", threonine     }, { "T", threonine     },
	{ "TRYPTOPHAN",    tryptophan    }, { "TRP", tryptophan    }, { "W", tryptophan    },
	{ "TYROSINE",      tyrosine      }, { "TYR", tyrosine      }, { "Y", tyrosine      },
	{ "VALINE",        valine        }, { "VAL", valine        }, { "V", valine        },
};

void AminoAcid::overwriteWarn(const std::string &name) {
	if (get(name)) {
		std::cout << "WARNING > Overwriting amino acid with name: " << name << "\n\n";
	}
}

AminoAcid::AminoAcid(
	std::string name, std::string abbr1, std::string abbr3, 
	std::string formula, std::string properties
) :
	name(name), abbr1(abbr1), abbr3(abbr3), 
	formula(formula), properties(properties) {}

bool AminoAcid::isPolar() const {
	return properties[0];
}
bool AminoAcid::isBasic() const {
	return properties[1];
}
bool AminoAcid::isAcidic() const {
	return properties[2];
}
bool AminoAcid::containsSulfur() const {
	for (size_t i = 0; i < formula.size(); ++i) {
		if (std::tolower(formula[i]) == 's') {
			return 1;
		}
	}
	return 0;
}

const AminoAcid *AminoAcid::get(const std::string &name) {
	const AminoAcid *result = nullptr;
	try {
		result = dict.at(name).get();
	}
	catch (std::out_of_range) {}
	return result;
}

const AminoAcid *AminoAcid::set(
	std::string name, std::string abbr1, std::string abbr3,
	std::string formula, std::string properties
) {
	//Handle invalid arguments
	if (!name.empty()) {
		overwriteWarn(name);
	}
	if (!abbr1.empty()) {
		overwriteWarn(abbr1);
		if (abbr1.size() != 1) {
			std::cerr << "ERROR > Invalid abbr1: must contain exactly 1 character.\n\n";
			return nullptr;
		}
	}
	if (!abbr3.empty()) {
		overwriteWarn(abbr3);
		if (abbr3.size() != 3) {
			std::cerr << "ERROR > Invalid abbr3: must contain exactly 3 characters.\n\n";
			return nullptr;
		}
	}
	if (!properties.empty() && properties.size() != 3) {
		std::cerr << "ERROR > Invalid properties string: must contain exactly 3 characters.\n\n";
		return nullptr;
	}
	for (unsigned int i = 0; i < properties.size(); ++i) {
		if (properties[i] != '0' && properties[i] != '1') {
			std::cerr << "ERROR > Invalid properties string: must only contain '0' and '1'.\n\n";
			return nullptr;
		}
	}
	if (name.empty() && abbr1.empty() && abbr3.empty()) {
		std::cerr << "ERROR > At least one of [name, abbr1, abbr3] must be provided.\n\n";
		return nullptr;
	}

	const std::string UNKNOWN_STR = "";
	std::shared_ptr<AminoAcid> aminoAcid(
		new AminoAcid(
			name.empty() ? UNKNOWN_STR : name, 
			abbr1.empty() ? UNKNOWN_STR : abbr1,
			abbr3.empty() ? UNKNOWN_STR : abbr3, 
			formula.empty() ? UNKNOWN_STR : formula, 
			properties
		)
	);
	if (!name.empty()) {
		dict[name] = aminoAcid;
	}
	if (!abbr1.empty()) {
		dict[abbr1] = aminoAcid;
	}
	if (!abbr3.empty()) {
		dict[abbr3] = aminoAcid;
	}
	return aminoAcid.get();
}

void AminoAcid::erase(const AminoAcid *aminoAcid) {
	std::string name = aminoAcid->name;
	std::string abbr1 = aminoAcid->abbr1;
	std::string abbr3 = aminoAcid->abbr3;

	if (get(name)) {
		dict.erase(name);
	}
	if (get(abbr1)) {
		dict.erase(abbr1);
	}
	if (get(abbr3)) {
		dict.erase(abbr3);
	}
}

void AminoAcid::erase(const std::string &name) {
	if (get(name)) {
		dict.erase(name);
	}
}

void AminoAcid::showDict() {
	const unsigned int PADDING = 4;

	//Initialize max sizes to sizes of column labels
	size_t maxKeySize = 3 + PADDING;
	size_t maxNameSize = 4 + PADDING;
	size_t maxFormulaSize = 7 + PADDING;

	//Increase sizes to fit largest elements
	for (const auto &pair : dict) {
		if (pair.first.size() + PADDING > maxKeySize) {
			maxKeySize = pair.first.size() + PADDING;
		}
		if (pair.second->name.size() + PADDING > maxNameSize) {
			maxNameSize = pair.second->name.size() + PADDING;
		}
		if (pair.second->formula.size() + PADDING > maxFormulaSize) {
			maxFormulaSize = pair.second->formula.size() + PADDING;
		}
	}

	const unsigned int ABBR_1_SIZE = 5 + PADDING;
	const unsigned int ABBR_3_SIZE = 5 + PADDING;
	const unsigned int PROPERTIES_SIZE = 10 + PADDING;
	const char SEPARATOR = ' ';

	//Display dictionary in formatted table
	std::cout << std::left << std::setw(maxKeySize) << std::setfill(SEPARATOR) << "key";
	std::cout << std::left << std::setw(maxNameSize) << std::setfill(SEPARATOR) << "name";
	std::cout << std::left << std::setw(ABBR_1_SIZE) << std::setfill(SEPARATOR) << "abbr1";
	std::cout << std::left << std::setw(ABBR_3_SIZE) << std::setfill(SEPARATOR) << "abbr3";
	std::cout << std::left << std::setw(maxFormulaSize) << std::setfill(SEPARATOR) << "formula";
	std::cout << std::left << std::setw(PROPERTIES_SIZE) << std::setfill(SEPARATOR) << "properties";
	std::cout << '\n';
	for (const auto &pair : dict) {
		std::cout << std::left << std::setw(maxKeySize) << std::setfill(SEPARATOR) << pair.first;
		std::cout << std::left << std::setw(maxNameSize) << std::setfill(SEPARATOR) << pair.second->name;
		std::cout << std::left << std::setw(ABBR_1_SIZE) << std::setfill(SEPARATOR) << pair.second->abbr1;
		std::cout << std::left << std::setw(ABBR_3_SIZE) << std::setfill(SEPARATOR) << pair.second->abbr3;
		std::cout << std::left << std::setw(maxFormulaSize) << std::setfill(SEPARATOR) << pair.second->formula;
		std::cout << std::left << std::setw(PROPERTIES_SIZE) << std::setfill(SEPARATOR) << pair.second->properties;
		std::cout << '\n';
	}
	std::cout << '\n';
}
