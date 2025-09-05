/**
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * This file is part of the Fossil Logic project, which aims to develop high-
 * performance, cross-platform applications and libraries. The code contained
 * herein is subject to the terms and conditions defined in the project license.
 *
 * Author: Michael Gene Brockus (Dreamer)
 *
 * Copyright (C) 2024 Fossil Logic. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#ifndef FOSSIL_IO_SOAP_H
#define FOSSIL_IO_SOAP_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Sanitize input text by removing or replacing "rot-brain" and meme-based language.
 *
 * @param text The input text to sanitize.
 * @return A dynamically allocated sanitized string (must be freed by the caller).
 */
char *fossil_io_soap_sanitize(const char *text);

/**
 * @brief Suggest proper alternatives for rot-brain words or grammar fixes.
 *
 * @param text The input text.
 * @return A dynamically allocated string with suggestions (must be freed by the caller).
 */
char *fossil_io_soap_suggest(const char *text);

/**
 * @brief Add a custom word or phrase to the filter.
 *
 * @param phrase The phrase to add.
 * @return 0 on success, nonzero on failure.
 */
int fossil_io_soap_add_custom_filter(const char *phrase);

/**
 * @brief Clear all custom filters.
 */
void fossil_io_soap_clear_custom_filters(void);

/**
 * @brief Detect the tone of a sentence.
 *
 * @param text The input text.
 * @return A string representing the detected tone ("formal", "casual", "sarcastic", etc.).
 */
const char *fossil_io_soap_detect_tone(const char *text);

/**
 * @brief Analyze sentence structure and flag grammatical inconsistencies.
 *
 * @param text Input string to analyze.
 * @return 0 if grammar is clean, non-zero otherwise.
 */
int fossil_io_soap_check_grammar(const char *text);

/**
 * @brief Normalize all informal or abbreviated expressions.
 *
 * @param text Input string to normalize.
 * @return A newly allocated normalized string (caller must free).
 */
char *fossil_io_soap_normalize(const char *text);

/**
 * @brief Normalize internet slang or leetspeak in input text.
 *
 * @param text The input string.
 * @return A dynamically allocated cleaned-up version (must be freed).
 */
char *fossil_io_soap_normalize_slang(const char *text);

/**
 * @brief Apply a grammar correction pass over the input text.
 *
 * @param text The input text.
 * @return A dynamically allocated corrected string (must be freed).
 */
char *fossil_io_soap_correct_grammar(const char *text);

/**
 * @brief Detect exaggeration or hyperbolic language in a sentence.
 *
 * @param text The input string.
 * @return 1 if exaggeration detected, 0 otherwise.
 */
int fossil_io_soap_detect_exaggeration(const char *text);

/**
 * @brief Replace offensive language with neutral alternatives.
 *
 * @param text The input string.
 * @return A dynamically allocated sanitized string (must be freed).
 */
char *fossil_io_soap_filter_offensive(const char *text);

/** 
 * Detects ragebait content in the given text.
 * @param text Input string to analyze.
 * @return Non-zero if ragebait patterns are found, 0 otherwise.
 */
int fossil_io_soap_detect_ragebait(const char *text);

/** 
 * Detects clickbait content in the given text.
 * @param text Input string to analyze.
 * @return Non-zero if clickbait patterns are found, 0 otherwise.
 */
int fossil_io_soap_detect_clickbait(const char *text);

/** 
 * Detects spam content in the given text.
 * @param text Input string to analyze.
 * @return Non-zero if spam patterns are found, 0 otherwise.
 */
int fossil_io_soap_detect_spam(const char *text);

/** 
 * Detects woke-related content in the given text.
 * @param text Input string to analyze.
 * @return Non-zero if woke patterns are found, 0 otherwise.
 */
int fossil_io_soap_detect_woke(const char *text);

/** 
 * Detects propaganda content in the given text.
 * @param text Input string to analyze.
 * @return Non-zero if propaganda patterns are found, 0 otherwise.
 */
int fossil_io_soap_detect_propaganda(const char *text);

/** 
 * Detects automated/bot content in the given text.
 * @param text Input string to analyze.
 * @return Non-zero if bot patterns are found, 0 otherwise.
 */
int fossil_io_soap_detect_bot(const char *text);

/** 
 * Detects fake news content in the given text.
 * @param text Input string to analyze.
 * @return Non-zero if fake news patterns are found, 0 otherwise.
 */
int fossil_io_soap_detect_fake_news(const char *text);

/** 
 * Detects sarcastic tone in the given text.
 * @param text Input string to analyze.
 * @return Non-zero if sarcastic patterns are found, 0 otherwise.
 */
int fossil_io_soap_detect_sarcasm(const char *text);

/** 
 * Detects formal tone in the given text.
 * @param text Input string to analyze.
 * @return Non-zero if formal patterns are found, 0 otherwise.
 */
int fossil_io_soap_detect_formal(const char *text);

/** 
 * Detects "snowflake"-related content in the given text.
 * @param text Input string to analyze.
 * @return Non-zero if snowflake patterns are found, 0 otherwise.
 */
int fossil_io_soap_detect_snowflake(const char *text);

/** 
 * Detects "offensive"-related content in the given text.
 * @param text Input string to analyze.
 * @return Non-zero if offensive patterns are found, 0 otherwise.
 */
int fossil_io_soap_detect_offensive(const char *text);

#ifdef __cplusplus
}

#include <string>
#include <memory>

/**
 * C++ wrapper for the SOAP API.
 */
namespace fossil {
    namespace io {

