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
#include "fossil/io/soap.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdio.h>

// LOOKUP MAP

typedef struct {
    char *processed_text;                  // sanitized / normalized / grammar-corrected
    char *summary;                         // optional summary
    fossil_io_soap_scores_t scores;        // readability / clarity / quality
    fossil_io_soap_grammar_style_t style;  // grammar / style

    /* ================= Detectors ================= */
    struct {
        /* word-level */
        int brain_rot;
        int leet;

        /* sentence-level */
        int spam;
        int ragebait;
        int clickbait;
        int bot;
        int marketing;
        int technobabble;
        int hype;
        int political;
        int offensive;
        int misinfo;      // shorthand for misinformation
        int morse;

        /* document-level */
        int propaganda;
        int conspiracy;

        /* stylistic / behavioral */
        int formal;
        int casual;
        int sarcasm;
        int neutral;
        int aggressive;
        int emotional;
        int passive_aggressive;

        /* structural analysis (redundancy, cohesion, repetition) */
        int redundant_sentences;
        int poor_cohesion;
        int repeated_words;

    } flags;
} fossil_io_soap_result_verbose_t;

typedef struct { const char *pattern; } pattern_t;

// ========================= SPAM PATTERNS =========================
static const pattern_t spam_patterns[] = {
    { "buy now" }, { "click here" }, { "free gift" }, { "subscribe" },
    { "limited offer" }, { "act now" }, { "instant access" },
    { "order today" }, { "exclusive deal" }, { "don't miss out" },
    { "special promotion" }, { "save big" }, { "offer ends soon" },
    { "get your free" }, { "risk free" }, { "join now" }, { "bonus" },
    { "money back" }, { "claim your prize" }, { "hot deal" },
    { "best price" }, { "lowest price" }, { "order now" }, { "urgent" },
    { "today only" }, { "final chance" }, { "exclusive access" },
    { "secret deal" }, { "free trial" }, { "sign up today" }, { "claim now" },
    { "free shipping" }, { "mega discount" }, { "limited stock" }, { "special bonus" },
    { "cash reward" }, { "earn money" }, { "work from home" }, { "get rich" },
    { "fast cash" }, { "profit now" }, { "exclusive membership" }, { "offer expires" },
    { "exclusive content" }, { "free download" }, { "instant win" }, { "prize inside" },
    { "register free" }, { "money saving" }, { "exclusive coupon" }, { "one-time offer" },
    { "bonus gift" }, { "super deal" }, { "limited edition" }, { "exclusive bonus" },
    { "big savings" }, { "act fast" }, { "click to claim" }, { "get access" },
    { "amazing offer" }, { "deal alert" }, { "hot offer" }, { "free voucher" },
    { "apply now" }, { "exclusive promotion" }, { "today's deal" }, { "instant offer" },
    { "urgent offer" }, { "reward points" }, { "exclusive giveaway" }, { "free subscription" },
    { "unlock now" }, { "best deal" }, { "fast offer" }, { "money making" },
    { "limited opportunity" }, { "special access" }, { "claim gift" }, { "instant reward" },
    { "exclusive deal today" }, { "cash bonus" }, { "bonus reward" }, { "top offer" },
    { "mega offer" }, { "discount coupon" }, { "free trial offer" }, { "secret offer" },
    { "one day only" }, { "hurry" }, { "final offer" }, { "special discount" },
    { "cash prize" }, { "exclusive chance" }, { "register now" }, { "big bonus" },
    { "limited availability" }, { "exclusive access code" }, { "free access" }, { "hot sale" },
    { "get it now" }, { "special offer code" }, { "exclusive bonus deal" }, { "fast track" },
    { "instant access code" }, { "limited edition gift" }, { "exclusive prize" }, { "amazing deal" },
    { "cash back" }, { "super offer" }, { "exclusive membership access" }, { "hot discount" },
    { "grab now" }, { "limited promotion" }, { "today only offer" }, { "exclusive opportunity" },
    { "special reward" }, { "claim your bonus" }, { "urgent deal" }, { "biggest discount" },
    { "free gift card" }, { "final chance offer" }, { "exclusive voucher" }, { "super discount" },
    { NULL }
};

// ========================= RAGEBAIT PATTERNS =========================
static const pattern_t ragebait_patterns[] = {
    { "you won't believe" }, { "shocking" }, { "outrageous" }, { "unbelievable" }, { "infuriating" },
    { "angry" }, { "furious" }, { "disgusting" }, { "ridiculous" }, { "scandal" },
    { "horrifying" }, { "outrage" }, { "outraged" }, { "triggered" }, { "furor" },
    { "angry reaction" }, { "outrageous claim" }, { "outrageous story" }, { "unacceptable" },
    { "outrageous behavior" }, { "shocking truth" }, { "unbelievable act" }, { "inexcusable" },
    { "angry mob" }, { "furious debate" }, { "disgusting act" }, { "ridiculous claim" }, { "shocking event" },
    { "infuriating story" }, { "outrageous incident" }, { "unbelievable event" }, { "horrifying act" },
    { "furious reaction" }, { "outrageous remark" }, { "triggering statement" }, { "infuriating post" },
    { "scandalous behavior" }, { "outraged citizens" }, { "outrageous opinion" }, { "disgusting statement" },
    { "ridiculous act" }, { "unbelievable behavior" }, { "shocking revelation" }, { "unacceptable act" },
    { "outrageous attack" }, { "furious argument" }, { "angry post" }, { "infuriating video" }, { "outrageous content" },
    { "ridiculous claim" }, { "horrifying story" }, { "triggered audience" }, { "outrageous rant" },
    { "furious post" }, { "disgusting action" }, { "outrageous content" }, { "shocking message" },
    { "angry rant" }, { "outrageous content" }, { "ridiculous argument" }, { "infuriating opinion" },
    { "unbelievable statement" }, { "horrifying content" }, { "outraged viewers" }, { "furious commentary" },
    { "triggered audience" }, { "scandalous statement" }, { "outrageous post" }, { "infuriating message" },
    { "ridiculous video" }, { "disgusting content" }, { "unacceptable behavior" }, { "outrageous video" },
    { "angry commentary" }, { "furious reaction" }, { "shocking opinion" }, { "triggered post" }, { "outrageous action" },
    { "ridiculous message" }, { "infuriating video" }, { "furious statement" }, { "outrageous comment" },
    { "shocking post" }, { "angry message" }, { "unbelievable content" }, { "disgusting video" }, { "outrageous tweet" },
    { "ridiculous commentary" }, { "furious post" }, { "infuriating action" }, { "outrageous tweet" },
    { "horrifying video" }, { "triggered comment" }, { "outrageous content" }, { "shocking message" },
    { "unbelievable tweet" }, { "furious comment" }, { "angry post" }, { "outrageous content" },
    { "ridiculous content" }, { "infuriating post" }, { "triggered tweet" }, { NULL }
};

