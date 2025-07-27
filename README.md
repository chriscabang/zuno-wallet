<pre>
  _____
 |__  /   _ _ __   ___
   / / | | | '_ \ / _ \
  / /| |_| | | | | (_) |
 /____\__,_|_| |_|\___/
</pre>

> A work-in-progress

A minimalist, open-source cold wallet for Bitcoin built for the
Raspberry Pi Zero 2 W using Arch Linux ARM. Designed for offline key generation
and address derivation, this project prioritizes simplicity, portability, and
air-gapped security.

> **Disclaimer**
> This project is provided for educational and experimental purposes only.
> It is not guaranteed to be secure or suitable for managing real funds.
>
> Use at your own risk.
> The author(s) of this project take no responsibility for loss of funds, data,
> or damage arising from its use. Always verify the code yourself and do not
> rely on it for production-grade wallet security.

## Features (Phase 1)
- BIP39 mnemonic generation (using `/dev/urandom`)
- PBKDF2-HMAC-SHA512 seed derivation
- BIP32/BIP44 hierarchical key derivation
- Base58Check legacy Bitcoin (P2PKH) address generation
- C-based implementation using [libbtc](https://github.com/libbtc/libbtc)
- CMocka test suite for deterministic address validation

## Hardware
- Raspberry Pi Zero 2 W
- Optional: OLED Display (I2C), Rotary Encoder (planned Phase 2+)

## Build Requirements
- Arch Linux ARM (32-bit)
- OpenSSL
- libbtc (compiled from source)
- cmocka (for tests)

## Build
```bash
make

## Run
```bash
./zuno-wallet

## test
```bash
make tests
