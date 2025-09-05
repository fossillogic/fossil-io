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
#include "fossil/io/cstring.h"
#include "fossil/io/output.h"
#include <string.h>   // For strlen, strnlen, strncasecmp
#include <strings.h>  // For strncasecmp on POSIX
#include <stdlib.h>
#include <ctype.h>    // For toupper, tolower
#include <time.h>

#ifndef HAVE_STRNLEN
size_t strnlen(const char *s, size_t maxlen) {
    size_t i;
    for (i = 0; i < maxlen && s[i]; i++);
    return i;
}
#endif

#ifndef HAVE_STRNCASECMP
int strncasecmp(const char *s1, const char *s2, size_t n) {
    for (size_t i = 0; i < n && s1[i] && s2[i]; i++) {
        int diff = tolower((unsigned char)s1[i]) - tolower((unsigned char)s2[i]);
        if (diff != 0) return diff;
    }
    return 0;
}
#endif

// ============================================================================
// C String Functions
// ============================================================================

cstring fossil_io_cstring_create(const char *init) {
    if (!init) return NULL;
    size_t length = strlen(init);
    cstring str = (cstring)malloc(length + 1);
    if (str) {
        strcpy(str, init);
    }
    return str;
}

void fossil_io_cstring_free(cstring str) {
    if (str) {
        free(str);
    }
}

int fossil_io_cstring_silly(const char *input, char *output, size_t size) {
    if (!input || !output || size == 0) return -1;

    size_t len = strlen(input);
    if (len + 1 > size) return -1;

    for (size_t i = 0; i < len; i++) {
        char c = input[i];
        // Random case change
        if (isalpha((unsigned char)c)) {
            if (rand() % 2) {
                c = (char)toupper((unsigned char)c);
            } else {
                c = (char)tolower((unsigned char)c);
            }
        }
        // Occasionally insert silly symbol
        if (rand() % 10 == 0 && (i + 1 < size - 1)) {
            output[i++] = '~';
        }
        output[i] = c;
    }
    output[len] = '\0';
    return 0;
}

int fossil_io_cstring_piglatin(const char *input, char *output, size_t size) {
    if (!input || !output || size == 0) return -1;

    output[0] = '\0';
    const char *delims = " \t\n";
    char buffer[256];
    char word[128];

    // Copy input safely into a working buffer
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    char *token = strtok(buffer, delims);
    while (token) {
        size_t word_len = strlen(token);
        if (word_len == 0) {
            token = strtok(NULL, delims);
            continue;
        }

        // Vowel start → add "yay"
        if (strchr("AEIOUaeiou", token[0])) {
            strncpy(word, token, sizeof(word) - 4);  // leave room for "yay"
            word[sizeof(word) - 4] = '\0';
            strncat(word, "yay", sizeof(word) - strlen(word) - 1);
        } 
        // Consonant start → move first letter, add "ay"
        else {
            strncpy(word, token + 1, sizeof(word) - 4);  // leave room for "<c>ay"
            word[sizeof(word) - 4] = '\0';

            size_t len = strlen(word);
            if (len < sizeof(word) - 3) {
                word[len] = token[0];
                word[len + 1] = 'a';
                word[len + 2] = 'y';
                word[len + 3] = '\0';
            } else {
                return -1; // truncated
            }
        }

        // Check space in output before appending
        if (strlen(output) + strlen(word) + 2 > size) return -1;
        strcat(output, word);
        strcat(output, " ");

        token = strtok(NULL, delims);
    }

    return 0;
}

int fossil_io_cstring_leetspeak(const char *input, char *output, size_t size) {
    if (!input || !output || size == 0) return -1;

    size_t out_idx = 0;
    for (size_t i = 0; input[i] && out_idx < size - 1; i++) {
        char c = input[i];
        char repl[3] = {0};

        switch (tolower((unsigned char)c)) {
            case 'a': strcpy(repl, "4"); break;
            case 'e': strcpy(repl, "3"); break;
            case 'i': strcpy(repl, "1"); break;
            case 'o': strcpy(repl, "0"); break;
            case 's': strcpy(repl, "5"); break;
            case 't': strcpy(repl, "7"); break;
            default: repl[0] = c; repl[1] = '\0'; break;
        }

        size_t repl_len = strlen(repl);
        if (out_idx + repl_len >= size - 1) return -1;
        strcpy(&output[out_idx], repl);
        out_idx += repl_len;
    }
    output[out_idx] = '\0';
    return 0;
}

// -------------------
// Mocking SpongeBob
// -------------------
char* fossil_io_cstring_mocking(const char *str) {
    if (!str) return NULL;
    size_t len = strlen(str);
    char *out = malloc(len + 1);
    if (!out) return NULL;

    for (size_t i = 0; i < len; i++) {
        if (i % 2 == 0)
            out[i] = tolower((unsigned char)str[i]);
        else
            out[i] = toupper((unsigned char)str[i]);
    }
    out[len] = '\0';
    return out;
}

// -------------------
// ROT13 Cipher
// -------------------
char* fossil_io_cstring_rot13(const char *str) {
    if (!str) return NULL;
    size_t len = strlen(str);
    char *out = malloc(len + 1);
    if (!out) return NULL;

    for (size_t i = 0; i < len; i++) {
        char c = str[i];
        if ('a' <= c && c <= 'z')
            out[i] = ((c - 'a' + 13) % 26) + 'a';
        else if ('A' <= c && c <= 'Z')
            out[i] = ((c - 'A' + 13) % 26) + 'A';
        else
            out[i] = c;
    }
    out[len] = '\0';
    return out;
}

