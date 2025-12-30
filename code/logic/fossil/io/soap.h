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
 * fossil_io_soap_options_t
 *
 * Central configuration structure controlling all SOAP analysis, detection,
 * normalization, and output behavior.
 *
 * Each field acts as an enable/disable switch (0 = off, non-zero = on),
 * allowing callers to compose lightweight scans or full forensic analysis
 * pipelines without recompilation.
 *
 * NOTE:
 *  - Options are intentionally flat to avoid nested config churn.
 *  - Fields may be interpreted as weights or thresholds in future versions.
 *  - Unknown or unsupported flags MUST be ignored for forward compatibility.
 */
typedef struct {

    /* =====================================================================
     * Core quality signals (scored / weighted)
     * ===================================================================== */

    /** Evaluate overall writing quality using combined heuristics:
     *  grammar, clarity, coherence, redundancy, and signal density.
     */
    int detect_quality;

    /** Measure clarity and logical coherence of ideas and sentence flow. */
    int detect_clarity;

    /** Classify tone (e.g., neutral, aggressive, sarcastic). */
    int detect_tone;

    /** Compute readability metrics (sentence length, word complexity, flow). */
    int detect_readability;

    /** Detect low-signal language such as meme-speak, excessive slang,
     *  filler phrasing, or stylistic degradation ("brain rot").
     */
    int detect_brain_rot;

    /* =====================================================================
     * Content & intent detection
     * ===================================================================== */

    /** Heuristic detection of spam-like or templated content. */
    int detect_spam;

    /** Identify emotionally manipulative rage-bait patterns. */
    int detect_ragebait;

    /** Detect exaggerated or curiosity-driven clickbait phrasing. */
    int detect_clickbait;

    /** Detect non-human or automated text patterns. */
    int detect_bot;

    /** Detect marketing, sales, or promotional language. */
    int detect_marketing;

    /** Detect dense but low-meaning technical jargon. */
    int detect_technobabble;

    /** Detect hype-driven exaggeration or inflated claims. */
    int detect_hype;

    /** Identify political framing or messaging. */
    int detect_political;

    /** Detect conspiratorial framing or narrative patterns. */
    int detect_conspiracy;

    /** Detect offensive, abusive, or demeaning language. */
    int detect_offensive;

    /** Detect agenda-driven messaging or ideological persuasion. */
    int detect_propaganda;

    /** Heuristic detection of false, misleading, or unsupported claims. */
    int detect_misinformation;

    /* =====================================================================
     * Behavioral / stylistic signals
     * ===================================================================== */

    /** Detect formal or academic writing style. */
    int detect_formal;

    /** Detect casual or conversational writing style. */
    int detect_casual;

    /** Detect sarcasm or ironic phrasing. */
    int detect_sarcasm;

    /** Detect emotionally neutral or objective tone. */
    int detect_neutral;

    /** Detect aggressive or confrontational language. */
    int detect_aggressive;

    /** Detect emotionally charged language. */
    int detect_emotional;

    /** Detect passive-aggressive phrasing patterns. */
    int detect_passive_aggressive;

    int detect_morse;

    /* =====================================================================
     * Structural & linguistic analysis
     * ===================================================================== */

    /** Perform grammar validation and error detection. */
    int analyze_grammar;

    /** Analyze overall writing style and consistency. */
    int analyze_style;

    /** Measure passive voice usage frequency. */
    int analyze_passive_voice;

    /** Analyze sentence complexity and syntactic depth. */
    int analyze_sentence_complexity;

    /** Measure paragraph-level cohesion and flow. */
    int analyze_paragraph_cohesion;

    /** Detect redundant phrasing or repeated ideas. */
    int analyze_redundancy;

    /** Detect excessive word or phrase repetition. */
    int analyze_word_repetition;

    /* =====================================================================
     * Normalization & correction passes
     * ===================================================================== */

    /** Sanitize low-quality or unsafe language. */
    int apply_sanitization;

    /** Normalize whitespace, punctuation, and casing. */
    int apply_normalization;

    /** Apply grammar correction heuristics. */
    int apply_grammar_correction;

    /* =====================================================================
     * Output controls
     * ===================================================================== */

    /** Include a human-readable summary in the output. */
    int include_summary;

    /** Include numerical quality and analysis scores. */
    int include_scores;

    /** Include detected flags and classifications. */
    int include_flags;

    /** Include style and tone descriptors. */
    int include_style;

    /** Include internal diagnostics and debug metadata. */
    int include_debug;

} fossil_io_soap_options_t;

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
 */
char *fossil_io_soap_sanitize(const char *text);

/**
 * fossil_io_soap_suggest
 *
 * Generates improvement suggestions for clarity, tone, or quality without
 * directly modifying the original text.
 *
 * Intended for assistive or review workflows.
 */
char *fossil_io_soap_suggest(const char *text);

