/*
 * Curl.hpp
 *
 *  Created on: Dec 7, 2017
 *      Author: pavelgamov
 */

#pragma once

#include <string>
#include "Config.hpp"
#include <curl/curl.h>
#include <vector>

class Curl {
public:
	Curl(const Config& cfg);
	~Curl();

public:
	bool request(const std::string& endp, const std::string& str);

private:
	static size_t on_data_write(void *data, size_t size, size_t nmemb, void *userp);

private:
	const std::string host_;
	const std::string port_;
	const std::string apiKey_;

	CURL *easy_handle_;
	curl_slist *headers_;

	std::vector<char*> response_;

	static int curlReady_;
};
