#pragma once

#include <string>
#include <vector>

#include "Selection.h"
#include "bio/ConnectorType.h"
#include "graphics/Model.h"
#include "graphics/Color.h"

class ColorCommand {
private:
	const std::vector<std::string> *commandWords;
	const Selection *selection;

	bool colorAtom(const std::vector<std::string> &args);
	bool colorConnector(
		const std::vector<std::string> &args,
		ConnectorType connectorType
	);

public:
	ColorCommand(
		const std::vector<std::string> *commandWords,
		const Selection *selection
	);
	
	//Execute the command; return true on success and false on failure
	void execute();
};
