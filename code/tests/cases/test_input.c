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

FOSSIL_TEST(c_test_io_trim_leading_and_trailing_spaces) {
    char s[] = "   hello world   ";
    fossil_io_trim(s);
    ASSUME_ITS_EQUAL_CSTR("hello world", s);
}

FOSSIL_TEST(c_test_io_trim_leading_tabs_and_newlines) {
    char s[] = "\t\n\r  hello world\n\t\r";
    fossil_io_trim(s);
    ASSUME_ITS_EQUAL_CSTR("hello world", s);
}

FOSSIL_TEST(c_test_io_trim_only_leading_whitespace) {
    char s[] = "   hello";
    fossil_io_trim(s);
    ASSUME_ITS_EQUAL_CSTR("hello", s);
}

FOSSIL_TEST(c_test_io_trim_only_trailing_whitespace) {
    char s[] = "hello   \n";
    fossil_io_trim(s);
    ASSUME_ITS_EQUAL_CSTR("hello", s);
}

FOSSIL_TEST(c_test_io_trim_no_whitespace) {
    char s[] = "helloworld";
    fossil_io_trim(s);
    ASSUME_ITS_EQUAL_CSTR("helloworld", s);
}

FOSSIL_TEST(c_test_io_trim_all_whitespace) {
    char s[] = "   \t\n\r  ";
    fossil_io_trim(s);
    ASSUME_ITS_EQUAL_CSTR("", s);
}

FOSSIL_TEST(c_test_io_trim_empty_string) {
    char s[] = "";
    fossil_io_trim(s);
    ASSUME_ITS_EQUAL_CSTR("", s);
}

FOSSIL_TEST(c_test_io_trim_whitespace_only_newline) {
    char s[] = "\n";
    fossil_io_trim(s);
    ASSUME_ITS_EQUAL_CSTR("", s);
}

FOSSIL_TEST(c_test_io_trim_whitespace_middle_preserved) {
    char s[] = "  hello   world  ";
    fossil_io_trim(s);
    ASSUME_ITS_EQUAL_CSTR("hello   world", s);
}

FOSSIL_TEST(c_test_io_gets_from_stream) {
    const char *input_data = "test input\n";
    fossil_io_file_t input_stream = {tmpfile(), "tempfile"};
    fwrite(input_data, 1, strlen(input_data), input_stream.file);
    rewind(input_stream.file);

    char buf[20];
    char *result = fossil_io_gets_from_stream(buf, sizeof(buf), &input_stream);
    ASSUME_ITS_EQUAL_CSTR("test input", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream.file);
}

FOSSIL_TEST(c_test_io_gets_from_stream_no_offensive) {
    char input[] = "This is a clean sentence.\n";
    char expected[] = "This is a clean sentence.";
    char buffer[256];

    fossil_io_file_t stream = {tmpfile(), "tempfile"};
    fwrite(input, 1, strlen(input), stream.file);
    rewind(stream.file);
    char *result = fossil_io_gets_from_stream(buffer, sizeof(buffer), &stream);
    fclose(stream.file);

    ASSUME_ITS_EQUAL_CSTR(expected, result);
}

FOSSIL_TEST(c_test_io_gets_from_stream_with_punctuation) {
    char input[] = "This is a test with punctuation, and special characters!\n";
    char expected[] = "This is a test with punctuation, and special characters!";
    char buffer[256];

    fossil_io_file_t stream = {tmpfile(), "tempfile"};
    fwrite(input, 1, strlen(input), stream.file);
    rewind(stream.file);
    char *result = fossil_io_gets_from_stream(buffer, sizeof(buffer), &stream);
    fclose(stream.file);

    ASSUME_ITS_EQUAL_CSTR(expected, result);
}

FOSSIL_TEST(c_test_io_gets_from_stream_empty_input) {
    const char *input_data = "\n";
    fossil_io_file_t input_stream = {tmpfile(), "tempfile"};
    fwrite(input_data, 1, strlen(input_data), input_stream.file);
    rewind(input_stream.file);

    char buf[20];
    char *result = fossil_io_gets_from_stream(buf, sizeof(buf), &input_stream);
    ASSUME_ITS_EQUAL_CSTR("", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream.file);
}

