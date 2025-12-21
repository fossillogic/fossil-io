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

typedef struct {

    /* =====================================================================
     * Core quality signals (scored / weighted)
     * ===================================================================== */
    int detect_quality;           // overall writing quality
    int detect_clarity;           // clarity & coherence
    int detect_tone;              // tone classification
    int detect_readability;       // readability metrics
    int detect_brain_rot;         // meme-speak, slang abuse, low-signal phrasing

    /* =====================================================================
     * Content & intent detection
     * ===================================================================== */
    int detect_spam;
    int detect_ragebait;
    int detect_clickbait;
    int detect_bot;
    int detect_marketing;
    int detect_technobabble;
    int detect_hype;
    int detect_political;
    int detect_conspiracy;
    int detect_offensive;
    int detect_propaganda;        // agenda-driven manipulation
    int detect_misinformation;    // false or misleading claims (heuristic)

    /* =====================================================================
     * Behavioral / stylistic signals
     * ===================================================================== */
    int detect_formal;
    int detect_casual;
    int detect_sarcasm;
    int detect_neutral;
    int detect_aggressive;
    int detect_emotional;
    int detect_passive_aggressive;

    /* =====================================================================
     * Structural & linguistic analysis
     * ===================================================================== */
    int analyze_grammar;
    int analyze_style;
    int analyze_passive_voice;
    int analyze_sentence_complexity;
    int analyze_paragraph_cohesion;
    int analyze_redundancy;
    int analyze_word_repetition;

    /* =====================================================================
     * Normalization & correction passes
     * ===================================================================== */
    int apply_sanitization;       // sanitize low-quality language
    int apply_normalization;      // whitespace, punctuation, casing
    int apply_grammar_correction;

    /* =====================================================================
     * Output controls
     * ===================================================================== */
    int include_summary;
    int include_scores;
    int include_flags;
    int include_style;
    int include_debug;            // include internal diagnostics

} fossil_io_soap_options_t;

// ============================================================================
// Sanitize, Analysis, & Summary
// ============================================================================

char *fossil_io_soap_sanitize(const char *text);
char *fossil_io_soap_suggest(const char *text);
char *fossil_io_soap_summarize(const char *text);

// ============================================================================
// Grammar & Style Analysis
// ============================================================================

typedef struct {
    int grammar_ok;          // 1 = clean, 0 = issues found
    int passive_voice_pct;   // 0–100
    const char *style;       // "formal", "technical", "casual", etc.
} fossil_io_soap_grammar_style_t;

fossil_io_soap_grammar_style_t
fossil_io_soap_analyze_grammar_style(const char *text);

char *fossil_io_soap_correct_grammar(const char *text);

// ============================================================================
// Readability, Clarity, & Quality Analysis
// ============================================================================

typedef struct {
    int readability;   // 0–100
    int clarity;       // 0–100
    int quality;       // 0–100
} fossil_io_soap_scores_t;

fossil_io_soap_scores_t fossil_io_soap_score(const char *text);
const char *fossil_io_soap_readability_label(int readability_score);

// ============================================================================
// Unified Detector API with Flow Type
// ============================================================================

int fossil_io_soap_detect(
    const char *text,
    const char *detector_id,
    const char *flow_type
);

// ============================================================================
// Custom Vocabulary Control
// ============================================================================

int fossil_io_soap_custom_load(const char *phrase);
void fossil_io_soap_custom_unload(void);
char *fossil_io_soap_custom_filter(const char *text);

// ============================================================================
// Text Splitting & Normalization
// ============================================================================

char **fossil_io_soap_split(const char *text, const char *flow_type);
char *fossil_io_soap_reflow(const char *text, int width);
char *fossil_io_soap_normalize(const char *text);
char *fossil_io_soap_capitalize(const char *text, int mode);

// ============================================================================
// High-Level Text Processing
// ============================================================================

char *fossil_io_soap_process(
    const char *text,
    const char *flow_type,
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
            
                Options() { std::memset(&c_options, 0, sizeof(c_options)); }
            
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
            static bool detect(const std::string &text, const std::string &detector_id,
                               const std::string &flow_type) {
                return fossil_io_soap_detect(text.c_str(), detector_id.c_str(), flow_type.c_str()) != 0;
            }
        
            // ===============================
            // Custom Vocabulary
            // ===============================
            static bool custom_load(const std::string &phrase) {
                return fossil_io_soap_custom_load(phrase.c_str()) == 0;
            }
        
            static void custom_unload() {
                fossil_io_soap_custom_unload();
            }
        
            static std::string custom_filter(const std::string &text) {
                char *res = fossil_io_soap_custom_filter(text.c_str());
                std::string out = res ? res : "";
                free(res);
                return out;
            }
        
            // ===============================
            // Splitting & Normalization
            // ===============================
            static std::vector<std::string> split(const std::string &text, const std::string &flow_type) {
                char **arr = fossil_io_soap_split(text.c_str(), flow_type.c_str());
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
                                       const std::string &flow_type,
                                       const Options *options = nullptr) {
                const fossil_io_soap_options_t *opt_ptr = options ? &options->c_options : nullptr;
                char *res = fossil_io_soap_process(text.c_str(), flow_type.c_str(), opt_ptr);
                std::string out = res ? res : "";
                free(res);
                return out;
            }
        };
    
    } // namespace io

} // namespace fossil

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
