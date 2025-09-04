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
#ifndef FOSSIL_IO_CSTRING_H
#define FOSSIL_IO_CSTRING_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Type definitions */
typedef char* cstring;
typedef const char* ccstring;

typedef struct {
    char *buffer;
    size_t length;
    size_t capacity;
} fossil_io_cstring_stream;

/**
 * @brief Creates a new cstring with the given initial value.
 * 
 * @param init The initial value for the cstring.
 * @return A new cstring initialized with the given value.
 */
cstring fossil_io_cstring_create(const char *init);

/**
 * @brief Frees the memory allocated for the given cstring.
 * 
 * @param str The cstring to be freed.
 */
void fossil_io_cstring_free(cstring str);

/**
 * @brief Creates a copy of the given cstring.
 * 
 * @param str The cstring to be copied.
 * @return A new cstring that is a copy of the given cstring.
 */
cstring fossil_io_cstring_copy(ccstring str);

/**
 * @brief Duplicates the given cstring.
 * 
 * @param str The cstring to be duplicated.
 * @return A new cstring that is a duplicate of the given cstring.
 */
cstring fossil_io_cstring_dup(ccstring str);

/**
 * @brief Concatenates two cstrings into a new cstring.
 * 
 * @param s1 The first cstring.
 * @param s2 The second cstring.
 * @return A new cstring that is the concatenation of s1 and s2.
 */
cstring fossil_io_cstring_concat(ccstring s1, ccstring s2);

/**
 * @brief Returns the length of the given cstring.
 * 
 * @param str The cstring whose length is to be determined.
 * @return The length of the given cstring.
 */
size_t fossil_io_cstring_length(ccstring str);

/**
 * @brief Compares two cstrings.
 * 
 * @param s1 The first cstring.
 * @param s2 The second cstring.
 * @return An integer less than, equal to, or greater than zero if s1 is found, respectively, to be less than, to match, or be greater than s2.
 */
int fossil_io_cstring_compare(ccstring s1, ccstring s2);

/**
 * @brief Trims whitespace from the beginning and end of the given cstring.
 * 
 * @param str The cstring to be trimmed.
 */
void fossil_io_cstring_trim(cstring str);

/**
 * @brief Splits the given cstring by the specified delimiter.
 * 
 * @param str The cstring to be split.
 * @param delimiter The character to split the cstring by.
 * @param count Pointer to a size_t variable where the number of resulting substrings will be stored.
 * @return An array of cstrings resulting from the split operation.
 */
cstring *fossil_io_cstring_split(ccstring str, char delimiter, size_t *count);

/**
 * @brief Replaces all occurrences of a substring within a cstring with another substring.
 * 
 * @param str The original cstring.
 * @param old The substring to be replaced.
 * @param new_str The substring to replace with.
 * @return A new cstring with the replacements made.
 */
cstring fossil_io_cstring_replace(ccstring str, ccstring old, ccstring new_str);

/**
 * @brief Converts all characters in the given cstring to uppercase.
 * 
 * @param str The cstring to be converted.
 * @return The cstring with all characters converted to uppercase.
 */
cstring fossil_io_cstring_to_upper(cstring str);

/**
 * @brief Converts all characters in the given cstring to lowercase.
 * 
 * @param str The cstring to be converted.
 * @return The cstring with all characters converted to lowercase.
 */
cstring fossil_io_cstring_to_lower(cstring str);

/**
 * @brief Checks if the given cstring starts with the specified prefix.
 * 
 * @param str The cstring to be checked.
 * @param prefix The prefix to check for.
 * @return 1 if the cstring starts with the prefix, 0 otherwise.
 */
int fossil_io_cstring_starts_with(ccstring str, ccstring prefix);

/**
 * @brief Checks if the given cstring ends with the specified suffix.
 * 
 * @param str The cstring to be checked.
 * @param suffix The suffix to check for.
 * @return 1 if the cstring ends with the suffix, 0 otherwise.
 */
int fossil_io_cstring_ends_with(ccstring str, ccstring suffix);

/**
 * @brief Extracts a substring from the given cstring.
 * 
 * @param str The original cstring.
 * @param start The starting index of the substring.
 * @param length The length of the substring.
 * @return A new cstring that is the specified substring of the original cstring.
 */
cstring fossil_io_cstring_substring(ccstring str, size_t start, size_t length);

/**
 * @brief Reverses the given cstring.
 * 
 * @param str The cstring to be reversed.
 * @return A new cstring that is the reverse of the given cstring.
 */
cstring fossil_io_cstring_reverse(cstring str);

/**
 * @brief Checks if the given cstring contains the specified substring.
 * 
 * @param str The cstring to be checked.
 * @param substr The substring to check for.
 * @return 1 if the cstring contains the substring, 0 otherwise.
 */
int fossil_io_cstring_contains(ccstring str, ccstring substr);

/**
 * @brief Repeats the given cstring the specified number of times.
 * 
 * @param str The cstring to be repeated.
 * @param count The number of times to repeat the cstring.
 * @return A new cstring that is the original cstring repeated the specified number of times.
 */
cstring fossil_io_cstring_repeat(ccstring str, size_t count);

/**
 * @brief Strips the given character from the beginning and end of the cstring.
 * 
 * @param str The cstring to be stripped.
 * @param ch The character to strip.
 * @return A new cstring that is the original cstring with the specified character stripped from the beginning and end.
 */
cstring fossil_io_cstring_strip(ccstring str, char ch);

/**
 * @brief Counts the number of occurrences of a substring within the given cstring.
 * 
 * @param str The cstring to be searched.
 * @param substr The substring to search for.
 * @return The number of occurrences of the substring within the cstring.
 */
size_t fossil_io_cstring_count(ccstring str, ccstring substr);

/**
 * @brief Pads the given cstring with the specified character on the left side.
 * 
 * @param str The cstring to be padded.
 * @param total_length The total length of the resulting cstring.
 * @param pad_char The character to pad with.
 * @return A new cstring that is the original cstring padded on the left side.
 */
cstring fossil_io_cstring_pad_left(ccstring str, size_t total_length, char pad_char);

/**
 * @brief Pads the given cstring with the specified character on the right side.
 * 
 * @param str The cstring to be padded.
 * @param total_length The total length of the resulting cstring.
 * @param pad_char The character to pad with.
 * @return A new cstring that is the original cstring padded on the right side.
 */
