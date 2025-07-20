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
 * @brief Generate a hashed audit trail of all detected issues in input.
 *
 * @param text Input string to audit.
 * @return A JSON-formatted string with all flags and replacements, signed (caller must free).
 */
char *fossil_io_soap_generate_audit_block(const char *text);

/**
 * @brief Return a digest summary of all transformations made to a string.
 *
 * @param original Original input.
 * @param transformed Transformed result.
 * @return A cryptographic hash or short audit ID.
 */
char *fossil_io_soap_diff_digest(const char *original, const char *transformed);

/**
 * @brief Generate a hashed audit trail of all detected issues in input.
 *
 * @param text Input string to audit.
 * @return A JSON-formatted string with all flags and replacements, signed (caller must free).
 */
char *fossil_io_soap_generate_audit_block(const char *text);

/**
 * @brief Return a digest summary of all transformations made to a string.
 *
 * @param original Original input.
 * @param transformed Transformed result.
 * @return A cryptographic hash or short audit ID.
 */
char *fossil_io_soap_diff_digest(const char *original, const char *transformed);

/**
 * @brief Detect sarcasm, satire, or irony in a phrase.
 *
 * @param text Input string.
 * @return 1 if detected, 0 if not.
 */
int fossil_io_soap_detect_sarcasm(const char *text);

/**
 * @brief Determine if the tone of the input is informative, persuasive, emotional, etc.
 *
 * @param text Input text.
 * @return A short string such as "informative", "emotional", "neutral".
 */
const char *fossil_io_soap_detect_intent(const char *text);

/**
 * @brief Flag potentially offensive, biased, or manipulative phrases.
 *
 * @param text Input string.
 * @return A sanitized copy of the input with flagged content marked (caller must free).
 */
char *fossil_io_soap_flag_ethics(const char *text);

/**
 * @brief Return a list of ethical categories the input violates.
 *
 * @param text Input string.
 * @return JSON or CSV string of category names (caller must free).
 */
char *fossil_io_soap_list_ethics_flags(const char *text);

/**
 * @brief Load SOAP behavior extensions from a `.fish` or `.jellyfish` mindset file.
 *
 * @param filename Path to file to load.
 * @return 0 on success, non-zero on error.
 */
int fossil_io_soap_load_mindset_file(const char *filename);

/**
 * @brief Serialize SOAP's current state into a `.jellyfish` compatible format.
 *
 * @return A `.jellyfish` format string with active filters, models, and thresholds (caller must free).
 */
char *fossil_io_soap_export_mindset(void);

/**
 * @brief Score the semantic similarity between two phrases.
 *
 * @param a First input.
 * @param b Second input.
 * @return A similarity score from 0.0 to 1.0.
 */
float fossil_io_soap_semantic_similarity(const char *a, const char *b);

/**
 * @brief Determine if the given input matches a known propaganda or meme pattern.
 *
 * @param text Input to check.
 * @return 1 if matched, 0 otherwise.
 */
int fossil_io_soap_detect_memetic_pattern(const char *text);

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
