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

FOSSIL_TEST(cpp_soap_options_default_zeroed) {
    Soap::Options opt;

    const unsigned char *raw =
        reinterpret_cast<const unsigned char*>(&opt.c_options);

    for (size_t i = 0; i < sizeof(opt.c_options); ++i) {
        ASSUME_ITS_TRUE(raw[i] == 0);
    }
}

FOSSIL_TEST(cpp_soap_options_builder_full) {
    Soap::Options opt;

    opt.detect_quality(true)
       .detect_clarity(true)
       .detect_tone(true)
       .detect_readability(true)
       .detect_brain_rot(true)
       .detect_spam(true)
       .detect_ragebait(true)
       .detect_clickbait(true)
       .detect_bot(true)
       .detect_marketing(true)
       .detect_technobabble(true)
       .detect_hype(true)
       .detect_political(true)
       .detect_conspiracy(true)
       .detect_offensive(true)
       .detect_propaganda(true)
       .detect_misinformation(true)
       .detect_formal(true)
       .detect_casual(true)
       .detect_sarcasm(true)
       .detect_neutral(true)
       .detect_aggressive(true)
       .detect_emotional(true)
       .detect_passive_aggressive(true)
       .analyze_grammar(true)
       .analyze_style(true)
       .analyze_passive_voice(true)
       .analyze_sentence_complexity(true)
       .analyze_paragraph_cohesion(true)
       .analyze_redundancy(true)
       .analyze_word_repetition(true)
       .apply_sanitization(true)
       .apply_normalization(true)
       .apply_grammar_correction(true)
       .include_summary(true)
       .include_scores(true)
       .include_flags(true)
       .include_style(true)
       .include_debug(true);

    ASSUME_ITS_TRUE(opt.c_options.detect_ragebait);
    ASSUME_ITS_TRUE(opt.c_options.analyze_grammar);
    ASSUME_ITS_TRUE(opt.c_options.include_debug);
}

FOSSIL_TEST(cpp_soap_detect_known_id) {
    ASSUME_ITS_TRUE(
        Soap::detect(
            "This is outrageous and infuriating!",
            "ragebait",
            "sentence"
        )
    );
}

FOSSIL_TEST(cpp_soap_detect_unknown_id_safe) {
    ASSUME_ITS_FALSE(
        Soap::detect(
            "Normal text.",
            "nonexistent_detector",
            "sentence"
        )
    );
}

FOSSIL_TEST(cpp_soap_detect_empty_text_safe) {
    ASSUME_ITS_FALSE(
        Soap::detect("", "spam", "sentence")
    );
}

FOSSIL_TEST(cpp_soap_analyze_grammar_style_basic) {
    auto r = Soap::analyze_grammar_style(
        "The ball was thrown by the boy."
    );

    ASSUME_ITS_TRUE(r.passive_voice_pct >= 0);
    ASSUME_ITS_TRUE(r.passive_voice_pct <= 100);
    ASSUME_ITS_TRUE(!r.style.empty());
}

FOSSIL_TEST(cpp_soap_analyze_grammar_style_empty_safe) {
    auto r = Soap::analyze_grammar_style("");

    ASSUME_ITS_TRUE(r.passive_voice_pct >= 0);
}

FOSSIL_TEST(cpp_soap_correct_grammar_basic) {
    std::string out =
        Soap::correct_grammar(
            "I should of gone to the store."
        );

    ASSUME_ITS_TRUE(out.size() > 0);
}

FOSSIL_TEST(cpp_soap_correct_grammar_empty_safe) {
    std::string out = Soap::correct_grammar("");
    ASSUME_ITS_EQUAL_CSTR(out.c_str(), "");
}

FOSSIL_TEST(cpp_soap_correct_grammar_basic) {
    std::string out =
        Soap::correct_grammar(
            "I should of gone to the store."
        );

    ASSUME_ITS_TRUE(out.size() > 0);
}

FOSSIL_TEST(cpp_soap_correct_grammar_empty_safe) {
    std::string out = Soap::correct_grammar("");
    ASSUME_ITS_EQUAL_CSTR(out.c_str(), "");
}

