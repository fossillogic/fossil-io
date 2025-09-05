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

// Category enum
typedef enum {
    SOAP_CAT_RAGEBAIT,
    SOAP_CAT_CLICKBAIT,
    SOAP_CAT_SPAM,
    SOAP_CAT_WOKE,
    SOAP_CAT_PROPAGANDA,
    SOAP_CAT_BOT,
    SOAP_CAT_FAKE_NEWS,
    SOAP_CAT_COUNT   // Sentinel
} soap_category_t;

// Detector function type
typedef int (*soap_detector_fn)(const char *text);

// Detector registry entry
typedef struct {
    soap_category_t category;     // Category
    const char *name;             // Human-readable name
    const char **patterns;         // Lookup table of phrases
    soap_detector_fn detector;     // Optional custom function
} soap_detector_t;

// Optional: reasoning / TI metadata
typedef struct {
    const char *category_name;
    const char *matched_pattern;
    int confidence; // 0-100
} soap_ti_reason_t;

/** Lookup table for rot-brain words and their suggested replacements */
static const struct {
    const char *bad;
    const char *suggested;
} FOSSIL_SOAP_SUGGESTIONS[] = {
    {"u", "you"},
    {"gonna", "going to"},
    {"ppl", "people"},
    {"funny", "laugh out loud"},
    {"lol", "laugh out loud"},
    {"idk", "I don't know"},
    {"wanna", "want to"},
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
    {"omg", "surprising"},
    {"brb", "be right back"},
    {"imo", "in my opinion"},
    {"lmao", "laughing"},
    {"nvm", "never mind"},
    {"tbh", "to be honest"},
    {"tldr", "too long; didn't read"},
    {"ttyl", "talk to you later"},
    {"wyd", "what are you doing"},
    {"wtf", "what the heck"},
    {"yolo", "you only live once"},
    {"rot-brain", "stupid"},
    {"rot brain", "stupid"},
    {"rotbrain", "stupid"},
    {"smh", "shaking my head"},
    {"fomo", "fear of missing out"},
    {"bff", "best friend forever"},
    {"irl", "in real life"},
    {"afaik", "as far as I know"},
    {"btw", "by the way"},
    {"omw", "on my way"},
    {"ikr", "I know right"},
    {"ikr", "I know, right"},
    {"tgif", "thank goodness it's Friday"},
    {"np", "no problem"},
    {"rofl", "rolling on the floor laughing"},
    {"lmk", "let me know"},
    {"dm", "direct message"},
    {"ikr", "I know, right"},
    {"rn", "right now"},
    {"ikr", "I know, right"},
    {"smh", "shaking my head"},
    {"yw", "you're welcome"},
    {"af", "very"},
    {"ftw", "for the win"},
    {"gg", "good game"},
    {"pov", "point of view"},
    {"omfg", "oh my goodness"},
    {"tl;dr", "too long; didn't read"},
    {"fwiw", "for what it's worth"},
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
    {"would of", "would have"},
    {"not never", "never"},
    {"free gift", "gift"},
    {"very unique", "unique"},
    {"actually true", "true"},
    {"past history", "history"},
    {"future plans", "plans"},
    {"true fact", "fact"},
    {"basic fundamentals", "fundamentals"},
    {"completely destroyed", "destroyed"},
    {"revert back", "revert"},
    {"each and every", "each"},
    {"end result", "result"},
    {"final outcome", "outcome"},
    {"unexpected surprise", "surprise"},
    {"advance planning", "planning"},
    {"close proximity", "proximity"},
    {"ATM machine", "ATM"},
    {"PIN number", "PIN"},
    {"ISBN number", "ISBN"},
    {"LCD display", "LCD"},
    {"HIV virus", "HIV"},
    {"true facts", "facts"},
    {"past experiences", "experiences"},
    {NULL, NULL} // Sentinel to mark the end
};

/** Lookup table for sarcastic phrases */
static const char *SOAP_SARCASTIC_PATTERNS[] = {
    "Oh, great", "Yeah, right", "Nice job", "Well done", "Good luck with that",
    "Sure, why not", "Fantastic", "Brilliant", "Wonderful", "Perfect",
    "Oh, just what I needed", "Wow, amazing", "How original", "Incredible",
    "As if that will work", "Sure, that's smart", "Totally believable",
    "Oh, really?", "You're a genius", "Thanks a lot",
    "Couldn't be better", "That's exactly what I wanted", "Well, isn't that special",
    "Lovely", "Just perfect", "What could go wrong?", "Right, because that makes sense",
    "Great idea", "Absolutely flawless", "Marvelous", "Just wonderful", NULL
};