// -------------------
// Shuffle String
// -------------------
char* fossil_io_cstring_shuffle(const char *str) {
    if (!str) return NULL;
    size_t len = strlen(str);
    char *out = malloc(len + 1);
    if (!out) return NULL;

    strcpy(out, str);

    // Seed RNG once per run
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }

    for (size_t i = 0; i < len; i++) {
        size_t j = rand() % len;
        char tmp = out[i];
        out[i] = out[j];
        out[j] = tmp;
    }

    return out;
}

// -------------------
// UPPER_SNAKE_CASE
// -------------------
char* fossil_io_cstring_upper_snake(const char *str) {
    if (!str) return NULL;
    size_t len = strlen(str);

    // Worst case: every char becomes "_X"
    char *out = malloc(len * 2 + 1);
    if (!out) return NULL;

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (isspace((unsigned char)str[i])) {
            out[j++] = '_';
        } else if (isalpha((unsigned char)str[i])) {
            out[j++] = toupper((unsigned char)str[i]);
        } else {
            out[j++] = str[i];
        }
    }
    out[j] = '\0';
    return out;
}

cstring fossil_io_cstring_copy(ccstring str) {
    if (!str) return NULL;
    return fossil_io_cstring_create(str);
}

// ---------------- Number <-> Words Conversion ----------------

static const char *units[] = {
    "zero", "one", "two", "three", "four", "five",
    "six", "seven", "eight", "nine", "ten", "eleven",
    "twelve", "thirteen", "fourteen", "fifteen",
    "sixteen", "seventeen", "eighteen", "nineteen"
};

static const char *tens[] = {
    "", "", "twenty", "thirty", "forty", "fifty",
    "sixty", "seventy", "eighty", "ninety"
};

// Helper for number to words (recursive for <1000)
static int fossil_io_cstring_number_to_words_inner(int num, char *buffer, size_t size) {
    size_t used = strlen(buffer);

    if (num >= 100) {
        int hundreds = num / 100;
        if (used + strlen(units[hundreds]) + 8 >= size) return -1;
        strcat(buffer, units[hundreds]);
        strcat(buffer, " hundred");
        num %= 100;
        if (num > 0) {
            strcat(buffer, " and ");
        }
        used = strlen(buffer);
    }

    if (num >= 20) {
        int t = num / 10;
        if (used + strlen(tens[t]) + 2 >= size) return -1;
        strcat(buffer, tens[t]);
        num %= 10;
        if (num > 0) {
            strcat(buffer, "-");
            strcat(buffer, units[num]);
        }
    } else if (num > 0 || used == 0) {
        if (used + strlen(units[num]) + 1 >= size) return -1;
        strcat(buffer, units[num]);
    }
    return 0;
}

int fossil_io_cstring_number_to_words(int num, char *buffer, size_t size) {
    if (!buffer || size == 0) return -1;
    buffer[0] = '\0';

    if (num < 0 || num > 9999) return -1; // Limit to 0..9999

    if (num >= 1000) {
        int thousands = num / 1000;
        if (strlen(buffer) + strlen(units[thousands]) + 10 >= size) return -1;
        strcat(buffer, units[thousands]);
        strcat(buffer, " thousand");
        num %= 1000;
        if (num > 0) strcat(buffer, " ");
    }

    if (num > 0 || strlen(buffer) == 0) {
        if (fossil_io_cstring_number_to_words_inner(num, buffer, size) != 0) return -1;
    }

    return 0;
}

// Helper for words to number
static int fossil_io_word_to_value(const char *word) {
    for (int i = 0; i < 20; i++) if (strcmp(word, units[i]) == 0) return i;
    for (int i = 2; i < 10; i++) if (strcmp(word, tens[i]) == 0) return i * 10;
    if (strcmp(word, "hundred") == 0) return -100; // multiplier
    if (strcmp(word, "thousand") == 0) return -1000; // multiplier
    return -1; // not found
}

int fossil_io_cstring_number_from_words(const char *str, int *out) {
    if (!str || !out) return -1;

    int total = 0, current = 0;

    char buffer[256];
    strncpy(buffer, str, sizeof(buffer)-1);
    buffer[sizeof(buffer)-1] = '\0';

    // Lowercase and remove extra characters
    for (char *p = buffer; *p; ++p) *p = (char)tolower((unsigned char)*p);

    char *token = strtok(buffer, " -");
    while (token) {
        int val = fossil_io_word_to_value(token);
        if (val >= 0) {
            current += val;
        } else if (val == -100) { // hundred
            if (current == 0) return -1;
            current *= 100;
        } else if (val == -1000) { // thousand
            if (current == 0) return -1;
            total += current * 1000;
            current = 0;
        } else {
            return -1; // unknown word
        }
        token = strtok(NULL, " -");
    }

    *out = total + current;
    return 0;
}

cstring fossil_io_cstring_dup(ccstring str) {
    if (!str) return NULL;
    size_t length = strlen(str);
    cstring new_str = (cstring)malloc(length + 1);
    if (new_str) {
        strcpy(new_str, str);
    }
    return new_str;
}

cstring fossil_io_cstring_concat(ccstring s1, ccstring s2) {
    if (!s1 || !s2) return NULL;
    size_t length1 = strlen(s1);
    size_t length2 = strlen(s2);
    cstring str = (cstring)malloc(length1 + length2 + 1);
    if (str) {
        strcpy(str, s1);
        strcpy(str + length1, s2);
    }
    return str;
}

size_t fossil_io_cstring_length(ccstring str) {
    if (!str) return 0;
    return strlen(str);
}

int fossil_io_cstring_compare(ccstring s1, ccstring s2) {
    if (!s1 || !s2) return -1;
    return strcmp(s1, s2);
}

