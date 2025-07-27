#include <stdio.h>
#include <string.h>

#include <openssl/evp.h>

#include "zuno.h"

int mnemonic_to_seed(const char* mnemonic, const char* passphrase, uint8_t* seed) {
  const char* salt_prefix = "mnemonic"; // :TODO: change this, as input?
  char salt[256] = { 0 };

  if (passphrase) {
    snprintf(salt, sizeof(salt), "%s%s", salt_prefix, passphrase);
  } else {
    snprintf(salt, sizeof(salt), "%s", salt_prefix);
  }

  int result = PKCS5_PBKDF2_HMAC(
    mnemonic,
    strlen(mnemonic),
    (unsigned char*) salt,
    strlen(salt),
    2048,
    EVP_sha512(),
    SEED_BYTES,
    seed
  );

  return result;
}
