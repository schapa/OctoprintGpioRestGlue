export ROOT_DIR := .
export BUILD_DIR := ./build
export BUILD_ROOT :=$(BUILD_DIR)


# common CFLAGS and LDFLAGS, BUILD_TYPE (debug, release)
include common-defs.mk
include app.mk

.PHONY: all emulator firmware clean

all: emulator firmware 

emulator:
	$(MAKE) -f emulator.mk
	
firmware:
	$(MAKE) -f firmware.mk

clean::
	$(MAKE) -f emulator.mk $@
	$(MAKE) -f firmware.mk $@
	
include build.mk
