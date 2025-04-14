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
#include "fossil/io/input.h"
#include "fossil/io/output.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>

#ifdef __WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

// Function to trim leading and trailing spaces from a string
void fossil_io_trim(char *str) {
    if (str == NULL) return;

    // Trim leading spaces
    char *start = str;
    while (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r') {
        start++;
    }

    // Move the trimmed string to the beginning
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }

    // Trim trailing spaces
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[--len] = '\0';
    }
}

int fossil_io_display_menu(const char *prompt, const char *choices[], int num_choices) {
    if (prompt != NULL) {
        printf("%s\n", prompt);
    }

    for (int i = 0; i < num_choices; i++) {
        printf("%d. %s\n", i + 1, choices[i]);
    }

    int choice;
    do {
        printf("Please choose an option (1-%d): ", num_choices);
        if (fossil_io_scanf("%d", &choice) != 1 || choice < 1 || choice > num_choices) {
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice < 1 || choice > num_choices);

    return choice - 1; // Return the index of the chosen option
}

void fossil_io_show_progress(int progress) {
    int width = 50; // Width of the progress bar
    int pos = (progress * width) / 100;
    printf("[");
    for (int i = 0; i < width; i++) {
        if (i < pos) {
            printf("=");
        } else if (i == pos) {
            printf(">");
        } else {
            printf(" ");
        }
    }
    printf("] %d%%\r", progress);
    fflush(stdout);
}

// Function to get a sanitized line of input from a provided stream (or stdin by default)
char *fossil_io_gets_from_stream(char *buf, size_t size, FILE *input_stream) {
    if (buf == NULL || size == 0 || input_stream == NULL) {
        fprintf(stderr, "Error: Invalid buffer or stream.\n");
        return NULL;
    }

    // Use fgets to get the input from the stream
    if (fgets(buf, size, input_stream) == NULL) {
        if (feof(input_stream)) {
            return NULL; // End of file reached
        }
        fprintf(stderr, "Error: Failed to read from input stream.\n");
        return NULL;
    }

    // Ensure the string is null-terminated
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0'; // Remove the newline character
    }

    // Trim any leading or trailing whitespace
    fossil_io_trim(buf);

    return buf;
}

char *fossil_io_gets_from_stream_ex(char *buf, size_t size, FILE *input_stream, int *error_code) {
    if (buf == NULL || size == 0 || input_stream == NULL || error_code == NULL) {
        fprintf(stderr, "Error: Invalid buffer, stream, or error code.\n");
        return NULL;
    }

    // Use fgets to get the input from the stream
    if (fgets(buf, size, input_stream) == NULL) {
        if (feof(input_stream)) {
            *error_code = EOF;
            return NULL; // End of file reached
        }
        *error_code = ferror(input_stream);
        fprintf(stderr, "Error: Failed to read from input stream.\n");
        return NULL;
    }

    // Ensure the string is null-terminated
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0'; // Remove the newline character
    }

    // Trim any leading or trailing whitespace
    fossil_io_trim(buf);

    return buf;
}

int fossil_io_scanf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = vscanf(format, args);
    va_end(args);
    return result;
}

int fossil_io_fscanf(FILE *input_stream, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = vfscanf(input_stream, format, args);
    va_end(args);
    return result;
}

int fossil_io_validate_input_buffer(const char *buf, size_t size) {
    if (buf == NULL || size == 0) {
        fprintf(stderr, "Error: Invalid buffer or size.\n");
        return 0;
    }
    return 1;
}

char *fossil_io_gets_utf8(char *buf, size_t size, FILE *input_stream) {
    if (!fossil_io_validate_input_buffer(buf, size)) {
        return NULL;
    }

    // Use fgets to get the input from the stream
    if (fgets(buf, size, input_stream) == NULL) {
        if (feof(input_stream)) {
            return NULL; // End of file reached
        }
        fprintf(stderr, "Error: Failed to read from input stream.\n");
        return NULL;
    }

    // Ensure the string is null-terminated
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0'; // Remove the newline character
    }

    // Trim any leading or trailing whitespace
    fossil_io_trim(buf);

    return buf;
}

int fossil_io_validate_is_int(const char *input, int *output) {
    if (input == NULL || output == NULL) {
        return 0;
    }

    char *endptr;
    long value = strtol(input, &endptr, 10);

    if (*endptr != '\0' || value < INT_MIN || value > INT_MAX) {
        return 0;
    }

    *output = (int)value;
    return 1;
}

int fossil_io_validate_is_float(const char *input, float *output) {
    if (input == NULL || output == NULL) {
        return 0;
    }

    char *endptr;
    float value = strtof(input, &endptr);

    if (*endptr != '\0') {
        return 0;
    }

    *output = value;
    return 1;
}

int fossil_io_validate_is_alnum(const char *input) {
    if (input == NULL) {
        return 0;
    }

    while (*input != '\0') {
        if (!isalnum(*input)) {
            return 0;
        }
        input++;
    }

    return 1;
}

int fossil_io_validate_is_email(const char *input) {
    if (input == NULL) {
        return 0;
    }

    // Check for the presence of an '@' character
    const char *at = strchr(input, '@');
    if (at == NULL) {
        return 0;
    }

    // Check for the presence of a '.' character after the '@' character
    const char *dot = strchr(at, '.');
    if (dot == NULL) {
        return 0;
    }

    return 1;
}

int fossil_io_validate_is_length(const char *input, size_t max_length) {
    if (input == NULL) {
        return 0;
    }

    return strlen(input) <= max_length;
}

int fossil_io_validate_sanitize_string(const char *input, char *output, size_t output_size) {
    if (input == NULL || output == NULL || output_size == 0) {
        return 0;
    }

    // Copy the input string to the output buffer
    strncpy(output, input, output_size);

    return 1;
}
