#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "zuno.h"
#include "bip39.h"

#include <openssl/evp.h>
#include <openssl/sha.h>

#include <btc/bip32.h>
#include <btc/utils.h>
#include <btc/base58.h>
#include <btc/ecc.h>
#include <btc/ripemd160.h>
// :FIXME: for BIP84 support, this header is not existing anymore
// #include <btc/bech32.h> // for BIP84

// :TODO: Use logly for logging
// :TODO: refactor and clean

#define MNEMONIC_BUFFER_SIZE 256
// #define SEED_BYTES 64
// 
// int mnemonic_to_seed(const char* mnemonic, const char* passphrase, uint8_t* seed) {
//   const char* salt_prefix = "mnemonic"; // :TOD: changed this
//   char salt[256] = { 0 };
// 
//   if (passphrase) {
//     snprintf(salt, sizeof(salt), "%s%s", salt_prefix, passphrase);
//   } else {
//     snprintf(salt, sizeof(salt), "%s", salt_prefix);
//   }
// 
//   int result = PKCS5_PBKDF2_HMAC(
//     mnemonic,
//     strlen(mnemonic),
//     (unsigned char*) salt,
//     strlen(salt),
//     2048,
//     EVP_sha512(),
//     SEED_BYTES,
//     seed
//   );
// 
//   return result;
// }

int main(void) {
  // Step 1: Generate mnemonic
  const char* wordlist_file = "utils/english.txt";
  if (load_bip39_wordlist(wordlist_file) != 0) {
    fprintf(stderr, "Failed to load BIP39 wordlist.\n");
    return 1;
  }

  char mnemonic[MNEMONIC_BUFFER_SIZE];
  generate_mnemonic(mnemonic, sizeof(mnemonic));

  printf("Mnemonic: \n%s\n", mnemonic);

  // Step 2: Convert mnemonic to seed (PBKDF2)
  uint8_t seed[SEED_BYTES];
  if (!mnemonic_to_seed(mnemonic, NULL, seed)) {
    fprintf(stderr, "PBKDF2 seed generation failed\n");
    return 1;
  }

  // Step 3: Derive master node
  btc_ecc_start();

  btc_hdnode node;
  if (!btc_hdnode_from_seed(seed, sizeof(seed), &node)) {
    fprintf(stderr, "Failed to derive master node\n");
    return 1;
  }

  // Print master xpub and xprv
  const btc_chainparams* chain = &btc_chainparams_main;
  char master_xpub[112];
  char master_xprv[112];
  btc_hdnode_serialize_public(&node, chain, master_xpub, sizeof(master_xpub));
  btc_hdnode_serialize_private(&node, chain, master_xprv, sizeof(master_xprv));

  printf("Master XPUB: %s\n", master_xpub);
  printf("Master XPRV: %s\n", master_xprv);

  // Step 4: Derive path to get the address

  // A. BIP44 standard (m/44'/0'/0'/0/0) for legacy (P2PKH) address generation
  btc_hdnode_private_ckd_prime(&node, 44);
  btc_hdnode_private_ckd_prime(&node, 0);
  btc_hdnode_private_ckd_prime(&node, 0);
  btc_hdnode_private_ckd(&node, 0);
  btc_hdnode_private_ckd(&node, 0);

  // Get compressed bip44_pubkeyhash
  uint8_t hash160[20];
  uint8_t sha256_out[SHA256_DIGEST_LENGTH];
  uint8_t pubkey[33];

  uint8_t bip44_pubkeyhash[21];
  memcpy(pubkey, node.public_key, BTC_ECKEY_COMPRESSED_LENGTH);
  SHA256(pubkey, BTC_ECKEY_COMPRESSED_LENGTH, sha256_out);
  btc_ripemd160(sha256_out, 32, hash160);

  bip44_pubkeyhash[0] = 0x00; // Mainnet address prefix
  memcpy(bip44_pubkeyhash + 1, hash160, 20);

  // :TODO: To be done in later phase, hardening.
  //        bech32 is not supported in libbtc library
  // // B. BIP44 standard (m/44'/0'/0'/0/0)
  // btc_hdnode_private_ckd_prime(&node, 84);
  // btc_hdnode_private_ckd_prime(&node, 0);
  // btc_hdnode_private_ckd_prime(&node, 0);
  // btc_hdnode_private_ckd(&node, 0);
  // btc_hdnode_private_ckd(&node, 0);

  // btc_hdnode_fill_public_key(&node);

  // uint8_t bip84_pubkeyhash[21];
  // memcpy(pubkey, node.public_key, BTC_ECKEY_COMPRESSED_LENGTH);
  // SHA256(pubkey, BTC_ECKEY_COMPRESSED_LENGTH, sha256_out);
  // btc_ripemd160(sha256_out, 32, hash160);

  // bip84_pubkeyhash[0] = 0x00;
  // btc_bech32_address_from_witness("bc", bip84_pubkeyhash, 21, bech32_addr, sizeof(bech32_addr));

  // Step 5: Encode address in Base58Check
  char address[64];
  btc_base58_encode_check(bip44_pubkeyhash, sizeof(bip44_pubkeyhash), address, sizeof(address));

  printf("Bitcoin Address (m/44'/0'/0'/0/0): %s\n", address);

  // Cleanup
  btc_ecc_stop();

  return 0;
}
