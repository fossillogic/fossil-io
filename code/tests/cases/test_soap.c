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

// ============================================================================
// Sanitize, Analysis, & Summary
// ============================================================================

FOSSIL_TEST(c_test_soap_sanitize_basic) {
    const char *input = "Hello\x01World!\nThis is a test.";
    char *sanitized = fossil_io_soap_sanitize(input);
    ASSUME_ITS_TRUE(sanitized != NULL);
    if (sanitized != NULL) {
        ASSUME_ITS_CSTR_CONTAINS(sanitized, "hello world!\nthis is a test.");
        free(sanitized);
    }
}

FOSSIL_TEST(c_test_soap_sanitize_control_chars) {
    const char *input = "Hello\x02World\x03!";
    char *sanitized = fossil_io_soap_sanitize(input);
    ASSUME_ITS_TRUE(sanitized != NULL);
    if (sanitized != NULL) {
        ASSUME_ITS_CSTR_CONTAINS(sanitized, "hello world !");
        free(sanitized);
    }
}

FOSSIL_TEST(c_test_soap_sanitize_mixed_case) {
    const char *input = "ThIs Is A TeSt!";
    char *sanitized = fossil_io_soap_sanitize(input);
    ASSUME_ITS_TRUE(sanitized != NULL);
    if (sanitized != NULL) {
        ASSUME_ITS_EQUAL_CSTR(sanitized, "this is a test!");
        free(sanitized);
    }
}

FOSSIL_TEST(c_test_soap_sanitize_preserves_newline) {
    const char *input = "Hello\nWorld!";
    char *sanitized = fossil_io_soap_sanitize(input);
    ASSUME_ITS_TRUE(sanitized != NULL);
    if (sanitized != NULL) {
        ASSUME_ITS_EQUAL_CSTR(sanitized, "hello\nworld!");
        free(sanitized);
    }
}

FOSSIL_TEST(c_test_soap_sanitize_only_control_chars) {
    const char *input = "\x01\x02\x03";
    char *sanitized = fossil_io_soap_sanitize(input);
    ASSUME_ITS_TRUE(sanitized != NULL);
    if (sanitized != NULL) {
        ASSUME_ITS_EQUAL_CSTR(sanitized, "");
        free(sanitized);
    }
}

FOSSIL_TEST(c_test_soap_sanitize_long_sentence) {
    const char *input = "This is a very long sentence with multiple clauses, some control characters like \x04 and \x05, and mixed CASE to test the sanitizer's ability to clean and normalize the text properly.";
    char *sanitized = fossil_io_soap_sanitize(input);
    ASSUME_ITS_TRUE(sanitized != NULL);
    if (sanitized != NULL) {
        ASSUME_ITS_CSTR_CONTAINS(sanitized, "this is a very long sentence with multiple clauses some control characters like and  and mixed case to test the sanitizer's ability to clean and normalize the text properly.");
        free(sanitized);
    }
}

FOSSIL_TEST(c_test_soap_sanitize_paragraph) {
    const char *input = "First line with control\x06.\nSecond line with MIXED case and more control\x07.";
    char *sanitized = fossil_io_soap_sanitize(input);
    ASSUME_ITS_TRUE(sanitized != NULL);
    if (sanitized != NULL) {
        ASSUME_ITS_CSTR_CONTAINS(sanitized, "first line with control .\nsecond line with mixed case and more control .");
        free(sanitized);
    }
}

FOSSIL_TEST(c_test_soap_sanitize_multiple_control_chars) {
    const char *input = "This\x08is\x09a\x0Atest\x0Bwith\x0Cmany\x0Dcontrol\x0Echars.";
    char *sanitized = fossil_io_soap_sanitize(input);
    ASSUME_ITS_TRUE(sanitized != NULL);
    if (sanitized != NULL) {
        ASSUME_ITS_CSTR_CONTAINS(sanitized, "this is test with manyontrolhars.");
        free(sanitized);
    }
}