void fossil_io_cstring_trim(cstring str) {
    if (!str) return;
    size_t length = strlen(str);
    size_t start = 0;
    size_t end = length - 1;
    while (start < length && isspace(str[start])) {
        start++;
    }
    while (end > start && isspace(str[end])) {
        end--;
    }
    size_t count = end - start + 1;
    if (start > 0) {
        memmove(str, str + start, count);
    }
    str[count] = '\0';
}

cstring *fossil_io_cstring_split(ccstring str, char delimiter, size_t *count) {
    if (!str || !count) return NULL;
    size_t length = strlen(str);
    size_t num_delimiters = 0;
    for (size_t i = 0; i < length; i++) {
        if (str[i] == delimiter) {
            num_delimiters++;
        }
    }
    *count = num_delimiters + 1;
    cstring *result = (cstring*)malloc(*count * sizeof(cstring));
    if (result) {
        size_t start = 0;
        size_t index = 0;
        for (size_t i = 0; i < length; i++) {
            if (str[i] == delimiter) {
                size_t count = i - start;
                result[index] = (cstring)malloc(count + 1);
                if (result[index]) {
                    memcpy(result[index], str + start, count);
                    result[index][count] = '\0';
                    start = i + 1;
                    index++;
                } else {
                    // Free previously allocated memory on error
                    for (size_t j = 0; j < index; j++) {
                        free(result[j]);
                    }
                    free(result);
                    return NULL;
                }
            }
        }
        size_t count = length - start;
        result[index] = (cstring)malloc(count + 1);
        if (result[index]) {
            memcpy(result[index], str + start, count);
            result[index][count] = '\0';
        } else {
            // Free previously allocated memory on error
            for (size_t j = 0; j < index; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
    }
    return result;
}

cstring fossil_io_cstring_replace(ccstring str, ccstring old, ccstring new_str) {
    if (!str || !old || !new_str) return NULL;
    size_t old_length = strlen(old);
    size_t new_length = strlen(new_str);
    size_t count = 0;
    size_t index = 0;
    size_t length = strlen(str);
    while (index < length) {
        if (strstr(str + index, old) == str + index) {
            count++;
            index += old_length;
        } else {
            index++;
        }
    }
    cstring result = (cstring)malloc(length + count * (new_length - old_length) + 1);
    if (result) {
        index = 0;
        size_t start = 0;
        while (index < length) {
            if (strstr(str + index, old) == str + index) {
                strcpy(result + start, new_str);
                start += new_length;
                index += old_length;
            } else {
                result[start] = str[index];
                start++;
                index++;
            }
        }
        result[start] = '\0';
    }
    return result;
}

cstring fossil_io_cstring_to_upper(cstring str) {
    if (!str) return NULL;
    size_t length = strlen(str);
    cstring result = (cstring)malloc(length + 1);
    if (result) {
        for (size_t i = 0; i < length; i++) {
            result[i] = toupper(str[i]);
        }
        result[length] = '\0';
    }
    return result;
}

cstring fossil_io_cstring_to_lower(cstring str) {
    if (!str) return NULL;
    size_t length = strlen(str);
    cstring result = (cstring)malloc(length + 1);
    if (result) {
        for (size_t i = 0; i < length; i++) {
            result[i] = tolower(str[i]);
        }
        result[length] = '\0';
    }
    return result;
}

int fossil_io_cstring_starts_with(ccstring str, ccstring prefix) {
    if (!str || !prefix) return 0;
    size_t str_length = strlen(str);
    size_t prefix_length = strlen(prefix);
    if (prefix_length > str_length) {
        return 0;
    }
    return strncmp(str, prefix, prefix_length) == 0;
}

int fossil_io_cstring_ends_with(ccstring str, ccstring suffix) {
    if (!str || !suffix) return 0;
    size_t str_length = strlen(str);
    size_t suffix_length = strlen(suffix);
    if (suffix_length > str_length) {
        return 0;
    }
    return strncmp(str + str_length - suffix_length, suffix, suffix_length) == 0;
}

cstring fossil_io_cstring_substring(ccstring str, size_t start, size_t length) {
    if (!str) return NULL;
    size_t str_length = strlen(str);
    if (start >= str_length) {
        return NULL;
    }
    cstring result = (cstring)malloc(length + 1);
    if (result) {
        size_t count = str_length - start;
        if (length < count) {
            count = length;
        }
        memcpy(result, str + start, count);
        result[count] = '\0';
    }
    return result;
}

cstring fossil_io_cstring_reverse(cstring str) {
    if (!str) return NULL;
    size_t length = strlen(str);
    cstring result = (cstring)malloc(length + 1);
    if (result) {
        for (size_t i = 0; i < length; i++) {
            result[i] = str[length - i - 1];
        }
        result[length] = '\0';
    }
    return result;
}

int fossil_io_cstring_contains(ccstring str, ccstring substr) {
    if (!str || !substr) return 0;
    return strstr(str, substr) != NULL;
}

cstring fossil_io_cstring_repeat(ccstring str, size_t count) {
    if (!str || count == 0) return NULL;
    size_t length = strlen(str);
    size_t new_length = length * count;
    cstring result = (cstring)malloc(new_length + 1);
    if (result) {
        for (size_t i = 0; i < count; i++) {
            memcpy(result + i * length, str, length);
        }
        result[new_length] = '\0';
    }
    return result;
}

cstring fossil_io_cstring_strip(ccstring str, char ch) {
    if (!str) return NULL;
    size_t length = strlen(str);
    size_t start = 0;
    size_t end = length - 1;
    while (start < length && str[start] == ch) {
        start++;
    }
    while (end > start && str[end] == ch) {
        end--;
    }
    size_t count = end - start + 1;
    cstring result = (cstring)malloc(count + 1);
    if (result) {
        memcpy(result, str + start, count);
        result[count] = '\0';
    }
    return result;
}

size_t fossil_io_cstring_count(ccstring str, ccstring substr) {
    if (!str || !substr) return 0;
    size_t count = 0;
    size_t length = strlen(substr);
    while ((str = strstr(str, substr)) != NULL) {
        count++;
        str += length;
    }
    return count;
}

cstring fossil_io_cstring_pad_left(ccstring str, size_t total_length, char pad_char) {
    if (!str || total_length == 0) return NULL;
    size_t length = strlen(str);
    if (length >= total_length) {
        return fossil_io_cstring_copy(str);
    }
    size_t pad_length = total_length - length;
    cstring result = (cstring)malloc(total_length + 1);
    if (result) {
        memset(result, pad_char, pad_length);
        strcpy(result + pad_length, str);
        result[total_length] = '\0';
    }
    return result;
}

cstring fossil_io_cstring_pad_right(ccstring str, size_t total_length, char pad_char) {
    if (!str || total_length == 0) return NULL;
    size_t length = strlen(str);
    if (length >= total_length) {
        return fossil_io_cstring_copy(str);
    }
    size_t pad_length = total_length - length;
    cstring result = (cstring)malloc(total_length + 1);
    if (result) {
        strcpy(result, str);
        memset(result + length, pad_char, pad_length);
        result[total_length] = '\0';
    }
    return result;
}

int fossil_io_cstring_icmp(ccstring str1, ccstring str2) {
    // Handle NULL pointers: both NULL means equal, one NULL means less/greater
    if (str1 == str2) return 0;
    if (!str1) return (str2 && *str2) ? -1 : 0;
    if (!str2) return (*str1) ? 1 : 0;

    // Then check
    while (*str1 && *str2) {
        int c1 = tolower((unsigned char)*str1);
        int c2 = tolower((unsigned char)*str2);
        if (c1 != c2) {
            return c1 - c2;
        }
        str1++;
        str2++;
    }
    return tolower((unsigned char)*str1) - tolower((unsigned char)*str2);
}

int fossil_io_cstring_icontains(ccstring str, ccstring substr) {
    if (!str || !substr || !*substr) return 0;
    size_t substr_len = strlen(substr);
    for (const char *p = str; *p; ++p) {
        size_t i = 0;
        while (i < substr_len &&
               tolower((unsigned char)p[i]) == tolower((unsigned char)substr[i])) {
            i++;
        }
        if (i == substr_len) {
            return 1; // Found (case-insensitive)
        }
        if (!p[i]) break;
    }
    return 0; // Not found
}

cstring fossil_io_cstring_format(ccstring format, ...) {
    va_list args;
    va_start(args, format);
    va_list args_copy;
    va_copy(args_copy, args);

    int length = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (length < 0) {
        va_end(args_copy);
        return NULL;
    }

    char *buffer = malloc((size_t)length + 1);
    if (!buffer) {
        va_end(args_copy);
        return NULL;
    }

    vsnprintf(buffer, (size_t)length + 1, format, args_copy);
    va_end(args_copy);
    return buffer;
}

cstring fossil_io_cstring_join(ccstring *strings, size_t count, char delimiter) {
    if (!strings || count == 0) return fossil_io_cstring_dup("");

    size_t total = 0;
    for (size_t i = 0; i < count; ++i) {
        total += strlen(strings[i]);
    }
    total += count - 1; // space for delimiters

    char *result = malloc(total + 1);
    if (!result) return NULL;

    result[0] = '\0';
    for (size_t i = 0; i < count; ++i) {
        strcat(result, strings[i]);
        if (i != count - 1) {
            size_t len = strlen(result);
            result[len] = delimiter;
            result[len + 1] = '\0';
        }
    }

    return result;
}

int fossil_io_cstring_index_of(ccstring str, ccstring substr) {
    if (!str || !substr) return -1;
    const char *found = strstr(str, substr);
    return found ? (int)(found - str) : -1;
}

int fossil_io_cstring_equals(ccstring a, ccstring b) {
    return strcmp(a, b) == 0;
}

int fossil_io_cstring_iequals(ccstring a, ccstring b) {
    if (!a || !b) return 0;
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++; b++;
    }
    return *a == *b;
}

cstring fossil_io_cstring_escape_json(ccstring str) {
    if (!str) return NULL;

    size_t len = strlen(str);
    size_t est = len * 2 + 1;
    char *escaped = malloc(est);
    if (!escaped) return NULL;

    char *dst = escaped;
    for (const char *src = str; *src; ++src) {
        switch (*src) {
            case '\"': *dst++ = '\\'; *dst++ = '\"'; break;
            case '\\': *dst++ = '\\'; *dst++ = '\\'; break;
            case '\n': *dst++ = '\\'; *dst++ = 'n'; break;
            case '\r': *dst++ = '\\'; *dst++ = 'r'; break;
            case '\t': *dst++ = '\\'; *dst++ = 't'; break;
            case '\b': *dst++ = '\\'; *dst++ = 'b'; break;
            case '\f': *dst++ = '\\'; *dst++ = 'f'; break;
            default:
                *dst++ = *src;
        }
    }
    *dst = '\0';
    return escaped;
}

cstring fossil_io_cstring_unescape_json(ccstring str) {
    if (!str) return NULL;

    char *unescaped = malloc(strlen(str) + 1);
    if (!unescaped) return NULL;

    char *dst = unescaped;
    for (const char *src = str; *src; ++src) {
        if (*src == '\\') {
            ++src;
            switch (*src) {
                case 'n': *dst++ = '\n'; break;
                case 'r': *dst++ = '\r'; break;
                case 't': *dst++ = '\t'; break;
                case 'b': *dst++ = '\b'; break;
                case 'f': *dst++ = '\f'; break;
                case '\\': *dst++ = '\\'; break;
                case '\"': *dst++ = '\"'; break;
                default: *dst++ = *src; break;
            }
        } else {
            *dst++ = *src;
        }
    }
    *dst = '\0';
    return unescaped;
}

cstring fossil_io_cstring_normalize_spaces(cstring str) {
    if (!str) return NULL;

    size_t len = strlen(str);
    char *result = malloc(len + 1);
    if (!result) return NULL;

    int in_space = 0;
    char *dst = result;
    const char *src = str;

    // Skip leading spaces
    while (*src && isspace((unsigned char)*src)) {
        src++;
    }

    for (; *src; ++src) {
        if (isspace((unsigned char)*src)) {
            if (!in_space) {
                *dst++ = ' ';
                in_space = 1;
            }
        } else {
            *dst++ = *src;
            in_space = 0;
        }
    }

    // Trim trailing space
    if (dst > result && dst[-1] == ' ') dst--;
    *dst = '\0';

    return result;
}

cstring fossil_io_cstring_strip_quotes(ccstring str) {
    if (!str) return NULL;
    size_t len = strlen(str);
    if (len < 2) return fossil_io_cstring_dup(str);

    if ((str[0] == '\'' && str[len - 1] == '\'') ||
        (str[0] == '"'  && str[len - 1] == '"')) {
        char *result = malloc(len - 1); // len - 2 + 1
        if (!result) return NULL;
        memcpy(result, str + 1, len - 2);
        result[len - 2] = '\0';
        return result;
    }

    return fossil_io_cstring_dup(str);
}

cstring fossil_io_cstring_append(cstring *dest, ccstring src) {
    if (!dest || !src) return NULL;

    size_t old_len = *dest ? strlen(*dest) : 0;
    size_t add_len = strlen(src);

    char *new_str = realloc(*dest, old_len + add_len + 1);
    if (!new_str) return NULL;

    memcpy(new_str + old_len, src, add_len + 1); // includes null terminator
    *dest = new_str;
    return new_str;
}

// ============================================================================
// Secure String Functions
// ============================================================================

cstring fossil_io_cstring_create_safe(const char *init, size_t max_len) {
    if (!init) return NULL;
    size_t len = strnlen(init, max_len);
    cstring result = fossil_io_cstring_create(init);
    if (!result) return NULL;
    result[len] = '\0'; // enforce null-termination within max_len
    return result;
}

void fossil_io_cstring_free_safe(cstring *str) {
    if (str && *str) {
        fossil_io_cstring_free(*str);
        *str = NULL;
    }
}

cstring fossil_io_cstring_copy_safe(ccstring str, size_t max_len) {
    if (!str) return NULL;
    size_t len = strnlen(str, max_len);
    cstring copy = fossil_io_cstring_copy(str);
    if (!copy) return NULL;
    copy[len] = '\0';
    return copy;
}

cstring fossil_io_cstring_dup_safe(ccstring str, size_t max_len) {
    return fossil_io_cstring_copy_safe(str, max_len);
}

cstring fossil_io_cstring_concat_safe(ccstring s1, ccstring s2, size_t max_len) {
    if (!s1 || !s2) return NULL;
    size_t len1 = strnlen(s1, max_len);
    size_t len2 = strnlen(s2, max_len - len1);
    cstring result = fossil_io_cstring_create(s1);
    if (!result) return NULL;
    fossil_io_cstring_append(&result, s2);
    result[len1 + len2] = '\0';
    return result;
}

size_t fossil_io_cstring_length_safe(ccstring str, size_t max_len) {
    if (!str) return 0;
    return strnlen(str, max_len);
}

int fossil_io_cstring_compare_safe(ccstring s1, ccstring s2, size_t max_len) {
    if (!s1 || !s2) return (s1 == s2) ? 0 : (s1 ? 1 : -1);
    return strncasecmp(s1, s2, max_len);
}

int fossil_io_cstring_append_safe(cstring *dest, ccstring src, size_t max_len) {
    if (!dest || !src || !*dest) return -1;
    size_t current_len = strnlen(*dest, max_len);
    size_t append_len = strnlen(src, max_len - current_len);
    if (current_len + append_len >= max_len) return -1;
    fossil_io_cstring_append(dest, src);
    (*dest)[current_len + append_len] = '\0';
    return 0;
}

cstring fossil_io_cstring_trim_safe(ccstring str, size_t max_len) {
    if (!str) return NULL;
    cstring copy = fossil_io_cstring_copy_safe(str, max_len);
    if (!copy) return NULL;

    // Trim left
    size_t start = 0;
    while (start < max_len && isspace((unsigned char)copy[start])) start++;

    // Trim right
    size_t end = strnlen(copy, max_len);
    while (end > start && isspace((unsigned char)copy[end - 1])) end--;

    size_t trimmed_len = end - start;
    cstring trimmed = (cstring)malloc(trimmed_len + 1);
    if (!trimmed) {
        free(copy);
        return NULL;
    }
    memcpy(trimmed, copy + start, trimmed_len);
    trimmed[trimmed_len] = '\0';
    free(copy);
    return trimmed;
}

cstring *fossil_io_cstring_split_safe(ccstring str, char delimiter, size_t *count, size_t max_len) {
    if (!str || !count) return NULL;

    size_t n = 0;
    for (size_t i = 0; i < strnlen(str, max_len); i++) {
        if (str[i] == delimiter) n++;
    }
    n++; // number of substrings

    cstring *result = (cstring *)calloc(n, sizeof(cstring));
    if (!result) return NULL;

    size_t idx = 0;
    const char *start = str;
    for (size_t i = 0; i <= strnlen(str, max_len); i++) {
        if (str[i] == delimiter || str[i] == '\0') {
            size_t len = &str[i] - start;
            if (len > max_len) len = max_len;
            result[idx++] = fossil_io_cstring_create_safe(start, len);
            start = &str[i + 1];
        }
    }
    *count = n;
    return result;
}

cstring fossil_io_cstring_replace_safe(ccstring str, ccstring old, ccstring new_str, size_t max_len) {
    if (!str || !old || !new_str) return NULL;
    size_t str_len = strnlen(str, max_len);
    cstring result = fossil_io_cstring_create_safe(str, max_len);
    if (!result) return NULL;

    const char *pos = strstr(result, old);
    while (pos) {
        // compute lengths
        size_t prefix_len = pos - result;
        size_t old_len = strnlen(old, max_len);
        size_t new_len = strnlen(new_str, max_len);
        if (prefix_len + new_len > max_len) break;

        // perform replacement
        memmove(result + prefix_len + new_len, pos + old_len, str_len - prefix_len - old_len + 1);
        memcpy(result + prefix_len, new_str, new_len);

        pos = strstr(result + prefix_len + new_len, old);
    }
    result[max_len - 1] = '\0';
    return result;
}

// Upper/lower conversion
cstring fossil_io_cstring_to_upper_safe(ccstring str, size_t max_len) {
    if (!str) return NULL;
    cstring copy = fossil_io_cstring_copy_safe(str, max_len);
    for (size_t i = 0; i < strnlen(copy, max_len); i++) copy[i] = toupper((unsigned char)copy[i]);
    return copy;
}

cstring fossil_io_cstring_to_lower_safe(ccstring str, size_t max_len) {
    if (!str) return NULL;
    cstring copy = fossil_io_cstring_copy_safe(str, max_len);
    for (size_t i = 0; i < strnlen(copy, max_len); i++) copy[i] = tolower((unsigned char)copy[i]);
    return copy;
}

// Safe format string
cstring fossil_io_cstring_format_safe(size_t max_len, ccstring format, ...) {
    if (!format) return NULL;
    cstring buffer = (cstring)malloc(max_len);
    if (!buffer) return NULL;
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, max_len, format, args);
    va_end(args);
    buffer[max_len - 1] = '\0';
    return buffer;
}

