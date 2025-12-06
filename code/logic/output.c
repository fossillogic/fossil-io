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
int32_t FOSSIL_IO_OUTPUT_ENABLE = 1; // Can disable output during unit testing 

// ================================================================
// RESET
// ================================================================
#define FOSSIL_IO_COLOR_RESET            "\033[0m"


// ================================================================
// DARK (STANDARD) COLORS
// ================================================================
#define FOSSIL_IO_COLOR_BLACK            "\033[30m"
#define FOSSIL_IO_COLOR_RED              "\033[31m"
#define FOSSIL_IO_COLOR_GREEN            "\033[32m"
#define FOSSIL_IO_COLOR_YELLOW           "\033[33m"
#define FOSSIL_IO_COLOR_BLUE             "\033[34m"
#define FOSSIL_IO_COLOR_MAGENTA          "\033[35m"
#define FOSSIL_IO_COLOR_CYAN             "\033[36m"
#define FOSSIL_IO_COLOR_WHITE            "\033[37m"
#define FOSSIL_IO_COLOR_GRAY             "\033[90m"
#define FOSSIL_IO_COLOR_ORANGE           "\033[38;5;208m"
#define FOSSIL_IO_COLOR_PINK             "\033[38;5;205m"
#define FOSSIL_IO_COLOR_PURPLE           "\033[38;5;93m"
#define FOSSIL_IO_COLOR_BROWN            "\033[38;5;94m"
#define FOSSIL_IO_COLOR_TEAL             "\033[38;5;30m"
#define FOSSIL_IO_COLOR_SILVER           "\033[38;5;7m"

// ================================================================
// BRIGHT COLORS
// ================================================================
#define FOSSIL_IO_COLOR_BRIGHT_BLACK     "\033[90m"
#define FOSSIL_IO_COLOR_BRIGHT_RED       "\033[91m"
#define FOSSIL_IO_COLOR_BRIGHT_GREEN     "\033[92m"
#define FOSSIL_IO_COLOR_BRIGHT_YELLOW    "\033[93m"
#define FOSSIL_IO_COLOR_BRIGHT_BLUE      "\033[94m"
#define FOSSIL_IO_COLOR_BRIGHT_MAGENTA   "\033[95m"
#define FOSSIL_IO_COLOR_BRIGHT_CYAN      "\033[96m"
#define FOSSIL_IO_COLOR_BRIGHT_WHITE     "\033[97m"

// ================================================================
// BACKGROUND COLORS
// ================================================================
#define FOSSIL_IO_BG_BLACK            "\033[40m"
#define FOSSIL_IO_BG_RED              "\033[41m"
#define FOSSIL_IO_BG_GREEN            "\033[42m"
#define FOSSIL_IO_BG_YELLOW           "\033[43m"
#define FOSSIL_IO_BG_BLUE             "\033[44m"
#define FOSSIL_IO_BG_MAGENTA          "\033[45m"
#define FOSSIL_IO_BG_CYAN             "\033[46m"
#define FOSSIL_IO_BG_WHITE            "\033[47m"
#define FOSSIL_IO_BG_GRAY             "\033[100m"
#define FOSSIL_IO_BG_ORANGE           "\033[48;5;208m"
#define FOSSIL_IO_BG_PINK             "\033[48;5;205m"
#define FOSSIL_IO_BG_PURPLE           "\033[48;5;93m"
#define FOSSIL_IO_BG_BROWN            "\033[48;5;94m"
#define FOSSIL_IO_BG_TEAL             "\033[48;5;30m"
#define FOSSIL_IO_BG_SILVER           "\033[48;5;7m"

// Bright background colors
#define FOSSIL_IO_BG_BRIGHT_BLACK     "\033[100m"
#define FOSSIL_IO_BG_BRIGHT_RED       "\033[101m"
#define FOSSIL_IO_BG_BRIGHT_GREEN     "\033[102m"
#define FOSSIL_IO_BG_BRIGHT_YELLOW    "\033[103m"
#define FOSSIL_IO_BG_BRIGHT_BLUE      "\033[104m"
#define FOSSIL_IO_BG_BRIGHT_MAGENTA   "\033[105m"
#define FOSSIL_IO_BG_BRIGHT_CYAN      "\033[106m"
#define FOSSIL_IO_BG_BRIGHT_WHITE     "\033[107m"

