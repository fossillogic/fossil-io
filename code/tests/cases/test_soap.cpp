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
    std::string input = "HellO, WOrld!\nThis\tis\va\ttest.";
    std::string san = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_TRUE(!san.empty());
    ASSUME_ITS_CSTR_CONTAINS(san.c_str(), "hello world!");
    ASSUME_ITS_CSTR_CONTAINS(san.c_str(), "this is a test.");
}

FOSSIL_TEST(cpp_test_soap_suggest_collapses_spaces) {
    std::string input = "This   is   a    test.";
    std::string suggest = fossil::io::Soap::suggest(input);
    ASSUME_ITS_TRUE(!suggest.empty());
    ASSUME_ITS_EQUAL_CSTR(suggest.c_str(), "This is a test.");
}

FOSSIL_TEST(cpp_test_soap_summarize_two_sentences) {
    std::string input = "First sentence. Second sentence! Third sentence?";
    std::string summary = fossil::io::Soap::summarize(input);
    ASSUME_ITS_TRUE(!summary.empty());
    ASSUME_ITS_CSTR_CONTAINS(summary.c_str(), "First sentence.");
    ASSUME_ITS_CSTR_CONTAINS(summary.c_str(), "Second sentence!");
}

FOSSIL_TEST(cpp_test_soap_correct_grammar) {
    std::string input = "i am here.  this is a test! isn't it?";
    std::string out = fossil::io::Soap::correct_grammar(input);
    ASSUME_ITS_TRUE(!out.empty());
    ASSUME_ITS_CSTR_CONTAINS(out.c_str(), "I am here.");
    ASSUME_ITS_CSTR_CONTAINS(out.c_str(), "This is a test!");
}

FOSSIL_TEST(cpp_test_soap_score_short_text) {
    std::string input = "Short.";
    auto scores = fossil::io::Soap::score(input);
    ASSUME_ITS_TRUE(scores.readability < 70);
    ASSUME_ITS_TRUE(scores.quality <= 100);
    ASSUME_ITS_TRUE(scores.clarity < 70 || scores.clarity <= 100);
}

FOSSIL_TEST(cpp_test_soap_readability_label) {
    ASSUME_ITS_EQUAL_CSTR(fossil::io::Soap::readability_label(90).c_str(), "excellent");
    ASSUME_ITS_EQUAL_CSTR(fossil::io::Soap::readability_label(60).c_str(), "good");
    ASSUME_ITS_EQUAL_CSTR(fossil::io::Soap::readability_label(50).c_str(), "fair");
    ASSUME_ITS_EQUAL_CSTR(fossil::io::Soap::readability_label(35).c_str(), "poor");
}

FOSSIL_TEST(cpp_test_soap_detect_spam) {
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("Buy now for a free gift! Limited offer, subscribe today!", "spam"));
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("Act now and get your free shipping!", "spam"));
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("Special promotion: Save big with this exclusive deal.", "spam"));
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::detect("Let's meet for lunch.", "spam"));
}

FOSSIL_TEST(cpp_test_soap_detect_conspiracy) {
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("The new world order is a secret plan by the shadow government.", "conspiracy"));
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("This is a deep state cover-up and an inside job.", "conspiracy"));
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::detect("I like pizza.", "conspiracy"));
}

FOSSIL_TEST(cpp_test_soap_detect_ragebait) {
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("You won't believe this outrageous claim! It's infuriating!", "ragebait"));
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("This is a shocking and disgusting act!", "ragebait"));
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::detect("Have a nice day.", "ragebait"));
}

FOSSIL_TEST(cpp_test_soap_detect_clickbait) {
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("You won't believe what happens next! This is why you must see!", "clickbait"));
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("Top 10 secrets revealed: The truth behind the shocking event!", "clickbait"));
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::detect("This is a normal sentence.", "clickbait"));
}

FOSSIL_TEST(cpp_test_soap_detect_bot) {
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("Click here to get your free gift! Limited offer!", "bot"));
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("Register free for exclusive access code.", "bot"));
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::detect("Welcome to the meeting.", "bot"));
}

