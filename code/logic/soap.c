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
    {"tgif", "thank goodness it's Friday"},
    {"np", "no problem"},
    {"rofl", "rolling on the floor laughing"},
    {"lmk", "let me know"},
    {"dm", "direct message"},
    {"rn", "right now"},
    {"yw", "you're welcome"},
    {"af", "very"},
    {"ftw", "for the win"},
    {"gg", "good game"},
    {"pov", "point of view"},
    {"omfg", "oh my goodness"},
    {"tl;dr", "too long; didn't read"},
    {"fwiw", "for what it's worth"},
    {"bday", "birthday"},
    {"gr8", "great"},
    {"hmu", "hit me up"},
    {"jk", "just kidding"},
    {"k", "okay"},
    {"l8r", "later"},
    {"msg", "message"},
    {"pls", "please"},
    {"plz", "please"},
    {"thx", "thanks"},
    {"tho", "though"},
    {"w/", "with"},
    {"w/o", "without"},
    {"xoxo", "hugs and kisses"},
    {"y", "why"},
    {"b/c", "because"},
    {"cuz", "because"},
    {"coz", "because"},
    {"dunno", "don't know"},
    {"g2g", "got to go"},
    {"hbu", "how about you"},
    {"idc", "I don't care"},
    {"ily", "I love you"},
    {"l8", "late"},
    {"n/a", "not applicable"},
    {"nvm", "never mind"},
    {"omw", "on my way"},
    {"ppl", "people"},
    {"qt", "cutie"},
    {"sup", "what's up"},
    {"tba", "to be announced"},
    {"tbc", "to be continued"},
    {"w/e", "whatever"},
    {"wth", "what the heck"},
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
    {"irregardless", "regardless"},
    {"supposably", "supposedly"},
    {"could care less", "couldn't care less"},
    {"for all intensive purposes", "for all intents and purposes"},
    {"escape goat", "scapegoat"},
    {"expresso", "espresso"},
    {"nucular", "nuclear"},
    {"pacifically", "specifically"},
    {"definately", "definitely"},
    {"probly", "probably"},
    {"prolly", "probably"},
    {"alot", "a lot"},
    {"could of", "could have"},
    {"should of", "should have"},
    {"would of", "would have"},
    {"wouldn't of", "wouldn't have"},
    {"shouldn't of", "shouldn't have"},
    {"couldn't of", "couldn't have"},
    {"less people", "fewer people"},
    {"less cars", "fewer cars"},
    {"less books", "fewer books"},
    {"less problems", "fewer problems"},
    {"less mistakes", "fewer mistakes"},
    {"between you and I", "between you and me"},
    {"me and him", "he and I"},
    {"me and her", "she and I"},
    {"me and them", "they and I"},
    {"me and John", "John and I"},
    {"I seen", "I saw"},
    {"I done", "I did"},
    {"I been", "I have been"},
    {"I had went", "I had gone"},
    {"I had ran", "I had run"},
    {"I had ate", "I had eaten"},
    {"I had wrote", "I had written"},
    {"I had broke", "I had broken"},
    {"I had spoke", "I had spoken"},
    {"I had took", "I had taken"},
    {"I had drank", "I had drunk"},
    {"I had drove", "I had driven"},
    {"I had froze", "I had frozen"},
    {"I had chose", "I had chosen"},
    {"I had gave", "I had given"},
    {"I had rode", "I had ridden"},
    {"I had stole", "I had stolen"},
    {"I had swam", "I had swum"},
    {"I had wore", "I had worn"},
    {"I had wrote", "I had written"},
    {"I had ate", "I had eaten"},
    {"I had spoke", "I had spoken"},
    {"I had broke", "I had broken"},
    {"I had took", "I had taken"},
    {"I had drank", "I had drunk"},
    {"I had drove", "I had driven"},
    {"I had froze", "I had frozen"},
    {"I had chose", "I had chosen"},
    {"I had gave", "I had given"},
    {"I had rode", "I had ridden"},
    {"I had stole", "I had stolen"},
    {"I had swam", "I had swum"},
    {"I had wore", "I had worn"},
    {"I had wrote", "I had written"},
    {"I had ate", "I had eaten"},
    {"I had spoke", "I had spoken"},
    {"I had broke", "I had broken"},
    {"I had took", "I had taken"},
    {"I had drank", "I had drunk"},
    {"I had drove", "I had driven"},
    {"I had froze", "I had frozen"},
    {"I had chose", "I had chosen"},
    {"I had gave", "I had given"},
    {"I had rode", "I had ridden"},
    {"I had stole", "I had stolen"},
    {"I had swam", "I had swum"},
    {"I had wore", "I had worn"},
    {NULL, NULL} // Sentinel to mark the end
};

