#include "ColorCommand.h"

bool ColorCommand::colorAtom(const std::vector<std::string> &args) {
	if (args.size() == 3) {
		int r, g, b;
		std::string rStr = args[0];
		std::string gStr = args[1];
		std::string bStr = args[2];
		try {
			r = std::stoi(rStr);
			g = std::stoi(gStr);
			b = std::stoi(bStr);

			if (r > 255 || r < 0 || g > 255 || g < 0 || b > 255 || b < 0) {
				throw std::exception();
			}

			Color color = Color::fromByte(
				(unsigned char)r, (unsigned char)g, (unsigned char)b
			);
			Model::setAtomColor(&color, selection);
		}
		catch (...) {
			std::cerr << "ERROR > Invalid color argument: " <<
				"color values must be numbers between 0 and 255\n\n";
			return false;
		}
	}
	else if (args.size() == 1) {
		if (args[0] == "default") {
			Model::colorAtomsDefault(selection);
		}
		else if (args[0] == "structure") {
			Model::colorAtomsByStructure(selection);
		}
		else {
			try {
				Color color = Color::fromName(args[0]);
				Model::setAtomColor(&color, selection);
			}
			catch (...) {
				std::cerr << "ERROR > Invalid color argument\n\n";
				return false;
			}
		}
	}
	else {
		std::cerr << "ERROR > Invalid command\n\n";
		return false;
	}
	return true;
}

bool ColorCommand::colorConnector(
	const std::vector<std::string> &args,
	ConnectorType connectorType
) {
	if (args.size() == 3) {
		int r, g, b;
		std::string rStr = args[0];
		std::string gStr = args[1];
		std::string bStr = args[2];
		try {
			r = std::stoi(rStr);
			g = std::stoi(gStr);
			b = std::stoi(bStr);

			if (r > 255 || r < 0 || g > 255 || g < 0 || b > 255 || b < 0) {
				throw std::exception();
			}

			Color color = Color::fromByte(
				(unsigned char)r, (unsigned char)g, (unsigned char)b
			);
			Model::setConnectorColor(&color, selection, connectorType);
		}
		catch (...) {
			std::cerr << "ERROR > Invalid color argument: " <<
				"color values must be numbers between 0 and 255\n\n";
			return false;
		}
	}
	else if (args.size() == 1) {
		if (args[0] == "default") {
			Model::colorConnectorsDefault(selection, connectorType);
		}
		else if (args[0] == "structure") {
			Model::colorConnectorsByStructure(selection, connectorType);
		}
		else {
			try {
				Color color = Color::fromName(args[0]);
				Model::setConnectorColor(&color, selection, connectorType);
			}
			catch (...) {
				std::cerr << "ERROR > Invalid color argument\n\n";
				return false;
			}
		}
	}
	else {
		std::cerr << "ERROR > Invalid command\n\n";
		return false;
	}
	return true;
}

ColorCommand::ColorCommand(
	const std::vector<std::string> *commandWords, 
	const Selection *selection) :
	commandWords(commandWords), selection(selection) {}

void ColorCommand::execute() {
	if ((*commandWords)[1] == "atom" && commandWords->size() > 2) {
		colorAtom(
			std::vector<std::string>(
				commandWords->begin() + 2, 
				commandWords->end()
			)
		);
	}
	else if ((*commandWords)[1] == "backbone" && commandWords->size() > 2) {
		colorConnector(
			std::vector<std::string>(
				commandWords->begin() + 2,
				commandWords->end()
			),
			ConnectorType::BACKBONE
		);
	}
	else if ((*commandWords)[1] == "ssbond" && commandWords->size() > 2) {
		colorConnector(
			std::vector<std::string>(
				commandWords->begin() + 2,
				commandWords->end()
			),
			ConnectorType::DISULFIDE_BOND
		);
	}
	else if (commandWords->size() > 1) {
		if (!colorAtom(
			std::vector<std::string>(
				commandWords->begin() + 1,
				commandWords->end()
			)
		)) {
			return;
		}

		if (!colorConnector(
			std::vector<std::string>(
				commandWords->begin() + 1,
				commandWords->end()
			),
			ConnectorType::BACKBONE
		)) {
			return;
		}

		if (!colorConnector(
			std::vector<std::string>(
				commandWords->begin() + 1,
				commandWords->end()
			),
			ConnectorType::DISULFIDE_BOND
		)) {
			return;
		}
	}
	else {
		std::cerr << "ERROR > Invalid command\n\n";
	}
}
