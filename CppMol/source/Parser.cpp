#include "Parser.h"

std::string Parser::removeSpaces(std::string str) {
	str.erase(
		std::remove(str.begin(), str.end(), ' '),
		str.end()
	);
	return str;
}

std::string Parser::lowercase(std::string str) {
	for (size_t i = 0; i < str.size(); ++i) {
		str[i] = std::tolower(str[i]);
	}
	return str;
}

std::string Parser::getArg(const std::string &paramName, const std::string &str) {
	std::string paramIndicator = paramName + "=";
	size_t paramPos = str.find(paramIndicator);

	if (paramPos == std::string::npos ||
		str.size() <= paramPos + paramIndicator.size()) {

		return "";
	}

	std::string paramFromPos = str.substr(paramPos);
	size_t argEndPos = paramFromPos.find(" ");
	if (argEndPos == std::string::npos) {
		argEndPos = str.size();
	}
	size_t argSize = argEndPos - (paramIndicator.size());
	return paramFromPos.substr(paramIndicator.size(), argSize);
}
