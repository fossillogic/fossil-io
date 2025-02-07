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

#define HASH_TABLE_SIZE 1024

typedef struct fossil_io_soap_hash_node_t {
    char *word;
    struct fossil_io_soap_hash_node_t *next;
} fossil_io_soap_hash_node_t;

static fossil_io_soap_hash_node_t *offensive_words[HASH_TABLE_SIZE] = {0};
static fossil_io_soap_hash_node_t *rotbrain_words[HASH_TABLE_SIZE] = {0};

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
static const char *FOSSIL_SOAP_ROTBRAIN[] = {
    "meme1",
    "meme2",

    "rizz", "skibidi", "yeet", "sus", "vibe", "lit", "no cap", "bet", "fam", "bruh",
    "flex", "ghost", "goat", "gucci", "hype", "janky", "lowkey", "mood", "salty", "shade",
    "slay", "snatched", "stan", "tea", "thirsty", "woke", "yolo", "zaddy", "drip", "fire",
    "lol", "omg", "brb"

    // Support for other terms can be added via PR to this repository
};

// Simple hash function
static uint32_t hash_string(const char *str) {
    uint32_t hash = 5381;
    while (*str) {
        hash = ((hash << 5) + hash) + (unsigned char)(*str++);
    }
    return hash % HASH_TABLE_SIZE;
}

