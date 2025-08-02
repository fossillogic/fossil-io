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
#include "fossil/io/cryptic.h"
#include "fossil/io/output.h"

// XOR encrypt/decrypt
void fossil_io_cryptic_xor(uint8_t *data, size_t len, const uint8_t *key, size_t keylen) {
    if (!data || !key || keylen == 0) return;
    for (size_t i = 0; i < len; ++i)
        data[i] ^= key[i % keylen];
}

// Deterministic shuffle using linear congruential generator (LCG)
static uint32_t lcg_next(uint32_t x) {
    return (1103515245 * x + 12345) & 0x7FFFFFFF;
}

void fossil_io_cryptic_shuffle(uint8_t *data, size_t len, uint32_t seed) {
    if (!data || len < 2) return;
    for (size_t i = len - 1; i > 0; --i) {
        seed = lcg_next(seed);
        size_t j = seed % (i + 1);
        uint8_t tmp = data[i];
        data[i] = data[j];
        data[j] = tmp;
    }
}

void fossil_io_cryptic_unshuffle(uint8_t *data, size_t len, uint32_t seed) {
    if (!data || len < 2) return;
    size_t idx[len];
    for (size_t i = 0; i < len; ++i) idx[i] = i;

    for (size_t i = len - 1; i > 0; --i) {
        seed = lcg_next(seed);
        size_t j = seed % (i + 1);
        size_t tmp = idx[i];
        idx[i] = idx[j];
        idx[j] = tmp;
    }

    uint8_t tmp[len];
    for (size_t i = 0; i < len; ++i)
        tmp[idx[i]] = data[i];
    for (size_t i = 0; i < len; ++i)
        data[i] = tmp[i];
}

uint32_t fossil_io_cryptic_checksum(const uint8_t *data, size_t len) {
    uint32_t sum = 0;
    for (size_t i = 0; i < len; ++i)
        sum += data[i];
    return sum;
}

// Fast entropy function using clock jitter
uint32_t fossil_io_cryptic_entropy32(void) {
    volatile uint32_t x = 0;
    for (int i = 0; i < 256; ++i)
        x ^= (uint32_t)(uintptr_t)&x >> (i % 8);
    return (x ^ (x >> 13) ^ (x << 7));
}

void fossil_io_cryptic_entropy_buf(uint8_t *out, size_t len) {
    if (!out) return;
    for (size_t i = 0; i < len; ++i)
        out[i] = (uint8_t)(fossil_io_cryptic_entropy32() & 0xFF);
}

// Simple scrambling (bit rotation + XOR)
void fossil_io_cryptic_scramble(uint8_t *data, size_t len) {
    if (!data) return;
    for (size_t i = 0; i < len; ++i)
        data[i] = (data[i] << 3) | (data[i] >> 5) ^ 0xA5;
}

// Reverse scramble
void fossil_io_cryptic_obfuscate(uint8_t *data, size_t len) {
    fossil_io_cryptic_scramble(data, len);
}

void fossil_io_cryptic_deobfuscate(uint8_t *data, size_t len) {
    if (!data) return;
    for (size_t i = 0; i < len; ++i) {
        uint8_t x = data[i] ^ 0xA5;
        data[i] = (x >> 3) | (x << 5);
    }
}

// Basic file I/O using fossil_fstream_t (portable)
int fossil_io_cryptic_save(const char *filename, const uint8_t *data, size_t len) {
    if (!filename || !data) return -1;
    fossil_fstream_t stream;
    if (fossil_fstream_open(&stream, filename, "wb") != 0) return -1;
    size_t written = fossil_fstream_write(&stream, data, 1, len);
    fossil_fstream_close(&stream);
    return (written == len) ? 0 : -1;
}

int fossil_io_cryptic_load(const char *filename, uint8_t *buf, size_t buflen) {
    if (!filename || !buf) return -1;
    fossil_fstream_t stream;
    if (fossil_fstream_open(&stream, filename, "rb") != 0) return -1;
    size_t read = fossil_fstream_read(&stream, buf, 1, buflen);
    fossil_fstream_close(&stream);
    return (int)read;
}

static uint32_t entropy_state[4] = {0xA5A5A5A5, 0x5A5A5A5A, 0x3C3C3C3C, 0xC3C3C3C3};

static void mix_entropy_state(uint32_t input) {
    entropy_state[0] ^= (entropy_state[3] << 5) | (entropy_state[1] >> 3);
    entropy_state[1] ^= (input ^ entropy_state[0]);
    entropy_state[2] += (entropy_state[1] ^ (input << 7));
    entropy_state[3] = ((entropy_state[3] >> 11) | (entropy_state[2] << 21)) ^ input;
}

void fossil_io_cryptic_entropy_seed(uint32_t seed) {
    volatile uint32_t jitter = (uint32_t)(uintptr_t)&seed;
    for (int i = 0; i < 8; ++i) {
        jitter ^= (uint32_t)(uintptr_t)&jitter >> (i % 7);
        mix_entropy_state(seed ^ jitter ^ (uint32_t)clock());
    }
}
