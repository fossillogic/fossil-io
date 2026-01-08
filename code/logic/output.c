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
#include "fossil/io/cstring.h"
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

typedef enum {
    FOSSIL_CIPHER_ENCODE,
    FOSSIL_CIPHER_DECODE
} fossil_cipher_mode_t;

typedef struct {
    char c;
    const char *morse;
} morse_entry_t;

static const morse_entry_t morse_table[] = {
    {'a', ".-"}, {'b', "-..."}, {'c', "-.-."},
    {'d', "-.."}, {'e', "."},   {'f', "..-."},
    {'g', "--."}, {'h', "...."}, {'i', ".."},
    {'j', ".---"},{'k', "-.-"},  {'l', ".-.."},
    {'m', "--"},  {'n', "-."},   {'o', "---"},
    {'p', ".--."},{'q', "--.-"}, {'r', ".-."},
    {'s', "..."}, {'t', "-"},    {'u', "..-"},
    {'v', "...-"},{'w', ".--"},  {'x', "-..-"},
    {'y', "-.--"},{'z', "--.."},
    {'0',"-----"},{'1',".----"},{'2',"..---"},
    {'3',"...--"},{'4',"....-"},{'5',"....."},
    {'6',"-...."},{'7',"--..."},{'8',"---.."},
    {'9',"----."},
    {0,NULL}
};

static char *fossil_cipher_rot13(const char *in, fossil_cipher_mode_t mode) {
    (void)mode;

    size_t len = strlen(in);
    char *out = malloc(len + 1);
    if (!out) return NULL;

    for (size_t i = 0; i < len; i++) {
        char c = in[i];
        if (c >= 'a' && c <= 'z')
            out[i] = ((c - 'a' + 13) % 26) + 'a';
        else if (c >= 'A' && c <= 'Z')
            out[i] = ((c - 'A' + 13) % 26) + 'A';
        else
            out[i] = c;
    }

    out[len] = '\0';
    return out;
}

static char *fossil_cipher_leet(const char *in, fossil_cipher_mode_t mode) {
    size_t len = strlen(in);
    char *out = malloc(len * 2 + 1); // safe expansion
    if (!out) return NULL;

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        char c = tolower(in[i]);
        switch (c) {
            case 'a': out[j++]='4'; break;
            case 'e': out[j++]='3'; break;
            case 'i': out[j++]='1'; break;
            case 'o': out[j++]='0'; break;
            case 's': out[j++]='5'; break;
            case 't': out[j++]='7'; break;
            default:  out[j++]=in[i]; break;
        }
    }

    out[j] = '\0';
    return out;
}

static char *fossil_cipher_haxsor(const char *in, fossil_cipher_mode_t mode) {
    size_t len = strlen(in);
    char *out = malloc(len * 3 + 1);
    if (!out) return NULL;

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        char c = tolower(in[i]);
        if (c == 'x') {
            out[j++]='x'; out[j++]='0'; out[j++]='r';
        } else if (c == 'e') {
            out[j++]='3';
        } else if (c == 'a') {
            out[j++]='@';
        } else {
            out[j++]=in[i];
        }
    }

    out[j] = '\0';
    return out;
}

static char *fossil_cipher_morse(const char *in, fossil_cipher_mode_t mode) {
    size_t cap = strlen(in) * 6 + 1;
    char *out = malloc(cap);
    if (!out) return NULL;

    size_t j = 0;
    for (size_t i = 0; in[i]; i++) {
        char c = tolower(in[i]);
        if (c == ' ') {
            out[j++]=' '; out[j++]=' ';
            continue;
        }
        for (int k = 0; morse_table[k].c; k++) {
            if (morse_table[k].c == c) {
                size_t mlen = strlen(morse_table[k].morse);
                memcpy(out + j, morse_table[k].morse, mlen);
                j += mlen;
                out[j++]=' ';
                break;
            }
        }
    }

    out[j] = '\0';
    return out;
}

