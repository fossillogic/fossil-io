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
             * @param censor_char Character to use for censoring (default: '*').
             * @return A dynamically allocated sanitized string (must be freed by the caller).
             */
            static std::string sanitize(const std::string &text, char censor_char = '*') {
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
             * @param censor_char Character to use for censoring (default: '*').
             * @return A dynamically allocated sanitized string (must be freed by the caller).
             */
            static char* sanitize(const char* text, char censor_char = '*') {
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