// ========================= CLICKBAIT PATTERNS (MAXIMIZED) =========================
static const pattern_t clickbait_patterns[] = {
    { "you won't believe" }, { "this is why" }, { "what happens next" },
    { "the reason is" }, { "will shock you" }, { "revealed" }, { "top 10" },
    { "number one" }, { "amazing" }, { "secret" }, { "never guess" }, { "incredible" },
    { "shocking" }, { "see why" }, { "you must know" }, { "don't miss" }, { "unbelievable" },
    { "mind blown" }, { "this trick" }, { "life hack" }, { "what they don't want you to know" },
    { "you need this" }, { "before you die" }, { "must see" }, { "everyone is talking about" },
    { "game changer" }, { "epic" }, { "surprising" }, { "this works" }, { "watch this" },
    { "amazing fact" }, { "shocking truth" }, { "revealing" }, { "you'll never guess" },
    { "crazy" }, { "you have to see" }, { "hidden" }, { "astonishing" }, { "exclusive" },
    { "catch this" }, { "once in a lifetime" }, { "incredible story" }, { "jaw dropping" },
    { "life changing" }, { "you won't believe what happened" }, { "secret revealed" },
    { "top secret" }, { "must know" }, { "before it's too late" }, { "what happens when" },
    { "amazing discovery" }, { "unseen" }, { "mind blowing fact" }, { "shocking event" },
    { "this will blow your mind" }, { "what happened next will shock you" }, { "the truth revealed" },
    { "you'll be surprised" }, { "find out why" }, { "the shocking reason" }, { "the real story" },
    { "the truth behind" }, { "you can't miss this" }, { "must watch" }, { "the ultimate guide" },
    { "the best ever" }, { "the most amazing" }, { "the secret to" }, { "the real reason" },
    { "the shocking secret" }, { "the truth about" }, { "the one trick" }, { "the only way" },
    { "the best kept secret" }, { "the shocking truth" }, { "the untold story" }, { "the hidden truth" },
    { "the secret nobody tells you" }, { "the secret everyone is talking about" },
    { "the secret to success" }, { "the secret revealed" }, { "the shocking discovery" },
    { "the secret ingredient" }, { "the secret formula" }, { "the secret weapon" },
    { "the secret behind the success" }, { "the shocking confession" }, { "the shocking admission" },
    { "the shocking revelation" }, { "the shocking expose" }, { "the shocking expose revealed" },
    { "the shocking expose exposed" }, { "the shocking expose uncovered" }, { "the shocking expose unmasked" },
    { "the shocking expose unmasked revealed" }, { "the shocking expose unmasked exposed" },
    { "the shocking expose unmasked uncovered" }, { "the shocking expose unmasked unmasked" },
    { "the shocking expose unmasked unmasked revealed" }, { "the shocking expose unmasked unmasked exposed" },
    { "the shocking expose unmasked unmasked uncovered" }, { "the shocking expose unmasked unmasked unmasked" },
    { NULL }
};

// ========================= BOT PATTERNS (MAXIMIZED) =========================
static const pattern_t bot_patterns[] = {
    { "buy now" }, { "click here" }, { "subscribe" }, { "free gift" },
    { "limited offer" }, { "act now" }, { "instant access" }, { "order today" },
    { "exclusive deal" }, { "don't miss out" }, { "special promotion" }, { "save big" },
    { "offer ends soon" }, { "get your free" }, { "risk free" }, { "join now" },
    { "bonus" }, { "money back" }, { "claim your prize" }, { "hot deal" },
    { "best price" }, { "lowest price" }, { "order now" }, { "urgent" },
    { "today only" }, { "final chance" }, { "exclusive access" }, { "secret deal" },
    { "free trial" }, { "sign up today" }, { "claim now" }, { "free shipping" },
    { "register free" }, { "money saving" }, { "exclusive coupon" }, { "one-time offer" },
    { "bonus gift" }, { "super deal" }, { "limited edition" }, { "exclusive bonus" },
    { "big savings" }, { "act fast" }, { "click to claim" }, { "get access" },
    { "amazing offer" }, { "deal alert" }, { "hot offer" }, { "free voucher" },
    { "apply now" }, { "exclusive promotion" }, { "today's deal" }, { "instant offer" },
    { "urgent offer" }, { "reward points" }, { "exclusive giveaway" }, { "free subscription" },
    { "unlock now" }, { "best deal" }, { "fast offer" }, { "money making" },
    { "limited opportunity" }, { "special access" }, { "claim gift" }, { "instant reward" },
    { "exclusive deal today" }, { "cash bonus" }, { "bonus reward" }, { "top offer" },
    { "mega offer" }, { "discount coupon" }, { "free trial offer" }, { "secret offer" },
    { "one day only" }, { "hurry" }, { "final offer" }, { "special discount" },
    { "cash prize" }, { "exclusive chance" }, { "register now" }, { "big bonus" },
    { "limited availability" }, { "exclusive access code" }, { "free access" }, { "hot sale" },
    { "get it now" }, { "special offer code" }, { "exclusive bonus deal" }, { "fast track" },
    { "instant access code" }, { "limited edition gift" }, { "exclusive prize" }, { "amazing deal" },
    { "cash back" }, { "super offer" }, { "exclusive membership access" }, { "hot discount" },
    { "grab now" }, { "limited promotion" }, { "today only offer" }, { "exclusive opportunity" },
    { "special reward" }, { "claim your bonus" }, { "urgent deal" }, { "biggest discount" },
    { "free gift card" }, { "final chance offer" }, { "exclusive voucher" }, { "super discount" },
    { NULL }
};

// ========================= MARKETING PATTERNS (MAXIMIZED) =========================
static const pattern_t marketing_patterns[] = {
    { "limited time" }, { "special offer" }, { "act now" }, { "exclusive" },
    { "sale ends soon" }, { "best deal" }, { "discount" }, { "save now" },
    { "offer expires" }, { "today only" }, { "final chance" }, { "bonus gift" },
    { "unlock savings" }, { "order today" }, { "get it now" }, { "exclusive access" },
    { "mega discount" }, { "special promotion" }, { "cash reward" }, { "register now" },
    { "limited time offer" }, { "exclusive deal" }, { "save big" }, { "lowest price" },
    { "free shipping" }, { "money back" }, { "risk free" }, { "bonus" },
    { "exclusive coupon" }, { "super deal" }, { "limited edition" }, { "exclusive bonus" },
    { "big savings" }, { "act fast" }, { "click to claim" }, { "get access" },
    { "amazing offer" }, { "deal alert" }, { "hot offer" }, { "free voucher" },
    { "apply now" }, { "exclusive promotion" }, { "today's deal" }, { "instant offer" },
    { "urgent offer" }, { "reward points" }, { "exclusive giveaway" }, { "free subscription" },
    { "unlock now" }, { "fast offer" }, { "money making" }, { "limited opportunity" },
    { "special access" }, { "claim gift" }, { "instant reward" }, { "exclusive deal today" },
    { "cash bonus" }, { "bonus reward" }, { "top offer" }, { "mega offer" },
    { "discount coupon" }, { "free trial offer" }, { "secret offer" }, { "one day only" },
    { "hurry" }, { "final offer" }, { "special discount" }, { "cash prize" },
    { "exclusive chance" }, { "big bonus" }, { "limited availability" }, { "exclusive access code" },
    { "free access" }, { "hot sale" }, { "special offer code" }, { "exclusive bonus deal" },
    { "fast track" }, { "instant access code" }, { "limited edition gift" }, { "exclusive prize" },
    { "amazing deal" }, { "cash back" }, { "super offer" }, { "exclusive membership access" },
    { "hot discount" }, { "grab now" }, { "limited promotion" }, { "today only offer" },
    { "exclusive opportunity" }, { "special reward" }, { "claim your bonus" }, { "urgent deal" },
    { "biggest discount" }, { "free gift card" }, { "final chance offer" }, { "exclusive voucher" },
    { "super discount" }, { NULL }
};

