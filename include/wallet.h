/**
 * wallet.h
 * **Author:** Chris Cabang <chriscabang@outlook.com>
 * **Date:** Jul 27 16:02:58 2025
 * **License:** :TODO:
 *
 * # Zuno Wallet API v1.0
 *
 * Minimalist, security-first interface for air-gapped cold wallet operations.
 *
 * Copyright (c) 2025 Chris Cabang
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef WALLET_H
#define WALLET_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
 * Types and Structs
 *===========================================================================*/

/**
/**
 * Opaque wallet session/context handle.
 */
typedef struct wallet_context_s wallet_context_t;

 * Result code for all operations.
 */
typedef enum {
  WALLET_OK           =  0,   // Success
  WALLET_FAIL         = -1,   // Failure
  WALLET_UNAUTHORIZED = -2,   // Authentication error
  WALLET_LOCKED       = -3,   // Locked out
  WALLET_TIMEOUT      = -4,   // Time out
  WALLET_UNAVAILABLE  = -5,   // Operation not available (locked, not ready, or other)
  WALLET_UNSUPPORTED  = -10   // Feature not supported
} ZK_RV;

/**
 * BIP32/44/84 Hierarchical Derivation path.
 *
 * See BIP44 for usage. Example:
 *
 * m / purpose' / coint_type' / account' / change / address_index
 * eg. m/44'/0'/0'/0/5
 *      - [44'] BIP44
 *      - [0']  Bitcoin
 *      - [0']  Main account
 *      - [0]   External chain
 *      - [5]   Address #5
 */
typedef struct {
  uint32_t purpose;     // 44 for BIP44, 84 for BIP84
  uint32_t coin_type;   // 0 for BTC, 60 for ETH
  uint32_t account;     // HD accound index (BIP44) - 0: main, 1: secondary
  uint32_t chain;       // 0 = external, 1 = change
  uint32_t index;       // address index: 0, 1, 2, ... for individual addresses
} hd_path_t;

/**
 * @brief Callback signature for displaying string (prompts, addresses, mnemonics, etc.)
 *
 * @param text Null terminated string to display.
 */
typedef void (*wallet_print_cb)(const char* text);

/**
 * @brief Callback signature for clearing/blanking the display.
 */
typedef void (*wallet_clear_cb)();

/**
 * @brief Callback signature for single-byte secure user input (PIN, keypad index).
 *
 * @return Single byte or 0xFF on error/abort.
 */
typedef uint8_t (*wallet_input_byte_cb)(void);


/*=============================================================================
 * Context Management
 *===========================================================================*/

/**
 * @brief Initialize wallet context.
 *
 * @return WALLET_OK on success, error code otherwise.
 */
ZK_RV wallet_init(void);

/**
 * @brief Securely shut down the wallet system and clear all sensitive memory.
 */
void wallet_shutdown(void);

/**
 * @brief Get wallet status or health as a simple string. Display to
 *        output callback.
 *
 *        Displays:
 *          - firmware version
 *          - locked status
 *          - backup status (verified?)
 *          - failed authenticate count
 *          - export address count
 *          - export limit warning enabled (or not).
 *          WIP:
 *          - tamper detection
 *          - secure boot status (ok)
 *          - firmware hash
 *
 * @return Read-only pointer to the current context, or NULL if not initialized.
 */
const uint32_t wallet_status(void);


/*=============================================================================
 * Authentication
 *===========================================================================*/

/**
 * @brief Set or update the wallet PIN code. Uses input callback for PIN input.
 *
 * @return        WALLET_OK on success, error code otherwise.
 */
ZK_RV wallet_set_pin(void);

/**
 * @brief Authenticate user via PIN code. Uses input callback for PIN input.
 *
 * @return WALLET_OK on success, error code otherwise.
 */
ZK_RV wallet_login(void);

/**
 * @brief Logout and clear authentication/session data from memory.
 */
void wallet_logout(void);


/*=============================================================================
 * Seed Phrase Management
 *===========================================================================*/