cstring fossil_io_cstring_pad_right(ccstring str, size_t total_length, char pad_char);

/**
 * @brief Performs a case-insensitive comparison of two C strings.
 *
 * Compares the characters of `str1` and `str2` one by one, ignoring case.
 *
 * @param str1 The first null-terminated string to compare.
 * @param str2 The second null-terminated string to compare.
 * @return 1 if the strings are equal (case-insensitive), 0 otherwise.
 */
int fossil_io_cstring_icmp(ccstring str1, ccstring str2);

/**
 * @brief Checks if a substring is contained within a string (case-insensitive).
 *
 * Iterates through `str` and checks if `substr` occurs at any position, ignoring case.
 *
 * @param str The null-terminated string to search within.
 * @param substr The null-terminated substring to search for.
 * @return 1 if `substr` is found within `str` (case-insensitive), 0 otherwise.
 */
int fossil_io_cstring_icontains(ccstring str, ccstring substr);

/**
 * @brief Creates a new cstring using a formatted string (like sprintf).
 * 
 * @param format The format string.
 * @param ... The arguments for the format string.
 * @return A newly allocated cstring.
 */
cstring fossil_io_cstring_format(ccstring format, ...);

/**
 * @brief Joins an array of strings with a delimiter.
 * 
 * @param strings The array of cstrings.
 * @param count The number of elements in the array.
 * @param delimiter The delimiter to insert between strings.
 * @return A new cstring consisting of all strings joined by the delimiter.
 */
cstring fossil_io_cstring_join(ccstring *strings, size_t count, char delimiter);

/**
 * @brief Finds the first index of a substring within a string.
 * 
 * @param str The cstring to search.
 * @param substr The substring to find.
 * @return The index of the first occurrence, or -1 if not found.
 */
int fossil_io_cstring_index_of(ccstring str, ccstring substr);

/**
 * @brief Checks if two cstrings are exactly equal (case-sensitive).
 *
 * Compares two null-terminated strings character by character.
 *
 * @param a The first cstring to compare.
 * @param b The second cstring to compare.
 * @return 1 if the strings are exactly equal, 0 otherwise.
 */
int fossil_io_cstring_equals(ccstring a, ccstring b);

/**
 * @brief Checks if two cstrings are equal, ignoring case.
 *
 * Compares two null-terminated strings in a case-insensitive manner.
 *
 * @param a The first cstring to compare.
 * @param b The second cstring to compare.
 * @return 1 if the strings are equal ignoring case, 0 otherwise.
 */
int fossil_io_cstring_iequals(ccstring a, ccstring b);

/**
 * @brief Escapes a cstring for safe use in JSON strings.
 *
 * Replaces special characters like quotes, backslashes, and control characters
 * with their escaped equivalents (e.g., `"` becomes `\"`, `\n` becomes `\\n`, etc.).
 *
 * @param str The original cstring to escape.
 * @return A newly allocated cstring containing the JSON-escaped result.
 */
cstring fossil_io_cstring_escape_json(ccstring str);

/**
 * @brief Unescapes a JSON-escaped cstring.
 *
 * Converts escaped sequences such as `\\n`, `\\t`, `\\uXXXX`, and `\\\"` into
 * their actual character representations.
 *
 * @param str The JSON-escaped cstring to unescape.
 * @return A newly allocated cstring with escape sequences resolved.
 */
cstring fossil_io_cstring_unescape_json(ccstring str);

/**
 * @brief Normalizes whitespace in a cstring by collapsing multiple spaces.
 *
 * Replaces sequences of two or more whitespace characters with a single space.
 * Leading and trailing whitespace is trimmed.
 *
 * @param str The cstring to normalize.
 * @return A newly allocated cstring with normalized whitespace.
 */
cstring fossil_io_cstring_normalize_spaces(cstring str);

/**
 * @brief Removes surrounding single or double quotes from a cstring.
 *
 * If the string begins and ends with the same quote character (`"` or `'`),
 * both are removed. Does not modify the contents inside the quotes.
 *
 * @param str The cstring to process.
 * @return A newly allocated cstring without surrounding quotes.
 */
cstring fossil_io_cstring_strip_quotes(ccstring str);

/**
 * @brief Appends src to dest in-place, resizing as necessary.
 * 
 * @param dest A pointer to the destination cstring.
 * @param src The string to append.
 * @return 0 on success, non-zero on allocation failure.
 */
cstring fossil_io_cstring_append(cstring *dest, ccstring src);

// Secure String 

/**
 * @brief Creates a new cstring with the given initial value safely.
 *
 * Allocates a new buffer for the string, ensuring null-termination.
 *
 * @param init The initial value for the cstring.
 * @return A new cstring initialized with the given value, or NULL on failure.
 */
cstring fossil_io_cstring_create_safe(const char *init, size_t max_len);

/**
 * @brief Frees a cstring safely.
 *
 * Sets the pointer to NULL after freeing to prevent dangling references.
 *
 * @param str Pointer to the cstring to free.
 */
void fossil_io_cstring_free_safe(cstring *str);

/**
 * @brief Creates a safe copy of the given cstring.
 *
 * @param str The cstring to copy.
 * @param max_len Maximum number of characters to copy.
 * @return A newly allocated cstring copy, or NULL on failure.
 */
cstring fossil_io_cstring_copy_safe(ccstring str, size_t max_len);

/**
 * @brief Duplicates the given cstring safely.
 *
 * @param str The cstring to duplicate.
 * @param max_len Maximum number of characters to duplicate.
 * @return A newly allocated duplicate cstring, or NULL on failure.
 */
cstring fossil_io_cstring_dup_safe(ccstring str, size_t max_len);

/**
 * @brief Concatenates two cstrings safely.
 *
 * Ensures that the resulting string is null-terminated and does not overflow.
 *
 * @param s1 First cstring.
 * @param s2 Second cstring.
 * @param max_len Maximum allowed length of the resulting string.
 * @return A new concatenated cstring, or NULL on failure.
 */
cstring fossil_io_cstring_concat_safe(ccstring s1, ccstring s2, size_t max_len);

