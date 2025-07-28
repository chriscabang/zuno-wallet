WALLET=zuno

ROOT     = $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
SRC      = $(filter-out src/main.c, $(wildcard src/*.c))
INCLUDE  = $(ROOT)/include
BUILD    = $(ROOT)/build

TEST_SRC = $(wildcard tests/test_*.c)
TEST_BIN = $(patsubst tests/%.c, %, $(TEST_SRC))

CFLAGS  += -Wall -O2
CFLAGS  += -I$(INCLUDE)
CFLAGS  += -I/usr/include
CFLAGS  += -Ilib/libbtc/include

LDFLAGS += -lssl -lcrypto
LDFLAGS += lib/libbtc/build/libbtc.a

CC      := gcc

.DEFAULT_GOAL = all

prerequisites:
	mkdir -p $(BUILD)

$(CC): prerequisites
	@echo "Compiling $(WALLET)"

all: prerequisites $(WALLET)

tests: $(TEST_BIN)

$(WALLET): src/main.c $(SRC)
	$(CC) $(CFLAGS) -o $(BUILD)/$@ $^ $(LDFLAGS)

%: tests/%.c $(SRC)
	$(CC) $(CFLAGS) -o $(BUILD)/$@ $^ $(LDFLAGS) -lcmocka

clean:
	rm -r $(BUILD)
