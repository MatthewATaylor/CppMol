#pragma once

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>
#include <sstream>

class Parser {
public:
	static std::string removeSpaces(std::string str);
	static std::string lowercase(std::string str);

	//Split string into vector of substrings separated by the delimiter
	static std::vector<std::string> split(const std::string &str, char delimiter);

	/*
	Get argument from str where paramName and the argument
	are separated by "="

	Return "" if the parameter is not provided with an argument
	*/
	static std::string getArg(const std::string &paramName, const std::string &str);
};