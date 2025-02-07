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

#define FOSSIL_IO_BUFFER_SIZE 1000

// Function to apply color
void fossil_io_apply_color(const char *color) {
    if (strcmp(color, "red") == 0) {
        printf(FOSSIL_IO_COLOR_RED);
    } else if (strcmp(color, "green") == 0) {
        printf(FOSSIL_IO_COLOR_GREEN);
    } else if (strcmp(color, "yellow") == 0) {
        printf(FOSSIL_IO_COLOR_YELLOW);
    } else if (strcmp(color, "blue") == 0) {
        printf(FOSSIL_IO_COLOR_BLUE);
    } else if (strcmp(color, "magenta") == 0) {
        printf(FOSSIL_IO_COLOR_MAGENTA);
    } else if (strcmp(color, "cyan") == 0) {
        printf(FOSSIL_IO_COLOR_CYAN);
    } else if (strcmp(color, "white") == 0) {
        printf(FOSSIL_IO_COLOR_WHITE);
    }
    // Add more colors if needed
}

// Function to apply text attributes (e.g., bold, underline)
void fossil_io_apply_attribute(const char *attribute) {
    if (strcmp(attribute, "bold") == 0) {
        printf("\033[1m");
    } else if (strcmp(attribute, "underline") == 0) {
        printf("\033[4m");
    } else if (strcmp(attribute, "reset") == 0) {
        printf(FOSSIL_IO_COLOR_RESET);
    } else if (strcmp(attribute, "normal") == 0) {
        printf(FOSSIL_IO_ATTR_NORMAL);
    } else if (strcmp(attribute, "reversed") == 0) {
        printf(FOSSIL_IO_ATTR_REVERSED);
    } else if (strcmp(attribute, "blink") == 0) {
        printf(FOSSIL_IO_ATTR_BLINK);
    } else if (strcmp(attribute, "hidden") == 0) {
        printf(FOSSIL_IO_ATTR_HIDDEN);
    }
    // Add more attributes as needed
}

// Function to print text with attributes, colors, and format specifiers
void fossil_io_print_with_attributes(const char *format, ...) {
    va_list args;
    va_start(args, format);

    // Create a buffer to hold the formatted string
    char buffer[FOSSIL_IO_BUFFER_SIZE];
    vsnprintf(buffer, sizeof(buffer), format, args);

    // Variable to keep track of the current position in the buffer
    const char *current_pos = buffer;
    const char *start = NULL;
    const char *end = NULL;

    // Iterate over the buffer and process color/attribute inside `{}` and format specifiers
    while ((start = strchr(current_pos, '{')) != NULL) {
        // Print text before '{'
        printf("%.*s", (int)(start - current_pos), current_pos);
        
        // Find the matching '}'
        end = strchr(start, '}');
        if (end) {
            // Extract attributes inside '{}'
            size_t length = end - start - 1;
            char attributes[length + 1];
            strncpy(attributes, start + 1, length);
            attributes[length] = '\0';

            // Split by comma to separate color and attribute
            char *color = NULL;
            char *attribute = NULL;
            char *comma_pos = strchr(attributes, ',');
            if (comma_pos) {
                *comma_pos = '\0';  // Null-terminate the color string
                color = attributes; // Color part
                attribute = comma_pos + 1; // Attribute part
            } else {
                color = attributes; // Only one part (could be color or attribute)
            }

            // Apply color and/or attribute
            if (color) {
                fossil_io_apply_color(color);
            }
            if (attribute) {
                fossil_io_apply_attribute(attribute);
            }

            // Move past '}' and continue processing
            current_pos = end + 1;
        }
    }

    // Print remaining text after last '}'
    printf("%s", current_pos);

    va_end(args);
}

// Internal utility function for color printing
void fossil_io_print_color(const char *color, const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("%s", color);
    vprintf(format, args);
    printf("%s", FOSSIL_IO_COLOR_RESET);
    va_end(args);
}

// Function to print a sanitized string with attributes inside {}
void fossil_io_puts(const char *str) {
    if (str != NULL) {
        char sanitized_str[FOSSIL_IO_BUFFER_SIZE];
        strncpy(sanitized_str, str, sizeof(sanitized_str));
        sanitized_str[sizeof(sanitized_str) - 1] = '\0'; // Ensure null termination
        
        // Print the sanitized string with attributes
        fossil_io_print_with_attributes(sanitized_str);
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

// Function to print sanitized formatted output with attributes
void fossil_io_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    // Create a buffer to hold the formatted string
    char buffer[FOSSIL_IO_BUFFER_SIZE];
    vsnprintf(buffer, sizeof(buffer), format, args);

    // Print the sanitized output with attributes
    fossil_io_print_with_attributes(buffer);

    va_end(args);
}