//
typedef struct {
    const char *name;
    char *(*fn)(const char *, fossil_cipher_mode_t);
} fossil_cipher_entry_t;

// Caesar cipher (shift by 3 for encode, -3 for decode)
static char *fossil_cipher_caesar(const char *in, fossil_cipher_mode_t mode) {
    int shift = (mode == FOSSIL_CIPHER_ENCODE) ? 3 : 23; // 23 == -3 mod 26
    size_t len = strlen(in);
    char *out = malloc(len + 1);
    if (!out) return NULL;

    for (size_t i = 0; i < len; i++) {
        char c = in[i];
        if (c >= 'a' && c <= 'z')
            out[i] = ((c - 'a' + shift) % 26) + 'a';
        else if (c >= 'A' && c <= 'Z')
            out[i] = ((c - 'A' + shift) % 26) + 'A';
        else
            out[i] = c;
    }
    out[len] = '\0';
    return out;
}

// Binary encode/decode (encode: text to 0/1, decode: 8-bit binary to text)
static char *fossil_cipher_binary(const char *in, fossil_cipher_mode_t mode) {
    if (mode == FOSSIL_CIPHER_ENCODE) {
        size_t len = strlen(in);
        char *out = malloc(len * 9 + 1); // 8 bits + space per char
        if (!out) return NULL;
        size_t j = 0;
        for (size_t i = 0; i < len; i++) {
            for (int b = 7; b >= 0; b--) {
                out[j++] = ((in[i] >> b) & 1) ? '1' : '0';
            }
            out[j++] = ' ';
        }
        if (j > 0) out[j - 1] = '\0'; // remove last space
        else out[j] = '\0';
        return out;
    } else { // decode
        size_t len = strlen(in);
        char *out = malloc(len / 8 + 2);
        if (!out) return NULL;
        size_t j = 0;
        for (size_t i = 0; i + 7 < len;) {
            unsigned char val = 0;
            for (int b = 0; b < 8; b++, i++) {
                if (in[i] == '1') val = (val << 1) | 1;
                else if (in[i] == '0') val = (val << 1);
                else break;
            }
            out[j++] = val;
            // Skip space if present
            if (in[i] == ' ') i++;
        }
        out[j] = '\0';
        return out;
    }
}

char *fossil_io_apply_cipher(
    const char *text,
    const char *name,
    fossil_cipher_mode_t mode
) {
    if (!text || !name) return NULL;

    static const fossil_cipher_entry_t table[] = {
        { "leet",   fossil_cipher_leet },
        { "haxsor", fossil_cipher_haxsor },
        { "rot13",  fossil_cipher_rot13 },
        { "caesar", fossil_cipher_caesar },
        { "morse",  fossil_cipher_morse },
        { "binary", fossil_cipher_binary },
        { NULL, NULL }
    };

    for (int i = 0; table[i].name; i++) {
        if (fossil_io_cstring_iequals(name, table[i].name)) {
            return table[i].fn(text, mode);
        }
    }

    return fossil_io_cstring_dup(text); // unknown cipher = passthrough
}

