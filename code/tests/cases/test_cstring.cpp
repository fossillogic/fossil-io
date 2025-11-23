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
FOSSIL_SUITE(cpp_string_suite);
fossil_io_file_t cpp_string;

// Setup function for the test suite
FOSSIL_SETUP(cpp_string_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_string_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(cpp_test_cstring_create_and_free) {
    const char *init = "Hello, World!";
    cstring str = fossil_io_cstring_create(init);
    ASSUME_NOT_CNULL(str);
    ASSUME_ITS_EQUAL_CSTR(init, str);
    fossil_io_cstring_free(str);
}

FOSSIL_TEST(cpp_test_cstring_copy) {
    const char *init = "Hello, World!";
    cstring str = fossil_io_cstring_create(init);
    cstring copy = fossil_io_cstring_copy(str);
    ASSUME_NOT_CNULL(copy);
    ASSUME_ITS_EQUAL_CSTR(init, copy);
    fossil_io_cstring_free(str);
    fossil_io_cstring_free(copy);
}

FOSSIL_TEST(cpp_test_cstring_concat) {
    const char *s1 = "Hello, ";
    const char *s2 = "World!";
    cstring result = fossil_io_cstring_concat(s1, s2);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(cpp_test_cstring_length) {
    const char *str = "Hello, World!";
    size_t length = fossil_io_cstring_length(str);
    ASSUME_ITS_EQUAL_SIZE(strlen(str), length);
}

FOSSIL_TEST(cpp_test_cstring_compare) {
    const char *s1 = "Hello";
    const char *s2 = "Hello";
    const char *s3 = "World";
    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_compare(s1, s2));
    ASSUME_ITS_TRUE(fossil_io_cstring_compare(s1, s3) < 0);
    ASSUME_ITS_TRUE(fossil_io_cstring_compare(s3, s1) > 0);
}

FOSSIL_TEST(cpp_test_cstring_trim) {
    cstring str = fossil_io_cstring_create("   Hello, World!   ");
    fossil_io_cstring_trim(str);
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", str);
    fossil_io_cstring_free(str);
}

FOSSIL_TEST(cpp_test_cstring_split) {
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

FOSSIL_TEST(cpp_test_cstring_replace) {
    const char *str = "Hello, World!";
    const char *old = "World";
    const char *new_str = "Fossil";
    cstring result = fossil_io_cstring_replace(str, old, new_str);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello, Fossil!", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(cpp_test_cstring_to_upper) {
    cstring str = fossil_io_cstring_create("Hello, World!");
    cstring result = fossil_io_cstring_to_upper(str);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("HELLO, WORLD!", result);
    fossil_io_cstring_free(str);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(cpp_test_cstring_to_lower) {
    cstring str = fossil_io_cstring_create("Hello, World!");
    cstring result = fossil_io_cstring_to_lower(str);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("hello, world!", result);
    fossil_io_cstring_free(str);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(cpp_test_cstring_starts_with) {
    const char *str = "Hello, World!";
    const char *prefix = "Hello";
    ASSUME_ITS_TRUE(fossil_io_cstring_starts_with(str, prefix));
}

FOSSIL_TEST(cpp_test_cstring_ends_with) {
    const char *str = "Hello, World!";
    const char *suffix = "World!";
    ASSUME_ITS_TRUE(fossil_io_cstring_ends_with(str, suffix));
}

FOSSIL_TEST(cpp_test_cstring_substring) {
    const char *str = "Hello, World!";
    cstring result = fossil_io_cstring_substring(str, 7, 5);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("World", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(cpp_test_cstring_reverse) {
    cstring str = fossil_io_cstring_create("Hello, World!");
    cstring result = fossil_io_cstring_reverse(str);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("!dlroW ,olleH", result);
    fossil_io_cstring_free(str);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(cpp_test_cstring_contains) {
    const char *str = "Hello, World!";
    const char *substr = "World";
    ASSUME_ITS_TRUE(fossil_io_cstring_contains(str, substr));
}

FOSSIL_TEST(cpp_test_cstring_repeat) {
    const char *str = "Hello";
    cstring result = fossil_io_cstring_repeat(str, 3);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("HelloHelloHello", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(cpp_test_cstring_strip) {
    const char *str = "!!!Hello, World!!!";
    cstring result = fossil_io_cstring_strip(str, '!');
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello, World", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(cpp_test_cstring_count) {
    const char *str = "Hello, World! Hello, Fossil!";
    const char *substr = "Hello";
    size_t count = fossil_io_cstring_count(str, substr);
    ASSUME_ITS_EQUAL_SIZE(2, count);
}

FOSSIL_TEST(cpp_test_cstring_pad_left) {
    const char *str = "Hello";
    cstring result = fossil_io_cstring_pad_left(str, 10, '*');
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("*****Hello", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(cpp_test_cstring_pad_right) {
    const char *str = "Hello";
    cstring result = fossil_io_cstring_pad_right(str, 10, '*');
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello*****", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(cpp_test_cstring_icmp) {
    const char *s1 = "Hello";
    const char *s2 = "hello";
    const char *s3 = "World";
    const char *s4 = "Hell";
    const char *s5 = "Hello!";

    // Case-insensitive equal
    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_icmp(s1, s2));
    // Not equal, s1 < s3
    ASSUME_ITS_TRUE(fossil_io_cstring_icmp(s1, s3) < 0);
    // Not equal, s3 > s1
    ASSUME_ITS_TRUE(fossil_io_cstring_icmp(s3, s1) > 0);
    // Prefix: s1 > s4
    ASSUME_ITS_TRUE(fossil_io_cstring_icmp(s1, s4) > 0);
    // s1 < s5 (shorter)
    ASSUME_ITS_TRUE(fossil_io_cstring_icmp(s1, s5) < 0);
    // NULL handling
    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_icmp(nullptr, nullptr));
    ASSUME_ITS_TRUE(fossil_io_cstring_icmp(nullptr, "abc") < 0);
    ASSUME_ITS_TRUE(fossil_io_cstring_icmp("abc", nullptr) > 0);
    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_icmp(nullptr, ""));
    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_icmp("", nullptr));
}

FOSSIL_TEST(cpp_test_cstring_icontains) {
    const char *str = "Hello, World!";
    const char *substr1 = "world";
    const char *substr2 = "planet";
    ASSUME_ITS_TRUE(fossil_io_cstring_icontains(str, substr1));
    ASSUME_ITS_FALSE(fossil_io_cstring_icontains(str, substr2));
}

FOSSIL_TEST(cpp_test_cstring_format) {
    cstring result = fossil_io_cstring_format("Value: %d, %s", 42, "test");
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Value: 42, test", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(cpp_test_cstring_join) {
    ccstring arr[] = {"one", "two", "three"};
    cstring result = fossil_io_cstring_join(arr, 3, ',');
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("one,two,three", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(cpp_test_cstring_index_of) {
    const char *str = "Hello, World!";
    ASSUME_ITS_EQUAL_I32(7, fossil_io_cstring_index_of(str, "World"));
    ASSUME_ITS_EQUAL_I32(-1, fossil_io_cstring_index_of(str, "planet"));
}

FOSSIL_TEST(cpp_test_cstring_equals) {
    const char *a = "Test";
    const char *b = "Test";
    const char *c = "test";
    ASSUME_ITS_TRUE(fossil_io_cstring_equals(a, b));
    ASSUME_ITS_FALSE(fossil_io_cstring_equals(a, c));
}

FOSSIL_TEST(cpp_test_cstring_iequals) {
    const char *a = "Test";
    const char *b = "test";
    const char *c = "toast";
    ASSUME_ITS_TRUE(fossil_io_cstring_iequals(a, b));
    ASSUME_ITS_FALSE(fossil_io_cstring_iequals(a, c));
}

FOSSIL_TEST(cpp_test_cstring_escape_json) {
    cstring result = fossil_io_cstring_escape_json("He said: \"Hello\"\n");
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("He said: \\\"Hello\\\"\\n", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(cpp_test_cstring_unescape_json) {
    cstring result = fossil_io_cstring_unescape_json("He said: \\\"Hello\\\"\\n");
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("He said: \"Hello\"\n", result);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(cpp_test_cstring_normalize_spaces) {
    cstring str = fossil_io_cstring_create("   Hello   World   ");
    cstring result = fossil_io_cstring_normalize_spaces(str);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello World", result);
    fossil_io_cstring_free(str);
    fossil_io_cstring_free(result);
}

FOSSIL_TEST(cpp_test_cstring_strip_quotes) {
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

FOSSIL_TEST(cpp_test_cstring_append) {
    cstring str = fossil_io_cstring_create("Hello");
    cstring result = fossil_io_cstring_append(&str, ", World!");
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", result);
    fossil_io_cstring_free(str);
}

FOSSIL_TEST(cpp_test_cstring_stream_create_and_free) {
    fossil_io_cstring_stream *stream = fossil_io_cstring_stream_create(1024);
    ASSUME_NOT_CNULL(stream);
    fossil_io_cstring_stream_free(stream);
}

FOSSIL_TEST(cpp_test_cstring_stream_write_and_read) {
    fossil_io_cstring_stream *stream = fossil_io_cstring_stream_create(1024);
    fossil_io_cstring_stream_write(stream, "Hello, World!");
    ccstring result = fossil_io_cstring_stream_read(stream);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", result);
    fossil_io_cstring_stream_free(stream);
}

FOSSIL_TEST(cpp_test_cstring_stream_multiple_writes) {
    fossil_io_cstring_stream *stream = fossil_io_cstring_stream_create(1024);
    fossil_io_cstring_stream_write(stream, "Hello, ");
    fossil_io_cstring_stream_write(stream, "World!");
    ccstring result = fossil_io_cstring_stream_read(stream);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", result);
    fossil_io_cstring_stream_free(stream);
}

FOSSIL_TEST(cpp_test_cstring_stream_empty_read) {
    fossil_io_cstring_stream *stream = fossil_io_cstring_stream_create(1024);
    ccstring result = fossil_io_cstring_stream_read(stream);
    ASSUME_NOT_CNULL(result);
    ASSUME_ITS_EQUAL_CSTR("", result);
    fossil_io_cstring_stream_free(stream);
}

FOSSIL_TEST(cpp_test_cstring_class_create_and_free) {
    fossil::io::CString str("Hello, World!");
    ASSUME_NOT_CNULL(str.str());
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", str.str());
}

FOSSIL_TEST(cpp_test_cstring_class_copy) {
    fossil::io::CString str("Hello, World!");
    fossil::io::CString copy = fossil::io::CString::copy("Hello, World!");
    ASSUME_NOT_CNULL(copy.str());
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", copy.str());
}

FOSSIL_TEST(cpp_test_cstring_class_concat) {
    fossil::io::CString result = fossil::io::CString::concat("Hello, ", "World!");
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", result.str());
}

FOSSIL_TEST(cpp_test_cstring_class_length) {
    fossil::io::CString str("Hello, World!");
    size_t length = str.length();
    ASSUME_ITS_EQUAL_SIZE(strlen("Hello, World!"), length);
}

FOSSIL_TEST(cpp_test_cstring_class_compare) {
    fossil::io::CString s1("Hello");
    fossil::io::CString s2("Hello");
    fossil::io::CString s3("World");
    ASSUME_ITS_EQUAL_I32(0, s1.compare("Hello"));
    ASSUME_ITS_TRUE(s1.compare("World") < 0);
    ASSUME_ITS_TRUE(s3.compare("Hello") > 0);
}

FOSSIL_TEST(cpp_test_cstring_class_trim) {
    fossil::io::CString str("   Hello, World!   ");
    str.trim();
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", str.str());
}

FOSSIL_TEST(cpp_test_cstring_class_split) {
    fossil::io::CString str("Hello,World,Test");
    size_t count;
    std::vector<std::string> result = str.split(',', &count);
    ASSUME_ITS_EQUAL_SIZE(3, count);
    ASSUME_ITS_EQUAL_CSTR("Hello", result[0].c_str());
    ASSUME_ITS_EQUAL_CSTR("World", result[1].c_str());
    ASSUME_ITS_EQUAL_CSTR("Test", result[2].c_str());
}

FOSSIL_TEST(cpp_test_cstring_class_replace) {
    fossil::io::CString str("Hello, World!");
    fossil::io::CString result = str.replace("World", "Fossil");
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("Hello, Fossil!", result.str());
}

FOSSIL_TEST(cpp_test_cstring_class_to_upper) {
    fossil::io::CString str("Hello, World!");
    fossil::io::CString result = str.to_upper();
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("HELLO, WORLD!", result.str());
}

FOSSIL_TEST(cpp_test_cstring_class_to_lower) {
    fossil::io::CString str("Hello, World!");
    fossil::io::CString result = str.to_lower();
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("hello, world!", result.str());
}

FOSSIL_TEST(cpp_test_cstring_class_starts_with) {
    fossil::io::CString str("Hello, World!");
    ASSUME_ITS_TRUE(str.starts_with("Hello"));
}

FOSSIL_TEST(cpp_test_cstring_class_ends_with) {
    fossil::io::CString str("Hello, World!");
    ASSUME_ITS_TRUE(str.ends_with("World!"));
}

FOSSIL_TEST(cpp_test_cstring_class_substring) {
    fossil::io::CString str("Hello, World!");
    fossil::io::CString result = str.substring(7, 5);
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("World", result.str());
}

FOSSIL_TEST(cpp_test_cstring_class_reverse) {
    fossil::io::CString str("Hello, World!");
    fossil::io::CString result = str.reverse();
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("!dlroW ,olleH", result.str());
}

FOSSIL_TEST(cpp_test_cstring_class_contains) {
    fossil::io::CString str("Hello, World!");
    ASSUME_ITS_TRUE(str.contains("World"));
}

FOSSIL_TEST(cpp_test_cstring_class_repeat) {
    fossil::io::CString str("Hello");
    fossil::io::CString result = str.repeat(3);
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("HelloHelloHello", result.str());
}

FOSSIL_TEST(cpp_test_cstring_class_strip) {
    fossil::io::CString str("!!!Hello, World!!!");
    fossil::io::CString result = str.strip('!');
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("Hello, World", result.str());
}

FOSSIL_TEST(cpp_test_cstring_class_count) {
    fossil::io::CString str("Hello, World! Hello, Fossil!");
    size_t count = str.count("Hello");
    ASSUME_ITS_EQUAL_SIZE(2, count);
}

FOSSIL_TEST(cpp_test_cstring_class_pad_left) {
    fossil::io::CString str("Hello");
    fossil::io::CString result = str.pad_left(10, '*');
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("*****Hello", result.str());
}

FOSSIL_TEST(cpp_test_cstring_class_pad_right) {
    fossil::io::CString str("Hello");
    fossil::io::CString result = str.pad_right(10, '*');
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("Hello*****", result.str());
}

FOSSIL_TEST(cpp_test_cstring_stream_class_create_and_free) {
    fossil::io::CStringStream stream(1024);
    ASSUME_NOT_CNULL(stream.read().c_str());
}

FOSSIL_TEST(cpp_test_cstring_stream_class_write_and_read) {
    fossil::io::CStringStream stream(1024);
    stream.write("Hello, World!");
    std::string result = stream.read();
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", result.c_str());
}

FOSSIL_TEST(cpp_test_cstring_stream_class_multiple_writes) {
    fossil::io::CStringStream stream(1024);
    stream.write("Hello, ");
    stream.write("World!");
    std::string result = stream.read();
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", result.c_str());
}

FOSSIL_TEST(cpp_test_cstring_stream_class_empty_read) {
    fossil::io::CStringStream stream(1024);
    std::string result = stream.read();
    ASSUME_ITS_EQUAL_CSTR("", result.c_str());
}

FOSSIL_TEST(cpp_test_cstring_class_icontains) {
    fossil::io::CString str("Hello, World!");
    ASSUME_ITS_TRUE(str.icontains("world"));
    ASSUME_ITS_TRUE(str.icontains("WORLD"));
    ASSUME_ITS_TRUE(!str.icontains("fossil"));
}

FOSSIL_TEST(cpp_test_cstring_class_join) {
    std::vector<std::string> parts = {"one", "two", "three"};
    fossil::io::CString result = fossil::io::CString::join(parts, ',');
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("one,two,three", result.str());
}

FOSSIL_TEST(cpp_test_cstring_class_index_of) {
    fossil::io::CString str("Hello, World!");
    ASSUME_ITS_EQUAL_I32(7, str.index_of("World"));
    ASSUME_ITS_EQUAL_I32(-1, str.index_of("Fossil"));
}

FOSSIL_TEST(cpp_test_cstring_class_equals) {
    fossil::io::CString str("Hello, World!");
    ASSUME_ITS_TRUE(str.equals("Hello, World!"));
    ASSUME_ITS_TRUE(!str.equals("hello, world!"));
}

FOSSIL_TEST(cpp_test_cstring_class_iequals) {
    fossil::io::CString str("Hello, World!");
    ASSUME_ITS_TRUE(str.iequals("hello, world!"));
    ASSUME_ITS_TRUE(str.iequals("HELLO, WORLD!"));
    ASSUME_ITS_TRUE(!str.iequals("Hello, Fossil!"));
}

FOSSIL_TEST(cpp_test_cstring_class_escape_json) {
    fossil::io::CString str("He said: \"Hello, World!\"\n");
    fossil::io::CString escaped = str.escape_json();
    ASSUME_NOT_CNULL(escaped.str());
    ASSUME_ITS_TRUE(std::string(escaped.str()).find("\\\"Hello, World!\\\"") != std::string::npos);
}

FOSSIL_TEST(cpp_test_cstring_class_unescape_json) {
    fossil::io::CString str("He said: \\\"Hello, World!\\\"\\n");
    fossil::io::CString unescaped = str.unescape_json();
    ASSUME_NOT_CNULL(unescaped.str());
    ASSUME_ITS_TRUE(std::string(unescaped.str()).find("\"Hello, World!\"\n") != std::string::npos);
}

FOSSIL_TEST(cpp_test_cstring_class_normalize_spaces) {
    fossil::io::CString str("Hello,    World!   This   is   Fossil.");
    fossil::io::CString normalized = str.normalize_spaces();
    ASSUME_NOT_CNULL(normalized.str());
    ASSUME_ITS_EQUAL_CSTR("Hello, World! This is Fossil.", normalized.str());
}

FOSSIL_TEST(cpp_test_cstring_class_strip_quotes) {
    fossil::io::CString str1("\"Hello, World!\"");
    fossil::io::CString str2("'Hello, World!'");
    fossil::io::CString str3("Hello, World!");
    fossil::io::CString result1 = str1.strip_quotes();
    fossil::io::CString result2 = str2.strip_quotes();
    fossil::io::CString result3 = str3.strip_quotes();
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", result1.str());
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", result2.str());
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", result3.str());
}

FOSSIL_TEST(cpp_test_cstring_class_append) {
    fossil::io::CString str("Hello");
    int rc = str.append(", World!");
    ASSUME_ITS_EQUAL_I32(0, rc);
    ASSUME_ITS_EQUAL_CSTR("Hello, World!", str.str());
}

FOSSIL_TEST(cpp_test_cstring_class_copy_safe) {
    std::string src = "SafeCopy";
    fossil::io::CString result = fossil::io::CString::copy_safe(src);
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("SafeCopy", result.str());
}

FOSSIL_TEST(cpp_test_cstring_class_dup_safe) {
    std::string src = "SafeDup";
    fossil::io::CString result = fossil::io::CString::dup_safe(src);
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("SafeDup", result.str());
}

FOSSIL_TEST(cpp_test_cstring_class_concat_safe) {
    std::string s1 = "Safe";
    std::string s2 = "Concat";
    fossil::io::CString result = fossil::io::CString::concat_safe(s1, s2, 16);
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("SafeConcat", result.str());
    fossil::io::CString truncated = fossil::io::CString::concat_safe(s1, s2, 5);
    ASSUME_ITS_EQUAL_CSTR("SafeC", truncated.str());
}

FOSSIL_TEST(cpp_test_cstring_class_substring_safe) {
    fossil::io::CString str("SafeSubstringTest");
    fossil::io::CString result = str.substring_safe(4, 9, 20);
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("Substring", result.str());
    fossil::io::CString limited = str.substring_safe(4, 20, 10);
    ASSUME_ITS_TRUE(strlen(limited.str()) <= 10);
}

FOSSIL_TEST(cpp_test_cstring_class_reverse_safe) {
    fossil::io::CString str("SafeReverse");
    fossil::io::CString result = str.reverse_safe(11);
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("esreveRefaS", result.str());
    fossil::io::CString limited = str.reverse_safe(4);
    ASSUME_ITS_EQUAL_CSTR("efaS", limited.str());
}

FOSSIL_TEST(cpp_test_cstring_class_contains_safe) {
    fossil::io::CString str("SafeContainsTest");
    ASSUME_ITS_TRUE(str.contains_safe("Contains", 20));
    ASSUME_ITS_FALSE(str.contains_safe("Missing", 20));
}

FOSSIL_TEST(cpp_test_cstring_class_repeat_safe) {
    fossil::io::CString str("Safe");
    fossil::io::CString result = str.repeat_safe(3, 20);
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("SafeSafeSafe", result.str());
    fossil::io::CString limited = str.repeat_safe(10, 8);
    ASSUME_ITS_TRUE(strlen(limited.str()) <= 8);
}

FOSSIL_TEST(cpp_test_cstring_class_strip_safe) {
    fossil::io::CString str("!!!Safe!!!");
    fossil::io::CString result = str.strip_safe('!', 20);
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("Safe", result.str());
    fossil::io::CString limited = str.strip_safe('!', 4);
    ASSUME_ITS_TRUE(strlen(limited.str()) <= 4);
}

FOSSIL_TEST(cpp_test_cstring_class_count_safe) {
    fossil::io::CString str("Safe Safe Safe");
    size_t count = str.count_safe("Safe", 20);
    ASSUME_ITS_EQUAL_SIZE(3, count);
    size_t limited = str.count_safe("Safe", 4);
    ASSUME_ITS_EQUAL_SIZE(1, limited);
}

FOSSIL_TEST(cpp_test_cstring_class_pad_left_safe) {
    fossil::io::CString str("Safe");
    fossil::io::CString result = str.pad_left_safe(8, '*', 8);
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("****", result.str());
    fossil::io::CString limited = str.pad_left_safe(10, '*', 6);
    ASSUME_ITS_TRUE(strlen(limited.str()) <= 6);
}

FOSSIL_TEST(cpp_test_cstring_class_pad_right_safe) {
    fossil::io::CString str("Safe");
    fossil::io::CString result = str.pad_right_safe(8, '*', 8);
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("Safe***", result.str());
    fossil::io::CString limited = str.pad_right_safe(10, '*', 6);
    ASSUME_ITS_TRUE(strlen(limited.str()) <= 6);
}

FOSSIL_TEST(cpp_test_cstring_class_starts_with_safe) {
    fossil::io::CString str("SafePrefixTest");
    ASSUME_ITS_TRUE(str.starts_with_safe("Safe", 20));
    ASSUME_ITS_FALSE(str.starts_with_safe("Prefix", 4));
}

FOSSIL_TEST(cpp_test_cstring_class_ends_with_safe) {
    fossil::io::CString str("SafeSuffixTest");
    ASSUME_ITS_TRUE(str.ends_with_safe("Test", 20));
    ASSUME_ITS_FALSE(str.ends_with_safe("Suffix", 4));
}

FOSSIL_TEST(cpp_test_cstring_class_equals_safe) {
    fossil::io::CString str("SafeEquals");
    ASSUME_ITS_TRUE(str.equals_safe("SafeEquals", 20));
    ASSUME_ITS_FALSE(str.equals_safe("safeequals", 20));
    ASSUME_ITS_TRUE(str.equals_safe("SafeEquals", 4));
}

FOSSIL_TEST(cpp_test_cstring_class_iequals_safe) {
    fossil::io::CString str("SafeEquals");
    ASSUME_ITS_TRUE(str.iequals_safe("safeequals", 20));
    ASSUME_ITS_TRUE(str.iequals_safe("Safe", 20)); // "SafeEquals" equals "Safe" for first 4 chars, case-insensitive
    ASSUME_ITS_TRUE(str.iequals_safe("SafeEquals", 4));
}

FOSSIL_TEST(cpp_test_cstring_class_icontains_safe) {
    fossil::io::CString str("SafeContainsTest");
    ASSUME_ITS_TRUE(str.icontains_safe("contains", 20));
    ASSUME_ITS_FALSE(str.icontains_safe("missing", 20));
    ASSUME_ITS_FALSE(str.icontains_safe("contains", 5));
}

FOSSIL_TEST(cpp_test_cstring_class_strip_quotes_safe) {
    fossil::io::CString str1("\"SafeQuotes\"");
    fossil::io::CString str2("'SafeQuotes'");
    fossil::io::CString str3("SafeQuotes");
    fossil::io::CString result1 = str1.strip_quotes_safe(20);
    fossil::io::CString result2 = str2.strip_quotes_safe(20);
    fossil::io::CString result3 = str3.strip_quotes_safe(20);
    ASSUME_ITS_EQUAL_CSTR("SafeQuotes", result1.str());
    ASSUME_ITS_EQUAL_CSTR("SafeQuotes", result2.str());
    ASSUME_ITS_EQUAL_CSTR("SafeQuotes", result3.str());
}

FOSSIL_TEST(cpp_test_cstring_class_normalize_spaces_safe) {
    fossil::io::CString str("Safe   Spaces   Test");
    fossil::io::CString result = str.normalize_spaces_safe(20);
    result.trim();
    ASSUME_NOT_CNULL(result.str());
    ASSUME_ITS_EQUAL_CSTR("Safe Spaces Test", result.str());
    fossil::io::CString limited = str.normalize_spaces_safe(4);
    ASSUME_ITS_TRUE(strlen(limited.str()) <= 4);
}

FOSSIL_TEST(cpp_test_cstring_class_index_of_safe) {
    fossil::io::CString str("SafeIndexTest");
    ASSUME_ITS_EQUAL_I32(4, str.index_of_safe("Index", 20));
    ASSUME_ITS_EQUAL_I32(-1, str.index_of_safe("Missing", 20));
    ASSUME_ITS_EQUAL_I32(-1, str.index_of_safe("Index", 4));
}

FOSSIL_TEST(cpp_test_cstring_class_silly) {
    fossil::io::CString str("Hello, World!");
    std::string silly = str.silly();
    ASSUME_NOT_CNULL(silly.c_str());
    ASSUME_ITS_TRUE(silly.length() > 0);
}

FOSSIL_TEST(cpp_test_cstring_class_piglatin) {
    fossil::io::CString str("Hello, World!");
    std::string pig = str.piglatin();
    ASSUME_NOT_CNULL(pig.c_str());
    ASSUME_ITS_TRUE(pig.length() > 0);
}

FOSSIL_TEST(cpp_test_cstring_class_leetspeak) {
    fossil::io::CString str("Hello, World!");
    std::string leet = str.leetspeak();
    ASSUME_NOT_CNULL(leet.c_str());
    ASSUME_ITS_TRUE(leet.length() > 0);
}

FOSSIL_TEST(cpp_test_cstring_class_mocking) {
    fossil::io::CString str("Hello, World!");
    fossil::io::CString mocking = str.mocking();
    ASSUME_NOT_CNULL(mocking.str());
    ASSUME_ITS_TRUE(mocking.length() == str.length());
}

FOSSIL_TEST(cpp_test_cstring_class_rot13) {
    fossil::io::CString str("Hello, World!");
    fossil::io::CString rot = str.rot13();
    ASSUME_NOT_CNULL(rot.str());
    ASSUME_ITS_TRUE(rot.length() == str.length());
}

FOSSIL_TEST(cpp_test_cstring_class_shuffle) {
    fossil::io::CString str("Hello, World!");
    fossil::io::CString shuffled = str.shuffle();
    ASSUME_NOT_CNULL(shuffled.str());
    ASSUME_ITS_TRUE(shuffled.length() == str.length());
}

FOSSIL_TEST(cpp_test_cstring_class_upper_snake) {
    fossil::io::CString str("Hello World Fossil");
    fossil::io::CString snake = str.upper_snake();
    ASSUME_NOT_CNULL(snake.str());
    ASSUME_ITS_TRUE(std::string(snake.str()).find('_') != std::string::npos);
}

FOSSIL_TEST(cpp_test_cstring_number_from_words_basic) {
    int value = 0;
    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_number_from_words("zero", &value));
    ASSUME_ITS_EQUAL_I32(0, value);

    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_number_from_words("one", &value));
    ASSUME_ITS_EQUAL_I32(1, value);

    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_number_from_words("ten", &value));
    ASSUME_ITS_EQUAL_I32(10, value);

    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_number_from_words("twenty-three", &value));
    ASSUME_ITS_EQUAL_I32(23, value);

    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_number_from_words("one hundred", &value));
    ASSUME_ITS_EQUAL_I32(100, value);

    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_number_from_words("one thousand two hundred thirty-four", &value));
    ASSUME_ITS_EQUAL_I32(1234, value);
}

FOSSIL_TEST(cpp_test_cstring_number_from_words_invalid) {
    int value = 0;
    ASSUME_ITS_TRUE(fossil_io_cstring_number_from_words("not-a-number", &value) != 0);
    // "" is typically interpreted as zero, so this should be equal to 0
    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_number_from_words("", &value));
    ASSUME_ITS_TRUE(fossil_io_cstring_number_from_words("twenty one hundred apples", &value) != 0);
}

FOSSIL_TEST(cpp_test_cstring_number_to_words_basic) {
    char buffer[128];
    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_number_to_words(0, buffer, sizeof(buffer)));
    ASSUME_ITS_EQUAL_CSTR("zero", buffer);

    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_number_to_words(1, buffer, sizeof(buffer)));
    ASSUME_ITS_EQUAL_CSTR("one", buffer);

    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_number_to_words(23, buffer, sizeof(buffer)));
    ASSUME_ITS_EQUAL_CSTR("twenty-three", buffer);

    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_number_to_words(100, buffer, sizeof(buffer)));
    ASSUME_ITS_EQUAL_CSTR("one hundred", buffer);

    ASSUME_ITS_EQUAL_I32(0, fossil_io_cstring_number_to_words(1234, buffer, sizeof(buffer)));
    ASSUME_ITS_EQUAL_CSTR("one thousand two hundred and thirty-four", buffer);
}

FOSSIL_TEST(cpp_test_cstring_number_to_words_buffer_too_small) {
    char buffer[8];
    ASSUME_ITS_TRUE(fossil_io_cstring_number_to_words(123456, buffer, sizeof(buffer)) != 0);
}

FOSSIL_TEST(cpp_test_cstring_class_number_from_words_and_to_words) {
    using fossil::io::CString;
    ASSUME_ITS_EQUAL_I32(23, CString::number_from_words("twenty-three"));
    ASSUME_ITS_EQUAL_I32(100, CString::number_from_words("one hundred"));
    ASSUME_ITS_EQUAL_I32(0, CString::number_from_words("zero"));

    ASSUME_ITS_EQUAL_CSTR("twenty-three", CString::number_to_words(23).c_str());
    ASSUME_ITS_EQUAL_CSTR("one hundred", CString::number_to_words(100).c_str());
    ASSUME_ITS_EQUAL_CSTR("zero", CString::number_to_words(0).c_str());
}

FOSSIL_TEST(cpp_test_cstring_class_number_from_words_exception) {
    using fossil::io::CString;
    bool thrown = false;
    try {
        CString::number_from_words("not-a-number");
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    ASSUME_ITS_TRUE(thrown);
}

FOSSIL_TEST(cpp_test_cstring_class_number_to_words_exception) {
    using fossil::io::CString;
    bool thrown = false;
    try {
        // Use a very large number that would overflow the buffer
        CString::number_to_words(1234567890);
    } catch (const std::runtime_error&) {
        thrown = true;
    }
    ASSUME_ITS_TRUE(thrown);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_string_tests) {
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_create_and_free);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_copy);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_concat);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_length);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_compare);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_trim);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_split);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_replace);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_to_upper);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_to_lower);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_starts_with);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_ends_with);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_substring);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_reverse);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_contains);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_repeat);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_strip);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_count);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_pad_left);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_pad_right);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_icmp);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_icontains);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_format);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_join);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_index_of);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_equals);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_iequals);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_escape_json);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_unescape_json);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_normalize_spaces);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_strip_quotes);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_append);

    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_copy_safe);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_dup_safe);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_concat_safe);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_substring_safe);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_reverse_safe);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_contains_safe);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_repeat_safe);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_strip_safe);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_count_safe);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_pad_left_safe);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_pad_right_safe);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_starts_with_safe);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_ends_with_safe);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_equals_safe);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_iequals_safe);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_icontains_safe);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_strip_quotes_safe);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_normalize_spaces_safe);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_index_of_safe);

    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_stream_create_and_free);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_stream_write_and_read);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_stream_multiple_writes);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_stream_empty_read);

    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_silly);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_piglatin);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_leetspeak);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_mocking);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_rot13);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_shuffle);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_upper_snake);

    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_create_and_free);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_copy);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_concat);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_length);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_compare);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_trim);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_split);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_replace);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_to_upper);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_to_lower);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_starts_with);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_ends_with);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_substring);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_reverse);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_contains);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_repeat);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_strip);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_count);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_pad_left);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_pad_right);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_icontains);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_join);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_index_of);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_equals);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_iequals);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_escape_json);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_unescape_json);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_normalize_spaces);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_strip_quotes);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_append);

    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_number_from_words_basic);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_number_from_words_invalid);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_number_to_words_basic);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_number_to_words_buffer_too_small);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_number_from_words_and_to_words);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_number_from_words_exception);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_class_number_to_words_exception);

    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_stream_class_create_and_free);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_stream_class_write_and_read);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_stream_class_multiple_writes);
    FOSSIL_TEST_ADD(cpp_string_suite, cpp_test_cstring_stream_class_empty_read);

    FOSSIL_TEST_REGISTER(cpp_string_suite);
}
