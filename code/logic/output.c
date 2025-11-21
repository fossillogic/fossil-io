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
#include "fossil/io/output.h"
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int32_t FOSSIL_IO_COLOR_ENABLE = 1; // Flag to enable/disable color output

// Define color codes for output
#define FOSSIL_IO_COLOR_RESET       "\033[0m"
#define FOSSIL_IO_COLOR_RED         "\033[31m"
#define FOSSIL_IO_COLOR_GREEN       "\033[32m"
#define FOSSIL_IO_COLOR_YELLOW      "\033[33m"
#define FOSSIL_IO_COLOR_BLUE        "\033[34m"
#define FOSSIL_IO_COLOR_MAGENTA     "\033[35m"
#define FOSSIL_IO_COLOR_CYAN        "\033[36m"
#define FOSSIL_IO_COLOR_WHITE       "\033[37m"

// Bright colors
#define FOSSIL_IO_COLOR_BRIGHT_RED   "\033[91m"
#define FOSSIL_IO_COLOR_BRIGHT_GREEN "\033[92m"
#define FOSSIL_IO_COLOR_BRIGHT_YELLOW "\033[93m"
#define FOSSIL_IO_COLOR_BRIGHT_BLUE  "\033[94m"
#define FOSSIL_IO_COLOR_BRIGHT_MAGENTA "\033[95m"
#define FOSSIL_IO_COLOR_BRIGHT_CYAN  "\033[96m"
#define FOSSIL_IO_COLOR_BRIGHT_WHITE "\033[97m"

// Define text attributes
#define FOSSIL_IO_ATTR_BOLD         "\033[1m"
#define FOSSIL_IO_ATTR_UNDERLINE    "\033[4m"
#define FOSSIL_IO_ATTR_REVERSED     "\033[7m"
#define FOSSIL_IO_ATTR_BLINK        "\033[5m"
#define FOSSIL_IO_ATTR_HIDDEN       "\033[8m"
#define FOSSIL_IO_ATTR_NORMAL       "\033[22m" // For reverting to normal text

// Additional attributes
#define FOSSIL_IO_ATTR_ITALIC       "\033[3m"
#define FOSSIL_IO_ATTR_STRIKETHROUGH "\033[9m"

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
    // Bright colors
    else if (strcmp(color, "bright_red") == 0) {
        printf(FOSSIL_IO_COLOR_BRIGHT_RED);
    } else if (strcmp(color, "bright_green") == 0) {
        printf(FOSSIL_IO_COLOR_BRIGHT_GREEN);
    } else if (strcmp(color, "bright_yellow") == 0) {
        printf(FOSSIL_IO_COLOR_BRIGHT_YELLOW);
    } else if (strcmp(color, "bright_blue") == 0) {
        printf(FOSSIL_IO_COLOR_BRIGHT_BLUE);
    } else if (strcmp(color, "bright_magenta") == 0) {
        printf(FOSSIL_IO_COLOR_BRIGHT_MAGENTA);
    } else if (strcmp(color, "bright_cyan") == 0) {
        printf(FOSSIL_IO_COLOR_BRIGHT_CYAN);
    } else if (strcmp(color, "bright_white") == 0) {
        printf(FOSSIL_IO_COLOR_BRIGHT_WHITE);
    } else if (strcmp(color, "reset") == 0) {
        printf(FOSSIL_IO_COLOR_RESET); // Reset to default if color not recognized
    }
}

