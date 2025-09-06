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

using fossil::io::Soap;

FOSSIL_TEST(cpp_test_io_soap_sanitize) {
    std::string input = "This is a rot-brain sentence.";
    std::string expected = "This is a stupid sentence.";
    std::string result = Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_no_offensive) {
    std::string input = "This is a clean sentence.";
    std::string expected = "This is a clean sentence.";
    std::string result = Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_with_punctuation) {
    std::string input = "This is a test with punctuation, and special characters!";
    std::string expected = "This is a test with punctuation, and special characters!";
    std::string result = Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_empty_input) {
    std::string input = "";
    std::string expected = "";
    std::string result = Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_only_whitespace) {
    std::string input = "   ";
    std::string expected = "   ";
    std::string result = Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_long_input) {
    std::string input = "This is an extremely lengthy input string that surpasses the buffer limit";
    std::string expected = "This is an extremely lengthy input string that surpasses the buffer limit";
    std::string result = Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_suggest) {
    std::string input = "This is a rot-brain sentence.";
    std::string expected = "This is a stupid sentence.";
    std::string result = Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_suggest_no_offensive) {
    std::string input = "This is a clean sentence.";
    std::string expected = "This is a clean sentence.";
    std::string result = Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_add_custom_filter) {
    std::string phrase = "custom";
    int result = Soap::add_custom_filter(phrase);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST(cpp_test_io_soap_detect_tone_sarcastic) {
    std::string input = "Oh, great. Another meeting.";
    std::string expected = "sarcastic";
    std::string result = Soap::detect_tone(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_detect_tone_formal) {
    std::string input = "Dear Sir or Madam,";
    std::string expected = "formal";
    std::string result = Soap::detect_tone(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_detect_tone_casual) {
    std::string input = "Hey, what's up?";
    std::string expected = "casual";
    std::string result = Soap::detect_tone(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_leetspeak) {
    std::string input = "Th1s 1s 4 l33tspeak s3nt3nc3.";
    std::string expected = "This is a leetspeak sentence.";
    std::string result = Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_mixed_case) {
    std::string input = "This Is A Rot-Brain Sentence.";
    std::string expected = "This Is A stupid Sentence.";
    std::string result = Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_with_special_chars) {
    std::string input = "This is a test with special chars #$%^&*!";
    std::string expected = "This is a test with special chars #$%^&*!";
    std::string result = Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_with_newlines) {
    std::string input = "This is a test\nwith newlines.";
    std::string expected = "This is a test\nwith newlines.";
    std::string result = Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_sanitize_with_tabs) {
    std::string input = "This is a test\twith tabs.";
    std::string expected = "This is a test\twith tabs.";
    std::string result = Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_suggest_leetspeak) {
    std::string input = "Th1s 1s 4 l33tspeak s3nt3nc3.";
    std::string expected = "This is a leetspeak sentence.";
    std::string result = Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_suggest_mixed_case) {
    std::string input = "This Is A Rot-Brain Sentence.";
    std::string expected = "This Is A stupid Sentence.";
    std::string result = Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_suggest_with_special_chars) {
    std::string input = "This is a test with special chars #$%^&*!";
    std::string expected = "This is a test with special chars #$%^&*!";
    std::string result = Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_suggest_with_newlines) {
    std::string input = "This is a test\nwith newlines.";
    std::string expected = "This is a test\nwith newlines.";
    std::string result = Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_suggest_with_tabs) {
    std::string input = "This is a test\twith tabs.";
    std::string expected = "This is a test\twith tabs.";
    std::string result = Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

FOSSIL_TEST(cpp_test_io_soap_normalize_slang_basic) {
    std::string input = "idk why ppl do that lol";
    std::string expected = "I don't know why people do that laugh out loud";
    std::string result = Soap::normalize_slang(input);
    ASSUME_ITS_EQUAL_CSTR(expected.c_str(), result.c_str());
}

// detect cases

FOSSIL_TEST(cpp_test_io_soap_detect_clickbait_true) {
    std::string input = "You won't believe what happened next!";
    ASSUME_ITS_TRUE(Soap::is_clickbait(input));
}

FOSSIL_TEST(cpp_test_io_soap_detect_clickbait_false) {
    std::string input = "Scientists publish new findings in journal.";
    ASSUME_ITS_TRUE(!Soap::is_clickbait(input));
}

FOSSIL_TEST(cpp_test_io_soap_detect_ragebait_true) {
    std::string input = "This outrageous policy will destroy everything!";
    ASSUME_ITS_TRUE(Soap::is_ragebait(input));
}

FOSSIL_TEST(cpp_test_io_soap_detect_ragebait_false) {
    std::string input = "The policy was discussed in parliament.";
    ASSUME_ITS_TRUE(!Soap::is_ragebait(input));
}

FOSSIL_TEST(cpp_test_io_soap_detect_spam_true) {
    std::string input = "Congratulations! You've won a free iPhone. Click here!";
    ASSUME_ITS_TRUE(Soap::is_spam(input));
}

FOSSIL_TEST(cpp_test_io_soap_detect_spam_false) {
    std::string input = "Thank you for your purchase.";
    ASSUME_ITS_TRUE(!Soap::is_spam(input));
}

FOSSIL_TEST(cpp_test_io_soap_detect_woke_true) {
    std::string input = "We need to raise awareness about social justice issues.";
    ASSUME_ITS_TRUE(Soap::is_woke(input));
}

FOSSIL_TEST(cpp_test_io_soap_detect_woke_false) {
    std::string input = "Let's discuss the quarterly financial report.";
    ASSUME_ITS_TRUE(!Soap::is_woke(input));
}

FOSSIL_TEST(cpp_test_io_soap_detect_bot_true) {
    std::string input = "Hello, I am an automated assistant. How may I help you?";
    ASSUME_ITS_TRUE(Soap::is_bot(input));
}

FOSSIL_TEST(cpp_test_io_soap_detect_bot_false) {
    std::string input = "Hey, are you coming to the party tonight?";
    ASSUME_ITS_TRUE(!Soap::is_bot(input));
}

FOSSIL_TEST(cpp_test_io_soap_detect_sarcastic_true) {
    std::string input = "Oh, fantastic, another bug in production!";
    ASSUME_ITS_TRUE(Soap::is_sarcastic(input));
}

FOSSIL_TEST(cpp_test_io_soap_detect_sarcastic_false) {
    std::string input = "The deployment was successful.";
    ASSUME_ITS_TRUE(!Soap::is_sarcastic(input));
}

FOSSIL_TEST(cpp_test_io_soap_detect_formal_true) {
    std::string input = "To whom it may concern, please find attached the requested documents.";
    ASSUME_ITS_TRUE(Soap::is_formal(input));
}

FOSSIL_TEST(cpp_test_io_soap_detect_formal_false) {
    std::string input = "Hey, check this out!";
    ASSUME_ITS_TRUE(!Soap::is_formal(input));
}

FOSSIL_TEST(cpp_test_io_soap_detect_snowflake_true) {
    std::string input = "People these days are such snowflakes.";
    ASSUME_ITS_TRUE(Soap::is_snowflake(input));
}

FOSSIL_TEST(cpp_test_io_soap_detect_snowflake_false) {
    std::string input = "The weather is cold and snowy.";
    ASSUME_ITS_TRUE(!Soap::is_snowflake(input));
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
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_normalize_slang_basic);

    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_clickbait_true);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_clickbait_false);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_ragebait_true);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_ragebait_false);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_spam_true);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_spam_false);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_woke_true);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_woke_false);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_bot_true);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_bot_false);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_sarcastic_true);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_sarcastic_false);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_formal_true);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_formal_false);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_snowflake_true);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_io_soap_detect_snowflake_false);

    FOSSIL_TEST_REGISTER(cpp_soap_suite);
}