FOSSIL_TEST(c_test_io_gets_from_stream_only_whitespace) {
    const char *input_data = "   \n";
    fossil_io_file_t input_stream = {tmpfile(), "tempfile"};
    fwrite(input_data, 1, strlen(input_data), input_stream.file);
    rewind(input_stream.file);

    char buf[20];
    char *result = fossil_io_gets_from_stream(buf, sizeof(buf), &input_stream);
    ASSUME_ITS_EQUAL_CSTR("", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream.file);
}

FOSSIL_TEST(c_test_io_gets_from_stream_long_input) {
    const char *input_data = "This is a very long input string that exceeds the buffer size\n";
    fossil_io_file_t input_stream = {tmpfile(), "tempfile"};
    fwrite(input_data, 1, strlen(input_data), input_stream.file);
    rewind(input_stream.file);

    char buf[20];
    char *result = fossil_io_gets_from_stream(buf, sizeof(buf), &input_stream);
    ASSUME_ITS_EQUAL_CSTR("This is a very long", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream.file);
}

FOSSIL_TEST(c_test_io_gets_from_stream_ex) {
    const char *input_data = "test input\n";
    fossil_io_file_t input_stream = {tmpfile(), "tempfile"};
    fwrite(input_data, 1, strlen(input_data), input_stream.file);
    rewind(input_stream.file);

    char buf[20];
    int error_code = 0;
    char *result = fossil_io_gets_from_stream_ex(buf, sizeof(buf), &input_stream, &error_code);
    ASSUME_ITS_EQUAL_CSTR("test input", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream.file);
}

FOSSIL_TEST(c_test_io_gets_utf8) {
    const char *input_data = "test input\n";
    fossil_io_file_t input_stream = {tmpfile(), "tempfile"};
    fwrite(input_data, 1, strlen(input_data), input_stream.file);
    rewind(input_stream.file);

    char buf[20];
    char *result = fossil_io_gets_utf8(buf, sizeof(buf), &input_stream);
    ASSUME_ITS_EQUAL_CSTR("test input", buf);
    ASSUME_NOT_CNULL(result);
    fclose(input_stream.file);
}

FOSSIL_TEST(c_test_io_validate_is_int_valid) {
    const char *input = "12345";
    int output;
    int result = fossil_io_validate_is_int(input, &output);
    ASSUME_ITS_TRUE(result);
    ASSUME_ITS_EQUAL_I32(12345, output);
}

