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
 * Tokenization: Split input into words and check for offensive language and meme speak.
 */
void fossil_soap_tokenize_and_process(char *input);

/**
 * Run fuzzer and test the enhanced sanitization.
 * Fuzzed inputs are generated and passed through the sanitization function to check robustness.
 */
void fossil_soap_fuzzer(const char *input);

/**
 * Fuzzer Helper: Generate test cases for sanitization.
 * Generates fuzzed input by appending random letters and symbols, then sanitizing the string.
 */
void fossil_soap_fuzzer(const char *input);

/**
 * Utility: Trim extra spaces and ensure proper sentence capitalization.
 * Ensures that the input string has only single spaces between words and proper sentence capitalization.
 */
void fossil_soap_correct_grammar(char *input);

/**
 * Main sanitization function: Handles offensive words, meme speak, synonyms, and grammar correction.
 * Sanitizes the input string based on predefined rules for offensive words, meme speak, and synonyms.
 */
void fossil_soap_sanitize(char *input);

/**
 * Check if a word is offensive.
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

#ifdef __cplusplus
}
#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
