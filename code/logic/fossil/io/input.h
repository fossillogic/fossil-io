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
#ifndef FOSSIL_IO_INPUT_H
#define FOSSIL_IO_INPUT_H

#include <stdarg.h>
#include "file.h"

/* Contexts */
typedef enum {
    FOSSIL_CTX_GENERIC,
    FOSSIL_CTX_HTML,
    FOSSIL_CTX_SQL,
    FOSSIL_CTX_SHELL,
    FOSSIL_CTX_FILENAME,
    FOSSIL_CTX_NONE
} fossil_context_t;

/* Bitmask flags for string sanitization results */
#define FOSSIL_SAN_OK        0x00  /* No issues detected; string is clean */
#define FOSSIL_SAN_MODIFIED  0x01  /* Input was modified during sanitization */
#define FOSSIL_SAN_SCRIPT    0x02  /* Script or JavaScript patterns detected */
#define FOSSIL_SAN_SQL       0x04  /* SQL injection patterns detected */
#define FOSSIL_SAN_SHELL     0x08  /* Shell or command execution patterns detected */
#define FOSSIL_SAN_BASE64    0x10  /* Suspiciously long base64 sequences detected */
#define FOSSIL_SAN_PATH      0x20  /* Path traversal or filesystem patterns detected */
#define FOSSIL_SAN_BOT       0x40  /* Bot or automated agent patterns detected */
#define FOSSIL_SAN_SPAM      0x80  /* Spam or suspicious marketing content detected */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reads a single character from the input stream.
 * 
 * @param input_stream  Pointer to the input stream to read from.
 * @return              The character read as an unsigned char cast to an int, or EOF on end-of-file or error.
 */
int fossil_io_getc(fossil_io_file_t *input_stream);

/**
 * @brief Trim leading and trailing whitespace from a string in place.
 *
 * This function removes all whitespace characters (spaces, tabs, newlines, etc.)
 * from the beginning and end of the input string. The trimming is done **in place**,
 * modifying the original string, and the result is always null-terminated.
 *
 * @param str Pointer to the null-terminated string to be trimmed.
 *            Must not be NULL. If the string is empty or contains only whitespace,
 *            it will be converted to an empty string ("").
 *
 * @note This function does not allocate memory; it simply shifts characters within
 *       the existing string buffer. The caller is responsible for ensuring that
 *       the buffer is writable and large enough to hold the original string.
 *
 * @example
 * char s[] = "   hello world  \n";
 * fossil_io_trim(s);
 * // s now contains "hello world"
 */
void fossil_io_trim(char *str);

/**
 * Reads a line from the input stream and stores it into the buffer pointed to by 'buf'.
 * 
 * @param buf           Pointer to the buffer where the line will be stored.
 * @param size          Maximum number of characters to be read, including the null terminator.
 * @param input_stream  Pointer to the input stream to read from.
 * @return              On success, the function returns 'buf'. If the end-of-file is reached or an error occurs, it returns NULL.
 */
char *fossil_io_gets_from_stream(char *buf, size_t size, fossil_io_file_t *input_stream);

/**
 * Reads a line from the input stream with error reporting.
 *
 * @param buf           Pointer to the buffer where the line will be stored.
 * @param size          Maximum number of characters to be read, including the null terminator.
 * @param input_stream  Pointer to the input stream to read from.
 * @param error_code    Pointer to an integer to store the error code (e.g., EOF, input error).
 * @return              On success, the function returns 'buf'. If the end-of-file is reached or an error occurs, it returns NULL.
 */
char *fossil_io_gets_from_stream_ex(char *buf, size_t size, fossil_io_file_t *input_stream, int *error_code);

/**
 * Reads formatted input from the standard input stream.
 * 
 * @param format        The format string specifying how the input should be interpreted.
 * @param ...           Additional arguments for storing the input values.
 * @return              On success, the number of input items successfully matched and assigned is returned.
 *                      On failure, EOF is returned.
 */
