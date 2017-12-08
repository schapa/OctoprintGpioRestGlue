
export PROGNAME := GpioGlue

export CFLAGS := \
	-fmessage-length=0 \
	-fsigned-char \
	-Wall \
	-Wextra \
	-I./inc \
	-ffunction-sections \
	-fdata-sections \
	$(shell pkg-config --cflags yaml-0.1) \
	$(shell pkg-config --cflags jansson) \
	$(shell pkg-config --cflags libcurl) \
	
export SRC := \
	$(wildcard ./src/*.c) \
	$(wildcard ./src/*.cpp) \

export LDFLAGS := \
	-ffunction-sections \
	-fdata-sections \
	-rdynamic \

export LDLIBS :=\
	-lc \
	-lpthread \
	$(shell pkg-config --libs yaml-0.1) \
	$(shell pkg-config --libs jansson) \
	$(shell pkg-config --libs libcurl) \

ifeq ($(shell uname),Darwin)
	LDFLAGS += -Xlinker -dead_strip 
else
	LDFLAGS += -Xlinker --gc-sections 
endif

