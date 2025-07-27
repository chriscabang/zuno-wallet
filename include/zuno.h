#ifndef ZUNO_H
#define ZUNO_H

#include <stdint.h>

#define SEED_BYTES 64

int mnemonic_to_seed(const char* mnemonic, const char* passphrase, uint8_t* seed);

#endif//ZUNO_H
