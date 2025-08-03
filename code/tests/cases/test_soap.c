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
#include <fossil/pizza/framework.h>

#include "fossil/io/framework.h"

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilites
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Define the test suite and add test cases
FOSSIL_SUITE(c_soap_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_soap_suite) {
    // Setup code here
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

FOSSIL_TEST(c_test_io_soap_sanitize) {
    const char *input = "This is a rot-brain sentence.";
    const char *expected = "This is a stupid sentence.";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_sanitize_no_offensive) {
    const char *input = "This is a clean sentence.";
    const char *expected = "This is a clean sentence.";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_sanitize_with_punctuation) {
    const char *input = "This is a test with punctuation, and special characters!";
    const char *expected = "This is a test with punctuation, and special characters!";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_sanitize_empty_input) {
    const char *input = "";
    const char *expected = "";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_sanitize_only_whitespace) {
    const char *input = "   ";
    const char *expected = "   ";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_sanitize_long_input) {
    const char *input = "This is an extremely lengthy input string that surpasses the buffer limit";
    const char *expected = "This is an extremely lengthy input string that surpasses the buffer limit";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_suggest) {
    const char *input = "This is a rot-brain sentence.";
    const char *expected = "This is a stupid sentence.";
    char *result = fossil_io_soap_suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_suggest_no_offensive) {
    const char *input = "This is a clean sentence.";
    const char *expected = "This is a clean sentence.";
    char *result = fossil_io_soap_suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_add_custom_filter) {
    const char *phrase = "custom";
    int result = fossil_io_soap_add_custom_filter(phrase);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST(c_test_io_soap_detect_tone_sarcastic) {
    const char *input = "Oh, great. Another meeting.";
    const char *expected = "sarcastic";
    const char *result = fossil_io_soap_detect_tone(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
}

FOSSIL_TEST(c_test_io_soap_detect_tone_formal) {
    const char *input = "Dear Sir or Madam,";
    const char *expected = "formal";
    const char *result = fossil_io_soap_detect_tone(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
}

FOSSIL_TEST(c_test_io_soap_detect_tone_casual) {
    const char *input = "Hey, what's up?";
    const char *expected = "casual";
    const char *result = fossil_io_soap_detect_tone(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
}

FOSSIL_TEST(c_test_io_soap_sanitize_leetspeak) {
    const char *input = "Th1s 1s 4 l33tspeak s3nt3nc3.";
    const char *expected = "This is a leetspeak sentence.";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_sanitize_mixed_case) {
    const char *input = "This Is A Rot-Brain Sentence.";
    const char *expected = "This Is A stupid Sentence.";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_sanitize_with_special_chars) {
    const char *input = "This is a test with special chars #$%^&*!";
    const char *expected = "This is a test with special chars #$%^&*!";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_sanitize_with_newlines) {
    const char *input = "This is a test\nwith newlines.";
    const char *expected = "This is a test\nwith newlines.";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_sanitize_with_tabs) {
    const char *input = "This is a test\twith tabs.";
    const char *expected = "This is a test\twith tabs.";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_suggest_leetspeak) {
    const char *input = "Th1s 1s 4 l33tspeak s3nt3nc3.";
    const char *expected = "This is a leetspeak sentence.";
    char *result = fossil_io_soap_suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_suggest_mixed_case) {
    const char *input = "This Is A Rot-Brain Sentence.";
    const char *expected = "This Is A stupid Sentence.";
    char *result = fossil_io_soap_suggest(input);

    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_suggest_with_special_chars) {
    const char *input = "This is a test with special chars #$%^&*!";
    const char *expected = "This is a test with special chars #$%^&*!";
    char *result = fossil_io_soap_suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_suggest_with_newlines) {
    const char *input = "This is a test\nwith newlines.";
    const char *expected = "This is a test\nwith newlines.";
    char *result = fossil_io_soap_suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_suggest_with_tabs) {
    const char *input = "This is a test\twith tabs.";
    const char *expected = "This is a test\twith tabs.";
    char *result = fossil_io_soap_suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_check_grammar_valid) {
    const char *input = "She writes clearly and concisely.";
    ASSUME_ITS_TRUE(fossil_io_soap_check_grammar(input) == 0);
}

FOSSIL_TEST(c_test_io_soap_check_grammar_error) {
    const char *input = "Him go store.";
    ASSUME_ITS_TRUE(fossil_io_soap_check_grammar(input) == 1);
}

FOSSIL_TEST(c_test_io_soap_normalize_informal) {
    const char *input = "u gotta see this";
    const char *expected = "you have to see this";
    char *result = fossil_io_soap_normalize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_normalize_slang_basic) {
    const char *input = "idk why ppl do that lol";
    const char *expected = "I don't know why people do that laugh out loud";
    char *result = fossil_io_soap_normalize_slang(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_detect_exaggeration_true) {
    const char *input = "This is the worst thing to ever happen in history!";
    ASSUME_ITS_TRUE(fossil_io_soap_detect_exaggeration(input) == 1);
}

FOSSIL_TEST(c_test_io_soap_detect_exaggeration_false) {
    const char *input = "The weather is mildly unpleasant today.";
    ASSUME_ITS_TRUE(fossil_io_soap_detect_exaggeration(input) == 0);
}

FOSSIL_TEST(c_test_io_soap_filter_offensive_basic) {
    const char *input = "You're an idiot.";
    const char *expected = "You're an misguided.";
    char *result = fossil_io_soap_filter_offensive(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_detect_clickbait_true) {
    const char *input = "You won't believe what happened next!";
    ASSUME_ITS_TRUE(fossil_io_soap_detect_clickbait(input) == 1);
}

FOSSIL_TEST(c_test_io_soap_detect_clickbait_false) {
    const char *input = "Scientists publish new findings in journal.";
    ASSUME_ITS_TRUE(fossil_io_soap_detect_clickbait(input) == 0);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_soap_tests) {
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_no_offensive);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_with_punctuation);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_empty_input);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_only_whitespace);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_long_input);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_suggest);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_suggest_no_offensive);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_add_custom_filter);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_detect_tone_sarcastic);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_detect_tone_formal);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_detect_tone_casual);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_leetspeak);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_mixed_case);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_with_special_chars);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_with_newlines);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_sanitize_with_tabs);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_suggest_leetspeak);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_suggest_mixed_case);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_suggest_with_special_chars);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_suggest_with_newlines);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_suggest_with_tabs);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_check_grammar_valid);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_check_grammar_error);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_normalize_informal);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_normalize_slang_basic);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_detect_exaggeration_true);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_detect_exaggeration_false);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_filter_offensive_basic);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_detect_clickbait_true);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_detect_clickbait_false);

    FOSSIL_TEST_REGISTER(c_soap_suite);
}
