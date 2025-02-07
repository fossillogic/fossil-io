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
#include <fossil/test/framework.h>

#include "fossil/io/framework.h"

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilites
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Define the test suite and add test cases
FOSSIL_TEST_SUITE(c_soap_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_soap_suite) {
    fossil_io_soap_create();
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_soap_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_CASE(c_test_io_soap_sanitize_no_offensive) {
    char input[] = "This is a clean sentence.";
    char expected[] = "This is a clean sentence.";
    char *sanitized = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, sanitized);
}

FOSSIL_TEST_CASE(c_test_io_soap_sanitize_with_offensive) {
    char input[] = "This is a test with curse1 and racist_phrase1.";
    char expected[] = "This is a test with *** and ***.";
    char *sanitized = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, sanitized);
}

FOSSIL_TEST_CASE(c_test_io_soap_is_offensive_true) {
    const char *word = "curse1";
    int32_t result = fossil_io_soap_is_offensive(word);
    ASSUME_ITS_EQUAL_I32(EXIT_FAILURE, result);
}

FOSSIL_TEST_CASE(c_test_io_soap_is_offensive_false) {
    const char *word = "hello";
    int32_t result = fossil_io_soap_is_offensive(word);
    ASSUME_ITS_EQUAL_I32(EXIT_SUCCESS, result);
}

FOSSIL_TEST_CASE(c_test_io_soap_is_rotbrain_true) {
    const char *word = "meme1";
    int32_t result = fossil_io_soap_is_rotbrain(word);
    ASSUME_ITS_EQUAL_I32(EXIT_FAILURE, result);
}

FOSSIL_TEST_CASE(c_test_io_soap_is_rotbrain_false) {
    const char *word = "normal";
    int32_t result = fossil_io_soap_is_rotbrain(word);
    ASSUME_ITS_EQUAL_I32(EXIT_SUCCESS, result);
}

FOSSIL_TEST_CASE(c_test_io_soap_count_offensive) {
    const char *input = "This is a test with curse1 and racist_phrase1.";
    int32_t result = fossil_io_soap_count_offensive(input);
    ASSUME_ITS_EQUAL_I32(2, result);
}

FOSSIL_TEST_CASE(c_test_io_soap_count_rotbrain) {
    const char *input = "This is a test with meme1 and meme2.";
    int32_t result = fossil_io_soap_count_rotbrain(input);
    ASSUME_ITS_EQUAL_I32(2, result);
}

FOSSIL_TEST_CASE(c_test_io_soap_count_positive) {
    const char *input = "This is a wonderful and amazing day.";
    int32_t result = fossil_io_soap_count_positive(input);
    ASSUME_ITS_EQUAL_I32(2, result);
}

FOSSIL_TEST_CASE(c_test_io_soap_sanitize_with_offensive_punctuation) {
    char input[] = "This is a test with curse1, and racist_phrase1!";
    char expected[] = "This is a test with ***, and ***!";
    char *sanitized = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, sanitized);
}

FOSSIL_TEST_CASE(c_test_io_soap_sanitize_with_rotbrain) {
    char input[] = "This is a test with rizz and yeet.";
    char expected[] = "This is a test with [ROT] and [ROT].";
    char *sanitized = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, sanitized);
}

FOSSIL_TEST_CASE(c_test_io_soap_sanitize_with_rotbrain_punctuation) {
    char input[] = "This is a test with rizz, and yeet!";
    char expected[] = "This is a test with [ROT], and [ROT]!";
    char *sanitized = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, sanitized);
}

FOSSIL_TEST_CASE(c_test_io_soap_is_offensive_with_punctuation) {
    const char *word = "curse1!";
    int32_t result = fossil_io_soap_is_offensive(word);
    ASSUME_ITS_EQUAL_I32(EXIT_FAILURE, result);
}

FOSSIL_TEST_CASE(c_test_io_soap_is_rotbrain_with_punctuation) {
    const char *word = "rizz!";
    int32_t result = fossil_io_soap_is_rotbrain(word);
    ASSUME_ITS_EQUAL_I32(EXIT_FAILURE, result);
}