/**
 * fossil_io_soap_summarize
 *
 * Produces a concise summary capturing the primary intent and content of
 * the input text.
 */
char *fossil_io_soap_summarize(const char *text);

// ============================================================================
// Grammar & Style Analysis
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

} fossil_io_soap_grammar_style_t;

/**
 * Analyzes grammar correctness and stylistic characteristics.
 */
fossil_io_soap_grammar_style_t
fossil_io_soap_analyze_grammar_style(const char *text);

/**
 * Applies grammar correction heuristics and returns corrected text.
 */
char *fossil_io_soap_correct_grammar(const char *text);

// ============================================================================
// Readability, Clarity, & Quality Analysis
// ============================================================================

/**
 * fossil_io_soap_scores_t
 *
 * Numerical scoring results normalized to 0–100.
 */
typedef struct {

    /** Ease of reading and comprehension. */
    int readability;

    /** Logical clarity and coherence. */
    int clarity;

    /** Overall writing quality. */
    int quality;

} fossil_io_soap_scores_t;

/**
 * Computes readability, clarity, and quality scores.
 */
fossil_io_soap_scores_t fossil_io_soap_score(const char *text);

/**
 * Converts a readability score into a human-readable label.
 */
const char *fossil_io_soap_readability_label(int readability_score);

/**
 * Generic detection interface for a single detector identifier.
 */
int fossil_io_soap_detect(const char *text, const char *detector_id);

/**
 * Splits text into logical units (sentences, paragraphs, blocks)
 * based on flow type.
 */
char **fossil_io_soap_split(const char *text);

/**
 * Reflows text to a target line width.
 */
char *fossil_io_soap_reflow(const char *text, int width);

/**
 * Normalizes whitespace, punctuation, and casing.
 */
char *fossil_io_soap_normalize(const char *text);

/**
 * Applies capitalization rules.
 *
 * mode may indicate sentence-case, title-case, or preserve-case.
 */
char *fossil_io_soap_capitalize(const char *text, int mode);

// ============================================================================
// High-Level Text Processing
// ============================================================================

/**
 * fossil_io_soap_process
 *
 * Executes a full SOAP processing pipeline using the supplied options.
 *
 * This function may perform:
 *  - Detection and scoring
 *  - Grammar and style analysis
 *  - Sanitization and normalization
 *  - Summary and metadata generation
 *
 * Returns:
 *  - Newly allocated output string (caller owns memory)
 *  - NULL on failure
 */
