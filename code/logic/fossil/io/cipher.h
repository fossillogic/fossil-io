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
#ifndef FOSSIL_IO_SOAP_H
#define FOSSIL_IO_SOAP_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


// ============================================================================
// SOAP v3 Analysis & Processing Options (Primary Control Surface)
// ============================================================================

/**
 * fossil_io_soap_grammar_style_t
 *
 * Aggregate result of grammar and stylistic analysis.
 */
typedef struct {

    /** 1 if no grammar issues detected, 0 otherwise. */
    int grammar_ok;

    /** Percentage of sentences using passive voice (0–100). */
    int passive_voice_pct;

    /** Classified writing style label (static string). */
    const char *style;

    /** Number of grammar errors detected. */
    int grammar_error_count;

    /** Number of stylistic inconsistencies detected. */
    int style_inconsistency_count;

    /** Array of grammar error descriptions (NULL-terminated, may be NULL). */
    const char **grammar_errors;

    /** Array of stylistic inconsistency descriptions (NULL-terminated, may be NULL). */
    const char **style_inconsistencies;

    /** Confidence score for detected style (0–100). */
    int style_confidence;

    /** Reserved for future expansion (set to 0). */
    int reserved[4];

} fossil_io_soap_grammar_style_t;

/**
 * fossil_io_soap_scores_t
 *
 * Numerical scoring results normalized to 0–100.
 * All fields are maximized (set to 100) by default for maximal scoring.
 */
typedef struct {

    /** Ease of reading and comprehension (0–100, maximal = 100). */
    int readability;

    /** Logical clarity and coherence (0–100, maximal = 100). */
    int clarity;

    /** Overall writing quality (0–100, maximal = 100). */
    int quality;

} fossil_io_soap_scores_t;

// ============================================================================
// Sanitize, Analysis, & Summary
// ============================================================================

/**
 * fossil_io_soap_sanitize
 *
 * Cleans and filters low-quality, unsafe, or disallowed language while
 * preserving original meaning where possible.
 *
 * Returns:
 *  - Newly allocated sanitized string (caller owns memory)
 *  - NULL on allocation or processing failure
 *
 * Internal logic:
 *  - Duplicates the input string.
 *  - Replaces control characters (except newline) with spaces.
 *  - Lowercases the text.
 *  - Returns the sanitized result.
 */
char *fossil_io_soap_sanitize(const char *text);

/**
 * fossil_io_soap_suggest
 *
 * Generates improvement suggestions for clarity, tone, or quality without
 * directly modifying the original text.
 *
 * Intended for assistive or review workflows.
 *
 * Internal logic:
 *  - Duplicates the input string.
 *  - Collapses multiple spaces into one.
 *  - Returns the suggestion string.
 */
char *fossil_io_soap_suggest(const char *text);

/**
 * fossil_io_soap_summarize
 *
 * Produces a concise summary capturing the primary intent and content of
 * the input text.
 *
 * Internal logic:
 *  - Splits the input into sentences.
 *  - Concatenates the first two sentences (if available) as the summary.
 *  - Returns the summary string.
 */
char *fossil_io_soap_summarize(const char *text);

// ============================================================================
// Grammar & Style Analysis
// ============================================================================

/**
 * Analyzes grammar correctness and stylistic characteristics.
 *
 * Internal logic:
 *  - Scans the input text for passive voice indicators (e.g., "was", "were").
 *  - Counts total words and computes the percentage of passive voice usage.
 *  - Classifies style as "emotional" if exclamation or question marks are present,
 *    "formal" if certain formal words or punctuation are found, otherwise "neutral".
 *  - Returns a struct with grammar_ok (always 1 in this stub), passive_voice_pct, and style label.
 */
fossil_io_soap_grammar_style_t
fossil_io_soap_analyze_grammar_style(const char *text);

/**
 * Applies grammar correction heuristics and returns corrected text.
 *
 * Internal logic:
 *  - Allocates a new string buffer for the output.
 *  - Iterates through the input, tracking sentence boundaries, quotes, parentheses, and URLs.
 *  - Normalizes whitespace, collapses repeated punctuation, and capitalizes sentence starts.
 *  - Detects and replaces common contractions and fixes common grammar mistakes inline.
 *  - Ensures terminal punctuation at the end of the output.
 *  - Returns the corrected string (caller must free).
 */
