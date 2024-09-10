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
 * Check if a word is a curse word or racist phrase.
 * This function checks whether a given word is considered offensive.
 * It compares the word against a predefined list of offensive words and phrases
 * to determine its offensiveness.
 *
 * @param word The word to be checked for offensiveness.
 * @return     True if the word is offensive, false otherwise.
 */
int32_t fossil_soap_is_offensive(const char *word);

/**
 * Get the number of offensive words found in a string.
 * This function counts the number of offensive words found in the input string.
 * It scans the entire string and checks each word against a list of offensive words
 * to determine the count of offensive occurrences.
 *
 * @param input The input string to be checked for offensive words.
 * @return      The number of offensive words found.
 */
int32_t fossil_soap_count_offensive(const char *input);

#ifdef __cplusplus
}
#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
