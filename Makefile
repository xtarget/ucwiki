CC = gcc
BUILD_PATH = build
EXEC = $(BUILD_PATH)/ucwiki
SD_PATH=sundown
SRC  = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=$(BUILD_PATH)/%.o) $(SD_PATH)/libsundown.a

INCLUDES = -Iinc \
	   -I/usr/include \
	   -I./$(SD_PATH)/src \
	   -I./$(SD_PATH)/html \
	   -I./ctemplate-1.0

CFLAGS = -W -Wall -g
LDFLAGS = -L/usr/lib -L$(BUILD_PATH)

all: prepare sundown_lib $(EXEC)

prepare:
	mkdir -p $(BUILD_PATH)
	cp -R example $(BUILD_PATH)

sundown_lib:
	make -C $(SD_PATH) libsundown.a

$(BUILD_PATH)/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)


strip:
	strip $(EXEC)

clean:
	-@(rm -f $(EXEC) $(OBJ))

clear: clean
	make -C $(SD_PATH) clean
