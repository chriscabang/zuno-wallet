#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "bip39.h"

static const char* wordlist[BIP39_WORDLIST_SIZE];

int load_bip39_wordlist(const char* file) {
  FILE *fp = fopen(file, "r");
  if (!fp) {
    perror("Failed to open wordlist");
    return -1;
  }

  static char buffer[2048][16];
  int i = 0;
  while (fgets(buffer[i], sizeof(buffer[i]), fp) && i < BIP39_WORDLIST_SIZE) {
    buffer[i][strcspn(buffer[i], "\r\n")] = 0;
    wordlist[i] = buffer[i];
    i++;
  }
  fclose(fp);

  if (i != BIP39_WORDLIST_SIZE) {
    fprintf(stderr, "Expected %d words, got %d\n", BIP39_WORDLIST_SIZE, i);
    return -1;
  }

  return 0;
}

void generate_mnemonic(char* output, int len) {
  FILE *fp = fopen("/dev/urandom", "rb");
  if (!fp) {
    perror("Failed to open /dev/urandom");
    strncpy(output, "error: no entropy", len);
    return;
  }

  output[0] = '\0';
  for (int i = 0; i < BIP39_WORD_COUNT; i++) {
    unsigned short entropy;
    fread(&entropy, sizeof(entropy), 1, fp);
    int index = entropy % BIP39_WORDLIST_SIZE;
    strncat(output, wordlist[index], len - strlen(output) - 1);
    if (i != BIP39_WORD_COUNT - 1) strncat(output, " ", len - strlen(output) - 1);
  }

  fclose(fp);
}
