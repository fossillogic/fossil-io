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
#ifndef FOSSIL_IO_OUTPUT_H
#define FOSSIL_IO_OUTPUT_H

#include <stdarg.h>
#include "file.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int32_t FOSSIL_IO_COLOR_ENABLE; // Flag to enable/disable color output
extern int32_t FOSSIL_IO_OUTPUT_ENABLE; // Can disable output during unit testing

/**
 * -----------------------------------------------------------------------------
 * New attribute parser using '@' syntax for advanced inline terminal styling.
 *
 * This function enables rich, inline styling of terminal output by parsing and
 * applying terminal attributes using a custom '@' syntax embedded within output
 * strings. The parser interprets attribute directives and translates them into
 * the appropriate ANSI escape sequences for styling text in the terminal.
 *
 * Example usage in strings:
 *   "Normal text @color:red Red @reset Normal"
 *   "Score: @color:green 100 @reset"
 *   "Warning: @bg:yellow,attr:bold Important! @reset"
 *
 * Supported attribute types and their available options:
 *
 *   - @color:<name>
 *       Set the foreground (text) color. Only applied if color output is enabled.
 *       Available color names:
 *         black, red, green, yellow, blue, magenta, cyan, white, gray, orange,
 *         pink, purple, brown, teal, silver,
 *         bright_black, bright_red, bright_green, bright_yellow, bright_blue,
 *         bright_magenta, bright_cyan, bright_white,
 *         reset
 *
 *   - @bg:<name>
 *       Set the background color. Only applied if color output is enabled.
 *       Available background color names:
 *         black, red, green, yellow, blue, magenta, cyan, white, gray, orange,
 *         pink, purple, brown, teal, silver,
 *         bright_black, bright_red, bright_green, bright_yellow, bright_blue,
 *         bright_magenta, bright_cyan, bright_white,
 *         reset
 *
 *   - @attr:<name>
 *       Set text attributes. Multiple attributes can be specified, separated by commas.
 *       Available attribute names:
 *         bold, dim, italic, underline, blink, reverse, reversed, hidden, strikethrough,
 *         normal, reset_bold, reset_dim, reset_italic, reset_underline, reset_blink,
 *         reset_reverse, reset_hidden, reset_strike, reset
 *
 *   - @pos:<name>
 *       Move the cursor to a named position. Useful for advanced terminal UIs.
 *       Available position names:
 *         top, bottom, left, right, center, top-left, top-right, bottom-left,
 *         bottom-right, middle-left, middle-right
 *
 *   - @reset
 *       Reset all attributes and colors to their defaults.
 *
 * Multiple attributes can be combined in a single directive using commas, e.g.:
 *   "@color:red,attr:bold"
 *
 * The parser processes the string, applies the requested attributes, and
 * continues outputting the rest of the string with the new styles in effect.
 * When @reset is encountered, all styles are cleared.
 *
 * If output is disabled (FOSSIL_IO_OUTPUT_ENABLE == 0) or the input string
 * is NULL, the function prints the string "cnullptr" to stderr to indicate
 * a null pointer was encountered.
 *
 * This feature is ideal for building rich, styled terminal UIs, colored logs,
 * and dynamic output with minimal effort and without manual ANSI escape codes.
 *
 * See the implementation for details on how each attribute is handled.
 */

/**
 * Redirects the output to a specified stream.
 *
 * This function allows you to change the default output destination to a custom stream.
 * It is useful when you want to redirect output to a file or another output stream.
 *
 * @param stream The output stream where subsequent output should be redirected.
 */
void fossil_io_redirect_output(fossil_io_file_t *stream);

/**
 * Prints a string to the output.
 * 
 * This function outputs the provided string `str` to the terminal or console. It is a simple utility function
 * that can be used for printing plain text to the screen. The string is printed as-is, with no formatting or
 * color modifications applied.
 *
 * @param str The string to be printed. This should be a null-terminated string.
 */
void fossil_io_puts(const char *str);

/** 
 * Prints a formatted string to the output.
 *
 * This function allows for formatted output, similar to `printf`. It takes a format string that can include
 * format specifiers (e.g., `%d`, `%s`, `%f`), and the additional arguments provided will be formatted accordingly.
 * The function uses a variable argument list (`...`) to handle a wide variety of format specifiers and argument types.
 * 
 * The format string can also include custom formatting markers enclosed in curly braces `{}`, such as `{red}` for
 * color or `{bold}` for text attributes, which will be processed and applied to the output.
 *
 * Example usage:
 * ```c
 * fossil_io_printf("Hello, %s! Your score is %d\n", "Alice", 95);
 * ```
 *
 * @param format The format string, which contains the text to be printed, along with format specifiers.
 * @param ... The additional arguments to be formatted. These arguments are inserted into the format string
 *            in the order they appear, based on the format specifiers.
 */
void fossil_io_printf(const char *format, ...);

/**
 * Prints a character to the output.
 * 
 * This function is a basic utility to print a single character to the output. It is especially useful when you
 * need to print individual characters rather than strings or formatted text.
 *
 * Example usage:
 * ```c
 * fossil_io_putchar('A');
 * ```
 *
 * @param c The character to be printed. This should be a single character.
 */
