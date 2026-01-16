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
#ifndef FOSSIL_IO_CIPHER_H
#define FOSSIL_IO_CIPHER_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Opaque Types
 * ============================================================================
 */

typedef struct fossil_io_regex fossil_io_regex_t;
typedef struct fossil_io_regex_match fossil_io_regex_match_t;

/* ============================================================================
 * Compile & Destroy
 * ============================================================================
 */

/**
 * fossil_io_regex_compile
 *
 * @pattern   Regular expression pattern
 * @options   NULL-terminated list of string option IDs
 * @error_out Optional pointer to receive error string (owned by caller)
 *
 * Returns compiled regex or NULL on error.
 */
fossil_io_regex_t *fossil_io_regex_compile(
    const char *pattern,
    const char **options,
    char **error_out);

/**
 * Free compiled regex.
 */
void fossil_io_regex_free(fossil_io_regex_t *re);

/* ============================================================================
 * Matching
 * ============================================================================
 */

/**
 * Execute regex against input text.
 *
 * Returns:
 *   1  = match
 *   0  = no match
 *  <0  = error
 */
int fossil_io_regex_match(
    const fossil_io_regex_t *re,
    const char *text,
    fossil_io_regex_match_t **out_match);

/**
 * Free match object.
 */
void fossil_io_regex_match_free(fossil_io_regex_match_t *m);

/* ============================================================================
 * Capture Groups
 * ============================================================================
 */

int fossil_io_regex_group_count(const fossil_io_regex_match_t *m);

const char *fossil_io_regex_group(const fossil_io_regex_match_t *m, int index);

#ifdef __cplusplus
}

#include <string>
#include <vector>
#include <stdexcept>

namespace fossil {

    namespace io {
        /* ============================================================================
         * Regex (C++ Wrapper)
         * ============================================================================
         *
         * RAII-safe wrapper around fossil_io_regex_t
         * ABI-stable with the C implementation
         */
        
        class Regex {
        public:
            /* ------------------------------------------------------------------------
             * Constructors / Destructor
             * ------------------------------------------------------------------------
             */
        
            Regex() noexcept
                : re_(nullptr)
            {}
        
            explicit Regex(
                const std::string &pattern,
                const std::vector<std::string> &options = {})
                : re_(nullptr)
            {
                compile(pattern, options);
            }
        
            Regex(const Regex &) = delete;
            Regex &operator=(const Regex &) = delete;
        
            Regex(Regex &&other) noexcept
                : re_(other.re_)
            {
                other.re_ = nullptr;
            }
        
            Regex &operator=(Regex &&other) noexcept {
                if (this != &other) {
                    reset();
                    re_ = other.re_;
                    other.re_ = nullptr;
                }
                return *this;
            }
        
            ~Regex() {
                reset();
            }
        
            /* ------------------------------------------------------------------------
             * Compilation
             * ------------------------------------------------------------------------
             */
        
            void compile(
                const std::string &pattern,
                const std::vector<std::string> &options = {}
            )
            {
                reset();
        
                std::vector<const char *> opt_ids;
                opt_ids.reserve(options.size() + 1);
        
                for (const auto &o : options)
                    opt_ids.push_back(o.c_str());
        
                opt_ids.push_back(nullptr);
        
                char *err = nullptr;
        
                re_ = fossil_io_regex_compile(
                    pattern.c_str(),
                    opt_ids.empty() ? nullptr : opt_ids.data(),
                    &err
                );
        
                if (!re_) {
                    std::string msg = err ? err : "regex compilation failed";
                    if (err)
                        free(err);
                    throw std::runtime_error(msg);
                }
            }
        
            bool is_compiled() const noexcept
            {
                return re_ != nullptr;
            }
        
            /* ------------------------------------------------------------------------
             * Matching
             * ------------------------------------------------------------------------
             */
        
            bool match(const std::string &text)
            {
                ensure_compiled();
        
                fossil_io_regex_match_t *m = nullptr;
                int rc = fossil_io_regex_match(re_, text.c_str(), &m);
        
                if (rc <= 0)
                    return false;
        
                fossil_io_regex_match_free(m);
                return true;
            }
        
            bool match(
                const std::string &text,
                std::vector<std::string> &groups
            )
            {
                ensure_compiled();
                groups.clear();
        
                fossil_io_regex_match_t *m = nullptr;
                int rc = fossil_io_regex_match(re_, text.c_str(), &m);
        
                if (rc <= 0)
                    return false;
        
                int count = fossil_io_regex_group_count(m);
                groups.reserve(count);
        
                for (int i = 0; i < count; ++i) {
                    const char *g = fossil_io_regex_group(m, i);
                    groups.emplace_back(g ? g : "");
                }
        
                fossil_io_regex_match_free(m);
                return true;
            }
        
            /* ------------------------------------------------------------------------
             * Utilities
             * ------------------------------------------------------------------------
             */
        
            void reset() noexcept
            {
                if (re_) {
                    fossil_io_regex_free(re_);
                    re_ = nullptr;
                }
            }
        
        private:
            fossil_io_regex_t *re_;
        
            void ensure_compiled() const
            {
                if (!re_)
                    throw std::logic_error("regex not compiled");
            }
        };
    
    } /* namespace io */
} /* namespace fossil */

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
