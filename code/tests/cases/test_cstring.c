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
FOSSIL_SUITE(c_string_suite);
fossil_fstream_t c_string;

// Setup function for the test suite
FOSSIL_SETUP(c_string_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_string_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_cstring_create_and_free) {
    const char *init = "Hello, World!";
    cstring str = fossil_io_cstring_create(init);
    ASSUME_NOT_CNULL(str);
    ASSUME_ITS_EQUAL_CSTR(init, str);
    fossil_io_cstring_free(str);
}

FOSSIL_TEST(c_test_cstring_copy) {
    const char *init = "Hello, World!";
    cstring str = fossil_io_cstring_create(init);
    cstring copy = fossil_io_cstring_copy(str);
    ASSUME_NOT_CNULL(copy);
    ASSUME_ITS_EQUAL_CSTR(init, copy);
    fossil_io_cstring_free(str);
    fossil_io_cstring_free(copy);
}

FOSSIL_TEST(c_test_cstring_concat) {
    const char *s1 = "Hello, ";
    const char *s2 = "World!";
    cstring result = fossil_io_cstring_concat(s1, s2);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(c_test_cstring_length) {
    const char *str = "Hello, World!";
    size_t length = fossil_io_cstring_length(str);
    ASSUME_ITS_EQUAL_SIZE(strlen(str), length);
}

FOSSIL_TEST(c_test_cstring_compare) {
    const char *s1 = "Hello";
    const char *s2 = "Hello";
    const char *s3 = "World";
    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_compare(s1, s2));
    ASSUME_ITS_TRUE(fossil_io_cstring_compare(s1, s3) < 0);
    ASSUME_ITS_TRUE(fossil_io_cstring_compare(s3, s1) > 0);
}

FOSSIL_TEST(c_test_cstring_trim) {
    cstring str = fossil_io_cstring_create("   Hello, World!   ");
    fossil_io_cstring_trim(str);
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", str);
    fossil_io_cstring_free(str);
}

FOSSIL_TEST(c_test_cstring_split) {
    const char *str = "Hello,World,Test";
    size_t count;
    cstring *result = fossil_io_cstring_split(str, ',', &count);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_SIZE(3, count);
    ASSUME_ITS_EQUAL_CSTR("Hello", result[0]);
    ASSUME_ITS_EQUAL_CSTR("World", result[1]);
    ASSUME_ITS_EQUAL_CSTR("Test", result[2]);
    for (size_t i = 0; i < count; i++) {
        fossil_io_cstring_free(result[i]);
    }
    free(result);
}