int fossil_io_scanf(const char *format, ...);

/**
 * Reads formatted input from the specified input stream.
 * 
 * @param input_stream  Pointer to the input stream to read from.
 * @param format        The format string specifying how the input should be interpreted.
 * @param ...           Additional arguments for storing the input values.
 * @return              On success, the number of input items successfully matched and assigned is returned.
 *                      On failure, EOF is returned.
 */
int fossil_io_fscanf(fossil_io_file_t *input_stream, const char *format, ...);

/**
 * Validates the input buffer and size before reading.
 *
 * @param buf           Pointer to the buffer where the input will be stored.
 * @param size          Size of the buffer.
 * @return              1 if the buffer and size are valid; 0 otherwise.
 */
int fossil_io_validate_input_buffer(const char *buf, size_t size);

/**
 * Reads a UTF-8 encoded line from the input stream.
 *
 * @param buf           Pointer to the buffer where the line will be stored.
 * @param size          Maximum number of characters to be read, including the null terminator.
 * @param input_stream  Pointer to the input stream to read from.
 * @return              On success, the function returns 'buf'. If the end-of-file is reached or an error occurs, it returns NULL.
 */
char *fossil_io_gets_utf8(char *buf, size_t size, fossil_io_file_t *input_stream);

/**
 * @brief Validates if the input string is a valid integer.
 * 
 * @param input The input string to validate.
 * @param output Pointer to an integer where the parsed value will be stored if valid.
 * @return true if the input is a valid integer, false otherwise.
 */
int fossil_io_validate_is_int(const char *input, int *output);

/**
 * @brief Validates if the input string is a valid float.
 * 
 * @param input The input string to validate.
 * @param output Pointer to a float where the parsed value will be stored if valid.
 * @return true if the input is a valid float, false otherwise.
 */
int fossil_io_validate_is_float(const char *input, float *output);

/**
 * @brief Validates if the input string contains only alphanumeric characters.
 * 
 * @param input The input string to validate.
 * @return true if the input is alphanumeric, false otherwise.
 */
int fossil_io_validate_is_alnum(const char *input);

/**
 * @brief Validates if the input string is a valid email address.
 * 
 * @param input The input string to validate.
 * @return true if the input is a valid email address, false otherwise.
 */
int fossil_io_validate_is_email(const char *input);

/**
 * @brief Validates if the input string does not exceed the specified maximum length.
 * 
 * @param input The input string to validate.
 * @param max_length The maximum allowed length of the input string.
 * @return true if the input length is within the specified limit, false otherwise.
 */
int fossil_io_validate_is_length(const char *input, size_t max_length);

/**
 * Check if password is weak or bad.
 *
 * Returns:
 *   1 if password is weak/bad
 *   0 if password passes the basic checks
 */
int fossil_io_validate_is_weak_password(const char *password,
                                        const char *username,
                                        const char *email);

/**
 * Check if a user-agent string looks like a bot/crawler
 */
int fossil_io_validate_is_suspicious_bot(const char *input);

/**
 * Check if an email belongs to a disposable / suspicious domain
 */
int fossil_io_validate_is_disposable_email(const char *input);

/**
 * Check if a string looks like a bot-style username:
 *  - Too many digits in a row
 *  - High ratio of digits to letters
 *  - Contains suspicious words like "bot", "test", "fake"
 *  - Looks like random noise (entropy check)
 */
int fossil_io_validate_is_suspicious_user(const char *input);

