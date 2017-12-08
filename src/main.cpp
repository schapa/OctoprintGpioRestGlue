
#include <iostream>
#include <list>

#include "Config.hpp"
#include "Curl.hpp"
#include "Listener.hpp"
#include <unistd.h>

int main() {
	const std::string path("./config.yaml");
	Config cfg(path);

	const Config::Storage& storage = cfg.storage();
	size_t itemsCnt = 0;
	static const std::string prefix("actions.");
	for (auto it = storage.begin(), end = storage.end(); it != end; ++it) {
		if (!it->first.compare(0, prefix.size(), prefix))
			itemsCnt++;
	}
	itemsCnt /= 4;
	std::list<Listener*> listeners;
	for (size_t i = 0; i < itemsCnt; i++) {
		const std::string key = prefix + std::to_string(i) +".";

		const std::string name = cfg.get(key + "name");
		const std::string pin = cfg.get(key + "gpio");
		const std::string control = cfg.get(key + "control");
		const std::string json = cfg.get(key + "json");

		listeners.push_back(new Listener(cfg, name, pin, control, json));
	}

	bool active = false;
	do {
		sleep(1);
		active = false;
		for (auto it = listeners.begin(), end = listeners.end(); it != end; ++it) {
			active |= (*it)->isActive();
		}
	} while (active);

	return 0;
}