// ========================= TECHNOBABBLE PATTERNS (MAXIMIZED) =========================
static const pattern_t technobabble_patterns[] = {
    { "quantum entanglement" }, { "machine learning" }, { "deep neural network" },
    { "blockchain" }, { "AI-driven" }, { "cloud computing" }, { "hyperconverged" },
    { "cyber-physical" }, { "nanotechnology" }, { "augmented reality" }, { "edge computing" },
    { "IoT-enabled" }, { "autonomous system" }, { "next-gen" }, { "distributed ledger" },
    { "big data" }, { "artificial intelligence" }, { "neural net" }, { "virtual reality" },
    { "smart contract" }, { "tokenization" }, { "decentralized" }, { "cryptocurrency" },
    { "internet of things" }, { "5G" }, { "metaverse" }, { "digital twin" },
    { "self-driving" }, { "robotics" }, { "bioinformatics" }, { "genomics" },
    { "quantum computing" }, { "blockchain technology" }, { "AI-powered" }, { "data lake" },
    { "serverless" }, { "microservices" }, { "containerization" }, { "orchestration" },
    { "edge AI" }, { "fog computing" }, { "digital transformation" }, { "smart city" },
    { "predictive analytics" }, { "prescriptive analytics" }, { "data mining" },
    { "data science" }, { "data engineering" }, { "cloud-native" }, { "multi-cloud" },
    { "hybrid cloud" }, { "zero trust" }, { "blockchain-enabled" }, { "AI-first" },
    { NULL }
};

// ========================= HYPE PATTERNS (MAXIMIZED) =========================
static const pattern_t hype_patterns[] = {
    { "amazing" }, { "incredible" }, { "epic" }, { "unbelievable" }, { "mind-blowing" },
    { "groundbreaking" }, { "revolutionary" }, { "next level" }, { "must see" },
    { "once in a lifetime" }, { "life changing" }, { "game changer" }, { "exclusive" },
    { "jaw dropping" }, { "life altering" }, { "legendary" }, { "phenomenal" },
    { "unprecedented" }, { "record-breaking" }, { "world-class" }, { "state-of-the-art" },
    { "cutting edge" }, { "breakthrough" }, { "unmatched" }, { "unrivaled" },
    { "unparalleled" }, { "best ever" }, { "never before" }, { "must have" },
    { "can't miss" }, { "top rated" }, { "blockbuster" }, { "sensational" },
    { "spectacular" }, { "mind blowing" }, { "outstanding" }, { "astounding" },
    { "stunning" }, { "breathtaking" }, { "extraordinary" }, { "remarkable" },
    { "unreal" }, { "fantastic" }, { "superb" }, { "magnificent" }, { "marvelous" },
    { NULL }
};

// ========================= POLITICAL PATTERNS (MAXIMIZED) =========================
static const pattern_t political_patterns[] = {
    { "vote" }, { "policy" }, { "government" }, { "election" }, { "legislation" },
    { "reform" }, { "candidate" }, { "campaign" }, { "democracy" }, { "party" },
    { "protest" }, { "senate" }, { "congress" }, { "bill" }, { "law" },
    { "parliament" }, { "representative" }, { "constituent" }, { "lobby" },
    { "politician" }, { "political" }, { "governor" }, { "mayor" }, { "president" },
    { "prime minister" }, { "cabinet" }, { "minister" }, { "secretary" },
    { "referendum" }, { "ballot" }, { "poll" }, { "platform" }, { "manifesto" },
    { "coalition" }, { "majority" }, { "minority" }, { "opposition" }, { "incumbent" },
    { "constituency" }, { "district" }, { "jurisdiction" }, { "executive" },
    { "judiciary" }, { "legislature" }, { "bureaucracy" }, { "regulation" },
    { "statute" }, { "ordinance" }, { "decree" }, { "mandate" }, { "impeachment" },
    { NULL }
};

// ========================= OFFENSIVE PATTERNS (MAXIMIZED) =========================
static const pattern_t offensive_patterns[] = {
    { "idiot" }, { "stupid" }, { "dumb" }, { "fool" }, { "loser" },
    { "moron" }, { "hate" }, { "jerk" }, { "trash" }, { "scum" },
    { "nonsense" }, { "pathetic" }, { "worthless" }, { "disgusting" },
    { "ignorant" }, { "cretin" }, { "imbecile" }, { "retard" }, { "sucks" },
    { "garbage" }, { "ugly" }, { "fat" }, { "gross" }, { "stinks" },
    { "douche" }, { "bastard" }, { "asshole" }, { "prick" }, { "bitch" },
    { "slut" }, { "whore" }, { "dick" }, { "piss off" }, { "shut up" },
    { "kill yourself" }, { "kys" }, { "die" }, { "go to hell" }, { "freak" },
    { "weirdo" }, { "psycho" }, { "lunatic" }, { "nutcase" }, { "twat" },
    { "cunt" }, { "fuck" }, { "shit" }, { "bullshit" }, { "crap" },
    { NULL }
};

// ========================= PROPAGANDA PATTERNS (MAXIMIZED) =========================
static const pattern_t propaganda_patterns[] = {
    { "truth" }, { "freedom" }, { "patriot" }, { "justice" }, { "liberty" },
    { "hero" }, { "enemy" }, { "threat" }, { "corruption" }, { "defend" },
    { "protect" }, { "powerful" }, { "nation" }, { "agenda" }, { "righteous" },
    { "traitor" }, { "evil" }, { "glory" }, { "sacrifice" }, { "martyr" },
    { "victory" }, { "defeat" }, { "mission" }, { "cause" }, { "movement" },
    { "revolution" }, { "uprising" }, { "resistance" }, { "regime" }, { "dictator" },
    { "oppression" }, { "liberation" }, { "sovereignty" }, { "unity" }, { "solidarity" },
    { "loyalty" }, { "allegiance" }, { "honor" }, { "duty" }, { "sacred" },
    { "destiny" }, { "manifesto" }, { "propaganda" }, { "indoctrinate" }, { "brainwash" },
    { NULL }
};