FOSSIL_TEST(c_test_soap_suggest_spaces) {
    const char *input = "This   is   a    test.";
    char *suggestion = fossil_io_soap_suggest(input);
    ASSUME_ITS_TRUE(suggestion != NULL);
    if (suggestion != NULL) {
        ASSUME_ITS_CSTR_CONTAINS(suggestion, "This is a test.");
        free(suggestion);
    }
}

FOSSIL_TEST(c_test_soap_summarize_short) {
    const char *input = "First sentence. Second sentence. Third sentence.";
    char *summary = fossil_io_soap_summarize(input);
    ASSUME_ITS_TRUE(summary != NULL);
    if (summary != NULL) {
        ASSUME_ITS_CSTR_CONTAINS(summary, "First sentence. Second sentence.");
        free(summary);
    }
}

FOSSIL_TEST(c_test_soap_analyze_grammar_style_passive) {
    const char *input = "The ball was thrown by John. It was caught.";
    fossil_io_soap_grammar_style_t result = fossil_io_soap_analyze_grammar_style(input);
    ASSUME_ITS_TRUE(result.passive_voice_pct >= 0);
    FOSSIL_TEST_ASSUME(strcmp(result.style, "neutral") == 0 || strcmp(result.style, "formal") == 0 || strcmp(result.style, "emotional") == 0, "Expected style to be neutral, formal, or emotional");
}

FOSSIL_TEST(c_test_soap_correct_grammar_basic) {
    const char *input = "this is a test. it works!";
    char *corrected = fossil_io_soap_correct_grammar(input);
    ASSUME_ITS_TRUE(corrected != NULL);
    if (corrected != NULL) {
        ASSUME_ITS_CSTR_CONTAINS(corrected, "This is a test. It works!");
        free(corrected);
    }
}

FOSSIL_TEST(c_test_soap_score_short_text) {
    const char *input = "Hi.";
    fossil_io_soap_scores_t scores = fossil_io_soap_score(input);
    ASSUME_ITS_TRUE(scores.readability < 70);
    ASSUME_ITS_TRUE(scores.clarity < 70);
    ASSUME_ITS_TRUE(scores.quality <= 100);
}

FOSSIL_TEST(c_test_soap_readability_label) {
    ASSUME_ITS_EQUAL_CSTR(fossil_io_soap_readability_label(95), "outstanding");
    ASSUME_ITS_EQUAL_CSTR(fossil_io_soap_readability_label(85), "excellent");
    ASSUME_ITS_EQUAL_CSTR(fossil_io_soap_readability_label(70), "very good");
    ASSUME_ITS_EQUAL_CSTR(fossil_io_soap_readability_label(60), "good");
    ASSUME_ITS_EQUAL_CSTR(fossil_io_soap_readability_label(50), "fair");
    ASSUME_ITS_EQUAL_CSTR(fossil_io_soap_readability_label(35), "poor");
    ASSUME_ITS_EQUAL_CSTR(fossil_io_soap_readability_label(20), "very poor");
    ASSUME_ITS_EQUAL_CSTR(fossil_io_soap_readability_label(10), "unreadable");
}

FOSSIL_TEST(c_test_soap_detect_spam) {
    const char *input = "Buy now! Limited offer!";
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect(input, "spam"), 1);
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect("Hello world.", "spam"), 0);
}

FOSSIL_TEST(c_test_soap_detect_conspiracy) {
    const char *input = "The shadow government has a secret plan.";
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect(input, "conspiracy"), 1);
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect("Hello world.", "conspiracy"), 0);
}

FOSSIL_TEST(c_test_soap_detect_ragebait) {
    const char *input = "You won't believe this outrageous claim!";
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect(input, "ragebait"), 1);
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect("Calm discussion.", "ragebait"), 0);
}

FOSSIL_TEST(c_test_soap_detect_clickbait) {
    const char *input = "You won't believe what happened next!";
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect(input, "clickbait"), 1);
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect("Ordinary news.", "clickbait"), 0);
}