/**
 * @brief Returns the length of the given cstring safely.
 *
 * Ensures that the string is properly null-terminated within max_len.
 *
 * @param str The cstring whose length is determined.
 * @param max_len Maximum characters to scan.
 * @return The length of the cstring up to max_len.
 */
size_t fossil_io_cstring_length_safe(ccstring str, size_t max_len);

/**
 * @brief Compares two cstrings safely.
 *
 * Ensures no buffer overrun; comparison stops at max_len or null terminator.
 *
 * @param s1 First cstring.
 * @param s2 Second cstring.
 * @param max_len Maximum characters to compare.
 * @return <0 if s1<s2, 0 if equal, >0 if s1>s2
 */
int fossil_io_cstring_compare_safe(ccstring s1, ccstring s2, size_t max_len);

/**
 * @brief Safely concatenates src to dest in-place with bounds checking.
 *
 * Resizes the buffer as needed. Ensures null-termination.
 *
 * @param dest Pointer to destination cstring.
 * @param src Source cstring to append.
 * @param max_len Maximum allowed length for dest after append.
 * @return 0 on success, non-zero on failure.
 */
int fossil_io_cstring_append_safe(cstring *dest, ccstring src, size_t max_len);

/**
 * @brief Trims whitespace safely from a cstring.
 *
 * Ensures null-termination and modifies in-place or allocates a new string.
 *
 * @param str The cstring to trim.
 * @param max_len Maximum length of the string to process.
 */
cstring fossil_io_cstring_trim_safe(ccstring str, size_t max_len);

/**
 * @brief Splits a cstring safely by a delimiter.
 *
 * Allocates an array of strings, each safely null-terminated.
 *
 * @param str The string to split.
 * @param delimiter Character to split by.
 * @param count Pointer to store number of substrings.
 * @param max_len Maximum length of each substring.
 * @return Array of safe cstrings, or NULL on failure.
 */
cstring *fossil_io_cstring_split_safe(ccstring str, char delimiter, size_t *count, size_t max_len);

/**
 * @brief Replaces all occurrences of a substring safely.
 *
 * Allocates a new string and ensures null-termination.
 *
 * @param str Original string.
 * @param old Substring to replace.
 * @param new_str Replacement substring.
 * @param max_len Maximum length of resulting string.
 * @return A new cstring with replacements, or NULL on failure.
 */
cstring fossil_io_cstring_replace_safe(ccstring str, ccstring old, ccstring new_str, size_t max_len);

/**
 * @brief Converts a cstring to uppercase safely.
 *
 * Allocates a new string, ensures null-termination.
 *
 * @param str The cstring to convert.
 * @param max_len Maximum allowed length.
 * @return Uppercase cstring, or NULL on failure.
 */
cstring fossil_io_cstring_to_upper_safe(ccstring str, size_t max_len);

/**
 * @brief Converts a cstring to lowercase safely.
 *
 * @param str The cstring to convert.
 * @param max_len Maximum allowed length.
 * @return Lowercase cstring, or NULL on failure.
 */
cstring fossil_io_cstring_to_lower_safe(ccstring str, size_t max_len);

/**
 * @brief Safely creates a formatted cstring.
 *
 * Allocates enough memory for the resulting string; guarantees null-termination.
 *
 * @param max_len Maximum length for formatted string.
 * @param format Format string.
 * @param ... Format arguments.
 * @return Newly allocated formatted cstring, or NULL on failure.
 */
cstring fossil_io_cstring_format_safe(size_t max_len, ccstring format, ...);

/**
 * @brief Safely joins multiple strings with a delimiter.
 *
 * @param strings Array of cstrings.
 * @param count Number of elements.
 * @param delimiter Character to insert.
 * @param max_len Maximum allowed length of resulting string.
 * @return New cstring or NULL on failure.
 */
cstring fossil_io_cstring_join_safe(ccstring *strings, size_t count, char delimiter, size_t max_len);

/**
 * @brief Safely escapes a cstring for JSON output.
 *
 * Ensures resulting string is null-terminated and memory-safe.
 *
 * @param str Input cstring.
 * @param max_len Maximum length of resulting escaped string.
 * @return Escaped cstring, or NULL on failure.
 */
cstring fossil_io_cstring_escape_json_safe(ccstring str, size_t max_len);

/**
 * @brief Safely unescapes a JSON-escaped cstring.
 *
 * @param str JSON-escaped string.
 * @param max_len Maximum length of resulting string.
 * @return Unescaped cstring, or NULL on failure.
 */
cstring fossil_io_cstring_unescape_json_safe(ccstring str, size_t max_len);

/**
 * @brief Extracts a substring from a given cstring safely.
 *
 * Ensures bounds checking and respects maximum length.
 *
 * @param str The original cstring.
 * @param start The starting index for the substring.
 * @param length The number of characters to include.
 * @param max_len Maximum allowed length to prevent buffer overrun.
 * @return A newly allocated cstring containing the substring, or NULL on failure.
 */
cstring fossil_io_cstring_substring_safe(ccstring str, size_t start, size_t length, size_t max_len);

/**
 * @brief Reverses a cstring safely.
 *
 * Creates a new string that is the reverse of the input while ensuring memory safety.
 *
 * @param str The cstring to reverse.
 * @param max_len Maximum length to process to prevent overflow.
 * @return A newly allocated reversed cstring, or NULL on failure.
 */
cstring fossil_io_cstring_reverse_safe(ccstring str, size_t max_len);

/**
 * @brief Checks if a cstring contains a substring safely.
 *
 * Ensures that no buffer overruns occur.
 *
 * @param str The cstring to search.
 * @param substr The substring to look for.
 * @param max_len Maximum number of characters to scan.
 * @return 1 if substring exists, 0 otherwise.
 */
int fossil_io_cstring_contains_safe(ccstring str, ccstring substr, size_t max_len);

/**
 * @brief Repeats a cstring multiple times safely.
 *
 * Ensures the resulting string does not exceed max_len.
 *
 * @param str The string to repeat.
 * @param count Number of repetitions.
 * @param max_len Maximum length of resulting string.
 * @return A newly allocated repeated cstring, or NULL on failure.
 */
cstring fossil_io_cstring_repeat_safe(ccstring str, size_t count, size_t max_len);