// Function to apply text attributes (e.g., bold, underline)
void fossil_io_apply_attribute(const char *attribute) {
    if (strcmp(attribute, "bold") == 0) {
        printf(FOSSIL_IO_ATTR_BOLD);
    } else if (strcmp(attribute, "underline") == 0) {
        printf(FOSSIL_IO_ATTR_UNDERLINE);
    } else if (strcmp(attribute, "reversed") == 0) {
        printf(FOSSIL_IO_ATTR_REVERSED);
    } else if (strcmp(attribute, "blink") == 0) {
        printf(FOSSIL_IO_ATTR_BLINK);
    } else if (strcmp(attribute, "hidden") == 0) {
        printf(FOSSIL_IO_ATTR_HIDDEN);
    } else if (strcmp(attribute, "normal") == 0) {
        printf(FOSSIL_IO_ATTR_NORMAL);
    } else if (strcmp(attribute, "italic") == 0) {
        printf(FOSSIL_IO_ATTR_ITALIC);
    } else if (strcmp(attribute, "strikethrough") == 0) {
        printf(FOSSIL_IO_ATTR_STRIKETHROUGH);
    } else if (strcmp(attribute, "reset") == 0) {
        printf(FOSSIL_IO_ATTR_NORMAL); // Reset to normal if attribute not recognized
    }
}

// Function to handle named positions (like top, bottom, left, right)
void fossil_io_apply_position(const char *pos) {
    if (strcmp(pos, "top") == 0) {
        printf("\033[1;1H"); // Move to top
    } else if (strcmp(pos, "bottom") == 0) {
        printf("\033[1000;1H"); // Move cursor to bottom-left
    } else if (strcmp(pos, "left") == 0) {
        printf("\033[1;1H"); // Move to top-left (as a general left start)
    } else if (strcmp(pos, "right") == 0) {
        printf("\033[1;1000H"); // Move to top-right
    } else if (strcmp(pos, "center") == 0) {
        printf("\033[25;40H"); // Approximate center for 80x50 terminal
    } else if (strcmp(pos, "top-left") == 0) {
        printf("\033[1;1H");
    } else if (strcmp(pos, "top-right") == 0) {
        printf("\033[1;1000H");
    } else if (strcmp(pos, "bottom-left") == 0) {
        printf("\033[1000;1H");
    } else if (strcmp(pos, "bottom-right") == 0) {
        printf("\033[1000;1000H");
    } else if (strcmp(pos, "middle-left") == 0) {
        printf("\033[25;1H"); // Mid vertical, far left
    } else if (strcmp(pos, "middle-right") == 0) {
        printf("\033[25;1000H"); // Mid vertical, far right
    } else {
        fprintf(stderr, "Unknown position: %s\n", pos);
    }
}

// Function to print text with attributes, colors, positions, and format specifiers
void fossil_io_print_with_attributes(const char *str) {
    if (str == NULL) {
        fputs("cnullptr\n", stderr);
        return;
    }

    const char *current_pos = str;
    const char *start = NULL;
    const char *end = NULL;

    while ((start = strchr(current_pos, '{')) != NULL) {
        // Output text before '{'
        fwrite(current_pos, 1, start - current_pos, stdout);

        // Find the matching '}'
        end = strchr(start, '}');
        if (end) {
            // Extract attributes inside '{}'
            size_t length = end - start - 1;
            char attributes[length + 1];
            strncpy(attributes, start + 1, length);
            attributes[length] = '\0';

            // Split by comma to separate color, attribute, or position
            char *color = NULL;
            char *attribute = NULL;
            char *pos = NULL;
            char *comma_pos = strchr(attributes, ',');
            if (comma_pos) {
                *comma_pos = '\0';  // Null-terminate the first part
                color = attributes; // Color or position part
                attribute = comma_pos + 1; // Attribute part
            } else {
                color = attributes; // Only one part (could be color, attribute, or position)
            }

            // Handle positions (like {pos:name})
            if (strstr(color, "pos:") == color) {
                pos = color + 4; // Skip the "pos:" prefix
                fossil_io_apply_position(pos);
            } else {
                // Apply color and/or attribute based on flags
                if (FOSSIL_IO_COLOR_ENABLE && color) {
                    fossil_io_apply_color(color);
                }
                if (attribute) {
                    fossil_io_apply_attribute(attribute);
                }
            }

            // Move past '}' and continue processing
            current_pos = end + 1;
        } else {
            // No matching '}', print the rest and break
            fputs(start, stdout);
            break;
        }
    }

    // Output remaining text after last '}'
    fputs(current_pos, stdout);
    fflush(stdout);
}