FOSSIL_TEST(cpp_soap_score_basic) {
    auto s = Soap::score("The cat sat on the mat.");

    ASSUME_ITS_TRUE(s.readability >= 0);
    ASSUME_ITS_TRUE(s.readability <= 100);
    ASSUME_ITS_TRUE(s.clarity >= 0);
    ASSUME_ITS_TRUE(s.quality >= 0);
}

FOSSIL_TEST(cpp_soap_readability_label_bounds) {
    ASSUME_ITS_TRUE(
        Soap::readability_label(0).size() > 0
    );

    ASSUME_ITS_TRUE(
        Soap::readability_label(100).size() > 0
    );
}

FOSSIL_TEST(cpp_soap_sanitize_basic) {
    std::string out =
        Soap::sanitize("You are an idiot!");

    ASSUME_ITS_TRUE(out.size() > 0);
}

FOSSIL_TEST(cpp_soap_suggest_basic) {
    std::string out =
        Soap::suggest("He got that rizz.");

    ASSUME_ITS_TRUE(out.size() > 0);
}

FOSSIL_TEST(cpp_soap_summarize_basic) {
    std::string out =
        Soap::summarize(
            "This paragraph explains something in great detail "
            "over many sentences that are unnecessary."
        );

    ASSUME_ITS_TRUE(out.size() > 0);
}

FOSSIL_TEST(cpp_soap_split_sentences) {
    auto parts =
        Soap::split(
            "Hello world. This is Fossil.",
            "sentence"
        );

    ASSUME_ITS_TRUE(parts.size() >= 2);
}

FOSSIL_TEST(cpp_soap_split_empty_safe) {
    auto parts = Soap::split("", "sentence");
    ASSUME_ITS_TRUE(parts.empty());
}

FOSSIL_TEST(cpp_soap_reflow_basic) {
    std::string out =
        Soap::reflow(
            "This is a long sentence that should wrap.",
            15
        );

    ASSUME_ITS_TRUE(out.find('\n') != std::string::npos);
}

FOSSIL_TEST(cpp_soap_normalize_basic) {
    std::string out =
        Soap::normalize("This   is   spaced.");

    ASSUME_ITS_EQUAL_CSTR(out.c_str(), "This is spaced.");
}

FOSSIL_TEST(cpp_soap_capitalize_upper) {
    std::string out =
        Soap::capitalize("hello world", 2);

    ASSUME_ITS_EQUAL_CSTR(out.c_str(), "HELLO WORLD");
}

FOSSIL_TEST(cpp_soap_process_no_options) {
    std::string out =
        Soap::process(
            "Simple text.",
            "sentence",
            nullptr
        );

    ASSUME_ITS_TRUE(out.size() > 0);
}

FOSSIL_TEST(cpp_soap_process_with_options) {
    Soap::Options opt;
    opt.detect_spam(true)
       .include_flags(true)
       .include_summary(true);

    std::string out =
        Soap::process(
            "Buy now, limited time offer!",
            "marketing",
            &opt
        );

    ASSUME_ITS_TRUE(out.size() > 0);
}

FOSSIL_TEST(cpp_soap_all_empty_inputs_safe) {
    Soap::Options opt;

    ASSUME_ITS_TRUE(
        Soap::sanitize("").size() == 0
    );

    ASSUME_ITS_TRUE(
        Soap::process("", "", &opt).size() >= 0
    );
}



// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_soap_tests) {
    // detect tests
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_options_default_zeroed);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_options_builder_full);

    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_detect_known_id);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_detect_unknown_id_safe);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_detect_empty_text_safe);

    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_analyze_grammar_style_basic);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_analyze_grammar_style_empty_safe);

    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_correct_grammar_basic);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_correct_grammar_empty_safe);

    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_score_basic);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_readability_label_bounds);

    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_sanitize_basic);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_suggest_basic);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_summarize_basic);

    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_split_sentences);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_split_empty_safe);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_reflow_basic);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_normalize_basic);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_capitalize_upper);

    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_process_no_options);
    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_process_with_options);

    FOSSIL_TEST_ADD(cpp_soap_suite, cpp_soap_all_empty_inputs_safe);

    FOSSIL_TEST_REGISTER(cpp_soap_suite);
}