// Safe join
cstring fossil_io_cstring_join_safe(ccstring *strings, size_t count, char delimiter, size_t max_len) {
    if (!strings || count == 0) return NULL;
    cstring result = fossil_io_cstring_create_safe("", max_len);
    for (size_t i = 0; i < count; i++) {
        if (i > 0) {
            char delim_str[2] = { delimiter, '\0' };
            fossil_io_cstring_append_safe(&result, delim_str, max_len);
        }
        fossil_io_cstring_append_safe(&result, strings[i], max_len);
    }
    return result;
}

// JSON escape/unescape
cstring fossil_io_cstring_escape_json_safe(ccstring str, size_t max_len) {
    if (!str) return NULL;
    cstring result = fossil_io_cstring_create_safe("", max_len);
    for (size_t i = 0; i < strnlen(str, max_len); i++) {
        char ch = str[i];
        if (ch == '"' || ch == '\\') fossil_io_cstring_append_safe(&result, "\\", max_len);
        char tmp[2] = { ch, 0 };
        fossil_io_cstring_append_safe(&result, tmp, max_len);
    }
    return result;
}

cstring fossil_io_cstring_unescape_json_safe(ccstring str, size_t max_len) {
    if (!str) return NULL;
    cstring result = fossil_io_cstring_create_safe("", max_len);
    for (size_t i = 0; i < strnlen(str, max_len); i++) {
        if (str[i] == '\\' && i + 1 < max_len) {
            i++;
            char tmp[2] = { str[i], 0 };
            fossil_io_cstring_append_safe(&result, tmp, max_len);
        } else {
            char tmp[2] = { str[i], 0 };
            fossil_io_cstring_append_safe(&result, tmp, max_len);
        }
    }
    return result;
}

