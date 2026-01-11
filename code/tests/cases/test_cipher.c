/**
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * This file is part of the Fossil Logic project, which aims to develop
 * high-performance, cross-platform applications and libraries. The code
 * contained herein is licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * Author: Michael Gene Brockus (Dreamer)
 * Date: 04/05/2014
 *
 * Copyright (C) 2014-2025 Fossil Logic. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include <fossil/pizza/framework.h>

#include "fossil/io/framework.h"


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilites
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Define the test suite and add test cases
FOSSIL_SUITE(c_cipher_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_cipher_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_cipher_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Cipher API tests

FOSSIL_TEST(c_test_cipher_caesar_default) {
    const char *plain = "Hello, World!";
    char *enc = fossil_io_cipher_encode(plain, "caesar");
    ASSUME_ITS_TRUE(enc != NULL);
    if (enc) {
        char *dec = fossil_io_cipher_decode(enc, "caesar");
        ASSUME_ITS_TRUE(dec != NULL);
        if (dec) {
            ASSUME_ITS_EQUAL_CSTR(dec, plain);
            free(dec);
        }
        free(enc);
    }
}

FOSSIL_TEST(c_test_cipher_caesar_shift_5_nowrap) {
    const char *plain = "Zebra!";
    char *enc = fossil_io_cipher_encode(plain, "caesar:shift=5,wrap=0");
    ASSUME_ITS_TRUE(enc != NULL);
    if (enc) {
        ASSUME_ITS_CSTR_CONTAINS(enc, "Ejgwf");
        char *dec = fossil_io_cipher_decode(enc, "caesar:shift=5,wrap=0");
        ASSUME_ITS_TRUE(dec != NULL);
        if (dec) {
            ASSUME_ITS_EQUAL_CSTR(dec, plain);
            free(dec);
        }
        free(enc);
    }
}

FOSSIL_TEST(c_test_cipher_vigenere_basic) {
    const char *plain = "ATTACKATDAWN";
    char *enc = fossil_io_cipher_encode(plain, "vigenere:key=LEMON");
    ASSUME_ITS_TRUE(enc != NULL);
    if (enc) {
        ASSUME_ITS_EQUAL_CSTR(enc, "LXFOPVEFRNHR");
        char *dec = fossil_io_cipher_decode(enc, "vigenere:key=LEMON");
        ASSUME_ITS_TRUE(dec != NULL);
        if (dec) {
            ASSUME_ITS_EQUAL_CSTR(dec, plain);
            free(dec);
        }
        free(enc);
    }
}

FOSSIL_TEST(c_test_cipher_base64_basic) {
    const char *plain = "hello world";
    char *enc = fossil_io_cipher_encode(plain, "base64");
    ASSUME_ITS_TRUE(enc != NULL);
    if (enc) {
        ASSUME_ITS_EQUAL_CSTR(enc, "aGVsbG8gd29ybGQ=");
        char *dec = fossil_io_cipher_decode(enc, "base64");
        ASSUME_ITS_TRUE(dec != NULL);
        if (dec) {
            ASSUME_ITS_EQUAL_CSTR(dec, plain);
            free(dec);
        }
        free(enc);
    }
}

FOSSIL_TEST(c_test_cipher_base64_url) {
    const char *plain = "test?";
    char *enc = fossil_io_cipher_encode(plain, "base64:url=1");
    ASSUME_ITS_TRUE(enc != NULL);
    if (enc) {
        ASSUME_ITS_CSTR_CONTAINS(enc, "dGVzdD8");
        char *dec = fossil_io_cipher_decode(enc, "base64:url=1");
        ASSUME_ITS_TRUE(dec != NULL);
        if (dec) {
            ASSUME_ITS_EQUAL_CSTR(dec, plain);
            free(dec);
        }
        free(enc);
    }
}

FOSSIL_TEST(c_test_cipher_base32_basic) {
    const char *plain = "foo";
    char *enc = fossil_io_cipher_encode(plain, "base32");
    ASSUME_ITS_TRUE(enc != NULL);
    if (enc) {
        ASSUME_ITS_CSTR_CONTAINS(enc, "MZXW6===");
        char *dec = fossil_io_cipher_decode(enc, "base32");
        ASSUME_ITS_TRUE(dec != NULL);
        if (dec) {
            ASSUME_ITS_EQUAL_CSTR(dec, plain);
            free(dec);
        }
        free(enc);
    }
}

FOSSIL_TEST(c_test_cipher_binary_basic) {
    const char *plain = "A";
    char *enc = fossil_io_cipher_encode(plain, "binary");
    ASSUME_ITS_TRUE(enc != NULL);
    if (enc) {
        ASSUME_ITS_EQUAL_CSTR(enc, "01000001");
        char *dec = fossil_io_cipher_decode(enc, "binary");
        ASSUME_ITS_TRUE(dec != NULL);
        if (dec) {
            ASSUME_ITS_EQUAL_CSTR(dec, plain);
            free(dec);
        }
        free(enc);
    }
}

FOSSIL_TEST(c_test_cipher_binary_sep_bits) {
    const char *plain = "AB";
    char *enc = fossil_io_cipher_encode(plain, "binary:sep=,");
    ASSUME_ITS_TRUE(enc != NULL);
    if (enc) {
        ASSUME_ITS_CSTR_CONTAINS(enc, "01000001,01000010");
        char *dec = fossil_io_cipher_decode(enc, "binary:sep=,");
        ASSUME_ITS_TRUE(dec != NULL);
        if (dec) {
            ASSUME_ITS_EQUAL_CSTR(dec, plain);
            free(dec);
        }
        free(enc);
    }
}

FOSSIL_TEST(c_test_cipher_morse_basic) {
    const char *plain = "SOS";
    char *enc = fossil_io_cipher_encode(plain, "morse");
    ASSUME_ITS_TRUE(enc != NULL);
    if (enc) {
        ASSUME_ITS_EQUAL_CSTR(enc, "... --- ...");
        char *dec = fossil_io_cipher_decode(enc, "morse");
        ASSUME_ITS_TRUE(dec != NULL);
        if (dec) {
            ASSUME_ITS_EQUAL_CSTR(dec, plain);
            free(dec);
        }
        free(enc);
    }
}

FOSSIL_TEST(c_test_cipher_baconian_basic) {
    const char *plain = "ABC";
    char *enc = fossil_io_cipher_encode(plain, "baconian");
    ASSUME_ITS_TRUE(enc != NULL);
    if (enc) {
        ASSUME_ITS_EQUAL_CSTR(enc, "AAAAABAAABAAABA");
        char *dec = fossil_io_cipher_decode(enc, "baconian");
        ASSUME_ITS_TRUE(dec != NULL);
        if (dec) {
            ASSUME_ITS_EQUAL_CSTR(dec, plain);
            free(dec);
        }
        free(enc);
    }
}

FOSSIL_TEST(c_test_cipher_railfence_basic) {
    const char *plain = "WEAREDISCOVEREDFLEEATONCE";
    char *enc = fossil_io_cipher_encode(plain, "railfence:rails=3");
    ASSUME_ITS_TRUE(enc != NULL);
    if (enc) {
        ASSUME_ITS_EQUAL_CSTR(enc, "WECRLTEERDSOEEFEAOCAIVDEN");
        char *dec = fossil_io_cipher_decode(enc, "railfence:rails=3");
        ASSUME_ITS_TRUE(dec != NULL);
        if (dec) {
            ASSUME_ITS_EQUAL_CSTR(dec, plain);
            free(dec);
        }
        free(enc);
    }
}

FOSSIL_TEST(c_test_cipher_haxor_basic) {
    const char *plain = "HAXOR";
    char *enc = fossil_io_cipher_encode(plain, "haxor");
    ASSUME_ITS_TRUE(enc != NULL);
    if (enc) {
        ASSUME_ITS_CSTR_CONTAINS(enc, "#4><0|2");
        char *dec = fossil_io_cipher_decode(enc, "haxor:reverse=1");
        ASSUME_ITS_TRUE(dec != NULL);
        if (dec) {
            // Accept lower or upper case due to implementation
            ASSUME_ITS_CSTR_CONTAINS(dec, "haxor");
            free(dec);
        }
        free(enc);
    }
}

FOSSIL_TEST(c_test_cipher_invalid_cipher_id) {
    char *enc = fossil_io_cipher_encode("test", "unknowncipher");
    ASSUME_ITS_TRUE(enc == NULL);
    char *dec = fossil_io_cipher_decode("test", "unknowncipher");
    ASSUME_ITS_TRUE(dec == NULL);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_cipher_tests) {
    FOSSIL_TEST_ADD(c_cipher_suite, c_test_cipher_caesar_default);
    FOSSIL_TEST_ADD(c_cipher_suite, c_test_cipher_caesar_shift_5_nowrap);
    FOSSIL_TEST_ADD(c_cipher_suite, c_test_cipher_vigenere_basic);
    FOSSIL_TEST_ADD(c_cipher_suite, c_test_cipher_base64_basic);
    FOSSIL_TEST_ADD(c_cipher_suite, c_test_cipher_base64_url);
    FOSSIL_TEST_ADD(c_cipher_suite, c_test_cipher_base32_basic);
    FOSSIL_TEST_ADD(c_cipher_suite, c_test_cipher_binary_basic);
    FOSSIL_TEST_ADD(c_cipher_suite, c_test_cipher_binary_sep_bits);
    FOSSIL_TEST_ADD(c_cipher_suite, c_test_cipher_morse_basic);
    FOSSIL_TEST_ADD(c_cipher_suite, c_test_cipher_baconian_basic);
    FOSSIL_TEST_ADD(c_cipher_suite, c_test_cipher_railfence_basic);
    FOSSIL_TEST_ADD(c_cipher_suite, c_test_cipher_haxor_basic);
    FOSSIL_TEST_ADD(c_cipher_suite, c_test_cipher_invalid_cipher_id);

    FOSSIL_TEST_REGISTER(c_cipher_suite);
}
