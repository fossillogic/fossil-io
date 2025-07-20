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

FOSSIL_TEST(c_test_io_soap_detect_sentiment_positive) {
    const char *input = "I love this product!";
    const char *expected = "positive";
    const char *result = fossil_io_soap_detect_sentiment(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
}

FOSSIL_TEST(c_test_io_soap_detect_sentiment_negative) {
    const char *input = "This is the worst idea ever.";
    const char *expected = "negative";
    const char *result = fossil_io_soap_detect_sentiment(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
}

FOSSIL_TEST(c_test_io_soap_detect_sentiment_neutral) {
    const char *input = "It is a pencil.";
    const char *expected = "neutral";
    const char *result = fossil_io_soap_detect_sentiment(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
}

FOSSIL_TEST(c_test_io_soap_detect_harmful_content) {
    const char *input = "You are worthless.";
    int result = fossil_io_soap_detect_harmful_content(input);
    ASSUME_ITS_EQUAL_I32(1, result);
}

FOSSIL_TEST(c_test_io_soap_normalize_slang) {
    const char *input = "brb, ttyl!";
    const char *expected = "be right back, talk to you later!";
    char *result = fossil_io_soap_normalize_slang(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_correct_grammar) {
    const char *input = "should of gone there";
    const char *expected = "should have gone there";
    char *result = fossil_io_soap_correct_grammar(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_filter_offensive) {
    const char *input = "You are such an idiot.";
    const char *expected = "You are such an ***.";
    char *result = fossil_io_soap_filter_offensive(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(c_test_io_soap_detect_clickbait) {
    const char *input = "You won't believe what happened next!";
    int result = fossil_io_soap_detect_clickbait(input);
    ASSUME_ITS_EQUAL_I32(1, result);
}

FOSSIL_TEST(c_test_io_soap_detect_exaggeration) {
    const char *input = "This is the greatest thing in the entire universe!";
    int result = fossil_io_soap_detect_exaggeration(input);
    ASSUME_ITS_EQUAL_I32(1, result);
}

FOSSIL_TEST(c_test_io_soap_detect_fallacy) {
    const char *input = "If we allow A, then B, C, and D will also happen!";
    const char *expected = "slippery slope"; // assuming this is detected
    const char *result = fossil_io_soap_detect_fallacy(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
}

FOSSIL_TEST(c_test_io_soap_summarize) {
    const char *input = "Although the product has some flaws, it is still worth buying because of its affordability.";
    const char *expected = "affordable despite some flaws";  // approximate key meaning

    char *result = fossil_io_soap_summarize(input);
    ASSUME_ITS_TRUE(result != NULL);

    // Check if expected meaning is included in the result
    ASSUME_ITS_TRUE(strstr(result, expected) != NULL);

    free(result);
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
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_detect_sentiment_positive);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_detect_sentiment_negative);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_detect_sentiment_neutral);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_detect_harmful_content);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_normalize_slang);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_correct_grammar);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_filter_offensive);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_detect_clickbait);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_detect_exaggeration);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_detect_fallacy);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_io_soap_summarize);

    FOSSIL_TEST_REGISTER(c_soap_suite);
}