FOSSIL_TEST(c_test_soap_detect_bot) {
    const char *input = "Subscribe for a free gift!";
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect(input, "bot"), 1);
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect("Welcome to the forum.", "bot"), 0);
}

FOSSIL_TEST(c_test_soap_detect_marketing) {
    const char *input = "Limited time offer! Save big!";
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect(input, "marketing"), 1);
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect("General information.", "marketing"), 0);
}

FOSSIL_TEST(c_test_soap_detect_technobabble) {
    const char *input = "Our AI-driven blockchain solution uses quantum entanglement.";
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect(input, "technobabble"), 1);
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect("Simple explanation.", "technobabble"), 0);
}

FOSSIL_TEST(c_test_soap_detect_hype) {
    const char *input = "This is a groundbreaking, mind-blowing discovery!";
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect(input, "hype"), 1);
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect("Routine update.", "hype"), 0);
}

FOSSIL_TEST(c_test_soap_detect_political) {
    const char *input = "The government passed new legislation.";
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect(input, "political"), 1);
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect("No politics here.", "political"), 0);
}

FOSSIL_TEST(c_test_soap_detect_offensive) {
    const char *input = "You are such an idiot!";
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect(input, "offensive"), 1);
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect("Polite conversation.", "offensive"), 0);
}

FOSSIL_TEST(c_test_soap_detect_misinfo) {
    const char *input = "This is fake news and a hoax.";
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect(input, "misinfo"), 1);
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect("Verified facts.", "misinfo"), 0);
}

FOSSIL_TEST(c_test_soap_detect_brain_rot) {
    const char *input = "asdfasdfasdf";
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect(input, "brain_rot"), 0);
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect("Normal sentence.", "brain_rot"), 0);
}

FOSSIL_TEST(c_test_soap_detect_formal) {
    const char *input = "Therefore, we must proceed accordingly.";
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect(input, "formal"), 1);
    ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect("Hey, what's up?", "formal"), 0);
}

FOSSIL_TEST(c_test_soap_split_sentences) {
    const char *input = "First. Second! Third?";
    char **split = fossil_io_soap_split(input);
    ASSUME_ITS_TRUE(split != NULL);
    if (split != NULL) {
        ASSUME_ITS_TRUE(split[0] != NULL && split[1] != NULL);
        for (int i = 0; split[i] != NULL; ++i) free(split[i]);
        free(split);
    }
}

FOSSIL_TEST(c_test_soap_reflow_width) {
    const char *input = "This is a long line that should be wrapped at a certain width.";
    char *reflowed = fossil_io_soap_reflow(input, 20);
    ASSUME_ITS_TRUE(reflowed != NULL);
    if (reflowed != NULL) {
        ASSUME_ITS_CSTR_CONTAINS(reflowed, "\n");
        free(reflowed);
    }
}

FOSSIL_TEST(c_test_soap_capitalize_sentence_and_title) {
    const char *input = "this is a test. another sentence.";
    char *sentence_case = fossil_io_soap_capitalize(input, 0);
    char *title_case = fossil_io_soap_capitalize(input, 1);
    ASSUME_ITS_TRUE(sentence_case != NULL && title_case != NULL);
    if (sentence_case != NULL) {
        ASSUME_ITS_CSTR_CONTAINS(sentence_case, "This is a test.");
        free(sentence_case);
    }
    if (title_case != NULL) {
        ASSUME_ITS_CSTR_CONTAINS(title_case, "This Is A Test.");
        free(title_case);
    }
}

FOSSIL_TEST(c_test_soap_rewrite_pipeline) {
    const char *input = "tHiS is a tESt.   it works!!!";
    char *rewritten = fossil_io_soap_rewrite(input);
    ASSUME_ITS_TRUE(rewritten != NULL);
    if (rewritten != NULL) {
        ASSUME_ITS_CSTR_CONTAINS(rewritten, "This is a test.");
        free(rewritten);
    }
}

