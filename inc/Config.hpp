/*
 * Config.hpp
 *
 *  Created on: Dec 7, 2017
 *      Author: pavelgamov
 */

#pragma once

#include <string>
#include <map>

class Config {

public:
	Config (const std::string& path);

	void dump() const;
	std::string apiKey() const;
	std::string host() const;
	std::string port() const;

private:
	std::string get(const std::string& key) const;

private:
	const std::string path_;
	std::map<std::string, std::string> map_;
};
