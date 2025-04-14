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
#ifndef FOSSIL_IO_OUTPUT_H
#define FOSSIL_IO_OUTPUT_H

#include <stdarg.h>
#include "stream.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * This code provides a robust set of functions for formatting and manipulating terminal output, 
 * allowing developers to apply color, text attributes (like bold, underline, etc.), and cursor positioning 
 * to improve the visual appeal and functionality of text-based applications. The functionality is primarily 
 * based on ANSI escape sequences, which are widely supported in most terminal environments (such as Linux terminals, 
 * macOS Terminal, and Windows terminals that support ANSI escape codes).
 *
 * The core concept behind this system is a simple markup language that allows developers to specify formatting 
 * instructions within the text output. These instructions are enclosed in curly braces `{}` within the format string 
 * and are processed dynamically to change the appearance of the text. The markup supports various styles, such as:
 * 
 * 1. **Colors** - The code includes a set of predefined color codes for changing the foreground color of text. 
 *    Supported colors include basic colors (e.g., red, green, blue) and bright colors (e.g., bright red, bright green, etc.).
 *    The colors are implemented using ANSI escape sequences and can be easily extended to support more colors if needed.
 *
 * 2. **Attributes** - Text attributes can be applied to the text to change its appearance. These attributes include:
 *    - **Bold** (for making text bold)
 *    - **Underline** (for adding an underline to the text)
 *    - **Reversed** (for inverting the colors of the text and background)
 *    - **Blink** (for making text blink)
 *    - **Hidden** (for hiding the text)
 *    - **Normal** (for reverting text back to its normal form, removing any attributes)
 *
 * 3. **Positioning** - The code introduces a flexible way to manipulate the position of the text in the terminal using 
 *    named positions such as `top`, `bottom`, `left`, and `right`. These positions allow for text to be dynamically 
 *    placed at specific locations on the terminal screen, enhancing the user experience for applications requiring 
 *    more control over text layout and movement. The positions are marked with `pos:` followed by the desired 
 *    position name (e.g., `{pos:top}` or `{pos:left}`).
 * 
 * 4. **Flexibility and Extendability** - The markup language allows for the use of multiple color and attribute 
 *    specifications in a single string. The color and attribute specifications can be combined, for instance, 
 *    `{red,bold}` for red and bold text, or `{green,underline}` for green and underlined text. This allows for 
 *    fine-grained control over the text output. The system is flexible enough to be extended with more attributes, 
 *    colors, and positioning options as required.
 *
 * 5. **Implementation Details** - The function `fossil_io_print_with_attributes` processes the format string 
 *    provided to it, looking for `{}` markers. When it encounters a `{}`, it checks if the enclosed string specifies 
 *    a color, attribute, or position, and then calls the respective helper functions (`fossil_io_apply_color` and 
 *    `fossil_io_apply_attribute`) to modify the terminal output accordingly. If a position marker is found (e.g., 
 *    `{pos:top}`), it adjusts the cursor position in the terminal to the specified location. The code uses standard 
 *    C string manipulation functions like `strchr`, `strncpy`, and `vsnprintf` to process the format string and apply 
 *    the requested changes to the output.
 * 
 * In summary, this system provides a highly customizable and intuitive way to format terminal text with colors, 
 * attributes, and positions, making it ideal for developers who want to build visually rich and interactive 
 * command-line interfaces. The markup-based approach is simple to use and can be easily extended to meet the 
 * needs of more complex applications.
 */

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
void fossil_io_fputs(fossil_fstream_t *stream, const char *str);

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
void fossil_io_fprintf(fossil_fstream_t *stream, const char *format, ...);

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
             * @param stream A pointer to the fossil_fstream_t stream.
             * @param str A null-terminated C string.
             */
            static void fputs(fossil_fstream_t *stream, const char *str) {
                fossil_io_fputs(stream, str);
            }

            /**
             * @brief Prints a formatted string to a specified fossil output stream.
             *
             * @param stream A pointer to the fossil_fstream_t stream.
             * @param format A printf-style format string.
             * @param ... Additional arguments to be formatted.
             */
            static void fprintf(fossil_fstream_t *stream, const char *format, ...) {
                va_list args;
                va_start(args, format);
                fossil_io_fprintf(stream, format, args);
                va_end(args);
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