/** Lookup table for formal phrases */
static const char *SOAP_FORMAL_PATTERS[] = {
    "Dear Sir or Madam", "To whom it may concern", "Yours sincerely", "Yours faithfully",
    "Best regards", "Respectfully", "I would like to", "I am writing to",
    "Please find attached", "Thank you for your consideration", "I look forward to your response",
    "Kindly note", "Please be advised", "It is my pleasure to", "I would appreciate your assistance",
    "Should you require any further information", "I remain at your disposal", "With kind regards",
    "Thank you for your attention", "I am writing on behalf of", "Please accept my apologies",
    "I wish to inform you", "We would be grateful if", "I hope this message finds you well",
    "I would be obliged if", "Kindly consider", "I trust this finds you well", "Allow me to express",
    "With utmost respect", "Permit me to", NULL
};

static const char *SOAP_RAGEBAIT_PATTERNS[] = {
    "you won't believe", "outrageous", "infuriating", "makes me angry",
    "how dare they", "ridiculous", "unbelievable", "trigger warning",
    "enraging", "shocking injustice", "furious", "disgusting", "outrage",
    "unacceptable", "appalling", "scandalous", "outraged", "angry reaction",
    "horrifying", "outrage alert", "infuriated", "rage induced",
    "madness", "shocking", "unthinkable", "angry outrage", "outrage fest",
    "provocative", "furious outrage", "triggered", NULL
};

static const char *SOAP_CLICKBAIT_PATTERNS[] = {
    "how to", "top 10", "amazing", "must see", "you won't believe what happened",
    "life changing", "secret revealed", "uncovered", "incredible", "mind blown",
    "you won't believe this", "shocking", "insane", "epic", "ultimate guide",
    "hidden truth", "never knew", "reveal", "best ever", "fantastic",
    "jaw dropping", "you must see", "exclusive", "surprising", "unreal",
    "best of", "amazing discovery", "life hack", "can't miss", "insider tips",
    NULL
};

static const char *SOAP_SPAM_PATTERNS[] = {
    "free money", "work from home", "act now", "earn cash fast", "get rich quick",
    "limited time offer", "buy now", "exclusive deal", "instant results", "100% free",
    "click here", "apply now", "offer expires", "make money online", "risk free",
    "guaranteed", "easy income", "double your money", "urgent", "special promotion",
    "no investment", "limited offer", "win big", "free trial", "claim prize",
    "extra cash", "instant payout", "hot deal", "bonus", "cash bonus", NULL
};

static const char *SOAP_WOKE_PATTERNS[] = {
    "safe space", "microaggression", "check your privilege", "diversity and inclusion",
    "equity over equality", "social justice", "systemic oppression", "cultural appropriation",
    "intersectionality", "allyship", "gender equality", "anti-racism", "inclusive language",
    "oppression", "privilege check", "marginalized voices", "bias awareness", "equity",
    "discrimination", "social activism", "representation matters", "critical race theory",
    "minority rights", "empowerment", "identity politics", "decolonize", "bias training",
    "social equity", "inclusive policy", "identity awareness", NULL
};

static const char *SOAP_PROPAGANDA_PATTERNS[] = {
    "state controlled", "enemy of the people", "propaganda", "hidden agenda",
    "government lies", "manipulated media", "biased reporting", "indoctrination",
    "fake narrative", "misinformation", "censorship", "manipulative", "brainwashing",
    "political agenda", "controlled press", "fabricated story", "influenced news",
    "spinning the facts", "media bias", "deception", "false narrative", "manipulated opinion",
    "state narrative", "hidden motives", "truth distortion", "media propaganda",
    "agenda driven", "false headlines", "information control", "media manipulation", NULL
};

static const char *SOAP_BOT_PATTERNS[] = {
    "automated message", "generated content", "bot detected", "scripted response",
    "robot account", "auto-generated", "spam bot", "bot activity", "synthetic post",
    "automation detected", "fake account", "bot interaction", "robotic reply", "spam account",
    "automation bot", "ai generated", "bot behavior", "scripted content", "robotic message",
    "automated post", "fake user", "mass message", "bot network", "automated reply",
    "auto reply", "bot comment", "robot post", "fake bot", "automated comment", "bot spam", NULL
};

