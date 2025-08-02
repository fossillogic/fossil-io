/*
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * This file is part of the Fossil Logic project, which aims to develop high-
 * performance, cross-platform applications and libraries. The code contained
 * herein is subject to the terms and conditions defined in the project license.
 *
 * Author: Michael Gene Brockus (Dreamer)
 *
 * Copyright (C) 2024 Fossil Logic. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#ifndef FOSSIL_IO_CRYPTIC_H
#define FOSSIL_IO_CRYPTIC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ==================================================================
// Functions
// ==================================================================

/**
 * Applies XOR encryption/decryption to the data buffer using the provided key.
 *
 * @param data Pointer to the data buffer to be encrypted/decrypted.
 * @param len Length of the data buffer.
 * @param key Pointer to the key buffer.
 * @param keylen Length of the key buffer.
 */
void fossil_io_cryptic_xor(uint8_t *data, size_t len, const uint8_t *key, size_t keylen);

/**
 * Shuffles the data buffer using the given seed for pseudo-randomness.
 *
 * @param data Pointer to the data buffer to shuffle.
 * @param len Length of the data buffer.
 * @param seed Seed value for the shuffle algorithm.
 */
void fossil_io_cryptic_shuffle(uint8_t *data, size_t len, uint32_t seed);

/**
 * Unshuffles the data buffer using the given seed to restore original order.
 *
 * @param data Pointer to the data buffer to unshuffle.
 * @param len Length of the data buffer.
 * @param seed Seed value used during the original shuffle.
 */
void fossil_io_cryptic_unshuffle(uint8_t *data, size_t len, uint32_t seed);

/**
 * Computes a checksum for the given data buffer.
 *
 * @param data Pointer to the data buffer.
 * @param len Length of the data buffer.
 * @return The computed 32-bit checksum.
 */
uint32_t fossil_io_cryptic_checksum(const uint8_t *data, size_t len);

/**
 * Scrambles the data buffer in-place for obfuscation.
 *
 * @param data Pointer to the data buffer to scramble.
 * @param len Length of the data buffer.
 */
void fossil_io_cryptic_scramble(uint8_t *data, size_t len);

/**
 * Generates a 32-bit entropy value.
 *
 * @return A pseudo-random 32-bit value.
 */
uint32_t fossil_io_cryptic_entropy32(void);

/**
 * Fills the output buffer with entropy bytes.
 *
 * @param out Pointer to the output buffer.
 * @param len Number of bytes to fill.
 */
void fossil_io_cryptic_entropy_buf(uint8_t *out, size_t len);

/**
 * Obfuscates the data buffer in-place.
 *
 * @param data Pointer to the data buffer to obfuscate.
 * @param len Length of the data buffer.
 */
void fossil_io_cryptic_obfuscate(uint8_t *data, size_t len);

/**
 * Deobfuscates the data buffer in-place.
 *
 * @param data Pointer to the data buffer to deobfuscate.
 * @param len Length of the data buffer.
 */
void fossil_io_cryptic_deobfuscate(uint8_t *data, size_t len);

/**
 * Saves the data buffer to a file.
 *
 * @param filename Path to the file to save.
 * @param data Pointer to the data buffer to save.
 * @param len Length of the data buffer.
 * @return 0 on success, non-zero on failure.
 */
int fossil_io_cryptic_save(const char *filename, const uint8_t *data, size_t len);

/**
 * Loads data from a file into the buffer.
 *
 * @param filename Path to the file to load.
 * @param buf Pointer to the buffer to load data into.
 * @param buflen Size of the buffer.
 * @return Number of bytes read on success, negative value on failure.
 */
int fossil_io_cryptic_load(const char *filename, uint8_t *buf, size_t buflen);

/**
 * Seeds the entropy generator with a 32-bit value.
 *
 * @param seed The seed value to use.
 */
void fossil_io_cryptic_entropy_seed(uint32_t seed);

/**
 * Generates a strong entropy value using a more robust method.
 *
 * @return A strong pseudo-random 32-bit value.
 */
uint32_t fossil_io_cryptic_entropy_strong(void);

/**
 * Fills the output buffer with strong entropy bytes.
 *
 * @param out Pointer to the output buffer.
 * @param len Number of bytes to fill.
 */
void fossil_io_cryptic_entropy_strong_buf(uint8_t *out, size_t len);

#ifdef __cplusplus
}

/**
 * C++ wrapper for the Parser API.
 */
namespace fossil {

    /**
     * Namespace for I/O operations.
     */
    namespace io {
        /**
         * Parser API for sanitizing strings.
         */
        class Cryptic {
        public:
            /**
             * Encrypts the given data using XOR with the provided key.
             * 
             * @param data Pointer to the data buffer to be encrypted.
             * @param len Length of the data buffer.
             * @param key Pointer to the key buffer.
             * @param keylen Length of the key buffer.
             */
            static void xor_encrypt(uint8_t *data, size_t len, const uint8_t *key, size_t keylen) {
                fossil_io_cryptic_xor(data, len, key, keylen);
            }

            /**
             * Generates a checksum for the given data.
             * 
             * @param data Pointer to the data buffer.
             * @param len Length of the data buffer.
             * @return The computed checksum.
             */
            static uint32_t checksum(const uint8_t *data, size_t len) {
                return fossil_io_cryptic_checksum(data, len);
            }

            /**
             * Scrambles the data buffer in-place.
             * 
             * @param data Pointer to the data buffer to scramble.
             * @param len Length of the data buffer.
             */
            static void scramble(uint8_t *data, size_t len) {
                fossil_io_cryptic_scramble(data, len);
            }

            /**
             * Obfuscates the data buffer in-place.
             * 
             * @param data Pointer to the data buffer to obfuscate.
             * @param len Length of the data buffer.
             */
            static void obfuscate(uint8_t *data, size_t len) {
                fossil_io_cryptic_obfuscate(data, len);
            }

            /**
             * Deobfuscates the data buffer in-place.
             * 
             * @param data Pointer to the data buffer to deobfuscate.
             * @param len Length of the data buffer.
             */
            static void deobfuscate(uint8_t *data, size_t len) {
                fossil_io_cryptic_deobfuscate(data, len);
            }

            /**
             * Generates a strong entropy value.
             * 
             * @return A strong pseudo-random 32-bit value.
             */
            static uint32_t entropy_strong(void) {
                return fossil_io_cryptic_entropy_strong();
            }

            /**
             * Fills the output buffer with strong entropy bytes.
             *
             * @param out Pointer to the output buffer.
             * @param len Number of bytes to fill.
             */
            static void entropy_strong_buf(uint8_t *out, size_t len) {
                fossil_io_cryptic_entropy_strong_buf(out, len);
            }
        };
    }
}

#endif /* FOSSIL_IO_FRAMEWORK_H */
