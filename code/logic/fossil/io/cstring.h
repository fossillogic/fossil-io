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
 * @brief Converts an English number string into an integer.
 * 
 * Example: "twenty-three" -> 23
 * 
 * @param str Input string containing the number in English.
 * @param out Pointer to integer where the parsed number will be stored.
 * @return 0 on success, non-zero on error (invalid input).
 */
int fossil_io_cstring_number_from_words(const char *str, int *out);

/**
 * @brief Converts an integer number into its English representation.
 * 
 * Example: 23 -> "twenty-three"
 * 
 * @param num The integer to convert.
 * @param buffer The output buffer to store the English string.
 * @param size The size of the output buffer.
 * @return 0 on success, non-zero if the buffer is too small.
 */
int fossil_io_cstring_number_to_words(int num, char *buffer, size_t size);

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
             * Converts an English number string into an integer.
             * 
             * Example: "twenty-three" -> 23
             * 
             * @param str Input string containing the number in English.
             * @return The parsed integer value, or throws std::invalid_argument on error.
             */
            static int number_from_words(const std::string &str) {
                int value = 0;
                int result = fossil_io_cstring_number_from_words(str.c_str(), &value);
                if (result != 0) throw std::invalid_argument("Invalid English number string");
                return value;
            }

            /**
             * Converts an integer number into its English representation.
             * 
             * Example: 23 -> "twenty-three"
             * 
             * @param num The integer to convert.
             * @return The English representation as a std::string.
             */
            static std::string number_to_words(int num) {
                char buffer[128];
                int result = fossil_io_cstring_number_to_words(num, buffer, sizeof(buffer));
                if (result != 0) throw std::runtime_error("Buffer too small for English number string");
                return std::string(buffer);
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
             * Reads the contents of the cstring stream.
             * 
             * @return The contents of the cstring stream as a cstring.
             */
            std::string read() const {
                return fossil_io_cstring_stream_read(_stream);
            }

        };

    }

}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
