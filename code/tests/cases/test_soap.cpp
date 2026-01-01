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
FOSSIL_SUITE(cpp_soap_suite);

// Setup function for the test suite
FOSSIL_SETUP(cpp_soap_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_soap_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

// ============================================================================
// Sanitize, Analysis, & Summary
// ============================================================================

FOSSIL_TEST(cpp_test_soap_sanitize_basic) {
    // Should remove control chars, normalize leet, lowercase
    std::string input = "Hello\x01W0rld!\n";
    std::string san = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR("hello world!\n", san.c_str());
}

FOSSIL_TEST(cpp_test_soap_sanitize_empty_and_null) {
    std::string san = fossil::io::Soap::sanitize("");
    ASSUME_ITS_EQUAL_CSTR("", san.c_str());

    // NULL input: C++ wrapper expects std::string, so skip this test
}

FOSSIL_TEST(cpp_test_soap_suggest_basic) {
    std::string sug = fossil::io::Soap::suggest("This   is   a   test.");
    ASSUME_ITS_EQUAL_CSTR("This is a test.", sug.c_str());
}

FOSSIL_TEST(cpp_test_soap_suggest_improvement) {
    std::string sug = fossil::io::Soap::suggest("bad grammar here");
    ASSUME_ITS_TRUE(!sug.empty());
}

FOSSIL_TEST(cpp_test_soap_summarize_basic) {
    std::string sum = fossil::io::Soap::summarize("First sentence. Second sentence. Third sentence.");
    ASSUME_ITS_EQUAL_CSTR("First sentence  Second sentence ", sum.c_str());
}

FOSSIL_TEST(cpp_test_soap_summarize_short_text) {
    std::string sum = fossil::io::Soap::summarize("Short text.");
    ASSUME_ITS_EQUAL_CSTR("Short text ", sum.c_str());
}

// ============================================================================
// Grammar & Style Analysis
// ============================================================================

FOSSIL_TEST(cpp_test_soap_analyze_grammar_style) {
    auto r = fossil::io::Soap::analyze_grammar_style("This is a test.");
    ASSUME_ITS_TRUE(r.grammar_ok);
    ASSUME_ITS_TRUE(r.passive_voice_pct >= 0 && r.passive_voice_pct <= 100);
    ASSUME_ITS_TRUE(!r.style.empty());
}

FOSSIL_TEST(cpp_test_soap_analyze_grammar_style_emotional) {
    auto r = fossil::io::Soap::analyze_grammar_style("Wow! This is amazing!");
    ASSUME_ITS_TRUE(r.style.find("emotional") != std::string::npos);
}

FOSSIL_TEST(cpp_test_soap_analyze_grammar_style_formal) {
    auto r = fossil::io::Soap::analyze_grammar_style("Therefore, we conclude the experiment;");
    ASSUME_ITS_TRUE(r.style.find("formal") != std::string::npos);
}

FOSSIL_TEST(cpp_test_soap_correct_grammar) {
    std::string input = "this is a test";
    std::string corr = fossil::io::Soap::correct_grammar(input);
    ASSUME_ITS_TRUE(!corr.empty());
    ASSUME_ITS_TRUE(corr[0] == 'T');
}

FOSSIL_TEST(cpp_test_soap_correct_grammar_terminal_punctuation) {
    std::string input = "this is a test without punctuation";
    std::string corr = fossil::io::Soap::correct_grammar(input);
    ASSUME_ITS_TRUE(!corr.empty());
    size_t len = corr.length();
    ASSUME_ITS_TRUE(corr[len-1] == '.' || corr[len-1] == '!' || corr[len-1] == '?');
}

// ============================================================================
// Readability, Clarity, & Quality Analysis
// ============================================================================

FOSSIL_TEST(cpp_test_soap_score_short_text) {
    auto s = fossil::io::Soap::score("short");
    ASSUME_ITS_TRUE(s.readability < 70);
    ASSUME_ITS_TRUE(s.clarity >= 70);
    ASSUME_ITS_TRUE(s.quality >= 70);
}

FOSSIL_TEST(cpp_test_soap_score_long_text) {
    std::string txt = "This is a sufficiently long text to test the readability scoring system. "
                      "It should not be penalized for being too short.";
    auto s = fossil::io::Soap::score(txt);
    ASSUME_ITS_TRUE(s.readability >= 70);
}

FOSSIL_TEST(cpp_test_soap_score_multiline) {
    auto s = fossil::io::Soap::score("Line one.\nLine two.");
    ASSUME_ITS_TRUE(s.clarity > 70);
}

FOSSIL_TEST(cpp_test_soap_readability_label) {
    ASSUME_ITS_EQUAL_CSTR("excellent", fossil::io::Soap::readability_label(90).c_str());
    ASSUME_ITS_EQUAL_CSTR("good", fossil::io::Soap::readability_label(70).c_str());
    ASSUME_ITS_EQUAL_CSTR("fair", fossil::io::Soap::readability_label(50).c_str());
    ASSUME_ITS_EQUAL_CSTR("poor", fossil::io::Soap::readability_label(30).c_str());
}

FOSSIL_TEST(cpp_test_soap_readability_label_boundaries) {
    ASSUME_ITS_EQUAL_CSTR("excellent", fossil::io::Soap::readability_label(81).c_str());
    ASSUME_ITS_EQUAL_CSTR("good", fossil::io::Soap::readability_label(61).c_str());
    ASSUME_ITS_EQUAL_CSTR("fair", fossil::io::Soap::readability_label(41).c_str());
    ASSUME_ITS_EQUAL_CSTR("poor", fossil::io::Soap::readability_label(0).c_str());
}

// ============================================================================
// Detection, Normalization, Split, Reflow, Capitalization
// ============================================================================

FOSSIL_TEST(cpp_test_soap_detect_spam) {
    bool found = fossil::io::Soap::detect("Buy now for a free gift!", "spam");
    ASSUME_ITS_TRUE(found);
}

FOSSIL_TEST(cpp_test_soap_detect_clickbait) {
    bool found = fossil::io::Soap::detect("You won't believe what happened next!", "clickbait");
    ASSUME_ITS_TRUE(found);
}

FOSSIL_TEST(cpp_test_soap_detect_no_match) {
    bool found = fossil::io::Soap::detect("This is a normal sentence.", "spam");
    ASSUME_ITS_TRUE(!found);
}

FOSSIL_TEST(cpp_test_soap_detect_case_insensitive) {
    bool found = fossil::io::Soap::detect("BUY NOW for a FREE gift!", "spam");
    ASSUME_ITS_TRUE(found);
}

FOSSIL_TEST(cpp_test_soap_detect_bot_pattern) {
    bool found = fossil::io::Soap::detect("Click here to subscribe!", "bot");
    ASSUME_ITS_TRUE(found);
}

FOSSIL_TEST(cpp_test_soap_normalize_leet_and_case) {
    std::string input = "H3LL0 W0RLD";
    std::string norm = fossil::io::Soap::normalize(input);
    ASSUME_ITS_EQUAL_CSTR("hello world", norm.c_str());
}

FOSSIL_TEST(cpp_test_soap_normalize_null_and_empty) {
    // NULL input: C++ wrapper expects std::string, so skip this test

    std::string norm = fossil::io::Soap::normalize("");
    ASSUME_ITS_EQUAL_CSTR("", norm.c_str());
}

FOSSIL_TEST(cpp_test_soap_split_and_reflow) {
    auto split = fossil::io::Soap::split("Sentence one. Sentence two.");
    ASSUME_ITS_TRUE(split.size() >= 2);

    std::string reflowed = fossil::io::Soap::reflow("This is a long line that should wrap.", 10);
    ASSUME_ITS_TRUE(!reflowed.empty());
}

FOSSIL_TEST(cpp_test_soap_split_empty) {
    auto split = fossil::io::Soap::split("");
    ASSUME_ITS_TRUE(split.empty());
}

FOSSIL_TEST(cpp_test_soap_reflow_shorter_than_width) {
    std::string reflowed = fossil::io::Soap::reflow("Short.", 20);
    ASSUME_ITS_EQUAL_CSTR("Short.", reflowed.c_str());
}

FOSSIL_TEST(cpp_test_soap_capitalize_modes) {
    std::string sent = fossil::io::Soap::capitalize("hello world. this is fossil.", 0);
    ASSUME_ITS_TRUE(!sent.empty());
    std::string title = fossil::io::Soap::capitalize("hello world", 1);
    ASSUME_ITS_TRUE(!title.empty());
}

FOSSIL_TEST(cpp_test_soap_capitalize_empty) {
    std::string sent = fossil::io::Soap::capitalize("", 0);
    ASSUME_ITS_EQUAL_CSTR("", sent.c_str());
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_soap_tests) {
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_sanitize_basic);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_sanitize_empty_and_null);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_suggest_basic);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_suggest_improvement);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_summarize_basic);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_summarize_short_text);

    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_analyze_grammar_style);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_analyze_grammar_style_emotional);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_analyze_grammar_style_formal);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_correct_grammar);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_correct_grammar_terminal_punctuation);

    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_score_short_text);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_score_long_text);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_score_multiline);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_readability_label);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_readability_label_boundaries);

    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_spam);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_clickbait);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_no_match);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_case_insensitive);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_bot_pattern);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_normalize_leet_and_case);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_normalize_null_and_empty);

    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_split_and_reflow);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_split_empty);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_reflow_shorter_than_width);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_capitalize_modes);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_capitalize_empty);

    FOSSIL_TEST_REGISTER(cpp_soap_suite);
}