static const char *SOAP_FAKE_NEWS_PATTERNS[] = {
    "breaking news", "shocking revelation", "unverified report", "fact check",
    "false report", "misleading headline", "fabricated story", "hoax", "viral misinformation",
    "fake article", "rumor", "clickbait news", "unreliable source", "false claim",
    "bogus report", "made-up story", "fake report", "misinformation campaign",
    "distorted facts", "conspiracy theory", "falsified news", "untruth", "sensationalized",
    "fake headline", "misleading article", "deceptive story", "questionable source",
    "hoax report", "unconfirmed news", "false story", NULL
};

static const char *SOAP_SNOWFLAKE_PATTERNS[] = {
    "snowflake",
    "triggered",
    "fragile ego",
    "offended easily",
    "sensitive snowflake",
    "microaggression",
    "safe space",
    "special snowflake",
    "delicate",
    "thin-skinned",
    "overly sensitive",
    "crybaby",
    "tender feelings",
    "too sensitive",
    "emotionally fragile",
    "overreacting",
    "touchy",
    "soft-hearted",
    "extra sensitive",
    "hyper-sensitive",
    "prickly",
    "easily upset",
    "nervous nellie",
    "fragile personality",
    "highly sensitive",
    "overly emotional",
    "whiny",
    "melodramatic",
    "delicate flower",
    "fragile soul",
    NULL
};

/** Lookup table for offensive content detection */
static const char *SOAP_OFFENSIVE_PATTERNS[] = {
    "idiot",
    "stupid",
    "dumb",
    "moron",
    "fool",
    "loser",
    "jerk",
    "trash",
    "garbage",
    "worthless",
    "pathetic",
    "ugly",
    "disgusting",
    "nonsense",
    "ridiculous",
    "absurd",
    "hate",
    "kill",
    "die",
    "sucks",
    "shut up",
    "loser",
    "dunce",
    "ignorant",
    "nasty",
    "offensive",
    "freak",
    "creep",
    "weirdo",
    "worthless",
    NULL // Sentinel to mark the end
};

static const char *EXAGGERATED_WORDS[] = {
    "literally",
    "always",
    "never",
    "every",
    "everyone",
    "nobody",
    "forever",
    "insane",
    "unbelievable",
    "outrageous",
    "epic",
    "mind-blowing",
    "extremely",
    "completely",
    "totally",
    "absolutely",
    "massive",
    "huge",
    "gigantic",
    "tremendous",
    "incredible",
    "unreal",
    "astonishing",
    "stunning",
    "jaw-dropping",
    "ridiculous",
    "crazy",
    "fantastic",
    "amazing",
    "phenomenal",
    NULL
};

// Category enum
typedef enum {
    SOAP_CAT_RAGEBAIT,
    SOAP_CAT_CLICKBAIT,
    SOAP_CAT_SPAM,
    SOAP_CAT_WOKE,
    SOAP_CAT_PROPAGANDA,
    SOAP_CAT_BOT,
    SOAP_CAT_FAKE_NEWS,
    SOAP_CAT_SARCASM,      // New category
    SOAP_CAT_FORMAL,       // New category
    SOAP_CAT_COUNT          // Sentinel
} soap_category_t;

// Detector registry
static const soap_detector_t SOAP_DETECTORS[SOAP_CAT_COUNT] = {
    { SOAP_CAT_RAGEBAIT,  "ragebait",  SOAP_RAGEBAIT_PATTERNS,  fossil_io_soap_detect_ragebait },
    { SOAP_CAT_CLICKBAIT, "clickbait", SOAP_CLICKBAIT_PATTERNS, fossil_io_soap_detect_clickbait },
    { SOAP_CAT_SPAM,      "spam",      SOAP_SPAM_PATTERNS,      fossil_io_soap_detect_spam },
    { SOAP_CAT_WOKE,      "woke",      SOAP_WOKE_PATTERNS,      fossil_io_soap_detect_woke },
    { SOAP_CAT_PROPAGANDA,"propaganda",SOAP_PROPAGANDA_PATTERNS,fossil_io_soap_detect_propaganda },
    { SOAP_CAT_BOT,       "bot",       SOAP_BOT_PATTERNS,       fossil_io_soap_detect_bot },
    { SOAP_CAT_FAKE_NEWS, "fake",      SOAP_FAKE_NEWS_PATTERNS, fossil_io_soap_detect_fake_news },
    { SOAP_CAT_SARCASM,   "sarcasm",   SOAP_SARCASTIC_PATTERNS, fossil_io_soap_detect_sarcasm },
    { SOAP_CAT_SNOWFLAKE, "snowflake", SOAP_SNOWFLAKE_PATTERNS, fossil_io_soap_detect_snowflake },
    { SOAP_CAT_FORMAL,    "formal",    SOAP_FORMAL_PATTERNS,    fossil_io_soap_detect_formal },
    { SOAP_CAT_OFFENSIVE, "offensive", SOAP_OFFENSIVE_PATTERNS, fossil_io_soap_detect_offensive }
};