/**
 * @brief Strips a specified character from both ends of a cstring safely.
 *
 * @param str The cstring to process.
 * @param ch The character to strip.
 * @param max_len Maximum length to process.
 * @return A new cstring with the specified character removed from start and end.
 */
cstring fossil_io_cstring_strip_safe(ccstring str, char ch, size_t max_len);

/**
 * @brief Counts the occurrences of a substring within a cstring safely.
 *
 * Ensures safe bounds checking.
 *
 * @param str The cstring to search.
 * @param substr The substring to count.
 * @param max_len Maximum length to scan.
 * @return The number of occurrences of the substring.
 */
size_t fossil_io_cstring_count_safe(ccstring str, ccstring substr, size_t max_len);

/**
 * @brief Pads a cstring on the left with a specific character safely.
 *
 * Ensures total length does not exceed max_len.
 *
 * @param str The cstring to pad.
 * @param total_length The desired total length after padding.
 * @param pad_char Character to pad with.
 * @param max_len Maximum allowed length of the resulting string.
 * @return A newly allocated padded cstring.
 */
cstring fossil_io_cstring_pad_left_safe(ccstring str, size_t total_length, char pad_char, size_t max_len);

/**
 * @brief Pads a cstring on the right with a specific character safely.
 *
 * Ensures total length does not exceed max_len.
 *
 * @param str The cstring to pad.
 * @param total_length The desired total length after padding.
 * @param pad_char Character to pad with.
 * @param max_len Maximum allowed length of the resulting string.
 * @return A newly allocated padded cstring.
 */
cstring fossil_io_cstring_pad_right_safe(ccstring str, size_t total_length, char pad_char, size_t max_len);

/**
 * @brief Checks if a cstring starts with a specified prefix safely.
 *
 * @param str The cstring to check.
 * @param prefix The prefix to test.
 * @param max_len Maximum characters to consider.
 * @return 1 if the cstring starts with the prefix, 0 otherwise.
 */
int fossil_io_cstring_starts_with_safe(ccstring str, ccstring prefix, size_t max_len);

/**
 * @brief Checks if a cstring ends with a specified suffix safely.
 *
 * @param str The cstring to check.
 * @param suffix The suffix to test.
 * @param max_len Maximum characters to consider.
 * @return 1 if the cstring ends with the suffix, 0 otherwise.
 */
int fossil_io_cstring_ends_with_safe(ccstring str, ccstring suffix, size_t max_len);

/**
 * @brief Compares two cstrings for exact equality safely.
 *
 * @param a First cstring.
 * @param b Second cstring.
 * @param max_len Maximum characters to compare.
 * @return 1 if equal, 0 otherwise.
 */
int fossil_io_cstring_equals_safe(ccstring a, ccstring b, size_t max_len);

/**
 * @brief Compares two cstrings for equality ignoring case safely.
 *
 * @param a First cstring.
 * @param b Second cstring.
 * @param max_len Maximum characters to compare.
 * @return 1 if equal ignoring case, 0 otherwise.
 */
int fossil_io_cstring_iequals_safe(ccstring a, ccstring b, size_t max_len);

/**
 * @brief Checks if a substring exists in a cstring ignoring case safely.
 *
 * @param str The cstring to search.
 * @param substr The substring to look for.
 * @param max_len Maximum characters to scan.
 * @return 1 if found, 0 otherwise.
 */
int fossil_io_cstring_icontains_safe(ccstring str, ccstring substr, size_t max_len);

/**
 * @brief Removes surrounding quotes from a cstring safely.
 *
 * @param str The cstring to process.
 * @param max_len Maximum characters to process.
 * @return A new cstring without surrounding quotes.
 */
cstring fossil_io_cstring_strip_quotes_safe(ccstring str, size_t max_len);

/**
 * @brief Normalizes whitespace in a cstring safely.
 *
 * Collapses multiple spaces into one and trims leading/trailing whitespace.
 *
 * @param str The cstring to normalize.
 * @param max_len Maximum characters to process.
 * @return A newly allocated normalized cstring.
 */
cstring fossil_io_cstring_normalize_spaces_safe(ccstring str, size_t max_len);

/**
 * @brief Returns the index of a substring in a cstring safely.
 *
 * @param str The cstring to search.
 * @param substr The substring to find.
 * @param max_len Maximum characters to consider.
 * @return Index of first occurrence, or -1 if not found.
 */
int fossil_io_cstring_index_of_safe(ccstring str, ccstring substr, size_t max_len);

// String Stream

/**
 * @brief Creates a new cstring stream with the specified initial size.
 * 
 * @param initial_size The initial size of the cstring stream.
 * @return A pointer to the newly created cstring stream.
 */
fossil_io_cstring_stream* fossil_io_cstring_stream_create(size_t initial_size);

/**
 * @brief Frees the memory allocated for the given cstring stream.
 * 
 * @param stream The cstring stream to be freed.
 */
void fossil_io_cstring_stream_free(fossil_io_cstring_stream *stream);

/**
 * @brief Writes a cstring to the given cstring stream.
 * 
 * @param stream The cstring stream to write to.
 * @param str The cstring to be written.
 */
void fossil_io_cstring_stream_write(fossil_io_cstring_stream *stream, ccstring str);

/**
 * @brief Reads the contents of the given cstring stream.
 * 
 * @param stream The cstring stream to read from.
 * @return The contents of the cstring stream as a cstring.
 */
ccstring fossil_io_cstring_stream_read(fossil_io_cstring_stream *stream);

/**
 * @brief Writes a string to the stream safely, respecting a maximum length.
 *
 * @param stream Pointer to the stream.
 * @param str Null-terminated string to append.
 * @param max_len Maximum number of characters to append.
 */
void fossil_io_cstring_stream_write_safe(fossil_io_cstring_stream *stream, ccstring str, size_t max_len);

/**
 * @brief Writes a formatted string to the stream (like printf).
 *
 * @param stream Pointer to the stream.
 * @param format Format string.
 * @param ... Format arguments.
 */
void fossil_io_cstring_stream_write_format(fossil_io_cstring_stream *stream, ccstring format, ...);

/**
 * @brief Inserts a cstring at a specified position in the stream.
 *
 * Shifts existing data to accommodate the new string.
 *
 * @param stream Pointer to the stream.
 * @param str String to insert.
 * @param pos Position to insert at (0 = beginning).
 */
