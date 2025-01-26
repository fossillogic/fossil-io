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

#define _GNU_SOURCE // Define _GNU_SOURCE to enable strcasestr and strcasecmp
#include <strings.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// List of offensive words and phrases (super hard to mainting thisw list as GitHub Copilot doesnt wanna help with this part of the SOAP API)
static const char *FOSSIL_SOAP_OFFENSIVE[] = {
    "curse1",
    "curse2",
    "racist_phrase1",
    "racist_phrase2", // for demo and unit testing we keep these four keywords

    // English offensive words and phrases
    "2g1c", "2 girls 1 cup", "acrotomophilia", "alabama hot pocket", "alaskan pipeline", "anal", "anilingus", "anus", "apeshit", "arsehole", "ass", "asshole", "assmunch", "auto erotic", "autoerotic", "babeland",
    "baby batter", "baby juice", "ball gag", "ball gravy", "ball kicking", "ball licking", "ball sack", "ball sucking", "bangbros", "bareback", "barely legal", "barenaked", "bastard", "bastardo", "bastinado", "bbw",
    "bdsm", "beaner", "beaners", "beaver cleaver", "beaver lips", "bestiality", "big black", "big breasts", "big knockers", "big tits", "bimbos", "birdlock", "bitch", "bitches", "black cock", "blonde action", "blonde on blonde action",
    "blowjob", "blow job", "blow your load", "blue waffle", "blumpkin", "bollocks", "bondage", "boner", "boob", "boobs", "booty call", "brown showers", "brunette action", "bukkake", "bulldyke", "bullet vibe", "bullshit",
    "bung hole", "bunghole", "busty", "butt", "buttcheeks", "butthole", "camel toe", "camgirl", "camslut", "camwhore", "carpet muncher", "carpetmuncher", "chocolate rosebuds", "circlejerk", "cleveland steamer", "clit",
    "clitoris", "clover clamps", "clusterfuck", "cock", "cocks", "coprolagnia", "coprophilia", "cornhole", "coon", "coons", "creampie", "cum", "cumming", "cunnilingus", "cunt", "darkie", "date rape", "daterape",
    "deep throat", "deepthroat", "dendrophilia", "dick", "dildo", "dingleberry", "dingleberries", "dirty pillows", "dirty sanchez", "doggie style", "doggiestyle", "doggy style", "doggystyle", "dog style", "dolcett",
    "domination", "dominatrix", "dommes", "donkey punch", "double dong", "double penetration", "dp action", "dry hump", "dvda", "eat my ass", "ecchi", "ejaculation", "erotic", "erotism", "escort", "eunuch", "faggot",
    "fecal", "felch", "fellatio", "feltch", "female squirting", "femdom", "figging", "fingerbang", "fingering", "fisting", "foot fetish", "footjob", "frotting", "fuck", "fuck buttons", "fuckin", "fucking", "fucktards",
    "fudge packer", "fudgepacker", "futanari", "gang bang", "gay sex", "genitals", "giant cock", "girl on", "girl on top", "girls gone wild", "goatcx", "goatse", "god damn", "gokkun", "golden shower", "goodpoop",
    "goo girl", "goregasm", "grope", "group sex", "g-spot", "guro", "hand job", "handjob", "hard core", "hardcore", "hentai", "homoerotic", "honkey", "hooker", "hot carl", "hot chick", "how to kill", "how to murder",
    "huge fat", "humping", "incest", "intercourse", "jack off", "jail bait", "jailbait", "jelly donut", "jerk off", "jigaboo", "jiggaboo", "jiggerboo", "jizz", "juggs", "kike", "kinbaku", "kinkster", "kinky", "knobbing",
    "leather restraint", "leather straight jacket", "lemon party", "lolita", "lovemaking", "make me come", "male squirting", "masturbate", "menage a trois", "milf", "missionary position", "motherfucker", "mound of venus",
    "mr hands", "muff diver", "muffdiving", "nambla", "nawashi", "negro", "neonazi", "nigga", "nigger", "nig nog", "nimphomania", "nipple", "nipples", "nsfw images", "nude", "nudity", "nympho", "nymphomania", "octopussy",
    "omorashi", "one cup two girls", "one guy one jar", "orgasm", "orgy", "paedophile", "paki", "panties", "panty", "pedobear", "pedophile", "pegging", "penis", "phone sex", "piece of shit", "pissing", "piss pig", "pisspig",
    "playboy", "pleasure chest", "pole smoker", "ponyplay", "poof", "poon", "poontang", "punany", "poop chute", "poopchute", "porn", "porno", "pornography", "prince albert piercing", "pthc", "pubes", "pussy", "queaf", "queef",
    "quim", "raghead", "raging boner", "rape", "raping", "rapist", "rectum", "reverse cowgirl", "rimjob", "rimming", "rosy palm", "rosy palm and her 5 sisters", "rusty trombone", "sadism", "santorum", "scat", "schlong",
    "scissoring", "semen", "sex", "sexo", "sexy", "shaved beaver", "shaved pussy", "shemale", "shibari", "shit", "shitblimp", "shitty", "shota", "shrimping", "skeet", "slanteye", "slut", "s&m", "smut", "snatch", "snowballing",
    "sodomize", "sodomy", "spic", "splooge", "splooge moose", "spooge", "spread legs", "spunk", "strap on", "strapon", "strappado", "strip club", "style doggy", "suck", "sucks", "suicide girls", "sultry women", "swastika",
    "swinger", "tainted love", "taste my", "tea bagging", "threesome", "throating", "tied up", "tight white", "tit", "tits", "titties", "titty", "tongue in a", "topless", "tosser", "towelhead", "tranny", "tribadism",
    "tub girl", "tubgirl", "tushy", "twat", "twink", "twinkie", "two girls one cup", "undressing", "upskirt", "urethra play", "urophilia", "vagina", "venus mound", "vibrator", "violet wand", "vorarephilia", "voyeur", "vulva",
    "wank", "wetback", "wet dream", "white power", "wrapping men", "wrinkled starfish", "xx", "xxx", "yaoi", "yellow showers", "yiffy", "zoophilia"

    // Support for other languages can be added via PR to this repository
};