/**
 * @brief Generate a new BIP39-compliant mnemonic (seed phrase).
 *
 * @return WALLET_OK on success, error code otherwise.
 */
ZK_RV wallet_generate_seed(void);

/**
 * @brief Reveal the wallet's mnemonic on the device display.
 */
void wallet_reveal_seed();

/**
 * @brief Restore wallet from an existing mnemonic seed phrase.
 *
 * @param mnemonic  Mnemonic string to use for recovery.
 * @return          WALLET_OK on success, error code otherwise.
 */
ZK_RV wallet_restore_seed(const char *mnemonic);


/*=============================================================================
 * Key & Address Management
 *===========================================================================*/

/**
 * @brief Derive the next unused receiving address.
 *
 * @params address_out  [Optional] Output buffer for address string (may be NULL
                        for display-only)
 * @params length       Output buffer size if address_out is used.
 * @return WALLET_OK on success, error code otherwise.
 */
ZK_RV wallet_derive_next_address(char *address_out, size_t length);

/**
 * @brief Get the public key for a given HD path.
 *
 * @param path        HD derivation path.
 * @param pubkey_out  [Optional] Output buffer for public key string (may be
 *                    NULL for display-only).
 * @param length      Output buffer size if pubkey_out is used.
 * @return WALLET_OK  on success, error code otherwise.
 */
ZK_RV wallet_get_pubkey(
    const hd_path_t *path, 
    char *pubkey_out,
    size_t length);


/*=============================================================================
 * Signing Operations
 *===========================================================================*/

/**
 * @brief Sign a transaction using the specified HD key.
 *
 * @param path          HD derivation path.
 * @param tx_data       Pointer to raw transaction data.
 * @param tx_len        Length of transaction data.
 * @param signature_out Output buffer for signature.
 * @param sig_len       [in] buffer size, [out] actual signature length.
 * @return              WALLET_OK on success, error code otherwise.
 */
ZK_RV wallet_sign_transaction(
    const hd_path_t *path,
    const uint8_t *tx_data, size_t tx_len,
    uint8_t *signature_out, size_t *sig_len);

/**
 * @brief Sign a message using the specified HD key.
 *
 * @param path          HD derivation path.
 * @param message       Pointer to message data.
 * @param msg_len       Message length.
 * @param signature_out Output buffer for signature.
 * @param sig_len       [in] buffer size, [out] actual length.
 * @return              WALLET_OK on success, error code otherwise.
 */
ZK_RV wallet_sign_message(
    const hd_path_t *path,
    const uint8_t *message, size_t msg_len,
    uint8_t *signature_out, size_t *sig_len);


/*=============================================================================
 * Entropy and Randomness
 *===========================================================================*/

/**
 * @brief Generate cryptographically secure random bytes.
 *
 * @param buffer Output buffer.
 * @param length Number of bytes.
 * @returns      true on success, false on error.
 */
bool wallet_generate_entropy(uint8_t *buffer, size_t length);


/*=============================================================================
 * Callback Registration
 *===========================================================================*/

/**
 * @brief Register global print callback.
 *
 * @param print_cb Secure string output callback.
 */
 void wallet_set_print_display_callbacks(wallet_print_cb print_cb);

/**
 * @brief Register global clear display callback.
 *
 * @param clear_cb Secure clear output callback.
 */
void wallet_set_clear_display_callback(wallet_clear_cb clear_cb);

/**
 * @brief Input callback for sensitive user input (PIN, confirmations, etc.).
 *
 * @param input_cb Function to read one byte of input.
 */
void wallet_set_input_byte_callback(wallet_input_byte_cb input_cb);


/*=============================================================================
 * Error Handling
 *===========================================================================*/

/**
 * @brief Get last error code set for this session/context.
 *
 * @return    The last ZK_RV set for the current thread/context.
 */
ZK_RV wallet_get_last_error(void);

/**
 * @brief Get human-readable error message for a result code.
 *
 * @param code  ZK_RV code.
 * @return      Human-readable error string.
 */
const char* wallet_get_error_message(ZK_RV code);


#ifdef __cplusplus
}
#endif

#endif//WALLET_H