FOSSIL_TEST(c_test_soap_format_pretty) {
    const char *input = "this is a test.   it should be formatted nicely.";
    char *formatted = fossil_io_soap_format(input);
    ASSUME_ITS_TRUE(formatted != NULL);
    if (formatted != NULL) {
        ASSUME_ITS_CSTR_CONTAINS(formatted, "This is a test.");
        free(formatted);
    }
}

FOSSIL_TEST(c_test_soap_declutter_camel_case) {
    const char *input = "ThisIsCamelCase and PascalCaseTest";
    char *decluttered = fossil_io_soap_declutter(input);
    ASSUME_ITS_TRUE(decluttered != NULL);
    if (decluttered != NULL) {
        ASSUME_ITS_CSTR_CONTAINS(decluttered, "This Is Camel Case");
        free(decluttered);
    }
}

FOSSIL_TEST(c_test_soap_punctuate_repeated) {
    const char *input = "Wow!!! Really???";
    char *punctuated = fossil_io_soap_punctuate(input);
    ASSUME_ITS_TRUE(punctuated != NULL);
    if (punctuated != NULL) {
        ASSUME_ITS_CSTR_CONTAINS(punctuated, "Wow! Really?");
        free(punctuated);
    }
}

FOSSIL_TEST(c_test_soap_process_full_pipeline) {
    const char *input = "tHiS is a tESt.   it works!!!";
    char *processed = fossil_io_soap_process(input);
    ASSUME_ITS_TRUE(processed != NULL);
    if (processed != NULL) {
        ASSUME_ITS_CSTR_CONTAINS(processed, "This is a test.");
        free(processed);
    }
}

// //
// FOSSIL_TEST(c_test_soap_sanitize_null_input) {
//     char *sanitized = fossil_io_soap_sanitize(NULL);
//     ASSUME_ITS_TRUE(sanitized == NULL);
// }

// FOSSIL_TEST(c_test_soap_sanitize_empty_string) {
//     char *sanitized = fossil_io_soap_sanitize("");
//     ASSUME_ITS_TRUE(sanitized != NULL);
//     if (sanitized != NULL) {
//         ASSUME_ITS_EQUAL_CSTR(sanitized, "");
//         free(sanitized);
//     }
// }

// FOSSIL_TEST(c_test_soap_suggest_null_input) {
//     char *suggestion = fossil_io_soap_suggest(NULL);
//     ASSUME_ITS_TRUE(suggestion == NULL);
// }

// FOSSIL_TEST(c_test_soap_summarize_empty_string) {
//     char *summary = fossil_io_soap_summarize("");
//     ASSUME_ITS_TRUE(summary != NULL);
//     if (summary != NULL) {
//         ASSUME_ITS_EQUAL_CSTR(summary, "");
//         free(summary);
//     }
// }

// FOSSIL_TEST(c_test_soap_analyze_grammar_style_empty) {
//     fossil_io_soap_grammar_style_t result = fossil_io_soap_analyze_grammar_style("");
//     ASSUME_ITS_TRUE(result.passive_voice_pct == 0);
//     ASSUME_ITS_EQUAL_CSTR(result.style, "neutral");
// }

// FOSSIL_TEST(c_test_soap_correct_grammar_null_input) {
//     char *corrected = fossil_io_soap_correct_grammar(NULL);
//     ASSUME_ITS_TRUE(corrected == NULL);
// }

// FOSSIL_TEST(c_test_soap_score_null_input) {
//     fossil_io_soap_scores_t scores = fossil_io_soap_score(NULL);
//     ASSUME_ITS_TRUE(scores.readability == 100);
//     ASSUME_ITS_TRUE(scores.clarity == 100);
//     ASSUME_ITS_TRUE(scores.quality == 100);
// }

// FOSSIL_TEST(c_test_soap_readability_label_edge) {
//     ASSUME_ITS_EQUAL_CSTR(fossil_io_soap_readability_label(100), "outstanding");
//     ASSUME_ITS_EQUAL_CSTR(fossil_io_soap_readability_label(0), "unreadable");
// }

// FOSSIL_TEST(c_test_soap_detect_null_input) {
//     ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect(NULL, "spam"), 0);
//     ASSUME_ITS_EQUAL_I32(fossil_io_soap_detect("Hello world.", NULL), 0);
// }

