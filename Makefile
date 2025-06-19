CC = gcc
CFLAGS = -Wall -Wextra -g
HEADERS = -Iinclude

SRC := $(wildcard src/*.c)

TARGET = out/executable

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(HEADERS) $^ -o $@

clean:
	rm -f $(TARGET) $(TEST_TARGET)

