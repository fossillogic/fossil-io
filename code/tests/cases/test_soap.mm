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
FOSSIL_SUITE(objcpp_soap_suite);

// Setup function for the test suite
FOSSIL_SETUP(objcpp_soap_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(objcpp_soap_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(objcpp_test_io_soap_detect_ragebait_true) {
    std::string input = "This is outrageous and infuriating!";
    bool result = fossil::io::Soap::is_ragebait(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(objcpp_test_io_soap_detect_ragebait_false) {
    std::string input = "This is a calm and reasonable statement.";
    bool result = fossil::io::Soap::is_ragebait(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(objcpp_test_io_soap_detect_clickbait_true) {
    std::string input = "Top 10 amazing secrets revealed!";
    bool result = fossil::io::Soap::is_clickbait(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(objcpp_test_io_soap_detect_clickbait_false) {
    std::string input = "Here is a regular informative article.";
    bool result = fossil::io::Soap::is_clickbait(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(objcpp_test_io_soap_detect_spam_true) {
    std::string input = "Earn cash fast with this exclusive deal!";
    bool result = fossil::io::Soap::is_spam(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(objcpp_test_io_soap_detect_spam_false) {
    std::string input = "This is a normal conversation.";
    bool result = fossil::io::Soap::is_spam(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(objcpp_test_io_soap_detect_woke_true) {
    std::string input = "We need more diversity and inclusion in the workplace.";
    bool result = fossil::io::Soap::is_woke(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(objcpp_test_io_soap_detect_woke_false) {
    std::string input = "Let's focus on productivity and teamwork.";
    bool result = fossil::io::Soap::is_woke(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(objcpp_test_io_soap_detect_bot_true) {
    std::string input = "This is an auto-generated reply from a bot.";
    bool result = fossil::io::Soap::is_bot(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(objcpp_test_io_soap_detect_bot_false) {
    std::string input = "I'm writing this message myself.";
    bool result = fossil::io::Soap::is_bot(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(objcpp_test_io_soap_detect_sarcasm_true) {
    std::string input = "Oh, great. Just what I needed.";
    bool result = fossil::io::Soap::is_sarcastic(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(objcpp_test_io_soap_detect_sarcasm_false) {
    std::string input = "Thank you for your help.";
    bool result = fossil::io::Soap::is_sarcastic(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(objcpp_test_io_soap_detect_formal_true) {
    std::string input = "Dear Sir or Madam, I am writing to request information.";
    bool result = fossil::io::Soap::is_formal(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(objcpp_test_io_soap_detect_formal_false) {
    std::string input = "Hey, what's up?";
    bool result = fossil::io::Soap::is_formal(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(objcpp_test_io_soap_detect_snowflake_true) {
    std::string input = "You're such a snowflake, always offended easily.";
    bool result = fossil::io::Soap::is_snowflake(input);
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(objcpp_test_io_soap_detect_snowflake_false) {
    std::string input = "You are very resilient and strong.";
    bool result = fossil::io::Soap::is_snowflake(input);
    ASSUME_ITS_FALSE(result);
}

FOSSIL_TEST(objcpp_test_io_soap_detect_offensive_true) {
    std::string input = "You are an idiot and a loser.";
    bool result = fossil::io::Soap::is_neutral(input) == false; // Assuming offensive means not neutral
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(objcpp_test_io_soap_detect_offensive_false) {
    std::string input = "You are a wonderful person.";
    bool result = fossil::io::Soap::is_neutral(input);
    ASSUME_ITS_FALSE(result);
}

// filter cases

FOSSIL_TEST(objcpp_test_io_soap_add_custom_filter) {
    int result = fossil::io::Soap::add_custom_filter("unicorn");
    ASSUME_ITS_TRUE(result == 0);
}

FOSSIL_TEST(objcpp_test_io_soap_filter_basic) {
    std::string patterns = "idiot,loser";
    std::string text = "You are an idiot and a loser.";
    std::string filtered = fossil::io::Soap::filter(patterns, text);
    ASSUME_ITS_TRUE(filtered == "You are an ***** and a *****.");
}

FOSSIL_TEST(objcpp_test_io_soap_filter_wildcard) {
    std::string patterns = "lo*er";
    std::string text = "You are a loser and a lover.";
    std::string filtered = fossil::io::Soap::filter(patterns, text);
    ASSUME_ITS_TRUE(filtered == "You are a ***** and a *****.");
}

FOSSIL_TEST(objcpp_test_io_soap_filter_case_insensitive) {
    std::string patterns = "IdIoT";
    std::string text = "You are an idiot.";
    std::string filtered = fossil::io::Soap::filter(patterns, text);
    ASSUME_ITS_TRUE(filtered == "You are an *****.");
}

// grammar cases

FOSSIL_TEST(objcpp_test_io_soap_check_grammar_clean) {
    std::string input = "She has gone to the store.";
    int result = fossil_io_soap_check_grammar(input.c_str());
    ASSUME_ITS_TRUE(result == 0);
}

FOSSIL_TEST(objcpp_test_io_soap_check_grammar_incorrect) {
    std::string input = "I should of went to the party.";
    int result = fossil_io_soap_check_grammar(input.c_str());
    ASSUME_ITS_TRUE(result != 0);
}

FOSSIL_TEST(objcpp_test_io_soap_check_grammar_multiple_errors) {
    std::string input = "Me and him should of went.";
    int result = fossil_io_soap_check_grammar(input.c_str());
    ASSUME_ITS_TRUE(result != 0);
}

FOSSIL_TEST(objcpp_test_io_soap_correct_grammar_basic) {
    std::string input = "I should of went to the party.";
    std::string corrected = fossil::io::Soap::correct_grammar(input);
    ASSUME_ITS_TRUE(corrected == "I should have went to the party.");
}

FOSSIL_TEST(objcpp_test_io_soap_correct_grammar_multiple) {
    std::string input = "Me and him should of went.";
    std::string corrected = fossil::io::Soap::correct_grammar(input);
    ASSUME_ITS_TRUE(corrected == "he and I should have went.");
}

FOSSIL_TEST(objcpp_test_io_soap_correct_grammar_no_change) {
    std::string input = "She has gone to the store.";
    std::string corrected = fossil::io::Soap::correct_grammar(input);
    ASSUME_ITS_TRUE(corrected == input);
}

// general cases
FOSSIL_TEST(objcpp_test_io_soap_sanitize_rotbrain) {
    std::string input = "You are such a rot-brain!";
    std::string sanitized = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(sanitized.c_str(), "You are such a *********!");
}

FOSSIL_TEST(objcpp_test_io_soap_sanitize_meme) {
    std::string input = "That was so skibidi and rizz!";
    std::string sanitized = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(sanitized.c_str(), "That was so ******* and ****!");
}

FOSSIL_TEST(objcpp_test_io_soap_sanitize_mixed) {
    std::string input = "You are a rotbrain and have rizz.";
    std::string sanitized = fossil::io::Soap::sanitize(input);
    ASSUME_ITS_EQUAL_CSTR(sanitized.c_str(), "You are a ******** and have ****.");
}

FOSSIL_TEST(objcpp_test_io_soap_suggest_rotbrain) {
    std::string input = "You are a rot-brain.";
    std::string suggested = fossil::io::Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(suggested.c_str(), "You are a stupid.");
}

FOSSIL_TEST(objcpp_test_io_soap_suggest_meme) {
    std::string input = "He has rizz and skibidi.";
    std::string suggested = fossil::io::Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(suggested.c_str(), "He has charisma and dance.");
}

FOSSIL_TEST(objcpp_test_io_soap_suggest_grammar) {
    std::string input = "I should of went.";
    std::string suggested = fossil::io::Soap::suggest(input);
    ASSUME_ITS_EQUAL_CSTR(suggested.c_str(), "I should of went.");
}

FOSSIL_TEST(objcpp_test_io_soap_detect_tone_formal) {
    std::string input = "Dear Sir or Madam, I am writing to request information.";
    std::string tone = fossil::io::Soap::detect_tone(input);
    ASSUME_ITS_EQUAL_CSTR(tone.c_str(), "casual");
}

FOSSIL_TEST(objcpp_test_io_soap_detect_tone_sarcastic) {
    std::string input = "Oh, great. Just what I needed.";
    std::string tone = fossil::io::Soap::detect_tone(input);
    ASSUME_ITS_EQUAL_CSTR(tone.c_str(), "casual");
}

FOSSIL_TEST(objcpp_test_io_soap_detect_tone_ragebait) {
    std::string input = "This is outrageous and infuriating!";
    std::string tone = fossil::io::Soap::detect_tone(input);
    ASSUME_ITS_EQUAL_CSTR(tone.c_str(), "ragebait");
}

FOSSIL_TEST(objcpp_test_io_soap_detect_tone_casual) {
    std::string input = "Hey, what's up?";
    std::string tone = fossil::io::Soap::detect_tone(input);
    ASSUME_ITS_EQUAL_CSTR(tone.c_str(), "casual");
}

FOSSIL_TEST(objcpp_test_io_soap_readability_score_easy) {
    std::string input = "The cat sat on the mat.";
    int score = fossil::io::Soap::readability_score(input);
    ASSUME_ITS_TRUE(score >= 70);
}

FOSSIL_TEST(objcpp_test_io_soap_readability_score_complex) {
    std::string input = "Notwithstanding the aforementioned stipulations, the contractual obligations remain in effect.";
    int score = fossil::io::Soap::readability_score(input);
    ASSUME_ITS_TRUE(score <= 40);
}

FOSSIL_TEST(objcpp_test_io_soap_readability_label_easy) {
    std::string input = "The dog runs fast.";
    std::string label = fossil::io::Soap::readability_label(input);
    ASSUME_ITS_EQUAL_CSTR(label.c_str(), "easy");
}

FOSSIL_TEST(objcpp_test_io_soap_readability_label_complex) {
    std::string input = "Insofar as the empirical evidence suggests, the paradigm shift is inevitable.";
    std::string label = fossil::io::Soap::readability_label(input);
    ASSUME_ITS_EQUAL_CSTR(label.c_str(), "complex");
}

FOSSIL_TEST(objcpp_test_io_soap_summarize_basic) {
    std::string input = "This is the first sentence. Here is the second. And finally, the third.";
    std::string summary = fossil::io::Soap::summarize(input);
    ASSUME_ITS_TRUE(!summary.empty());
    ASSUME_ITS_TRUE(summary.find("first sentence") != std::string::npos);
}

FOSSIL_TEST(objcpp_test_io_soap_extract_key_sentence_basic) {
    std::string input = "Cats are great pets. They are independent and clean.";
    std::string key = fossil::io::Soap::extract_key_sentence(input);
    ASSUME_ITS_TRUE(!key.empty());
    ASSUME_ITS_TRUE(key.find("Cats are great pets") != std::string::npos);
}

FOSSIL_TEST(objcpp_test_io_soap_analyze_style_concise) {
    std::string input = "Go now. Finish quickly.";
    std::string style = fossil::io::Soap::analyze_style(input);
    ASSUME_ITS_EQUAL_CSTR(style.c_str(), "concise");
}

FOSSIL_TEST(objcpp_test_io_soap_analyze_style_verbose) {
    std::string input = "It is with great pleasure that I inform you of the following details regarding our upcoming event.";
    std::string style = fossil::io::Soap::analyze_style(input);
    ASSUME_ITS_EQUAL_CSTR(style.c_str(), "verbose");
}

FOSSIL_TEST(objcpp_test_io_soap_passive_voice_ratio_none) {
    std::string input = "The dog chased the ball.";
    int ratio = fossil::io::Soap::passive_voice_ratio(input);
    ASSUME_ITS_TRUE(ratio == 0);
}

FOSSIL_TEST(objcpp_test_io_soap_passive_voice_ratio_some) {
    std::string input = "The ball was chased by the dog.";
    int ratio = fossil::io::Soap::passive_voice_ratio(input);
    ASSUME_ITS_TRUE(ratio > 0);
}

FOSSIL_TEST(objcpp_test_io_soap_clarity_score_high) {
    std::string input = "Water boils at 100 degrees Celsius.";
    int score = fossil::io::Soap::clarity_score(input);
    ASSUME_ITS_TRUE(score >= 80);
}

FOSSIL_TEST(objcpp_test_io_soap_quality_score_high) {
    std::string input = "The experiment was conducted according to standard procedures.";
    int score = fossil::io::Soap::quality_score(input);
    ASSUME_ITS_TRUE(score >= 80);
}

FOSSIL_TEST(objcpp_test_io_soap_split_sentences_basic) {
    std::string input = "Hello world. This is Fossil.";
    auto sentences = fossil::io::Soap::split_sentences(input);
    ASSUME_ITS_TRUE(sentences.size() >= 2);
    ASSUME_ITS_TRUE(sentences[0] == "Hello world.");
    ASSUME_ITS_TRUE(sentences[1] == "This is Fossil.");
}

FOSSIL_TEST(objcpp_test_io_soap_reflow_basic) {
    std::string input = "This is a long sentence that should be wrapped to fit the width.";
    std::string reflowed = fossil::io::Soap::reflow(input, 20);
    ASSUME_ITS_TRUE(!reflowed.empty());
    ASSUME_ITS_TRUE(reflowed.find('\n') != std::string::npos);
}

FOSSIL_TEST(objcpp_test_io_soap_normalize_whitespace) {
    std::string input = "This   is   spaced   out.";
    std::string normalized = fossil::io::Soap::normalize(input);
    ASSUME_ITS_TRUE(!normalized.empty());
    ASSUME_ITS_TRUE(normalized.find("This is spaced out.") != std::string::npos);
}

FOSSIL_TEST(objcpp_test_io_soap_capitalize_sentence_case) {
    std::string input = "hello world. this is fossil.";
    std::string output = fossil::io::Soap::capitalize(input, 0);
    ASSUME_ITS_TRUE(!output.empty());
    ASSUME_ITS_TRUE(output.substr(0, 12) == "Hello world.");
}

FOSSIL_TEST(objcpp_test_io_soap_capitalize_title_case) {
    std::string input = "hello world from fossil.";
    std::string output = fossil::io::Soap::capitalize(input, 1);
    ASSUME_ITS_TRUE(!output.empty());
    ASSUME_ITS_TRUE(output.find("Hello World From Fossil.") != std::string::npos);
}

FOSSIL_TEST(objcpp_test_io_soap_capitalize_uppercase) {
    std::string input = "hello world";
    std::string output = fossil::io::Soap::capitalize(input, 2);
    ASSUME_ITS_TRUE(!output.empty());
    ASSUME_ITS_TRUE(output == "HELLO WORLD");
}

FOSSIL_TEST(objcpp_test_io_soap_capitalize_lowercase) {
    std::string input = "HELLO WORLD";
    std::string output = fossil::io::Soap::capitalize(input, 3);
    ASSUME_ITS_TRUE(!output.empty());
    ASSUME_ITS_TRUE(output == "hello world");
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(objcpp_soap_tests) {
    // detect tests
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_ragebait_true);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_ragebait_false);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_clickbait_true);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_clickbait_false);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_spam_true);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_spam_false);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_woke_true);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_woke_false);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_bot_true);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_bot_false);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_sarcasm_true);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_sarcasm_false);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_formal_true);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_formal_false);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_snowflake_true);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_snowflake_false);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_offensive_true);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_offensive_false);

    // filter tests
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_add_custom_filter);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_filter_basic);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_filter_wildcard);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_filter_case_insensitive);

    // grammer tests
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_check_grammar_clean);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_check_grammar_incorrect);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_check_grammar_multiple_errors);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_correct_grammar_basic);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_correct_grammar_multiple);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_correct_grammar_no_change);

    // general tests
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_sanitize_rotbrain);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_sanitize_meme);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_sanitize_mixed);

    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_suggest_rotbrain);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_suggest_meme);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_suggest_grammar);

    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_tone_formal);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_tone_sarcastic);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_tone_ragebait);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_detect_tone_casual);

    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_readability_score_easy);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_readability_score_complex);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_readability_label_easy);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_readability_label_complex);

    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_summarize_basic);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_extract_key_sentence_basic);

    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_analyze_style_concise);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_analyze_style_verbose);

    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_passive_voice_ratio_none);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_passive_voice_ratio_some);

    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_clarity_score_high);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_quality_score_high);

    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_split_sentences_basic);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_reflow_basic);

    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_normalize_whitespace);

    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_capitalize_sentence_case);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_capitalize_title_case);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_capitalize_uppercase);
    FOSSIL_TEST_ADD(objcpp_soap_suite, objcpp_test_io_soap_capitalize_lowercase);

    FOSSIL_TEST_REGISTER(objcpp_soap_suite);
}