FOSSIL_TEST_CASE(c_test_io_soap_count_offensive_with_punctuation) {
    const char *input = "This is a test with curse1, and racist_phrase1!";
    int32_t result = fossil_io_soap_count_offensive(input);
    ASSUME_ITS_EQUAL_I32(2, result);
}

FOSSIL_TEST_CASE(c_test_io_soap_count_rotbrain_with_punctuation) {
    const char *input = "This is a test with rizz, and yeet!";
    int32_t result = fossil_io_soap_count_rotbrain(input);
    ASSUME_ITS_EQUAL_I32(2, result);
}

FOSSIL_TEST_CASE(c_test_io_soap_sanitize_empty_string) {
    char input[] = "";
    char expected[] = "";
    char *sanitized = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, sanitized);
}

FOSSIL_TEST_CASE(c_test_io_soap_sanitize_only_offensive) {
    char input[] = "curse1";
    char expected[] = "***";
    char *sanitized = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, sanitized);
}

FOSSIL_TEST_CASE(c_test_io_soap_sanitize_only_rotbrain) {
    char input[] = "rizz";
    char expected[] = "[ROT]";
    char *sanitized = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, sanitized);
}

FOSSIL_TEST_CASE(c_test_io_soap_sanitize_mixed_case) {
    char input[] = "CurSe1 and Racist_Phrase1";
    char expected[] = "*** and ***";
    char *sanitized = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, sanitized);
}

FOSSIL_TEST_CASE(c_test_io_soap_sanitize_with_numbers) {
    char input[] = "This is a test with curse1 and 123.";
    char expected[] = "This is a test with *** and 123.";
    char *sanitized = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, sanitized);
}

FOSSIL_TEST_CASE(c_test_io_soap_is_offensive_empty_string) {
    const char *word = "";
    int32_t result = fossil_io_soap_is_offensive(word);
    ASSUME_ITS_EQUAL_I32(EXIT_SUCCESS, result);
}

FOSSIL_TEST_CASE(c_test_io_soap_is_rotbrain_empty_string) {
    const char *word = "";
    int32_t result = fossil_io_soap_is_rotbrain(word);
    ASSUME_ITS_EQUAL_I32(EXIT_SUCCESS, result);
}

FOSSIL_TEST_CASE(c_test_io_soap_count_offensive_empty_string) {
    const char *input = "";
    int32_t result = fossil_io_soap_count_offensive(input);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST_CASE(c_test_io_soap_count_rotbrain_empty_string) {
    const char *input = "";
    int32_t result = fossil_io_soap_count_rotbrain(input);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST_CASE(c_test_io_soap_count_offensive_no_offensive) {
    const char *input = "This is a clean sentence.";
    int32_t result = fossil_io_soap_count_offensive(input);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST_CASE(c_test_io_soap_count_rotbrain_no_rotbrain) {
    const char *input = "This is a clean sentence.";
    int32_t result = fossil_io_soap_count_rotbrain(input);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST_CASE(c_test_io_soap_sanitize_with_mixed_offensive_rotbrain) {
    char input[] = "This is a test with curse1 and rizz.";
    char expected[] = "This is a test with *** and [ROT].";
    char *sanitized = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, sanitized);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_soap_tests) {
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_no_offensive);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_with_offensive);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_is_offensive_true);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_is_offensive_false);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_is_rotbrain_true);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_is_rotbrain_false);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_count_offensive);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_count_rotbrain);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_count_positive);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_with_offensive_punctuation);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_with_rotbrain);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_with_rotbrain_punctuation);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_is_offensive_with_punctuation);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_is_rotbrain_with_punctuation);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_count_offensive_with_punctuation);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_count_rotbrain_with_punctuation);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_empty_string);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_only_offensive);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_only_rotbrain);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_mixed_case);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_with_numbers);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_is_offensive_empty_string);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_is_rotbrain_empty_string);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_count_offensive_empty_string);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_count_rotbrain_empty_string);

    FOSSIL_TEST_REGISTER(c_soap_suite);
}