// FOSSIL_TEST(c_test_soap_split_empty_string) {
//     char **split = fossil_io_soap_split("");
//     ASSUME_ITS_TRUE(split != NULL);
//     if (split != NULL) {
//         ASSUME_ITS_TRUE(split[0] == NULL);
//         free(split);
//     }
// }

// FOSSIL_TEST(c_test_soap_reflow_zero_width) {
//     const char *input = "This is a test.";
//     char *reflowed = fossil_io_soap_reflow(input, 0);
//     ASSUME_ITS_TRUE(reflowed != NULL);
//     if (reflowed != NULL) {
//         ASSUME_ITS_EQUAL_CSTR(reflowed, input);
//         free(reflowed);
//     }
// }

// FOSSIL_TEST(c_test_soap_normalize_null_input) {
//     char *normalized = fossil_io_soap_normalize(NULL);
//     ASSUME_ITS_TRUE(normalized == NULL);
// }

// FOSSIL_TEST(c_test_soap_capitalize_null_input) {
//     char *sentence_case = fossil_io_soap_capitalize(NULL, 0);
//     char *title_case = fossil_io_soap_capitalize(NULL, 1);
//     ASSUME_ITS_TRUE(sentence_case == NULL && title_case == NULL);
// }

// FOSSIL_TEST(c_test_soap_rewrite_null_input) {
//     char *rewritten = fossil_io_soap_rewrite(NULL);
//     ASSUME_ITS_TRUE(rewritten == NULL);
// }

// FOSSIL_TEST(c_test_soap_format_null_input) {
//     char *formatted = fossil_io_soap_format(NULL);
//     ASSUME_ITS_TRUE(formatted == NULL);
// }

// FOSSIL_TEST(c_test_soap_declutter_null_input) {
//     char *decluttered = fossil_io_soap_declutter(NULL);
//     ASSUME_ITS_TRUE(decluttered == NULL);
// }

// FOSSIL_TEST(c_test_soap_punctuate_null_input) {
//     char *punctuated = fossil_io_soap_punctuate(NULL);
//     ASSUME_ITS_TRUE(punctuated == NULL);
// }

// FOSSIL_TEST(c_test_soap_process_null_input) {
//     char *processed = fossil_io_soap_process(NULL);
//     ASSUME_ITS_TRUE(processed == NULL);
// }

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_soap_tests) {
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_sanitize_basic);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_sanitize_control_chars);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_sanitize_mixed_case);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_sanitize_preserves_newline);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_sanitize_only_control_chars);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_sanitize_long_sentence);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_sanitize_paragraph);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_sanitize_multiple_control_chars);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_suggest_spaces);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_summarize_short);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_analyze_grammar_style_passive);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_correct_grammar_basic);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_score_short_text);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_readability_label);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_detect_spam);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_detect_conspiracy);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_detect_ragebait);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_detect_clickbait);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_detect_bot);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_detect_marketing);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_detect_technobabble);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_detect_hype);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_detect_political);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_detect_offensive);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_detect_misinfo);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_detect_brain_rot);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_detect_formal);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_split_sentences);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_reflow_width);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_capitalize_sentence_and_title);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_rewrite_pipeline);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_format_pretty);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_declutter_camel_case);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_punctuate_repeated);
    FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_process_full_pipeline);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_sanitize_null_input);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_sanitize_empty_string);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_suggest_null_input);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_summarize_empty_string);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_analyze_grammar_style_empty);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_correct_grammar_null_input);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_score_null_input);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_readability_label_edge);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_detect_null_input);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_split_empty_string);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_reflow_zero_width);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_normalize_null_input);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_capitalize_null_input);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_rewrite_null_input);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_format_null_input);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_declutter_null_input);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_punctuate_null_input);
    // FOSSIL_TEST_ADD(c_soap_suite, c_test_soap_process_null_input);

    FOSSIL_TEST_REGISTER(c_soap_suite);
}
