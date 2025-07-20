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
#include "fossil/io/soap.h"
#include "fossil/io/cstring.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_CUSTOM_FILTERS 64

/** Lookup table for rot-brain words and their suggested replacements */
static const struct {
    const char *bad;
    const char *suggested;
} FOSSIL_SOAP_SUGGESTIONS[] = {
    {"rizz", "charisma"},
    {"skibidi", "dance"},
    {"yeet", "throw"},
    {"sus", "suspicious"},
    {"vibe", "atmosphere"},
    {"lit", "exciting"},
    {"no cap", "honestly"},
    {"bet", "okay"},
    {"fam", "family"},
    {"bruh", "brother"},
    {"flex", "show off"},
    {"ghost", "ignore"},
    {"goat", "legend"},
    {"gucci", "good"},
    {"hype", "exciting"},
    {"janky", "low-quality"},
    {"lowkey", "somewhat"},
    {"mood", "feeling"},
    {"salty", "bitter"},
    {"shade", "insult"},
    {"slay", "impress"},
    {"snatched", "stylish"},
    {"stan", "superfan"},
    {"tea", "gossip"},
    {"thirsty", "desperate"},
    {"woke", "aware"},
    {"yolo", "live once"},
    {"zaddy", "attractive man"},
    {"drip", "fashion"},
    {"fire", "amazing"},
    {"lol", "funny"},
    {"omg", "surprising"},
    {"brb", "be right back"},
    {"idk", "I don't know"},
    {"imo", "in my opinion"},
    {"lmao", "laughing"},
    {"nvm", "never mind"},
    {"tbh", "to be honest"},
    {"tldr", "too long"},
    {"ttyl", "talk to you later"},
    {"wyd", "what are you doing"},
    {"wtf", "what the heck"},
    {"yolo", "you only live once"},
    {"rot-brain", "stupid"},
    {"rot brain", "stupid"},
    {"rotbrain", "stupid"},
    {NULL, NULL} // Sentinel to mark the end
};

/** Grammar suggestions for common mistakes */
static const struct {
    const char *incorrect;
    const char *correct;
} FOSSIL_SOAP_GRAMMAR_SUGGESTIONS[] = {
    {"gonna", "going to"},
    {"ain't", "isn't"},
    {"should of", "should have"},
    {"could of", "could have"},
    {"not never", "never"},
    {"free gift", "gift"},
    {"very unique", "unique"},
    {NULL, NULL} // Sentinel to mark the end
};

/** Lookup table for sarcastic phrases */
static const char *SARCASTIC_PHRASES[] = {
    "Oh, great",
    "Yeah, right",
    "Nice job",
    "Well done",
    "Good luck with that",
    "Sure, why not",
    "Fantastic",
    "Brilliant",
    "Wonderful",
    "Perfect",
    NULL // Sentinel to mark the end
};

/** Lookup table for formal phrases */
static const char *FORMAL_PHRASES[] = {
    "Dear Sir or Madam",
    "To whom it may concern",
    "Yours sincerely",
    "Yours faithfully",
    "Best regards",
    "Respectfully",
    "I would like to",
    "I am writing to",
    "Please find attached",
    "Thank you for your consideration",
    NULL // Sentinel to mark the end
};

static char custom_storage[MAX_CUSTOM_FILTERS][64];
static const char *custom_filters[MAX_CUSTOM_FILTERS] = {0};

/** Lookup table for words that need to be skipped due to misdetection */
static const char *SKIP_WORDS[] = {
    "limit",
    "size",
    NULL // Sentinel to mark the end
};

static void hash_to_hex(const uint8_t *hash, size_t len, char *out_hex) {
    for (size_t i = 0; i < len; ++i) {
        sprintf(out_hex + i * 2, "%02x", hash[i]);
    }
    out_hex[len * 2] = '\0';
}

/**
 * @brief Convert leetspeak to normal letters.
 */
static void fossil_io_soap_normalize_leetspeak(char *word) {
    for (size_t i = 0; word[i] != '\0'; i++) {
        switch (word[i]) {
            case '0': word[i] = 'o'; break;
            case '1': word[i] = 'i'; break;
            case '3': word[i] = 'e'; break;
            case '4': word[i] = 'a'; break;
            case '5': word[i] = 's'; break;
            case '7': word[i] = 't'; break;
            case '$': word[i] = 's'; break;
        }
    }
}