        /**
         * @brief SOAP API for sanitizing and analyzing user text input.
         * 
         * Provides C++ wrappers for detecting, transforming, or correcting slang, tone, sentiment,
         * clickbait, and other language features with a focus on clarity and safety.
         */
        class Soap {
        public:

            /**
             * @brief Sanitize input by replacing meme/rot-brain terms with standard alternatives.
             * 
             * @param text The input string.
             * @return A cleaned-up version of the text.
             */
            static std::string sanitize(const std::string &text) {
                std::unique_ptr<char, decltype(&free)> ptr(fossil_io_soap_sanitize(text.c_str()), free);
                return ptr ? std::string(ptr.get()) : std::string{};
            }

            /**
             * @brief Sanitize input text (C-style).
             *
             * @param text The input string.
             * @return A heap-allocated cleaned-up version (must be freed manually).
             */
            static char* sanitize(const char* text) {
                return fossil_io_soap_sanitize(text);
            }

            /**
             * @brief Suggest alternative expressions for slang or incorrect grammar.
             * 
             * @param text The input string.
             * @return A string with suggested improvements.
             */
            static std::string suggest(const std::string &text) {
                std::unique_ptr<char, decltype(&free)> ptr(fossil_io_soap_suggest(text.c_str()), free);
                return ptr ? std::string(ptr.get()) : std::string{};
            }

            /**
             * @brief Suggest improvements (C-style).
             * 
             * @param text The input string.
             * @return A heap-allocated suggestion (must be freed manually).
             */
            static char* suggest(const char* text) {
                return fossil_io_soap_suggest(text);
            }

            /**
             * @brief Add a custom slang or flagged phrase to the filter.
             * 
             * @param phrase The custom phrase.
             * @return 0 on success, nonzero on failure.
             */
            static int add_custom_filter(const std::string &phrase) {
                return fossil_io_soap_add_custom_filter(phrase.c_str());
            }

            /**
             * @brief Add a custom slang or flagged phrase to the filter (C-style).
             * 
             * @param phrase The custom phrase.
             * @return 0 on success, nonzero on failure.
             */
            static int add_custom_filter(const char* phrase) {
                return fossil_io_soap_add_custom_filter(phrase);
            }

            /**
             * @brief Clear all user-added custom filters.
             */
            static void clear_custom_filters() {
                fossil_io_soap_clear_custom_filters();
            }

            /**
             * @brief Detect tone of the input (e.g., "sarcastic", "formal").
             * 
             * @param text The input string.
             * @return A tone descriptor string.
             */
            static std::string detect_tone(const std::string &text) {
                return std::string(fossil_io_soap_detect_tone(text.c_str()));
            }

            /**
             * @brief Detect tone of the input (C-style).
             * 
             * @param text The input string.
             * @return A tone descriptor string.
             */
            static const char* detect_tone(const char* text) {
                return fossil_io_soap_detect_tone(text);
            }

            /**
             * @brief Check if the input contains exaggerated or hyperbolic language.
             * 
             * @param text The input string.
             * @return true if exaggerated, false otherwise.
             */
            static bool is_exaggerated(const std::string &text) {
                return fossil_io_soap_detect_exaggeration(text.c_str()) != 0;
            }

            /**
             * Detects whether the given text contains ragebait phrases.
             *
             * Ragebait phrases are text patterns designed to provoke anger,
             * outrage, or strong emotional reactions in the reader.
             *
             * @param text The input C string to analyze.
             * @return 1 if any ragebait phrase is detected, 0 otherwise.
             */
            static int is_ragebait(const std::string &text) {
                return fossil_io_soap_detect_ragebait(text.c_str());
            }

            /**
             * @brief Check if the input uses clickbait language.
             * 
             * @param text The input string.
             * @return true if clickbait detected, false otherwise.
             */
            static bool is_clickbait(const std::string &text) {
                return fossil_io_soap_detect_clickbait(text.c_str()) != 0;
            }
            
            /**
             * @brief Detects if a given text contains spammy patterns
             *
             * @param text Input string to analyze
             * @return 1 if spam detected, 0 otherwise
             */
            static int is_spam(const std::string &text){
                return fossil_io_soap_detect_spam(text.c_str()) != 0;
            }
            
            /**
             * @brief Detects if a given text contains "woke" tone patterns
             *
             * @param text Input string to analyze
             * @return 1 if woke tone detected, 0 otherwise
             */
            static int is_woke(const std::string &text){
                return fossil_io_soap_detect_woke(text.c_str()) != 0;
            }

            /**
             * @brief Normalize slang and internet abbreviations.
             * 
             * @param text The input string.
             * @return A cleaned version of the input text.
             */
            static std::string normalize_slang(const std::string &text) {
                std::unique_ptr<char, decltype(&free)> ptr(fossil_io_soap_normalize_slang(text.c_str()), free);
                return ptr ? std::string(ptr.get()) : std::string{};
            }

            /**
             * @brief Fix common grammar errors in input text.
             * 
             * @param text The input string.
             * @return A corrected version of the input.
             */
            static std::string correct_grammar(const std::string &text) {
                std::unique_ptr<char, decltype(&free)> ptr(fossil_io_soap_correct_grammar(text.c_str()), free);
                return ptr ? std::string(ptr.get()) : std::string{};
            }

            /**
             * @brief Replace offensive language with neutral alternatives.
             * 
             * @param text The input string.
             * @return A sanitized version with offensive terms filtered out.
             */
            static std::string filter_offensive(const std::string &text) {
                std::unique_ptr<char, decltype(&free)> ptr(fossil_io_soap_filter_offensive(text.c_str()), free);
                return ptr ? std::string(ptr.get()) : std::string{};
            }
        };

    } // namespace io
} // namespace fossil

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
