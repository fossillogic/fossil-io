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

#ifdef __cplusplus
extern "C" {
#endif

// Define color codes for output
#define FOSSIL_IO_COLOR_RESET       "\033[0m"
#define FOSSIL_IO_COLOR_RED         "\033[31m"
#define FOSSIL_IO_COLOR_GREEN       "\033[32m"
#define FOSSIL_IO_COLOR_YELLOW      "\033[33m"
#define FOSSIL_IO_COLOR_BLUE        "\033[34m"
#define FOSSIL_IO_COLOR_MAGENTA     "\033[35m"
#define FOSSIL_IO_COLOR_CYAN        "\033[36m"
#define FOSSIL_IO_COLOR_WHITE       "\033[37m"

// Define text attributes
#define FOSSIL_IO_ATTR_BOLD         "\033[1m"
#define FOSSIL_IO_ATTR_UNDERLINE    "\033[4m"
#define FOSSIL_IO_ATTR_REVERSED     "\033[7m"
#define FOSSIL_IO_ATTR_BLINK        "\033[5m"
#define FOSSIL_IO_ATTR_HIDDEN       "\033[8m"
#define FOSSIL_IO_ATTR_NORMAL       "\033[22m" // For reverting to normal text

/**
 * Prints a string to the output.
 *
 * @param str The string to be printed.
 */
void fossil_io_puts(const char *str);

/**
 * Prints a formatted string to the output.
 *
 * @param format The format string.
 * @param ... The additional arguments to be formatted.
 */
void fossil_io_printf(const char *format, ...);

/**
 * Prints a string to the output with a specified color.
 *
 * @param color The color code to be applied.
 * @param str The string to be printed.
 */
void fossil_io_print_color(const char *color, const char *format, ...);

/**
 * Prints a character to the output.
 *
 * @param c The character to be printed.
 */
void fossil_io_putchar(char c);

/**
 * Prints a character to the output with a specified color.
 *
 * @param c The character to be printed.
 * @param color The color code to be applied.
 */
void fossil_io_putchar_color(char c, const char *color);

#ifdef __cplusplus
}
/**
 * C++ wrapper for the output functions.
 */
namespace fossil {
    /**
     * Namespace for input/output operations.
     */
    namespace io {
        /**
         * Class for output operations.
         */
        class Output {
        public:
            /**
             * Prints a string to the output.
             *
             * @param str The string to be printed.
             */
            static void puts(const char *str) {
                fossil_io_puts(str);
            }

            /**
             * Prints a formatted string to the output.
             *
             * @param format The format string.
             * @param ... The additional arguments to be formatted.
             */
            static void printf(const char *format, ...) {
                va_list args;
                va_start(args, format);
                fossil_io_printf(format, args);
                va_end(args);
            }

            /**
             * Prints a string to the output with a specified color.
             *
             * @param color The color code to be applied.
             * @param str The string to be printed.
             */
            static void print_color(const char *color, const char *format, ...) {
                va_list args;
                va_start(args, format);
                fossil_io_print_color(color, format, args);
                va_end(args);
            }

            /**
             * Prints a character to the output.
             *
             * @param c The character to be printed.
             */
            static void putchar(char c) {
                fossil_io_putchar(c);
            }

            /**
             * Prints a character to the output with a specified color.
             *
             * @param c The character to be printed.
             * @param color The color code to be applied.
             */
            static void putchar_color(char c, const char *color) {
                fossil_io_putchar_color(c, color);
            }
        }
    }
}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
