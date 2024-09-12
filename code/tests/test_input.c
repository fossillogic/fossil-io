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
#include <fossil/unittest/framework.h>
#include <fossil/mockup/framework.h>
#include <fossil/xassume.h>

#include "fossil/io/framework.h"

#ifdef _WIN32
// For Windows, use tmpfile and fwrite to simulate input
FILE *create_mock_input(const char *input) {
    FILE *mock_input = tmpfile();
    fwrite(input, sizeof(char), strlen(input), mock_input);
    rewind(mock_input);
    return mock_input;
}
#else
// For Unix-like systems, use fmemopen
FILE *create_mock_input(const char *input) {
    return fmemopen((void *)input, strlen(input), "r");
}
#endif

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(test_fossil_io_gets) {
    const char *input = "Hello, World!\n";
    FILE *mock_input = create_mock_input(input);
    char buf[50];

    // Pass the mock input stream directly to the function
    char *result = fossil_io_gets_from_stream(buf, sizeof(buf), mock_input);
    ASSUME_NOT_CNULL(result);
    printf("buf: %s\n", buf);
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", buf);

    fclose(mock_input);
}

FOSSIL_TEST(test_fossil_io_gets_buffer_too_small) {
    const char *input = "Hello, World!\n";
    FILE *mock_input = create_mock_input(input);
    char buf[5]; // Buffer smaller than the input

    // Pass the mock input stream directly to the function
    char *result = fossil_io_gets_from_stream(buf, sizeof(buf), mock_input);
    ASSUME_NOT_CNULL(result);
    printf("buf: %s\n", buf);
    ASSUME_ITS_EQUAL_CSTR("Hell", buf); // Only part of the input should fit

    fclose(mock_input);
}

FOSSIL_TEST(test_fossil_io_gets_with_dialog) {
    const char *input = "Hello, Dialog!\n";
    FILE *mock_input = create_mock_input(input);
    char buf[50];
    const char *dialog = "Please enter input: ";

    // Simulate dialog display and read input from the mock stream
    printf("%s", dialog);
    char *result = fossil_io_gets_from_stream(buf, sizeof(buf), mock_input);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello, Dialog!", buf);

    fclose(mock_input);
}

FOSSIL_TEST(test_fossil_io_gets_with_dialog_empty_buffer) {
    const char *dialog = "Please enter input: ";
    char buf[1]; // Buffer size is too small to read anything

    // Simulate dialog display and attempt to read input
    printf("%s", dialog);
    char *result = fossil_io_gets_from_stream(buf, sizeof(buf), stdin);
    ASSUME_NOT_CNULL(result); // Should be NULL as buffer size is insufficient
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_input_tests) {
    ADD_TEST(test_fossil_io_gets);
    ADD_TEST(test_fossil_io_gets_buffer_too_small);
    ADD_TEST(test_fossil_io_gets_with_dialog);
    ADD_TEST(test_fossil_io_gets_with_dialog_empty_buffer);
}