/* ---------------- Substring ---------------- */
cstring fossil_io_cstring_substring_safe(ccstring str, size_t start, size_t length, size_t max_len) {
    if (!str) return NULL;
    size_t str_len = strnlen(str, max_len);
    if (start >= str_len) return fossil_io_cstring_create_safe("", max_len);
    if (start + length > str_len) length = str_len - start;
    if (length > max_len) length = max_len;
    return fossil_io_cstring_create_safe(str + start, length);
}

/* ---------------- Reverse ---------------- */
cstring fossil_io_cstring_reverse_safe(ccstring str, size_t max_len) {
    if (!str) return NULL;
    size_t len = strnlen(str, max_len);
    cstring copy = fossil_io_cstring_copy_safe(str, len);
    if (!copy) return NULL;
    for (size_t i = 0; i < len / 2; i++) {
        char tmp = copy[i];
        copy[i] = copy[len - 1 - i];
        copy[len - 1 - i] = tmp;
    }
    copy[len] = '\0';
    return copy;
}

/* ---------------- Contains ---------------- */
int fossil_io_cstring_contains_safe(ccstring str, ccstring substr, size_t max_len) {
    if (!str || !substr) return 0;
    size_t len_str = strnlen(str, max_len);
    size_t len_sub = strnlen(substr, max_len);
    if (len_sub > len_str) return 0;
    return strstr(str, substr) != NULL;
}