char *fossil_io_soap_correct_grammar(const char *text);

// ============================================================================
// Readability, Clarity, & Quality Analysis
// ============================================================================

/**
 * Computes readability, clarity, and quality scores.
 *
 * Internal logic:
 *  - Returns a struct with default scores (70/70/70).
 *  - Penalizes readability if text is very short.
 *  - Boosts clarity if text contains newlines.
 *  - Boosts quality if text does not contain "!!!".
 */
fossil_io_soap_scores_t fossil_io_soap_score(const char *text);

/**
 * Converts a readability score into a human-readable label.
 *
 * Internal logic:
 *  - Returns "excellent" for scores >80, "good" for >60, "fair" for >40, else "poor".
 */
const char *fossil_io_soap_readability_label(int readability_score);

/**
 * Generic detection interface for a single detector identifier.
 *
 * Internal logic:
 *  - Lowercases input.
 *  - Looks up detector patterns by ID.
 *  - Checks for pattern matches at document, sentence, and word level.
 *  - Handles special detectors (brain_rot, structural).
 *  - Returns 1 if any pattern matches, else 0.
 * 
 * Available detector options:
 *   - "conspiracy"
 *   - "spam"
 *   - "ragebait"
 *   - "clickbait"
 *   - "bot"
 *   - "marketing"
 *   - "technobabble"
 *   - "hype"
 *   - "political"
 *   - "offensive"
 *   - "misinfo"
 *   - "brain_rot"
 *   - "formal"
 *   - "casual"
 *   - "sarcasm"
 *   - "neutral"
 *   - "aggressive"
 *   - "emotional"
 *   - "passive"
 *   - "snowflake"
 *   - "redundant"
 *   - "poor_cohesion"
 *   - "repeated_words"
 */
int fossil_io_soap_detect(const char *text, const char *detector_id);

/**
 * Splits text into logical units (sentences, paragraphs, blocks)
 * based on flow type.
 *
 * Internal logic:
 *  - If text contains sentence-ending punctuation, splits as sentences.
 *  - Otherwise, splits as words.
 *  - Allocates and returns a NULL-terminated array of strings.
 */
char **fossil_io_soap_split(const char *text);

/**
 * Reflows text to a target line width.
 *
 * Internal logic:
 *  - Copies input to output, inserting newlines at spaces when width is reached.
 *  - Returns a newly allocated string.
 */
char *fossil_io_soap_reflow(const char *text, int width);

/**
 * Normalizes whitespace, punctuation, and casing.
 *
 * Internal logic:
 *  - Duplicates input.
 *  - Lowercases all letters.
 *  - Returns the normalized string.
 */
char *fossil_io_soap_normalize(const char *text);

/**
 * Applies capitalization rules.
 *
 * Internal logic:
 *  - If mode==0, applies sentence-case (capitalize after '.', '!', '?').
 *  - If mode==1, applies title-case (capitalize first letter of each word).
 *  - Returns a newly allocated string.
 */
char *fossil_io_soap_capitalize(const char *text, int mode);

/**
 * Attempts to rephrase text by correcting grammar, normalizing style, and reflowing lines.
 *
 * Internal logic:
 *  - Sanitizes and normalizes the input.
 *  - Corrects grammar.
 *  - Capitalizes sentences.
 *  - Reflows to 80 columns.
 *  - Returns the rewritten string (caller must free).
 */
char *fossil_io_soap_rewrite(const char *text);

/**
 * Pretty-prints text with consistent indentation and line breaks.
 *
 * Internal logic:
 *  - Removes excessive whitespace and normalizes.
 *  - Capitalizes sentences.
 *  - Reflows to 72 columns.
 *  - Returns the formatted string (caller must free).
 */
char *fossil_io_soap_format(const char *text);

/*
 * Declutter text by repairing word boundaries and whitespace.
 *
 * - Splits camelCase / PascalCase words
 * - Normalizes excessive whitespace
 * - Preserves numbers and symbols
 *
 * Parameters:
 *   text  - Input UTF-8 / ASCII text (NULL-safe)
 *
 * Returns:
 *   Newly allocated NUL-terminated string.
 *   Caller owns the memory and must free().
 *   Returns NULL on allocation failure or if text is NULL.
 */