void fossil_io_cstring_stream_insert(fossil_io_cstring_stream *stream, ccstring str, size_t pos);

/**
 * @brief Truncates the stream to the specified length.
 *
 * Reduces the stream length safely, optionally zeroing truncated data.
 *
 * @param stream Pointer to the stream.
 * @param new_length New desired length of the stream.
 */
void fossil_io_cstring_stream_truncate(fossil_io_cstring_stream *stream, size_t new_length);

/**
 * @brief Clears the stream content without freeing the buffer.
 *
 * Resets length to 0 and optionally zeroes memory.
 *
 * @param stream Pointer to the stream.
 */
void fossil_io_cstring_stream_clear(fossil_io_cstring_stream *stream);

/**
 * @brief Ensures the stream has at least the specified capacity.
 *
 * Resizes the buffer if necessary.
 *
 * @param stream Pointer to the stream.
 * @param min_capacity Minimum capacity required.
 * @return 0 on success, non-zero on allocation failure.
 */
int fossil_io_cstring_stream_reserve(fossil_io_cstring_stream *stream, size_t min_capacity);

/**
 * @brief Returns the current length of the stream.
 *
 * @param stream Pointer to the stream.
 * @return Length of data in the stream.
 */
size_t fossil_io_cstring_stream_length(fossil_io_cstring_stream *stream);

/**
 * @brief Returns the remaining capacity of the stream.
 *
 * @param stream Pointer to the stream.
 * @return Number of bytes available before resizing is required.
 */
size_t fossil_io_cstring_stream_capacity_remaining(fossil_io_cstring_stream *stream);

#ifdef __cplusplus
}

#include <string>
#include <vector>

/**
 * namespace for Fossil Logic.
 */
namespace fossil {

    /**
     * Namespace for I/O operations.
     */
    namespace io {
        
        class CString {
        private:
            cstring _str;

        public:
            /**
             * Constructor to create a new cstring with the given initial value.
             * 
             * @param init The initial value for the cstring.
             */
            CString(const std::string &init) {
                _str = fossil_io_cstring_create(init.c_str());
            }

            /**
             * Constructor to create a CString from an existing cstring.
             * 
             * @param str The cstring to wrap.
             */
            CString(ccstring str) {
                _str = fossil_io_cstring_copy(str);
            }

            /**
             * Default constructor to create an empty CString.
             */
            CString() {
                _str = fossil_io_cstring_create("");
            }

            /**
             * Destructor to free the memory allocated for the cstring.
             */
            ~CString() {
                fossil_io_cstring_free(_str);
            }

            /**
             * Creates a copy of the given cstring.
             * 
             * @param str The cstring to be copied.
             * @return A new CString that is a copy of the given cstring.
             */
            static CString copy(const std::string &str) {
                return CString(fossil_io_cstring_copy(str.c_str()));
            }

            /**
             * Duplicates the given cstring.
             * 
             * @param str The cstring to be duplicated.
             * @return A new CString that is a duplicate of the given cstring.
             */
            static CString dup(const std::string &str) {
                return CString(fossil_io_cstring_dup(str.c_str()));
            }

            /**
             * Concatenates two cstrings into a new CString.
             * 
             * @param s1 The first cstring.
             * @param s2 The second cstring.
             * @return A new CString that is the concatenation of s1 and s2.
             */
            static CString concat(const std::string &s1, const std::string &s2) {
                return CString(fossil_io_cstring_concat(s1.c_str(), s2.c_str()));
            }

            /**
             * Returns the length of the cstring.
             * 
             * @return The length of the cstring.
             */
            size_t length() const {
                return fossil_io_cstring_length(_str);
            }

            /**
             * Compares this cstring with another cstring.
             * 
             * @param other The other cstring to compare with.
             * @return An integer less than, equal to, or greater than zero if this cstring is found, respectively, to be less than, to match, or be greater than the other cstring.
             */
            int compare(const std::string &other) const {
                return fossil_io_cstring_compare(_str, other.c_str());
            }

            /**
             * Trims whitespace from the beginning and end of the cstring.
             */
            void trim() {
                fossil_io_cstring_trim(_str);
            }

            /**
             * Splits the cstring by the specified delimiter.
             * 
             * @param delimiter The character to split the cstring by.
             * @param count Pointer to a size_t variable where the number of resulting substrings will be stored.
             * @return An array of cstrings resulting from the split operation.
             */
            std::vector<std::string> split(char delimiter, size_t *count) const {
                cstring *result = fossil_io_cstring_split(_str, delimiter, count);
                std::vector<std::string> vec;
                for (size_t i = 0; i < *count; i++) {
                    vec.push_back(result[i]);
                }
                return vec;
            }

            /**
             * Replaces all occurrences of a substring within the cstring with another substring.
             * 
             * @param old The substring to be replaced.
             * @param new_str The substring to replace with.
             * @return A new CString with the replacements made.
             */
            CString replace(const std::string &old, const std::string &new_str) const {
                return CString(fossil_io_cstring_replace(_str, old.c_str(), new_str.c_str()));
            }

            /**
             * Converts all characters in the cstring to uppercase.
             * 
             * @return The cstring with all characters converted to uppercase.
             */
            CString to_upper() const {
                return CString(fossil_io_cstring_to_upper(_str));
            }

            /**
             * Converts all characters in the cstring to lowercase.
             * 
             * @return The cstring with all characters converted to lowercase.
             */
            CString to_lower() const {
                return CString(fossil_io_cstring_to_lower(_str));
            }

            /**
             * Checks if the cstring starts with the specified prefix.
             * 
             * @param prefix The prefix to check for.
             * @return True if the cstring starts with the prefix, false otherwise.
             */
            bool starts_with(const std::string &prefix) const {
                return fossil_io_cstring_starts_with(_str, prefix.c_str());
            }

            /**
             * Checks if the cstring ends with the specified suffix.
             * 
             * @param suffix The suffix to check for.
             * @return True if the cstring ends with the suffix, false otherwise.
             */
            bool ends_with(const std::string &suffix) const {
                return fossil_io_cstring_ends_with(_str, suffix.c_str());
            }