// ========================= MISINFORMATION PATTERNS (MAXIMIZED) =========================
static const pattern_t misinformation_patterns[] = {
    { "fake news" }, { "hoax" }, { "false" }, { "misleading" }, { "rumor" },
    { "conspiracy" }, { "unverified" }, { "scam" }, { "fraud" }, { "deceptive" },
    { "disinformation" }, { "fabricated" }, { "untrue" }, { "debunked" }, { "myth" },
    { "urban legend" }, { "clickbait" }, { "sensationalized" }, { "out of context" },
    { "doctored" }, { "forged" }, { "faked" }, { "manipulated" }, { "deepfake" },
    { "false claim" }, { "false narrative" }, { "false report" }, { "false story" },
    { "false information" }, { "false statement" }, { "falsehood" }, { "misreport" },
    { "misquote" }, { "misattribute" }, { "misrepresent" }, { "misstate" }, { "misinform" },
    { "mislead" }, { "misinterpret" }, { "misconstrue" }, { "miscommunicate" },
    { NULL }
};

// ========================= CONSPIRACY PATTERNS (MAXIMIZED) =========================
static const pattern_t conspiracy_patterns[] = {
    { "hidden agenda" }, { "secret plan" }, { "shadow government" }, { "cover-up" },
    { "inside job" }, { "elite" }, { "controlled opposition" }, { "false flag" },
    { "government plot" }, { "manipulation" }, { "deep state" }, { "puppet master" },
    { "new world order" }, { "illuminati" }, { "mind control" }, { "chemtrails" },
    { "aliens" }, { "ufo" }, { "area 51" }, { "moon landing hoax" }, { "flat earth" },
    { "reptilian" }, { "secret society" }, { "big pharma" }, { "big tech" },
    { "big oil" }, { "big brother" }, { "mass surveillance" }, { "plandemic" },
    { "crisis actor" }, { "hoax" }, { "cover up" }, { "deep cover" }, { "psyop" },
    { "psy ops" }, { "psy-ops" }, { "psy ops operation" }, { "psy-ops operation" },
    { "psy ops campaign" }, { "psy-ops campaign" }, { "psy ops program" }, { "psy-ops program" },
    { NULL }
};

// ========================= FORMAL PATTERNS (MAXIMIZED) =========================
static const pattern_t formal_patterns[] = {
    { "therefore" }, { "moreover" }, { "hence" }, { "thus" }, { "accordingly" },
    { "in conclusion" }, { "furthermore" }, { "consequently" }, { "as a result" }, { "in addition" },
    { "notwithstanding" }, { "nevertheless" }, { "nonetheless" }, { "whereas" }, { "hereby" },
    { "herein" }, { "herewith" }, { "heretofore" }, { "therein" }, { "therewith" },
    { "thereafter" }, { "thereupon" }, { "thereby" }, { "wherein" }, { "whereof" },
    { "whereupon" }, { "whereby" }, { "pursuant to" }, { "in accordance with" },
    { "insofar as" }, { "inasmuch as" }, { "in the event that" }, { "in the absence of" },
    { "in the presence of" }, { "in the course of" }, { "in the context of" },
    { "in the light of" }, { "in the interest of" }, { "in the process of" },
    { "in the case of" }, { "in the matter of" }, { "in the opinion of" },
    { NULL }
};

// ========================= CASUAL PATTERNS (MAXIMIZED) =========================
static const pattern_t casual_patterns[] = {
    { "hey" }, { "lol" }, { "omg" }, { "btw" }, { "idk" },
    { "yep" }, { "nah" }, { "cool" }, { "awesome" }, { "what's up" },
    { "sup" }, { "yo" }, { "dude" }, { "bro" }, { "fam" }, { "bruh" },
    { "lmao" }, { "rofl" }, { "haha" }, { "hehe" }, { "xd" }, { "ikr" },
    { "tbh" }, { "fyi" }, { "imo" }, { "imho" }, { "smh" }, { "fml" },
    { "ily" }, { "nope" }, { "yolo" }, { "meh" }, { "ugh" }, { "yay" },
    { "yesss" }, { "noooo" }, { "yaaaas" }, { "bruh moment" }, { "oh no" },
    { "omfg" }, { "wtf" }, { "wtfv" }, { "lmaoed" }, { "lmaoing" },
    { "haha lol" }, { "lol haha" }, { "xd lol" }, { "lol xd" }, { "fml lol" },
    { "smdh" }, { "sksksk" }, { "and i oop" }, { "yeet" }, { "yeeted" },
    { "yeeting" }, { "uwu" }, { "owo" }, { "rawr" }, { "rawr x3" },
    { NULL }
};

// ========================= SARCASM PATTERNS (MAXIMIZED) =========================
static const pattern_t sarcasm_patterns[] = {
    { "yeah right" }, { "as if" }, { "sure" }, { "oh great" }, { "fantastic" },
    { "brilliant" }, { "wow" }, { "amazing" }, { "just perfect" }, { "totally" },
    { "obviously" }, { "of course" }, { "nice job" }, { "good luck with that" },
    { "what a surprise" }, { "how original" }, { "so helpful" }, { "thanks a lot" },
    { "just what I needed" }, { "couldn't be better" }, { "love it" }, { "can't wait" },
    { "so excited" }, { "best ever" }, { "what a genius" }, { "so smart" },
    { "how clever" }, { "so funny" }, { "hilarious" }, { "great idea" },
    { NULL }
};

// ========================= NEUTRAL PATTERNS (MAXIMIZED) =========================
static const pattern_t neutral_patterns[] = {
    { "okay" }, { "alright" }, { "yes" }, { "no" }, { "maybe" },
    { "fine" }, { "good" }, { "bad" }, { "average" }, { "normal" },
    { "regular" }, { "standard" }, { "typical" }, { "usual" }, { "ordinary" },
    { "so-so" }, { "meh" }, { "not bad" }, { "not good" }, { "could be better" },
    { "could be worse" }, { "nothing special" }, { "not sure" }, { "not certain" },
    { "uncertain" }, { "possibly" }, { "perhaps" }, { "potentially" }, { "likely" },
    { "unlikely" }, { "neutral" }, { "indifferent" }, { "unbiased" }, { "impartial" },
    { NULL }
};

// ========================= AGGRESSIVE PATTERNS (MAXIMIZED) =========================
static const pattern_t aggressive_patterns[] = {
    { "attack" }, { "destroy" }, { "fight" }, { "kill" }, { "smash" },
    { "crush" }, { "annihilate" }, { "rage" }, { "strike" }, { "obliterate" },
    { "dominate" }, { "conquer" }, { "invade" }, { "overpower" }, { "subdue" },
    { "eliminate" }, { "eradicate" }, { "wipe out" }, { "take down" }, { "beat" },
    { "defeat" }, { "punish" }, { "hurt" }, { "harm" }, { "maul" }, { "batter" },
    { "brawl" }, { "clash" }, { "assault" }, { "barrage" }, { "bombard" },
    { "ambush" }, { "charge" }, { "storm" }, { "bust" }, { "wreck" },
    { "wreck havoc" }, { "tear apart" }, { "rip apart" }, { "rip to shreds" },
    { NULL }
};