FOSSIL_TEST(c_test_cstring_replace) {
    const char *str = "Hello, World!";
    const char *old = "World";
    const char *new_str = "Fossil";
    cstring result = fossil_io_cstring_replace(str, old, new_str);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello, Fossil!", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(c_test_cstring_to_upper) {
    cstring str = fossil_io_cstring_create("Hello, World!");
    cstring result = fossil_io_cstring_to_upper(str);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("HELLO, WORLD!", result);
    fossil_io_cstring_free(str);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(c_test_cstring_to_lower) {
    cstring str = fossil_io_cstring_create("Hello, World!");
    cstring result = fossil_io_cstring_to_lower(str);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("hello, world!", result);
    fossil_io_cstring_free(str);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(c_test_cstring_starts_with) {
    const char *str = "Hello, World!";
    const char *prefix = "Hello";
    ASSUME_ITS_TRUE(fossil_io_cstring_starts_with(str, prefix));
}

FOSSIL_TEST(c_test_cstring_ends_with) {
    const char *str = "Hello, World!";
    const char *suffix = "World!";
    ASSUME_ITS_TRUE(fossil_io_cstring_ends_with(str, suffix));
}

FOSSIL_TEST(c_test_cstring_substring) {
    const char *str = "Hello, World!";
    cstring result = fossil_io_cstring_substring(str, 7, 5);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("World", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(c_test_cstring_reverse) {
    cstring str = fossil_io_cstring_create("Hello, World!");
    cstring result = fossil_io_cstring_reverse(str);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("!dlroW ,olleH", result);
    fossil_io_cstring_free(str);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(c_test_cstring_contains) {
    const char *str = "Hello, World!";
    const char *substr = "World";
    ASSUME_ITS_TRUE(fossil_io_cstring_contains(str, substr));
}

FOSSIL_TEST(c_test_cstring_repeat) {
    const char *str = "Hello";
    cstring result = fossil_io_cstring_repeat(str, 3);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("HelloHelloHello", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(c_test_cstring_strip) {
    const char *str = "!!!Hello, World!!!";
    cstring result = fossil_io_cstring_strip(str, '!');
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello, World", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(c_test_cstring_count) {
    const char *str = "Hello, World! Hello, Fossil!";
    const char *substr = "Hello";
    size_t count = fossil_io_cstring_count(str, substr);
    ASSUME_ITS_EQUAL_SIZE(2, count);
}

FOSSIL_TEST(c_test_cstring_pad_left) {
    const char *str = "Hello";
    cstring result = fossil_io_cstring_pad_left(str, 10, '*');
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("*****Hello", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(c_test_cstring_pad_right) {
    const char *str = "Hello";
    cstring result = fossil_io_cstring_pad_right(str, 10, '*');
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello*****", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(c_test_cstring_stream_create_and_free) {
    fossil_io_cstring_stream *stream = fossil_io_cstring_stream_create(1024);
    ASSUME_NOT_CNULL(stream);
    fossil_io_cstring_stream_free(stream);
}

FOSSIL_TEST(c_test_cstring_stream_write_and_read) {
    fossil_io_cstring_stream *stream = fossil_io_cstring_stream_create(1024);
    fossil_io_cstring_stream_write(stream, "Hello, World!");
    ccstring result = fossil_io_cstring_stream_read(stream);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", result);
    fossil_io_cstring_stream_free(stream);
}

FOSSIL_TEST(c_test_cstring_stream_multiple_writes) {
    fossil_io_cstring_stream *stream = fossil_io_cstring_stream_create(1024);
    fossil_io_cstring_stream_write(stream, "Hello, ");
    fossil_io_cstring_stream_write(stream, "World!");
    ccstring result = fossil_io_cstring_stream_read(stream);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", result);
    fossil_io_cstring_stream_free(stream);
}

FOSSIL_TEST(c_test_cstring_stream_empty_read) {
    fossil_io_cstring_stream *stream = fossil_io_cstring_stream_create(1024);
    ccstring result = fossil_io_cstring_stream_read(stream);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("", result);
    fossil_io_cstring_stream_free(stream);
}

FOSSIL_TEST(c_test_cstring_icmp) {
    const char *s1 = "Hello";
    const char *s2 = "hello";
    const char *s3 = "World";
    const char *s4 = NULL;
    const char *s5 = "";
    // Case-insensitive compare: equal
    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_icmp(s1, s2));
    // Case-insensitive compare: not equal
    ASSUME_ITS_TRUE(fossil_io_cstring_icmp(s1, s3) < 0 || fossil_io_cstring_icmp(s1, s3) > 0);
    // NULL vs NULL: equal
    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_icmp(s4, s4));
    // NULL vs empty string: equal
    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_icmp(s4, s5));
    // NULL vs non-empty: less
    ASSUME_ITS_TRUE(fossil_io_cstring_icmp(s4, s1) < 0);
    // non-empty vs NULL: greater
    ASSUME_ITS_TRUE(fossil_io_cstring_icmp(s1, s4) > 0);
}

FOSSIL_TEST(c_test_cstring_icontains) {
    const char *str = "Hello, World!";
    const char *substr1 = "world";
    const char *substr2 = "planet";
    ASSUME_ITS_TRUE(fossil_io_cstring_icontains(str, substr1));
    ASSUME_ITS_FALSE(fossil_io_cstring_icontains(str, substr2));
}

FOSSIL_TEST(c_test_cstring_format) {
    cstring result = fossil_io_cstring_format("Value: %d, %s", 42, "test");
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Value: 42, test", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(c_test_cstring_join) {
    ccstring arr[] = {"one", "two", "three"};
    cstring result = fossil_io_cstring_join(arr, 3, ',');
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("one,two,three", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(c_test_cstring_index_of) {
    const char *str = "Hello, World!";
    ASSUME_ITS_EQUAL_I32(7, fossil_io_cstring_index_of(str, "World"));
    ASSUME_ITS_EQUAL_I32(-1, fossil_io_cstring_index_of(str, "planet"));
}

FOSSIL_TEST(c_test_cstring_equals) {
    const char *a = "Test";
    const char *b = "Test";
    const char *c = "test";
    ASSUME_ITS_TRUE(fossil_io_cstring_equals(a, b));
    ASSUME_ITS_FALSE(fossil_io_cstring_equals(a, c));
}

FOSSIL_TEST(c_test_cstring_iequals) {
    const char *a = "Test";
    const char *b = "test";
    const char *c = "toast";
    ASSUME_ITS_TRUE(fossil_io_cstring_iequals(a, b));
    ASSUME_ITS_FALSE(fossil_io_cstring_iequals(a, c));
}

FOSSIL_TEST(c_test_cstring_escape_json) {
    cstring result = fossil_io_cstring_escape_json("He said: \"Hello\"\n");
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("He said: \\\"Hello\\\"\\n", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(c_test_cstring_unescape_json) {
    cstring result = fossil_io_cstring_unescape_json("He said: \\\"Hello\\\"\\n");
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("He said: \"Hello\"\n", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(c_test_cstring_normalize_spaces) {
    cstring str = fossil_io_cstring_create("   Hello   World   ");
    cstring result = fossil_io_cstring_normalize_spaces(str);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello World", result);
    fossil_io_cstring_free(str);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(c_test_cstring_strip_quotes) {
    cstring result1 = fossil_io_cstring_strip_quotes("\"Hello\"");
    cstring result2 = fossil_io_cstring_strip_quotes("'World'");
    cstring result3 = fossil_io_cstring_strip_quotes("NoQuotes");
    ASSUME_NOT_CNULL(result1);
    ASSUME_NOT_CNULL(result2);
    ASSUME_NOT_CNULL(result3);
    ASSUME_ITS_EQUAL_CSTR("Hello", result1);
    ASSUME_ITS_EQUAL_CSTR("World", result2);
    ASSUME_ITS_EQUAL_CSTR("NoQuotes", result3);
    fossil_io_cstring_free(result1);
    fossil_io_cstring_free(result2);
    fossil_io_cstring_free(result3);
}

FOSSIL_TEST(c_test_cstring_append) {
    cstring str = fossil_io_cstring_create("Hello");
    cstring result = fossil_io_cstring_append(&str, ", World!");
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", result);
    fossil_io_cstring_free(str);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_string_tests) {
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_create_and_free);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_copy);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_concat);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_length);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_compare);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_trim);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_split);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_replace);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_to_upper);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_to_lower);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_starts_with);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_ends_with);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_substring);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_reverse);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_contains);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_repeat);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_strip);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_count);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_pad_left);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_pad_right);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_icmp);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_icontains);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_format);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_join);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_index_of);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_equals);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_iequals);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_escape_json);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_unescape_json);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_normalize_spaces);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_strip_quotes);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_append);

    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_stream_create_and_free);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_stream_write_and_read);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_stream_multiple_writes);
    FOSSIL_TEST_ADD(c_string_suite, c_test_cstring_stream_empty_read);

    FOSSIL_TEST_REGISTER(c_string_suite);
}