/** Lookup table for sarcastic phrases */
static const char *SOAP_SARCASTIC_PATTERNS[] = {
    "Oh, great", "Yeah, right", "Nice job", "Well done", "Good luck with that", "Sure, why not", "Fantastic", "Brilliant", "Wonderful", "Perfect",
    "Oh, just what I needed", "Wow, amazing", "How original", "Incredible", "As if that will work", "Sure, that's smart", "Totally believable", "Oh, really?",
    "You're a genius", "Thanks a lot", "Couldn't be better", "That's exactly what I wanted", "Well, isn't that special", "Lovely", "Just perfect",
    "What could go wrong?", "Right, because that makes sense", "Great idea", "Absolutely flawless", "Marvelous", "Just wonderful", "Oh, that's helpful",
    "Just what I expected", "Couldn't ask for more", "That's not suspicious at all", "Oh, that's convincing", "What a surprise", "How unexpected",
    "Just my luck", "Of course, why wouldn't it", "That's so typical", "What a coincidence", "Just in time", "Couldn't be happier",
    "That's exactly what I needed", "Oh, that's rich", "How fortunate", "Just fantastic", "Oh, that's brilliant", "Couldn't be more obvious",
    "How convenient", NULL
};

/** Lookup table for formal phrases */
static const char *SOAP_FORMAL_PATTERNS[] = {
    "Dear Sir or Madam", "To whom it may concern", "Yours sincerely", "Yours faithfully", "Best regards", "Respectfully",
    "I would like to", "I am writing to", "Please find attached", "Thank you for your consideration", "I look forward to your response",
    "Kindly note", "Please be advised", "It is my pleasure to", "I would appreciate your assistance", "Should you require any further information",
    "I remain at your disposal", "With kind regards", "Thank you for your attention", "I am writing on behalf of", "Please accept my apologies",
    "I wish to inform you", "We would be grateful if", "I hope this message finds you well", "I would be obliged if", "Kindly consider",
    "I trust this finds you well", "Allow me to express", "With utmost respect", "Permit me to", "I am pleased to inform you",
    "I would like to request", "I am delighted to", "I am honored to", "I am grateful for", "I am reaching out to",
    "I am writing regarding", "I am contacting you to", "I am pleased to submit", "I am pleased to provide", "I am pleased to announce",
    "I am pleased to offer", "I am pleased to confirm", "I am pleased to accept", "I am pleased to acknowledge", "I am pleased to extend",
    "I am pleased to invite", "I am pleased to welcome", "I am pleased to recommend", "I am pleased to endorse", NULL
};

static const char *SOAP_RAGEBAIT_PATTERNS[] = {
    "you won't believe", "outrageous", "infuriating", "makes me angry", "how dare they", "ridiculous", "unbelievable", "trigger warning", "enraging", "shocking injustice",
    "furious", "disgusting", "outrage", "unacceptable", "appalling", "scandalous", "outraged", "angry reaction", "horrifying", "outrage alert",
    "infuriated", "rage induced", "madness", "shocking", "unthinkable", "angry outrage", "outrage fest", "provocative", "furious outrage", "triggered",
    "ragebait", "fuming", "explosive reaction", "heated debate", "controversial", "offensive", "insulting", "hate-filled", "hate speech", "unjust",
    "unfair", "disrespectful", "call to action", "raging", "storm of anger", "backlash", "public outrage", "viral outrage", "internet rage", "mass outrage",
    "social media outrage", NULL
};

