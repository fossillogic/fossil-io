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
#ifndef FOSSIL_IO_ERROR_H
#define FOSSIL_IO_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

//

/**
 * This function is used to report an error message with a formatted string.
 *
 * @param format The format string for the error message.
 * @param ... The additional arguments to be formatted.
 */
void fossil_io_error(const char *format, ...);

/**
 * This function returns a string literal representing the symbolic name
 * of the given error code, such as "system.ok", "io.read", or "memory.alloc".
 * The returned string is intended to be a stable, human-readable identifier
 * for the error code, suitable for logging, debugging, or diagnostics.
 *
 * @param error_code The error code whose symbolic name is to be retrieved.
 * @return A constant string literal describing the symbolic name of the error code,
 *         or "unknown" if the code is not recognized.
 */
const char *fossil_io_what(const char *error_code);

#ifdef __cplusplus
}

#include <exception>

/**
 * This namespace provides a set of functions for handling input and output operations.
 */
namespace fossil {
    /**
     * This namespace provides a set of functions for handling input and output operations.
     */
    namespace io {

        /**
         * Exception class for handling errors.
         */
        class Error : public std::exception {
        public:
            explicit Error(ErrorCode code) : code_(code) {}

            const char* what() const noexcept override {
                return fossil_io_what(static_cast<fossil_status_t>(code_));
            }

            ErrorCode code() const noexcept {
                return code_;
            }

        private:
            ErrorCode code_;
        };
    }
}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
