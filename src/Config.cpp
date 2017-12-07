/*
 * Config.cpp
 *
 *  Created on: Dec 7, 2017
 *      Author: pavelgamov
 */


#include "Config.hpp"
#include <iostream>
#include <list>
#include <yaml.h>
#include <assert.h>
#include <stdio.h>


class YamlKey {
public:
	void push(const std::string &str) {
		key_.push_back(str);
	}
	void pop() {
		if (!key_.empty())
			key_.pop_back();
	}
	std::string value() {
		std::string key;
		for (auto it = key_.begin(), end = key_.end(); it != end; ++it) {
			if (!key.empty())
				key.append(".");
			key.append(*it);
		}
		return key;
	}
private:
	std::list<std::string> key_;
};

Config::Config(const std::string& path) : path_(path) {

	FILE *fh = fopen(path.c_str(), "r");
	if (!fh) {
		std::cerr << "File: \"" << path << "\" not found" << std::endl;
		exit(-1);
	}

	yaml_parser_t parser;
	yaml_token_t token;
	assert(yaml_parser_initialize(&parser));
	yaml_parser_set_input_file(&parser, fh);
	yaml_parser_scan(&parser, &token);

	YamlKey key;
	size_t blockNo = 0;
	while (token.type && token.type != YAML_STREAM_END_TOKEN) {
		switch (token.type) {
			case YAML_KEY_TOKEN: {
				yaml_token_delete(&token);
				yaml_parser_scan(&parser, &token);
				assert(YAML_SCALAR_TOKEN == token.type);
				key.push(reinterpret_cast<char*>(token.data.scalar.value));
				break;
			}
			case YAML_VALUE_TOKEN: {
				yaml_token_delete(&token);
				yaml_parser_scan(&parser, &token);
				if (YAML_SCALAR_TOKEN != token.type)
					continue;
				map_[key.value()] = reinterpret_cast<char*>(token.data.scalar.value);
				key.pop();
				break;
			}
			case YAML_BLOCK_SEQUENCE_START_TOKEN:
				blockNo = 0;
				break;
			case YAML_BLOCK_ENTRY_TOKEN:
				key.push(std::to_string(blockNo++));
				break;
			case YAML_BLOCK_END_TOKEN:
				key.pop();
				break;
			default:
				break;
		}
		if (token.type != YAML_STREAM_END_TOKEN) {
			yaml_token_delete(&token);
			yaml_parser_scan(&parser, &token);
		}
	}

	yaml_parser_delete(&parser);
	fclose(fh);
}

void Config::dump() const {
	for (auto it = map_.begin(), end = map_.end(); it != end; ++it)
		std::cout << it->first << " = " << it->second << std::endl;
}

std::string Config::get(const std::string& key) const {
	auto it = map_.find(key);
	if (it != map_.end())
		return it->second;
	std::cerr << __PRETTY_FUNCTION__ << ": " << key << " not found in config file" << std::endl;
	return "";
}

std::string Config::apiKey() const {
	return get("key");
}

std::string Config::host() const {
	return get("host");
}

std::string Config::port() const {
	return get("port");
}


