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
        fossil_io_printf("%s\n", prompt);
    }

    for (int i = 0; i < num_choices; i++) {
        fossil_io_printf("%d. %s\n", i + 1, choices[i]);
    }

    int choice;
    do {
        fossil_io_printf("Please choose an option (1-%d): ", num_choices);
        if (fossil_io_scanf("%d", &choice) != 1 || choice < 1 || choice > num_choices) {
            fossil_io_printf("Invalid choice. Please try again.\n");
        }
    } while (choice < 1 || choice > num_choices);

    return choice - 1; // Return the index of the chosen option
}

void fossil_io_show_progress(int progress) {
    int width = 50; // Width of the progress bar
    int pos = (progress * width) / 100;
    fossil_io_printf("[");
    for (int i = 0; i < width; i++) {
        if (i < pos) {
            fossil_io_printf("=");
        } else if (i == pos) {
            fossil_io_printf(">");
        } else {
            fossil_io_printf(" ");
        }
    }
    fossil_io_printf("] %d%%\r", progress);
    fflush(stdout);
}

int fossil_io_getc(fossil_fstream_t *input_stream) {
    if (input_stream == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Invalid input stream.\n");
        return EOF;
    }
    
    int c = fgetc(input_stream->file);
    if (c == EOF && ferror(input_stream->file)) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Failed to read from input stream.\n");
    }
    
    return c;
}

// Function to get a sanitized line of input from a provided stream (or stdin by default)
char *fossil_io_gets_from_stream(char *buf, size_t size, fossil_fstream_t *input_stream) {
    if (buf == NULL || size == 0 || input_stream == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Invalid buffer or stream.\n");
        return NULL;
    }

    // Use fgets to get the input from the stream
    if (fgets(buf, size, input_stream->file) == NULL) {
        if (feof(input_stream->file)) {
            return NULL; // End of file reached
        }
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Failed to read from input stream.\n");
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

char *fossil_io_gets_from_stream_ex(char *buf, size_t size, fossil_fstream_t *input_stream, int *error_code) {
    if (buf == NULL || size == 0 || input_stream == NULL || error_code == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Invalid buffer, stream, or error code.\n");
        return NULL;
    }

    // Use fgets to get the input from the stream
    if (fgets(buf, size, input_stream->file) == NULL) {
        if (feof(input_stream->file)) {
            *error_code = EOF;
            return NULL; // End of file reached
        }
        *error_code = ferror(input_stream->file);
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Failed to read from input stream.\n");
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

int fossil_io_fscanf(fossil_fstream_t *input_stream, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = vfscanf(input_stream->file, format, args);
    va_end(args);
    return result;
}

int fossil_io_validate_input_buffer(const char *buf, size_t size) {
    if (buf == NULL || size == 0) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Invalid buffer or size.\n");
        return 0;
    }
    return 1;
}

char *fossil_io_gets_utf8(char *buf, size_t size, fossil_fstream_t *input_stream) {
    if (!fossil_io_validate_input_buffer(buf, size)) {
        return NULL;
    }

    // Use fgets to get the input from the stream
    if (fgets(buf, size, input_stream->file) == NULL) {
        if (feof(input_stream->file)) {
            return NULL; // End of file reached
        }
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Failed to read from input stream.\n");
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

    // Validate the domain against a list of known mailing services
    const char *valid_services[] = {"gmail.com", "yahoo.com", "outlook.com", "hotmail.com", "icloud.com"};
    size_t num_services = sizeof(valid_services) / sizeof(valid_services[0]);

    for (size_t i = 0; i < num_services; i++) {
        if (strcmp(at + 1, valid_services[i]) == 0) {
            return 1;
        }
    }

    return 0;
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

int fossil_io_gets(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL) {
        return -1; // Error or EOF
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0'; // Remove newline character
    }
    return 0; // Success
}

typedef void (*fossil_io_action_callback_t)(void);

typedef struct {
    int key_code;                         /**< The integer key code (e.g., ASCII or special code). */
    const char *action;                    /**< The action string associated with this key. */
    fossil_io_action_callback_t callback;  /**< Optional function to call when key is pressed. */
} fossil_io_keybinding_t;

#define MAX_KEYBINDINGS 256
static fossil_io_keybinding_t _keybindings[MAX_KEYBINDINGS];
static size_t _num_keybindings = 0;

int fossil_io_register_keybinding(int key_code, const char *action) {
    if (!action || _num_keybindings >= MAX_KEYBINDINGS) return 1;

    // Check for duplicate
    for (size_t i = 0; i < _num_keybindings; i++) {
        if (_keybindings[i].key_code == key_code) return 2; // Duplicate key
    }

    _keybindings[_num_keybindings].key_code = key_code;
    _keybindings[_num_keybindings].action = strdup(action);
    if (!_keybindings[_num_keybindings].action) return 3; // Allocation failure

    _num_keybindings++;
    return 0;
}

int fossil_io_unregister_keybinding(int key_code) {
    for (size_t i = 0; i < _num_keybindings; i++) {
        if (_keybindings[i].key_code == key_code) {
            free((void*)_keybindings[i].action);
            // Shift remaining bindings
            for (size_t j = i; j < _num_keybindings - 1; j++) {
                _keybindings[j] = _keybindings[j + 1];
            }
            _num_keybindings--;
            return 0;
        }
    }
    return 1; // Key not found
}

const char* fossil_io_get_keybinding_action(int key_code) {
    for (size_t i = 0; i < _num_keybindings; i++) {
        if (_keybindings[i].key_code == key_code) {
            return _keybindings[i].action;
        }
    }
    return NULL;
}

/**
 * Registers a new key binding with optional callback.
 */
int fossil_io_register_keybinding_with_callback(int key_code, const char *action, fossil_io_action_callback_t cb) {
    if (!action || _num_keybindings >= MAX_KEYBINDINGS) return 1;

    // Check for duplicate key
    for (size_t i = 0; i < _num_keybindings; i++) {
        if (_keybindings[i].key_code == key_code) return 2; // Duplicate
    }

    _keybindings[_num_keybindings].key_code = key_code;
    _keybindings[_num_keybindings].action = strdup(action);
    _keybindings[_num_keybindings].callback = cb;
    if (!_keybindings[_num_keybindings].action) return 3; // Allocation failed

    _num_keybindings++;
    return 0;
}

/**
 * Enhanced key processing that calls registered callback.
 */
int fossil_io_process_keybinding(int key_code) {
    for (size_t i = 0; i < _num_keybindings; i++) {
        if (_keybindings[i].key_code == key_code) {
            if (_keybindings[i].callback) {
                _keybindings[i].callback();  // Execute callback
            } else {
                printf("Action triggered: %s\n", _keybindings[i].action);
            }
            return 1; // Action processed
        }
    }
    return 0; // No binding
}

size_t fossil_io_list_keybindings(fossil_io_keybinding_t *bindings, size_t max_bindings) {
    size_t count = (max_bindings < _num_keybindings) ? max_bindings : _num_keybindings;
    for (size_t i = 0; i < count; i++) {
        bindings[i].key_code = _keybindings[i].key_code;
        bindings[i].action = _keybindings[i].action;
    }
    return count;
}

void fossil_io_clear_keybindings(void) {
    for (size_t i = 0; i < _num_keybindings; i++) {
        free((void*)_keybindings[i].action);
    }
    _num_keybindings = 0;
}