// Function to apply background color
void fossil_io_apply_bg_color(ccstring bg_color) {
    if (fossil_io_cstring_iequals(bg_color, "black")) {
        printf(FOSSIL_IO_BG_BLACK);
    } else if (fossil_io_cstring_iequals(bg_color, "red")) {
        printf(FOSSIL_IO_BG_RED);
    } else if (fossil_io_cstring_iequals(bg_color, "green")) {
        printf(FOSSIL_IO_BG_GREEN);
    } else if (fossil_io_cstring_iequals(bg_color, "yellow")) {
        printf(FOSSIL_IO_BG_YELLOW);
    } else if (fossil_io_cstring_iequals(bg_color, "blue")) {
        printf(FOSSIL_IO_BG_BLUE);
    } else if (fossil_io_cstring_iequals(bg_color, "magenta")) {
        printf(FOSSIL_IO_BG_MAGENTA);
    } else if (fossil_io_cstring_iequals(bg_color, "cyan")) {
        printf(FOSSIL_IO_BG_CYAN);
    } else if (fossil_io_cstring_iequals(bg_color, "white")) {
        printf(FOSSIL_IO_BG_WHITE);
    } else if (fossil_io_cstring_iequals(bg_color, "gray")) {
        printf(FOSSIL_IO_BG_GRAY);
    } else if (fossil_io_cstring_iequals(bg_color, "orange")) {
        printf(FOSSIL_IO_BG_ORANGE);
    } else if (fossil_io_cstring_iequals(bg_color, "pink")) {
        printf(FOSSIL_IO_BG_PINK);
    } else if (fossil_io_cstring_iequals(bg_color, "purple")) {
        printf(FOSSIL_IO_BG_PURPLE);
    } else if (fossil_io_cstring_iequals(bg_color, "brown")) {
        printf(FOSSIL_IO_BG_BROWN);
    } else if (fossil_io_cstring_iequals(bg_color, "teal")) {
        printf(FOSSIL_IO_BG_TEAL);
    } else if (fossil_io_cstring_iequals(bg_color, "silver")) {
        printf(FOSSIL_IO_BG_SILVER);
    }
    // Bright background colors
    else if (fossil_io_cstring_iequals(bg_color, "bright_black")) {
        printf(FOSSIL_IO_BG_BRIGHT_BLACK);
    } else if (fossil_io_cstring_iequals(bg_color, "bright_red")) {
        printf(FOSSIL_IO_BG_BRIGHT_RED);
    } else if (fossil_io_cstring_iequals(bg_color, "bright_green")) {
        printf(FOSSIL_IO_BG_BRIGHT_GREEN);
    } else if (fossil_io_cstring_iequals(bg_color, "bright_yellow")) {
        printf(FOSSIL_IO_BG_BRIGHT_YELLOW);
    } else if (fossil_io_cstring_iequals(bg_color, "bright_blue")) {
        printf(FOSSIL_IO_BG_BRIGHT_BLUE);
    } else if (fossil_io_cstring_iequals(bg_color, "bright_magenta")) {
        printf(FOSSIL_IO_BG_BRIGHT_MAGENTA);
    } else if (fossil_io_cstring_iequals(bg_color, "bright_cyan")) {
        printf(FOSSIL_IO_BG_BRIGHT_CYAN);
    } else if (fossil_io_cstring_iequals(bg_color, "bright_white")) {
        printf(FOSSIL_IO_BG_BRIGHT_WHITE);
    } else if (fossil_io_cstring_iequals(bg_color, "reset")) {
        printf(FOSSIL_IO_COLOR_RESET);
    }
}

