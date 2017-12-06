export ROOT_DIR := .
export BUILD_DIR := ./build
export BUILD_ROOT :=$(BUILD_DIR)


# common CFLAGS and LDFLAGS, BUILD_TYPE (debug, release)
include common-defs.mk
include app.mk

.PHONY: all firmware emulator clean

all: firmware 

firmware:
	$(MAKE) -f firmware.mk
emulator:
	$(MAKE) -f emulator.mk

clean::
	$(MAKE) -f firmware.mk $@
	
include build.mk
