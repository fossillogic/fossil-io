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
FOSSIL_SUITE(c_cstring_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_cstring_suite)
{
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_cstring_suite)
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

// Tests for cstring_create and cstring_free
FOSSIL_TEST(c_test_cstring_create_free)
{
    const char *init = "Hello";
    cstring str = fossil_io_cstring_create(init);
    ASSUME_ITS_TRUE(str != NULL);
    ASSUME_ITS_EQUAL_CSTR(str, "Hello");
    fossil_io_cstring_free(str);
}

FOSSIL_TEST(c_test_cstring_create_null)
{
    cstring str = fossil_io_cstring_create(NULL);
    ASSUME_ITS_TRUE(str == NULL);
}

// Tests for money conversions
FOSSIL_TEST(c_test_money_to_string)
{
    char output[64];
    int result = fossil_io_cstring_money_to_string(1234.56, output, sizeof(output));
    ASSUME_ITS_EQUAL_I32(result, 0);
    ASSUME_ITS_TRUE(strstr(output, "1") != NULL);
}

FOSSIL_TEST(c_test_money_to_string_negative)
{
    char output[64];
    int result = fossil_io_cstring_money_to_string(-99.99, output, sizeof(output));
    ASSUME_ITS_EQUAL_I32(result, 0);
    ASSUME_ITS_TRUE(strstr(output, "-") != NULL);
}

FOSSIL_TEST(c_test_string_to_money)
{
    double amount;
    int result = fossil_io_cstring_string_to_money("1234.56", &amount);
    ASSUME_ITS_EQUAL_I32(result, 0);
    ASSUME_ITS_TRUE(amount > 1234.0 && amount < 1235.0);
}

// Tests for case-insensitive operations
FOSSIL_TEST(c_test_case_compare)
{
    int result = fossil_io_cstring_case_compare("Hello", "hello");
    ASSUME_ITS_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_case_search)
{
    ccstring result = fossil_io_cstring_case_search("Hello World", "world");
    ASSUME_ITS_TRUE(result != NULL);
}

FOSSIL_TEST(c_test_case_starts_with)
{
    int result = fossil_io_cstring_case_starts_with("HelloWorld", "hello");
    ASSUME_ITS_EQUAL_I32(result, 1);
}

FOSSIL_TEST(c_test_case_ends_with)
{
    int result = fossil_io_cstring_case_ends_with("HelloWorld", "WORLD");
    ASSUME_ITS_EQUAL_I32(result, 1);
}

FOSSIL_TEST(c_test_case_replace)
{
    cstring result = fossil_io_cstring_case_replace("Hello World", "world", "Universe");
    ASSUME_ITS_TRUE(result != NULL);
    ASSUME_ITS_TRUE(strstr(result, "Universe") != NULL);
    free(result);
}

// Tests for novelty transformations
FOSSIL_TEST(c_test_mocking_case)
{
    cstring result = fossil_io_cstring_mocking("hello");
    ASSUME_ITS_TRUE(result != NULL);
    free(result);
}

FOSSIL_TEST(c_test_rot13_transform)
{
    cstring result = fossil_io_cstring_rot13("hello");
    ASSUME_ITS_TRUE(result != NULL);
    ASSUME_ITS_EQUAL_CSTR(result, "uryyb");
    free(result);
}

FOSSIL_TEST(c_test_leetspeak)
{
    char output[128];
    int result = fossil_io_cstring_leetspeak("ELITE", output, sizeof(output));
    ASSUME_ITS_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_piglatin)
{
    char output[128];
    int result = fossil_io_cstring_piglatin("hello world", output, sizeof(output));
    ASSUME_ITS_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_shuffle)
{
    cstring result = fossil_io_cstring_shuffle("abcdef");
    ASSUME_ITS_TRUE(result != NULL);
    ASSUME_ITS_LENGTH_EQUAL_CSTR(result, 6);
    free(result);
}

FOSSIL_TEST(c_test_upper_snake)
{
    cstring result = fossil_io_cstring_upper_snake("Hello World");
    ASSUME_ITS_TRUE(result != NULL);
    ASSUME_ITS_TRUE(strstr(result, "HELLO") != NULL);
    free(result);
}

// Tests for basic string operations
FOSSIL_TEST(c_test_cstring_length)
{
    size_t len = fossil_io_cstring_length("Hello");
    ASSUME_ITS_EQUAL_U64(len, 5);
}

FOSSIL_TEST(c_test_cstring_compare)
{
    int result = fossil_io_cstring_compare("abc", "abc");
    ASSUME_ITS_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_cstring_concat)
{
    cstring result = fossil_io_cstring_concat("Hello", "World");
    ASSUME_ITS_TRUE(result != NULL);
    ASSUME_ITS_EQUAL_CSTR(result, "HelloWorld");
    free(result);
}

FOSSIL_TEST(c_test_cstring_to_upper)
{
    cstring result = fossil_io_cstring_to_upper("hello");
    ASSUME_ITS_TRUE(result != NULL);
    ASSUME_ITS_EQUAL_CSTR(result, "HELLO");
    free(result);
}