/* ---------------- Repeat ---------------- */
cstring fossil_io_cstring_repeat_safe(ccstring str, size_t count, size_t max_len) {
    if (!str || count == 0) return fossil_io_cstring_create_safe("", max_len);
    size_t len = strnlen(str, max_len);
    if (len * count > max_len) count = max_len / len;
    cstring result = fossil_io_cstring_create_safe("", max_len);
    for (size_t i = 0; i < count; i++) {
        fossil_io_cstring_append_safe(&result, str, max_len);
    }
    return result;
}

/* ---------------- Strip Character ---------------- */
cstring fossil_io_cstring_strip_safe(ccstring str, char ch, size_t max_len) {
    if (!str) return NULL;
    size_t len = strnlen(str, max_len);
    size_t start = 0, end = len;
    while (start < end && str[start] == ch) start++;
    while (end > start && str[end - 1] == ch) end--;
    return fossil_io_cstring_substring_safe(str, start, end - start, max_len);
}

/* ---------------- Count Substring ---------------- */
size_t fossil_io_cstring_count_safe(ccstring str, ccstring substr, size_t max_len) {
    if (!str || !substr) return 0;
    size_t count = 0;
    size_t len_sub = strnlen(substr, max_len);
    if (len_sub == 0) return 0;

    const char *p = str;
    size_t remaining = strnlen(str, max_len);
    while (remaining >= len_sub) {
        const char *found = strstr(p, substr);
        if (!found) break;
        count++;
        remaining -= (found - p + len_sub);
        p = found + len_sub;
    }
    return count;
}

