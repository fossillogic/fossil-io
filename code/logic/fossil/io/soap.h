/*
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
 * @brief Analyze the sentiment of the input text.
 *
 * @param text The input string.
 * @return A string like "positive", "negative", or "neutral".
 */
const char *fossil_io_soap_detect_sentiment(const char *text);

/**
 * @brief Detect potential hate speech or harmful content.
 *
 * @param text The input string.
 * @return 1 if harmful content is detected, 0 otherwise.
 */
int fossil_io_soap_detect_harmful_content(const char *text);

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
 * @brief Evaluate the clarity and readability level of the input text.
 *
 * @param text The input string.
 * @return Readability score (e.g., Flesch-Kincaid grade level).
 */
float fossil_io_soap_evaluate_readability(const char *text);

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
 * @brief Check if input contains clickbait-style language.
 *
 * @param text The input string.
 * @return 1 if clickbait is detected, 0 otherwise.
 */
int fossil_io_soap_detect_clickbait(const char *text);

/**
 * @brief Detect logical fallacies or flawed reasoning in a sentence.
 *
 * @param text The input string.
 * @return A string describing the detected fallacy, or NULL if none found.
 */
const char *fossil_io_soap_detect_fallacy(const char *text);

/**
 * @brief Summarize the key idea in the input sentence.
 *
 * @param text The input string.
 * @return A dynamically allocated short summary (must be freed).
 */
char *fossil_io_soap_summarize(const char *text);

/**
 * @brief Score how polite or impolite the tone of the input is.
 *
 * @param text The input string.
 * @return A score from 0.0 (rude) to 1.0 (very polite).
 */
float fossil_io_soap_politeness_score(const char *text);

#ifdef __cplusplus
}

#include <string>

/**
 * C++ wrapper for the SOAP API.
 */
namespace fossil {

    /**
     * Namespace for I/O operations.
     */
    namespace io {
        /**
         * SOAP API for sanitizing strings.
         */
        class Soap {
        public:
            /**
             * Sanitize input text by removing or replacing "rot-brain" and meme-based language.
             *
             * @param text The input text to sanitize.
             * @return A dynamically allocated sanitized string (must be freed by the caller).
             */
            static std::string sanitize(const std::string &text) {
                return fossil_io_soap_sanitize(text.c_str());
            }

            /**
             * Suggest proper alternatives for rot-brain words or grammar fixes.
             *
             * @param text The input text.
             * @param format_type Custom format for output (e.g., "*" or "#").
             * @return A dynamically allocated string with suggestions (must be freed by the caller).
             */
            static std::string suggest(const std::string &text) {
                return fossil_io_soap_suggest(text.c_str());
            }

            /**
             * Add a custom word or phrase to the filter.
             *
             * @param phrase The phrase to add.
             * @return 0 on success, nonzero on failure.
             */
            static int add_custom_filter(const std::string &phrase) {
                return fossil_io_soap_add_custom_filter(phrase.c_str());
            }

            /**
             * Clear all custom filters.
             */
            static void clear_custom_filters() {
                fossil_io_soap_clear_custom_filters();
            }

            /**
             * Detect the tone of a sentence.
             *
             * @param text The input text.
             * @return A string representing the detected tone ("formal", "casual", "sarcastic", etc.).
             */
            static std::string detect_tone(const std::string &text) {
                return fossil_io_soap_detect_tone(text.c_str());
            }

            /**
             * Sanitize input text by removing or replacing "rot-brain" and meme-based language.
             *
             * @param text The input text to sanitize.
             * @return A dynamically allocated sanitized string (must be freed by the caller).
             */
            static char* sanitize(const char* text) {
                return fossil_io_soap_sanitize(text);
            }

            /**
             * Suggest proper alternatives for rot-brain words or grammar fixes.
             *
             * @param text The input text.
             * @param format_type Custom format for output (e.g., "*" or "#").
             * @return A dynamically allocated string with suggestions (must be freed by the caller).
             */
            static char* suggest(const char* text) {
                return fossil_io_soap_suggest(text);
            }

            /**
             * Add a custom word or phrase to the filter.
             *
             * @param phrase The phrase to add.
             * @return 0 on success, nonzero on failure.
             */
            static int add_custom_filter(const char* phrase) {
                return fossil_io_soap_add_custom_filter(phrase);
            }

            /**
             * Detect the tone of a sentence.
             *
             * @param text The input text.
             * @return A string representing the detected tone ("formal", "casual", "sarcastic", etc.).
             */
            static const char* detect_tone(const char* text) {
                return fossil_io_soap_detect_tone(text);
            }

        };

    }

}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
