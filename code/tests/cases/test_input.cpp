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
FOSSIL_TEST_SUITE(cpp_input_suite);

// Setup function for the test suite
FOSSIL_SETUP(cpp_input_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_input_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_CASE(cpp_test_io_gets_from_stream) {
    const char *input_data = "test input\n";
    FILE *input_stream = tmpfile();
    fwrite(input_data, 1, strlen(input_data), input_stream);
    rewind(input_stream);

    char buf[20];
    char *result = fossil_io_gets_from_stream(buf, sizeof(buf), input_stream);
    ASSUME_ITS_EQUAL_CSTR("test input", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream);
}

FOSSIL_TEST_CASE(cpp_test_io_gets_from_stream_no_offensive) {
    char input[] = "This is a clean sentence.\n";
    char expected[] = "This is a clean sentence.";
    char buffer[256];

    FILE *stream = tmpfile();
    fwrite(input, 1, strlen(input), stream);
    rewind(stream);
    char *result = fossil_io_gets_from_stream(buffer, sizeof(buffer), stream);
    fclose(stream);

    ASSUME_ITS_EQUAL_CSTR(expected, result);
}

FOSSIL_TEST_CASE(cpp_test_io_gets_from_stream_with_punctuation) {
    char input[] = "This is a test with curse1, and racist_phrase1!\n";
    char expected[] = "This is a test with ***, and ***!";
    char buffer[256];

    FILE *stream = tmpfile();
    fwrite(input, 1, strlen(input), stream);
    rewind(stream);
    char *result = fossil_io_gets_from_stream(buffer, sizeof(buffer), stream);
    fclose(stream);

    ASSUME_ITS_EQUAL_CSTR(expected, result);
}

FOSSIL_TEST_CASE(cpp_test_io_gets_from_stream_empty_input) {
    const char *input_data = "\n";
    FILE *input_stream = tmpfile();
    fwrite(input_data, 1, strlen(input_data), input_stream);
    rewind(input_stream);

    char buf[20];
    char *result = fossil_io_gets_from_stream(buf, sizeof(buf), input_stream);
    ASSUME_ITS_EQUAL_CSTR("", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream);
}

FOSSIL_TEST_CASE(cpp_test_io_gets_from_stream_only_whitespace) {
    const char *input_data = "   \n";
    FILE *input_stream = tmpfile();
    fwrite(input_data, 1, strlen(input_data), input_stream);
    rewind(input_stream);

    char buf[20];
    char *result = fossil_io_gets_from_stream(buf, sizeof(buf), input_stream);
    ASSUME_ITS_EQUAL_CSTR("", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream);
}

FOSSIL_TEST_CASE(cpp_test_io_gets_from_stream_long_input) {
    const char *input_data = "This is a very long input string that exceeds the buffer size\n";
    FILE *input_stream = tmpfile();
    fwrite(input_data, 1, strlen(input_data), input_stream);
    rewind(input_stream);

    char buf[20];
    char *result = fossil_io_gets_from_stream(buf, sizeof(buf), input_stream);
    ASSUME_ITS_EQUAL_CSTR("This is a very long", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream);
}

FOSSIL_TEST_CASE(cpp_test_io_gets_from_stream_ex) {
    const char *input_data = "test input\n";
    FILE *input_stream = tmpfile();
    fwrite(input_data, 1, strlen(input_data), input_stream);
    rewind(input_stream);

    char buf[20];
    int error_code = 0;
    char *result = fossil_io_gets_from_stream_ex(buf, sizeof(buf), input_stream, &error_code);
    ASSUME_ITS_EQUAL_CSTR("test input", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream);
}

FOSSIL_TEST_CASE(cpp_test_io_gets_utf8) {
    const char *input_data = "test input\n";
    FILE *input_stream = tmpfile();
    fwrite(input_data, 1, strlen(input_data), input_stream);
    rewind(input_stream);

    char buf[20];
    char *result = fossil_io_gets_utf8(buf, sizeof(buf), input_stream);
    ASSUME_ITS_EQUAL_CSTR("test input", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream);
}

FOSSIL_TEST_CASE(cpp_test_io_gets_from_stream_class) {
    const char *input_data = "test input\n";
    FILE *input_stream = tmpfile();
    fwrite(input_data, 1, strlen(input_data), input_stream);
    rewind(input_stream);

    char buf[20];
    char *result = fossil::io::Input::gets_from_stream(buf, sizeof(buf), input_stream);
    ASSUME_ITS_EQUAL_CSTR("test input", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream);
}

FOSSIL_TEST_CASE(cpp_test_io_gets_from_stream_ex_class) {
    const char *input_data = "test input\n";
    FILE *input_stream = tmpfile();
    fwrite(input_data, 1, strlen(input_data), input_stream);
    rewind(input_stream);

    char buf[20];
    int error_code = 0;
    char *result = fossil::io::Input::gets_from_stream_ex(buf, sizeof(buf), input_stream, &error_code);
    ASSUME_ITS_EQUAL_CSTR("test input", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream);
}

FOSSIL_TEST_CASE(cpp_test_io_validate_input_buffer_class) {
    const char *buf = "test buffer";
    size_t size = strlen(buf);
    int result = fossil::io::Input::validate_input_buffer(buf, size);
    ASSUME_ITS_EQUAL_I32(1, result);
}

FOSSIL_TEST_CASE(cpp_test_io_gets_utf8_class) {
    const char *input_data = "test input\n";
    FILE *input_stream = tmpfile();
    fwrite(input_data, 1, strlen(input_data), input_stream);
    rewind(input_stream);

    char buf[20];
    char *result = fossil::io::Input::gets_utf8(buf, sizeof(buf), input_stream);
    ASSUME_ITS_EQUAL_CSTR("test input", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream);
}

FOSSIL_TEST_CASE(cpp_test_io_validate_is_int_valid) {
    const char *input = "12345";
    int output;
    int result = fossil_io_validate_is_int(input, &output);
    ASSUME_ITS_TRUE(result);
    ASSUME_ITS_EQUAL_I32(12345, output);
}

FOSSIL_TEST_CASE(cpp_test_io_validate_is_int_invalid) {
    const char *input = "123abc";
    int output;
    int result = fossil_io_validate_is_int(input, &output);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST_CASE(cpp_test_io_validate_is_float_valid) {
    const char *input = "123.45";
    float output;
    int result = fossil_io_validate_is_float(input, &output);
    ASSUME_ITS_TRUE(result);
    ASSUME_ITS_EQUAL_F32(123.45, output, 0.01);
}

FOSSIL_TEST_CASE(cpp_test_io_validate_is_float_invalid) {
    const char *input = "123.abc";
    float output;
    int result = fossil_io_validate_is_float(input, &output);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST_CASE(cpp_test_io_validate_is_alnum_valid) {
    const char *input = "abc123";
    int result = fossil_io_validate_is_alnum(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST_CASE(cpp_test_io_validate_is_alnum_invalid) {
    const char *input = "abc 123";
    int result = fossil_io_validate_is_alnum(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST_CASE(cpp_test_io_validate_is_email_valid) {
    const char *input = "test@example.com";
    int result = fossil_io_validate_is_email(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST_CASE(cpp_test_io_validate_is_email_invalid) {
    const char *input = "test@com";
    int result = fossil_io_validate_is_email(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST_CASE(cpp_test_io_validate_is_length_valid) {
    const char *input = "short";
    int result = fossil_io_validate_is_length(input, 10);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST_CASE(cpp_test_io_validate_is_length_invalid) {
    const char *input = "this is a very long string";
    int result = fossil_io_validate_is_length(input, 10);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST_CASE(cpp_test_io_validate_sanitize_string) {
    const char *input = "This is a test with curse1, and racist_phrase1!";
    char expected[] = "This is a test with ***, and ***!";
    char output[256];
    int result = fossil_io_validate_sanitize_string(input, output, sizeof(output));
    ASSUME_ITS_TRUE(result);
    ASSUME_ITS_EQUAL_CSTR(expected, output);
}

FOSSIL_TEST_CASE(cpp_test_io_validate_read_secure_line) {
    const char *input_data = "secure input\n";
    FILE *input_stream = tmpfile();
    fwrite(input_data, 1, strlen(input_data), input_stream);
    rewind(input_stream);

    char buffer[20];
    int result = fossil_io_validate_read_secure_line(buffer, sizeof(buffer));
    ASSUME_ITS_TRUE(result);
    ASSUME_ITS_EQUAL_CSTR("secure input", buffer);
    fclose(input_stream);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_input_tests) {
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_gets_from_stream);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_gets_from_stream_no_offensive);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_gets_from_stream_with_punctuation);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_gets_from_stream_empty_input);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_gets_from_stream_only_whitespace);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_gets_from_stream_long_input);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_gets_from_stream_ex);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_gets_utf8);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_gets_from_stream_class);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_gets_from_stream_ex_class);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_validate_input_buffer_class);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_gets_utf8_class);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_validate_is_int_valid);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_validate_is_int_invalid);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_validate_is_float_valid);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_validate_is_float_invalid);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_validate_is_alnum_valid);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_validate_is_alnum_invalid);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_validate_is_email_valid);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_validate_is_email_invalid);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_validate_is_length_valid);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_validate_is_length_invalid);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_validate_sanitize_string);
    FOSSIL_TEST_ADD(cpp_input_suite, cpp_test_io_validate_read_secure_line);

    FOSSIL_TEST_REGISTER(cpp_input_suite);
}
