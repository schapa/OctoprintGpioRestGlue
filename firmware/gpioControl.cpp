#include <stdlib.h>
#include <stddef.h>
#include <list>

#include <wiringPi.h>

class Gpio {
public:
	enum {
		EDGE_RISING = 1 << 0,
		EDGE_FALLING = 1 << 1,
	};
	typedef void (*cb) (size_t pin, bool value);

public:	
	Gpio &instance() { static Gpio instance; return instance; }
	bool subscribe(size_t pin, int flags, const cb &cb);

private:
	Gpio(){}
	~Gpio(){};

private:
	struct cb_item { size_t pin; int flags; cb callback; };
	std::list<cb_item> items;
};

bool Gpio::subscribe(size_t pin, int flags, const cb &cb) {
	bool res = false;

	cb_item item;
	item.pin = pin;
	item.flags = flags;
	item.callback = cb;
	this->items.push_back(item);	

	return res;
}