/**
 * @brief Validate and sanitize a string according to a specified context.
 *
 * This function scans the input string for suspicious content (scripts, SQL injection,
 * shell commands, bots, spam, path traversal, or long base64 blobs) and performs
 * context-aware sanitization by replacing disallowed characters with underscores.
 * It also returns a bitmask indicating the types of issues detected.
 *
 * @param input         The input string to be validated and sanitized. Must not be NULL.
 * @param output        The buffer to receive the sanitized string. Must not be NULL.
 * @param output_size   The size of the output buffer. Must be greater than 0.
 * @param ctx           The context in which the string will be used, which determines
 *                      the allowed character set and stricter rules for certain contexts.
 *
 * @return Bitmask of flags indicating results:
 *         - FOSSIL_SAN_OK        (0x00): No issues detected; string is clean.
 *         - FOSSIL_SAN_MODIFIED  (0x01): Input was modified during sanitization.
 *         - FOSSIL_SAN_SCRIPT    (0x02): Script or JavaScript patterns detected.
 *         - FOSSIL_SAN_SQL       (0x04): SQL injection patterns detected.
 *         - FOSSIL_SAN_SHELL     (0x08): Shell or command execution patterns detected.
 *         - FOSSIL_SAN_BASE64    (0x10): Suspiciously long base64 sequences detected.
 *         - FOSSIL_SAN_PATH      (0x20): Path traversal or filesystem patterns detected.
 *         - FOSSIL_SAN_BOT       (0x40): Bot or automated agent patterns detected.
 *         - FOSSIL_SAN_SPAM      (0x80): Spam or suspicious marketing content detected.
 *
 * @note The sanitized output is always null-terminated and will not exceed
 *       output_size bytes. The function uses heuristics and is not a substitute
 *       for context-specific escaping or prepared statements in SQL/HTML.
 */
int fossil_io_validate_sanitize_string(const char *input,
                                           char *output,
                                           size_t output_size,
                                           fossil_context_t ctx);

/**
 * Displays a menu of choices and returns the selected choice.
 * 
 * @param prompt The prompt message before displaying the menu.
 * @param choices Array of strings representing the choices.
 * @param num_choices The number of choices.
 * @return The index of the selected choice.
 */
int fossil_io_display_menu(const char *prompt, const char *choices[], int num_choices);

/**
 * Displays a simple progress bar.
 * 
 * @param progress The current progress (0-100).
 */
void fossil_io_show_progress(int progress);

/**
 * @brief Reads a line of input from standard input into the provided buffer.
 *
 * Reads at most `size - 1` characters from `stdin` and stores them in `buffer`.
 * If a newline character is read, it is replaced with a null terminator.
 *
 * @param buffer Pointer to the character buffer where input will be stored.
 * @param size The size of the buffer.
 * @return 0 on success, -1 on error or end-of-file.
 */
int fossil_io_gets(char *buffer, size_t size);

#ifdef __cplusplus
}

#include <cstddef>
#include <string>
#include <vector>
#include <utility>
#include <cstdarg>
#include <istream>

/**
 * Namespace for the Fossil Logic I/O library.
 */
namespace fossil {

    /**
     * Namespace for the I/O utilities.
     */
    namespace io {

        /**
         * Class for handling input operations.
         */
        class Input {
        public:
            /**
             * Reads a single character from the input stream.
             * 
             * @param input_stream  Pointer to the input stream to read from.
             * @return              The character read as an unsigned char cast to an int, or EOF on end-of-file or error.
             */
            static int getc(fossil_io_file_t *input_stream) {
                return fossil_io_getc(input_stream);
            }

            /**
             * @brief Trim leading and trailing whitespace from a string in place.
             *
             * This function removes all whitespace characters (spaces, tabs, newlines, etc.)
             * from the beginning and end of the input string. The trimming is done **in place**,
             * modifying the original string, and the result is always null-terminated.
             *
             * @param str Pointer to the null-terminated string to be trimmed.
             *            Must not be NULL. If the string is empty or contains only whitespace,
             *            it will be converted to an empty string ("").
             *
             * @note This function does not allocate memory; it simply shifts characters within
             *       the existing string buffer. The caller is responsible for ensuring that
             *       the buffer is writable and large enough to hold the original string.
             *
             * @example
             * char s[] = "   hello world  \n";
             * fossil_io_trim(s);
             * // s now contains "hello world"
             */
            static void trim(char *str) {
                fossil_io_trim(str);
            }