static const char *SOAP_CLICKBAIT_PATTERNS[] = {
    "how to", "top 10", "amazing", "must see", "you won't believe what happened", "life changing", "secret revealed", "uncovered", "incredible", "mind blown",
    "you won't believe this", "shocking", "insane", "epic", "ultimate guide", "hidden truth", "never knew", "reveal", "best ever", "fantastic", "jaw dropping",
    "you must see", "exclusive", "surprising", "unreal", "best of", "amazing discovery", "life hack", "can't miss", "insider tips", "what happened next",
    "this will change your life", "the truth about", "watch until the end", "don't miss this", "the secret to", "revealed", "breakthrough", "the real reason",
    "you need to know", "must watch", "unbelievable", "game changer", "before and after", "biggest ever", "most shocking", "crazy story", "you won't believe your eyes",
    "the best kept secret", "what experts don't tell you", "the ultimate list", NULL
};

static const char *SOAP_SPAM_PATTERNS[] = {
    "free money", "work from home", "act now", "earn cash fast", "get rich quick", "limited time offer", "buy now", "exclusive deal",
    "instant results", "100% free", "click here", "apply now", "offer expires", "make money online", "risk free", "guaranteed",
    "easy income", "double your money", "urgent", "special promotion", "no investment", "limited offer", "win big", "free trial",
    "claim prize", "extra cash", "instant payout", "hot deal", "bonus", "cash bonus", "lowest price", "save big", "limited stock",
    "don't miss out", "order now", "get started today", "exclusive offer", "limited time only", "no obligation", "money back guarantee",
    "fast cash", "get paid today", "easy steps", "no experience needed", "start earning now", "unbelievable deal", "limited seats",
    "special discount", "win a prize", "free access", "limited availability", NULL
};

static const char *SOAP_WOKE_PATTERNS[] = {
    "safe space", "microaggression", "check your privilege", "diversity and inclusion", "equity over equality",
    "social justice", "systemic oppression", "cultural appropriation", "intersectionality", "allyship",
    "gender equality", "anti-racism", "inclusive language", "oppression", "privilege check",
    "marginalized voices", "bias awareness", "equity", "discrimination", "social activism",
    "representation matters", "critical race theory", "minority rights", "empowerment", "identity politics",
    "decolonize", "bias training", "social equity", "inclusive policy", "identity awareness",
    "gender neutral", "pronoun respect", "intersectional feminism", "diversity training", "racial justice",
    "gender fluidity", "safe environment", "trigger warning", "progressive values", "inclusive spaces",
    "anti-bias", "restorative justice", "affirmative action", "equitable access", "community empowerment",
    "inclusive curriculum", "representation equity", "social responsibility", "inclusive leadership",
    "gender inclusivity", "racial equity", NULL
};

/** Lookup table for bot-like patterns */
static const char *SOAP_BOT_PATTERNS[] = {
    "ai generated", "algorithmic message", "artificial response", "auto reply", "auto responder", "auto-generated",
    "automated comment", "automated message", "automated post", "automated reply", "automation bot", "automation detected",
    "autonomous post", "bot activity", "bot behavior", "bot comment", "bot detected", "bot follower", "bot interaction",
    "bot like", "bot moderator", "bot network", "bot retweet", "bot spam", "bot upvote", "bulk message", "copy-paste answer",
    "fake account", "fake bot", "fake user", "generated content", "generic message", "machine generated", "mass message",
    "mass posting", "mass produced content", "non-human reply", "predefined response", "programmed answer", "repetitive comment",
    "robot account", "robot post", "robotic message", "robotic reply", "scripted content", "scripted response", "spam account",
    "spam bot", "system generated", "synthetic post", "template reply", "unoriginal reply", NULL
};

/** Lookup table for snowflake patterns */
static const char *SOAP_SNOWFLAKE_PATTERNS[] = {
    "snowflake", "triggered", "fragile ego", "offended easily", "sensitive snowflake",
    "microaggression", "safe space", "special snowflake", "delicate", "thin-skinned",
    "overly sensitive", "crybaby", "tender feelings", "too sensitive", "emotionally fragile",
    "overreacting", "touchy", "soft-hearted", "extra sensitive", "hyper-sensitive",
    "prickly", "easily upset", "nervous nellie", "fragile personality", "highly sensitive",
    "overly emotional", "whiny", "melodramatic", "delicate flower", "fragile soul",
    "overprotected", "coddled", "pampered", "overly sheltered", "easily offended",
    "thin skin", "overly dramatic", "sheltered", "overly cautious", "emotionally weak",
    "overly anxious", "overly reactive", "overly sentimental", "easily disturbed",
    "overly nurturing", "emotionally unstable", "overly caring", "overly empathetic",
    "overly worried", "overly fearful", NULL
};