static char custom_storage[MAX_CUSTOM_FILTERS][64];
static const char *custom_filters[MAX_CUSTOM_FILTERS] = {0};

/** Lookup table for words that need to be skipped due to misdetection */
static const char *SKIP_WORDS[] = {
    "limit",
    "size",
    "width",
    "height",
    "length",
    "depth",
    "volume",
    "capacity",
    "weight",
    "age",
    "year",
    "month",
    "day",
    "hour",
    "minute",
    "second",
    "ID",
    "serial",
    "version",
    "code",
    "label",
    "status",
    "level",
    NULL // Sentinel to mark the end
};

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

static int soap_detect_patterns(const char *text, const char **patterns) {
    if (!text || !patterns) return 0;

    for (size_t i = 0; patterns[i] != NULL; i++) {
        if (custom_strcasestr(text, patterns[i])) {
            return 1; // Pattern found
        }
    }
    return 0; // No match
}

int fossil_io_soap_detect_category(const char *text, soap_category_t category) {
    if (category >= SOAP_CAT_COUNT) return 0;
    return SOAP_DETECTORS[category].detector(text);
}

int fossil_io_soap_detect_multiple(const char *text, const soap_category_t *categories, int count, bool *out_matches) {
    if (!text || !categories || count <= 0) return 0;

    int matches = 0;
    for (int i = 0; i < count; i++) {
        int detected = 0;
        if (categories[i] < SOAP_CAT_COUNT) {
            detected = SOAP_DETECTORS[categories[i]].detector(text);
        }

        if (out_matches) out_matches[i] = detected ? true : false;
        if (detected) matches++;
    }

    return matches;
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
    if (!text) return "unknown";

    // Iterate over all detectors
    for (size_t i = 0; i < SOAP_CAT_COUNT; i++) {
        const soap_detector_t *detector = &SOAP_DETECTORS[i];
        if (detector->detect_fn && detector->detect_fn(text)) {
            return detector->name; // Return category name
        }
    }

    return "casual"; // Default tone
}

int fossil_io_soap_check_grammar(const char *text) {
    if (!text) return -1;
    int found = 0;
    for (size_t i = 0; FOSSIL_SOAP_GRAMMAR_SUGGESTIONS[i].incorrect; i++) {
        const char *incorrect = FOSSIL_SOAP_GRAMMAR_SUGGESTIONS[i].incorrect;
        const char *ptr = text;
        size_t ilen = strlen(incorrect);
        while ((ptr = custom_strcasestr(ptr, incorrect)) != NULL) {
            // Check word boundaries
            int before = (ptr == text) || !isalnum((unsigned char)ptr[-1]);
            int after = !isalnum((unsigned char)ptr[ilen]);
            if (before && after) {
                found = 1;
                break;
            }
            ptr += ilen;
        }
        if (found) break;
    }
    return found;
}

char *fossil_io_soap_normalize(const char *text) {
    if (!text) return NULL;

    char *normalized = fossil_io_cstring_dup(text); // Create modifiable copy
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

    for (size_t i = 0; FOSSIL_SOAP_SUGGESTIONS[i].bad != NULL; i++) {
        const char *bad = FOSSIL_SOAP_SUGGESTIONS[i].bad;
        const char *sugg = FOSSIL_SOAP_SUGGESTIONS[i].suggested;

        const char *found = NULL;
        while ((found = custom_strcasestr(result, bad)) != NULL) {
            size_t offset = (size_t)(found - result);
            size_t newlen = strlen(result) - strlen(bad) + strlen(sugg) + 1;
        
            char *temp = malloc(newlen);
            if (!temp) {
                free(result);
                return NULL;
            }
        
            strncpy(temp, result, offset);
            temp[offset] = '\0';
            strcat(temp, sugg);
            strcat(temp, result + offset + strlen(bad));
        
            free(result);
            result = temp;
        }
    }

    return result;
}

