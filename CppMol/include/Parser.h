#pragma once

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>

class Parser {
public:
	static std::string removeSpaces(std::string str);
	static std::string removeTrailingSpaces(std::string str);
	static std::string lowercase(std::string str);

	//Split string into vector of substrings separated by the delimiter
	static std::vector<std::string> split(const std::string &str, char delimiter);

	/*
	Get argument from list of params where paramName and the argument
	are separated by "="

	Return "" if the parameter is not provided with an argument
	*/
	static std::string getArg(
		const std::string &paramName, 
		const std::vector<std::string> &params
	);
};
