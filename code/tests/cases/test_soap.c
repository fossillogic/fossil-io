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

FOSSIL_TEST_CASE(c_test_soap_sanitize) {
    char input[] = "This is a test with curse1 and racist_phrase1.";
    char expected[] = "This is a test with *** and ***.";

    fossil_soap_sanitize(input);

    ASSUME_ITS_EQUAL_CSTR(expected, input);
}

FOSSIL_TEST_CASE(c_test_soap_is_offensive) {
    ASSUME_ITS_TRUE(fossil_soap_is_offensive("curse1"));
    ASSUME_ITS_TRUE(fossil_soap_is_offensive("racist_phrase2"));
    ASSUME_ITS_FALSE(fossil_soap_is_offensive("non_offensive_word"));
}

FOSSIL_TEST_CASE(c_test_soap_count_offensive) {
    char input[] = "This is a test with curse1 and racist_phrase1";
    ASSUME_ITS_EQUAL_I32(2, fossil_soap_count_offensive(input));
}

FOSSIL_TEST_CASE(c_test_soap_sanitize_multiple) {
    char input[] = "curse1 and curse2 are both offensive.";
    char expected[] = "*** and *** are both offensive.";

    fossil_soap_sanitize(input);

    ASSUME_ITS_EQUAL_CSTR(expected, input);
}

FOSSIL_TEST_CASE(c_test_soap_sanitize_no_offensive) {
    char input[] = "This is a clean sentence.";
    char expected[] = "This is a clean sentence.";

    fossil_soap_sanitize(input);

    ASSUME_ITS_EQUAL_CSTR(expected, input);
}

FOSSIL_TEST_CASE(c_test_soap_is_offensive_empty) {
    ASSUME_ITS_FALSE(fossil_soap_is_offensive(""));
}

FOSSIL_TEST_CASE(c_test_soap_count_offensive_none) {
    char input[] = "This is a clean sentence.";
    ASSUME_ITS_EQUAL_I32(0, fossil_soap_count_offensive(input));
}

FOSSIL_TEST_CASE(c_test_soap_count_offensive_multiple) {
    char input[] = "curse1, curse2, and curse1 are all offensive.";
    ASSUME_ITS_EQUAL_I32(3, fossil_soap_count_offensive(input));
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_soap_tests) {
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_sanitize);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_is_offensive);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_count_offensive);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_sanitize_multiple);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_sanitize_no_offensive);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_is_offensive_empty);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_count_offensive_none);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_count_offensive_multiple);

    FOSSIL_TEST_REGISTER(c_soap_suite);
}
