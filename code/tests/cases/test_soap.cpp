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
FOSSIL_SUITE(cpp_soap_suite);

// Setup function for the test suite
FOSSIL_SETUP(cpp_soap_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_soap_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(cpp_test_io_soap_sanitize) {
    const char *input = "This is a rot-brain sentence.";
    const char *expected = "This is a stupid sentence.";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_no_offensive) {
    const char *input = "This is a clean sentence.";
    const char *expected = "This is a clean sentence.";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_with_punctuation) {
    const char *input = "This is a test with punctuation, and special characters!";
    const char *expected = "This is a test with punctuation, and special characters!";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_empty_input) {
    const char *input = "";
    const char *expected = "";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_only_whitespace) {
    const char *input = "   ";
    const char *expected = "   ";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_long_input) {
    const char *input = "This is a very long input string that exceeds the buffer size";
    const char *expected = "This is a very long input string that exceeds the buffer size";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_suggest) {
    const char *input = "This is a rot-brain sentence.";
    const char *expected = "This is a stupid sentence.";
    char *result = fossil_io_soap_suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_suggest_no_offensive) {
    const char *input = "This is a clean sentence.";
    const char *expected = "This is a clean sentence.";
    char *result = fossil_io_soap_suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_add_custom_filter) {
    const char *phrase = "custom";
    int result = fossil_io_soap_add_custom_filter(phrase);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST(cpp_test_io_soap_detect_tone_sarcastic) {
    const char *input = "Oh, great. Another meeting.";
    const char *expected = "sarcastic";
    const char *result = fossil_io_soap_detect_tone(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
}

FOSSIL_TEST(cpp_test_io_soap_detect_tone_formal) {
    const char *input = "Dear Sir or Madam,";
    const char *expected = "formal";
    const char *result = fossil_io_soap_detect_tone(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
}

FOSSIL_TEST(cpp_test_io_soap_detect_tone_casual) {
    const char *input = "Hey, what's up?";
    const char *expected = "casual";
    const char *result = fossil_io_soap_detect_tone(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_leetspeak) {
    const char *input = "Th1s 1s 4 l33tspeak s3nt3nc3.";
    const char *expected = "This is a leetspeak sentence.";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_mixed_case) {
    const char *input = "This Is A Rot-Brain Sentence.";
    const char *expected = "This Is A stupid Sentence.";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_with_special_chars) {
    const char *input = "This is a test with special chars #$%^&*!";
    const char *expected = "This is a test with special chars #$%^&*!";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_with_newlines) {
    const char *input = "This is a test\nwith newlines.";
    const char *expected = "This is a test\nwith newlines.";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_with_tabs) {
    const char *input = "This is a test\twith tabs.";
    const char *expected = "This is a test\twith tabs.";
    char *result = fossil_io_soap_sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_suggest_leetspeak) {
    const char *input = "Th1s 1s 4 l33tspeak s3nt3nc3.";
    const char *expected = "This is a leetspeak sentence.";
    char *result = fossil_io_soap_suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_suggest_mixed_case) {
    const char *input = "This Is A Rot-Brain Sentence.";
    const char *expected = "This Is A stupid Sentence.";
    char *result = fossil_io_soap_suggest(input);

    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_suggest_with_special_chars) {
    const char *input = "This is a test with special chars #$%^&*!";
    const char *expected = "This is a test with special chars #$%^&*!";
    char *result = fossil_io_soap_suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_suggest_with_newlines) {
    const char *input = "This is a test\nwith newlines.";
    const char *expected = "This is a test\nwith newlines.";
    char *result = fossil_io_soap_suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_suggest_with_tabs) {
    const char *input = "This is a test\twith tabs.";
    const char *expected = "This is a test\twith tabs.";
    char *result = fossil_io_soap_suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_stl) {
    std::string input = "This is a rot-brain sentence.";
    std::string expected = "This is a stupid sentence.";
    std::string result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_no_offensive_stl) {
    std::string input = "This is a clean sentence.";
    std::string expected = "This is a clean sentence.";
    std::string result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_with_punctuation_stl) {
    std::string input = "This is a test with punctuation, and special characters!";
    std::string expected = "This is a test with punctuation, and special characters!";
    std::string result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_empty_input_stl) {
    std::string input = "";
    std::string expected = "";
    std::string result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_only_whitespace_stl) {
    std::string input = "   ";
    std::string expected = "   ";
    std::string result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_long_input_stl) {
    std::string input = "This is a very long input string that exceeds the buffer size";
    std::string expected = "This is a very long input string that exceeds the buffer size";
    std::string result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_suggest_stl) {
    std::string input = "This is a rot-brain sentence.";
    std::string expected = "This is a stupid sentence.";
    std::string result = fossil::io::Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_suggest_no_offensive_stl) {
    std::string input = "This is a clean sentence.";
    std::string expected = "This is a clean sentence.";
    std::string result = fossil::io::Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_add_custom_filter_stl) {
    std::string phrase = "custom";
    int result = fossil::io::Soap::add_custom_filter(phrase);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST(cpp_test_io_soap_detect_tone_sarcastic_stl) {
    std::string input = "Oh, great. Another meeting.";
    std::string expected = "sarcastic";
    std::string result = fossil::io::Soap::detect_tone(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_detect_tone_formal_stl) {
    std::string input = "Dear Sir or Madam,";
    std::string expected = "formal";
    std::string result = fossil::io::Soap::detect_tone(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_detect_tone_casual_stl) {
    std::string input = "Hey, what's up?";
    std::string expected = "casual";
    std::string result = fossil::io::Soap::detect_tone(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_leetspeak_stl) {
    std::string input = "Th1s 1s 4 l33tspeak s3nt3nc3.";
    std::string expected = "This is a leetspeak sentence.";
    std::string result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_mixed_case_stl) {
    std::string input = "This Is A Rot-Brain Sentence.";
    std::string expected = "This Is A stupid Sentence.";
    std::string result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_with_special_chars_stl) {
    std::string input = "This is a test with special chars #$%^&*!";
    std::string expected = "This is a test with special chars #$%^&*!";
    std::string result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_with_newlines_stl) {
    std::string input = "This is a test\nwith newlines.";
    std::string expected = "This is a test\nwith newlines.";
    std::string result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_with_tabs_stl) {
    std::string input = "This is a test\twith tabs.";
    std::string expected = "This is a test\twith tabs.";
    std::string result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_suggest_leetspeak_stl) {
    std::string input = "Th1s 1s 4 l33tspeak s3nt3nc3.";
    std::string expected = "This is a leetspeak sentence.";
    std::string result = fossil::io::Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_suggest_mixed_case_stl) {
    std::string input = "This Is A Rot-Brain Sentence.";
    std::string expected = "This Is A stupid Sentence.";
    std::string result = fossil::io::Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_suggest_with_special_chars_stl) {
    std::string input = "This is a test with special chars #$%^&*!";
    std::string expected = "This is a test with special chars #$%^&*!";
    std::string result = fossil::io::Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_suggest_with_newlines_stl) {
    std::string input = "This is a test\nwith newlines.";
    std::string expected = "This is a test\nwith newlines.";
    std::string result = fossil::io::Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_suggest_with_tabs_stl) {
    std::string input = "This is a test\twith tabs.";
    std::string expected = "This is a test\twith tabs.";
    std::string result = fossil::io::Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_cstr) {
    const char *input = "This is a rot-brain sentence.";
    const char *expected = "This is a stupid sentence.";
    char *result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_no_offensive_cstr) {
    const char *input = "This is a clean sentence.";
    const char *expected = "This is a clean sentence.";
    char *result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_with_punctuation_cstr) {
    const char *input = "This is a test with punctuation, and special characters!";
    const char *expected = "This is a test with punctuation, and special characters!";
    char *result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_empty_input_cstr) {
    const char *input = "";
    const char *expected = "";
    char *result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_only_whitespace_cstr) {
    const char *input = "   ";
    const char *expected = "   ";
    char *result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_long_input_cstr) {
    const char *input = "This is a very long input string that exceeds the buffer size";
    const char *expected = "This is a very long input string that exceeds the buffer size";
    char *result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_suggest_cstr) {
    const char *input = "This is a rot-brain sentence.";
    const char *expected = "This is a stupid sentence.";
    char *result = fossil::io::Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_suggest_no_offensive_cstr) {
    const char *input = "This is a clean sentence.";
    const char *expected = "This is a clean sentence.";
    char *result = fossil::io::Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_add_custom_filter_cstr) {
    const char *phrase = "custom";
    int result = fossil::io::Soap::add_custom_filter(phrase);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST(cpp_test_io_soap_detect_tone_sarcastic_cstr) {
    const char *input = "Oh, great. Another meeting.";
    const char *expected = "sarcastic";
    const char *result = fossil::io::Soap::detect_tone(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
}

FOSSIL_TEST(cpp_test_io_soap_detect_tone_formal_cstr) {
    const char *input = "Dear Sir or Madam,";
    const char *expected = "formal";
    const char *result = fossil::io::Soap::detect_tone(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
}

FOSSIL_TEST(cpp_test_io_soap_detect_tone_casual_cstr) {
    const char *input = "Hey, what's up?";
    const char *expected = "casual";
    const char *result = fossil::io::Soap::detect_tone(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_leetspeak_cstr) {
    const char *input = "Th1s 1s 4 l33tspeak s3nt3nc3.";
    const char *expected = "This is a leetspeak sentence.";
    char *result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_mixed_case_cstr) {
    const char *input = "This Is A Rot-Brain Sentence.";
    const char *expected = "This Is A stupid Sentence.";
    char *result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_with_special_chars_cstr) {
    const char *input = "This is a test with special chars #$%^&*!";
    const char *expected = "This is a test with special chars #$%^&*!";
    char *result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_with_newlines_cstr) {
    const char *input = "This is a test\nwith newlines.";
    const char *expected = "This is a test\nwith newlines.";
    char *result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_with_tabs_cstr) {
    const char *input = "This is a test\twith tabs.";
    const char *expected = "This is a test\twith tabs.";
    char *result = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_suggest_leetspeak_cstr) {
    const char *input = "Th1s 1s 4 l33tspeak s3nt3nc3.";
    const char *expected = "This is a leetspeak sentence.";
    char *result = fossil::io::Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_suggest_mixed_case_cstr) {
    const char *input = "This Is A Rot-Brain Sentence.";
    const char *expected = "This Is A stupid Sentence.";
    char *result = fossil::io::Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_suggest_with_special_chars_cstr) {
    const char *input = "This is a test with special chars #$%^&*!";
    const char *expected = "This is a test with special chars #$%^&*!";
    char *result = fossil::io::Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_suggest_with_newlines_cstr) {
    const char *input = "This is a test\nwith newlines.";
    const char *expected = "This is a test\nwith newlines.";
    char *result = fossil::io::Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

FOSSIL_TEST(cpp_test_io_soap_suggest_with_tabs_cstr) {
    const char *input = "This is a test\twith tabs.";
    const char *expected = "This is a test\twith tabs.";
    char *result = fossil::io::Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected, result);
    free(result);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_soap_tests) {
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_no_offensive);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_with_punctuation);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_empty_input);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_only_whitespace);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_long_input);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_no_offensive);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_add_custom_filter);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_tone_sarcastic);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_tone_formal);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_tone_casual);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_leetspeak);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_mixed_case);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_with_special_chars);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_with_newlines);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_with_tabs);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_leetspeak);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_mixed_case);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_with_special_chars);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_with_newlines);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_with_tabs);

    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_no_offensive_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_with_punctuation_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_empty_input_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_only_whitespace_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_long_input_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_no_offensive_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_add_custom_filter_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_tone_sarcastic_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_tone_formal_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_tone_casual_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_leetspeak_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_mixed_case_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_with_special_chars_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_with_newlines_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_with_tabs_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_leetspeak_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_mixed_case_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_with_special_chars_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_with_newlines_stl);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_with_tabs_stl);

    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_no_offensive_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_with_punctuation_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_empty_input_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_only_whitespace_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_long_input_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_no_offensive_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_add_custom_filter_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_tone_sarcastic_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_tone_formal_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_tone_casual_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_leetspeak_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_mixed_case_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_with_special_chars_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_with_newlines_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_sanitize_with_tabs_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_leetspeak_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_mixed_case_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_with_special_chars_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_with_newlines_cstr);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_suggest_with_tabs_cstr);

    FOSSIL_TEST_REGISTER(cpp_soap_suite);
}