// garbage words and phrases
// (slightly easier to maintain since it's just slang from social media spoken from people who need to touch grass)
const char *FOSSIL_SOAP_ROTBRAIN[] = {
    "rizz", "skibidi", "yeet", "sus", "vibe", "lit", "no cap", "bet", "fam", "bruh",
    "flex", "ghost", "goat", "gucci", "hype", "janky", "lowkey", "mood", "salty", "shade",
    "slay", "snatched", "stan", "tea", "thirsty", "woke", "yolo", "zaddy", "drip", "fire"

    // Support for other terms can be added via PR to this repository
};

// Fuzzy matching of synonyms (basic implementation)
const char *FOSSIL_SOAP_SYNONYMS[] = {
    "rizz", "charm", "flirt", "tease",
    "sus", "suspicious", "shady"
};


static inline char* _custom_fossil_strdup(const char* str) {
    if (!str) return NULL; // Handle NULL pointer gracefully

    size_t len = 0;
    while (str[len] != '\0') len++; // Calculate the length of the string

    char* dup = (char*)malloc((len + 1) * sizeof(char)); // Allocate memory for the duplicate string
    if (!dup) return NULL; // Check if malloc failed

    for (size_t i = 0; i < len; i++) {
        dup[i] = str[i]; // Copy each character from the original string to the duplicate
    }
    dup[len] = '\0'; // Add null terminator to the end of the duplicate string

    return dup;
}