int fossil_io_soap_detect_ragebait(const char *text) {
    return soap_detect_patterns(text, SOAP_RAGEBAIT_PATTERNS);
}

int fossil_io_soap_detect_clickbait(const char *text) {
    return soap_detect_patterns(text, SOAP_CLICKBAIT_PATTERNS);
}

int fossil_io_soap_detect_spam(const char *text) {
    return soap_detect_patterns(text, SOAP_SPAM_PATTERNS);
}

int fossil_io_soap_detect_woke(const char *text) {
    return soap_detect_patterns(text, SOAP_WOKE_PATTERNS);
}

int fossil_io_soap_detect_propaganda(const char *text) {
    return soap_detect_patterns(text, SOAP_PROPAGANDA_PATTERNS);
}

int fossil_io_soap_detect_bot(const char *text) {
    return soap_detect_patterns(text, SOAP_BOT_PATTERNS);
}

int fossil_io_soap_detect_fake_news(const char *text) {
    return soap_detect_patterns(text, SOAP_FAKE_NEWS_PATTERNS);
}

int fossil_io_soap_detect_sarcasm(const char *text) {
    return soap_detect_patterns(text, SOAP_SARCASTIC_PATTERNS);
}

int fossil_io_soap_detect_formal(const char *text) {
    return soap_detect_patterns(text, SOAP_FORMAL_PATTERNS);
}

int fossil_io_soap_detect_snowflake(const char *text) {
    return soap_detect_patterns(text, SOAP_SNOWFLAKE_PATTERNS);
}

int fossil_io_soap_detect_offensive(const char *text) {
    return soap_detect_patterns(text, SOAP_OFFENSIVE_PATTERNS);
}

int fossil_io_soap_detect_exaggeration(const char *text) {
    if (!text) return 0;

    // Check for exaggerated words as whole words only
    for (int i = 0; EXAGGERATED_WORDS[i] != NULL; i++) {
        const char *word = EXAGGERATED_WORDS[i];
        const char *ptr = text;
        size_t wlen = strlen(word);

        while ((ptr = custom_strcasestr(ptr, word)) != NULL) {
            int before = (ptr == text) || !isalnum((unsigned char)ptr[-1]);
            int after = !isalnum((unsigned char)ptr[wlen]);
            if (before && after) {
                return 1;
            }
            ptr += wlen;
        }
    }

    // Optionally, check for repeated exclamation marks or all-caps words
    int exclam_count = 0, caps_word = 0, in_word = 0, word_len = 0, caps_count = 0;
    for (const char *p = text; *p; ++p) {
        if (*p == '!') {
            exclam_count++;
            if (exclam_count >= 3) return 1;
        } else {
            exclam_count = 0;
        }

        if (isalpha((unsigned char)*p)) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
                caps_count = 0;
            }
            word_len++;
            if (isupper((unsigned char)*p)) caps_count++;
        } else {
            if (in_word && word_len >= 4 && caps_count == word_len) {
                caps_word = 1;
            }
            in_word = 0;
        }
    }
    if (caps_word) return 1;

    return 0;
}

char *fossil_io_soap_filter_offensive(const char *text) {
    if (!text) return NULL;

    char *result = fossil_io_cstring_dup(text);
    if (!result) return NULL;

    for (size_t i = 0; SOAP_OFFENSIVE_PATTERNS[i].offensive != NULL; i++) {
        const char *bad = SOAP_OFFENSIVE_PATTERNS[i].offensive;
        const char *good = SOAP_OFFENSIVE_PATTERNS[i].replacement;

        const char *found = NULL;
        while ((found = custom_strcasestr(result, bad)) != NULL) {
            size_t offset = (size_t)(found - result);
            size_t newlen = strlen(result) - strlen(bad) + strlen(good) + 1;

            char *temp = (char *)malloc(newlen);
            if (!temp) {
                free(result);
                return NULL;
            }

            // Copy prefix safely
            if (offset > 0) {
                memcpy(temp, result, offset);
            }
            temp[offset] = '\0';

            // Append replacement
            strcat(temp, good);

            // Append suffix
            strcat(temp, result + offset + strlen(bad));

            free(result);
            result = temp;
        }
    }

    return result;
}
