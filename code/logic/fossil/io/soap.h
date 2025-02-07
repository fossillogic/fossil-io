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
 * Setup soap to have a preloaded list of words for use in the sanitize step.
 */
void fossil_io_soap_create(void);

/**
 * Sanitize a string by replacing curse words with asterisks.
 * This function scans the input string for offensive words and replaces them with asterisks,
 * thereby making the string suitable for use in contexts where offensive language is not allowed.
 * The input string is modified in place.
 *
 * @param input The input string to be sanitized in-place.
 * @return The sanitized string.
 */
char *fossil_io_soap_sanitize(const char *input);

/**
 * Check if a word is an offensive word or phrase.
 * Returns EXIT_FAILURE if the word is considered offensive, EXIT_SUCCESS otherwise.
 */
int32_t fossil_io_soap_is_offensive(const char *word);

/**
 * Check if a word is meme speak.
 * Returns EXIT_FAILURE if the word is considered meme speak, EXIT_SUCCESS otherwise.
 */
int32_t fossil_io_soap_is_rotbrain(const char *word);

/**
 * Count offensive words in a string.
 * Returns the number of offensive words found in the input string.
 */
int32_t fossil_io_soap_count_offensive(const char *input);

/**
 * Count meme speak words in a string.
 * Returns the number of meme speak words found in the input string.
 */
int32_t fossil_io_soap_count_rotbrain(const char *input);

/**
 * Count positive words in a string.
 * Returns the number of positive words found in the input string.
 */
int32_t fossil_io_soap_count_positive(const char *input);

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
             * Setup soap to have a preloaded list of words for use in the sanitize step.
             */
            Soap() {
                fossil_io_soap_create();
            }

            /**
             * Destructor.
             */
            ~Soap() = default;

            /**
             * Sanitize a string by replacing curse words with asterisks.
             * This function scans the input string for offensive words and replaces them with asterisks,
             * thereby making the string suitable for use in contexts where offensive language is not allowed.
             * The input string is modified in place.
             *
             * @param input The input string to be sanitized in-place.
             */
            static void sanitize(std::string &input) {
                fossil_io_soap_sanitize(&input[0]);
            }

            /**
             * Sanitize a C string by replacing curse words with asterisks.
             * This function scans the input string for offensive words and replaces them with asterisks,
             * thereby making the string suitable for use in contexts where offensive language is not allowed.
             * The input string is modified in place.
             *
             * @param input The input C string to be sanitized in-place.
             */
            static void sanitize(char *input) {
                fossil_io_soap_sanitize(input);
            }

            /**
             * Check if a word is an offensive word or phrase.
             * Returns EXIT_FAILURE if the word is considered offensive, EXIT_SUCCESS otherwise.
             */
            static int32_t is_offensive(const std::string &word) {
                return fossil_io_soap_is_offensive(word.c_str());
            }

            /**
             * Check if a C string word is an offensive word or phrase.
             * Returns EXIT_FAILURE if the word is considered offensive, EXIT_SUCCESS otherwise.
             */
            static int32_t is_offensive(const char *word) {
                return fossil_io_soap_is_offensive(word);
            }

            /**
             * Check if a word is meme speak.
             * Returns EXIT_FAILURE if the word is considered meme speak, EXIT_SUCCESS otherwise.
             */
            static int32_t is_rotbrain(const std::string &word) {
                return fossil_io_soap_is_rotbrain(word.c_str());
            }

            /**
             * Check if a C string word is meme speak.
             * Returns EXIT_FAILURE if the word is considered meme speak, EXIT_SUCCESS otherwise.
             */
            static int32_t is_rotbrain(const char *word) {
                return fossil_io_soap_is_rotbrain(word);
            }

            /**
             * Count offensive words in a string.
             * Returns the number of offensive words found in the input string.
             */
            static int32_t count_offensive(const std::string &input) {
                return fossil_io_soap_count_offensive(input.c_str());
            }

            /**
             * Count offensive words in a C string.
             * Returns the number of offensive words found in the input string.
             */
            static int32_t count_offensive(const char *input) {
                return fossil_io_soap_count_offensive(input);
            }

            /**
             * Count meme speak words in a string.
             * Returns the number of meme speak words found in the input string.
             */
            static int32_t count_rotbrain(const std::string &input) {
                return fossil_io_soap_count_rotbrain(input.c_str());
            }

            /**
             * Count meme speak words in a C string.
             * Returns the number of meme speak words found in the input string.
             */
            static int32_t count_rotbrain(const char *input) {
                return fossil_io_soap_count_rotbrain(input);
            }

            /**
             * Count positive words in a string.
             * Returns the number of positive words found in the input string.
             */
            static int32_t count_positive(const std::string &input) {
                return fossil_io_soap_count_positive(input.c_str());
            }

            /**
             * Count positive words in a C string.
             * Returns the number of positive words found in the input string.
             */
            static int32_t count_positive(const char *input) {
                return fossil_io_soap_count_positive(input);
            }

        };

    }

}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