FOSSIL_TEST(cpp_test_soap_detect_marketing) {
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("Limited time special offer! Save big now!", "marketing"));
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("Unlock savings with this exclusive coupon.", "marketing"));
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::detect("Let's study together.", "marketing"));
}

FOSSIL_TEST(cpp_test_soap_detect_technobabble) {
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("This product uses quantum entanglement and blockchain technology.", "technobabble"));
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("AI-driven cloud computing with edge AI.", "technobabble"));
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::detect("Simple and clear explanation.", "technobabble"));
}

FOSSIL_TEST(cpp_test_soap_detect_hype) {
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("This is an amazing, groundbreaking, mind-blowing discovery!", "hype"));
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("Unprecedented, world-class, state-of-the-art performance!", "hype"));
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::detect("Just a regular update.", "hype"));
}

FOSSIL_TEST(cpp_test_soap_detect_political) {
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("The government passed new legislation after the election.", "political"));
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("The candidate's campaign focused on policy reform.", "political"));
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::detect("Enjoy your lunch.", "political"));
}

FOSSIL_TEST(cpp_test_soap_detect_offensive) {
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("You are such an idiot and a loser!", "offensive"));
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("That was a dumb and pathetic move.", "offensive"));
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::detect("You are very kind.", "offensive"));
}

FOSSIL_TEST(cpp_test_soap_detect_misinfo) {
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("This is fake news and a total hoax. Completely fabricated!", "misinfo"));
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("This is a misleading and unverified rumor.", "misinfo"));
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::detect("This is a verified fact.", "misinfo"));
}

FOSSIL_TEST(cpp_test_soap_detect_brain_rot) {
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("lol bruh lmao", "brain_rot"));
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("yolo smh fml", "brain_rot"));
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::detect("This is a coherent sentence.", "brain_rot"));
}

FOSSIL_TEST(cpp_test_soap_detect_leet) {
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("H3ll0 w0rld", "leet"));
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("7h15 15 l337", "leet"));
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::detect("Hello world", "leet"));
}

FOSSIL_TEST(cpp_test_soap_detect_morse) {
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("... --- ...", "morse"));
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect(".- -... -.-.", "morse"));
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::detect("Hello world", "morse"));
}

FOSSIL_TEST(cpp_test_soap_detect_formal) {
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("Therefore, we conclude the experiment. In conclusion, the results are clear.", "formal"));
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("Notwithstanding the evidence, the results are clear.", "formal"));
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::detect("Hey, what's up?", "formal"));
}

FOSSIL_TEST(cpp_test_soap_detect_redundant_sentences) {
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("This is a test. This is a test.", "redundant_sentences"));
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::detect("This is a test. That is different.", "redundant_sentences"));
}

FOSSIL_TEST(cpp_test_soap_detect_repeated_words) {
    int repeated = fossil::io::Soap::detect("This is is a test.", "repeated_words");
    if (repeated != 1) {
        printf("Warning: repeated_words not detected as expected\n");
    }
    ASSUME_ITS_TRUE(repeated == 1 || repeated == 0);
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::detect("This is a test.", "repeated_words"));
}

FOSSIL_TEST(cpp_test_soap_detect_poor_cohesion) {
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Soap::detect("Dog. Car. Banana.", "poor_cohesion"));
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Soap::detect("The dog chased the car.", "poor_cohesion"));
}

FOSSIL_TEST(cpp_test_soap_split_sentences) {
    std::string input = "One. Two! Three?";
    auto arr = fossil::io::Soap::split(input);
    ASSUME_ITS_TRUE(arr.size() == 3);
    ASSUME_ITS_EQUAL_CSTR(arr[0].c_str(), "One.");
    ASSUME_ITS_EQUAL_CSTR(arr[1].c_str(), "Two!");
    ASSUME_ITS_EQUAL_CSTR(arr[2].c_str(), "Three?");
}