// ========================= EMOTIONAL PATTERNS (MAXIMIZED) =========================
static const pattern_t emotional_patterns[] = {
    { "happy" }, { "sad" }, { "angry" }, { "excited" }, { "fear" },
    { "joy" }, { "love" }, { "hate" }, { "surprised" }, { "ecstatic" },
    { "depressed" }, { "anxious" }, { "nervous" }, { "thrilled" }, { "delighted" },
    { "miserable" }, { "furious" }, { "elated" }, { "content" }, { "disappointed" },
    { "frustrated" }, { "overjoyed" }, { "heartbroken" }, { "grateful" }, { "resentful" },
    { "jealous" }, { "envious" }, { "ashamed" }, { "proud" }, { "guilty" },
    { "hopeful" }, { "hopeless" }, { "relieved" }, { "stressed" }, { "calm" },
    { "peaceful" }, { "confused" }, { "bored" }, { "lonely" }, { "scared" },
    { NULL }
};

// ========================= PASSIVE_AGGRESSIVE PATTERNS (MAXIMIZED) =========================
static const pattern_t passive_aggressive_patterns[] = {
    { "sure" }, { "whatever" }, { "fine" }, { "if you say so" }, { "okay then" },
    { "no problem" }, { "as you wish" }, { "right" }, { "yeah, sure" },
    { "if that's what you want" }, { "I guess" }, { "if you insist" },
    { "not my problem" }, { "do what you want" }, { "go ahead" }, { "be my guest" },
    { "I don't care" }, { "it's up to you" }, { "suit yourself" }, { "have it your way" },
    { "whatever you say" }, { "if it makes you happy" }, { "I'm fine" },
    { "I'm okay" }, { "I'm good" }, { "it's fine" }, { "it's okay" },
    { "it's whatever" }, { "that's fine" }, { "that's okay" }, { "that's whatever" },
    { NULL }
};

//
/* ============================================================================
 * Snowflake patterns table (MAXIMIZED)
 * ============================================================================ */
static const pattern_t snowflake_patterns[] = {
    { "triggered" }, { "microaggression" }, { "safe space" }, { "privilege" },
    { "problematic" }, { "offended" }, { "snowflake" }, { "fragile" },
    { "woke" }, { "cancel culture" }, { "toxic" }, { "gaslighting" },
    { "trauma" }, { "oppressed" }, { "victim" }, { "identity politics" },
    { "intersectional" }, { "systemic" }, { "inclusion" }, { "diversity" },
    { "equity" }, { "ally" }, { "marginalized" }, { "lived experience" },
    { "call out" }, { "callout" }, { "safe zone" }, { "emotional labor" },
    { "tone policing" }, { "cultural appropriation" }, { "trigger warning" },
    { "unpack" }, { "problematic fave" }, { "problematic favorite" },
    { "privileged" }, { "decolonize" }, { "inclusivity" }, { "oppression" },
    { "systemic bias" }, { "implicit bias" }, { "white fragility" },
    { "white privilege" }, { "check your privilege" }, { "lived experiences" },
    { "identity" }, { "safe environment" }, { "emotional support" },
    { "emotional damage" }, { "emotional distress" }, { "emotional safety" },
    { "emotional wellbeing" }, { "emotional well-being" }, { "emotional health" },
    { "emotional needs" }, { "emotional response" }, { "emotional trauma" },
    { "emotional abuse" }, { "emotional burden" }, { "emotional exhaustion" },
    { "emotional intelligence" }, { "emotional resilience" }, { "emotional sensitivity" },
    { "emotional support animal" }, { "emotional trigger" }, { "emotional vulnerability" },
    { NULL }
};

/* ============================================================================
 * BrainRot patterns table (FULL, UNTRIMMED, BRACE-SAFE)
 * ============================================================================ */
static const pattern_t brain_rot_patterns[] = {
    {"lol"}, {"lmao"}, {"rofl"}, {"bruh"}, {"wtf"}, {"omg"}, {"haha"}, {"hehe"}, {"xd"}, {"xdxd"},
    {"yo"}, {"hey"}, {"sup"}, {"what's up"}, {"man"}, {"dude"}, {"bro"}, {"broski"}, {"homie"}, {"fam"},
    {"yolo"}, {"smh"}, {"fml"}, {"idk"}, {"ikr"}, {"tbh"}, {"ily"}, {"ily2"}, {"omfg"}, {"wtfv"},
    {"heh"}, {"meh"}, {"ugh"}, {"aww"}, {"yay"}, {"yayyy"}, {"yesss"}, {"nope"}, {"nah"}, {"yep"},
    {"bruhh"}, {"brooo"}, {"duuuude"}, {"lolz"}, {"lols"}, {"lul"}, {"lulz"}, {"hahaaha"}, {"roflmao"}, {"lmfao"},
    {"kek"}, {"pog"}, {"poggers"}, {"pogchamp"}, {"rip"}, {"gg"}, {"ggwp"}, {"rekt"}, {"owned"}, {"clap"},
    {"clapclap"}, {"facepalm"}, {"yikes"}, {"oops"}, {"oopsie"}, {"ayyy"}, {"ayyy lmao"}, {"hehehe"}, {"yayyyyy"}, {"nooooo"},
    {"yaaaas"}, {"bruh moment"}, {"oh no"}, {"ikr"}, {"omgosh"}, {"lmaoed"}, {"lmaoing"}, {"haha lol"}, {"lol haha"}, {"xd lol"},
    {"lol xd"}, {"fml lol"}, {"smdh"}, {"sksksk"}, {"and i oop"}, {"yeet"}, {"yeeted"}, {"yeeting"}, {"uwu"}, {"owo"},
    {"uwu uwu"}, {"owo owo"}, {"rawr"}, {"rawr x3"}, {">:("}, {"<3"}, {":3"}, {"-_-"}, {"^_^"}, {"T_T"},
    {";-;"}, {">:O"}, {"-.-"}, {">:("}, {"D:"}, {"XD"}, {"xD"}, {"XD XD"}, {">:|"}, {"-.-'"},
    {"-__-"}, {"o.O"}, {"O.o"}, {"lolwut"}, {"lold"}, {"lolol"}, {"lololol"}, {"haha lmao"}, {"roflol"}, {"roflolmao"},
    {"wtf lol"}, {"no cap"}, {"sus"}, {"sussy"}, {"sus af"}, {"based"}, {"cringe"}, {"bet"}, {"lit"}, {"fire"},
    {"goat"}, {"slay"}, {"flex"}, {"vibe"}, {"vibing"}, {"mood"}, {"big mood"}, {"dead"}, {"i'm dead"}, {"rip me"},
    {"lowkey"}, {"highkey"}, {"salty"}, {"thirsty"}, {"shade"}, {"shook"}, {"stan"}, {"simp"}, {"snacc"}, {"snack"},
    {"bop"}, {"bussin"}, {"drip"}, {"drippy"}, {"fam jam"}, {"fam squad"}, {"fam bam"}, {"fam fam"}, {"famalam"}, {"fam squad"},
    {"on fleek"}, {"fleek"}, {"savage"}, {"savage af"}, {"extra"}, {"basic"}, {"woke"}, {"cancelled"}, {"cancel"}, {"clout"},
    {"clout chaser"}, {"receipts"}, {"tea"}, {"spill the tea"}, {"spill tea"}, {"big yikes"}, {"big oof"}, {"oof"}, {"oop"},
    {"no chill"}, {"deadass"}, {"facts"}, {"big facts"}, {"periodt"}, {"period"}, {"sis"}, {"sis bro"}, {"sis fam"}, {"sis queen"},
    {NULL}
};

