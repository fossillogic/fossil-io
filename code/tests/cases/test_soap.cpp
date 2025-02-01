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

using namespace fossil::io;

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilites
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Define the test suite and add test cases
FOSSIL_TEST_SUITE(cpp_soap_suite);

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

FOSSIL_TEST_CASE(cpp_test_soap_sanitize) {
    char input[] = "This is a test with curse1 and racist_phrase1.";
    char expected[] = "This is a test with *** and ***.";

    fossil_soap_sanitize(input);

    ASSUME_ITS_EQUAL_CSTR(expected, input);
}

FOSSIL_TEST_CASE(cpp_test_soap_is_offensive) {
    ASSUME_ITS_TRUE(fossil_soap_is_offensive("curse1"));
    ASSUME_ITS_TRUE(fossil_soap_is_offensive("racist_phrase2"));
    ASSUME_ITS_FALSE(fossil_soap_is_offensive("non_offensive_word"));
}

FOSSIL_TEST_CASE(cpp_test_soap_count_offensive) {
    char input[] = "This is a test with curse1 and racist_phrase1";
    ASSUME_ITS_EQUAL_I32(2, fossil_soap_count_offensive(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_is_rotbrain) {
    ASSUME_ITS_TRUE(fossil_soap_is_rotbrain("lol"));
    ASSUME_ITS_TRUE(fossil_soap_is_rotbrain("brb"));
    ASSUME_ITS_FALSE(fossil_soap_is_rotbrain("hello"));
}

FOSSIL_TEST_CASE(cpp_test_soap_count_rotbrain) {
    char input[] = "This is a test with lol and brb";
    ASSUME_ITS_EQUAL_I32(2, fossil_soap_count_rotbrain(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_sanitize_multiple_offensive) {
    char input[] = "curse1 curse2 racist_phrase1 racist_phrase2";
    char expected[] = "*** *** *** ***";

    fossil_soap_sanitize(input);

    ASSUME_ITS_EQUAL_CSTR(expected, input);
}

FOSSIL_TEST_CASE(cpp_test_soap_sanitize_no_offensive) {
    char input[] = "This is a clean sentence.";
    char expected[] = "This is a clean sentence.";

    fossil_soap_sanitize(input);

    ASSUME_ITS_EQUAL_CSTR(expected, input);
}

FOSSIL_TEST_CASE(cpp_test_soap_is_offensive_case_insensitive) {
    ASSUME_ITS_TRUE(fossil_soap_is_offensive("CuRsE1"));
    ASSUME_ITS_TRUE(fossil_soap_is_offensive("RaCiSt_PhrAsE2"));
    ASSUME_ITS_FALSE(fossil_soap_is_offensive("Non_Offensive_Word"));
}

FOSSIL_TEST_CASE(cpp_test_soap_count_offensive_mixed_case) {
    char input[] = "This is a test with CuRsE1 and RaCiSt_PhrAsE1";
    ASSUME_ITS_EQUAL_I32(2, fossil_soap_count_offensive(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_is_rotbrain_case_insensitive) {
    ASSUME_ITS_TRUE(fossil_soap_is_rotbrain("LoL"));
    ASSUME_ITS_TRUE(fossil_soap_is_rotbrain("BrB"));
    ASSUME_ITS_FALSE(fossil_soap_is_rotbrain("Hello"));
}

FOSSIL_TEST_CASE(cpp_test_soap_count_rotbrain_mixed_case) {
    char input[] = "This is a test with LoL and BrB";
    ASSUME_ITS_EQUAL_I32(2, fossil_soap_count_rotbrain(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_sanitize_synonyms) {
    char input[] = "This is a test with rizz and sus.";
    char expected[] = "This is a test with *** and ***.";

    fossil_soap_sanitize(input);

    ASSUME_ITS_EQUAL_CSTR(expected, input);
}

FOSSIL_TEST_CASE(cpp_test_soap_count_rotbrain_with_punctuation) {
    char input[] = "This is a test with lol, and brb!";
    ASSUME_ITS_EQUAL_I32(2, fossil_soap_count_rotbrain(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_offensive) {
    char input[] = "This is a test with curse1 and racist_phrase1.";
    ASSUME_ITS_EQUAL_I32(1, fossil_soap_context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_rotbrain) {
    char input[] = "This is a test with lol and brb.";
    ASSUME_ITS_EQUAL_I32(2, fossil_soap_context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_clean) {
    char input[] = "This is a clean sentence.";
    ASSUME_ITS_EQUAL_I32(0, fossil_soap_context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_mixed) {
    char input[] = "This is a test with curse1 and lol.";
    ASSUME_ITS_EQUAL_I32(1, fossil_soap_context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_case_insensitive) {
    char input[] = "This is a test with CuRsE1 and LoL.";
    ASSUME_ITS_EQUAL_I32(1, fossil_soap_context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_with_punctuation) {
    char input[] = "This is a test with curse1, and lol!";
    ASSUME_ITS_EQUAL_I32(1, fossil_soap_context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_no_offensive_or_rotbrain) {
    char input[] = "This is a test with no offensive or rotbrain words.";
    ASSUME_ITS_EQUAL_I32(0, fossil_soap_context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_only_rotbrain) {
    char input[] = "This is a test with lol and brb.";
    ASSUME_ITS_EQUAL_I32(2, fossil_soap_context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_only_offensive) {
    char input[] = "This is a test with curse1 and racist_phrase1.";
    ASSUME_ITS_EQUAL_I32(1, fossil_soap_context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_mixed_case) {
    char input[] = "This is a test with CuRsE1 and LoL.";
    ASSUME_ITS_EQUAL_I32(1, fossil_soap_context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_with_special_characters) {
    char input[] = "This is a test with curse1@ and lol#.";
    ASSUME_ITS_EQUAL_I32(1, fossil_soap_context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_with_numbers) {
    char input[] = "This is a test with curse1 and lol123.";
    ASSUME_ITS_EQUAL_I32(1, fossil_soap_context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_with_mixed_content) {
    char input[] = "This is a test with curse1, lol, and clean words.";
    ASSUME_ITS_EQUAL_I32(1, fossil_soap_context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_with_spaces) {
    char input[] = "This is a test with    curse1    and    lol    .";
    ASSUME_ITS_EQUAL_I32(1, fossil_soap_context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_with_tabs) {
    char input[] = "This is a test with\tcurse1\tand\tlol\t.";
    ASSUME_ITS_EQUAL_I32(1, fossil_soap_context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_with_newlines) {
    char input[] = "This is a test with\ncurse1\nand\nlol\n.";
    ASSUME_ITS_EQUAL_I32(1, fossil_soap_context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_with_mixed_whitespace) {
    char input[] = "This is a test with \t\ncurse1 \t\nand \t\nlol \t\n.";
    ASSUME_ITS_EQUAL_I32(1, fossil_soap_context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_with_empty_string) {
    char input[] = "";
    ASSUME_ITS_EQUAL_I32(0, fossil_soap_context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_with_null_string) {
    ASSUME_ITS_EQUAL_I32(0, fossil_soap_context_aware(NULL));
}

FOSSIL_TEST_CASE(cpp_test_soap_sanitize_empty_string) {
    char input[] = "";
    char expected[] = "";

    fossil::io::Soap::sanitize(input);

    ASSUME_ITS_EQUAL_CSTR(expected, input);
}

FOSSIL_TEST_CASE(cpp_test_soap_sanitize_only_offensive) {
    char input[] = "curse1";
    char expected[] = "***";

    fossil::io::Soap::sanitize(input);

    ASSUME_ITS_EQUAL_CSTR(expected, input);
}

FOSSIL_TEST_CASE(cpp_test_soap_is_offensive_empty_string) {
    ASSUME_ITS_FALSE(fossil::io::Soap::is_offensive(""));
}

FOSSIL_TEST_CASE(cpp_test_soap_is_rotbrain_empty_string) {
    ASSUME_ITS_FALSE(fossil::io::Soap::is_rotbrain(""));
}

FOSSIL_TEST_CASE(cpp_test_soap_count_offensive_empty_string) {
    char input[] = "";
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::count_offensive(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_count_rotbrain_empty_string) {
    char input[] = "";
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::count_rotbrain(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_sanitize_mixed_content) {
    char input[] = "This is a test with curse1, lol, and non_offensive_word.";
    char expected[] = "This is a test with ***, ***, and non_offensive_word.";

    fossil::io::Soap::sanitize(input);

    ASSUME_ITS_EQUAL_CSTR(expected, input);
}

FOSSIL_TEST_CASE(cpp_test_soap_count_offensive_mixed_content) {
    char input[] = "This is a test with curse1, curse2, and non_offensive_word.";
    ASSUME_ITS_EQUAL_I32(2, fossil::io::Soap::count_offensive(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_count_rotbrain_mixed_content) {
    char input[] = "This is a test with lol, brb, and non_offensive_word.";
    ASSUME_ITS_EQUAL_I32(2, fossil::io::Soap::count_rotbrain(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_sanitize_with_punctuation) {
    char input[] = "curse1! curse2? racist_phrase1.";
    char expected[] = "***! ***? ***.";

    fossil::io::Soap::sanitize(input);

    ASSUME_ITS_EQUAL_CSTR(expected, input);
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_with_numbers_and_special_characters) {
    char input[] = "This is a test with curse1@123 and lol#456.";
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_with_mixed_case_and_punctuation) {
    char input[] = "This is a test with CuRsE1! and LoL?.";
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_with_multiple_offensive_and_rotbrain) {
    char input[] = "This is a test with curse1, curse2, lol, and brb.";
    ASSUME_ITS_EQUAL_I32(2, fossil::io::Soap::context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_with_only_special_characters) {
    char input[] = "!@#$%^&*()";
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_with_only_numbers) {
    char input[] = "1234567890";
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_with_mixed_numbers_and_words) {
    char input[] = "This is a test with 123curse1 and 456lol.";
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_with_unicode_characters) {
    char input[] = "This is a test with curse1 and lol 😊.";
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::context_aware(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_context_aware_with_long_text) {
    char input[] = "This is a very long test with multiple curse1, curse2, lol, and brb words to check the performance and accuracy of the context aware method.";
    ASSUME_ITS_EQUAL_I32(2, fossil::io::Soap::context_aware(input));
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_soap_tests) {
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_sanitize);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_is_offensive);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_count_offensive);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_is_rotbrain);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_count_rotbrain);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_sanitize_multiple_offensive);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_sanitize_no_offensive);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_is_offensive_case_insensitive);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_count_offensive_mixed_case);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_is_rotbrain_case_insensitive);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_count_rotbrain_mixed_case);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_sanitize_synonyms);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_offensive);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_rotbrain);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_clean);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_mixed);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_case_insensitive);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_with_punctuation);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_no_offensive_or_rotbrain);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_only_rotbrain);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_only_offensive);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_mixed_case);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_with_special_characters);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_with_numbers);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_with_mixed_content);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_with_spaces);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_with_tabs);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_with_newlines);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_with_mixed_whitespace);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_with_empty_string);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_with_null_string);

    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_sanitize_empty_string);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_sanitize_only_offensive);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_is_offensive_empty_string);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_is_rotbrain_empty_string);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_count_offensive_empty_string);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_count_rotbrain_empty_string);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_sanitize_mixed_content);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_count_offensive_mixed_content);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_count_rotbrain_mixed_content);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_sanitize_with_punctuation);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_with_numbers_and_special_characters);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_with_mixed_case_and_punctuation);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_with_multiple_offensive_and_rotbrain);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_with_only_special_characters);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_with_only_numbers);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_with_mixed_numbers_and_words);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_with_unicode_characters);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_context_aware_with_long_text);

    FOSSIL_TEST_REGISTER(cpp_soap_suite);
}