/** Lookup table for offensive content detection */
static const char *SOAP_OFFENSIVE_PATTERNS[] = {
    "idiot", "stupid", "dumb", "moron", "fool", "loser", "jerk", "trash", "garbage",
    "worthless", "pathetic", "ugly", "disgusting", "nonsense", "ridiculous", "absurd",
    "hate", "kill", "die", "sucks", "shut up", "dunce", "ignorant", "nasty",
    "offensive", "freak", "creep", "weirdo", "worthless", "imbecile", "retard",
    "scum", "vermin", "filth", "vile", "repulsive", "gross", "horrible", "evil",
    "abomination", "monster", "beast", "brainless", "airhead", "twit", "twat",
    "douche", "bastard", "maniac", "psycho", "lunatic", "savage", NULL
};

/** Lookup table for neutral tone patterns */
static const char *SOAP_NEUTRAL_PATTERNS[] = {
    "as expected", "according to plan", "no problem", "all good", "fine", "okay", "normal",
    "standard procedure", "routine", "average", "typical", "usual", "nothing special",
    "business as usual", "no issues", "no concerns", "no complaints", "acceptable",
    "satisfactory", "regular", "ordinary", "unremarkable", "moderate", "fair", "adequate",
    "sufficient", "reasonable", "not bad", "not great", "so-so", "meh", "neutral",
    "indifferent", "unbiased", "impartial", "objective", "even-handed", "middle ground",
    "balanced", "equitable", "nonpartisan", "detached", "reserved", "unemotional",
    "calm", "steady", "level-headed", "matter-of-fact", "plain", "straightforward",
    "clear", "direct", "simple", "uncomplicated", NULL
};

/** Lookup table for hype phrases */
static const char *SOAP_HYPE_PATTERNS[] = {
    "game-changing", "revolutionary", "cutting-edge", "disruptive", "never before seen",
    "unprecedented", "the ultimate", "next-level", "life-changing", "epic", "legendary",
    "the best ever", "mind-blowing", "world-class", "exclusive", "groundbreaking", "innovative",
    "unparalleled", "must-have", "hot new", "phenomenal", "incredible", "jaw-dropping",
    "guaranteed", "proven results", "miracle", "unstoppable", "the future of", "beyond belief",
    NULL
};

/** Political */
static const char *SOAP_POLITICAL_PATTERNS[] = { 
    "left-wing","right-wing","liberal agenda","conservative values","fake news media",
    "mainstream media","deep state","radical","extremist","social justice warrior","tax and spend",
    "big government","free market","personal freedom","authoritarian","totalitarian","censorship",
    "political correctness","cancel culture","big brother","elitist","grassroots movement",
    "identity politics","partisan","special interests","lobbyist","corrupt politicians","vote rigging",
    "rigged system","polarized nation",NULL };

/** Conspiracy */
static const char *SOAP_CONSPIRACY_PATTERNS[] = {
    "they don’t want you to know","hidden truth","cover up","shadow government","mind control",
    "secret society","illuminati","new world order","false flag","plandemic","chemtrails",
    "crisis actors","fabricated evidence","hollow earth","flat earth","alien cover-up",
    "government lies","deep underground base","suppressed technology","mass surveillance",
    "thought police","fabricated by the media","controlled opposition","secret labs",
    "fake moon landing","reptilian","massive hoax","in on it","they control everything",
    "behind closed doors",NULL };

/** Marketing */
static const char *SOAP_MARKETING_PATTERNS[] = {
    "limited time offer","act now","don’t miss out","guaranteed results","risk free",
    "sign up today","exclusive deal","free trial","buy one get one","special promotion",
    "limited stock","save big","lowest price ever","best deal","offer ends soon",
    "only for today","unlock savings","no obligation","instant access",
    "money back guarantee","redeem now","hot deal","flash sale","discounted rate",
    "claim your spot","exclusive offer","limited edition","join thousands",
    "be the first","special savings",NULL };