char *fossil_io_soap_declutter(const char *text);

/*
 * Normalize punctuation and sentence structure.
 *
 * - Collapses repeated punctuation (!!!??? → !)
 * - Normalizes ellipsis (...)
 * - Capitalizes sentence starts
 * - Ensures terminal punctuation
 *
 * Parameters:
 *   text  - Input UTF-8 / ASCII text (NULL-safe)
 *
 * Returns:
 *   Newly allocated NUL-terminated string.
 *   Caller owns the memory and must free().
 *   Returns NULL on allocation failure or if text is NULL.
 */
char *fossil_io_soap_punctuate(const char *text);

/**
 * fossil_io_soap_process
 *
 * Performs a full processing pipeline on the input text, including sanitization,
 * normalization, grammar correction, capitalization, and formatting.
 *
 * Returns:
 *  - Newly allocated processed string (caller owns memory)
 *  - NULL on allocation or processing failure
 *
 * Internal logic:
 *  - Sanitizes the input text.
 *  - Normalizes whitespace and punctuation.
 *  - Corrects grammar.
 *  - Applies sentence capitalization.
 *  - Formats the text for consistent indentation and line breaks.
 *  - Returns the processed result.
 */
char *fossil_io_soap_process(const char *text);

#ifdef __cplusplus
}

#include <string>
#include <memory>
#include <vector>

/**
 * C++ wrapper for the SOAP API.
 */
namespace fossil {

    namespace io {
    
        class Soap {
        public:
            // ===============================
            // Sanitize, Suggest, Summarize
            // ===============================

            /**
             * Sanitizes the input text by removing or replacing low-quality or unsafe language.
             * Returns a sanitized string. Throws away the result if allocation fails.
             *
             * Internal logic:
             *   - Duplicates the input string.
             *   - Replaces control characters (except newline) with spaces.
             *   - Normalizes leetspeak and lowercases the text.
             *   - Returns the sanitized result.
             */
            static std::string sanitize(const std::string &text) {
                char *res = fossil_io_soap_sanitize(text.c_str());
                std::string out = res ? res : "";
                free(res);
                return out;
            }

            /**
             * Suggests improvements for clarity, tone, or quality without modifying the original text.
             * Returns a suggestion string. Throws away the result if allocation fails.
             *
             * Internal logic:
             *   - Duplicates the input string.
             *   - Collapses multiple spaces into one.
             *   - Returns the suggestion string.
             */
            static std::string suggest(const std::string &text) {
                char *res = fossil_io_soap_suggest(text.c_str());
                std::string out = res ? res : "";
                free(res);
                return out;
            }

            /**
             * Summarizes the input text, producing a concise summary of its main content and intent.
             * Returns a summary string. Throws away the result if allocation fails.
             *
             * Internal logic:
             *   - Splits the input into sentences.
             *   - Concatenates the first two sentences (if available) as the summary.
             *   - Returns the summary string.
             */
            static std::string summarize(const std::string &text) {
                char *res = fossil_io_soap_summarize(text.c_str());
                std::string out = res ? res : "";
                free(res);
                return out;
            }

            // ===============================
            // Grammar & Style
            // ===============================

            /**
             * Holds the result of grammar and style analysis.
             */
            struct GrammarStyle {
                bool grammar_ok;           ///< True if no grammar issues detected.
                int passive_voice_pct;     ///< Percentage of passive voice usage.
                std::string style;         ///< Detected writing style label.
            };

            /**
             * Analyzes grammar correctness and stylistic characteristics of the input text.
             * Returns a GrammarStyle struct with analysis results.
             *
             * Internal logic:
             *   - Scans the input text for passive voice indicators (e.g., "was", "were").
             *   - Counts total words and computes the percentage of passive voice usage.
             *   - Classifies style as "emotional" if exclamation or question marks are present,
             *     "formal" if certain formal words or punctuation are found, otherwise "neutral".
             *   - Returns a struct with grammar_ok (always 1 in this stub), passive_voice_pct, and style label.
             */
            static GrammarStyle analyze_grammar_style(const std::string &text) {
                auto result = fossil_io_soap_analyze_grammar_style(text.c_str());
                return GrammarStyle{
                    result.grammar_ok != 0,
                    result.passive_voice_pct,
                    result.style ? result.style : ""
                };
            }