// ================================================================
// TEXT ATTRIBUTES
// ================================================================
#define FOSSIL_IO_ATTR_BOLD              "\033[1m"
#define FOSSIL_IO_ATTR_DIM               "\033[2m"
#define FOSSIL_IO_ATTR_ITALIC            "\033[3m"
#define FOSSIL_IO_ATTR_UNDERLINE         "\033[4m"
#define FOSSIL_IO_ATTR_BLINK             "\033[5m"
#define FOSSIL_IO_ATTR_REVERSE           "\033[7m"
#define FOSSIL_IO_ATTR_HIDDEN            "\033[8m"
#define FOSSIL_IO_ATTR_STRIKETHROUGH     "\033[9m"
#define FOSSIL_IO_ATTR_NORMAL            "\033[22;23;24;25;27;28m" // Reset all attributes
#define FOSSIL_IO_ATTR_REVERSED          "\033[7m"

// Reset specific attributes
#define FOSSIL_IO_ATTR_RESET_BOLD        "\033[22m"
#define FOSSIL_IO_ATTR_RESET_DIM         "\033[22m"
#define FOSSIL_IO_ATTR_RESET_ITALIC      "\033[23m"
#define FOSSIL_IO_ATTR_RESET_UNDERLINE   "\033[24m"
#define FOSSIL_IO_ATTR_RESET_BLINK       "\033[25m"
#define FOSSIL_IO_ATTR_RESET_REVERSE     "\033[27m"
#define FOSSIL_IO_ATTR_RESET_HIDDEN      "\033[28m"
#define FOSSIL_IO_ATTR_RESET_STRIKE      "\033[29m"

#define FOSSIL_IO_BUFFER_SIZE 1000

// Function to apply background color
void fossil_io_apply_bg_color(const char *bg_color) {
    if (strcmp(bg_color, "black") == 0) {
        printf(FOSSIL_IO_BG_BLACK);
    } else if (strcmp(bg_color, "red") == 0) {
        printf(FOSSIL_IO_BG_RED);
    } else if (strcmp(bg_color, "green") == 0) {
        printf(FOSSIL_IO_BG_GREEN);
    } else if (strcmp(bg_color, "yellow") == 0) {
        printf(FOSSIL_IO_BG_YELLOW);
    } else if (strcmp(bg_color, "blue") == 0) {
        printf(FOSSIL_IO_BG_BLUE);
    } else if (strcmp(bg_color, "magenta") == 0) {
        printf(FOSSIL_IO_BG_MAGENTA);
    } else if (strcmp(bg_color, "cyan") == 0) {
        printf(FOSSIL_IO_BG_CYAN);
    } else if (strcmp(bg_color, "white") == 0) {
        printf(FOSSIL_IO_BG_WHITE);
    } else if (strcmp(bg_color, "gray") == 0) {
        printf(FOSSIL_IO_BG_GRAY);
    } else if (strcmp(bg_color, "orange") == 0) {
        printf(FOSSIL_IO_BG_ORANGE);
    } else if (strcmp(bg_color, "pink") == 0) {
        printf(FOSSIL_IO_BG_PINK);
    } else if (strcmp(bg_color, "purple") == 0) {
        printf(FOSSIL_IO_BG_PURPLE);
    } else if (strcmp(bg_color, "brown") == 0) {
        printf(FOSSIL_IO_BG_BROWN);
    } else if (strcmp(bg_color, "teal") == 0) {
        printf(FOSSIL_IO_BG_TEAL);
    } else if (strcmp(bg_color, "silver") == 0) {
        printf(FOSSIL_IO_BG_SILVER);
    }
    // Bright background colors
    else if (strcmp(bg_color, "bright_black") == 0) {
        printf(FOSSIL_IO_BG_BRIGHT_BLACK);
    } else if (strcmp(bg_color, "bright_red") == 0) {
        printf(FOSSIL_IO_BG_BRIGHT_RED);
    } else if (strcmp(bg_color, "bright_green") == 0) {
        printf(FOSSIL_IO_BG_BRIGHT_GREEN);
    } else if (strcmp(bg_color, "bright_yellow") == 0) {
        printf(FOSSIL_IO_BG_BRIGHT_YELLOW);
    } else if (strcmp(bg_color, "bright_blue") == 0) {
        printf(FOSSIL_IO_BG_BRIGHT_BLUE);
    } else if (strcmp(bg_color, "bright_magenta") == 0) {
        printf(FOSSIL_IO_BG_BRIGHT_MAGENTA);
    } else if (strcmp(bg_color, "bright_cyan") == 0) {
        printf(FOSSIL_IO_BG_BRIGHT_CYAN);
    } else if (strcmp(bg_color, "bright_white") == 0) {
        printf(FOSSIL_IO_BG_BRIGHT_WHITE);
    } else if (strcmp(bg_color, "reset") == 0) {
        printf(FOSSIL_IO_COLOR_RESET);
    }
}

