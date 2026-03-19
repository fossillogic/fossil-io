/**
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * This file is part of the Fossil Logic project, which aims to develop
 * high-performance, cross-platform applications and libraries. The code
 * contained herein is licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * Author: Michael Gene Brockus (Dreamer)
 * Date: 04/05/2014
 *
 * Copyright (C) 2014-2025 Fossil Logic. All rights reserved.
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
FOSSIL_SUITE(c_input_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_input_suite)
{
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_input_suite)
{
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Input validation and sanitization tests

FOSSIL_TEST(c_test_validate_is_int_valid)
{
    int output;
    int result = fossil_io_validate_is_int("42", &output);
    ASSUME_ITS_TRUE(result == 1);
    ASSUME_ITS_EQUAL_I32(output, 42);
}

FOSSIL_TEST(c_test_validate_is_int_invalid)
{
    int output;
    int result = fossil_io_validate_is_int("not_a_number", &output);
    ASSUME_ITS_TRUE(result == 0);
}

FOSSIL_TEST(c_test_validate_is_float_valid)
{
    float output;
    int result = fossil_io_validate_is_float("3.14", &output);
    ASSUME_ITS_TRUE(result == 1);
    ASSUME_ITS_TRUE(output > 3.13 && output < 3.15);
}

FOSSIL_TEST(c_test_validate_is_float_invalid)
{
    float output;
    int result = fossil_io_validate_is_float("abc", &output);
    ASSUME_ITS_TRUE(result == 0);
}

FOSSIL_TEST(c_test_validate_is_alnum_valid)
{
    int result = fossil_io_validate_is_alnum("abc123XYZ");
    ASSUME_ITS_TRUE(result == 1);
}

FOSSIL_TEST(c_test_validate_is_alnum_invalid)
{
    int result = fossil_io_validate_is_alnum("abc-123");
    ASSUME_ITS_TRUE(result == 0);
}

FOSSIL_TEST(c_test_validate_is_email_valid)
{
    int result = fossil_io_validate_is_email("user@gmail.com");
    ASSUME_ITS_TRUE(result == 1);
}

FOSSIL_TEST(c_test_validate_is_email_invalid)
{
    int result = fossil_io_validate_is_email("notanemail");
    ASSUME_ITS_TRUE(result == 0);
}

FOSSIL_TEST(c_test_validate_is_length_valid)
{
    int result = fossil_io_validate_is_length("hello", 10);
    ASSUME_ITS_TRUE(result == 1);
}

FOSSIL_TEST(c_test_validate_is_length_invalid)
{
    int result = fossil_io_validate_is_length("hello", 3);
    ASSUME_ITS_TRUE(result == 0);
}

FOSSIL_TEST(c_test_validate_is_weak_password_too_short)
{
    int result = fossil_io_validate_is_weak_password("short", NULL, NULL);
    ASSUME_ITS_TRUE(result == 1);
}

FOSSIL_TEST(c_test_validate_is_weak_password_valid)
{
    int result = fossil_io_validate_is_weak_password("SecurePass123!", NULL, NULL);
    ASSUME_ITS_TRUE(result == 0);
}

FOSSIL_TEST(c_test_validate_is_weak_password_common)
{
    int result = fossil_io_validate_is_weak_password("password", NULL, NULL);
    ASSUME_ITS_TRUE(result == 1);
}

FOSSIL_TEST(c_test_validate_is_disposable_email_valid)
{
    int result = fossil_io_validate_is_disposable_email("user@mailinator.com");
    ASSUME_ITS_TRUE(result == 1);
}

FOSSIL_TEST(c_test_validate_is_disposable_email_invalid)
{
    int result = fossil_io_validate_is_disposable_email("user@gmail.com");
    ASSUME_ITS_TRUE(result == 0);
}

FOSSIL_TEST(c_test_validate_is_suspicious_bot_valid)
{
    int result = fossil_io_validate_is_suspicious_bot("Mozilla/5.0 (bot) crawler");
    ASSUME_ITS_TRUE(result == 1);
}

FOSSIL_TEST(c_test_validate_is_suspicious_bot_invalid)
{
    int result = fossil_io_validate_is_suspicious_bot("Mozilla/5.0 (Windows NT 10.0)");
    ASSUME_ITS_TRUE(result == 0);
}

FOSSIL_TEST(c_test_validate_is_suspicious_user_valid)
{
    int result = fossil_io_validate_is_suspicious_user("bot12345");
    ASSUME_ITS_TRUE(result == 1);
}

FOSSIL_TEST(c_test_validate_is_suspicious_user_invalid)
{
    int result = fossil_io_validate_is_suspicious_user("john_doe");
    ASSUME_ITS_TRUE(result == 0);
}

FOSSIL_TEST(c_test_sanitize_string_generic_context)
{
    char output[256];
    int result = fossil_io_validate_sanitize_string("hello@world", output, sizeof(output), FOSSIL_CTX_GENERIC);
    ASSUME_ITS_TRUE(result == FOSSIL_SAN_OK);
    ASSUME_ITS_EQUAL_CSTR(output, "hello@world");
}

FOSSIL_TEST(c_test_sanitize_string_script_detection)
{
    char output[256];
    int result = fossil_io_validate_sanitize_string("<script>alert('xss')</script>", output, sizeof(output), FOSSIL_CTX_HTML);
    ASSUME_ITS_TRUE((result & FOSSIL_SAN_SCRIPT) != 0);
}

FOSSIL_TEST(c_test_sanitize_string_sql_detection)
{
    char output[256];
    int result = fossil_io_validate_sanitize_string("admin' DROP TABLE users--", output, sizeof(output), FOSSIL_CTX_SQL);
    ASSUME_ITS_TRUE((result & FOSSIL_SAN_SQL) != 0);
}

FOSSIL_TEST(c_test_sanitize_string_shell_detection)
{
    char output[256];
    int result = fossil_io_validate_sanitize_string("rm -rf /", output, sizeof(output), FOSSIL_CTX_SHELL);
    ASSUME_ITS_TRUE((result & FOSSIL_SAN_SHELL) != 0);
}

FOSSIL_TEST(c_test_sanitize_string_path_traversal)
{
    char output[256];
    int result = fossil_io_validate_sanitize_string("../../../etc/passwd", output, sizeof(output), FOSSIL_CTX_FILENAME);
    ASSUME_ITS_TRUE((result & FOSSIL_SAN_PATH) != 0);
}

FOSSIL_TEST(c_test_sanitize_string_bot_detection)
{
    char output[256];
    int result = fossil_io_validate_sanitize_string("python-requests library", output, sizeof(output), FOSSIL_CTX_GENERIC);
    ASSUME_ITS_TRUE((result & FOSSIL_SAN_BOT) != 0);
}

FOSSIL_TEST(c_test_sanitize_string_html_context)
{
    char output[256];
    int result = fossil_io_validate_sanitize_string("hello@world!", output, sizeof(output), FOSSIL_CTX_HTML);
    ASSUME_ITS_TRUE(result != FOSSIL_SAN_OK);
}

FOSSIL_TEST(c_test_trim_string)
{
    char str[] = "   hello world   \n";
    fossil_io_trim(str);
    ASSUME_ITS_EQUAL_CSTR(str, "hello world");
}

FOSSIL_TEST(c_test_trim_empty_string)
{
    char str[] = "   \t\n";
    fossil_io_trim(str);
    ASSUME_ITS_EQUAL_CSTR(str, "");
}

FOSSIL_TEST(c_test_validate_input_buffer_valid)
{
    char buf[256] = "test input";
    int result = fossil_io_validate_input_buffer(buf, sizeof(buf));
    ASSUME_ITS_TRUE(result == 1);
}

FOSSIL_TEST(c_test_validate_input_buffer_invalid)
{
    int result = fossil_io_validate_input_buffer(NULL, 0);
    ASSUME_ITS_TRUE(result == 0);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_input_tests)
{
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_is_int_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_is_int_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_is_float_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_is_float_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_is_alnum_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_is_alnum_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_is_email_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_is_email_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_is_length_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_is_length_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_is_weak_password_too_short);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_is_weak_password_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_is_weak_password_common);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_is_disposable_email_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_is_disposable_email_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_is_suspicious_bot_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_is_suspicious_bot_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_is_suspicious_user_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_is_suspicious_user_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_sanitize_string_generic_context);
    FOSSIL_TEST_ADD(c_input_suite, c_test_sanitize_string_script_detection);
    FOSSIL_TEST_ADD(c_input_suite, c_test_sanitize_string_sql_detection);
    FOSSIL_TEST_ADD(c_input_suite, c_test_sanitize_string_shell_detection);
    FOSSIL_TEST_ADD(c_input_suite, c_test_sanitize_string_path_traversal);
    FOSSIL_TEST_ADD(c_input_suite, c_test_sanitize_string_bot_detection);
    FOSSIL_TEST_ADD(c_input_suite, c_test_sanitize_string_html_context);
    FOSSIL_TEST_ADD(c_input_suite, c_test_trim_string);
    FOSSIL_TEST_ADD(c_input_suite, c_test_trim_empty_string);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_input_buffer_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_validate_input_buffer_invalid);

    FOSSIL_TEST_REGISTER(c_input_suite);
}