            /**
             * Extracts a substring from the cstring.
             * 
             * @param start The starting index of the substring.
             * @param length The length of the substring.
             * @return A new CString that is the specified substring of the original cstring.
             */
            CString substring(size_t start, size_t length) const {
                return CString(fossil_io_cstring_substring(_str, start, length));
            }

            /**
             * Reverses the cstring.
             * 
             * @return A new CString that is the reverse of the cstring.
             */
            CString reverse() const {
                return CString(fossil_io_cstring_reverse(_str));
            }

            /**
             * Checks if the cstring contains the specified substring.
             * 
             * @param substr The substring to check for.
             * @return True if the cstring contains the substring, false otherwise.
             */
            bool contains(const std::string &substr) const {
                return fossil_io_cstring_contains(_str, substr.c_str());
            }

            /**
             * Repeats the cstring the specified number of times.
             * 
             * @param count The number of times to repeat the cstring.
             * @return A new CString that is the original cstring repeated the specified number of times.
             */
            CString repeat(size_t count) const {
                return CString(fossil_io_cstring_repeat(_str, count));
            }

            /**
             * Strips the given character from the beginning and end of the cstring.
             * 
             * @param ch The character to strip.
             * @return A new CString that is the original cstring with the specified character stripped from the beginning and end.
             */
            CString strip(char ch) const {
                return CString(fossil_io_cstring_strip(_str, ch));
            }

            /**
             * Counts the number of occurrences of a substring within the cstring.
             * 
             * @param substr The substring to search for.
             * @return The number of occurrences of the substring within the cstring.
             */
            size_t count(const std::string &substr) const {
                return fossil_io_cstring_count(_str, substr.c_str());
            }

            /**
             * Pads the cstring with the specified character on the left side.
             * 
             * @param total_length The total length of the resulting cstring.
             * @param pad_char The character to pad with.
             * @return A new CString that is the original cstring padded on the left side.
             */
            CString pad_left(size_t total_length, char pad_char) const {
                return CString(fossil_io_cstring_pad_left(_str, total_length, pad_char));
            }

            /**
             * Pads the cstring with the specified character on the right side.
             * 
             * @param total_length The total length of the resulting cstring.
             * @param pad_char The character to pad with.
             * @return A new CString that is the original cstring padded on the right side.
             */
            CString pad_right(size_t total_length, char pad_char) const {
                return CString(fossil_io_cstring_pad_right(_str, total_length, pad_char));
            }

            /**
             * Performs a case-insensitive comparison of two C strings.
             * 
             * @param other The other string to compare with.
             * @return True if the strings are equal (case-insensitive), false otherwise.
             */
            bool icmp(const std::string &other) const {
                return fossil_io_cstring_icmp(_str, other.c_str()) == 1;
            }

            /**
             * Checks if a substring is contained within the string (case-insensitive).
             * 
             * @param substr The substring to search for.
             * @return True if the substring is found (case-insensitive), false otherwise.
             */
            bool icontains(const std::string &substr) const {
                return fossil_io_cstring_icontains(_str, substr.c_str()) == 1;
            }

            /**
             * Creates a new CString using a formatted string (like sprintf).
             * 
             * @param format The format string.
             * @param ... The arguments for the format string.
             * @return A newly allocated CString.
             */
            static CString format(const char *format, ...) {
                va_list args;
                va_start(args, format);
                cstring result = fossil_io_cstring_format(format, args);
                va_end(args);
                return CString(result);
            }

            /**
             * Joins an array of strings with a delimiter.
             * 
             * @param strings The array of strings.
             * @param delimiter The delimiter to insert between strings.
             * @return A new CString consisting of all strings joined by the delimiter.
             */
            static CString join(const std::vector<std::string> &strings, char delimiter) {
                std::vector<ccstring> cstrs;
                for (const auto &s : strings) {
                    cstrs.push_back(s.c_str());
                }
                cstring result = fossil_io_cstring_join(const_cast<ccstring*>(cstrs.data()), cstrs.size(), delimiter);
                return CString(result);
            }

            /**
             * Finds the first index of a substring within the string.
             * 
             * @param substr The substring to find.
             * @return The index of the first occurrence, or -1 if not found.
             */
            int index_of(const std::string &substr) const {
                return fossil_io_cstring_index_of(_str, substr.c_str());
            }

            /**
             * Checks if two strings are exactly equal (case-sensitive).
             * 
             * @param other The other string to compare.
             * @return True if the strings are exactly equal, false otherwise.
             */
            bool equals(const std::string &other) const {
                return fossil_io_cstring_equals(_str, other.c_str()) == 1;
            }

            /**
             * Checks if two strings are equal, ignoring case.
             * 
             * @param other The other string to compare.
             * @return True if the strings are equal ignoring case, false otherwise.
             */
            bool iequals(const std::string &other) const {
                return fossil_io_cstring_iequals(_str, other.c_str()) == 1;
            }

            /**
             * Escapes the string for safe use in JSON strings.
             * 
             * @return A newly allocated CString containing the JSON-escaped result.
             */
            CString escape_json() const {
                return CString(fossil_io_cstring_escape_json(_str));
            }

            /**
             * Unescapes a JSON-escaped string.
             * 
             * @return A newly allocated CString with escape sequences resolved.
             */
            CString unescape_json() const {
                return CString(fossil_io_cstring_unescape_json(_str));
            }

            /**
             * Normalizes whitespace in the string by collapsing multiple spaces.
             * 
             * @return A newly allocated CString with normalized whitespace.
             */
            CString normalize_spaces() const {
                return CString(fossil_io_cstring_normalize_spaces(_str));
            }

            /**
             * Removes surrounding single or double quotes from the string.
             * 
             * @return A newly allocated CString without surrounding quotes.
             */
            CString strip_quotes() const {
                return CString(fossil_io_cstring_strip_quotes(_str));
            }

            /**
             * Appends src to this string in-place, resizing as necessary.
             * 
             * @param src The string to append.
             * @return 0 on success, non-zero on allocation failure.
             */
            int append(const std::string &src) {
                return fossil_io_cstring_append(&_str, src.c_str()) == nullptr ? 1 : 0;
            }
            
            /* ---------------- Safe Copy / Dup / Concat ---------------- */

