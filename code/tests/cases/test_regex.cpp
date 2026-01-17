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

FOSSIL_TEST(cpp_test_regex_compile_and_free) {
    fossil::io::Regex re;
    ASSUME_ITS_TRUE(!re.is_compiled());
    re.compile("abc");
    ASSUME_ITS_TRUE(re.is_compiled());
    re.reset();
    ASSUME_ITS_TRUE(!re.is_compiled());
}

FOSSIL_TEST(cpp_test_regex_compile_null_pattern) {
    fossil::io::Regex re;
    try {
        re.compile(std::string());
        ASSUME_ITS_TRUE(re.is_compiled());
    } catch (const std::exception &ex) {
        ASSUME_ITS_TRUE(true);
    }
}

FOSSIL_TEST(cpp_test_regex_match_success) {
    fossil::io::Regex re("hello");
    ASSUME_ITS_TRUE(re.is_compiled());
    ASSUME_ITS_TRUE(re.match("hello"));
}

FOSSIL_TEST(cpp_test_regex_match_failure) {
    fossil::io::Regex re("abc");
    ASSUME_ITS_TRUE(re.is_compiled());
    ASSUME_ITS_TRUE(!re.match("def"));
}

FOSSIL_TEST(cpp_test_regex_match_null_args) {
    fossil::io::Regex re;
    try {
        re.match("abc");
        ASSUME_ITS_TRUE(false);
    } catch (const std::logic_error &) {
        ASSUME_ITS_TRUE(true);
    }
    try {
        fossil::io::Regex re2("abc");
        re2.match("");
        ASSUME_ITS_TRUE(true);
    } catch (...) {
        ASSUME_ITS_TRUE(false);
    }
}

FOSSIL_TEST(cpp_test_regex_group_count_and_group) {
    fossil::io::Regex re("abc");
    ASSUME_ITS_TRUE(re.is_compiled());
    std::vector<std::string> groups;
    bool matched = re.match("abc", groups);
    ASSUME_ITS_TRUE(matched);
    ASSUME_ITS_EQUAL_I32(0, (int)groups.size()); // No groups supported in minimal compiler
}

FOSSIL_TEST(cpp_test_regex_compile_empty_pattern) {
    fossil::io::Regex re;
    re.compile("");
    ASSUME_ITS_TRUE(re.is_compiled());
}

FOSSIL_TEST(cpp_test_regex_compile_with_options) {
    std::vector<std::string> opts = { "icase", "multiline" };
    fossil::io::Regex re("abc", opts);
    ASSUME_ITS_TRUE(re.is_compiled());
}

FOSSIL_TEST(cpp_test_regex_match_partial) {
    fossil::io::Regex re("abc");
    ASSUME_ITS_TRUE(re.is_compiled());
    ASSUME_ITS_TRUE(re.match("abcdef"));
}

FOSSIL_TEST(cpp_test_regex_match_empty_text) {
    fossil::io::Regex re("abc");
    ASSUME_ITS_TRUE(re.is_compiled());
    ASSUME_ITS_TRUE(!re.match(""));
}

FOSSIL_TEST(cpp_test_regex_match_null_out_match) {
    fossil::io::Regex re("abc");
    ASSUME_ITS_TRUE(re.is_compiled());
    ASSUME_ITS_TRUE(re.match("abc"));
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_regex_tests) {
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_compile_and_free);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_compile_null_pattern);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_match_success);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_match_failure);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_match_null_args);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_group_count_and_group);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_compile_empty_pattern);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_compile_with_options);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_match_partial);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_match_empty_text);
    FOSSIL_TEST_ADD(cpp_regex_suite, cpp_test_regex_match_null_out_match);

    FOSSIL_TEST_REGISTER(cpp_regex_suite);
}
