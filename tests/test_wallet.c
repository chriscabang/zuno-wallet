#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdint.h>
#include <string.h>

#include "zuno.h"
#include "bip39.h"

#include <openssl/evp.h>
#include <openssl/sha.h>

#include <btc/bip32.h>
#include <btc/utils.h>
#include <btc/base58.h>
#include <btc/ecc.h>
#include <btc/ripemd160.h>

static void test_bip44_address_derivation(void **state) {
  (void) state;

  // Known mnemonic
  const char* mnemonic = 
    "merry area vehicle domain anger castle dolphin suggest minute gesture grant cage";
  const char* expected_address = "13HebaenhimhVRAwwjhG4Y845aHhmzaq1G";

  // Convert mnemonic to seed
  uint8_t seed[SEED_BYTES];
  assert_true(mnemonic_to_seed(mnemonic, NULL, seed));

  btc_ecc_start();

  btc_hdnode node;
  assert_true(btc_hdnode_from_seed(seed, sizeof(seed), &node));

  // Derive m/44'/0'/0'/0/0
  btc_hdnode_private_ckd_prime(&node, 44);
  btc_hdnode_private_ckd_prime(&node, 0);
  btc_hdnode_private_ckd_prime(&node, 0);
  btc_hdnode_private_ckd(&node, 0);
  btc_hdnode_private_ckd(&node, 0);

  // Get compressed pubkeyhash
  uint8_t hash160[20];
  uint8_t sha256_out[32];
  uint8_t pubkey[33];

  uint8_t pubkeyhash[21];
  memcpy(pubkey, node.public_key, BTC_ECKEY_COMPRESSED_LENGTH);
  SHA256(pubkey, BTC_ECKEY_COMPRESSED_LENGTH, sha256_out);
  btc_ripemd160(sha256_out, 32, hash160);

  pubkeyhash[0] = 0x00; // Mainnet prefix
  memcpy(pubkeyhash + 1, hash160, 20);

  char address[64];
  btc_base58_encode_check(pubkeyhash, sizeof(pubkeyhash), address, sizeof(address));

  btc_ecc_stop();

  // Assert generated address matches expected
  assert_string_equal(address, expected_address);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_bip44_address_derivation),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