void fossil_io_putchar(char c);

/**
 * Prints a string to the specified output stream.
 * 
 * This function is similar to `fossil_io_puts`, but instead of printing to the standard output, it allows you
 * to specify an output stream (like a file or a custom output stream). This can be useful when writing to files
 * or other output destinations.
 *
 * Example usage:
 * ```c
 * FILE *file = fopen("output.txt", "w");
 * fossil_io_fputs(file, "Hello, File Output!\n");
 * fclose(file);
 * ```
 *
 * @param stream The output stream where the string should be printed. This should be a valid pointer to a `FILE` object.
 * @param str The string to be printed. This should be a null-terminated string.
 */
void fossil_io_fputs(fossil_io_file_t *stream, ccstring str);

/**
 * Prints a formatted string to the specified output stream.
 * 
 * This function is similar to `fossil_io_printf`, but instead of printing to the standard output, it allows you
 * to specify an output stream. The format string can include format specifiers and custom formatting markers, just
 * like `fossil_io_printf`. This can be useful when writing formatted text to files or other output destinations.
 *
 * Example usage:
 * ```c
 * FILE *file = fopen("output.txt", "w");
 * fossil_io_fprintf(file, "Hello, %s! Your score is %d\n", "Alice", 95);
 * fclose(file);
 * ```
 *
 * @param stream The output stream where the formatted string should be printed. This should be a valid pointer to a `FILE` object.
 * @param format The format string, which contains the text to be printed, along with format specifiers.
 * @param ... The additional arguments to be formatted. These arguments are inserted into the format string
 *            in the order they appear, based on the format specifiers.
 */
void fossil_io_fprintf(fossil_io_file_t *stream, const char *format, ...);

/**
 * Formats a string and stores it in the provided buffer.
 *
 * This function works like snprintf, formatting the string according to the format specifiers
 * and storing the result in the buffer. It ensures that no more than `size` characters are written,
 * including the null terminator.
 *
 * @param buffer The buffer where the formatted string will be stored.
 * @param size The size of the buffer.
 * @param format The format string.
 * @param ... Additional arguments to be formatted.
 * @return The number of characters written (excluding the null terminator), or a negative value on error.
 */
int fossil_io_snprintf(char *buffer, size_t size, const char *format, ...);

// TUI part of the API

/**
 * Clears the terminal screen.
 *
 * This function sends the ANSI escape sequence to clear the terminal screen
 * and move the cursor to the top-left corner. It is useful when creating full-screen
 * terminal applications or refreshing the display.
 */
void fossil_io_clear_screen(void);

/**
 * Moves the cursor to a specific row and column on the terminal.
 *
 * @param row The row position (starting from 1).
 * @param col The column position (starting from 1).
 */
void fossil_io_move_cursor(int row, int col);

/**
 * Hides the cursor from the terminal screen.
 *
 * This is useful for creating cleaner UIs without a blinking cursor.
 */
void fossil_io_hide_cursor(void);

/**
 * Shows the cursor on the terminal screen.
 */
void fossil_io_show_cursor(void);

/**
 * Draws a horizontal line using a specified character.
 *
 * @param length The number of characters to draw.
 * @param ch The character to use for drawing.
 */
void fossil_io_draw_horizontal_line(int length, char ch);

/**
 * Draws a vertical line using a specified character.
 *
 * @param length The number of characters to draw.
 * @param ch The character to use for drawing.
 */
void fossil_io_draw_vertical_line(int length, char ch);

/**
 * Flushes the output stream, ensuring all buffered text is written.
 *
 * Useful when mixing multiple output functions or when printing from threads.
 */
void fossil_io_flush(void);

#ifdef __cplusplus
}

#include <string>
#include <sstream>
#include <cstdarg>
#include <iostream>

namespace fossil {

    namespace io {

        /**
         * @class Output
         * @brief C++ wrapper class for fossil_io output and terminal control functions.
         *
         * Provides convenient static methods and stream-style operators for
         * writing text output and controlling terminal behavior.
         */
        class Output {
        public:
            /**
             * @brief Default constructor.
             */
            Output() = default;

            /**
             * @brief Writes a null-terminated C string to the default output stream.
             *
             * @param str Pointer to the null-terminated string to be printed.
             */
            static void puts(const char *str) {
                fossil_io_puts(str);
            }

            /**
             * @brief Writes an STL string to the default output stream.
             *
             * @param str The std::string to be printed.
             */
            static void puts(const std::string &str) {
                fossil_io_puts(str.c_str());
            }

            /**
             * @brief Prints a formatted string using a format specifier and variadic arguments.
             *
             * @param format A printf-style format string.
             * @param ... Additional arguments to be formatted according to the format string.
             */
            static void printf(const char *format, ...) {
                va_list args;
                va_start(args, format);
                fossil_io_printf(format, args);
                va_end(args);
            }

            /**
             * @brief Prints a single character to the default output stream.
             *
             * @param c The character to be printed.
             */
            static void putchar(char c) {
                fossil_io_putchar(c);
            }