FOSSIL_TEST(c_test_cstring_to_lower)
{
    cstring result = fossil_io_cstring_to_lower("HELLO");
    ASSUME_ITS_TRUE(result != NULL);
    ASSUME_ITS_EQUAL_CSTR(result, "hello");
    free(result);
}

// Tests for substring and search operations
FOSSIL_TEST(c_test_cstring_contains)
{
    int result = fossil_io_cstring_contains("Hello World", "World");
    ASSUME_ITS_EQUAL_I32(result, 1);
}

FOSSIL_TEST(c_test_cstring_starts_with)
{
    int result = fossil_io_cstring_starts_with("HelloWorld", "Hello");
    ASSUME_ITS_EQUAL_I32(result, 1);
}

FOSSIL_TEST(c_test_cstring_ends_with)
{
    int result = fossil_io_cstring_ends_with("HelloWorld", "World");
    ASSUME_ITS_EQUAL_I32(result, 1);
}

FOSSIL_TEST(c_test_cstring_substring)
{
    cstring result = fossil_io_cstring_substring("HelloWorld", 0, 5);
    ASSUME_ITS_TRUE(result != NULL);
    ASSUME_ITS_EQUAL_CSTR(result, "Hello");
    free(result);
}

FOSSIL_TEST(c_test_cstring_index_of)
{
    int idx = fossil_io_cstring_index_of("HelloWorld", "World");
    ASSUME_ITS_EQUAL_I32(idx, 5);
}

// Tests for padding and repetition
FOSSIL_TEST(c_test_cstring_pad_left)
{
    cstring result = fossil_io_cstring_pad_left("Hi", 5, '*');
    ASSUME_ITS_TRUE(result != NULL);
    ASSUME_ITS_EQUAL_CSTR(result, "***Hi");
    free(result);
}

FOSSIL_TEST(c_test_cstring_pad_right)
{
    cstring result = fossil_io_cstring_pad_right("Hi", 5, '*');
    ASSUME_ITS_TRUE(result != NULL);
    ASSUME_ITS_EQUAL_CSTR(result, "Hi***");
    free(result);
}

FOSSIL_TEST(c_test_cstring_repeat)
{
    cstring result = fossil_io_cstring_repeat("ab", 3);
    ASSUME_ITS_TRUE(result != NULL);
    ASSUME_ITS_EQUAL_CSTR(result, "ababab");
    free(result);
}

// Tests for JSON operations
FOSSIL_TEST(c_test_escape_json)
{
    cstring result = fossil_io_cstring_escape_json("Hello\"World");
    ASSUME_ITS_TRUE(result != NULL);
    ASSUME_ITS_TRUE(strstr(result, "\\\"") != NULL);
    free(result);
}

FOSSIL_TEST(c_test_normalize_spaces)
{
    cstring result = fossil_io_cstring_normalize_spaces("Hello    World");
    ASSUME_ITS_TRUE(result != NULL);
    ASSUME_ITS_TRUE(strstr(result, "Hello World") != NULL);
    free(result);
}

FOSSIL_TEST(c_test_strip_quotes)
{
    cstring result = fossil_io_cstring_strip_quotes("\"Hello\"");
    ASSUME_ITS_TRUE(result != NULL);
    ASSUME_ITS_EQUAL_CSTR(result, "Hello");
    free(result);
}

// Tests for number conversions
FOSSIL_TEST(c_test_number_to_words)
{
    char buffer[128];
    int result = fossil_io_cstring_number_to_words(23, buffer, sizeof(buffer));
    ASSUME_ITS_EQUAL_I32(result, 0);
    ASSUME_ITS_TRUE(strlen(buffer) > 0);
}

FOSSIL_TEST(c_test_number_from_words)
{
    int num;
    int result = fossil_io_cstring_number_from_words("twenty-three", &num);
    ASSUME_ITS_EQUAL_I32(result, 0);
    ASSUME_ITS_EQUAL_I32(num, 23);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_cstring_tests)
{
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_cstring_create_free);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_cstring_create_null);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_money_to_string);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_money_to_string_negative);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_string_to_money);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_case_compare);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_case_search);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_case_starts_with);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_case_ends_with);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_case_replace);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_mocking_case);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_rot13_transform);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_leetspeak);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_piglatin);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_shuffle);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_upper_snake);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_cstring_length);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_cstring_compare);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_cstring_concat);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_cstring_to_upper);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_cstring_to_lower);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_cstring_contains);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_cstring_starts_with);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_cstring_ends_with);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_cstring_substring);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_cstring_index_of);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_cstring_pad_left);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_cstring_pad_right);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_cstring_repeat);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_escape_json);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_normalize_spaces);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_strip_quotes);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_number_to_words);
    FOSSIL_TEST_ADD(c_cstring_suite, c_test_number_from_words);

    FOSSIL_TEST_REGISTER(c_cstring_suite);
}
