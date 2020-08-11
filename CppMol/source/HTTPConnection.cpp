#include "HTTPConnection.h"

bool HTTPConnection::curlDidGlobalInit = false;

size_t HTTPConnection::writeCallback(char *contents, size_t size, size_t nmemb, void *userData) {
	std::string *pageContents = (std::string*)userData;
	pageContents->append(contents);
	return size * nmemb;
}

bool HTTPConnection::initCurl() {
	curl = curl_easy_init();
	if (!curl) {
		std::cerr << "ERROR > Failed to initialize libcurl.\n\n";
		return 0;
	}
	return 1;
}

HTTPConnection::HTTPConnection() {
	if (!curlDidGlobalInit) {
		curl_global_init(CURL_GLOBAL_ALL);
		curlDidGlobalInit = true;
	}
}

bool HTTPConnection::get(std::string url) {
	response.clear();

	if (!initCurl()) {
		return 0;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_1_0);

	//Configure SSL
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
	curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");

	//Write to response string
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

	//Perform GET request
	CURLcode result = curl_easy_perform(curl);
	if (result != CURLE_OK) {
		std::cerr << "ERROR > Failed to make request: " << curl_easy_strerror(result) << "\n\n";
		curl_easy_cleanup(curl);
		return 0;
	}

	curl_easy_cleanup(curl);
	return 1;
}