/* ---------------- Pad Left ---------------- */
cstring fossil_io_cstring_pad_left_safe(ccstring str, size_t total_length, char pad_char, size_t max_len) {
    if (!str) return NULL;
    size_t len = strnlen(str, max_len);
    if (total_length > max_len) total_length = max_len;
    if (len >= total_length) return fossil_io_cstring_substring_safe(str, 0, total_length, max_len);

    cstring result = fossil_io_cstring_create_safe("", max_len);
    for (size_t i = 0; i < total_length - len; i++) fossil_io_cstring_append_safe(&result, (ccstring)&pad_char, max_len);
    fossil_io_cstring_append_safe(&result, str, max_len);
    return result;
}

/* ---------------- Pad Right ---------------- */
cstring fossil_io_cstring_pad_right_safe(ccstring str, size_t total_length, char pad_char, size_t max_len) {
    if (!str) return NULL;
    size_t len = strnlen(str, max_len);
    if (total_length > max_len) total_length = max_len;
    cstring result = fossil_io_cstring_create_safe(str, len);
    for (size_t i = len; i < total_length; i++) fossil_io_cstring_append_safe(&result, (ccstring)&pad_char, max_len);
    return result;
}

/* ---------------- Starts With ---------------- */
int fossil_io_cstring_starts_with_safe(ccstring str, ccstring prefix, size_t max_len) {
    if (!str || !prefix) return 0;
    size_t len_prefix = strnlen(prefix, max_len);
    size_t len_str = strnlen(str, max_len);
    if (len_prefix > len_str) return 0;
    return strncmp(str, prefix, len_prefix) == 0;
}

/* ---------------- Ends With ---------------- */
int fossil_io_cstring_ends_with_safe(ccstring str, ccstring suffix, size_t max_len) {
    if (!str || !suffix) return 0;
    size_t len_suffix = strnlen(suffix, max_len);
    size_t len_str = strnlen(str, max_len);
    if (len_suffix > len_str) return 0;
    return strncmp(str + len_str - len_suffix, suffix, len_suffix) == 0;
}

/* ---------------- Equals / Case-insensitive Equals ---------------- */
int fossil_io_cstring_equals_safe(ccstring a, ccstring b, size_t max_len) {
    if (!a || !b) return 0;
    return strncmp(a, b, max_len) == 0;
}

int fossil_io_cstring_iequals_safe(ccstring a, ccstring b, size_t max_len) {
    if (!a || !b) return 0;
    for (size_t i = 0; i < max_len && a[i] && b[i]; i++) {
        if (tolower((unsigned char)a[i]) != tolower((unsigned char)b[i])) return 0;
        if (a[i+1] == '\0' && b[i+1] == '\0') break;
    }
    return 1;
}

/* ---------------- Case-insensitive Contains ---------------- */
int fossil_io_cstring_icontains_safe(ccstring str, ccstring substr, size_t max_len) {
    if (!str || !substr) return 0;
    size_t len_str = strnlen(str, max_len);
    size_t len_sub = strnlen(substr, max_len);
    if (len_sub > len_str) return 0;
    for (size_t i = 0; i <= len_str - len_sub; i++) {
        size_t j;
        for (j = 0; j < len_sub; j++) {
            if (tolower((unsigned char)str[i + j]) != tolower((unsigned char)substr[j])) break;
        }
        if (j == len_sub) return 1;
    }
    return 0;
}

/* ---------------- Strip Quotes ---------------- */
cstring fossil_io_cstring_strip_quotes_safe(ccstring str, size_t max_len) {
    if (!str) return NULL;
    size_t len = strnlen(str, max_len);
    if (len < 2) return fossil_io_cstring_copy_safe(str, max_len);
    if ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\'' && str[len - 1] == '\'')) {
        return fossil_io_cstring_substring_safe(str, 1, len - 2, max_len);
    }
    return fossil_io_cstring_copy_safe(str, max_len);
}

