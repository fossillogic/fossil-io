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
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

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

    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    char *token = strtok(buffer, delims);
    while (token) {
        size_t word_len = strlen(token);
        if (word_len == 0) continue;

        if (strchr("AEIOUaeiou", token[0])) {
            snprintf(word, sizeof(word), "%.*syay", (int)sizeof(buffer) - 1, token);
        } else {
            snprintf(word, sizeof(word), "%s%cay", token + 1, token[0]);
        }

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

// -------------------
// Zalgo Text (simplified)
// -------------------
char* fossil_io_cstring_zalgo(const char *str) {
    if (!str) return NULL;
    size_t len = strlen(str);

    // Each char may get up to 3 combining marks
    char *out = malloc(len * 10 + 1);
    if (!out) return NULL;

    static const char *zalgo_marks[] = {
        "\u0300", "\u0301", "\u0302", "\u0303", "\u0304",
        "\u0306", "\u0307", "\u0308", "\u030A", "\u0315",
        "\u0327", "\u0328", "\u0334", "\u033F", "\u0346"
    };
    static const size_t num_marks = sizeof(zalgo_marks) / sizeof(zalgo_marks[0]);

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        j += sprintf(out + j, "%c", str[i]);

        int marks = rand() % 3; // 0–2 zalgo marks
        for (int m = 0; m < marks; m++) {
            const char *mark = zalgo_marks[rand() % num_marks];
            j += sprintf(out + j, "%s", mark);
        }
    }
    out[j] = '\0';
    return out;
}

cstring fossil_io_cstring_copy(ccstring str) {
    if (!str) return NULL;
    return fossil_io_cstring_create(str);
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
// String Stream Functions
// ============================================================================

fossil_io_cstring_stream* fossil_io_cstring_stream_create(size_t initial_size) {
    if (initial_size == 0) return NULL;
    fossil_io_cstring_stream *stream = (fossil_io_cstring_stream*)malloc(sizeof(fossil_io_cstring_stream));
    if (stream) {
        stream->buffer = (char*)malloc(initial_size);
        if (stream->buffer) {
            stream->buffer[0] = '\0';
            stream->length = 0;
            stream->capacity = initial_size;
        } else {
            free(stream);
            stream = NULL;
        }
    }
    return stream;
}

void fossil_io_cstring_stream_free(fossil_io_cstring_stream *stream) {
    if (stream) {
        free(stream->buffer);
        free(stream);
    }
}

void fossil_io_cstring_stream_write(fossil_io_cstring_stream *stream, ccstring str) {
    if (!stream || !str) return;
    size_t length = strlen(str);
    size_t new_length = stream->length + length;
    if (new_length > stream->capacity) {
        size_t new_capacity = stream->capacity * 2;
        if (new_capacity < new_length) {
            new_capacity = new_length;
        }
        char *new_buffer = (char*)realloc(stream->buffer, new_capacity);
        if (new_buffer) {
            stream->buffer = new_buffer;
            stream->capacity = new_capacity;
        } else {
            return;
        }
    }
    memcpy(stream->buffer + stream->length, str, length);
    stream->length = new_length;
    stream->buffer[stream->length] = '\0';
}

ccstring fossil_io_cstring_stream_read(fossil_io_cstring_stream *stream) {
    if (!stream) return NULL;
    return stream->buffer;
}
