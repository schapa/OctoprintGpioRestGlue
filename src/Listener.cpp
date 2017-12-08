/*
 * Listener.cpp
 *
 *  Created on: Dec 8, 2017
 *      Author: pavelgamov
 */

#include "Listener.hpp"
#include <assert.h>
#include <iostream>
#include <sstream>
#include <poll.h>
#include <unistd.h>

Listener::Listener(const Config& cfg,
		const std::string& name, const std::string& pin, const std::string& control, const std::string& json)
	: running_(true),
	  cfg_(cfg),
	  name_(name),
	  pin_(pin),
	  control_(control),
	  json_(json) {

    if (pthread_create(&pid_, NULL, thread, static_cast<void*>(this)))
    		assert("pthread_create()");

}

Listener::~Listener() {
	running_ = false;
	pthread_join(pid_, NULL);
}

void* Listener::thread (void *arg) {
	Listener *thiz = static_cast<Listener*>(arg);

	std::stringstream ss;
	ss << thiz->name_ << " pin " << thiz->pin_ << " " << thiz->control_ << " " << thiz->json_ << std::endl;
	std::cout << ss.str();
	while (thiz->running_) {
		sleep(1);
		// poll here
		if (0) {
			Curl curl(thiz->cfg_);
			curl.request(thiz->control_, thiz->json_);
		}
	}
}