FOSSIL_TEST(c_test_io_validate_is_int_invalid) {
    const char *input = "123abc";
    int output;
    int result = fossil_io_validate_is_int(input, &output);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(c_test_io_validate_is_float_invalid) {
    const char *input = "123.abc";
    float output;
    int result = fossil_io_validate_is_float(input, &output);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(c_test_io_validate_is_alnum_valid) {
    const char *input = "abc123";
    int result = fossil_io_validate_is_alnum(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(c_test_io_validate_is_alnum_invalid) {
    const char *input = "abc 123";
    int result = fossil_io_validate_is_alnum(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(c_test_io_validate_is_email_valid) {
    const char *input = "test@gmail.com"; // now checks for valid email providers like gmail.com
    int result = fossil_io_validate_is_email(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(c_test_io_validate_is_email_invalid) {
    const char *input = "test@com";
    int result = fossil_io_validate_is_email(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(c_test_io_validate_is_length_valid) {
    const char *input = "short";
    int result = fossil_io_validate_is_length(input, 10);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(c_test_io_validate_is_length_invalid) {
    const char *input = "this is a very long string";
    int result = fossil_io_validate_is_length(input, 10);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(c_test_io_getc) {
    const char *input_data = "test input\n";
    fossil_io_file_t input_stream = {tmpfile(), "tempfile"};
    fwrite(input_data, 1, strlen(input_data), input_stream.file);
    rewind(input_stream.file);

    int ch = fossil_io_getc(&input_stream);
    ASSUME_ITS_EQUAL_I32('t', ch);
    fclose(input_stream.file);
}

FOSSIL_TEST(c_test_io_register_keybinding_success) {
    fossil_io_clear_keybindings();
    int result = fossil_io_register_keybinding(42, "jump");
    ASSUME_ITS_EQUAL_I32(0, result);
    const char *action = fossil_io_get_keybinding_action(42);
    ASSUME_ITS_EQUAL_CSTR("jump", action);
    fossil_io_clear_keybindings();
}

FOSSIL_TEST(c_test_io_register_keybinding_duplicate) {
    fossil_io_clear_keybindings();
    fossil_io_register_keybinding(42, "jump");
    int result = fossil_io_register_keybinding(42, "run");
    ASSUME_ITS_EQUAL_I32(2, result); // Duplicate key
    fossil_io_clear_keybindings();
}

FOSSIL_TEST(c_test_io_process_keybinding_no_binding) {
    fossil_io_clear_keybindings();
    int result = fossil_io_process_keybinding(77);
    ASSUME_ITS_EQUAL_I32(0, result);
    fossil_io_clear_keybindings();
}

FOSSIL_TEST(c_test_io_list_keybindings_populates_array) {
    fossil_io_clear_keybindings();
    fossil_io_register_keybinding(1, "up");
    fossil_io_register_keybinding(2, "down");
    fossil_io_keybinding_t bindings[4];
    size_t count = fossil_io_list_keybindings(bindings, 4);
    ASSUME_ITS_EQUAL_I32(2, (int)count);
    ASSUME_ITS_EQUAL_I32(1, bindings[0].key_code);
    ASSUME_ITS_EQUAL_CSTR("up", bindings[0].action);
    ASSUME_ITS_EQUAL_I32(2, bindings[1].key_code);
    ASSUME_ITS_EQUAL_CSTR("down", bindings[1].action);
    fossil_io_clear_keybindings();
}

FOSSIL_TEST(c_test_io_clear_keybindings_removes_all) {
    fossil_io_register_keybinding(1, "up");
    fossil_io_register_keybinding(2, "down");
    fossil_io_clear_keybindings();
    fossil_io_keybinding_t bindings[2];
    size_t count = fossil_io_list_keybindings(bindings, 2);
    ASSUME_ITS_EQUAL_I32(0, (int)count);
}

FOSSIL_TEST(c_test_io_validate_is_weak_password_bad) {
    const char *password = "password123";
    const char *username = "user";
    const char *email = "user@example.com";
    int result = fossil_io_validate_is_weak_password(password, username, email);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(c_test_io_validate_is_weak_password_good) {
    const char *password = "S3cure!Passw0rd";
    const char *username = "user";
    const char *email = "user@example.com";
    int result = fossil_io_validate_is_weak_password(password, username, email);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(c_test_io_validate_is_suspicious_bot_true) {
    const char *ua = "Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)";
    int result = fossil_io_validate_is_suspicious_bot(ua);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(c_test_io_validate_is_suspicious_bot_false) {
    const char *ua = "Mozilla/5.0 (Windows NT 10.0; Win64; x64)";
    int result = fossil_io_validate_is_suspicious_bot(ua);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(c_test_io_validate_is_disposable_email_true) {
    const char *input = "user@mailinator.com";
    int result = fossil_io_validate_is_disposable_email(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(c_test_io_validate_is_disposable_email_false) {
    const char *input = "user@gmail.com";
    int result = fossil_io_validate_is_disposable_email(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(c_test_io_validate_is_suspicious_user_true) {
    const char *input = "bot123456";
    int result = fossil_io_validate_is_suspicious_user(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(c_test_io_validate_is_suspicious_user_false) {
    const char *input = "john_doe";
    int result = fossil_io_validate_is_suspicious_user(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(c_test_io_validate_sanitize_string_script) {
    const char *input = "<script>alert('xss')</script>";
    char output[64];
    int flags = fossil_io_validate_sanitize_string(input, output, sizeof(output), FOSSIL_CTX_HTML);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_SCRIPT);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_MODIFIED);
}

FOSSIL_TEST(c_test_io_validate_sanitize_string_sql) {
    const char *input = "SELECT * FROM users WHERE name='admin' --";
    char output[64];
    int flags = fossil_io_validate_sanitize_string(input, output, sizeof(output), FOSSIL_CTX_SQL);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_SQL);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_MODIFIED);
}

FOSSIL_TEST(c_test_io_validate_sanitize_string_clean) {
    const char *input = "SafeString123";
    char output[64];
    int flags = fossil_io_validate_sanitize_string(input, output, sizeof(output), FOSSIL_CTX_GENERIC);
    ASSUME_ITS_EQUAL_I32(FOSSIL_SAN_OK, flags);
    ASSUME_ITS_EQUAL_CSTR(input, output);
}

FOSSIL_TEST(c_test_io_validate_is_suspicious_user_many_digits) {
    const char *input = "user1234567890";
    int result = fossil_io_validate_is_suspicious_user(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(c_test_io_validate_is_suspicious_user_high_digit_ratio) {
    const char *input = "a1b2c3d4e5f6g7h8i9j0";
    int result = fossil_io_validate_is_suspicious_user(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(c_test_io_validate_is_suspicious_user_contains_test) {
    const char *input = "testuser";
    int result = fossil_io_validate_is_suspicious_user(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(c_test_io_validate_is_suspicious_user_contains_fake) {
    const char *input = "fakeaccount";
    int result = fossil_io_validate_is_suspicious_user(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(c_test_io_validate_is_suspicious_user_entropy) {
    const char *input = "x7q9z2w8v5r1t3y6u0";
    int result = fossil_io_validate_is_suspicious_user(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(c_test_io_validate_sanitize_string_script_case_variants) {
    const char *inputs[] = {
        "<SCRIPT>alert('xss')</SCRIPT>",
        "<ScRiPt>alert('xss')</ScRiPt>",
        "<script type='text/javascript'>alert('xss')</script>",
        "<script src='evil.js'></script>",
        "<script>alert('xss')",
        " <script >alert('xss')</script > ",
        "text<script>alert('xss')</script>text"
    };
    char output[128];
    for (size_t i = 0; i < sizeof(inputs)/sizeof(inputs[0]); i++) {
        int flags = fossil_io_validate_sanitize_string(inputs[i], output, sizeof(output), FOSSIL_CTX_HTML);
        ASSUME_ITS_TRUE(flags & FOSSIL_SAN_SCRIPT);
        ASSUME_ITS_TRUE(flags & FOSSIL_SAN_MODIFIED);
    }
}

FOSSIL_TEST(c_test_io_validate_sanitize_string_script_embedded) {
    const char *input = "normal text <script>alert('xss')</script> more text";
    char output[128];
    int flags = fossil_io_validate_sanitize_string(input, output, sizeof(output), FOSSIL_CTX_HTML);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_SCRIPT);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_MODIFIED);
}

FOSSIL_TEST(c_test_io_validate_sanitize_string_script_incomplete_tag) {
    const char *input = "<script>alert('xss')";
    char output[64];
    int flags = fossil_io_validate_sanitize_string(input, output, sizeof(output), FOSSIL_CTX_HTML);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_SCRIPT);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_MODIFIED);
}

FOSSIL_TEST(c_test_io_validate_sanitize_string_script_with_event_handler) {
    const char *input = "<div onclick=\"evil()\">Click me</div>";
    char output[64];
    int flags = fossil_io_validate_sanitize_string(input, output, sizeof(output), FOSSIL_CTX_HTML);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_SCRIPT);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_MODIFIED);
}

FOSSIL_TEST(c_test_io_validate_sanitize_string_script_javascript_url) {
    const char *input = "<a href=\"javascript:alert('xss')\">link</a>";
    char output[64];
    int flags = fossil_io_validate_sanitize_string(input, output, sizeof(output), FOSSIL_CTX_HTML);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_SCRIPT);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_MODIFIED);
}

FOSSIL_TEST(c_test_io_validate_sanitize_string_sql_lowercase) {
    const char *input = "select * from users where name='admin';";
    char output[64];
    int flags = fossil_io_validate_sanitize_string(input, output, sizeof(output), FOSSIL_CTX_SQL);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_SQL);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_MODIFIED);
}

FOSSIL_TEST(c_test_io_validate_sanitize_string_sql_mixed_case) {
    const char *input = "SeLeCt * FrOm users WHERE name='admin' --";
    char output[64];
    int flags = fossil_io_validate_sanitize_string(input, output, sizeof(output), FOSSIL_CTX_SQL);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_SQL);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_MODIFIED);
}

FOSSIL_TEST(c_test_io_validate_sanitize_string_sql_union) {
    const char *input = "UNION SELECT password FROM users";
    char output[64];
    int flags = fossil_io_validate_sanitize_string(input, output, sizeof(output), FOSSIL_CTX_SQL);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_SQL);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_MODIFIED);
}

FOSSIL_TEST(c_test_io_validate_sanitize_string_sql_comment) {
    const char *input = "DROP TABLE users; --";
    char output[64];
    int flags = fossil_io_validate_sanitize_string(input, output, sizeof(output), FOSSIL_CTX_SQL);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_SQL);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_MODIFIED);
}

FOSSIL_TEST(c_test_io_validate_sanitize_string_sql_hex) {
    const char *input = "SELECT * FROM users WHERE id=0x1234";
    char output[64];
    int flags = fossil_io_validate_sanitize_string(input, output, sizeof(output), FOSSIL_CTX_SQL);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_SQL);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_MODIFIED);
}

FOSSIL_TEST(c_test_io_validate_sanitize_string_sql_no_keywords) {
    const char *input = "username123";
    char output[64];
    int flags = fossil_io_validate_sanitize_string(input, output, sizeof(output), FOSSIL_CTX_SQL);
    ASSUME_ITS_EQUAL_I32(FOSSIL_SAN_OK, flags);
    ASSUME_ITS_EQUAL_CSTR(input, output);
}

FOSSIL_TEST(c_test_io_validate_sanitize_string_sql_tricky_pattern) {
    const char *input = "SELECTnameFROMusers";
    char output[64];
    int flags = fossil_io_validate_sanitize_string(input, output, sizeof(output), FOSSIL_CTX_SQL);
    // Should not match since no space after SELECT
    ASSUME_ITS_EQUAL_I32(FOSSIL_SAN_OK, flags);
    ASSUME_ITS_EQUAL_CSTR(input, output);
}

FOSSIL_TEST(c_test_io_validate_sanitize_string_sql_multiple_keywords) {
    const char *input = "SELECT * FROM users; DROP TABLE users;";
    char output[64];
    int flags = fossil_io_validate_sanitize_string(input, output, sizeof(output), FOSSIL_CTX_SQL);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_SQL);
    ASSUME_ITS_TRUE(flags & FOSSIL_SAN_MODIFIED);
}

FOSSIL_TEST(c_test_io_validate_is_suspicious_bot_empty_string) {
    const char *ua = "";
    int result = fossil_io_validate_is_suspicious_bot(ua);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(c_test_io_validate_is_suspicious_bot_null) {
    const char *ua = NULL;
    int result = fossil_io_validate_is_suspicious_bot(ua);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(c_test_io_validate_is_suspicious_bot_partial_keyword) {
    const char *ua = "Mozilla/5.0 (compatible; botman/1.0)";
    int result = fossil_io_validate_is_suspicious_bot(ua);
    ASSUME_ITS_TRUE(result); // "bot" substring triggers detection
}

FOSSIL_TEST(c_test_io_validate_is_suspicious_bot_case_insensitive) {
    const char *ua = "Mozilla/5.0 (compatible; CRAWLER/1.0)";
    int result = fossil_io_validate_is_suspicious_bot(ua);
    ASSUME_ITS_TRUE(result); // "crawl" substring triggers detection
}

FOSSIL_TEST(c_test_io_validate_is_suspicious_bot_multiple_signatures) {
    const char *ua = "curl/7.68.0 python-requests/2.25.1";
    int result = fossil_io_validate_is_suspicious_bot(ua);
    ASSUME_ITS_TRUE(result); // both "curl" and "python-requests" trigger detection
}

FOSSIL_TEST(c_test_io_validate_is_suspicious_bot_legit_browser_with_bot_word) {
    const char *ua = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) Chrome/90.0 botnet";
    int result = fossil_io_validate_is_suspicious_bot(ua);
    ASSUME_ITS_TRUE(result); // "bot" substring triggers detection
}

FOSSIL_TEST(c_test_io_validate_is_suspicious_bot_non_bot_keywords) {
    const char *ua = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7)";
    int result = fossil_io_validate_is_suspicious_bot(ua);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(c_test_io_validate_is_weak_password_too_short) {
    const char *password = "abc12";
    const char *username = "user";
    const char *email = "user@example.com";
    int result = fossil_io_validate_is_weak_password(password, username, email);
    ASSUME_ITS_TRUE(result); // Too short
}

FOSSIL_TEST(c_test_io_validate_is_weak_password_too_long) {
    char password[80];
    memset(password, 'a', sizeof(password) - 1);
    password[sizeof(password) - 1] = '\0';
    const char *username = "user";
    const char *email = "user@example.com";
    int result = fossil_io_validate_is_weak_password(password, username, email);
    ASSUME_ITS_TRUE(result); // Too long
}

FOSSIL_TEST(c_test_io_validate_is_weak_password_low_diversity) {
    const char *password = "aaaaaaaa";
    const char *username = "user";
    const char *email = "user@example.com";
    int result = fossil_io_validate_is_weak_password(password, username, email);
    ASSUME_ITS_TRUE(result); // Not diverse
}

FOSSIL_TEST(c_test_io_validate_is_weak_password_common_pattern) {
    const char *password = "12345678";
    const char *username = "user";
    const char *email = "user@example.com";
    int result = fossil_io_validate_is_weak_password(password, username, email);
    ASSUME_ITS_TRUE(result); // Common weak password
}

FOSSIL_TEST(c_test_io_validate_is_weak_password_sequential_inc) {
    const char *password = "abcdefghi";
    const char *username = "user";
    const char *email = "user@example.com";
    int result = fossil_io_validate_is_weak_password(password, username, email);
    ASSUME_ITS_TRUE(result); // Sequential increasing
}

FOSSIL_TEST(c_test_io_validate_is_weak_password_sequential_dec) {
    const char *password = "987654321";
    const char *username = "user";
    const char *email = "user@example.com";
    int result = fossil_io_validate_is_weak_password(password, username, email);
    ASSUME_ITS_TRUE(result); // Sequential decreasing
}

FOSSIL_TEST(c_test_io_validate_is_weak_password_same_as_username) {
    const char *password = "user";
    const char *username = "user";
    const char *email = "user@example.com";
    int result = fossil_io_validate_is_weak_password(password, username, email);
    ASSUME_ITS_TRUE(result); // Same as username
}

FOSSIL_TEST(c_test_io_validate_is_weak_password_same_as_email) {
    const char *password = "user@example.com";
    const char *username = "user";
    const char *email = "user@example.com";
    int result = fossil_io_validate_is_weak_password(password, username, email);
    ASSUME_ITS_TRUE(result); // Same as email
}

FOSSIL_TEST(c_test_io_validate_is_weak_password_null_password) {
    const char *password = NULL;
    const char *username = "user";
    const char *email = "user@example.com";
    int result = fossil_io_validate_is_weak_password(password, username, email);
    ASSUME_ITS_TRUE(result); // NULL password
}

FOSSIL_TEST(c_test_io_validate_is_weak_password_minimum_valid) {
    const char *password = "Abc123!@";
    const char *username = "user";
    const char *email = "user@example.com";
    int result = fossil_io_validate_is_weak_password(password, username, email);
    ASSUME_ITS_FALSE(result); // Minimum valid, diverse
}

FOSSIL_TEST(c_test_io_validate_is_email_missing_at) {
    const char *input = "testgmail.com";
    int result = fossil_io_validate_is_email(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(c_test_io_validate_is_email_missing_domain_dot) {
    const char *input = "test@gmailcom";
    int result = fossil_io_validate_is_email(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(c_test_io_validate_is_email_empty_string) {
    const char *input = "";
    int result = fossil_io_validate_is_email(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(c_test_io_validate_is_email_null_input) {
    const char *input = NULL;
    int result = fossil_io_validate_is_email(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(c_test_io_validate_is_email_valid_yahoo) {
    const char *input = "user@yahoo.com";
    int result = fossil_io_validate_is_email(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(c_test_io_validate_is_email_valid_outlook) {
    const char *input = "person@outlook.com";
    int result = fossil_io_validate_is_email(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(c_test_io_validate_is_email_valid_hotmail) {
    const char *input = "someone@hotmail.com";
    int result = fossil_io_validate_is_email(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(c_test_io_validate_is_email_valid_icloud) {
    const char *input = "apple@icloud.com";
    int result = fossil_io_validate_is_email(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(c_test_io_validate_is_email_invalid_subdomain) {
    const char *input = "test@mail.gmail.com";
    int result = fossil_io_validate_is_email(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(c_test_io_validate_is_email_invalid_no_local_part) {
    const char *input = "@gmail.com";
    int result = fossil_io_validate_is_email(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(c_test_io_validate_is_email_invalid_no_domain) {
    const char *input = "test@";
    int result = fossil_io_validate_is_email(input);
    ASSUME_ITS_FALSE(result);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_input_tests) {
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_trim_leading_and_trailing_spaces);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_trim_leading_tabs_and_newlines);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_trim_only_leading_whitespace);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_trim_only_trailing_whitespace);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_trim_no_whitespace);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_trim_all_whitespace);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_trim_empty_string);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_trim_whitespace_only_newline);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_trim_whitespace_middle_preserved);

    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_from_stream);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_from_stream_no_offensive);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_from_stream_with_punctuation);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_from_stream_empty_input);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_from_stream_only_whitespace);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_from_stream_long_input);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_from_stream_ex);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_gets_utf8);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_int_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_int_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_float_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_alnum_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_alnum_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_email_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_email_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_length_valid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_length_invalid);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_getc);

    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_weak_password_bad);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_weak_password_good);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_suspicious_bot_true);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_suspicious_bot_false);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_disposable_email_true);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_disposable_email_false);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_suspicious_user_true);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_suspicious_user_false);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_sanitize_string_script);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_sanitize_string_sql);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_sanitize_string_clean);

    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_suspicious_user_many_digits);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_suspicious_user_high_digit_ratio);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_suspicious_user_contains_test);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_suspicious_user_contains_fake);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_suspicious_user_entropy);

    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_sanitize_string_script_case_variants);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_sanitize_string_script_embedded);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_sanitize_string_script_incomplete_tag);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_sanitize_string_script_with_event_handler);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_sanitize_string_script_javascript_url);

    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_sanitize_string_sql_lowercase);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_sanitize_string_sql_mixed_case);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_sanitize_string_sql_union);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_sanitize_string_sql_comment);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_sanitize_string_sql_hex);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_sanitize_string_sql_no_keywords);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_sanitize_string_sql_tricky_pattern);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_sanitize_string_sql_multiple_keywords);

    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_suspicious_bot_empty_string);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_suspicious_bot_null);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_suspicious_bot_partial_keyword);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_suspicious_bot_case_insensitive);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_suspicious_bot_multiple_signatures);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_suspicious_bot_legit_browser_with_bot_word);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_suspicious_bot_non_bot_keywords);

    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_weak_password_too_short);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_weak_password_too_long);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_weak_password_low_diversity);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_weak_password_common_pattern);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_weak_password_sequential_inc);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_weak_password_sequential_dec);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_weak_password_same_as_username);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_weak_password_same_as_email);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_weak_password_null_password);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_weak_password_minimum_valid);

    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_email_missing_at);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_email_missing_domain_dot);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_email_empty_string);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_email_null_input);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_email_valid_yahoo);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_email_valid_outlook);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_email_valid_hotmail);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_email_valid_icloud);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_email_invalid_subdomain);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_email_invalid_no_local_part);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_validate_is_email_invalid_no_domain);

    FOSSIL_TEST_ADD(c_input_suite, c_test_io_register_keybinding_success);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_register_keybinding_duplicate);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_process_keybinding_no_binding);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_list_keybindings_populates_array);
    FOSSIL_TEST_ADD(c_input_suite, c_test_io_clear_keybindings_removes_all);
    
    FOSSIL_TEST_REGISTER(c_input_suite);
}
