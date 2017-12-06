
export PROGNAME := GpioGlue

export CFLAGS := \
	-I./inc \
	-ffunction-sections \
	-fdata-sections \
	
export SRC := \
	$(wildcard ./src/*.c) \
	$(wildcard ./src/*.cpp) \

export LDFLAGS := \
        -fmessage-length=0 \
        -fsigned-char \
        -ffunction-sections \
        -fdata-sections \
        -Xlinker --gc-sections \
	-Wall \
        -Wextra \
        -lc \
        -lpthread \
        -rdynamic \
        -ljansson \
        -lcurl \