static char* custom_strdup(const char* str) {
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

// Insert a word into the hash set
static void insert_word(fossil_io_soap_hash_node_t **table, const char *word) {
    uint32_t hash = hash_string(word);
    fossil_io_soap_hash_node_t *node = (fossil_io_soap_hash_node_t *)malloc(sizeof(fossil_io_soap_hash_node_t));
    node->word = custom_strdup(word);
    node->next = table[hash];
    table[hash] = node;
}

// Check if a word exists in the hash set
static int word_exists(fossil_io_soap_hash_node_t **table, const char *word) {
    uint32_t hash = hash_string(word);
    for (fossil_io_soap_hash_node_t *node = table[hash]; node; node = node->next) {
        if (strcasecmp(node->word, word) == 0) {
            return 1; // Found
        }
    }
    return 0; // Not found
}

// Replace is_in_list with this:
static int32_t is_in_list(const char *word, fossil_io_soap_hash_node_t **table) {
    return word_exists(table, word) ? EXIT_FAILURE : EXIT_SUCCESS;
}

static int32_t detect_contextual_tone(const char *input) {
    int offensive_count = fossil_io_soap_count_offensive(input);
    int rotbrain_count = fossil_io_soap_count_rotbrain(input);

    if (offensive_count > 0 && rotbrain_count > 0) {
        return 3; // Mixed tone
    } else if (offensive_count > 0) {
        return 2; // Offensive tone
    } else if (rotbrain_count > 0) {
        return 1; // Rotbrain tone
    }
    return 0; // Neutral
}

// Boyer-Moore-Horspool case-insensitive substring search
static char *bmh_strcasestr(const char *haystack, const char *needle) {
    if (!haystack || !needle || *needle == '\0') return NULL;

    size_t h_len = strlen(haystack);
    size_t n_len = strlen(needle);

    if (n_len > h_len) return NULL;

    // Precompute the bad character shift table
    int shift[256];
    for (int i = 0; i < 256; i++) shift[i] = n_len;

    for (size_t i = 0; i < n_len - 1; i++) {
        shift[(unsigned char)tolower(needle[i])] = n_len - 1 - i;
        shift[(unsigned char)toupper(needle[i])] = n_len - 1 - i;
    }

    // Search using BMH
    size_t i = 0;
    while (i <= h_len - n_len) {
        size_t j = n_len - 1;
        while (j > 0 && tolower(haystack[i + j]) == tolower(needle[j])) {
            j--;
        }
        if (j == 0 && tolower(haystack[i]) == tolower(needle[0])) {
            return (char *)(haystack + i);
        }
        i += shift[(unsigned char)haystack[i + n_len - 1]];
    }
    return NULL;
}

// Function to replace a substring in a string (case-insensitive)
static char *replace_substring_case_insensitive(const char *input, const char *old_substr, const char *new_substr) {
    if (!input || !old_substr || !new_substr) return NULL;

    size_t input_len = strlen(input);
    size_t old_len = strlen(old_substr);
    size_t new_len = strlen(new_substr);

    // Count occurrences of old_substr in input
    size_t count = 0;
    const char *temp = input;
    while ((temp = bmh_strcasestr(temp, old_substr)) != NULL) {
        count++;
        temp += old_len;
    }

    if (count == 0) return custom_strdup(input);  // No matches, return a copy

    // Allocate new buffer with enough space
    size_t new_size = input_len + (new_len - old_len) * count + 1;
    char *output = (char *)malloc(new_size);
    if (!output) return NULL;

    char *out_ptr = output;
    temp = input;
    while (*temp) {
        char *match = bmh_strcasestr(temp, old_substr);
        if (!match) {
            strcpy(out_ptr, temp);
            break;
        }

        // Copy text before match
        size_t prefix_len = match - temp;
        strncpy(out_ptr, temp, prefix_len);
        out_ptr += prefix_len;

        // Copy replacement text
        strcpy(out_ptr, new_substr);
        out_ptr += new_len;

        temp = match + old_len;
    }
    
    *out_ptr = '\0';
    return output;
}

// =============================================================================
// Function implementations
// =============================================================================

// Preload word lists into the hash table at initialization
void fossil_io_soap_create(void) {
    for (size_t i = 0; i < sizeof(FOSSIL_SOAP_OFFENSIVE) / sizeof(FOSSIL_SOAP_OFFENSIVE[0]); i++) {
        if (!FOSSIL_SOAP_OFFENSIVE[i]) continue; // Skip NULL entries
        insert_word(offensive_words, FOSSIL_SOAP_OFFENSIVE[i]);
    }
    for (size_t i = 0; i < sizeof(FOSSIL_SOAP_ROTBRAIN) / sizeof(FOSSIL_SOAP_ROTBRAIN[0]); i++) {
        if (!FOSSIL_SOAP_ROTBRAIN[i]) continue; // Skip NULL entries
        insert_word(rotbrain_words, FOSSIL_SOAP_ROTBRAIN[i]);
    }
}

// Faster checks
int32_t fossil_io_soap_is_offensive(const char *word) {
    if (!word) return EXIT_SUCCESS; // Handle NULL input gracefully
    return is_in_list(word, offensive_words);
}

int32_t fossil_io_soap_is_rotbrain(const char *word) {
    if (!word) return EXIT_SUCCESS; // Handle NULL input gracefully
    return is_in_list(word, rotbrain_words);
}

int32_t fossil_io_soap_count_offensive(const char *input) {
    if (!input || *input == '\0') return 0;

    int count = 0;
    char *temp = custom_strdup(input);
    if (!temp) return 0;

    char *token = strtok(temp, " ,.!?;:()[]{}<>\"\'\\/-_+=*&^%$#@!~`|");
    while (token) {
        if (fossil_io_soap_is_offensive(token) == EXIT_FAILURE) {
            count++;
        }
        token = strtok(NULL, " ,.!?;:()[]{}<>\"\'\\/-_+=*&^%$#@!~`|");
    }

    free(temp);
    return count;
}

int32_t fossil_io_soap_count_rotbrain(const char *input) {
    if (!input || *input == '\0') return 0;

    int count = 0;
    char *temp = custom_strdup(input);
    if (!temp) return 0;

    char *token = strtok(temp, " ,.!?;:()[]{}<>\"\'\\/-_+=*&^%$#@!~`|");
    while (token) {
        if (fossil_io_soap_is_rotbrain(token) == EXIT_FAILURE) {
            count++;
        }
        token = strtok(NULL, " ,.!?;:()[]{}<>\"\'\\/-_+=*&^%$#@!~`|");
    }

    free(temp);
    return count;
}

int32_t fossil_io_soap_count_positive(const char *input) {
    if (!input || *input == '\0') return 0;

    int count = 0;
    char *temp = custom_strdup(input);
    if (!temp) return 0;

    char *token = strtok(temp, " ,.!?;:");
    while (token) {
        if (fossil_io_soap_is_offensive(token) == EXIT_SUCCESS && fossil_io_soap_is_rotbrain(token) == EXIT_SUCCESS) {
            count++;
        }
        token = strtok(NULL, " ,.!?;:");
    }

    free(temp);
    return count;
}

char *fossil_io_soap_sanitize(const char *input) {
    if (!input || *input == '\0') return NULL;

    char *sanitized = custom_strdup(input);
    if (!sanitized) return NULL;

    // Replace offensive words
    for (size_t i = 0; i < sizeof(FOSSIL_SOAP_OFFENSIVE) / sizeof(FOSSIL_SOAP_OFFENSIVE[0]); ++i) {
        char *temp = replace_substring_case_insensitive(sanitized, FOSSIL_SOAP_OFFENSIVE[i], "***");
        free(sanitized);
        sanitized = temp;
    }

    // Replace rotbrain words
    for (size_t i = 0; i < sizeof(FOSSIL_SOAP_ROTBRAIN) / sizeof(FOSSIL_SOAP_ROTBRAIN[0]); ++i) {
        char *temp = replace_substring_case_insensitive(sanitized, FOSSIL_SOAP_ROTBRAIN[i], "[ROT]");
        free(sanitized);
        sanitized = temp;
    }

    // Detect contextual tone
    int tone = detect_contextual_tone(input);
    switch (tone) {
        case 1:
            printf("Rotbrain tone detected.\n");
            break;
        case 2:
            printf("Offensive tone detected.\n");
            break;
        case 3:
            printf("Mixed tone detected.\n");
            break;
        default:
            printf("Neutral tone detected.\n");
            break;
    }

    return sanitized;
}