            /**
             * Reads a line from the input stream and stores it into the buffer pointed to by 'buf'.
             *
             * @param buf           Pointer to the buffer where the line will be stored.
             * @param size          Maximum number of characters to be read, including the null terminator.
             * @param input_stream  Pointer to the input stream to read from.
             * @return              On success, the function returns 'buf'. If the end-of-file is reached or an error occurs, it returns NULL.
             */
            static char *gets_from_stream(char *buf, size_t size, fossil_io_file_t *input_stream) {
                return fossil_io_gets_from_stream(buf, size, input_stream);
            }

            /**
             * Reads a line from the input stream with error reporting.
             *
             * @param buf           Pointer to the buffer where the line will be stored.
             * @param size          Maximum number of characters to be read, including the null terminator.
             * @param input_stream  Pointer to the input stream to read from.
             * @param error_code    Pointer to an integer to store the error code (e.g., EOF, input error).
             * @return              On success, the function returns 'buf'. If the end-of-file is reached or an error occurs, it returns NULL.
             */
            static char *gets_from_stream_ex(char *buf, size_t size, fossil_io_file_t *input_stream, int *error_code) {
                return fossil_io_gets_from_stream_ex(buf, size, input_stream, error_code);
            }

            /**
             * Validates the input buffer and size before reading.
             *
             * @param buf           Pointer to the buffer where the input will be stored.
             * @param size          Size of the buffer.
             * @return              1 if the buffer and size are valid; 0 otherwise.
             */
            static int validate_input_buffer(const char *buf, size_t size) {
                return fossil_io_validate_input_buffer(buf, size);
            }

            /**
             * Reads a UTF-8 encoded line from the input stream.
             *
             * @param buf           Pointer to the buffer where the line will be stored.
             * @param size          Maximum number of characters to be read, including the null terminator.
             * @param input_stream  Pointer to the input stream to read from.
             * @return              On success, the function returns 'buf'. If the end-of-file is reached or an error occurs, it returns NULL.
             */
            static char *gets_utf8(char *buf, size_t size, fossil_io_file_t *input_stream) {
                return fossil_io_gets_utf8(buf, size, input_stream);
            }

            /**
             * Reads a line of input from standard input into the provided buffer.
             *
             * Reads at most `size - 1` characters from `stdin` and stores them in `buffer`.
             * If a newline character is read, it is replaced with a null terminator.
             *
             * @param buffer Pointer to the character buffer where input will be stored.
             * @param size The size of the buffer.
             * @return 0 on success, -1 on error or end-of-file.
             */
            int gets(char *buffer, size_t size) {
                return fossil_io_gets(buffer, size);
            }

            /**
             * Reads formatted input from the standard input stream.
             *
             * @param format        The format string specifying how the input should be interpreted.
             * @param ...           Additional arguments for storing the input values.
             * @return              On success, the number of input items successfully matched and assigned is returned.
             *                      On failure, EOF is returned.
             */
            static int scanf(const char *format, ...) {
                va_list args;
                va_start(args, format);
                int result = fossil_io_scanf(format, args);
                va_end(args);
                return result;
            }

            /**
             * Reads formatted input from the specified input stream.
             *
             * @param input_stream  Pointer to the input stream to read from.
             * @param format        The format string specifying how the input should be interpreted.
             * @param ...           Additional arguments for storing the input values.
             * @return              On success, the number of input items successfully matched and assigned is returned.
             *                      On failure, EOF is returned.
             */
            static int fscanf(fossil_io_file_t *input_stream, const char *format, ...) {
                va_list args;
                va_start(args, format);
                int result = fossil_io_fscanf(input_stream, format, args);
                va_end(args);
                return result;
            }

            /**
             * @brief Validates if the input string is a valid integer.
             * 
             * @param input The input string to validate.
             * @param output Pointer to an integer where the parsed value will be stored if valid.
             * @return true if the input is a valid integer, false otherwise.
             */
            static int validate_is_int(const char *input, int *output) {
                return fossil_io_validate_is_int(input, output);
            }