/** Technobabble */
static const char *SOAP_TECHNOBABBLE_PATTERNS[] = {
    "synergy","blockchain-enabled","AI-powered","machine learning solution",
    "next-gen","hyper scalable","paradigm shift","deep tech","seamless integration",
    "big data","cloud-native","cutting-edge platform","quantum leap","value proposition",
    "turnkey solution","innovative ecosystem","frictionless","low-hanging fruit",
    "mission critical","digital transformation","smart disruption","empower users",
    "end-to-end","future-proof","holistic approach","thought leadership","revolutionize",
    "strategic alignment","game-changer","intelligent automation",NULL };

/** Lookup table for low-quality signals */
static const char *SOAP_QUALITY_PATTERNS[] = {
    "just saying", "you know", "like literally", "basically", "whatever", "stuff and things",
    "random nonsense", "blah blah", "and so on", "thingy", "meh", "idk", "not sure", "somehow",
    "something like that", "kind of", "sort of", "whatever works", "in a way", "obviously",
    "clearly", "everyone knows", "trust me", "believe me", "it is what it is", "that kind of thing",
    "doesn’t matter", "whatever you think",
    NULL
};

/** Lookup table for words that need to be skipped due to misdetection */
static const char *SKIP_WORDS[] = {
    "limit", "size", "width", "height", "length", "depth", "volume", "capacity", "weight",
    "age", "year", "month", "day", "hour", "minute", "second", "ID", "serial", "version",
    "code", "label", "status", "level",
    NULL // Sentinel to mark the end
};

static char custom_storage[MAX_CUSTOM_FILTERS][64];
static const char *custom_filters[MAX_CUSTOM_FILTERS] = {0};

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
 * @brief Simple regex-like pattern matcher for phrase arrays.
 *        Supports '*' (matches any sequence) and '?' (matches any single char).
 *        Case-insensitive.
 */
static int fossil_io_soap_simple_regex_match(const char *text, const char *pattern) {
    // Case-insensitive matching
    while (*pattern) {
        if (*pattern == '*') {
            pattern++;
            if (!*pattern) return 1; // Trailing '*' matches everything
            while (*text) {
                if (fossil_io_soap_simple_regex_match(text, pattern)) return 1;
                text++;
            }
            return 0;
        } else if (*pattern == '?') {
            if (!*text) return 0;
            text++;
            pattern++;
        } else {
            if (tolower((unsigned char)*text) != tolower((unsigned char)*pattern)) return 0;
            text++;
            pattern++;
        }
    }
    return *text == '\0';
}

/**
 * @brief Checks if any pattern in the array matches the text using simple regex.
 */