// Function to apply color
void fossil_io_apply_color(const char *color) {
    if (strcmp(color, "black") == 0) {
        printf(FOSSIL_IO_COLOR_BLACK);
    } else if (strcmp(color, "red") == 0) {
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
    } else if (strcmp(color, "gray") == 0) {
        printf(FOSSIL_IO_COLOR_GRAY);
    } else if (strcmp(color, "orange") == 0) {
        printf(FOSSIL_IO_COLOR_ORANGE);
    } else if (strcmp(color, "pink") == 0) {
        printf(FOSSIL_IO_COLOR_PINK);
    } else if (strcmp(color, "purple") == 0) {
        printf(FOSSIL_IO_COLOR_PURPLE);
    } else if (strcmp(color, "brown") == 0) {
        printf(FOSSIL_IO_COLOR_BROWN);
    } else if (strcmp(color, "teal") == 0) {
        printf(FOSSIL_IO_COLOR_TEAL);
    } else if (strcmp(color, "silver") == 0) {
        printf(FOSSIL_IO_COLOR_SILVER);
    }
    // Bright colors
    else if (strcmp(color, "bright_black") == 0) {
        printf(FOSSIL_IO_COLOR_BRIGHT_BLACK);
    } else if (strcmp(color, "bright_red") == 0) {
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

// Function to apply text attributes (e.g., bold, underline, dim, etc.)
void fossil_io_apply_attribute(const char *attribute) {
    if (strcmp(attribute, "bold") == 0) {
        printf(FOSSIL_IO_ATTR_BOLD);
    } else if (strcmp(attribute, "dim") == 0) {
        printf(FOSSIL_IO_ATTR_DIM);
    } else if (strcmp(attribute, "italic") == 0) {
        printf(FOSSIL_IO_ATTR_ITALIC);
    } else if (strcmp(attribute, "underline") == 0) {
        printf(FOSSIL_IO_ATTR_UNDERLINE);
    } else if (strcmp(attribute, "blink") == 0) {
        printf(FOSSIL_IO_ATTR_BLINK);
    } else if (strcmp(attribute, "reverse") == 0) {
        printf(FOSSIL_IO_ATTR_REVERSE);
    } else if (strcmp(attribute, "reversed") == 0) {
        printf(FOSSIL_IO_ATTR_REVERSED);
    } else if (strcmp(attribute, "hidden") == 0) {
        printf(FOSSIL_IO_ATTR_HIDDEN);
    } else if (strcmp(attribute, "strikethrough") == 0) {
        printf(FOSSIL_IO_ATTR_STRIKETHROUGH);
    } else if (strcmp(attribute, "normal") == 0) {
        printf(FOSSIL_IO_ATTR_NORMAL);
    } else if (strcmp(attribute, "reset_bold") == 0) {
        printf(FOSSIL_IO_ATTR_RESET_BOLD);
    } else if (strcmp(attribute, "reset_dim") == 0) {
        printf(FOSSIL_IO_ATTR_RESET_DIM);
    } else if (strcmp(attribute, "reset_italic") == 0) {
        printf(FOSSIL_IO_ATTR_RESET_ITALIC);
    } else if (strcmp(attribute, "reset_underline") == 0) {
        printf(FOSSIL_IO_ATTR_RESET_UNDERLINE);
    } else if (strcmp(attribute, "reset_blink") == 0) {
        printf(FOSSIL_IO_ATTR_RESET_BLINK);
    } else if (strcmp(attribute, "reset_reverse") == 0) {
        printf(FOSSIL_IO_ATTR_RESET_REVERSE);
    } else if (strcmp(attribute, "reset_hidden") == 0) {
        printf(FOSSIL_IO_ATTR_RESET_HIDDEN);
    } else if (strcmp(attribute, "reset_strike") == 0) {
        printf(FOSSIL_IO_ATTR_RESET_STRIKE);
    } else if (strcmp(attribute, "reset") == 0) {
        printf(FOSSIL_IO_ATTR_NORMAL);
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

/*
 * Function to print text with attributes, colors, background colors, positions, and format specifiers.
 * Supports {color}, {color,attribute}, {bg:bg_color}, {bg:bg_color,attribute}, {pos:name}, and combinations.
 */
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

            // Check for bg: or pos: prefix
            if (strncmp(attributes, "bg:", 3) == 0) {
                // Handle background color and optional attribute
                char *bg_color = attributes + 3;
                char *comma_pos = strchr(bg_color, ',');
                if (comma_pos) {
                    *comma_pos = '\0';
                    if (FOSSIL_IO_COLOR_ENABLE) fossil_io_apply_bg_color(bg_color);
                    fossil_io_apply_attribute(comma_pos + 1);
                } else {
                    if (FOSSIL_IO_COLOR_ENABLE) fossil_io_apply_bg_color(bg_color);
                }
            } else if (strncmp(attributes, "pos:", 4) == 0) {
                // Handle position
                fossil_io_apply_position(attributes + 4);
            } else {
                // Handle color and/or attribute
                char *color = attributes;
                char *attribute = NULL;
                char *comma_pos = strchr(attributes, ',');
                if (comma_pos) {
                    *comma_pos = '\0';
                    color = attributes;
                    attribute = comma_pos + 1;
                }
                if (FOSSIL_IO_COLOR_ENABLE && color && color[0] != '\0') {
                    fossil_io_apply_color(color);
                }
                if (attribute && attribute[0] != '\0') {
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
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
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
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
    putchar(c);
}

// Function to print sanitized formatted output with attributes
void fossil_io_printf(const char *format, ...) {
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
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
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
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
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
    va_list args;
    va_start(args, format);

    // Create a buffer to hold the formatted string
    char buffer[FOSSIL_IO_BUFFER_SIZE];
    vsnprintf(buffer, sizeof(buffer), format, args);

    // Print the sanitized formatted string with attributes to the specified stream
    fossil_io_fprint_with_attributes(stream, buffer);

    va_end(args);
}

// Function to format a string into a buffer
int fossil_io_snprintf(char *buffer, size_t size, const char *format, ...) {
    if (!FOSSIL_IO_OUTPUT_ENABLE) return -1;
    va_list args;
    va_start(args, format);
    int result = vsnprintf(buffer, size, format, args);
    va_end(args);
    return result;
}

// TUI PART

void fossil_io_clear_screen(void) {
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
    printf("\033[2J\033[H");
}

void fossil_io_move_cursor(int row, int col) {
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
    printf("\033[%d;%dH", row, col);
}

void fossil_io_hide_cursor(void) {
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
    printf("\033[?25l");
}

void fossil_io_show_cursor(void) {
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
    printf("\033[?25h");
}

void fossil_io_draw_horizontal_line(int length, char ch) {
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
    for (int i = 0; i < length; ++i) {
        putchar(ch);
    }
    putchar('\n');
}

void fossil_io_draw_vertical_line(int length, char ch) {
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
    for (int i = 0; i < length; ++i) {
        putchar(ch);
        putchar('\n');
    }
}

void fossil_io_flush(void) {
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
    fflush(stdout);
}
