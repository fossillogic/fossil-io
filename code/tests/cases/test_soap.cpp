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
    ASSUME_ITS_TRUE(fossil_soap_is_rotbrain("meme_speak1"));
    ASSUME_ITS_TRUE(fossil_soap_is_rotbrain("meme_speak2"));
    ASSUME_ITS_FALSE(fossil_soap_is_rotbrain("normal_word"));
}

FOSSIL_TEST_CASE(cpp_test_soap_count_rotbrain) {
    char input[] = "This is a test with meme_speak1 and meme_speak2";
    ASSUME_ITS_EQUAL_I32(2, fossil_soap_count_rotbrain(input));
}

FOSSIL_TEST_CASE(cpp_test_soap_correct_grammar) {
    char input[] = "this   is  a  sentence   with  extra   spaces.";
    char expected[] = "This is a sentence with extra spaces.";

    fossil_soap_correct_grammar(input);

    ASSUME_ITS_EQUAL_CSTR(expected, input);
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
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_correct_grammar);

    FOSSIL_TEST_REGISTER(cpp_soap_suite);
}
