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
	std::string get(const std::string& key) const;

	typedef std::map<std::string, std::string> Storage;
	const Storage& storage() const { return map_; }

private:
	const std::string path_;
	std::map<std::string, std::string> map_;
};
