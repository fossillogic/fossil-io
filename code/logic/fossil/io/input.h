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
#ifndef FOSSIL_IO_INPUT_H
#define FOSSIL_IO_INPUT_H

#include <stdarg.h>
#include "stream.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reads a single character from the input stream.
 * 
 * @param input_stream  Pointer to the input stream to read from.
 * @return              The character read as an unsigned char cast to an int, or EOF on end-of-file or error.
 */
int fossil_io_getc(fossil_fstream_t *input_stream);

/**
 * Reads a line from the input stream and stores it into the buffer pointed to by 'buf'.
 * 
 * @param buf           Pointer to the buffer where the line will be stored.
 * @param size          Maximum number of characters to be read, including the null terminator.
 * @param input_stream  Pointer to the input stream to read from.
 * @return              On success, the function returns 'buf'. If the end-of-file is reached or an error occurs, it returns NULL.
 */
char *fossil_io_gets_from_stream(char *buf, size_t size, fossil_fstream_t *input_stream);

/**
 * Reads a line from the input stream with error reporting.
 *
 * @param buf           Pointer to the buffer where the line will be stored.
 * @param size          Maximum number of characters to be read, including the null terminator.
 * @param input_stream  Pointer to the input stream to read from.
 * @param error_code    Pointer to an integer to store the error code (e.g., EOF, input error).
 * @return              On success, the function returns 'buf'. If the end-of-file is reached or an error occurs, it returns NULL.
 */
char *fossil_io_gets_from_stream_ex(char *buf, size_t size, fossil_fstream_t *input_stream, int *error_code);

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
int fossil_io_fscanf(fossil_fstream_t *input_stream, const char *format, ...);

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
char *fossil_io_gets_utf8(char *buf, size_t size, fossil_fstream_t *input_stream);

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
 * @brief Sanitizes the input string and stores the sanitized result in the output buffer.
 * 
 * @param input The input string to sanitize.
 * @param output The buffer where the sanitized string will be stored.
 * @param output_size The size of the output buffer.
 * @return A fossil_io_validate_error_t indicating the result of the sanitization process.
 */
int fossil_io_validate_sanitize_string(const char *input, char *output, size_t output_size);

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

#ifdef __cplusplus
}

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
            static int getc(fossil_fstream_t *input_stream) {
                return fossil_io_getc(input_stream);
            }

            /**
             * Reads a line from the input stream and stores it into the buffer pointed to by 'buf'.
             *
             * @param buf           Pointer to the buffer where the line will be stored.
             * @param size          Maximum number of characters to be read, including the null terminator.
             * @param input_stream  Pointer to the input stream to read from.
             * @return              On success, the function returns 'buf'. If the end-of-file is reached or an error occurs, it returns NULL.
             */
            static char *gets_from_stream(char *buf, size_t size, fossil_fstream_t *input_stream) {
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
            static char *gets_from_stream_ex(char *buf, size_t size, fossil_fstream_t *input_stream, int *error_code) {
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
            static char *gets_utf8(char *buf, size_t size, fossil_fstream_t *input_stream) {
                return fossil_io_gets_utf8(buf, size, input_stream);
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
            static int fscanf(fossil_fstream_t *input_stream, const char *format, ...) {
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
             * @brief Sanitizes the input string and stores the sanitized result in the output buffer.
             * 
             * @param input The input string to sanitize.
             * @param output The buffer where the sanitized string will be stored.
             * @param output_size The size of the output buffer.
             * @return A fossil_io_validate_error_t indicating the result of the sanitization process.
             */
            static int validate_sanitize_string(const char *input, char *output, size_t output_size) {
                return fossil_io_validate_sanitize_string(input, output, output_size);
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
