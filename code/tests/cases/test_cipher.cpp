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
FOSSIL_SUITE(cpp_cipher_suite);

// Setup function for the test suite
FOSSIL_SETUP(cpp_cipher_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_cipher_suite) {
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

// C++ Cipher class API tests

using fossil::io::Cipher;

FOSSIL_TEST(cpp_test_cipher_class_caesar_default) {
    const std::string plain = "Hello, World!";
    std::string enc = Cipher::encode(plain, "caesar");
    ASSUME_ITS_TRUE(!enc.empty());
    if (!enc.empty()) {
        std::string dec = Cipher::decode(enc, "caesar");
        ASSUME_ITS_TRUE(!dec.empty());
        if (!dec.empty()) {
            ASSUME_ITS_EQUAL_CSTR(dec.c_str(), plain.c_str());
        }
    }
}

FOSSIL_TEST(cpp_test_cipher_class_caesar_shift_5_nowrap) {
    const std::string plain = "Zebra!";
    std::string enc = Cipher::encode(plain, "caesar:shift=5,wrap=0");
    ASSUME_ITS_TRUE(!enc.empty());
    if (!enc.empty()) {
        ASSUME_ITS_CSTR_CONTAINS(enc.c_str(), "Ejgwf");
        std::string dec = Cipher::decode(enc, "caesar:shift=5,wrap=0");
        ASSUME_ITS_TRUE(!dec.empty());
        if (!dec.empty()) {
            ASSUME_ITS_EQUAL_CSTR(dec.c_str(), plain.c_str());
        }
    }
}

FOSSIL_TEST(cpp_test_cipher_class_vigenere_basic) {
    const std::string plain = "ATTACKATDAWN";
    std::string enc = Cipher::encode(plain, "vigenere:key=LEMON");
    ASSUME_ITS_TRUE(!enc.empty());
    if (!enc.empty()) {
        ASSUME_ITS_EQUAL_CSTR(enc.c_str(), "LXFOPVEFRNHR");
        std::string dec = Cipher::decode(enc, "vigenere:key=LEMON");
        ASSUME_ITS_TRUE(!dec.empty());
        if (!dec.empty()) {
            ASSUME_ITS_EQUAL_CSTR(dec.c_str(), plain.c_str());
        }
    }
}

FOSSIL_TEST(cpp_test_cipher_class_base64_basic) {
    const std::string plain = "hello world";
    std::string enc = Cipher::encode(plain, "base64");
    ASSUME_ITS_TRUE(!enc.empty());
    if (!enc.empty()) {
        ASSUME_ITS_EQUAL_CSTR(enc.c_str(), "aGVsbG8gd29ybGQ=");
        std::string dec = Cipher::decode(enc, "base64");
        ASSUME_ITS_TRUE(!dec.empty());
        if (!dec.empty()) {
            ASSUME_ITS_EQUAL_CSTR(dec.c_str(), plain.c_str());
        }
    }
}

FOSSIL_TEST(cpp_test_cipher_class_base64_url) {
    const std::string plain = "test?";
    std::string enc = Cipher::encode(plain, "base64:url=1");
    ASSUME_ITS_TRUE(!enc.empty());
    if (!enc.empty()) {
        ASSUME_ITS_CSTR_CONTAINS(enc.c_str(), "dGVzdD8");
        std::string dec = Cipher::decode(enc, "base64:url=1");
        ASSUME_ITS_TRUE(!dec.empty());
        if (!dec.empty()) {
            ASSUME_ITS_EQUAL_CSTR(dec.c_str(), plain.c_str());
        }
    }
}

FOSSIL_TEST(cpp_test_cipher_class_base32_basic) {
    const std::string plain = "foo";
    std::string enc = Cipher::encode(plain, "base32");
    ASSUME_ITS_TRUE(!enc.empty());
    if (!enc.empty()) {
        ASSUME_ITS_CSTR_CONTAINS(enc.c_str(), "MZXW6===");
        std::string dec = Cipher::decode(enc, "base32");
        ASSUME_ITS_TRUE(!dec.empty());
        if (!dec.empty()) {
            ASSUME_ITS_EQUAL_CSTR(dec.c_str(), plain.c_str());
        }
    }
}

FOSSIL_TEST(cpp_test_cipher_class_binary_basic) {
    const std::string plain = "A";
    std::string enc = Cipher::encode(plain, "binary");
    ASSUME_ITS_TRUE(!enc.empty());
    if (!enc.empty()) {
        ASSUME_ITS_EQUAL_CSTR(enc.c_str(), "01000001");
        std::string dec = Cipher::decode(enc, "binary");
        ASSUME_ITS_TRUE(!dec.empty());
        if (!dec.empty()) {
            ASSUME_ITS_EQUAL_CSTR(dec.c_str(), plain.c_str());
        }
    }
}

FOSSIL_TEST(cpp_test_cipher_class_binary_sep_bits) {
    const std::string plain = "AB";
    std::string enc = Cipher::encode(plain, "binary:sep=,");
    ASSUME_ITS_TRUE(!enc.empty());
    if (!enc.empty()) {
        ASSUME_ITS_CSTR_CONTAINS(enc.c_str(), "01000001,01000010");
        std::string dec = Cipher::decode(enc, "binary:sep=,");
        ASSUME_ITS_TRUE(!dec.empty());
        if (!dec.empty()) {
            ASSUME_ITS_EQUAL_CSTR(dec.c_str(), plain.c_str());
        }
    }
}

FOSSIL_TEST(cpp_test_cipher_class_morse_basic) {
    const std::string plain = "SOS";
    std::string enc = Cipher::encode(plain, "morse");
    ASSUME_ITS_TRUE(!enc.empty());
    if (!enc.empty()) {
        ASSUME_ITS_EQUAL_CSTR(enc.c_str(), "... --- ...");
        std::string dec = Cipher::decode(enc, "morse");
        ASSUME_ITS_TRUE(!dec.empty());
        if (!dec.empty()) {
            ASSUME_ITS_EQUAL_CSTR(dec.c_str(), plain.c_str());
        }
    }
}

FOSSIL_TEST(cpp_test_cipher_class_baconian_basic) {
    const std::string plain = "ABC";
    std::string enc = Cipher::encode(plain, "baconian");
    ASSUME_ITS_TRUE(!enc.empty());
    if (!enc.empty()) {
        ASSUME_ITS_EQUAL_CSTR(enc.c_str(), "AAAAABAAABAAABA");
        std::string dec = Cipher::decode(enc, "baconian");
        ASSUME_ITS_TRUE(!dec.empty());
        if (!dec.empty()) {
            ASSUME_ITS_EQUAL_CSTR(dec.c_str(), plain.c_str());
        }
    }
}

FOSSIL_TEST(cpp_test_cipher_class_railfence_basic) {
    const std::string plain = "WEAREDISCOVEREDFLEEATONCE";
    std::string enc = Cipher::encode(plain, "railfence:rails=3");
    ASSUME_ITS_TRUE(!enc.empty());
    if (!enc.empty()) {
        ASSUME_ITS_EQUAL_CSTR(enc.c_str(), "WECRLTEERDSOEEFEAOCAIVDEN");
        std::string dec = Cipher::decode(enc, "railfence:rails=3");
        ASSUME_ITS_TRUE(!dec.empty());
        if (!dec.empty()) {
            ASSUME_ITS_EQUAL_CSTR(dec.c_str(), plain.c_str());
        }
    }
}

FOSSIL_TEST(cpp_test_cipher_class_haxor_basic) {
    const std::string plain = "HAXOR";
    std::string enc = Cipher::encode(plain, "haxor");
    ASSUME_ITS_TRUE(!enc.empty());
    if (!enc.empty()) {
        ASSUME_ITS_CSTR_CONTAINS(enc.c_str(), "#4><0|2");
        std::string dec = Cipher::decode(enc, "haxor:reverse=1");
        ASSUME_ITS_TRUE(!dec.empty());
        if (!dec.empty()) {
            ASSUME_ITS_CSTR_CONTAINS(dec.c_str(), "haxor");
        }
    }
}

FOSSIL_TEST(cpp_test_cipher_class_invalid_cipher_id) {
    std::string enc = Cipher::encode("test", "unknowncipher");
    ASSUME_ITS_TRUE(enc.empty());
    std::string dec = Cipher::decode("test", "unknowncipher");
    ASSUME_ITS_TRUE(dec.empty());
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_cipher_tests) {
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_caesar_default);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_caesar_shift_5_nowrap);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_vigenere_basic);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_base64_basic);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_base64_url);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_base32_basic);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_binary_basic);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_binary_sep_bits);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_morse_basic);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_baconian_basic);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_railfence_basic);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_haxor_basic);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_invalid_cipher_id);

    FOSSIL_TEST_REGISTER(cpp_cipher_suite);
}
