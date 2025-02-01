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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Sanitize a string by replacing curse words with asterisks.
 * This function scans the input string for offensive words and replaces them with asterisks,
 * thereby making the string suitable for use in contexts where offensive language is not allowed.
 * The input string is modified in place.
 *
 * @param input The input string to be sanitized in-place.
 */
void fossil_soap_sanitize(char *input);

/**
 * Check if a word is an offensive word or phrase.
 * Returns EXIT_FAILURE if the word is considered offensive, EXIT_SUCCESS otherwise.
 */
int32_t fossil_soap_is_offensive(const char *word);

/**
 * Check if a word is meme speak.
 * Returns EXIT_FAILURE if the word is considered meme speak, EXIT_SUCCESS otherwise.
 */
int32_t fossil_soap_is_rotbrain(const char *word);

/**
 * Count offensive words in a string.
 * Returns the number of offensive words found in the input string.
 */
int32_t fossil_soap_count_offensive(const char *input);

/**
 * Count meme speak words in a string.
 * Returns the number of meme speak words found in the input string.
 */
int32_t fossil_soap_count_rotbrain(const char *input);

/**
 * Check if a string is context-aware.
 * Returns EXIT_FAILURE if the string is context-aware, EXIT_SUCCESS otherwise.
 */
int32_t fossil_soap_context_aware(const char *input);

#ifdef __cplusplus
}

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
             * Sanitize a string by replacing curse words with asterisks.
             * This function scans the input string for offensive words and replaces them with asterisks,
             * thereby making the string suitable for use in contexts where offensive language is not allowed.
             * The input string is modified in place.
             *
             * @param input The input string to be sanitized in-place.
             */
            static void sanitize(char *input) {
                fossil_soap_sanitize(input);
            }

            /**
             * Check if a word is an offensive word or phrase.
             * Returns EXIT_FAILURE if the word is considered offensive, EXIT_SUCCESS otherwise.
             */
            static int32_t is_offensive(const char *word) {
                return fossil_soap_is_offensive(word);
            }

            /**
             * Check if a word is meme speak.
             * Returns EXIT_FAILURE if the word is considered meme speak, EXIT_SUCCESS otherwise.
             */
            static int32_t is_rotbrain(const char *word) {
                return fossil_soap_is_rotbrain(word);
            }

            /**
             * Count offensive words in a string.
             * Returns the number of offensive words found in the input string.
             */
            static int32_t count_offensive(const char *input) {
                return fossil_soap_count_offensive(input);
            }

            /**
             * Count meme speak words in a string.
             * Returns the number of meme speak words found in the input string.
             */
            static int32_t count_rotbrain(const char *input) {
                return fossil_soap_count_rotbrain(input);
            }

            /**
             * Check if a string is context-aware.
             * Returns EXIT_FAILURE if the string is context-aware, EXIT_SUCCESS otherwise.
             */
            static int32_t context_aware(const char *input) {
                return fossil_soap_context_aware(input);
            }
        };
    }
}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