/* ============================================================================
 * Internal helpers
 * ============================================================================ */

static char *dupstr(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s);
    char *r = (char*)malloc(n + 1);
    if (r) memcpy(r, s, n + 1);
    return r;
}

static void strtolower(char *s) {
    if (!s) return;
    for (; *s; s++) *s = (char)tolower((unsigned char)*s);
}

static int soap_is_abbrev(const char *s) {
    static const char *abbr[] = {
        "mr.", "mrs.", "dr.", "vs.", "etc.", "e.g.", "i.e.", NULL
    };
    for (int i = 0; abbr[i]; i++)
        if (strcasecmp(s, abbr[i]) == 0)
            return 1;
    return 0;
}

/* ============================================================================
 * Leetspeak normalization
 * ============================================================================ */

static char leet_map(char c) {
    switch(c) {
        case '4': case '@': return 'a';
        case '3': return 'e';
        case '1': return 'i';
        case '0': return 'o';
        case '5': case '$': return 's';
        case '7': return 't';
        default: return c;
    }
}

static void normalize_leet(char *s) {
    if (!s) return;
    for (; *s; s++) *s = leet_map(*s);
}

/* ============================================================================
 * Morse decoding (basic)
 * ============================================================================ */

typedef struct { const char *code; char c; } morse_t;

static const morse_t morse_table[] = {
    {".-", 'a'}, {"-...", 'b'}, {"-.-.", 'c'}, {"-..", 'd'},
    {".", 'e'}, {"..-.", 'f'}, {"--.", 'g'}, {"....", 'h'},
    {"..", 'i'}, {".---", 'j'}, {"-.-", 'k'}, {".-..", 'l'},
    {"--", 'm'}, {"-.", 'n'}, {"---", 'o'}, {".--.", 'p'},
    {"--.-", 'q'}, {".-.", 'r'}, {"...", 's'}, {"-", 't'},
    {"..-", 'u'}, {"...-", 'v'}, {".--", 'w'}, {"-..-", 'x'},
    {"-.--", 'y'}, {"--..", 'z'}, {NULL, 0}
};

static char morse_lookup(const char *code) {
    for (int i=0; morse_table[i].code; i++)
        if (strcmp(morse_table[i].code, code)==0) return morse_table[i].c;
    return '?';
}

static char *decode_morse(const char *text) {
    if (!text) return NULL;
    char *out = (char*)malloc(strlen(text)+1);
    size_t oi=0, bi=0;
    char buf[8];
    for (const char *p=text;;p++) {
        if (*p=='.'||*p=='-') buf[bi++]=*p;
        else {
            if(bi){buf[bi]=0; out[oi++]=morse_lookup(buf); bi=0;}
            if(*p==' '||*p=='/') out[oi++]=' ';
            if(!*p) break;
        }
    }
    out[oi]=0;
    return out;
}

/* ============================================================================
 * Sanitization
 * ============================================================================ */

char *fossil_io_soap_normalize(const char *text) {
    if (!text) return NULL;
    char *s = dupstr(text);
    normalize_leet(s);
    strtolower(s);
    return s;
}

char *fossil_io_soap_sanitize(const char *text) {
    if (!text) return NULL;
    char *s = dupstr(text);
    for(char *p=s; *p; p++) if((unsigned char)*p<32 && *p!='\n') *p=' ';
    char *norm = fossil_io_soap_normalize(s);
    free(s);
    return norm;
}

/* ============================================================================
 * Grammar & style analysis
 * ============================================================================ */

fossil_io_soap_grammar_style_t fossil_io_soap_analyze_grammar_style(const char *text) {
    fossil_io_soap_grammar_style_t r = {1,0,"neutral"};
    if(!text) return r;
    int passive=0, words=0;
    const char *p=text;
    while(*p){
        if(isspace((unsigned char)*p)) words++;
        if(!strncmp(p,"was ",4)||!strncmp(p,"were ",5)) passive++;
        p++;
    }
    if(words) r.passive_voice_pct = (passive*100)/words;
    if(strstr(text,"!")||strstr(text,"?")) r.style="emotional";
    else if(strstr(text,"therefore")||strstr(text,";")) r.style="formal";
    return r;
}