static int fossil_io_soap_regex_patterns(const char *text, const char **patterns) {
    if (!text || !patterns) return 0;
    for (size_t i = 0; patterns[i] != NULL; i++) {
        if (fossil_io_soap_simple_regex_match(text, patterns[i])) {
            return 1;
        }
    }
    return 0;
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
 *        Uses regex-like matching for phrase arrays.
 *        Censored words are replaced with '*'.
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

            int is_rotbrain = fossil_io_soap_regex_patterns(word, (const char *[]){"rot-brain", "rot brain", "rotbrain", NULL});
            int is_meme = fossil_io_soap_regex_patterns(word, (const char *[]){"skibidi", "rizz", "yeet", "sus", "vibe", "lit", NULL});

            if (word_idx > 0 && (is_rotbrain || is_meme) && !should_skip_word(word)) {
                for (size_t j = 0; j < word_idx; j++) {
                    output[out_idx++] = '*';
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

    int is_rotbrain = fossil_io_soap_regex_patterns(word, (const char *[]){"rot-brain", "rot brain", "rotbrain", NULL});
    int is_meme = fossil_io_soap_regex_patterns(word, (const char *[]){"skibidi", "rizz", "yeet", "sus", "vibe", "lit", NULL});

    if (word_idx > 0 && (is_rotbrain || is_meme) && !should_skip_word(word)) {
        for (size_t j = 0; j < word_idx; j++) {
            output[out_idx++] = '*';
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

/**
 * @brief Detects the tone of the given text using regex-like pattern matching.
 *        Returns a comma-separated string of detected tones, or "casual" if none found.
 */
const char *fossil_io_soap_detect_tone(const char *text) {
    if (!text) return "unknown";

    static char tone_result[256];
    tone_result[0] = '\0';
    int found = 0;

    struct {
        const char **patterns;
        const char *label;
    } tone_checks[] = {
        {SOAP_RAGEBAIT_PATTERNS, "ragebait"},
        {SOAP_CLICKBAIT_PATTERNS, "clickbait"},
        {SOAP_SPAM_PATTERNS, "spam"},
        {SOAP_WOKE_PATTERNS, "woke"},
        {SOAP_BOT_PATTERNS,           "bot"},
        {SOAP_SARCASTIC_PATTERNS,     "sarcastic"},
        {SOAP_SNOWFLAKE_PATTERNS,     "snowflake"},
        {SOAP_FORMAL_PATTERNS,        "formal"},
        {SOAP_OFFENSIVE_PATTERNS,     "offensive"},
        {SOAP_NEUTRAL_PATTERNS,       "neutral"},
        { SOAP_QUALITY_PATTERNS,      "quality"     },
        { SOAP_HYPE_PATTERNS,         "hype"        },
        { SOAP_POLITICAL_PATTERNS,    "political"   },
        { SOAP_CONSPIRACY_PATTERNS,   "conspiracy"  },
        { SOAP_MARKETING_PATTERNS,    "marketing"   },
        { SOAP_TECHNOBABBLE_PATTERNS, "technobabble"},
        { NULL,                       NULL          }
    };

    // Split text into words for more accurate matching
    char temp[512];
    strncpy(temp, text, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';

    char *word = temp;
    for (size_t i = 0; tone_checks[i].patterns != NULL; i++) {
        int match = 0;
        char *ptr = word;
        while (*ptr) {
            // Skip leading non-alnum
            while (*ptr && !isalnum((unsigned char)*ptr)) ptr++;
            if (!*ptr) break;
            char *start = ptr;
            while (*ptr && (isalnum((unsigned char)*ptr) || *ptr == '\'' || *ptr == '-')) ptr++;
            char saved = *ptr;
            *ptr = '\0';

            // Normalize leetspeak for each word
            char norm[64];
            strncpy(norm, start, sizeof(norm) - 1);
            norm[sizeof(norm) - 1] = '\0';
            fossil_io_soap_normalize_leetspeak(norm);

            // Check word and full text for pattern match
            if (fossil_io_soap_regex_patterns(norm, tone_checks[i].patterns) ||
                fossil_io_soap_regex_patterns(text, tone_checks[i].patterns)) {
                match = 1;
                *ptr = saved;
                break;
            }
            *ptr = saved;
            if (*ptr) ptr++;
        }
        if (match) {
            if (found) strcat(tone_result, ",");
            strcat(tone_result, tone_checks[i].label);
            found = 1;
        }
    }

    if (!found) return "casual";
    return tone_result;
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

char *fossil_io_soap_correct_grammar(const char *text) {
    if (!text) return NULL;

    char *corrected = fossil_io_cstring_dup(text); // Create modifiable copy
    if (!corrected) return NULL;

    for (size_t i = 0; FOSSIL_SOAP_GRAMMAR_SUGGESTIONS[i].incorrect; i++) {
        const char *incorrect = FOSSIL_SOAP_GRAMMAR_SUGGESTIONS[i].incorrect;
        const char *correct = FOSSIL_SOAP_GRAMMAR_SUGGESTIONS[i].correct;
        const char *ptr;
        while ((ptr = custom_strcasestr(corrected, incorrect))) {
            // Check word boundaries
            size_t prefix_len = ptr - corrected;
            size_t inc_len = strlen(incorrect);
            size_t cor_len = strlen(correct);
            int before = (ptr == corrected) || !isalnum((unsigned char)ptr[-1]);
            int after = !isalnum((unsigned char)ptr[inc_len]);
            if (!(before && after)) {
                ptr += inc_len;
                continue;
            }
            size_t new_len = strlen(corrected) - inc_len + cor_len + 1;
            char *temp = malloc(new_len);
            if (!temp) break;
            snprintf(temp, new_len, "%.*s%s%s",
                     (int)prefix_len, corrected, correct, ptr + inc_len);
            free(corrected);
            corrected = temp;
        }
    }

    return corrected;
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
            // Check word boundaries for whole word replacement
            size_t offset = (size_t)(found - result);
            size_t bad_len = strlen(bad);
            size_t sugg_len = strlen(sugg);
            int before = (found == result) || !isalnum((unsigned char)found[-1]);
            int after = !isalnum((unsigned char)found[bad_len]);
            if (!(before && after)) {
                found += bad_len;
                continue;
            }

            size_t newlen = strlen(result) - bad_len + sugg_len + 1;
            char *temp = malloc(newlen);
            if (!temp) {
                free(result);
                return NULL;
            }

            snprintf(temp, newlen, "%.*s%s%s",
                     (int)offset, result, sugg, found + bad_len);
            free(result);
            result = temp;
        }
    }

    return result;
}

// detection helper

static int soap_detect_patterns(const char *text, const char **patterns) {
    if (!text || !patterns) return 0;

    for (size_t i = 0; patterns[i] != NULL; i++) {
        if (custom_strcasestr(text, patterns[i])) {
            return 1; // Pattern found
        }
    }
    return 0; // No match
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

int fossil_io_soap_detect_bot(const char *text) {
    return soap_detect_patterns(text, SOAP_BOT_PATTERNS);
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

int fossil_io_soap_detect_neutral(const char *text) {
    return soap_detect_patterns(text, SOAP_NEUTRAL_PATTERNS);
}

int fossil_io_soap_detect_hype(const char *text) {
    return soap_detect_patterns(text, SOAP_HYPE_PATTERNS);
}

int fossil_io_soap_detect_quality(const char *text) {
    return soap_detect_patterns(text, SOAP_QUALITY_PATTERNS);
}

int fossil_io_soap_detect_political(const char *text) {
    return soap_detect_patterns(text, SOAP_POLITICAL_PATTERNS);
}

int fossil_io_soap_detect_conspiracy(const char *text) {
    return soap_detect_patterns(text, SOAP_CONSPIRACY_PATTERNS);
}

int fossil_io_soap_detect_marketing(const char *text) {
    return soap_detect_patterns(text, SOAP_MARKETING_PATTERNS);
}

int fossil_io_soap_detect_technobabble(const char *text) {
    return soap_detect_patterns(text, SOAP_TECHNOBABBLE_PATTERNS);
}

/**
 * @brief Filter text by replacing words/phrases matching any pattern (comma-separated) with '*'.
 *        Patterns support '*' and '?' wildcards, case-insensitive.
 */
char *fossil_io_soap_filter(const char *patterns, const char *text) {
    if (!patterns || !text) return NULL;

    // Copy patterns string to safely tokenize
    char patterns_buf[512];
    strncpy(patterns_buf, patterns, sizeof(patterns_buf) - 1);
    patterns_buf[sizeof(patterns_buf) - 1] = '\0';

    // Build pattern array
    const char *pattern_arr[MAX_CUSTOM_FILTERS + 1];
    size_t pat_count = 0;
    char *token = strtok(patterns_buf, ",");
    while (token && pat_count < MAX_CUSTOM_FILTERS) {
        // Trim leading/trailing spaces
        while (*token == ' ') token++;
        size_t len = strlen(token);
        while (len > 0 && token[len - 1] == ' ') token[--len] = '\0';
        pattern_arr[pat_count++] = token;
        token = strtok(NULL, ",");
    }
    pattern_arr[pat_count] = NULL;

    size_t len = strlen(text);
    char *output = (char *)malloc(len * 2 + 1);
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
            int matched = 0;
            if (word_idx > 0) {
                for (size_t j = 0; pattern_arr[j] != NULL; j++) {
                    if (fossil_io_soap_simple_regex_match(word, pattern_arr[j])) {
                        matched = 1;
                        break;
                    }
                }
            }
            if (matched) {
                for (size_t j = 0; j < word_idx; j++) output[out_idx++] = '*';
            } else {
                for (size_t j = 0; j < word_idx; j++) output[out_idx++] = word[j];
            }
            output[out_idx++] = text[i];
            word_idx = 0;
        }
    }
    word[word_idx] = '\0';
    int matched = 0;
    if (word_idx > 0) {
        for (size_t j = 0; pattern_arr[j] != NULL; j++) {
            if (fossil_io_soap_simple_regex_match(word, pattern_arr[j])) {
                matched = 1;
                break;
            }
        }
    }
    if (matched) {
        for (size_t j = 0; j < word_idx; j++) output[out_idx++] = '*';
    } else {
        for (size_t j = 0; j < word_idx; j++) output[out_idx++] = word[j];
    }
    output[out_idx] = '\0';
    return output;
}
