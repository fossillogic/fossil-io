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

using fossil::io::Cipher;

FOSSIL_TEST(cpp_test_cipher_encode_decode_caesar) {
    const std::string plain = "HelloWorld";
    std::string encoded = Cipher::encode(plain, "caesar");
    ASSUME_ITS_TRUE(!encoded.empty());
    std::string decoded = Cipher::decode(encoded, "caesar");
    ASSUME_ITS_TRUE(!decoded.empty());
    ASSUME_ITS_EQUAL_CSTR(plain.c_str(), decoded.c_str());
}

FOSSIL_TEST(cpp_test_cipher_encode_decode_vigenere) {
    const std::string plain = "OpenAI";
    std::string encoded = Cipher::encode(plain, "vigenere");
    ASSUME_ITS_TRUE(!encoded.empty());
    std::string decoded = Cipher::decode(encoded, "vigenere");
    ASSUME_ITS_TRUE(!decoded.empty());
    ASSUME_ITS_EQUAL_CSTR(plain.c_str(), decoded.c_str());
}

FOSSIL_TEST(cpp_test_cipher_encode_decode_base64) {
    const std::string plain = "Encode this!";
    std::string encoded = Cipher::encode(plain, "base64");
    ASSUME_ITS_TRUE(!encoded.empty());
    std::string decoded = Cipher::decode(encoded, "base64");
    ASSUME_ITS_TRUE(!decoded.empty());
    ASSUME_ITS_EQUAL_CSTR(plain.c_str(), decoded.c_str());
}

FOSSIL_TEST(cpp_test_cipher_encode_decode_base32) {
    const std::string plain = "Base32Test";
    std::string encoded = Cipher::encode(plain, "base32");
    ASSUME_ITS_TRUE(!encoded.empty());
    std::string decoded = Cipher::decode(encoded, "base32");
    ASSUME_ITS_TRUE(!decoded.empty());
    ASSUME_ITS_EQUAL_CSTR(plain.c_str(), decoded.c_str());
}

FOSSIL_TEST(cpp_test_cipher_encode_decode_binary) {
    const std::string plain = "Bin";
    std::string encoded = Cipher::encode(plain, "binary");
    ASSUME_ITS_TRUE(!encoded.empty());
    std::string decoded = Cipher::decode(encoded, "binary");
    ASSUME_ITS_TRUE(!decoded.empty());
    ASSUME_ITS_EQUAL_CSTR(plain.c_str(), decoded.c_str());
}

FOSSIL_TEST(cpp_test_cipher_encode_decode_morse) {
    const std::string plain = "SOS";
    std::string encoded = Cipher::encode(plain, "morse");
    ASSUME_ITS_TRUE(!encoded.empty());
    std::string decoded = Cipher::decode(encoded, "morse");
    ASSUME_ITS_TRUE(!decoded.empty());
    ASSUME_ITS_EQUAL_CSTR(plain.c_str(), decoded.c_str());
}

FOSSIL_TEST(cpp_test_cipher_encode_decode_baconian) {
    const std::string plain = "abc";
    std::string encoded = Cipher::encode(plain, "baconian");
    ASSUME_ITS_TRUE(!encoded.empty());
    std::string decoded = Cipher::decode(encoded, "baconian");
    ASSUME_ITS_TRUE(!decoded.empty());
    ASSUME_ITS_EQUAL_CSTR(plain.c_str(), decoded.c_str());
}

FOSSIL_TEST(cpp_test_cipher_encode_decode_railfence) {
    const std::string plain = "railfence";
    std::string encoded = Cipher::encode(plain, "railfence");
    ASSUME_ITS_TRUE(!encoded.empty());
    std::string decoded = Cipher::decode(encoded, "railfence");
    ASSUME_ITS_TRUE(!decoded.empty());
    ASSUME_ITS_EQUAL_CSTR(plain.c_str(), decoded.c_str());
}

FOSSIL_TEST(cpp_test_cipher_encode_decode_haxor) {
    const std::string plain = "leet";
    std::string encoded = Cipher::encode(plain, "haxor");
    ASSUME_ITS_TRUE(!encoded.empty());
    std::string decoded = Cipher::decode(encoded, "haxor");
    ASSUME_ITS_TRUE(!decoded.empty());
    ASSUME_ITS_EQUAL_CSTR(plain.c_str(), decoded.c_str());
}

FOSSIL_TEST(cpp_test_cipher_encode_decode_leet) {
    const std::string plain = "elite";
    std::string encoded = Cipher::encode(plain, "leet");
    ASSUME_ITS_TRUE(!encoded.empty());
    std::string decoded = Cipher::decode(encoded, "leet");
    ASSUME_ITS_TRUE(!decoded.empty());
    ASSUME_ITS_EQUAL_CSTR(plain.c_str(), decoded.c_str());
}

FOSSIL_TEST(cpp_test_cipher_encode_decode_rot13) {
    const std::string plain = "rot13test";
    std::string encoded = Cipher::encode(plain, "rot13");
    ASSUME_ITS_TRUE(!encoded.empty());
    std::string decoded = Cipher::decode(encoded, "rot13");
    ASSUME_ITS_TRUE(!decoded.empty());
    ASSUME_ITS_EQUAL_CSTR(plain.c_str(), decoded.c_str());
}

FOSSIL_TEST(cpp_test_cipher_encode_decode_atbash) {
    const std::string plain = "atbash";
    std::string encoded = Cipher::encode(plain, "atbash");
    ASSUME_ITS_TRUE(!encoded.empty());
    std::string decoded = Cipher::decode(encoded, "atbash");
    ASSUME_ITS_TRUE(!decoded.empty());
    ASSUME_ITS_EQUAL_CSTR(plain.c_str(), decoded.c_str());
}

FOSSIL_TEST(cpp_test_cipher_case_insensitive_id) {
    const std::string plain = "CaseTest";
    std::string encoded = Cipher::encode(plain, "CaEsAr");
    ASSUME_ITS_TRUE(!encoded.empty());
    std::string decoded = Cipher::decode(encoded, "cAeSaR");
    ASSUME_ITS_TRUE(!decoded.empty());
    ASSUME_ITS_EQUAL_CSTR(plain.c_str(), decoded.c_str());
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_cipher_tests) {
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_encode_decode_caesar);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_encode_decode_vigenere);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_encode_decode_base64);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_encode_decode_base32);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_encode_decode_binary);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_encode_decode_morse);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_encode_decode_baconian);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_encode_decode_railfence);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_encode_decode_haxor);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_encode_decode_leet);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_encode_decode_rot13);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_encode_decode_atbash);
    FOSSIL_TEST_ADD(cpp_cipher_suite, cpp_test_cipher_case_insensitive_id);

    FOSSIL_TEST_REGISTER(cpp_cipher_suite);
}