// Function to apply color
void fossil_io_apply_color(ccstring color) {
    if (fossil_io_cstring_iequals(color, "black")) {
        printf(FOSSIL_IO_COLOR_BLACK);
    } else if (fossil_io_cstring_iequals(color, "red")) {
        printf(FOSSIL_IO_COLOR_RED);
    } else if (fossil_io_cstring_iequals(color, "green")) {
        printf(FOSSIL_IO_COLOR_GREEN);
    } else if (fossil_io_cstring_iequals(color, "yellow")) {
        printf(FOSSIL_IO_COLOR_YELLOW);
    } else if (fossil_io_cstring_iequals(color, "blue")) {
        printf(FOSSIL_IO_COLOR_BLUE);
    } else if (fossil_io_cstring_iequals(color, "magenta")) {
        printf(FOSSIL_IO_COLOR_MAGENTA);
    } else if (fossil_io_cstring_iequals(color, "cyan")) {
        printf(FOSSIL_IO_COLOR_CYAN);
    } else if (fossil_io_cstring_iequals(color, "white")) {
        printf(FOSSIL_IO_COLOR_WHITE);
    } else if (fossil_io_cstring_iequals(color, "gray")) {
        printf(FOSSIL_IO_COLOR_GRAY);
    } else if (fossil_io_cstring_iequals(color, "orange")) {
        printf(FOSSIL_IO_COLOR_ORANGE);
    } else if (fossil_io_cstring_iequals(color, "pink")) {
        printf(FOSSIL_IO_COLOR_PINK);
    } else if (fossil_io_cstring_iequals(color, "purple")) {
        printf(FOSSIL_IO_COLOR_PURPLE);
    } else if (fossil_io_cstring_iequals(color, "brown")) {
        printf(FOSSIL_IO_COLOR_BROWN);
    } else if (fossil_io_cstring_iequals(color, "teal")) {
        printf(FOSSIL_IO_COLOR_TEAL);
    } else if (fossil_io_cstring_iequals(color, "silver")) {
        printf(FOSSIL_IO_COLOR_SILVER);
    }
    // Bright colors
    else if (fossil_io_cstring_iequals(color, "bright_black")) {
        printf(FOSSIL_IO_COLOR_BRIGHT_BLACK);
    } else if (fossil_io_cstring_iequals(color, "bright_red")) {
        printf(FOSSIL_IO_COLOR_BRIGHT_RED);
    } else if (fossil_io_cstring_iequals(color, "bright_green")) {
        printf(FOSSIL_IO_COLOR_BRIGHT_GREEN);
    } else if (fossil_io_cstring_iequals(color, "bright_yellow")) {
        printf(FOSSIL_IO_COLOR_BRIGHT_YELLOW);
    } else if (fossil_io_cstring_iequals(color, "bright_blue")) {
        printf(FOSSIL_IO_COLOR_BRIGHT_BLUE);
    } else if (fossil_io_cstring_iequals(color, "bright_magenta")) {
        printf(FOSSIL_IO_COLOR_BRIGHT_MAGENTA);
    } else if (fossil_io_cstring_iequals(color, "bright_cyan")) {
        printf(FOSSIL_IO_COLOR_BRIGHT_CYAN);
    } else if (fossil_io_cstring_iequals(color, "bright_white")) {
        printf(FOSSIL_IO_COLOR_BRIGHT_WHITE);
    } else if (fossil_io_cstring_iequals(color, "reset")) {
        printf(FOSSIL_IO_COLOR_RESET);
    }
}

// Function to apply text attributes (e.g., bold, underline, dim, etc.)
void fossil_io_apply_attribute(ccstring attribute) {
    if (fossil_io_cstring_iequals(attribute, "bold")) {
        printf(FOSSIL_IO_ATTR_BOLD);
    } else if (fossil_io_cstring_iequals(attribute, "dim")) {
        printf(FOSSIL_IO_ATTR_DIM);
    } else if (fossil_io_cstring_iequals(attribute, "italic")) {
        printf(FOSSIL_IO_ATTR_ITALIC);
    } else if (fossil_io_cstring_iequals(attribute, "underline")) {
        printf(FOSSIL_IO_ATTR_UNDERLINE);
    } else if (fossil_io_cstring_iequals(attribute, "blink")) {
        printf(FOSSIL_IO_ATTR_BLINK);
    } else if (fossil_io_cstring_iequals(attribute, "reverse")) {
        printf(FOSSIL_IO_ATTR_REVERSE);
    } else if (fossil_io_cstring_iequals(attribute, "reversed")) {
        printf(FOSSIL_IO_ATTR_REVERSED);
    } else if (fossil_io_cstring_iequals(attribute, "hidden")) {
        printf(FOSSIL_IO_ATTR_HIDDEN);
    } else if (fossil_io_cstring_iequals(attribute, "strikethrough")) {
        printf(FOSSIL_IO_ATTR_STRIKETHROUGH);
    } else if (fossil_io_cstring_iequals(attribute, "normal")) {
        printf(FOSSIL_IO_ATTR_NORMAL);
    } else if (fossil_io_cstring_iequals(attribute, "reset_bold")) {
        printf(FOSSIL_IO_ATTR_RESET_BOLD);
    } else if (fossil_io_cstring_iequals(attribute, "reset_dim")) {
        printf(FOSSIL_IO_ATTR_RESET_DIM);
    } else if (fossil_io_cstring_iequals(attribute, "reset_italic")) {
        printf(FOSSIL_IO_ATTR_RESET_ITALIC);
    } else if (fossil_io_cstring_iequals(attribute, "reset_underline")) {
        printf(FOSSIL_IO_ATTR_RESET_UNDERLINE);
    } else if (fossil_io_cstring_iequals(attribute, "reset_blink")) {
        printf(FOSSIL_IO_ATTR_RESET_BLINK);
    } else if (fossil_io_cstring_iequals(attribute, "reset_reverse")) {
        printf(FOSSIL_IO_ATTR_RESET_REVERSE);
    } else if (fossil_io_cstring_iequals(attribute, "reset_hidden")) {
        printf(FOSSIL_IO_ATTR_RESET_HIDDEN);
    } else if (fossil_io_cstring_iequals(attribute, "reset_strike")) {
        printf(FOSSIL_IO_ATTR_RESET_STRIKE);
    } else if (fossil_io_cstring_iequals(attribute, "reset")) {
        printf(FOSSIL_IO_ATTR_NORMAL);
    }
}

