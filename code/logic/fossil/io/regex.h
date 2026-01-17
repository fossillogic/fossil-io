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
#ifndef FOSSIL_IO_REGEX_H
#define FOSSIL_IO_REGEX_H

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
 * Compile a regular expression pattern into a regex object.
 *
 * @param pattern   The regular expression pattern as a null-terminated string.
 * @param options   A NULL-terminated array of option string IDs (may be NULL).
 * @param error_out Optional pointer to receive an error message string (allocated, must be freed by caller).
 * @return          Pointer to compiled regex object, or NULL on error.
 */
fossil_io_regex_t *fossil_io_regex_compile(
    const char *pattern,
    const char **options,
    char **error_out);

/**
 * Free a compiled regex object.
 *
 * @param re Pointer to the regex object to free.
 */
void fossil_io_regex_free(fossil_io_regex_t *re);

/* ============================================================================
 * Matching
 * ============================================================================
 */

/**
 * Execute a compiled regex against input text.
 *
 * @param re        Pointer to compiled regex object.
 * @param text      Input text to match against.
 * @param out_match Optional pointer to receive match object (allocated, must be freed).
 * @return          1 if match found, 0 if no match, <0 on error.
 */
int fossil_io_regex_match(
    const fossil_io_regex_t *re,
    const char *text,
    fossil_io_regex_match_t **out_match);

/**
 * Free a regex match object.
 *
 * @param m Pointer to the match object to free.
 */
void fossil_io_regex_match_free(fossil_io_regex_match_t *m);

/* ============================================================================
 * Capture Groups
 * ============================================================================
 */

/**
 * Get the number of capture groups in a match object.
 *
 * @param m Pointer to the match object.
 * @return  Number of capture groups.
 */
int fossil_io_regex_group_count(const fossil_io_regex_match_t *m);

/**
 * Get the string value of a specific capture group.
 *
 * @param m     Pointer to the match object.
 * @param index Index of the capture group (0-based).
 * @return      Pointer to the group string, or NULL if not available.
 */
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

            /**
             * @brief Default constructor. Initializes an empty Regex object.
             * The regex is not compiled until compile() is called.
             */
            Regex() noexcept
                : re_(nullptr)
            {}

            /**
             * @brief Constructs and compiles a Regex object from a pattern and options.
             * Throws std::runtime_error if compilation fails.
             *
             * @param pattern Regular expression pattern.
             * @param options Optional vector of option strings.
             */
            explicit Regex(
                const std::string &pattern,
                const std::vector<std::string> &options = {})
                : re_(nullptr)
            {
                compile(pattern, options);
            }

            /**
             * @brief Deleted copy constructor. Regex objects are non-copyable.
             */
            Regex(const Regex &) = delete;

            /**
             * @brief Deleted copy assignment operator. Regex objects are non-copyable.
             */
            Regex &operator=(const Regex &) = delete;

            /**
             * @brief Move constructor. Transfers ownership of the compiled regex.
             *
             * @param other Regex object to move from.
             */
            Regex(Regex &&other) noexcept
                : re_(other.re_)
            {
                other.re_ = nullptr;
            }

            /**
             * @brief Move assignment operator. Transfers ownership of the compiled regex.
             *
             * @param other Regex object to move from.
             * @return Reference to this object.
             */
            Regex &operator=(Regex &&other) noexcept {
                if (this != &other) {
                    reset();
                    re_ = other.re_;
                    other.re_ = nullptr;
                }
                return *this;
            }

            /**
             * @brief Destructor. Frees the compiled regex object if present.
             */
            ~Regex() {
                reset();
            }

            /* ------------------------------------------------------------------------
             * Compilation
             * ------------------------------------------------------------------------
             */

            /**
             * @brief Compiles the given pattern and options into a regex object.
             * Throws std::runtime_error if compilation fails.
             *
             * @param pattern Regular expression pattern.
             * @param options Optional vector of option strings.
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

            /**
             * @brief Checks if the regex object is compiled.
             *
             * @return true if compiled, false otherwise.
             */
            bool is_compiled() const noexcept
            {
                return re_ != nullptr;
            }

            /* ------------------------------------------------------------------------
             * Matching
             * ------------------------------------------------------------------------
             */

            /**
             * @brief Checks if the regex matches the given text.
             *
             * @param text Input text to match against.
             * @return true if match found, false otherwise.
             * @throws std::logic_error if regex is not compiled.
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

            /**
             * @brief Matches the regex against the given text and extracts capture groups.
             *
             * @param text Input text to match against.
             * @param groups Output vector to receive capture group strings.
             * @return true if match found, false otherwise.
             * @throws std::logic_error if regex is not compiled.
             */
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

            /**
             * @brief Frees the compiled regex object and resets the internal pointer.
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

            /**
             * @brief Ensures that the regex object is compiled.
             * Throws std::logic_error if not compiled.
             */
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