char *fossil_io_soap_correct_grammar(const char *text)
{
    if (!text) return NULL;

    // --- Begin context-aware grammar correction ---
    char *out = malloc(strlen(text) * 2 + 4);
    if (!out) return NULL;

    const char *p = text;
    char *q = out;

    int new_sentence = 1;
    int last_space = 0;
    char last_char = 0;

    int in_quote = 0;
    int in_paren = 0;
    int in_url = 0;
    int word_start = 1;

    // For abbreviation detection
    char prev_word[32] = {0};
    int prev_word_len = 0;

    struct {
        const char *from;
        const char *to;
    } contractions[] = {
        {" dont ", " don't "}, {" cant ", " can't "}, {" wont ", " won't "},
        {" isnt ", " isn't "}, {" arent ", " aren't "}, {" wasnt ", " wasn't "},
        {" werent ", " weren't "}, {" doesnt ", " doesn't "}, {" didnt ", " didn't "},
        {" hasnt ", " hasn't "}, {" havent ", " haven't "}, {" hadnt ", " hadn't "},
        {" couldnt ", " couldn't "}, {" wouldnt ", " wouldn't "}, {" shouldnt ", " shouldn't "},
        {" mustnt ", " mustn't "}, {" neednt ", " needn't "}, {" darent ", " daren't "},
        {" im ", " I'm "}, {" ive ", " I've "}, {" ill ", " I'll "}, {" id ", " I'd "},
        {" youre ", " you're "}, {" youve ", " you've "}, {" youll ", " you'll "},
        {" youd ", " you'd "}, {" hes ", " he's "}, {" hed ", " he'd "}, {" hell ", " he'll "},
        {" shes ", " she's "}, {" shed ", " she'd "}, {" shell ", " she'll "},
        {" its ", " it's "}, {" were ", " we're "}, {" theyre ", " they're "},
        {" theyve ", " they've "}, {" theyll ", " they'll "}, {" theyd ", " they'd "},
        {" thats ", " that's "}, {" theres ", " there's "}, {" whats ", " what's "},
        {" whos ", " who's "}, {" wheres ", " where's "}, {" whens ", " when's "},
        {" whys ", " why's "}, {" hows ", " how's "}, {" couldve ", " could've "},
        {" wouldve ", " would've "}, {" shouldve ", " should've "}, {" mightve ", " might've "},
        {" mustve ", " must've "}, {" mayve ", " may've "}, {" lets ", " let's "},
        {" thats ", " that's "}, {" theres ", " there's "}, {" dont.", " don't."},
        {" dont,", " don't,"}, {" cant.", " can't."}, {" cant,", " can't,"},
        {" wont.", " won't."}, {" wont,", " won't,"}, {NULL, NULL}
    };

    while (*p) {
        char c = *p++;

        // Track quote and paren state
        if (c == '"' || c == '\'') in_quote ^= 1;
        if (c == '(') in_paren++;
        if (c == ')') if (in_paren > 0) in_paren--;

        // URL detection (cheap, no regex)
        if (!in_url && (strncmp(p - 1, "http://", 7) == 0 ||
                        strncmp(p - 1, "https://", 8) == 0))
            in_url = 1;
        if (in_url && isspace((unsigned char)c))
            in_url = 0;

        // Normalize whitespace
        if (isspace((unsigned char)c)) {
            if (!last_space) {
                *q++ = ' ';
                last_space = 1;
            }
            word_start = 1;
            continue;
        }
        last_space = 0;

        // Word boundary and sentence word count
        if (isalpha((unsigned char)c) && word_start) {
            word_start = 0;
        }
        if (!isalpha((unsigned char)c))
            word_start = 1;

        // Abbreviation detection (look back for word before '.')
        if (c == '.' && prev_word_len > 0) {
            prev_word[prev_word_len] = 0;
        }

        // Capitalize start of sentences (with abbreviation/ellipsis/number awareness)
        int is_abbrev = 0;
        if (new_sentence && isalpha((unsigned char)c)) {
            // Check for abbreviation before
            if (prev_word_len > 0 && soap_is_abbrev(prev_word))
                is_abbrev = 1;
            if (!is_abbrev)
                c = (char)toupper((unsigned char)c);
            new_sentence = 0;
        }

        // Punctuation collapse, but smarter
        if (ispunct((unsigned char)c) && c == last_char) {
            if (c == '.' || c == '!' || c == '?')
                continue; // collapse
        }

        // Sentence boundary detection (punctuation-aware)
        if ((c == '.' || c == '!' || c == '?') && !in_quote) {
            // Decimal number: don't end sentence
            if (c == '.' && isdigit((unsigned char)*p))
                ;
            // Abbreviation: don't end sentence
            else if (is_abbrev)
                ;
            // Ellipsis: don't end sentence
            else if (c == '.' && *p == '.' && *(p+1) == '.')
                ;
            else
                new_sentence = 1;
        }

        // Context-aware contraction replacement (inline, not global)
        if (!in_quote && !in_url && !in_paren) {
            for (int i = 0; contractions[i].from; i++) {
                size_t len = strlen(contractions[i].from);
                if (strncmp(p - 1, contractions[i].from, len) == 0) {
                    memcpy(q - 1, contractions[i].to, strlen(contractions[i].to));
                    p += len - 1;
                    q += strlen(contractions[i].to) - 1;
                    break;
                }
            }
        }

        *q++ = c;
        last_char = c;

        // Track previous word for abbreviation check
        if (isalpha((unsigned char)c)) {
            if (prev_word_len < (int)sizeof(prev_word) - 1)
                prev_word[prev_word_len++] = (char)tolower((unsigned char)c);
        } else {
            prev_word_len = 0;
        }
    }

    // Ensure terminal punctuation
    if (q > out && !ispunct((unsigned char)q[-1])) {
        *q++ = '.';
    }

    *q = '\0';

    // --- End context-aware grammar correction ---
    return out;
}

/* ============================================================================
 * Readability / scoring
 * ============================================================================ */

fossil_io_soap_scores_t fossil_io_soap_score(const char *text){
    fossil_io_soap_scores_t s={70,70,70};
    if(!text) return s;
    size_t len = strlen(text);
    if(len<40) s.readability-=10;
    if(strchr(text,'\n')) s.clarity+=10;
    if(!strstr(text,"!!!")) s.quality+=10;
    return s;
}

const char *fossil_io_soap_readability_label(int score){
    if(score>80) return "excellent";
    if(score>60) return "good";
    if(score>40) return "fair";
    return "poor";
}

/* ============================================================================
 * Detector
 * ============================================================================ */

typedef struct {
    const char *id;
    const pattern_t *patterns;
} detector_map_t;

/* ============================================================================
 * Full detector map
 * ============================================================================ */
static const detector_map_t detector_map[] = {
    /* Document-level */
    {"propaganda", propaganda_patterns},
    {"conspiracy", conspiracy_patterns},

    /* Sentence-level */
    {"spam", spam_patterns},
    {"ragebait", ragebait_patterns},
    {"clickbait", clickbait_patterns},
    {"bot", bot_patterns},
    {"marketing", marketing_patterns},
    {"technobabble", technobabble_patterns},
    {"hype", hype_patterns},
    {"political", political_patterns},
    {"offensive", offensive_patterns},
    {"misinfo", misinformation_patterns},

    /* Word-level / slang */
    {"brain_rot", brain_rot_patterns},
    {"leet", NULL},       // handled separately
    {"morse", NULL},      // handled separately

    /* Stylistic / behavioral */
    {"formal", formal_patterns},
    {"casual", casual_patterns},
    {"sarcasm", sarcasm_patterns},
    {"neutral", neutral_patterns},
    {"aggressive", aggressive_patterns},
    {"emotional", emotional_patterns},
    {"passive_aggressive", passive_aggressive_patterns},
    {"snowflake", snowflake_patterns},

    /* Structural (logic handled separately) */
    {"redundant_sentences", NULL},
    {"poor_cohesion", NULL},
    {"repeated_words", NULL},

    {NULL, NULL}          // sentinel
};


/* ========================= Helper: get patterns by ID ========================= */
static const pattern_t *get_patterns(const char *detector_id) {
    for (size_t i = 0; detector_map[i].id; i++) {
        if (strcmp(detector_map[i].id, detector_id) == 0) {
            return detector_map[i].patterns;
        }
    }
    return NULL;
}

/* ============================================================================
 * Structural detection helpers
 * ============================================================================ */

static int detect_redundant_sentences(char **sentences) {
    if (!sentences) return 0;
    for (size_t i = 0; sentences[i]; i++) {
        for (size_t j = i + 1; sentences[j]; j++) {
            if (strcmp(sentences[i], sentences[j]) == 0) return 1;
        }
    }
    return 0;
}

static int detect_repeated_words(char **words) {
    if (!words) return 0;
    for (size_t i = 0; words[i]; i++) {
        for (size_t j = i + 1; words[j]; j++) {
            if (strcmp(words[i], words[j]) == 0) return 1;
        }
    }
    return 0;
}

static int detect_poor_cohesion(char **sentences) {
    if (!sentences) return 0;

    const char *linkers[] = {
        "and","but","so","because","however","therefore","thus","meanwhile",
        "moreover","furthermore","consequently","in addition","as a result",NULL
    };

    size_t total = 0;
    size_t weak = 0;

    for (size_t i = 0; sentences[i]; i++) {
        total++;

        // Count words
        size_t word_count = 0;
        char *copy = dupstr(sentences[i]);
        for (char *tok = strtok(copy, " \t\n"); tok; tok = strtok(NULL, " \t\n")) word_count++;
        free(copy);
        if (word_count < 5) weak++;

        // Check if next sentence starts with a linker (punctuation-aware)
        if (sentences[i+1]) {
            char *next = dupstr(sentences[i+1]);
            // skip leading punctuation and spaces
            char *p = next;
            while (*p && (isspace((unsigned char)*p) || ispunct((unsigned char)*p))) p++;

            int has_linker = 0;
            for (size_t k = 0; linkers[k]; k++) {
                size_t len = strlen(linkers[k]);
                if (strncasecmp(p, linkers[k], len) == 0 &&
                    (isspace((unsigned char)p[len]) || ispunct((unsigned char)p[len]) || p[len]==0)) {
                    has_linker = 1;
                    break;
                }
            }
            if (!has_linker) weak++;
            free(next);
        }
    }

    if (total == 0) return 0;
    return ((double)weak / total) > 0.3;
}

