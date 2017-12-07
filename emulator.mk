# toolchain defs
include $(ROOT_DIR)/toolchain.mk

OUT_SFX := x86
BUILD_DIR :=$(BUILD_DIR)/$(OUT_SFX)

PROGNAME := $(PROGNAME)-$(OUT_SFX)
MAP_FILE := $(BUILD_ROOT)/$(PROGNAME).map

# firmware specific sources
export ASM_SRC := 

SRC += \
	$(wildcard ./emulator/*.c) \
	$(wildcard ./emulator/*.cpp) \

CFLAGS += \
	-I./emulator/ \

CFLAGS += \
	-O0 \
	-ggdb \


.PHONY: all clean info

all: info $(PROGNAME)

info:
	@echo "[BUILD] $(PROGNAME)"

clean::
	@echo "[CLEAN] $(PROGNAME)"
	rm -f $(BUILD_ROOT)/$(PROGNAME) $(BUILD_ROOT)/$(FIRMWARE) $(BUILD_ROOT)/$(MAP_FILE)

include $(ROOT_DIR)/build.mk
