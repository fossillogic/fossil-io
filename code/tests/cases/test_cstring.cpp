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
FOSSIL_SUITE(cpp_cstring_suite);

// Setup function for the test suite
FOSSIL_SETUP(cpp_cstring_suite)
{
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_cstring_suite)
{
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Tests for CString constructor and basic operations
FOSSIL_TEST(cpp_test_cstring_create_free)
{
    fossil::io::CString str("Hello");
    ASSUME_ITS_EQUAL_CSTR(str.str(), "Hello");
}

FOSSIL_TEST(cpp_test_cstring_create_null)
{
    fossil::io::CString str("");
    ASSUME_ITS_EQUAL_CSTR(str.str(), "");
}

// Tests for money conversions
FOSSIL_TEST(cpp_test_money_to_string)
{
    std::string result = fossil::io::CString::money_to_string(1234.56);
    ASSUME_ITS_TRUE(result.find("1") != std::string::npos);
}

FOSSIL_TEST(cpp_test_money_to_string_negative)
{
    std::string result = fossil::io::CString::money_to_string(-99.99);
    ASSUME_ITS_TRUE(result.find("-") != std::string::npos);
}

FOSSIL_TEST(cpp_test_string_to_money)
{
    double amount = fossil::io::CString::from_money("1234.56");
    ASSUME_ITS_TRUE(amount > 1234.0 && amount < 1235.0);
}

// Tests for case-insensitive operations
FOSSIL_TEST(cpp_test_case_compare)
{
    fossil::io::CString str1("Hello");
    ASSUME_ITS_EQUAL_I32(str1.icmp("hello"), 0);
}

FOSSIL_TEST(cpp_test_case_search)
{
    fossil::io::CString str("Hello World");
    ASSUME_ITS_TRUE(str.icontains("world"));
}

FOSSIL_TEST(cpp_test_case_starts_with)
{
    fossil::io::CString str("HelloWorld");
    ASSUME_ITS_TRUE(str.starts_with("Hello"));
}

FOSSIL_TEST(cpp_test_case_ends_with)
{
    fossil::io::CString str("HelloWorld");
    ASSUME_ITS_TRUE(str.ends_with("World"));
}

FOSSIL_TEST(cpp_test_case_replace)
{
    fossil::io::CString str("Hello World");
    fossil::io::CString result = str.replace("World", "Universe");
    ASSUME_ITS_TRUE(std::string(result.str()).find("Universe") != std::string::npos);
}

// Tests for novelty transformations
FOSSIL_TEST(cpp_test_mocking_case)
{
    fossil::io::CString str("hello");
    fossil::io::CString result = str.mocking();
    ASSUME_ITS_TRUE(result.str() != nullptr);
}

FOSSIL_TEST(cpp_test_rot13_transform)
{
    fossil::io::CString str("hello");
    fossil::io::CString result = str.rot13();
    ASSUME_ITS_EQUAL_CSTR(result.str(), "uryyb");
}

FOSSIL_TEST(cpp_test_leetspeak)
{
    fossil::io::CString str("ELITE");
    std::string result = str.leetspeak();
    ASSUME_ITS_TRUE(result.length() > 0);
}

FOSSIL_TEST(cpp_test_piglatin)
{
    fossil::io::CString str("hello world");
    std::string result = str.piglatin();
    ASSUME_ITS_TRUE(result.length() > 0);
}

FOSSIL_TEST(cpp_test_shuffle)
{
    fossil::io::CString str("abcdef");
    fossil::io::CString result = str.shuffle();
    ASSUME_ITS_LENGTH_EQUAL_CSTR(result.str(), 6);
}

FOSSIL_TEST(cpp_test_upper_snake)
{
    fossil::io::CString str("Hello World");
    fossil::io::CString result = str.upper_snake();
    ASSUME_ITS_TRUE(std::string(result.str()).find("HELLO") != std::string::npos);
}

// Tests for basic string operations
FOSSIL_TEST(cpp_test_cstring_length)
{
    fossil::io::CString str("Hello");
    ASSUME_ITS_EQUAL_U64(str.length(), 5);
}

FOSSIL_TEST(cpp_test_cstring_compare)
{
    fossil::io::CString str("abc");
    ASSUME_ITS_EQUAL_I32(str.compare("abc"), 0);
}

FOSSIL_TEST(cpp_test_cstring_concat)
{
    fossil::io::CString result = fossil::io::CString::concat("Hello", "World");
    ASSUME_ITS_EQUAL_CSTR(result.str(), "HelloWorld");
}

FOSSIL_TEST(cpp_test_cstring_to_upper)
{
    fossil::io::CString str("hello");
    fossil::io::CString result = str.to_upper();
    ASSUME_ITS_EQUAL_CSTR(result.str(), "HELLO");
}

FOSSIL_TEST(cpp_test_cstring_to_lower)
{
    fossil::io::CString str("HELLO");
    fossil::io::CString result = str.to_lower();
    ASSUME_ITS_EQUAL_CSTR(result.str(), "hello");
}

// Tests for substring and search operations
FOSSIL_TEST(cpp_test_cstring_contains)
{
    fossil::io::CString str("Hello World");
    ASSUME_ITS_TRUE(str.contains("World"));
}

FOSSIL_TEST(cpp_test_cstring_starts_with)
{
    fossil::io::CString str("HelloWorld");
    ASSUME_ITS_TRUE(str.starts_with("Hello"));
}

FOSSIL_TEST(cpp_test_cstring_ends_with)
{
    fossil::io::CString str("HelloWorld");
    ASSUME_ITS_TRUE(str.ends_with("World"));
}

FOSSIL_TEST(cpp_test_cstring_substring)
{
    fossil::io::CString str("HelloWorld");
    fossil::io::CString result = str.substring(0, 5);
    ASSUME_ITS_EQUAL_CSTR(result.str(), "Hello");
}

FOSSIL_TEST(cpp_test_cstring_index_of)
{
    fossil::io::CString str("HelloWorld");
    ASSUME_ITS_EQUAL_I32(str.index_of("World"), 5);
}

// Tests for padding and repetition
FOSSIL_TEST(cpp_test_cstring_pad_left)
{
    fossil::io::CString str("Hi");
    fossil::io::CString result = str.pad_left(5, '*');
    ASSUME_ITS_EQUAL_CSTR(result.str(), "***Hi");
}

FOSSIL_TEST(cpp_test_cstring_pad_right)
{
    fossil::io::CString str("Hi");
    fossil::io::CString result = str.pad_right(5, '*');
    ASSUME_ITS_EQUAL_CSTR(result.str(), "Hi***");
}

FOSSIL_TEST(cpp_test_cstring_repeat)
{
    fossil::io::CString str("ab");
    fossil::io::CString result = str.repeat(3);
    ASSUME_ITS_EQUAL_CSTR(result.str(), "ababab");
}

// Tests for JSON operations
FOSSIL_TEST(cpp_test_escape_json)
{
    fossil::io::CString str("Hello\"World");
    fossil::io::CString result = str.escape_json();
    ASSUME_ITS_TRUE(std::string(result.str()).find("\\\"") != std::string::npos);
}

FOSSIL_TEST(cpp_test_normalize_spaces)
{
    fossil::io::CString str("Hello    World");
    fossil::io::CString result = str.normalize_spaces();
    ASSUME_ITS_TRUE(std::string(result.str()).find("Hello World") != std::string::npos);
}

FOSSIL_TEST(cpp_test_strip_quotes)
{
    fossil::io::CString str("\"Hello\"");
    fossil::io::CString result = str.strip_quotes();
    ASSUME_ITS_EQUAL_CSTR(result.str(), "Hello");
}

// Tests for number conversions
FOSSIL_TEST(cpp_test_number_to_words)
{
    std::string result = fossil::io::CString::number_to_words(23);
    ASSUME_ITS_TRUE(result.length() > 0);
}

FOSSIL_TEST(cpp_test_number_from_words)
{
    int num = fossil::io::CString::number_from_words("twenty-three");
    ASSUME_ITS_EQUAL_I32(num, 23);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_cstring_tests)
{
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_cstring_create_free);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_cstring_create_null);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_money_to_string);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_money_to_string_negative);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_string_to_money);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_case_compare);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_case_search);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_case_starts_with);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_case_ends_with);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_case_replace);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_mocking_case);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_rot13_transform);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_leetspeak);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_piglatin);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_shuffle);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_upper_snake);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_cstring_length);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_cstring_compare);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_cstring_concat);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_cstring_to_upper);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_cstring_to_lower);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_cstring_contains);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_cstring_starts_with);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_cstring_ends_with);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_cstring_substring);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_cstring_index_of);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_cstring_pad_left);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_cstring_pad_right);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_cstring_repeat);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_escape_json);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_normalize_spaces);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_strip_quotes);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_number_to_words);
    FOSSIL_TEST_ADD(cpp_cstring_suite, cpp_test_number_from_words);

    FOSSIL_TEST_REGISTER(cpp_cstring_suite);
}