/* ---------------- Normalize Spaces ---------------- */
cstring fossil_io_cstring_normalize_spaces_safe(ccstring str, size_t max_len) {
    if (!str) return NULL;
    cstring result = fossil_io_cstring_create_safe("", max_len);
    int in_space = 0;
    for (size_t i = 0; i < strnlen(str, max_len); i++) {
        if (isspace((unsigned char)str[i])) {
            if (!in_space) {
                fossil_io_cstring_append_safe(&result, " ", max_len);
                in_space = 1;
            }
        } else {
            char tmp[2] = { str[i], 0 };
            fossil_io_cstring_append_safe(&result, tmp, max_len);
            in_space = 0;
        }
    }
    return result;
}

/* ---------------- Index Of ---------------- */
int fossil_io_cstring_index_of_safe(ccstring str, ccstring substr, size_t max_len) {
    if (!str || !substr) return -1;
    const char *found = strstr(str, substr);
    if (!found) return -1;
    size_t idx = found - str;
    return (idx < max_len) ? (int)idx : -1;
}

// ============================================================================
// String Stream Functions
// ============================================================================

/* ---------------- Creation & Destruction ---------------- */
fossil_io_cstring_stream* fossil_io_cstring_stream_create(size_t initial_size) {
    fossil_io_cstring_stream *stream = (fossil_io_cstring_stream*)malloc(sizeof(fossil_io_cstring_stream));
    if (!stream) return NULL;
    stream->buffer = (char*)malloc(initial_size);
    if (!stream->buffer) {
        free(stream);
        return NULL;
    }
    stream->buffer[0] = '\0';
    stream->length = 0;
    stream->capacity = initial_size;
    return stream;
}

void fossil_io_cstring_stream_free(fossil_io_cstring_stream *stream) {
    if (!stream) return;
    free(stream->buffer);
    free(stream);
}

/* ---------------- Internal Helper ---------------- */
static int fossil_io_cstring_stream_ensure_capacity(fossil_io_cstring_stream *stream, size_t needed) {
    if (!stream) return -1;
    if (needed <= stream->capacity) return 0;
    size_t new_capacity = stream->capacity * 2;
    if (new_capacity < needed) new_capacity = needed;
    char *new_buf = (char*)realloc(stream->buffer, new_capacity);
    if (!new_buf) return -1;
    stream->buffer = new_buf;
    stream->capacity = new_capacity;
    return 0;
}

/* ---------------- Basic Write ---------------- */
void fossil_io_cstring_stream_write(fossil_io_cstring_stream *stream, ccstring str) {
    if (!stream || !str) return;
    size_t len = strlen(str);
    if (fossil_io_cstring_stream_ensure_capacity(stream, stream->length + len + 1) != 0) return;
    memcpy(stream->buffer + stream->length, str, len);
    stream->length += len;
    stream->buffer[stream->length] = '\0';
}

/* ---------------- Safe Write ---------------- */
void fossil_io_cstring_stream_write_safe(fossil_io_cstring_stream *stream, ccstring str, size_t max_len) {
    if (!stream || !str) return;
    size_t len = strnlen(str, max_len);
    if (fossil_io_cstring_stream_ensure_capacity(stream, stream->length + len + 1) != 0) return;
    memcpy(stream->buffer + stream->length, str, len);
    stream->length += len;
    stream->buffer[stream->length] = '\0';
}

/* ---------------- Formatted Write ---------------- */
void fossil_io_cstring_stream_write_format(fossil_io_cstring_stream *stream, ccstring format, ...) {
    if (!stream || !format) return;
    va_list args;
    va_start(args, format);
    int needed = vsnprintf(NULL, 0, format, args);
    va_end(args);
    if (needed <= 0) return;
    if (fossil_io_cstring_stream_ensure_capacity(stream, stream->length + needed + 1) != 0) return;

    va_start(args, format);
    vsnprintf(stream->buffer + stream->length, needed + 1, format, args);
    va_end(args);
    stream->length += needed;
}

/* ---------------- Insert ---------------- */
void fossil_io_cstring_stream_insert(fossil_io_cstring_stream *stream, ccstring str, size_t pos) {
    if (!stream || !str || pos > stream->length) return;
    size_t len = strlen(str);
    if (fossil_io_cstring_stream_ensure_capacity(stream, stream->length + len + 1) != 0) return;
    memmove(stream->buffer + pos + len, stream->buffer + pos, stream->length - pos + 1);
    memcpy(stream->buffer + pos, str, len);
    stream->length += len;
}

/* ---------------- Truncate ---------------- */
void fossil_io_cstring_stream_truncate(fossil_io_cstring_stream *stream, size_t new_length) {
    if (!stream) return;
    if (new_length >= stream->length) return;
    stream->length = new_length;
    stream->buffer[new_length] = '\0';
}

/* ---------------- Clear ---------------- */
void fossil_io_cstring_stream_clear(fossil_io_cstring_stream *stream) {
    if (!stream) return;
    stream->length = 0;
    if (stream->buffer) stream->buffer[0] = '\0';
}

/* ---------------- Reserve ---------------- */
int fossil_io_cstring_stream_reserve(fossil_io_cstring_stream *stream, size_t min_capacity) {
    if (!stream) return -1;
    return fossil_io_cstring_stream_ensure_capacity(stream, min_capacity);
}

/* ---------------- Read ---------------- */
ccstring fossil_io_cstring_stream_read(fossil_io_cstring_stream *stream) {
    if (!stream) return NULL;
    return stream->buffer;
}

/* ---------------- Length / Remaining ---------------- */
size_t fossil_io_cstring_stream_length(fossil_io_cstring_stream *stream) {
    return stream ? stream->length : 0;
}

size_t fossil_io_cstring_stream_capacity_remaining(fossil_io_cstring_stream *stream) {
    return stream ? (stream->capacity - stream->length) : 0;
}
