/*
 * Listener.hpp
 *
 *  Created on: Dec 8, 2017
 *      Author: pavelgamov
 */

#pragma once

#include <pthread.h>
#include "Config.hpp"
#include "Curl.hpp"
#include <atomic>

class Listener {

public:
	Listener(const Config& cfg, const std::string& name, const std::string& pin, const std::string& contol, const std::string& json);
	~Listener();

private:
	static void* thread (void *arg);
private:
	std::atomic<bool> running_;
	const Config& cfg_;
	pthread_t pid_;

	const std::string name_;
	const std::string pin_;
	const std::string control_;
	const std::string json_;
};
