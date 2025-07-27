#ifndef BIP39_H
#define BIP39_H

// :TODO: dynamically set through a jumper (default: 24)
#define BIP39_WORD_COUNT 12
#define BIP39_WORDLIST_SIZE 2048

int  load_bip39_wordlist(const char* file);
void generate_mnemonic(char* output, int len);

#endif//BIP39_H