            /**
             * @brief Creates a safe copy of the given string.
             *
             * Allocates a new CString with a copy of the input string, using the
             * secure underlying C function to prevent buffer overflows.
             *
             * @param str The string to copy.
             * @return A new CString containing a safe copy of the input string.
             */
            static CString copy_safe(const std::string &str) {
                return CString(fossil_io_cstring_copy_safe(str.c_str(), str.size()));
            }
            
            /**
             * @brief Duplicates a string safely.
             *
             * Creates a new CString with the same content as the input string,
             * using a secure duplication function that respects the specified length.
             *
             * @param str The string to duplicate.
             * @return A new CString duplicate of the input string.
             */
            static CString dup_safe(const std::string &str) {
                return CString(fossil_io_cstring_dup_safe(str.c_str(), str.size()));
            }
            
            /**
             * @brief Concatenates two strings safely.
             *
             * Produces a new CString containing s1 followed by s2, using the secure
             * concatenation function and respecting the maximum allowed length.
             *
             * @param s1 The first string.
             * @param s2 The second string.
             * @param max_len Maximum length of the resulting string to prevent overflow.
             * @return A new CString with the concatenated content.
             */
            static CString concat_safe(const std::string &s1, const std::string &s2, size_t max_len) {
                return CString(fossil_io_cstring_concat_safe(s1.c_str(), s2.c_str(), max_len));
            }
            
            /* ---------------- Safe Substring / Reverse / Contains ---------------- */
            
            /**
             * @brief Extracts a safe substring from the CString.
             *
             * Uses secure substring logic to prevent reading beyond the maximum length.
             *
             * @param start Starting index of the substring.
             * @param length Length of the substring.
             * @param max_len Maximum number of characters to read from the original string.
             * @return A new CString containing the substring.
             */
            CString substring_safe(size_t start, size_t length, size_t max_len) const {
                return CString(fossil_io_cstring_substring_safe(_str, start, length, max_len));
            }
            
            /**
             * @brief Reverses the CString safely.
             *
             * Returns a new CString containing the reverse of the original string
             * while respecting the maximum length.
             *
             * @param max_len Maximum length to process for safety.
             * @return A new reversed CString.
             */
            CString reverse_safe(size_t max_len) const {
                return CString(fossil_io_cstring_reverse_safe(_str, max_len));
            }
            
            /**
             * @brief Checks if the CString contains a substring safely.
             *
             * Performs a bounds-checked search within the string.
             *
             * @param substr The substring to search for.
             * @param max_len Maximum number of characters to check.
             * @return True if substring is found, false otherwise.
             */
            bool contains_safe(const std::string &substr, size_t max_len) const {
                return fossil_io_cstring_contains_safe(_str, substr.c_str(), max_len) != 0;
            }
            
            /* ---------------- Safe Repeat / Strip / Count ---------------- */
            
            /**
             * @brief Repeats the CString a specified number of times safely.
             *
             * Generates a new CString that repeats the content up to max_len characters.
             *
             * @param count Number of repetitions.
             * @param max_len Maximum total length of the resulting string.
             * @return A new repeated CString.
             */
            CString repeat_safe(size_t count, size_t max_len) const {
                return CString(fossil_io_cstring_repeat_safe(_str, count, max_len));
            }
            
            /**
             * @brief Strips a character from both ends of the CString safely.
             *
             * Produces a new CString with leading and trailing characters removed,
             * respecting max_len for safety.
             *
             * @param ch The character to strip.
             * @param max_len Maximum number of characters to consider.
             * @return A new CString with the character stripped.
             */
            CString strip_safe(char ch, size_t max_len) const {
                return CString(fossil_io_cstring_strip_safe(_str, ch, max_len));
            }
            
            /**
             * @brief Counts the number of occurrences of a substring safely.
             *
             * Uses a secure method that respects maximum string length.
             *
             * @param substr The substring to count.
             * @param max_len Maximum number of characters to process.
             * @return Number of occurrences of substr in the CString.
             */
            size_t count_safe(const std::string &substr, size_t max_len) const {
                return fossil_io_cstring_count_safe(_str, substr.c_str(), max_len);
            }
            
            /* ---------------- Safe Padding ---------------- */
            
            /**
             * @brief Pads the CString on the left safely.
             *
             * Ensures the resulting string does not exceed max_len characters.
             *
             * @param total_length The total desired length after padding.
             * @param pad_char Character to pad with.
             * @param max_len Maximum allowed length to prevent overflow.
             * @return A new left-padded CString.
             */
            CString pad_left_safe(size_t total_length, char pad_char, size_t max_len) const {
                return CString(fossil_io_cstring_pad_left_safe(_str, total_length, pad_char, max_len));
            }
            
            /**
             * @brief Pads the CString on the right safely.
             *
             * Ensures the resulting string does not exceed max_len characters.
             *
             * @param total_length The total desired length after padding.
             * @param pad_char Character to pad with.
             * @param max_len Maximum allowed length to prevent overflow.
             * @return A new right-padded CString.
             */
            CString pad_right_safe(size_t total_length, char pad_char, size_t max_len) const {
                return CString(fossil_io_cstring_pad_right_safe(_str, total_length, pad_char, max_len));
            }
            
            /* ---------------- Safe Prefix / Suffix ---------------- */
            
            /**
             * @brief Checks if the CString starts with a prefix safely.
             *
             * Considers max_len for safe comparison.
             *
             * @param prefix The prefix to check.
             * @param max_len Maximum number of characters to read.
             * @return True if CString starts with prefix, false otherwise.
             */
            bool starts_with_safe(const std::string &prefix, size_t max_len) const {
                return fossil_io_cstring_starts_with_safe(_str, prefix.c_str(), max_len) != 0;
            }
            
            /**
             * @brief Checks if the CString ends with a suffix safely.
             *
             * Considers max_len for safe comparison.
             *
             * @param suffix The suffix to check.
             * @param max_len Maximum number of characters to read.
             * @return True if CString ends with suffix, false otherwise.
             */
            bool ends_with_safe(const std::string &suffix, size_t max_len) const {
                return fossil_io_cstring_ends_with_safe(_str, suffix.c_str(), max_len) != 0;
            }
            
            /* ---------------- Safe Equality Checks ---------------- */
            
