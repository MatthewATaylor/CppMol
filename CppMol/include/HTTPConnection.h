#pragma once

#include <string>
#include <iostream>

#include <curl/curl.h>

class HTTPConnection {
private:
	static bool curlDidGlobalInit;
	CURL *curl = nullptr;

	static size_t writeCallback(char *contents, size_t size, size_t nmemb, void *userData);

	bool initCurl();

public:
	std::string response;

	HTTPConnection();

	/*
	Perform a GET request with the url and return true
	if the request was successful or false otherwise
	*/
	bool get(std::string url);
};