/**
 * @brief Fuzzy matching using Levenshtein distance.
 */
static int fuzzy_match(const char *str1, const char *str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t dist[len1 + 1][len2 + 1];

    for (size_t i = 0; i <= len1; i++) dist[i][0] = i;
    for (size_t j = 0; j <= len2; j++) dist[0][j] = j;

    for (size_t i = 1; i <= len1; i++) {
        for (size_t j = 1; j <= len2; j++) {
            int cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
            dist[i][j] = fmin(fmin(dist[i - 1][j] + 1, dist[i][j - 1] + 1), dist[i - 1][j - 1] + cost);
        }
    }
    return dist[len1][len2];
}

/**
 * @brief Check if a word should be skipped.
 */
static int should_skip_word(const char *word) {
    for (size_t i = 0; SKIP_WORDS[i] != NULL; i++) {
        if (strcmp(word, SKIP_WORDS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Case-insensitive string comparison.
 */
static int custom_strcasecmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
        }
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

/**
 * @brief Case-insensitive substring search.
 */
static const char *custom_strcasestr(const char *haystack, const char *needle) {
    if (!*needle) return haystack;

    for (; *haystack; haystack++) {
        if (tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
            const char *h = haystack, *n = needle;
            while (*h && *n && tolower((unsigned char)*h) == tolower((unsigned char)*n)) {
                h++;
                n++;
            }
            if (!*n) return haystack;
        }
    }
    return NULL;
}

/**
 * @brief Look up a suggested alternative for a given word, checking both custom filters and predefined suggestions.
 */
static const char *fossil_io_soap_get_suggestion(const char *word) {
    if (should_skip_word(word)) {
        return NULL;
    }

    // Check in custom filters first
    for (size_t i = 0; i < MAX_CUSTOM_FILTERS && custom_filters[i] != NULL; i++) {
        if (custom_strcasecmp(word, custom_filters[i]) == 0) {
            return custom_filters[i];  // Use the custom filter word itself as suggestion
        }
        if (fuzzy_match(word, custom_filters[i]) <= 2) {
            return custom_filters[i];  // Return fuzzy match result
        }
    }

    // Check in predefined suggestions
    for (size_t i = 0; FOSSIL_SOAP_SUGGESTIONS[i].bad != NULL; i++) {
        if (custom_strcasecmp(word, FOSSIL_SOAP_SUGGESTIONS[i].bad) == 0) {
            return FOSSIL_SOAP_SUGGESTIONS[i].suggested;
        }
    }

    // Check in grammar suggestions
    for (size_t i = 0; FOSSIL_SOAP_GRAMMAR_SUGGESTIONS[i].incorrect != NULL; i++) {
        if (custom_strcasecmp(word, FOSSIL_SOAP_GRAMMAR_SUGGESTIONS[i].incorrect) == 0) {
            return FOSSIL_SOAP_GRAMMAR_SUGGESTIONS[i].correct;
        }
    }

    return NULL;
}

/**
 * @brief Sanitize input text by removing or replacing "rot-brain" and meme-based language.
 * @param censor_char Character to use for censored words (e.g., "*" or "#").
 */
char *fossil_io_soap_sanitize(const char *text) {
    if (!text) return NULL;

    size_t len = strlen(text);
    char *output = (char *)malloc(len * 2 + 1); // Allocate more space to handle replacements
    if (!output) return NULL;

    size_t out_idx = 0;
    char word[64];
    size_t word_idx = 0;

    for (size_t i = 0; text[i] != '\0'; i++) {
        if (isalnum((unsigned char)text[i]) || text[i] == '\'' || text[i] == '-') {
            word[word_idx++] = text[i];
            if (word_idx >= sizeof(word) - 1) word_idx = sizeof(word) - 2;
        } else {
            word[word_idx] = '\0';
            fossil_io_soap_normalize_leetspeak(word);
            const char *suggested = fossil_io_soap_get_suggestion(word);
            if (word_idx > 0 && suggested && !should_skip_word(word)) {
                for (size_t j = 0; j < strlen(suggested); j++) {
                    output[out_idx++] = suggested[j];
                }
            } else {
                for (size_t j = 0; j < word_idx; j++) {
                    output[out_idx++] = word[j];
                }
            }
            output[out_idx++] = text[i];
            word_idx = 0;
        }
    }
    word[word_idx] = '\0';
    fossil_io_soap_normalize_leetspeak(word);
    const char *suggested = fossil_io_soap_get_suggestion(word);
    if (word_idx > 0 && suggested && !should_skip_word(word)) {
        for (size_t j = 0; j < strlen(suggested); j++) {
            output[out_idx++] = suggested[j];
        }
    } else {
        for (size_t j = 0; j < word_idx; j++) {
            output[out_idx++] = word[j];
        }
    }
    output[out_idx] = '\0';
    return output;
}

char *fossil_io_soap_suggest(const char *text) {
    if (!text) return NULL;

    size_t len = strlen(text);
    char *output = (char *)malloc(len * 2 + 64); // Allocate more space to handle replacements
    if (!output) return NULL;

    size_t out_idx = 0;
    char word[64];
    size_t word_idx = 0;

    for (size_t i = 0; text[i] != '\0'; i++) {
        if (isalnum((unsigned char)text[i]) || text[i] == '\'' || text[i] == '-') {
            word[word_idx++] = text[i];
            if (word_idx >= sizeof(word) - 1) word_idx = sizeof(word) - 2;
        } else {
            word[word_idx] = '\0';
            fossil_io_soap_normalize_leetspeak(word);
            const char *suggested = fossil_io_soap_get_suggestion(word);
            if (word_idx > 0 && suggested && !should_skip_word(word)) {
                strncpy(&output[out_idx], suggested, len * 2 + 64 - out_idx);
                out_idx += strlen(suggested);
            } else {
                strncpy(&output[out_idx], word, len * 2 + 64 - out_idx);
                out_idx += word_idx;
            }
            output[out_idx++] = text[i];
            word_idx = 0;
        }
    }
    word[word_idx] = '\0';
    fossil_io_soap_normalize_leetspeak(word);
    const char *suggested = fossil_io_soap_get_suggestion(word);
    if (word_idx > 0 && suggested && !should_skip_word(word)) {
        strncpy(&output[out_idx], suggested, len * 2 + 64 - out_idx);
        out_idx += strlen(suggested);
    } else {
        strncpy(&output[out_idx], word, len * 2 + 64 - out_idx);
        out_idx += word_idx;
    }
    output[out_idx] = '\0';
    return output;
}

/**
 * @brief Add a custom word or phrase to the filter.
 */
int fossil_io_soap_add_custom_filter(const char *phrase) {
    for (size_t i = 0; i < MAX_CUSTOM_FILTERS; i++) {
        if (custom_filters[i] == NULL) {
            size_t j = 0;
            while (phrase[j] != '\0' && j < sizeof(custom_storage[i]) - 1) {
                custom_storage[i][j] = tolower((unsigned char)phrase[j]);
                j++;
            }
            custom_storage[i][j] = '\0';
            custom_filters[i] = custom_storage[i];
            return 0;
        }
    }
    return -1;
}

/**
 * @brief Clear all custom filters.
 */
void fossil_io_soap_clear_custom_filters(void) {
    memset(custom_filters, 0, sizeof(custom_filters));
}

const char *fossil_io_soap_detect_tone(const char *text) {
    for (size_t i = 0; SARCASTIC_PHRASES[i] != NULL; i++) {
        if (custom_strcasestr(text, SARCASTIC_PHRASES[i])) {
            return "sarcastic";
        }
    }

    for (size_t i = 0; FORMAL_PHRASES[i] != NULL; i++) {
        if (custom_strcasestr(text, FORMAL_PHRASES[i])) {
            return "formal";
        }
    }

    return "casual";
}

int fossil_io_soap_check_grammar(const char *text) {
    if (!text) return -1;
    for (size_t i = 0; FOSSIL_SOAP_GRAMMAR_SUGGESTIONS[i].incorrect; i++) {
        if (custom_strcasestr(text, FOSSIL_SOAP_GRAMMAR_SUGGESTIONS[i].incorrect)) {
            return 1; // Grammar issue found
        }
    }
    return 0;
}

char *fossil_io_soap_normalize(const char *text) {
    if (!text) return NULL;

    char *normalized = strdup(text); // Create modifiable copy
    if (!normalized) return NULL;

    for (size_t i = 0; FOSSIL_SOAP_SUGGESTIONS[i].bad; i++) {
        const char *bad = FOSSIL_SOAP_SUGGESTIONS[i].bad;
        const char *fix = FOSSIL_SOAP_SUGGESTIONS[i].suggested;
        const char *ptr;
        while ((ptr = custom_strcasestr(normalized, bad))) {
            size_t prefix_len = ptr - normalized;
            size_t new_len = strlen(normalized) - strlen(bad) + strlen(fix) + 1;
            char *temp = malloc(new_len);
            if (!temp) break;
            snprintf(temp, new_len, "%.*s%s%s",
                     (int)prefix_len, normalized, fix, ptr + strlen(bad));
            free(normalized);
            normalized = temp;
        }
    }

    return normalized;
}


char *fossil_io_soap_normalize_slang(const char *text) {
    if (!text) return NULL;

    char *result = fossil_io_cstring_dup(text);
    if (!result) return NULL;

    for (size_t i = 0; result[i]; i++) {
        result[i] = tolower(result[i]);
    }

    for (size_t i = 0; FOSSIL_SOAP_SUGGESTIONS[i].bad != NULL; i++) {
        const char *bad = FOSSIL_SOAP_SUGGESTIONS[i].bad;
        const char *sugg = FOSSIL_SOAP_SUGGESTIONS[i].suggested;

        char *found = NULL;
        while ((found = custom_strcasestr(result, bad)) != NULL) {
            size_t offset = found - result;
            size_t newlen = strlen(result) - strlen(bad) + strlen(sugg) + 1;

            char *temp = malloc(newlen);
            if (!temp) {
                free(result);
                return NULL;
            }

            strncpy(temp, result, offset);
            temp[offset] = '\0';
            strcat(temp, sugg);
            strcat(temp, found + strlen(bad));

            free(result);
            result = temp;
        }
    }

    return result;
}

int fossil_io_soap_detect_clickbait(const char *text) {
    if (!text) return 0;

    static const char *CLICKBAIT_PATTERNS[] = {
        "you won't believe",
        "shocking",
        "what happened next",
        "top [0-9]",
        "things you didn't know",
        "one weird trick",
        "will blow your mind",
        "can't handle this",
        "before you die",
        NULL
    };

    for (int i = 0; CLICKBAIT_PATTERNS[i] != NULL; i++) {
        if (custom_strcasestr(text, CLICKBAIT_PATTERNS[i])) {
            return 1;
        }
    }

    return 0;
}

int fossil_io_soap_detect_exaggeration(const char *text) {
    if (!text) return 0;

    static const char *EXAGGERATED_WORDS[] = {
        "literally", "always", "never", "every", "everyone", "nobody",
        "forever", "insane", "unbelievable", "outrageous", "epic", "mind-blowing",
        NULL
    };

    for (int i = 0; EXAGGERATED_WORDS[i] != NULL; i++) {
        if (custom_strcasestr(text, EXAGGERATED_WORDS[i])) {
            return 1;
        }
    }

    return 0;
}

char *fossil_io_soap_filter_offensive(const char *text) {
    if (!text) return NULL;

    static const struct {
        const char *offensive;
        const char *replacement;
    } OFFENSIVE_WORDS[] = {
        {"dumb", "uninformed"},
        {"stupid", "ill-advised"},
        {"idiot", "misguided"},
        {"moron", "uninformed"},
        {"sucks", "is not ideal"},
        {NULL, NULL}
    };

    char *result = fossil_io_cstring_dup(text);
    if (!result) return NULL;

    for (size_t i = 0; OFFENSIVE_WORDS[i].offensive != NULL; i++) {
        const char *bad = OFFENSIVE_WORDS[i].offensive;
        const char *good = OFFENSIVE_WORDS[i].replacement;

        char *found = NULL;
        while ((found = custom_strcasestr(result, bad)) != NULL) {
            size_t offset = found - result;
            size_t newlen = strlen(result) - strlen(bad) + strlen(good) + 1;

            char *temp = malloc(newlen);
            if (!temp) {
                free(result);
                return NULL;
            }

            strncpy(temp, result, offset);
            temp[offset] = '\0';
            strcat(temp, good);
            strcat(temp, found + strlen(bad));

            free(result);
            result = temp;
        }
    }

    return result;
}
