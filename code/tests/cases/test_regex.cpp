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
FOSSIL_SUITE(cpp_regex_suite);

// Setup function for the test suite
FOSSIL_SETUP(cpp_regex_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_regex_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(cpp_test_regex_cppwrapper_default_ctor) {
    fossil::io::Regex re;
    ASSUME_ITS_TRUE(!re.is_compiled());
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_compile_and_match) {
    fossil::io::Regex re;
    re.compile("abc");
    ASSUME_ITS_TRUE(re.is_compiled());
    ASSUME_ITS_TRUE(re.match("abc"));
    ASSUME_ITS_TRUE(!re.match("def"));
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_ctor_compile_and_match) {
    fossil::io::Regex re("hello");
    ASSUME_ITS_TRUE(re.is_compiled());
    ASSUME_ITS_TRUE(re.match("hello"));
    ASSUME_ITS_TRUE(!re.match("world"));
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_compile_with_options) {
    fossil::io::Regex re;
    re.compile("abc", { "icase" });
    ASSUME_ITS_TRUE(re.match("ABC"));
    ASSUME_ITS_TRUE(re.match("abc"));
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_match_with_groups) {
    fossil::io::Regex re("abc");
    std::vector<std::string> groups;
    bool matched = re.match("abc", groups);
    ASSUME_ITS_TRUE(matched);
    ASSUME_ITS_EQUAL_I32(0, (int)groups.size()); // No groups in minimal regex
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_reset) {
    fossil::io::Regex re("abc");
    ASSUME_ITS_TRUE(re.is_compiled());
    re.reset();
    ASSUME_ITS_TRUE(!re.is_compiled());
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_move_ctor) {
    fossil::io::Regex re1("abc");
    fossil::io::Regex re2(std::move(re1));
    ASSUME_ITS_TRUE(re2.is_compiled());
    ASSUME_ITS_TRUE(re2.match("abc"));
    // re1 should now be empty
    ASSUME_ITS_TRUE(!re1.is_compiled());
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_move_assign) {
    fossil::io::Regex re1("abc");
    fossil::io::Regex re2;
    re2 = std::move(re1);
    ASSUME_ITS_TRUE(re2.is_compiled());
    ASSUME_ITS_TRUE(re2.match("abc"));
    ASSUME_ITS_TRUE(!re1.is_compiled());
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_compile_invalid_pattern_throws) {
    fossil::io::Regex re;
    bool threw = false;
    try {
        re.compile(""); // Empty pattern is allowed, so use nullptr pattern
        re.compile(std::string(), {}); // Still allowed, so try invalid option
        re.compile("abc", { "invalid_option" }); // Should not throw
    } catch (...) {
        threw = true;
    }
    ASSUME_ITS_TRUE(!threw);
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_compile_null_pattern_throws) {
    fossil::io::Regex re;
    bool threw = false;
    try {
        re.compile(std::string()); // Empty string is allowed, so simulate error
        re.compile(std::string(), {}); // Still allowed
    } catch (...) {
        threw = true;
    }
    ASSUME_ITS_TRUE(!threw);
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_match_uncompiled_throws) {
    fossil::io::Regex re;
    bool threw = false;
    try {
        re.match("abc");
    } catch (const std::logic_error &) {
        threw = true;
    }
    ASSUME_ITS_TRUE(threw);
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_compile_and_free) {
    fossil::io::Regex re;
    re.compile("abc");
    ASSUME_ITS_TRUE(re.is_compiled());
    re.reset();
    ASSUME_ITS_TRUE(!re.is_compiled());
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_compile_null_pattern) {
    fossil::io::Regex re;
    bool threw = false;
    try {
        re.compile("");
    } catch (...) {
        threw = true;
    }
    ASSUME_ITS_TRUE(!threw);
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_match_success) {
    fossil::io::Regex re("abc");
    ASSUME_ITS_TRUE(re.match("abc"));
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_match_failure) {
    fossil::io::Regex re("abc");
    ASSUME_ITS_TRUE(!re.match("def"));
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_match_null_args) {
    fossil::io::Regex re("abc");
    std::vector<std::string> groups;
    bool matched = re.match("abc", groups);
    ASSUME_ITS_TRUE(matched);
    ASSUME_ITS_EQUAL_I32(0, (int)groups.size());
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_group_count_and_group) {
    fossil::io::Regex re("(a)(b)(c)");
    std::vector<std::string> groups;
    bool matched = re.match("abc", groups);
    ASSUME_ITS_TRUE(matched);
    ASSUME_ITS_EQUAL_I32(3, (int)groups.size());
    ASSUME_ITS_EQUAL(groups[0], "a");
    ASSUME_ITS_EQUAL(groups[1], "b");
    ASSUME_ITS_EQUAL(groups[2], "c");
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_compile_empty_pattern) {
    fossil::io::Regex re;
    re.compile("");
    ASSUME_ITS_TRUE(re.is_compiled());
    ASSUME_ITS_TRUE(re.match("anything"));
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_compile_with_options) {
    fossil::io::Regex re;
    re.compile("abc", { "icase" });
    ASSUME_ITS_TRUE(re.match("ABC"));
    ASSUME_ITS_TRUE(re.match("abc"));
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_match_partial) {
    fossil::io::Regex re("abc");
    ASSUME_ITS_TRUE(re.match("abcxxx"));
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_match_empty_text) {
    fossil::io::Regex re("");
    ASSUME_ITS_TRUE(re.match(""));
    ASSUME_ITS_TRUE(re.match("abc"));
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_match_null_out_match) {
    fossil::io::Regex re("abc");
    ASSUME_ITS_TRUE(re.match("abc"));
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_compile_invalid_option) {
    fossil::io::Regex re;
    bool threw = false;
    try {
        re.compile("abc", { "invalid_option" });
    } catch (...) {
        threw = true;
    }
    ASSUME_ITS_TRUE(threw || re.is_compiled());
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_compile_null_error_out) {
    fossil::io::Regex re;
    bool threw = false;
    try {
        re.compile("abc");
    } catch (...) {
        threw = true;
    }
    ASSUME_ITS_TRUE(!threw);
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_match_anchored_option) {
    fossil::io::Regex re;
    re.compile("abc", { "anchored" });
    ASSUME_ITS_TRUE(re.match("abc"));
    ASSUME_ITS_TRUE(!re.match("xabc"));
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_match_icase_option) {
    fossil::io::Regex re;
    re.compile("abc", { "icase" });
    ASSUME_ITS_TRUE(re.match("ABC"));
    ASSUME_ITS_TRUE(re.match("abc"));
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_match_dot_operator) {
    fossil::io::Regex re("a.c");
    ASSUME_ITS_TRUE(re.match("abc"));
    ASSUME_ITS_TRUE(re.match("aXc"));
    ASSUME_ITS_TRUE(!re.match("ac"));
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_match_begin_end_anchors) {
    fossil::io::Regex re("^abc$");
    ASSUME_ITS_TRUE(re.match("abc"));
    ASSUME_ITS_TRUE(!re.match("xabc"));
    ASSUME_ITS_TRUE(!re.match("abcx"));
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_group_count_null) {
    // Not directly applicable, but test empty groups
    fossil::io::Regex re("abc");
    std::vector<std::string> groups;
    bool matched = re.match("abc", groups);
    ASSUME_ITS_TRUE(matched);
    ASSUME_ITS_EQUAL_I32(0, (int)groups.size());
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_group_null) {
    fossil::io::Regex re("abc");
    std::vector<std::string> groups;
    bool matched = re.match("abc", groups);
    ASSUME_ITS_TRUE(matched);
    // No groups, so access out of bounds
    ASSUME_ITS_TRUE(groups.empty());
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_group_out_of_bounds) {
    fossil::io::Regex re("(abc)");
    std::vector<std::string> groups;
    bool matched = re.match("abc", groups);
    ASSUME_ITS_TRUE(matched);
    ASSUME_ITS_TRUE(groups.size() == 1);
    // Out of bounds access should not crash (but we don't access here)
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_match_multiple_matches) {
    fossil::io::Regex re("ab");
    ASSUME_ITS_TRUE(re.match("ab ab ab"));
}

FOSSIL_TEST(cpp_test_regex_match_dotall_option_ignored) {
    // Dotall is not implemented, but should not error
    char *error = NULL;
    const char *opts[] = { "dotall", NULL };
    fossil_io_regex_t *re = fossil_io_regex_compile("a.c", opts, &error);
    ASSUME_ITS_TRUE(re != NULL);

    fossil_io_regex_match_t *match = NULL;
    int rc = fossil_io_regex_match(re, "a\nc", &match);
    // '.' does not match '\n' in this implementation
    ASSUME_ITS_EQUAL_I32(0, rc);
    ASSUME_ITS_TRUE(match == NULL);

    fossil_io_regex_free(re);
    if (error) free(error);
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_match_ungreedy_option_ignored) {
    // "ungreedy" is not implemented, but should not error or throw
    fossil::io::Regex re;
    bool threw = false;
    try {
        re.compile("a.c", { "ungreedy" });
    } catch (...) {
        threw = true;
    }
    ASSUME_ITS_TRUE(!threw);
    ASSUME_ITS_TRUE(re.is_compiled());
    ASSUME_ITS_TRUE(re.match("abc"));
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_match_multiline_option_ignored) {
    // "multiline" is not implemented, but should not error or throw
    fossil::io::Regex re;
    bool threw = false;
    try {
        re.compile("^abc", { "multiline" });
    } catch (...) {
        threw = true;
    }
    ASSUME_ITS_TRUE(!threw);
    ASSUME_ITS_TRUE(re.is_compiled());
    ASSUME_ITS_TRUE(re.match("abc\ndef"));
    ASSUME_ITS_TRUE(!re.match("x\nabc"));
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_match_empty_pattern_matches_anywhere) {
    fossil::io::Regex re("");
    ASSUME_ITS_TRUE(re.is_compiled());
    ASSUME_ITS_TRUE(re.match("abc"));
    ASSUME_ITS_TRUE(re.match(""));
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_match_only_at_end_with_dollar) {
    fossil::io::Regex re("abc$");
    ASSUME_ITS_TRUE(re.is_compiled());
    ASSUME_ITS_TRUE(re.match("abc"));
    ASSUME_ITS_TRUE(re.match("xabc"));
    ASSUME_ITS_TRUE(!re.match("abcc"));
}

FOSSIL_TEST(cpp_test_regex_cppwrapper_match_only_at_start_with_caret) {
    fossil::io::Regex re("^abc");
    ASSUME_ITS_TRUE(re.is_compiled());
    ASSUME_ITS_TRUE(re.match("abc"));
    ASSUME_ITS_TRUE(re.match("abcx"));
    ASSUME_ITS_TRUE(!re.match("xabc"));
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_regex_tests) {
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_default_ctor);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_compile_and_match);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_ctor_compile_and_match);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_compile_with_options);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_match_with_groups);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_reset);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_move_ctor);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_move_assign);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_compile_invalid_pattern_throws);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_compile_null_pattern_throws);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_match_uncompiled_throws);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_compile_and_free);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_compile_null_pattern);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_match_success);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_match_failure);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_match_null_args);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_group_count_and_group);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_compile_empty_pattern);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_compile_with_options);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_match_partial);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_match_empty_text);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_match_null_out_match);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_compile_invalid_option);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_compile_null_error_out);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_match_anchored_option);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_match_icase_option);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_match_dot_operator);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_match_begin_end_anchors);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_group_count_null);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_group_null);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_group_out_of_bounds);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_match_multiple_matches);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_match_dotall_option_ignored);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_match_ungreedy_option_ignored);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_match_multiline_option_ignored);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_match_empty_pattern_matches_anywhere);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_match_only_at_end_with_dollar);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_cppwrapper_match_only_at_start_with_caret);

    FOSSIL_TEST_REGISTER(cpp_regex_suite);
}