// Function to handle named positions (like top, bottom, left, right)
void fossil_io_apply_position(ccstring pos) {
    if (fossil_io_cstring_iequals(pos, "top")) {
        printf("\033[1;1H");
    } else if (fossil_io_cstring_iequals(pos, "bottom")) {
        printf("\033[1000;1H");
    } else if (fossil_io_cstring_iequals(pos, "left")) {
        printf("\033[1;1H");
    } else if (fossil_io_cstring_iequals(pos, "right")) {
        printf("\033[1;1000H");
    } else if (fossil_io_cstring_iequals(pos, "center")) {
        printf("\033[25;40H");
    } else if (fossil_io_cstring_iequals(pos, "top-left")) {
        printf("\033[1;1H");
    } else if (fossil_io_cstring_iequals(pos, "top-right")) {
        printf("\033[1;1000H");
    } else if (fossil_io_cstring_iequals(pos, "bottom-left")) {
        printf("\033[1000;1H");
    } else if (fossil_io_cstring_iequals(pos, "bottom-right")) {
        printf("\033[1000;1000H");
    } else if (fossil_io_cstring_iequals(pos, "middle-left")) {
        printf("\033[25;1H");
    } else if (fossil_io_cstring_iequals(pos, "middle-right")) {
        printf("\033[25;1000H");
    } else {
        fprintf(stderr, "Unknown position: %s\n", pos);
    }
}

/*
 * Function to print text with attributes, colors, background colors, positions, and format specifiers.
 * Supports {color}, {color,attribute}, {bg:bg_color}, {bg:bg_color,attribute}, {pos:name}, and combinations.
 */
