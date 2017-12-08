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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h> // strerror

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
	ss.str("");

	// tell kernel to give us access to pin
	ss << "echo " << thiz->pin_ << " > /sys/class/gpio/export";
	system(ss.str().c_str());
	ss.str("");

	const std::string pinPath ("/sys/class/gpio/gpio" + thiz->pin_);

	// make input pin
	ss << "echo in > " << pinPath << "/direction";
	system(ss.str().c_str());
	ss.str("");

	const std::string pinValuePath(pinPath + "/value");

	int fd = open(pinValuePath.c_str(), O_RDONLY);
	if (fd < 0) {
		std::cerr << "File '" << pinValuePath << "' could not be accessed" << std::endl;
		return NULL;
	}

	struct pollfd pollfd[1];
	pollfd[0].fd = fd;
	pollfd[0].events = POLLPRI | POLLERR;
	pollfd[0].revents = 0;

	char previousVal;
	if (!thiz->readGpio(fd, previousVal))
		return NULL;
	while (thiz->running_) {
		int res = poll(pollfd, 1, 40);
		if (res < 0) {
			std::cerr << "Polling for '" << pinValuePath << "' failed " << strerror(res) << std::endl;
			return NULL;
		}
		char value;
		if (!thiz->readGpio(fd, value))
			return NULL;
		if (previousVal != value) {
			std::cout << "Poll '" << pinValuePath << "' value " << value << std::endl;
			previousVal = value;
			if (value) {
				Curl curl(thiz->cfg_);
				curl.request(thiz->control_, thiz->json_);
			}

		}
	}
	return NULL;
}


bool Listener::readGpio(int fd, char &val) {

	lseek(fd, 0, SEEK_SET);
	int res = read(fd, &val, sizeof(val));
	if (res < 0 || res != sizeof(val)) {
		std::cerr << "Reading fd '" << fd << "' failed " << strerror(res) << std::endl;
		return false;
	}
	return true;
}