char *fossil_io_soap_process(
    const char *text,
    const fossil_io_soap_options_t *options
);

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
            // Options
            // ===============================
            struct Options {
                fossil_io_soap_options_t c_options;
            
                Options() { memset(&c_options, 0, sizeof(c_options)); }
            
                // ===============================
                // Core quality signals
                // ===============================
                Options& detect_quality(bool v) { c_options.detect_quality = v; return *this; }
                Options& detect_clarity(bool v) { c_options.detect_clarity = v; return *this; }
                Options& detect_tone(bool v) { c_options.detect_tone = v; return *this; }
                Options& detect_readability(bool v) { c_options.detect_readability = v; return *this; }
                Options& detect_brain_rot(bool v) { c_options.detect_brain_rot = v; return *this; }
            
                // ===============================
                // Content & intent detection
                // ===============================
                Options& detect_spam(bool v) { c_options.detect_spam = v; return *this; }
                Options& detect_ragebait(bool v) { c_options.detect_ragebait = v; return *this; }
                Options& detect_clickbait(bool v) { c_options.detect_clickbait = v; return *this; }
                Options& detect_bot(bool v) { c_options.detect_bot = v; return *this; }
                Options& detect_marketing(bool v) { c_options.detect_marketing = v; return *this; }
                Options& detect_technobabble(bool v) { c_options.detect_technobabble = v; return *this; }
                Options& detect_hype(bool v) { c_options.detect_hype = v; return *this; }
                Options& detect_political(bool v) { c_options.detect_political = v; return *this; }
                Options& detect_conspiracy(bool v) { c_options.detect_conspiracy = v; return *this; }
                Options& detect_offensive(bool v) { c_options.detect_offensive = v; return *this; }
                Options& detect_propaganda(bool v) { c_options.detect_propaganda = v; return *this; }
                Options& detect_misinformation(bool v) { c_options.detect_misinformation = v; return *this; }
            
                // ===============================
                // Behavioral / stylistic signals
                // ===============================
                Options& detect_formal(bool v) { c_options.detect_formal = v; return *this; }
                Options& detect_casual(bool v) { c_options.detect_casual = v; return *this; }
                Options& detect_sarcasm(bool v) { c_options.detect_sarcasm = v; return *this; }
                Options& detect_neutral(bool v) { c_options.detect_neutral = v; return *this; }
                Options& detect_aggressive(bool v) { c_options.detect_aggressive = v; return *this; }
                Options& detect_emotional(bool v) { c_options.detect_emotional = v; return *this; }
                Options& detect_passive_aggressive(bool v) { c_options.detect_passive_aggressive = v; return *this; }
            
                // ===============================
                // Structural & linguistic analysis
                // ===============================
                Options& analyze_grammar(bool v) { c_options.analyze_grammar = v; return *this; }
                Options& analyze_style(bool v) { c_options.analyze_style = v; return *this; }
                Options& analyze_passive_voice(bool v) { c_options.analyze_passive_voice = v; return *this; }
                Options& analyze_sentence_complexity(bool v) { c_options.analyze_sentence_complexity = v; return *this; }
                Options& analyze_paragraph_cohesion(bool v) { c_options.analyze_paragraph_cohesion = v; return *this; }
                Options& analyze_redundancy(bool v) { c_options.analyze_redundancy = v; return *this; }
                Options& analyze_word_repetition(bool v) { c_options.analyze_word_repetition = v; return *this; }
            
                // ===============================
                // Normalization & correction passes
                // ===============================
                Options& apply_sanitization(bool v) { c_options.apply_sanitization = v; return *this; }
                Options& apply_normalization(bool v) { c_options.apply_normalization = v; return *this; }
                Options& apply_grammar_correction(bool v) { c_options.apply_grammar_correction = v; return *this; }
            
                // ===============================
                // Output controls
                // ===============================
                Options& include_summary(bool v) { c_options.include_summary = v; return *this; }
                Options& include_scores(bool v) { c_options.include_scores = v; return *this; }
                Options& include_flags(bool v) { c_options.include_flags = v; return *this; }
                Options& include_style(bool v) { c_options.include_style = v; return *this; }
                Options& include_debug(bool v) { c_options.include_debug = v; return *this; }
            };
        
            // ===============================
            // Sanitize, Suggest, Summarize
            // ===============================
            static std::string sanitize(const std::string &text) {
                char *res = fossil_io_soap_sanitize(text.c_str());
                std::string out = res ? res : "";
                free(res);
                return out;
            }
        
            static std::string suggest(const std::string &text) {
                char *res = fossil_io_soap_suggest(text.c_str());
                std::string out = res ? res : "";
                free(res);
                return out;
            }
        
            static std::string summarize(const std::string &text) {
                char *res = fossil_io_soap_summarize(text.c_str());
                std::string out = res ? res : "";
                free(res);
                return out;
            }
        
            // ===============================
            // Grammar & Style
            // ===============================
            struct GrammarStyle {
                bool grammar_ok;
                int passive_voice_pct;
                std::string style;
            };
        
            static GrammarStyle analyze_grammar_style(const std::string &text) {
                auto result = fossil_io_soap_analyze_grammar_style(text.c_str());
                return GrammarStyle{
                    result.grammar_ok != 0,
                    result.passive_voice_pct,
                    result.style ? result.style : ""
                };
            }
        
            static std::string correct_grammar(const std::string &text) {
                char *res = fossil_io_soap_correct_grammar(text.c_str());
                std::string out = res ? res : "";
                free(res);
                return out;
            }
        
            // ===============================
            // Scores
            // ===============================
            struct Scores {
                int readability;
                int clarity;
                int quality;
            };
        
            static Scores score(const std::string &text) {
                auto result = fossil_io_soap_score(text.c_str());
                return Scores{ result.readability, result.clarity, result.quality };
            }
        
            static std::string readability_label(int score) {
                const char *label = fossil_io_soap_readability_label(score);
                return label ? label : "";
            }
        
            // ===============================
            // Detection
            // ===============================
            static bool detect(const std::string &text, const std::string &detector_id) {
                return fossil_io_soap_detect(text.c_str(), detector_id.c_str()) != 0;
            }
      
            // ===============================
            // Splitting & Normalization
            // ===============================
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
        
            static std::string reflow(const std::string &text, int width) {
                char *res = fossil_io_soap_reflow(text.c_str(), width);
                std::string out = res ? res : "";
                free(res);
                return out;
            }
        
            static std::string normalize(const std::string &text) {
                char *res = fossil_io_soap_normalize(text.c_str());
                std::string out = res ? res : "";
                free(res);
                return out;
            }
        
            static std::string capitalize(const std::string &text, int mode) {
                char *res = fossil_io_soap_capitalize(text.c_str(), mode);
                std::string out = res ? res : "";
                free(res);
                return out;
            }
        
            // ===============================
            // High-Level Process
            // ===============================
            static std::string process(const std::string &text,
                                       const Options *options = nullptr) {
                const fossil_io_soap_options_t *opt_ptr = options ? &options->c_options : nullptr;
                char *res = fossil_io_soap_process(text.c_str(), opt_ptr);
                std::string out = res ? res : "";
                free(res);
                return out;
            }
        };
    
    } // namespace io

} // namespace fossil

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