void fossil_io_print_with_attributes(ccstring str) {
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;

    if (str == NULL) {
        fossil_io_file_write(FOSSIL_STDERR, "cnullptr\n", 1, strlen("cnullptr\n"));
        return;
    }

    ccstring current_pos = str;
    ccstring start = NULL;
    ccstring end = NULL;

    while ((start = strchr(current_pos, '{')) != NULL) {
        // Output text before '{'
        if (start > current_pos) {
            fossil_io_file_write(FOSSIL_STDOUT, current_pos, 1, start - current_pos);
        }

        // Find the matching '}'
        end = strchr(start, '}');
        if (end && end > start) {
            // Extract attributes inside '{}'
            size_t length = end - start - 1;
            if (length == 0) {
                // Empty braces, print as is
                fossil_io_file_write(FOSSIL_STDOUT, "{", 1, 1);
                fossil_io_file_write(FOSSIL_STDOUT, "}", 1, 1);
                current_pos = end + 1;
                continue;
            }
            char attributes[length + 1];
            strncpy(attributes, start + 1, length);
            attributes[length] = '\0';

            // Check for encode: or decode: prefix
            if (strncmp(attributes, "encode:", 7) == 0) {
                char *cipher_name = attributes + 7;
                char *close = strchr(end + 1, '{');
                ccstring text_start = end + 1;
                ccstring text_end = close ? close : str + strlen(str);
                size_t text_len = text_end - text_start;
                char *text = malloc(text_len + 1);
                if (text) {
                    strncpy(text, text_start, text_len);
                    text[text_len] = '\0';
                    char *encoded = fossil_io_apply_cipher(text, cipher_name, FOSSIL_CIPHER_ENCODE);
                    if (encoded) {
                        fossil_io_file_write(FOSSIL_STDOUT, encoded, 1, strlen(encoded));
                        free(encoded);
                    }
                    free(text);
                }
                current_pos = text_end;
                continue;
            } else if (strncmp(attributes, "decode:", 7) == 0) {
                char *cipher_name = attributes + 7;
                char *close = strchr(end + 1, '{');
                ccstring text_start = end + 1;
                ccstring text_end = close ? close : str + strlen(str);
                size_t text_len = text_end - text_start;
                char *text = malloc(text_len + 1);
                if (text) {
                    strncpy(text, text_start, text_len);
                    text[text_len] = '\0';
                    char *decoded = fossil_io_apply_cipher(text, cipher_name, FOSSIL_CIPHER_DECODE);
                    if (decoded) {
                        fossil_io_file_write(FOSSIL_STDOUT, decoded, 1, strlen(decoded));
                        free(decoded);
                    }
                    free(text);
                }
                current_pos = text_end;
                continue;
            }
            // Check for bg: or pos: prefix
            if (strncmp(attributes, "bg:", 3) == 0) {
                char *bg_color = attributes + 3;
                char *comma_pos = strchr(bg_color, ',');
                if (comma_pos) {
                    *comma_pos = '\0';
                    if (FOSSIL_IO_COLOR_ENABLE && bg_color[0] != '\0') fossil_io_apply_bg_color(bg_color);
                    if (comma_pos[1] != '\0') fossil_io_apply_attribute(comma_pos + 1);
                } else {
                    if (FOSSIL_IO_COLOR_ENABLE && bg_color[0] != '\0') fossil_io_apply_bg_color(bg_color);
                }
            } else if (strncmp(attributes, "pos:", 4) == 0) {
                if (attributes[4] != '\0') fossil_io_apply_position(attributes + 4);
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
            // No matching '}', print '{' and continue searching
            fossil_io_file_write(FOSSIL_STDOUT, start, 1, 1);
            current_pos = start + 1;
        }
    }

    // Output remaining text after last '}'
    if (current_pos && *current_pos) {
        fossil_io_file_write(FOSSIL_STDOUT, current_pos, 1, strlen(current_pos));
    }
    fossil_io_file_flush(FOSSIL_STDOUT);
}

// Function to print a sanitized formatted string to a specific file stream with attributes
void fossil_io_fprint_with_attributes(fossil_io_file_t *stream, ccstring str) {
    if (str != NULL && stream != NULL) {
        char sanitized_str[FOSSIL_IO_BUFFER_SIZE];
        strncpy(sanitized_str, str, sizeof(sanitized_str));
        sanitized_str[sizeof(sanitized_str) - 1] = '\0'; // Ensure null termination

        ccstring current_pos = sanitized_str;
        ccstring start = NULL;
        ccstring end = NULL;
        while ((start = strchr(current_pos, '{')) != NULL) {
            // Write text before '{' to the file
            fossil_io_file_write(stream, current_pos, 1, start - current_pos);
            end = strchr(start, '}');
            if (end && end > start) {
                // Skip the attribute section
                current_pos = end + 1;
            } else {
                // No matching '}', write '{' and continue searching
                fossil_io_file_write(stream, start, 1, 1);
                current_pos = start + 1;
            }
        }
        // Write remaining text after last '}'
        fossil_io_file_write(stream, current_pos, 1, strlen(current_pos));
    }
}

//
// OUTPUT FUNCTIONS
//

// Function to print a sanitized string with attributes inside {}
void fossil_io_puts(ccstring str) {
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
    if (str != NULL) {
        char sanitized_str[FOSSIL_IO_BUFFER_SIZE];
        strncpy(sanitized_str, str, sizeof(sanitized_str));
        sanitized_str[sizeof(sanitized_str) - 1] = '\0'; // Ensure null termination
        
        // Print the sanitized string with attributes
        fossil_io_print_with_attributes(sanitized_str);
    } else {
        fossil_io_file_write(FOSSIL_STDERR, "cnullptr\n", 1, strlen("cnullptr\n"));
    }
}

// Function to print a single character
void fossil_io_putchar(char c) {
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
    fossil_io_file_write(FOSSIL_STDOUT, &c, 1, 1);
}

// Function to print sanitized formatted output with attributes
void fossil_io_printf(ccstring format, ...) {
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
void fossil_io_fputs(fossil_io_file_t *stream, ccstring str) {
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
    if (str != NULL && stream != NULL) {
        char sanitized_str[FOSSIL_IO_BUFFER_SIZE];
        strncpy(sanitized_str, str, sizeof(sanitized_str));
        sanitized_str[sizeof(sanitized_str) - 1] = '\0'; // Ensure null termination

        // Apply color/attributes and sanitize the string before printing
        fossil_io_fprint_with_attributes(stream, sanitized_str);
    } else {
        fossil_io_file_write(FOSSIL_STDERR, "cnullptr\n", 1, strlen("cnullptr\n"));
    }
}

// Function to print a sanitized formatted string to a specific file stream
void fossil_io_fprintf(fossil_io_file_t *stream, ccstring format, ...) {
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
int fossil_io_snprintf(char *buffer, size_t size, ccstring format, ...) {
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
    cstring esc = fossil_io_cstring_create("\033[2J\033[H");
    fossil_io_file_write(FOSSIL_STDOUT, esc, 1, fossil_io_cstring_length(esc));
    fossil_io_cstring_free(esc);
}

void fossil_io_move_cursor(int row, int col) {
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
    cstring buf = fossil_io_cstring_format("\033[%d;%dH", row, col);
    fossil_io_file_write(FOSSIL_STDOUT, buf, 1, fossil_io_cstring_length(buf));
    fossil_io_cstring_free(buf);
}

void fossil_io_hide_cursor(void) {
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
    cstring esc = fossil_io_cstring_create("\033[?25l");
    fossil_io_file_write(FOSSIL_STDOUT, esc, 1, fossil_io_cstring_length(esc));
    fossil_io_cstring_free(esc);
}

void fossil_io_show_cursor(void) {
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
    cstring esc = fossil_io_cstring_create("\033[?25h");
    fossil_io_file_write(FOSSIL_STDOUT, esc, 1, fossil_io_cstring_length(esc));
    fossil_io_cstring_free(esc);
}

void fossil_io_draw_horizontal_line(int length, char ch) {
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
    for (int i = 0; i < length; ++i) {
        fossil_io_file_write(FOSSIL_STDOUT, &ch, 1, 1);
    }
    fossil_io_file_write(FOSSIL_STDOUT, "\n", 1, 1);
}

void fossil_io_draw_vertical_line(int length, char ch) {
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
    for (int i = 0; i < length; ++i) {
        fossil_io_file_write(FOSSIL_STDOUT, &ch, 1, 1);
        fossil_io_file_write(FOSSIL_STDOUT, "\n", 1, 1);
    }
}

void fossil_io_flush(void) {
    if (!FOSSIL_IO_OUTPUT_ENABLE) return;
    fossil_io_file_flush(FOSSIL_STDOUT);
}