            /**
             * @brief Checks equality safely (case-sensitive).
             *
             * Compares CString to another string with max_len bounds.
             *
             * @param other The string to compare.
             * @param max_len Maximum characters to compare.
             * @return True if equal, false otherwise.
             */
            bool equals_safe(const std::string &other, size_t max_len) const {
                return fossil_io_cstring_equals_safe(_str, other.c_str(), max_len) != 0;
            }
            
            /**
             * @brief Checks equality safely (case-insensitive).
             *
             * Compares CString to another string with max_len bounds, ignoring case.
             *
             * @param other The string to compare.
             * @param max_len Maximum characters to compare.
             * @return True if equal ignoring case, false otherwise.
             */
            bool iequals_safe(const std::string &other, size_t max_len) const {
                return fossil_io_cstring_iequals_safe(_str, other.c_str(), max_len) != 0;
            }
            
            /**
             * @brief Checks if CString contains a substring safely (case-insensitive).
             *
             * @param substr Substring to check.
             * @param max_len Maximum characters to examine.
             * @return True if found, false otherwise.
             */
            bool icontains_safe(const std::string &substr, size_t max_len) const {
                return fossil_io_cstring_icontains_safe(_str, substr.c_str(), max_len) != 0;
            }
            
            /* ---------------- Safe Quotes / Whitespace / Index ---------------- */
            
            /**
             * @brief Removes surrounding quotes safely.
             *
             * Considers max_len and removes only leading/trailing single or double quotes.
             *
             * @param max_len Maximum characters to process.
             * @return A new CString with quotes removed.
             */
            CString strip_quotes_safe(size_t max_len) const {
                return CString(fossil_io_cstring_strip_quotes_safe(_str, max_len));
            }
            
            /**
             * @brief Normalizes spaces safely.
             *
             * Collapses multiple consecutive spaces into one, respecting max_len.
             *
             * @param max_len Maximum characters to process.
             * @return A new CString with normalized spaces.
             */
            CString normalize_spaces_safe(size_t max_len) const {
                return CString(fossil_io_cstring_normalize_spaces_safe(_str, max_len));
            }
            
            /**
             * @brief Finds the index of a substring safely.
             *
             * @param substr Substring to find.
             * @param max_len Maximum characters to search.
             * @return Index of substring or -1 if not found.
             */
            int index_of_safe(const std::string &substr, size_t max_len) const {
                return fossil_io_cstring_index_of_safe(_str, substr.c_str(), max_len);
            }

            /**
             * Returns the underlying cstring.
             * 
             * @return The underlying cstring.
             */
            cstring str() const {
                return _str;
            }
        };

        class CStringStream {
        private:
            fossil_io_cstring_stream* _stream;
        
        public:
            /**
             * Constructor to create a new cstring stream with the specified initial size.
             * 
             * @param initial_size The initial size of the cstring stream.
             */
            CStringStream(size_t initial_size) {
                _stream = fossil_io_cstring_stream_create(initial_size);
            }
        
            /**
             * Destructor to free the memory allocated for the cstring stream.
             */
            ~CStringStream() {
                fossil_io_cstring_stream_free(_stream);
            }
        
            /**
             * Writes a cstring to the cstring stream.
             * 
             * @param str The cstring to be written.
             */
            void write(const std::string &str) {
                fossil_io_cstring_stream_write(_stream, str.c_str());
            }
        
            /**
             * Writes a cstring to the stream safely, respecting a maximum length.
             * 
             * @param str The cstring to append.
             * @param max_len Maximum number of characters to append.
             */
            void write_safe(const std::string &str, size_t max_len) {
                fossil_io_cstring_stream_write_safe(_stream, str.c_str(), max_len);
            }
        
            /**
             * Writes a formatted string to the stream (like printf).
             * 
             * @param format Format string.
             * @param ... Format arguments.
             */
            void write_format(const char *format, ...) {
                va_list args;
                va_start(args, format);
                fossil_io_cstring_stream_write_format(_stream, format, args);
                va_end(args);
            }
        
            /**
             * Inserts a cstring at a specified position in the stream.
             * 
             * @param str String to insert.
             * @param pos Position to insert at (0 = beginning).
             */
            void insert(const std::string &str, size_t pos) {
                fossil_io_cstring_stream_insert(_stream, str.c_str(), pos);
            }
        
            /**
             * Truncates the stream to the specified length.
             * 
             * @param new_length New desired length of the stream.
             */
            void truncate(size_t new_length) {
                fossil_io_cstring_stream_truncate(_stream, new_length);
            }
        
            /**
             * Clears the stream content without freeing the buffer.
             */
            void clear() {
                fossil_io_cstring_stream_clear(_stream);
            }
        
            /**
             * Ensures the stream has at least the specified capacity.
             * 
             * @param min_capacity Minimum capacity required.
             * @return True if successful, false if allocation failed.
             */
            bool reserve(size_t min_capacity) {
                return fossil_io_cstring_stream_reserve(_stream, min_capacity) == 0;
            }
        
            /**
             * Returns the current length of the stream.
             * 
             * @return Length of data in the stream.
             */
            size_t length() const {
                return fossil_io_cstring_stream_length(_stream);
            }
        
            /**
             * Returns the remaining capacity of the stream before resizing is required.
             * 
             * @return Number of bytes available in the stream.
             */
            size_t capacity_remaining() const {
                return fossil_io_cstring_stream_capacity_remaining(_stream);
            }
        
            /**
             * Reads the contents of the cstring stream.
             * 
             * @return The contents of the cstring stream as a std::string.
             */
            std::string read() const {
                ccstring buf = fossil_io_cstring_stream_read(_stream);
                return buf ? std::string(buf) : std::string();
            }
        
            /**
             * Appends a cstring using the << operator for chaining.
             * 
             * @param str The string to append.
             * @return Reference to this stream for chaining.
             */
            CStringStream& operator<<(const std::string &str) {
                write(str);
                return *this;
            }
        
            /**
             * Appends a number using the << operator for chaining.
             * Converts the number to string before writing.
             * 
             * @param num The integer to append.
             * @return Reference to this stream for chaining.
             */
            CStringStream& operator<<(int num) {
                char buf[32];
                snprintf(buf, sizeof(buf), "%d", num);
                write(buf);
                return *this;
            }
        };

    }

}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
