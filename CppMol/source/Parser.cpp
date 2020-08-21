#include "Parser.h"

std::string Parser::removeSpaces(std::string str) {
	str.erase(
		std::remove(str.begin(), str.end(), ' '),
		str.end()
	);
	return str;
}

std::string Parser::removeTrailingSpaces(std::string str) {
	if (str.size() > 0) {
		for (size_t i = str.size() - 1; i != static_cast<unsigned>(-1); --i) {
			if (str[i] != ' ') {
				break;
			}
			str.erase(str.begin() + i);
		}
	}
	return str;
}

std::string Parser::lowercase(std::string str) {
	for (size_t i = 0; i < str.size(); ++i) {
		str[i] = std::tolower(str[i]);
	}
	return str;
}

std::vector<std::string> Parser::split(const std::string &str, char delimiter) {
	std::istringstream strStream(str);
	std::string element;
	std::vector<std::string> result;
	while (std::getline(strStream, element, delimiter)) {
		if (!element.empty()) {
			result.push_back(element);
		}
	}
	return result;
}

std::string Parser::getArg(
	const std::string &paramName, 
	const std::vector<std::string> &params
) {
	std::string paramIndicator = paramName + "=";
	for (size_t i = 0; i < params.size(); ++i) {
		if (params[i].size() > paramIndicator.size()) {
			std::string paramPrefix = params[i].substr(0, paramIndicator.size());
			if (paramPrefix == paramIndicator) {
				return params[i].substr(paramIndicator.size());
			}
		}
	}
	return "";
}