// Utility: Case-insensitive substring search
static char *custom_strcasestr(const char *haystack, const char *needle) {
    if (!haystack || !needle) return NULL;

    size_t needle_len = strlen(needle);
    if (needle_len == 0) return (char *)haystack;

    for (; *haystack; ++haystack) {
        if (strncasecmp(haystack, needle, needle_len) == 0) {
            return (char *)haystack;
        }
    }
    return NULL;
}

// Utility: Replace a substring in a string (case-insensitive)
static void replace_substring_case_insensitive(char *str, const char *old_substr, const char *new_substr) {
    char *position = custom_strcasestr(str, old_substr);
    if (position != NULL) {
        size_t old_len = strlen(old_substr);
        size_t new_len = strlen(new_substr);
        size_t tail_len = strlen(position + old_len);

        // Adjust memory for new length
        if (new_len > old_len) {
            memmove(position + new_len, position + old_len, tail_len + 1);
        } else {
            memmove(position + new_len, position + old_len, tail_len + 1);
            memcpy(position, new_substr, new_len);
        }
    }
}

// Tokenization: Split input into words
void fossil_soap_tokenize_and_process(char *input) {
    char *copy = _custom_fossil_strdup(input);
    if (copy == NULL) return;

    char *token = strtok(copy, " ");
    while (token != NULL) {
        if (fossil_soap_is_offensive(token) == EXIT_FAILURE) {
            printf("Offensive word detected: %s\n", token);
        }
        if (fossil_soap_is_rotbrain(token) == EXIT_FAILURE) {
            printf("Meme speak detected: %s\n", token);
        }
        token = strtok(NULL, " ");
    }
    free(copy);
}

// Synonym detection and replacement function (case-insensitive)
void replace_synonym_with_placeholder(char *input, const char *old_word, const char *new_word) {
    while (custom_strcasestr(input, old_word) != NULL) {
        replace_substring_case_insensitive(input, old_word, new_word);
    }
}

// Run fuzzer and test the enhanced sanitization
void fossil_soap_fuzzer(const char *input) {
    const size_t fuzz_count = 10;
    printf("\n=== Fuzzer Test Cases ===\n");
    for (size_t i = 0; i < fuzz_count; ++i) {
        char fuzzed_input[512];
        snprintf(fuzzed_input, sizeof(fuzzed_input), "%s %c%s", 
                 input, 
                 (rand() % 26) + 'A',
                 "!!");
        printf("Fuzzed Input %zu: %s\n", i + 1, fuzzed_input);
        char sanitized_output[512];
        strncpy(sanitized_output, fuzzed_input, sizeof(sanitized_output));
        fossil_soap_sanitize_nlp(sanitized_output);
        printf("Sanitized Output %zu: %s\n", i + 1, sanitized_output);
    }
}

// Fuzzer Helper: Generate test cases for sanitization
void fossil_soap_fuzzer(const char *input) {
    const size_t fuzz_count = 10;  // Number of fuzzed test cases to generate
    printf("\n=== Fuzzer Test Cases ===\n");
    for (size_t i = 0; i < fuzz_count; ++i) {
        char fuzzed_input[512];
        snprintf(fuzzed_input, sizeof(fuzzed_input), "%s %c%s", 
                 input, 
                 (rand() % 26) + 'A',  // Random uppercase letter
                 "!!");
        printf("Fuzzed Input %zu: %s\n", i + 1, fuzzed_input);
        char sanitized_output[512];
        strncpy(sanitized_output, fuzzed_input, sizeof(sanitized_output));
        fossil_soap_sanitize(sanitized_output);
        printf("Sanitized Output %zu: %s\n", i + 1, sanitized_output);
    }
}

// Utility: Trim extra spaces and ensure proper sentence capitalization
void fossil_soap_correct_grammar(char *input) {
    if (input == NULL || *input == '\0') return;

    char *dst = input;
    int capitalize = 1;

    for (char *src = input; *src; ++src) {
        if (isspace(*src)) {
            if (dst == input || isspace(*(dst - 1))) continue;  // Skip duplicate spaces
            *dst++ = ' ';  // Single space
        } else {
            *dst++ = capitalize ? toupper(*src) : tolower(*src);
            capitalize = ispunct(*src) || isspace(*src);
        }
    }
    if (dst > input && isspace(*(dst - 1))) dst--;  // Remove trailing space
    *dst = '\0';
}