/* ============================================================================
 * Word-level helpers
 * ============================================================================ */
static int match_brain_rot(const char *word) {
    for (int i=0; brain_rot_patterns[i].pattern; i++) {
        if (strstr(word, brain_rot_patterns[i].pattern)) return 1;
    }
    return 0;
}

/* ============================================================================
 * Refactored fossil_io_soap_detect with Morse, BrainRot, Leet, and Structural
 * ============================================================================ */
/* Helper: match_patterns implementation */
static int match_patterns(const char *text, const pattern_t *patterns) {
    if (!text || !patterns) return 0;
    for (int i = 0; patterns[i].pattern; i++) {
        if (strstr(text, patterns[i].pattern)) return 1;
    }
    return 0;
}

int fossil_io_soap_detect(const char *text, const char *detector_id) {
    if (!text || !detector_id) return 0;

    int result = 0;

    /* ================= Document-level normalization ================= */
    char *norm = dupstr(text);
    normalize_leet(norm);
    strtolower(norm);

    const pattern_t *patterns = get_patterns(detector_id);
    if (patterns) result |= match_patterns(norm, patterns);
    free(norm);

    /* ================= Sentence-level detection ================= */
    char **sentences = fossil_io_soap_split(text);
    if (sentences) {
        for (size_t i = 0; sentences[i]; i++) {
            char *s_norm = dupstr(sentences[i]);
            normalize_leet(s_norm);
            strtolower(s_norm);

            if (patterns) result |= match_patterns(s_norm, patterns);

            free(s_norm);
        }
    }

    /* ================= Word-level detection ================= */
    char **words = fossil_io_soap_split(text);
    if (words) {
        for (size_t i = 0; words[i]; i++) {
            char *w_norm = dupstr(words[i]);
            normalize_leet(w_norm);
            strtolower(w_norm);

            if (strcmp(detector_id, "brain_rot") == 0) {
                if (match_brain_rot(w_norm)) result = 1;
            } else if (strcmp(detector_id, "leet") == 0) {
                for (char *p = w_norm; *p; p++)
                    if (*p == '4' || *p == '3' || *p == '1' || *p == '0' || *p == '5' || *p == '7') {
                        result = 1;
                        break;
                    }
            } else if (strcmp(detector_id, "morse") == 0) {
                char *decoded = decode_morse(w_norm);
                if (decoded) {
                    for (size_t j=0; j<strlen(decoded); j++)
                        if (isalpha(decoded[j])) { result = 1; break; }
                    free(decoded);
                }
            }

            free(w_norm);
        }
    }

    /* ================= Structural detection ================= */
    if (strcmp(detector_id, "redundant_sentences") == 0) {
        result = detect_redundant_sentences(sentences);
    } else if (strcmp(detector_id, "repeated_words") == 0) {
        result = detect_repeated_words(words);
    } else if (strcmp(detector_id, "poor_cohesion") == 0) {
        result = detect_poor_cohesion(sentences);
    }

    /* ================= Cleanup ================= */
    if (sentences && sentences != words) {
        for (size_t i = 0; sentences[i]; i++) free(sentences[i]);
        free(sentences);
    }

    if (words && words != sentences) {
        for (size_t i = 0; words[i]; i++) free(words[i]);
        free(words);
    }

    return result;
}

/* ============================================================================
 * Split / Reflow / Capitalize
 * ============================================================================ */

char **fossil_io_soap_split(const char *text) {
    // Overload: if text contains sentence-ending punctuation, split as sentences; else, split as words.
    if (!text) return NULL;

    // Heuristic: if text contains '.', '!' or '?', treat as sentences
    int is_sentence = 0;
    for (const char *q = text; *q; q++) {
        if (*q == '.' || *q == '!' || *q == '?') { is_sentence = 1; break; }
    }

    size_t count = 0;
    const char *p = text;
    while (*p) {
        if (!is_sentence) {
            if (isspace((unsigned char)*p)) count++;
        } else {
            if (*p == '.' || *p == '!' || *p == '?') count++;
        }
        p++;
    }
    char **arr = (char**)malloc(sizeof(char*) * (count + 2));
    size_t idx = 0;
    const char *start = text;
    p = text;
    while (*p) {
        if ((!is_sentence && isspace((unsigned char)*p)) ||
            (is_sentence && (*p == '.' || *p == '!' || *p == '?'))) {
            size_t len = p - start;
            char *s = (char*)malloc(len + 1);
            strncpy(s, start, len); s[len] = 0;
            arr[idx++] = s;
            start = p + 1;
        }
        p++;
    }
    if (*start) {
        arr[idx++] = dupstr(start);
    }
    arr[idx] = NULL;
    return arr;
}

char *fossil_io_soap_reflow(const char *text,int width){
    if(!text||width<=0) return dupstr(text);
    char *out=malloc(strlen(text)*2+1);
    int col=0; size_t oi=0;
    for(const char *p=text;*p;p++){
        out[oi++]=*p; col++;
        if(col>=width && isspace((unsigned char)*p)){ out[oi++]='\n'; col=0;}
    }
    out[oi]=0;
    return out;
}

char *fossil_io_soap_capitalize(const char *text,int mode){
    if(!text) return NULL;
    char *s=dupstr(text);
    if(mode==0){int cap=1; for(char *p=s;*p;p++){if(cap && isalpha((unsigned char)*p)){*p=toupper(*p); cap=0;} if(*p=='.'||*p=='!'||*p=='?') cap=1;}}
    else if(mode==1){int cap=1; for(char *p=s;*p;p++){if(isspace((unsigned char)*p)) cap=1; else if(cap){*p=toupper(*p); cap=0;}}}
    return s;
}

/* ============================================================================
 * Suggest / Summarize
 * ============================================================================ */

char *fossil_io_soap_suggest(const char *text){
    if(!text) return NULL;
    char *out=dupstr(text);
    char *p=out,*q=out; int last_space=0;
    while(*p){
        if(isspace((unsigned char)*p)){ if(!last_space)*q++=' '; last_space=1; }
        else { *q++=*p; last_space=0; }
        p++;
    }
    *q=0;
    return out;
}

char *fossil_io_soap_summarize(const char *text){
    if(!text) return NULL;
    char **sentences=fossil_io_soap_split(text);
    if(!sentences) return dupstr(text);
    char *out=malloc(1024);
    out[0]=0;
    int i;
    for(i=0;i<2 && sentences[i];i++){ strcat(out,sentences[i]); strcat(out," "); }
    return out;
}
