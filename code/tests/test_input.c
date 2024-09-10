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


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(test_fossil_io_gets) {
    const char *input = "Hello, World!\n";
    FILE *mock_input = fmemopen((void *)input, strlen(input), "r");
    char buf[50];

    // Redirect stdin to the mock input
    stdin = mock_input;

    char *result = fossil_io_gets(buf, sizeof(buf));
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", buf);

    fclose(mock_input);
}

FOSSIL_TEST(test_fossil_io_gets_buffer_too_small) {
    const char *input = "Hello, World!\n";
    FILE *mock_input = fmemopen((void *)input, strlen(input), "r");
    char buf[5]; // Buffer smaller than the input

    // Redirect stdin to the mock input
    stdin = mock_input;

    char *result = fossil_io_gets(buf, sizeof(buf));
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hell", buf); // Only part of the input should fit

    fclose(mock_input);
}

FOSSIL_TEST(test_fossil_io_gets_empty_buffer) {
    char buf[1]; // Buffer size is too small to read anything

    char *result = fossil_io_gets(buf, sizeof(buf));
    ASSUME_ITS_CNULL(result); // Should be NULL as buffer size is insufficient
}

FOSSIL_TEST(test_fossil_io_gets_with_dialog) {
    const char *input = "Hello, Dialog!\n";
    FILE *mock_input = fmemopen((void *)input, strlen(input), "r");
    char buf[50];
    const char *dialog = "Please enter input: ";

    // Redirect stdin to the mock input
    stdin = mock_input;

    char *result = fossil_io_gets_with_dialog(buf, sizeof(buf), dialog);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello, Dialog!", buf);

    fclose(mock_input);
}

FOSSIL_TEST(test_fossil_io_gets_with_dialog_buffer_too_small) {
    const char *input = "Hello, Dialog!\n";
    FILE *mock_input = fmemopen((void *)input, strlen(input), "r");
    char buf[5]; // Buffer smaller than the input
    const char *dialog = "Please enter input: ";

    // Redirect stdin to the mock input
    stdin = mock_input;

    char *result = fossil_io_gets_with_dialog(buf, sizeof(buf), dialog);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hell", buf); // Only part of the input should fit

    fclose(mock_input);
}

FOSSIL_TEST(test_fossil_io_gets_with_dialog_empty_buffer) {
    const char *dialog = "Please enter input: ";
    char buf[1]; // Buffer size is too small to read anything

    char *result = fossil_io_gets_with_dialog(buf, sizeof(buf), dialog);
    ASSUME_ITS_CNULL(result); // Should be NULL as buffer size is insufficient
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_input_tests) {
    ADD_TEST(test_fossil_io_gets);
    ADD_TEST(test_fossil_io_gets_buffer_too_small);
    ADD_TEST(test_fossil_io_gets_empty_buffer);
    ADD_TEST(test_fossil_io_gets_with_dialog);
    ADD_TEST(test_fossil_io_gets_with_dialog_buffer_too_small);
    ADD_TEST(test_fossil_io_gets_with_dialog_empty_buffer);
}