// Main sanitization function
void fossil_soap_sanitize(char *input) {
    if (input == NULL || *input == '\0') return;

    // Handle synonyms and offensive words
    for (size_t i = 0; i < sizeof(FOSSIL_SOAP_OFFENSIVE) / sizeof(FOSSIL_SOAP_OFFENSIVE[0]); ++i) {
        while (custom_strcasestr(input, FOSSIL_SOAP_OFFENSIVE[i]) != NULL) {
            replace_substring_case_insensitive(input, FOSSIL_SOAP_OFFENSIVE[i], "***");
        }
    }

    // Handle meme speak
    for (size_t i = 0; i < sizeof(FOSSIL_SOAP_ROTBRAIN) / sizeof(FOSSIL_SOAP_ROTBRAIN[0]); ++i) {
        while (custom_strcasestr(input, FOSSIL_SOAP_ROTBRAIN[i]) != NULL) {
            replace_substring_case_insensitive(input, FOSSIL_SOAP_ROTBRAIN[i], "[ROT]");
        }
    }

    // Handle synonyms
    for (size_t i = 0; i < sizeof(FOSSIL_SOAP_SYNONYMS) / sizeof(FOSSIL_SOAP_SYNONYMS[0]); ++i) {
        while (custom_strcasestr(input, FOSSIL_SOAP_SYNONYMS[i]) != NULL) {
            replace_substring_case_insensitive(input, FOSSIL_SOAP_SYNONYMS[i], "[SYNONYM]");
        }
    }

    // Optional: Correct grammar after sanitizing
    fossil_soap_correct_grammar(input);
}

// Check if a word is offensive
int32_t fossil_soap_is_offensive(const char *word) {
    if (word == NULL || *word == '\0') return EXIT_SUCCESS;

    for (size_t i = 0; i < sizeof(FOSSIL_SOAP_OFFENSIVE) / sizeof(FOSSIL_SOAP_OFFENSIVE[0]); ++i) {
        if (strcasecmp(word, FOSSIL_SOAP_OFFENSIVE[i]) == 0) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

// Check if a word is meme speak
int32_t fossil_soap_is_rotbrain(const char *word) {
    if (word == NULL || *word == '\0') return EXIT_SUCCESS;

    for (size_t i = 0; i < sizeof(FOSSIL_SOAP_ROTBRAIN) / sizeof(FOSSIL_SOAP_ROTBRAIN[0]); ++i) {
        if (strcasecmp(word, FOSSIL_SOAP_ROTBRAIN[i]) == 0) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

// Count offensive words in a string
int32_t fossil_soap_count_offensive(const char *input) {
    if (input == NULL || *input == '\0') return 0;

    int count = 0;
    char *copy = _custom_fossil_strdup(input);
    if (copy == NULL) return EXIT_SUCCESS;

    char *token = strtok(copy, " ");
    while (token != NULL) {
        if (fossil_soap_is_offensive(token) == EXIT_FAILURE) {
            count++;
        }
        token = strtok(NULL, " ");
    }
    free(copy);
    return count;
}

// Count meme speak words in a string
int32_t fossil_soap_count_rotbrain(const char *input) {
    if (input == NULL || *input == '\0') return 0;

    int count = 0;
    char *copy = _custom_fossil_strdup(input);
    if (copy == NULL) return EXIT_SUCCESS;

    char *token = strtok(copy, " ");
    while (token != NULL) {
        if (fossil_soap_is_rotbrain(token) == EXIT_FAILURE) {
            count++;
        }
        token = strtok(NULL, " ");
    }
    free(copy);
    return count;
}
