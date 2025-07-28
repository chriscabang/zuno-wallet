APP=zuno-wallet

ROOT     = $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
SRC      = $(wildcard src/*.c)
INCLUDE  = $(ROOT)/include
TEST     = test_runner

CFLAGS  += -Wall -O2
CFLAGS  += -I$(INCLUDE)
CFLAGS  += -I/usr/include
CFLAGS  += -Ilib/libbtc/include
# CFLAGS  += -Ilib/u8g2
# CFLAGS  += -Ilib/u8g2/sys/linux-i2c/common
# CFLAGS  += -Ilib/u8g2/csrc

LDFLAGS += -lssl -lcrypto
LDFLAGS += lib/libbtc/build/libbtc.a
# LDFLAGS += lib/u8g2/build/libu8g2.a

CC      := gcc

.DEFAULT_GOAL = all

all: $(APP)
# lib/u8g2/sys/linux-i2c/common/linux-i2c.c lib/u8g2/sys/linux-i2c/common/linux-gpio.c
$(APP): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

tests: $(TEST)
	./$(TEST)
# lib/u8g2/sys/linux-i2c/common/linux-i2c.c lib/u8g2/sys/linux-i2c/common/linux-gpio.c
$(TEST): tests/test_display.c src/zuno.c src/bip39.c src/oled.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -lcmocka

clean:
	find . -type f -name $(APP) -delete
	find tests/ -type f -name "test_*" ! -name "*.c" ! -name "*.py" -delete
