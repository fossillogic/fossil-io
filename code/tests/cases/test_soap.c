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
FOSSIL_SUITE(c_soap_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_soap_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_soap_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_soap_detect_ragebait) {
    ASSUME_ITS_TRUE(
        fossil_io_soap_detect(
            "This is outrageous and infuriating!",
            "ragebait",
            "sentence"
        )
    );

    ASSUME_ITS_FALSE(
        fossil_io_soap_detect(
            "This is a calm and reasonable statement.",
            "ragebait",
            "sentence"
        )
    );
}

FOSSIL_TEST(c_test_soap_detect_clickbait) {
    ASSUME_ITS_TRUE(
        fossil_io_soap_detect(
            "Top 10 secrets revealed!",
            "clickbait",
            "headline"
        )
    );

    ASSUME_ITS_FALSE(
        fossil_io_soap_detect(
            "Here is an informational article.",
            "clickbait",
            "paragraph"
        )
    );
}

FOSSIL_TEST(c_test_soap_detect_spam) {
    ASSUME_ITS_TRUE(
        fossil_io_soap_detect(
            "Earn money fast with this exclusive deal!",
            "spam",
            "marketing"
        )
    );

    ASSUME_ITS_FALSE(
        fossil_io_soap_detect(
            "Let’s meet tomorrow at noon.",
            "spam",
            "conversation"
        )
    );
}

FOSSIL_TEST(c_test_soap_detect_offensive) {
    ASSUME_ITS_TRUE(
        fossil_io_soap_detect(
            "You are an idiot.",
            "offensive",
            "sentence"
        )
    );

    ASSUME_ITS_FALSE(
        fossil_io_soap_detect(
            "You did a great job.",
            "offensive",
            "sentence"
        )
    );
}

FOSSIL_TEST(c_test_soap_analyze_grammar_style_clean) {
    fossil_io_soap_grammar_style_t r =
        fossil_io_soap_analyze_grammar_style(
            "She has gone to the store."
        );

    ASSUME_ITS_TRUE(r.grammar_ok);
    ASSUME_ITS_TRUE(r.passive_voice_pct == 0);
    ASSUME_ITS_TRUE(r.style != NULL);
}

FOSSIL_TEST(c_test_soap_analyze_grammar_style_passive) {
    fossil_io_soap_grammar_style_t r =
        fossil_io_soap_analyze_grammar_style(
            "The ball was thrown by the boy."
        );

    ASSUME_ITS_FALSE(r.grammar_ok == 0); /* grammar may still be valid */
    ASSUME_ITS_TRUE(r.passive_voice_pct > 0);
}

FOSSIL_TEST(c_test_soap_correct_grammar) {
    char *out =
        fossil_io_soap_correct_grammar(
            "I should of went to the party."
        );

    ASSUME_ITS_TRUE(out != NULL);
    ASSUME_ITS_TRUE(strstr(out, "should have") != NULL);
    free(out);
}

FOSSIL_TEST(c_test_soap_correct_grammar_no_change) {
    const char *in = "She has gone to the store.";
    char *out = fossil_io_soap_correct_grammar(in);

    ASSUME_ITS_TRUE(out != NULL);
    ASSUME_ITS_EQUAL_CSTR(out, in);
    free(out);
}

FOSSIL_TEST(c_test_soap_score_simple_text) {
    fossil_io_soap_scores_t s =
        fossil_io_soap_score(
            "The cat sat on the mat."
        );

    ASSUME_ITS_TRUE(s.readability >= 60);
    ASSUME_ITS_TRUE(s.clarity >= 60);
    ASSUME_ITS_TRUE(s.quality >= 60);
}

FOSSIL_TEST(c_test_soap_score_complex_text) {
    fossil_io_soap_scores_t s =
        fossil_io_soap_score(
            "Notwithstanding the aforementioned stipulations, "
            "the contractual obligations remain binding."
        );

    ASSUME_ITS_TRUE(s.readability <= 50);
}

FOSSIL_TEST(c_test_soap_sanitize) {
    char *out =
        fossil_io_soap_sanitize(
            "You are such a rot-brain!"
        );

    ASSUME_ITS_TRUE(out != NULL);
    ASSUME_ITS_TRUE(strchr(out, '*') != NULL);
    free(out);
}

FOSSIL_TEST(c_test_soap_suggest) {
    char *out =
        fossil_io_soap_suggest(
            "He has rizz."
        );

    ASSUME_ITS_TRUE(out != NULL);
    ASSUME_ITS_TRUE(strlen(out) > 0);
    free(out);
}

FOSSIL_TEST(c_test_soap_split_sentences) {
    char **parts =
        fossil_io_soap_split(
            "Hello world. This is Fossil.",
            "sentence"
        );

    ASSUME_ITS_TRUE(parts != NULL);
    ASSUME_ITS_TRUE(parts[0] && parts[1]);

    free(parts[0]);
    free(parts[1]);
    free(parts);
}

FOSSIL_TEST(c_test_soap_normalize) {
    char *out =
        fossil_io_soap_normalize(
            "This   is   spaced   out."
        );

    ASSUME_ITS_TRUE(out != NULL);
    ASSUME_ITS_EQUAL_CSTR(out, "This is spaced out.");
    free(out);
}

FOSSIL_TEST(c_test_soap_reflow) {
    char *out =
        fossil_io_soap_reflow(
            "This is a long sentence that should wrap cleanly.",
            20
        );

    ASSUME_ITS_TRUE(out != NULL);
    ASSUME_ITS_TRUE(strchr(out, '\n') != NULL);
    free(out);
}

FOSSIL_TEST(c_test_soap_capitalize_upper) {
    char *out =
        fossil_io_soap_capitalize(
            "hello world",
            2 /* uppercase */
        );

    ASSUME_ITS_TRUE(out != NULL);
    ASSUME_ITS_EQUAL_CSTR(out, "HELLO WORLD");
    free(out);
}

FOSSIL_TEST(c_test_soap_process_basic) {
    fossil_io_soap_options_t opt = {0};

    opt.detect_ragebait   = 1;
    opt.detect_offensive  = 1;
    opt.include_summary   = 1;
    opt.apply_normalization = 1;

    char *out =
        fossil_io_soap_process(
            "This is outrageous and infuriating!",
            "sentence",
            &opt
        );

    ASSUME_ITS_TRUE(out != NULL);
    ASSUME_ITS_TRUE(strlen(out) > 0);
    free(out);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_soap_tests) {
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_detect_ragebait);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_detect_clickbait);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_detect_spam);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_detect_offensive);

    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_analyze_grammar_style_clean);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_analyze_grammar_style_passive);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_correct_grammar);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_correct_grammar_no_change);

    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_score_simple_text);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_score_complex_text);

    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_sanitize);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_suggest);

    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_split_sentences);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_normalize);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_reflow);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_capitalize_upper);

    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_process_basic);

    FOSSIL_TEST_REGISTER(c_soap_suite);
}
