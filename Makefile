APP=zuno-wallet

CC=gcc
CFLAGS=-Iinclude -I/usr/include -I../libbtc/include -Wall -O2
LDFLAGS=../libbtc/build/libbtc.a -lssl -lcrypto

SRC=src/main.c src/zuno.c src/bip39.c
TEST=tests/test_wallet

all: $(APP)

$(APP): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

tests: $(TEST)
	./$(TEST)

$(TEST): tests/test_wallet.c src/zuno.c src/bip39.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -lcmocka

clean:
	find . -type f -name $(APP) -delete
	find tests/ -type f -name "test_*" ! -name "test_*.c" -delete
