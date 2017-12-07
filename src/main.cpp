
#include <iostream>

#include "Config.hpp"
#include "Curl.hpp"


int main() {
	const std::string path("./config.yaml");

	Config cfg(path);
//	cfg.dump();

	Curl curl(cfg);
	curl.request(
			"api/printer/printhead",
			"{\"command\": \"jog\",   \"x\": 10,   \"y\": -5,   \"z\": 0.02 }");

//	std::cout << cfg.key() << std::endl;

	return 0;
}