            /**
             * Applies grammar correction heuristics to the input text.
             * Returns a corrected string. Throws away the result if allocation fails.
             *
             * Internal logic:
             *   - Allocates a new string buffer for the output.
             *   - Iterates through the input, tracking sentence boundaries, quotes, parentheses, and URLs.
             *   - Normalizes whitespace, collapses repeated punctuation, and capitalizes sentence starts.
             *   - Detects and replaces common contractions and fixes common grammar mistakes inline.
             *   - Ensures terminal punctuation at the end of the output.
             *   - Returns the corrected string (caller must free).
             */
            static std::string correct_grammar(const std::string &text) {
                char *res = fossil_io_soap_correct_grammar(text.c_str());
                std::string out = res ? res : "";
                free(res);
                return out;
            }

            // ===============================
            // Scores
            // ===============================

            /**
             * Holds readability, clarity, and quality scores.
             */
            struct Scores {
                int readability;   ///< Ease of reading and comprehension.
                int clarity;       ///< Logical clarity and coherence.
                int quality;       ///< Overall writing quality.
            };

            /**
             * Computes readability, clarity, and quality scores for the input text.
             * Returns a Scores struct with normalized values (0–100).
             *
             * Internal logic:
             *   - Returns a struct with default scores (70/70/70).
             *   - Penalizes readability if text is very short.
             *   - Boosts clarity if text contains newlines.
             *   - Boosts quality if text does not contain "!!!".
             */
            static Scores score(const std::string &text) {
                auto result = fossil_io_soap_score(text.c_str());
                return Scores{ result.readability, result.clarity, result.quality };
            }

            /**
             * Converts a readability score into a human-readable label.
             * Returns the label as a string.
             *
             * Internal logic:
             *   - Returns "excellent" for scores >80, "good" for >60, "fair" for >40, else "poor".
             */
            static std::string readability_label(int score) {
                const char *label = fossil_io_soap_readability_label(score);
                return label ? label : "";
            }

            // ===============================
            // Detection
            // ===============================

            /**
             * Runs a generic detector by identifier on the input text.
             * Returns true if the detector is triggered, false otherwise.
             *
             * Internal logic:
             *   - Normalizes input (leetspeak, lowercase).
             *   - Looks up detector patterns by ID.
             *   - Checks for pattern matches at document, sentence, and word level.
             *   - Handles special detectors (brain_rot, leet, morse, structural).
             *   - Returns 1 if any pattern matches, else 0.
             * 
             * Available detector options:
             *   - "conspiracy"
             *   - "spam"
             *   - "ragebait"
             *   - "clickbait"
             *   - "bot"
             *   - "marketing"
             *   - "technobabble"
             *   - "hype"
             *   - "political"
             *   - "offensive"
             *   - "misinfo"
             *   - "brain_rot"
             *   - "leet"
             *   - "morse"
             *   - "formal"
             *   - "casual"
             *   - "sarcasm"
             *   - "neutral"
             *   - "aggressive"
             *   - "emotional"
             *   - "passive"
             *   - "snowflake"
             *   - "redundant"
             *   - "poor_cohesion"
             *   - "repeated_words"
             */
            static bool detect(const std::string &text, const std::string &detector_id) {
                return fossil_io_soap_detect(text.c_str(), detector_id.c_str()) != 0;
            }

            // ===============================
            // Splitting & Normalization
            // ===============================

            /**
             * Splits the input text into logical units (sentences, paragraphs, or blocks).
             * Returns a vector of strings, each representing a unit.
             *
             * Internal logic:
             *   - If text contains sentence-ending punctuation, splits as sentences.
             *   - Otherwise, splits as words.
             *   - Allocates and returns a NULL-terminated array of strings.
             */
            static std::vector<std::string> split(const std::string &text) {
                char **arr = fossil_io_soap_split(text.c_str());
                std::vector<std::string> result;
                if (!arr) return result;
                for (size_t i = 0; arr[i]; ++i) {
                    result.emplace_back(arr[i]);
                    free(arr[i]);
                }
                free(arr);
                return result;
            }

