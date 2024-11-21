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
FOSSIL_TEST_SUITE(c_input_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_input_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_input_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_CASE(c_test_io_gets_empty_input) {
    const char *input = "\n";
    FILE *mock_input = c_create_mock_input(input);
    char buf[50];

    // Pass the mock input stream directly to the function
    char *result = fossil_io_gets_from_stream(buf, sizeof(buf), mock_input);
    ASSUME_NOT_CNULL(result);
    printf("buf: %s\n", buf);
    ASSUME_ITS_EQUAL_CSTR("", buf);

    fclose(mock_input);
}

FOSSIL_TEST_CASE(c_test_io_gets_null_buffer) {
    const char *input = "Hello, World!\n";
    FILE *mock_input = c_create_mock_input(input);

    // Pass a NULL buffer to the function
    char *result = fossil_io_gets_from_stream(NULL, 50, mock_input);
    ASSUME_IS_NULL(result);

    fclose(mock_input);
}

FOSSIL_TEST_CASE(c_test_io_gets_null_stream) {
    char buf[50];

    // Pass a NULL stream to the function
    char *result = fossil_io_gets_from_stream(buf, sizeof(buf), NULL);
    ASSUME_IS_NULL(result);
}

FOSSIL_TEST_CASE(c_test_io_gets_with_dialog_null_dialog) {
    const char *input = "Hello, World!\n";
    FILE *mock_input = c_create_mock_input(input);
    char buf[50];

    // Pass a NULL dialog to the function
    char *result = fossil_io_gets_with_dialog(buf, sizeof(buf), NULL);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", buf);

    fclose(mock_input);
}

FOSSIL_TEST_CASE(c_test_io_gets_with_dialog_long_input) {
    const char *input = "This is a very long input string that exceeds the buffer size.\n";
    FILE *mock_input = c_create_mock_input(input);
    char buf[20];
    const char *dialog = "Please enter input: ";

    // Simulate dialog display and read input from the mock stream
    printf("%s", dialog);
    char *result = fossil_io_gets_with_dialog(buf, sizeof(buf), dialog);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("This is a very lo", buf); // Only part of the input should fit

    fclose(mock_input);
}

FOSSIL_TEST_REGISTER(c_input_suite);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_with_dialog_empty_buffer);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_empty_input);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_null_buffer);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_null_stream);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_with_dialog_null_dialog);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_with_dialog_long_input);

    FOSSIL_TEST_REGISTER(c_input_suite);
}
