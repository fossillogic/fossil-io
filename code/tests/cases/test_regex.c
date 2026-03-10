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
FOSSIL_SUITE(c_regex_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_regex_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_regex_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_regex_compile_and_free) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("abc", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_compile_null_pattern) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile(NULL, NULL, &error);
    ASSUME_ITS_TRUE(re == NULL);
    ASSUME_ITS_TRUE(error != NULL);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_match_success) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("hello", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "hello", &match);
    ASSUME_ITS_EQUAL_I32(1, rc);
    ASSUME_ITS_TRUE(match != NULL);
    fossil_io_regex_match_free(match);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_match_failure) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("abc", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "def", &match);
    ASSUME_ITS_EQUAL_I32(0, rc);
    ASSUME_ITS_TRUE(match == NULL);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_match_null_args) {
    int rc = fossil_io_regex_match(NULL, "abc", NULL);
    ASSUME_ITS_TRUE(rc < 0);
    rc = fossil_io_regex_match((fossil_io_regex_t *)1, NULL, NULL);
    ASSUME_ITS_TRUE(rc < 0);
}

FOSSIL_TEST(c_test_regex_group_count_and_group) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("abc", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "abc", &match);
    ASSUME_ITS_EQUAL_I32(1, rc);
    ASSUME_ITS_TRUE(match != NULL);
    int count = fossil_io_regex_group_count(match);
    ASSUME_ITS_EQUAL_I32(0, count); // No groups supported in minimal compiler
    const char *group = fossil_io_regex_group(match, 0);
    ASSUME_ITS_TRUE(group == NULL);
    fossil_io_regex_match_free(match);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_compile_empty_pattern) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_compile_with_options) {
    char *error = NULL;
    const char *opts[] = { "icase", "multiline", NULL };
    fossil_io_regex_t *re = fossil_io_regex_compile("abc", opts, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_match_partial) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("abc", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "abcdef", &match);
    ASSUME_ITS_EQUAL_I32(1, rc);
    ASSUME_ITS_TRUE(match != NULL);
    fossil_io_regex_match_free(match);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_match_empty_text) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("abc", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "", &match);
    ASSUME_ITS_EQUAL_I32(0, rc);
    ASSUME_ITS_TRUE(match == NULL);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_match_null_out_match) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("abc", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    int rc = fossil_io_regex_match(re, "abc", NULL);
    ASSUME_ITS_EQUAL_I32(1, rc);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_dot_any_char) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("a.c", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "abc", &match);
    ASSUME_ITS_EQUAL_I32(1, rc);
    fossil_io_regex_match_free(match);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_caret_anchor) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("^abc", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "abcdef", &match);
    ASSUME_ITS_EQUAL_I32(1, rc);
    fossil_io_regex_match_free(match);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_dollar_anchor) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("abc$", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "xyzabc", &match);
    ASSUME_ITS_EQUAL_I32(1, rc);
    fossil_io_regex_match_free(match);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_char_class) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("[abc]", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "b", &match);
    ASSUME_ITS_EQUAL_I32(1, rc);
    fossil_io_regex_match_free(match);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_negated_char_class) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("[^abc]", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "x", &match);
    ASSUME_ITS_EQUAL_I32(1, rc);
    fossil_io_regex_match_free(match);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_escape_digit) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("\\d", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "5", &match);
    ASSUME_ITS_EQUAL_I32(1, rc);
    fossil_io_regex_match_free(match);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_escape_not_digit) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("\\D", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "a", &match);
    ASSUME_ITS_EQUAL_I32(1, rc);
    fossil_io_regex_match_free(match);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_escape_whitespace) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("\\s", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, " ", &match);
    ASSUME_ITS_EQUAL_I32(1, rc);
    fossil_io_regex_match_free(match);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_escape_word_char) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("\\w", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "a", &match);
    ASSUME_ITS_EQUAL_I32(1, rc);
    fossil_io_regex_match_free(match);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_quantifier_star) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("a*b", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "aaab", &match);
    ASSUME_ITS_EQUAL_I32(1, rc);
    fossil_io_regex_match_free(match);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_quantifier_plus) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("a+b", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "aab", &match);
    ASSUME_ITS_EQUAL_I32(1, rc);
    fossil_io_regex_match_free(match);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_quantifier_question) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("a?b", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "b", &match);
    ASSUME_ITS_EQUAL_I32(1, rc);
    fossil_io_regex_match_free(match);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_icase_option) {
    char *error = NULL;
    const char *opts[] = { "icase", NULL };
    fossil_io_regex_t *re = fossil_io_regex_compile("abc", opts, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "ABC", &match);
    ASSUME_ITS_EQUAL_I32(1, rc);
    fossil_io_regex_match_free(match);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_anchored_option) {
    char *error = NULL;
    const char *opts[] = { "anchored", NULL };
    fossil_io_regex_t *re = fossil_io_regex_compile("abc", opts, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "xyzabc", &match);
    ASSUME_ITS_EQUAL_I32(0, rc);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_group_length) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("abc", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "abc", &match);
    ASSUME_ITS_EQUAL_I32(1, rc);
    size_t len = fossil_io_regex_group_length(match, 0);
    ASSUME_ITS_EQUAL_I32(0, len);
    fossil_io_regex_match_free(match);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_match_length) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("hello", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "hello world", &match);
    ASSUME_ITS_EQUAL_I32(1, rc);
    ASSUME_ITS_EQUAL_I32(5, match->match_len);
    fossil_io_regex_match_free(match);
    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(c_test_regex_escape_literal) {
    char *error = NULL;
    fossil_io_regex_t *re = fossil_io_regex_compile("\\.", NULL, &error);
    ASSUME_ITS_TRUE(re != NULL);
    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, ".", &match);
    ASSUME_ITS_EQUAL_I32(1, rc);
    fossil_io_regex_match_free(match);
    fossil_io_regex_free(re);
    if (error) free(error);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_regex_tests) {
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_compile_and_free);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_compile_null_pattern);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_match_success);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_match_failure);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_match_null_args);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_group_count_and_group);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_compile_empty_pattern);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_compile_with_options);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_match_partial);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_match_empty_text);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_match_null_out_match);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_dot_any_char);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_caret_anchor);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_dollar_anchor);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_char_class);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_negated_char_class);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_escape_digit);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_escape_not_digit);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_escape_whitespace);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_escape_word_char);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_quantifier_star);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_quantifier_plus);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_quantifier_question);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_icase_option);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_anchored_option);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_group_length);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_match_length);
    FOSSIL_TEST_ADD(c_regex_suite, c_test_regex_escape_literal);

    FOSSIL_TEST_REGISTER(c_regex_suite);
}
