CC = gcc
CFLAGS = -Wall -Wextra -g
HEADERS = -Iinclude

CUNIT_DIR ?= /opt/homebrew/opt/cunit
CUNIT_INCLUDE = -I$(CUNIT_DIR)/include
CUNIT_LIB = -L$(CUNIT_DIR)/lib -lcunit

SRC := $(wildcard src/*.c)
TEST_SRC := $(wildcard tests/*.c)

TARGET = out/executable
TEST_TARGET = out/test_executable

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(HEADERS) $^ -o $@ -lz

test: $(TEST_SRC) $(filter-out src/main.c,$(SRC))
	$(CC) -DTEST_ENV $(CFLAGS) $(CUNIT_INCLUDE) $(HEADERS) $^ -o $(TEST_TARGET) $(CUNIT_LIB) -lz
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)