// Function to print a sanitized formatted string to a specific file stream with attributes
void fossil_io_fprint_with_attributes(fossil_io_file_t *stream, const char *str) {
    if (str != NULL && stream != NULL) {
        char sanitized_str[FOSSIL_IO_BUFFER_SIZE];
        strncpy(sanitized_str, str, sizeof(sanitized_str));
        sanitized_str[sizeof(sanitized_str) - 1] = '\0'; // Ensure null termination

        // Remove attribute/color escape codes for file output
        const char *current_pos = sanitized_str;
        const char *start = NULL;
        const char *end = NULL;
        while ((start = strchr(current_pos, '{')) != NULL) {
            // Write text before '{' to the file
            fwrite(current_pos, 1, start - current_pos, stream->file);
            end = strchr(start, '}');
            if (end) {
                // Skip the attribute section
                current_pos = end + 1;
            } else {
                // No matching '}', write the rest and break
                fputs(start, stream->file);
                break;
            }
        }
        // Write remaining text after last '}'
        fputs(current_pos, stream->file);
    } else {
        //fputs("cnullptr\n", stderr);
    }
}

//
// OUTPUT FUNCTIONS
//

// Function to print a sanitized string with attributes inside {}
void fossil_io_puts(const char *str) {
    if (str != NULL) {
        char sanitized_str[FOSSIL_IO_BUFFER_SIZE];
        strncpy(sanitized_str, str, sizeof(sanitized_str));
        sanitized_str[sizeof(sanitized_str) - 1] = '\0'; // Ensure null termination
        
        // Print the sanitized string with attributes
        fossil_io_print_with_attributes(sanitized_str);
    } else {
        fputs("cnullptr\n", stderr);
    }
}

// Function to print a single character
void fossil_io_putchar(char c) {
    putchar(c);
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

// Function to print a sanitized string to a specific file stream
void fossil_io_fputs(fossil_io_file_t *stream, const char *str) {
    if (str != NULL && stream != NULL) {
        char sanitized_str[FOSSIL_IO_BUFFER_SIZE];
        strncpy(sanitized_str, str, sizeof(sanitized_str));
        sanitized_str[sizeof(sanitized_str) - 1] = '\0'; // Ensure null termination

        // Apply color/attributes and sanitize the string before printing
        fossil_io_fprint_with_attributes(stream, sanitized_str);
    } else {
        fputs("cnullptr\n", stderr);
    }
}

// Function to print a sanitized formatted string to a specific file stream
void fossil_io_fprintf(fossil_io_file_t *stream, const char *format, ...) {
    va_list args;
    va_start(args, format);

    // Create a buffer to hold the formatted string
    char buffer[FOSSIL_IO_BUFFER_SIZE];
    vsnprintf(buffer, sizeof(buffer), format, args);

    // Print the sanitized formatted string with attributes to the specified stream
    fossil_io_fprint_with_attributes(stream, buffer);

    va_end(args);
}

// TUI PART

void fossil_io_clear_screen(void) {
    printf("\033[2J\033[H");
}

void fossil_io_move_cursor(int row, int col) {
    printf("\033[%d;%dH", row, col);
}

void fossil_io_hide_cursor(void) {
    printf("\033[?25l");
}

void fossil_io_show_cursor(void) {
    printf("\033[?25h");
}

void fossil_io_draw_horizontal_line(int length, char ch) {
    for (int i = 0; i < length; ++i) {
        putchar(ch);
    }
    putchar('\n');
}

void fossil_io_draw_vertical_line(int length, char ch) {
    for (int i = 0; i < length; ++i) {
        putchar(ch);
        putchar('\n');
    }
}

void fossil_io_flush(void) {
    fflush(stdout);
}