            /**
             * @brief Writes a null-terminated string to a specified fossil output stream.
             *
             * @param stream A pointer to the fossil_io_file_t stream.
             * @param str A null-terminated C string.
             */
            static void fputs(fossil_io_file_t *stream, const char *str) {
                fossil_io_fputs(stream, str);
            }

            /**
             * @brief Prints a formatted string to a specified fossil output stream.
             *
             * @param stream A pointer to the fossil_io_file_t stream.
             * @param format A printf-style format string.
             * @param ... Additional arguments to be formatted.
             */
            static void fprintf(fossil_io_file_t *stream, const char *format, ...) {
                va_list args;
                va_start(args, format);
                fossil_io_fprintf(stream, format, args);
                va_end(args);
            }

            /**
             * @brief Formats a string and stores it in the provided buffer.
             *
             * This method works like snprintf, formatting the string according to the format specifiers
             * and storing the result in the buffer. It ensures that no more than `size` characters are written,
             * including the null terminator.
             *
             * @param buffer The buffer where the formatted string will be stored.
             * @param size The size of the buffer.
             * @param format The format string.
             * @param ... Additional arguments to be formatted.
             * @return The number of characters written (excluding the null terminator), or a negative value on error.
             */
            static int snprintf(char *buffer, size_t size, const char *format, ...) {
                va_list args;
                va_start(args, format);
                int result = fossil_io_snprintf(buffer, size, format, args);
                va_end(args);
                return result;
            }

            /**
             * @brief Stream operator for outputting an STL string.
             *
             * @param str The string to output.
             * @return Reference to this Output object.
             */
            Output& operator<<(const std::string& str) {
                fossil_io_puts(str.c_str());
                return *this;
            }

            /**
             * @brief Stream operator for outputting a C-style string.
             *
             * @param str The null-terminated C string to output.
             * @return Reference to this Output object.
             */
            Output& operator<<(const char* str) {
                fossil_io_puts(str);
                return *this;
            }

            /**
             * @brief Stream operator for outputting a single character.
             *
             * @param c The character to output.
             * @return Reference to this Output object.
             */
            Output& operator<<(char c) {
                fossil_io_putchar(c);
                return *this;
            }

            /**
             * @brief Stream operator for outputting an integer.
             *
             * @param value The integer to output.
             * @return Reference to this Output object.
             */
            Output& operator<<(int value) {
                std::stringstream ss;
                ss << value;
                fossil_io_puts(ss.str().c_str());
                return *this;
            }

            /**
             * @brief Stream operator for outputting a double.
             *
             * @param value The double-precision floating point number to output.
             * @return Reference to this Output object.
             */
            Output& operator<<(double value) {
                std::stringstream ss;
                ss << value;
                fossil_io_puts(ss.str().c_str());
                return *this;
            }

            /**
             * @brief Stream operator for outputting a float.
             *
             * @param value The single-precision floating point number to output.
             * @return Reference to this Output object.
             */
            Output& operator<<(float value) {
                return (*this) << static_cast<double>(value);
            }

            /**
             * @brief Stream operator for handling manipulators (like std::endl).
             *
             * @param manip A stream manipulator function.
             * @return Reference to this Output object.
             */
            Output& operator<<(std::ostream& (*manip)(std::ostream&)) {
                if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) {
                    fossil_io_putchar('\n');
                }
                return *this;
            }

            // ------------------
            // TUI CONTROL METHODS
            // ------------------

            /**
             * @brief Clears the terminal screen and moves the cursor to the top-left corner.
             *
             * Sends the ANSI escape sequence to perform a full screen clear.
             */
            static void clear_screen() {
                fossil_io_clear_screen();
            }

            /**
             * @brief Moves the terminal cursor to the specified position.
             *
             * @param row The 1-based row number.
             * @param col The 1-based column number.
             */
            static void move_cursor(int row, int col) {
                fossil_io_move_cursor(row, col);
            }

            /**
             * @brief Hides the terminal cursor.
             *
             * Useful when rendering full-screen UIs or animations.
             */
            static void hide_cursor() {
                fossil_io_hide_cursor();
            }

            /**
             * @brief Shows the terminal cursor if previously hidden.
             */
            static void show_cursor() {
                fossil_io_show_cursor();
            }

            /**
             * @brief Draws a horizontal line using a specified character.
             *
             * @param length The number of times to repeat the character.
             * @param ch The character to draw.
             */
            static void draw_horizontal_line(int length, char ch) {
                fossil_io_draw_horizontal_line(length, ch);
            }

            /**
             * @brief Draws a vertical line using a specified character.
             *
             * @param length The number of times to repeat the character vertically.
             * @param ch The character to draw.
             */
            static void draw_vertical_line(int length, char ch) {
                fossil_io_draw_vertical_line(length, ch);
            }

            /**
             * @brief Flushes the output stream.
             *
             * Ensures all buffered output is written immediately.
             */
            static void flush() {
                fossil_io_flush();
            }
        };

        /**
         * @brief Global output instance for stream-style usage.
         *
         * Example:
         * @code
         *     fossil::io::out << "Hello, World!" << std::endl;
         * @endcode
         */
        inline Output out;

    } // namespace io

} // namespace fossil


#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
