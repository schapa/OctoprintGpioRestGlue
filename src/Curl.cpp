/*
 * Curl.cpp
 *
 *  Created on: Dec 7, 2017
 *      Author: pavelgamov
 */

#include "Curl.hpp"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

int Curl::curlReady_;

Curl::Curl(const Config& cfg) :
	host_(cfg.host()),
	port_(cfg.port()),
	apiKey_(cfg.apiKey()),
	easy_handle_(NULL),
	headers_(NULL) {
	if (!curlReady_)
		curl_global_init(CURL_GLOBAL_ALL);
	++curlReady_;
}

Curl::~Curl() {
	--curlReady_;
	if (!curlReady_)
		curl_global_cleanup();
}

size_t Curl::on_data_write(void *data, size_t size, size_t nmemb, void *userp)
{
	const size_t realsize = size * nmemb;

	Curl *cli = static_cast<Curl*>(userp);

	const size_t current_size = cli->response_.size();
	cli->response_.resize(cli->response_.size() + realsize);
	memcpy(cli->response_.data() + current_size, data, realsize);

	return realsize;
}

bool Curl::request(const std::string& endp, const std::string& str) {
	easy_handle_ = curl_easy_init();
	assert(easy_handle_);

	curl_easy_setopt(easy_handle_, CURLOPT_WRITEFUNCTION, on_data_write);
	curl_easy_setopt(easy_handle_, CURLOPT_WRITEDATA,     (void *) this);

	const std::string url = host_ + "/" + endp;
	curl_easy_setopt(easy_handle_, CURLOPT_URL, url.c_str());
	if (!port_.empty()) {
		const long port = strtol (port_.c_str(), NULL, 10);
		curl_easy_setopt(easy_handle_, CURLOPT_PORT, port);
	}

	const std::string header = "X-Api-Key: " + apiKey_;
	headers_ = curl_slist_append(headers_, "Content-Type: application/json");
	headers_ = curl_slist_append(headers_, header.c_str());

	curl_easy_setopt(easy_handle_, CURLOPT_POSTFIELDS, str.c_str());

	curl_easy_setopt(easy_handle_, CURLOPT_HTTPHEADER, headers_);
	curl_easy_setopt(easy_handle_, CURLOPT_TIMEOUT_MS, 1000lu);

	curl_easy_setopt(easy_handle_, CURLOPT_FORBID_REUSE, 1l);
	curl_easy_setopt(easy_handle_, CURLOPT_FRESH_CONNECT, 1l);

	const CURLcode res = curl_easy_perform(easy_handle_);
	curl_slist_free_all(headers_);
	curl_easy_cleanup(easy_handle_);

	if (res == CURLE_OK)
		response_.push_back(0);

	std::cout << __PRETTY_FUNCTION__  << " " << (char*)response_.data() << std::endl;

	return res == CURLE_OK;
}