            /**
             * Reflows the input text to a specified target line width.
             * Returns the reflowed string. Throws away the result if allocation fails.
             *
             * Internal logic:
             *   - Copies input to output, inserting newlines at spaces when width is reached.
             *   - Returns a newly allocated string.
             */
            static std::string reflow(const std::string &text, int width) {
                char *res = fossil_io_soap_reflow(text.c_str(), width);
                std::string out = res ? res : "";
                free(res);
                return out;
            }

            /**
             * Normalizes whitespace, punctuation, and casing in the input text.
             * Returns the normalized string. Throws away the result if allocation fails.
             *
             * Internal logic:
             *   - Duplicates input.
             *   - Normalizes leetspeak and lowercases all letters.
             *   - Returns the normalized string.
             */
            static std::string normalize(const std::string &text) {
                char *res = fossil_io_soap_normalize(text.c_str());
                std::string out = res ? res : "";
                free(res);
                return out;
            }

            /**
             * Applies capitalization rules to the input text.
             * The mode parameter specifies sentence-case, title-case, or preserve-case.
             * Returns the capitalized string. Throws away the result if allocation fails.
             *
             * Internal logic:
             *   - If mode==0, applies sentence-case (capitalize after '.', '!', '?').
             *   - If mod/**
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
#ifndef FOSSIL_IO_CIPHER_H
#define FOSSIL_IO_CIPHER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Encode text using a named cipher.
 *
 * cipher_id format:
 *   "<name>"
 *   "<name>:<param>"
 *   "<name>:<param1>,<param2>"
 *
 * Examples:
 *   "caesar:3"
 *   "vigenere:KEY"
 *   "railfence:3"
 *   "base32"
 *
 * Returns newly allocated string (caller frees).
 */
char *fossil_io_cipher_encode(const char *text, const char *cipher_id);

/**
 * Decode text using a named cipher.
 *
 * Same cipher_id format as encode.
 */
char *fossil_io_cipher_decode(const char *text, const char *cipher_id);

#ifdef __cplusplus
}

#include <string>

namespace fossil {
    namespace io {
    
        /**
         * Cipher
         *
         * Thin C++ wrapper around Fossil IO cipher encode/decode API.
         *
         * This class:
         *  - Owns returned buffers safely
         *  - Does not throw
         *  - Does not allocate unless the C layer does
         *  - Is header-only and ABI-neutral
         */
        class Cipher {
        public:
            /**
             * Encode text using a named cipher.
             *
             * Example:
             *   Cipher::encode("hello", "caesar:3");
             */
            static std::string encode(const std::string &text,
                                      const std::string &cipher_id) {
                return encode(text.c_str(), cipher_id.c_str());
            }
        
            static std::string encode(const char *text,
                                      const char *cipher_id) {
                if (!text || !cipher_id)
                    return std::string();
        
                char *out = fossil_io_cipher_encode(text, cipher_id);
                if (!out)
                    return std::string();
        
                std::string result(out);
                std::free(out);
                return result;
            }
        
            /**
             * Decode text using a named cipher.
             *
             * Example:
             *   Cipher::decode("khoor", "caesar:3");
             */
            static std::string decode(const std::string &text,
                                      const std::string &cipher_id) {
                return decode(text.c_str(), cipher_id.c_str());
            }
        
            static std::string decode(const char *text,
                                      const char *cipher_id) {
                if (!text || !cipher_id)
                    return std::string();
        
                char *out = fossil_io_cipher_decode(text, cipher_id);
                if (!out)
                    return std::string();
        
                std::string result(out);
                std::free(out);
                return result;
            }
        
            /**
             * Convenience helpers for common patterns
             */
            static std::string caesar(const std::string &text, int shift) {
                return encode(text, "caesar:" + std::to_string(shift));
            }
        
            static std::string rot13(const std::string &text) {
                return encode(text, "rot13");
            }
        
            static std::string vigenere(const std::string &text,
                                        const std::string &key) {
                return encode(text, "vigenere:" + key);
            }
        
            static std::string base64(const std::string &text) {
                return encode(text, "base64");
            }
        };
    
    } // namespace io
} // namespace fossil

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
    
    } // namespace io

} // namespace fossil

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
