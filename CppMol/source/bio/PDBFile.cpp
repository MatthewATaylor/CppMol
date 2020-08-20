#include "bio/PDBFile.h"

PDBFile::PDBFile(const std::string &url) {
	//Valid extension (.pdb)
	if (url.size() > 4) {
		std::string extension = url.substr(url.size() - 4);
		extension = Parser::lowercase(extension);
		if (extension != ".pdb") {
			std::cerr << "ERROR > Invalid webpage: expected a PDB file\n\n";
			return;
		}
	}

	HTTPConnection connection;
	if (!connection.get(url)) {
		return;
	}

	std::string title = "";

	std::stringstream responseStream(connection.response);
	std::string fileLine;
	while (std::getline(responseStream, fileLine)) {
		if (fileLine.size() != 80) {
			std::cerr << "ERROR > Invalid PDB file: lines must contain 80 columns\n\n";
			return;
		}

		if (fileLine.substr(0, 5) == "TITLE") {
			title += Parser::removeTrailingSpaces(fileLine.substr(10, 70));
		}

		//Append residue to sequence
		else if (fileLine.substr(0, 6) == "SEQRES") {
			//Add unique chain and chain size
			int residueNum = 1;
			char chain = fileLine[11];
			bool chainAdded = false;
			for (size_t i = 0; i < chains.size(); ++i) {
				if (chains[i].identifier == chain) {
					chainAdded = true;
					break;
				}
			}
			if (!chainAdded) {
				residueNum = 1; //Reset residue counter

				std::string chainSizeStr = Parser::removeSpaces(fileLine.substr(13, 4));
				std::stringstream chainSizeStream(chainSizeStr);
				size_t chainSize;
				chainSizeStream >> chainSize;
				chains.push_back({ chain, chainSize });
			}

			//Loop through 3-character residue names
			for (unsigned int i = 19; i <= 67; i += 4) {
				if (fileLine.substr(i, 3) == "   ") { //Residue list ended
					break;
				}
				std::string name = Parser::removeSpaces(fileLine.substr(i, 3));
				sequence.push_back({ name, residueNum++, chain });
			}
		}

		//Append helix
		else if (fileLine.substr(0, 5) == "HELIX") {
			char chain = fileLine[19];

			std::string residueStartStr = fileLine.substr(21, 4);
			int residueStart;
			try {
				residueStart = std::stoi(residueStartStr);
			}
			catch (...) {
				std::cerr << "ERROR > Invalid residue number: " << residueStartStr <<
					". Skipping helix...\n\n";
				continue;
			}

			std::string residueEndStr = fileLine.substr(33, 4);
			int residueEnd;
			try {
				residueEnd = std::stoi(residueEndStr);
			}
			catch (...) {
				std::cerr << "ERROR > Invalid residue number: " << residueEndStr <<
					". Skipping helix...\n\n";
				continue;
			}

			std::string typeStr = fileLine.substr(38, 2);
			int type;
			std::string typeErrorMessage = "ERROR > Invalid helix type: " + typeStr +
				". Skipping helix...\n\n";
			try {
				type = std::stoi(typeStr);
			}
			catch (...) {
				std::cerr << typeErrorMessage;
				continue;
			}
			if (type < 1 || type > 10) {
				std::cerr << typeErrorMessage;
				continue;
			}

			helices.push_back({ type, chain, residueStart, residueEnd });
		}

		//Append sheet
		else if (fileLine.substr(0, 5) == "SHEET") {
			char chain = fileLine[21];

			std::string residueStartStr = fileLine.substr(22, 4);
			int residueStart;
			try {
				residueStart = std::stoi(residueStartStr);
			}
			catch (...) {
				std::cerr << "ERROR > Invalid residue number: " << residueStartStr <<
					". Skipping sheet...\n\n";
				continue;
			}

			std::string residueEndStr = fileLine.substr(33, 4);
			int residueEnd;
			try {
				residueEnd = std::stoi(residueEndStr);
			}
			catch (...) {
				std::cerr << "ERROR > Invalid residue number: " << residueEndStr <<
					". Skipping sheet...\n\n";
				continue;
			}

			sheets.push_back({ chain, residueStart, residueEnd });
		}

		//Append disulfide bond
		else if (fileLine.substr(0, 6) == "SSBOND") {
			char chain1 = fileLine[15];

			std::string residue1Str = fileLine.substr(17, 4);
			int residue1;
			try {
				residue1 = std::stoi(residue1Str);
			}
			catch (...) {
				std::cerr << "ERROR > Invalid residue number: " << residue1Str <<
					". Skipping disulfide bond...\n\n";
				continue;
			}

			char chain2 = fileLine[29];

			std::string residue2Str = fileLine.substr(31, 4);
			int residue2;
			try {
				residue2 = std::stoi(residue2Str);
			}
			catch (...) {
				std::cerr << "ERROR > Invalid residue number: " << residue2Str <<
					". Skipping disulfide bond...\n\n";
				continue;
			}

			disulfideBonds.push_back({ chain1, residue1, chain2, residue2 });
		}

		//Append atom
		else if (fileLine.substr(0, 4) == "ATOM") {
			std::string name = Parser::removeSpaces(fileLine.substr(12, 4));
			std::string residueName = Parser::removeSpaces(fileLine.substr(17, 3));
			char chain = fileLine[21];

			std::string residueStr = fileLine.substr(22, 4);
			int residueNum;
			try {
				residueNum = std::stoi(residueStr);
			}
			catch (...) {
				std::cerr << "ERROR > Invalid residue number: " << residueStr <<
					". Skipping atom...\n\n";
				continue;
			}

			std::string xStr = fileLine.substr(30, 8);
			std::string yStr = fileLine.substr(38, 8);
			std::string zStr = fileLine.substr(46, 8);

			Vec3 coords(std::stof(xStr), std::stof(yStr), std::stof(zStr));

			std::string element = fileLine.substr(76, 2);
			element = Parser::removeSpaces(element);

			atoms.push_back({ name, residueName, chain, residueNum, coords, element });
		}
	}

	std::cout << "Loaded: " << title << "\n\n";
}