            /**
             * @brief Validates if the input string is a valid float.
             * 
             * @param input The input string to validate.
             * @param output Pointer to a float where the parsed value will be stored if valid.
             * @return true if the input is a valid float, false otherwise.
             */
            static int validate_is_float(const char *input, float *output) {
                return fossil_io_validate_is_float(input, output);
            }

            /**
             * @brief Validates if the input string contains only alphanumeric characters.
             * 
             * @param input The input string to validate.
             * @return true if the input is alphanumeric, false otherwise.
             */
            static int validate_is_alnum(const char *input) {
                return fossil_io_validate_is_alnum(input);
            }

            /**
             * @brief Validates if the input string is a valid email address.
             * 
             * @param input The input string to validate.
             * @return true if the input is a valid email address, false otherwise.
             */
            static int validate_is_email(const char *input) {
                return fossil_io_validate_is_email(input);
            }

            /**
             * @brief Validates if the input string does not exceed the specified maximum length.
             * 
             * @param input The input string to validate.
             * @param max_length The maximum allowed length of the input string.
             * @return true if the input length is within the specified limit, false otherwise.
             */
            static int validate_is_length(const char *input, size_t max_length) {
                return fossil_io_validate_is_length(input, max_length);
            }

            /**
             * Check if password is weak or bad.
             * Returns true if weak/bad, false otherwise.
             */
            static bool is_weak_password(const std::string &password,
                                       const std::string &username = "",
                                       const std::string &email = "") {
                return fossil_io_validate_is_weak_password(
                    password.c_str(),
                    username.empty() ? nullptr : username.c_str(),
                    email.empty() ? nullptr : email.c_str()) != 0;
            }
        
            /**
             * Check if a user-agent string looks like a bot/crawler
             */
            static bool is_suspicious_bot(const std::string &userAgent) {
                return fossil_io_validate_is_suspicious_bot(userAgent.c_str()) != 0;
            }
        
            /**
             * Check if an email belongs to a disposable / suspicious domain
             */
            static bool is_disposable_email(const std::string &email) {
                return fossil_io_validate_is_disposable_email(email.c_str()) != 0;
            }
        
            /**
             * Check if a string looks like a bot-style username
             */
            static bool is_suspicious_user(const std::string &username) {
                return fossil_io_validate_is_suspicious_user(username.c_str()) != 0;
            }
        
            /**
             * Sanitize a string according to context.
             * Returns the bitmask flags from the sanitizer.
             */
            static int validate_sanitize_string(std::string &input, fossil_context_t ctx) {
                std::vector<char> buffer(input.size() + 1);
                int flags = fossil_io_validate_sanitize_string(
                    input.c_str(),
                    buffer.data(),
                    buffer.size(),
                    ctx
                );
                input.assign(buffer.data());
                return flags;
            }

            /**
             * @brief Displays a menu of choices and returns the selected choice.
             * 
             * @param prompt The prompt message before displaying the menu.
             * @param choices Array of strings representing the choices.
             * @param num_choices The number of choices.
             * @return The index of the selected choice.
             */
            static int display_menu(const char *prompt, const char *choices[], int num_choices) {
                return fossil_io_display_menu(prompt, choices, num_choices);
            }

            /**
             * @brief Displays a simple progress bar.
             * 
             * @param progress The current progress (0-100).
             */
            static void show_progress(int progress) {
                fossil_io_show_progress(progress);
            }

            /**
             * @brief Overloads the input stream operator for the Input class.
             * 
             * @param input_stream The input stream to read from.
             * @param input The Input object to populate.
             * @return The input stream after reading.
             */
            friend std::istream &operator>>(std::istream &input_stream, Input & /*input*/) {
                // Implement the logic for populating the Input object from the input stream.
                // This is a placeholder implementation.
                char buffer[256];
                input_stream.getline(buffer, sizeof(buffer));
                // Process the buffer as needed to populate the Input object.
                return input_stream;
            }

        };

    }

}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