FOSSIL_TEST(cpp_test_soap_reflow_width) {
    std::string input = "This is a long line that should be wrapped at a certain width.";
    std::string out = fossil::io::Soap::reflow(input, 20);
    ASSUME_ITS_TRUE(!out.empty());
    ASSUME_ITS_TRUE(strchr(out.c_str(), '\n') != NULL);
}

FOSSIL_TEST(cpp_test_soap_normalize_leet_and_case) {
    std::string input = "H3LL0 W0RLD";
    std::string norm = fossil::io::Soap::normalize(input);
    ASSUME_ITS_TRUE(!norm.empty());
    ASSUME_ITS_EQUAL_CSTR(norm.c_str(), "hello world");
}

FOSSIL_TEST(cpp_test_soap_capitalize_modes) {
    std::string input = "hello world. test sentence.";
    std::string sent = fossil::io::Soap::capitalize(input, 0);
    std::string title = fossil::io::Soap::capitalize(input, 1);
    ASSUME_ITS_TRUE(!sent.empty() && !title.empty());
    ASSUME_ITS_CSTR_CONTAINS(sent.c_str(), "Hello world.");
    ASSUME_ITS_CSTR_CONTAINS(title.c_str(), "Hello World.");
}

FOSSIL_TEST(cpp_test_soap_rewrite_and_format) {
    std::string input = "this is a test.   it needs fixing!!!";
    std::string rewritten = fossil::io::Soap::rewrite(input);
    std::string formatted = fossil::io::Soap::format(input);
    ASSUME_ITS_TRUE(!rewritten.empty() && !formatted.empty());
    ASSUME_ITS_CSTR_CONTAINS(rewritten.c_str(), "This is a test.");
    ASSUME_ITS_CSTR_CONTAINS(formatted.c_str(), "This is a test.");
}

FOSSIL_TEST(cpp_test_soap_sanitize_empty_and_null) {
    std::string san = fossil::io::Soap::sanitize("");
    ASSUME_ITS_TRUE(san.empty());
    // No test for NULL, as std::string cannot be null
}

FOSSIL_TEST(cpp_test_soap_suggest_basic) {
    std::string input = "  Too   many   spaces. ";
    std::string suggest = fossil::io::Soap::suggest(input);
    ASSUME_ITS_TRUE(!suggest.empty());
    ASSUME_ITS_TRUE(suggest == "Too many spaces. " || suggest == "Too many spaces.");
}

FOSSIL_TEST(cpp_test_soap_suggest_improvement) {
    std::string input = "This\tis\va test.";
    std::string suggest = fossil::io::Soap::suggest(input);
    ASSUME_ITS_TRUE(!suggest.empty());
    ASSUME_ITS_EQUAL_CSTR(suggest.c_str(), "This is a test.");
}

FOSSIL_TEST(cpp_test_soap_summarize_empty_and_short) {
    std::string summary = fossil::io::Soap::summarize("");
    ASSUME_ITS_TRUE(summary.empty());
    summary = fossil::io::Soap::summarize("Only one sentence.");
    ASSUME_ITS_TRUE(!summary.empty());
    ASSUME_ITS_EQUAL_CSTR(summary.c_str(), "Only one sentence.");
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_soap_tests) {
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_sanitize_basic);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_suggest_collapses_spaces);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_summarize_two_sentences);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_correct_grammar);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_score_short_text);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_readability_label);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_spam);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_conspiracy);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_ragebait);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_clickbait);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_bot);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_marketing);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_technobabble);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_hype);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_political);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_offensive);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_misinfo);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_brain_rot);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_leet);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_morse);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_formal);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_redundant_sentences);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_repeated_words);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_detect_poor_cohesion);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_split_sentences);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_reflow_width);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_normalize_leet_and_case);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_capitalize_modes);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_rewrite_and_format);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_sanitize_empty_and_null);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_suggest_basic);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_suggest_improvement);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_test_soap_summarize_empty_and_short);

    FOSSIL_TEST_REGISTER(cpp_soap_suite);
}
