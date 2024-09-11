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
#include "fossil/io/output.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define FOSSIL_IO_BUFFER_SIZE 1024

// Internal utility function for color printing
static void fossil_io_print_color(const char *color, const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("%s", color);
    vprintf(format, args);
    printf("%s", FOSSIL_IO_COLOR_RESET);
    va_end(args);
}

// Function to print a sanitized string with a newline
void fossil_io_puts(const char *str) {
    if (str != NULL) {
        char sanitized_str[FOSSIL_IO_BUFFER_SIZE];
        strncpy(sanitized_str, str, sizeof(sanitized_str));
        sanitized_str[sizeof(sanitized_str) - 1] = '\0'; // Ensure null termination
        fossil_soap_sanitize(sanitized_str);
        puts(sanitized_str);
    } else {
        fputs("NULL\n", stderr);
    }
}

// Function to print a single character
void fossil_io_putchar(char c) {
    putchar(c);
}

// Function to print a single character in color
void fossil_io_putchar_color(char c, const char *color) {
    printf("%s%c%s", color, c, FOSSIL_IO_COLOR_RESET);
}

// Function to print sanitized formatted output
void fossil_io_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    // Create a buffer to hold the formatted string
    char buffer[FOSSIL_IO_BUFFER_SIZE];
    vsnprintf(buffer, sizeof(buffer), format, args);

    // Sanitize the buffer
    fossil_soap_sanitize(buffer);

    // Print the sanitized output
    printf("%s", buffer);

    va_end(args);
}
