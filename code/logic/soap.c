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
#include <ctype.h>
#include <stdio.h>

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

char *fossil_io_soap_correct_grammar(const char *text){
    return dupstr(text); // placeholder: safe no-op
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
    "buy now","click here","free gift","subscribe","limited offer","act now","instant access",
    "order today","exclusive deal","don't miss out","special promotion","save big","offer ends soon",
    "get your free","risk free","join now","bonus","money back","claim your prize","hot deal",
    "best price","lowest price","order now","urgent","today only","final chance","exclusive access",
    "secret deal","free trial","sign up today","claim now","free shipping","mega discount",
    "limited stock","special bonus","cash reward","earn money","work from home","get rich",
    "fast cash","profit now","exclusive membership","offer expires","exclusive content",
    "free download","instant win","prize inside","register free","money saving","exclusive coupon",
    "one-time offer","bonus gift","super deal","limited edition","exclusive bonus","big savings",
    "act fast","click to claim","get access","amazing offer","deal alert","hot offer","free voucher",
    "apply now","exclusive promotion","today's deal","instant offer","urgent offer","reward points",
    "exclusive giveaway","free subscription","unlock now","best deal","fast offer","money making",
    "limited opportunity","special access","claim gift","instant reward","exclusive deal today",
    "cash bonus","bonus reward","top offer","mega offer","discount coupon","free trial offer",
    "secret offer","one day only","hurry","final offer","special discount","cash prize",
    "exclusive chance","register now","big bonus","limited availability","exclusive access code",
    "free access","hot sale","get it now","special offer code","exclusive bonus deal","fast track",
    "instant access code","limited edition gift","exclusive prize","amazing deal","cash back",
    "super offer","exclusive membership access","hot discount","grab now","limited promotion",
    "today only offer","exclusive opportunity","special reward","claim your bonus","urgent deal",
    "biggest discount","free gift card","final chance offer","exclusive voucher","super discount",
    NULL
};

// ========================= RAGEBAIT PATTERNS =========================
static const pattern_t ragebait_patterns[] = {
    "you won't believe","shocking","outrageous","unbelievable","infuriating","angry","furious",
    "disgusting","ridiculous","scandal","horrifying","outrage","outraged","triggered","furor",
    "angry reaction","outrageous claim","outrageous story","unacceptable","outrageous behavior",
    "shocking truth","unbelievable act","inexcusable","angry mob","furious debate","disgusting act",
    "ridiculous claim","shocking event","infuriating story","outrageous incident","unbelievable event",
    "horrifying act","furious reaction","outrageous remark","triggering statement","infuriating post",
    "scandalous behavior","outraged citizens","outrageous opinion","disgusting statement","ridiculous act",
    "unbelievable behavior","shocking revelation","unacceptable act","outrageous attack","furious argument",
    "angry post","infuriating video","outrageous content","ridiculous claim","horrifying story",
    "triggered audience","outrageous rant","furious post","disgusting action","outrageous content",
    "shocking message","angry rant","outrageous content","ridiculous argument","infuriating opinion",
    "unbelievable statement","horrifying content","outraged viewers","furious commentary","triggered audience",
    "scandalous statement","outrageous post","infuriating message","ridiculous video","disgusting content",
    "unacceptable behavior","outrageous video","angry commentary","furious reaction","shocking opinion",
    "triggered post","outrageous action","ridiculous message","infuriating video","furious statement",
    "outrageous comment","shocking post","angry message","unbelievable content","disgusting video",
    "outrageous tweet","ridiculous commentary","furious post","infuriating action","outrageous tweet",
    "horrifying video","triggered comment","outrageous content","shocking message","unbelievable tweet",
    "furious comment","angry post","outrageous content","ridiculous content","infuriating post",
    "triggered tweet","outrageous reaction","furious video","horrifying comment","shocking content",
    NULL
};

// ========================= CLICKBAIT PATTERNS =========================
static const pattern_t clickbait_patterns[] = {
    "this one trick","what happened next","will blow your mind","you need to see","can't miss",
    "the secret to","top 10","never guess","revealed","shocking discovery","amazing fact",
    "unbelievable result","hidden truth","little known","you won't believe this","life hack",
    "number one","you'll never guess","what no one tells you","ultimate guide","best way to",
    "never before seen","crazy trick","mind blowing","amazing secret","you have to see","secret revealed",
    "life changing","what they don't want you to know","incredible hack","one simple trick",
    "you won't believe what happens","top secret","you need this","amazing tip","never seen before",
    "hack that works","you must try","you won't expect","shocking truth","crazy discovery",
    "hidden secret","unbelievable hack","amazing guide","must see","ultimate tip","you won't believe it",
    "top ways to","what happens next","secret method","little known fact","you can't miss","amazing results",
    "what no one tells","unbelievable tip","you'll be shocked","secret trick","ultimate hack","never guess",
    "life hack revealed","hidden method","crazy tip","must watch","you won't believe these","top tips",
    "unbelievable guide","amazing hack","secret revealed","you won't expect this","shocking method",
    "ultimate guide revealed","incredible tip","you have to try","hidden trick","crazy guide",
    "life changing tip","must read","secret hack revealed","unbelievable trick","amazing method",
    "what happens next will shock you","top secret hack","you can't believe this","ultimate trick",
    "secret tip revealed","amazing hack revealed","life hack you need","shocking guide","you must know",
    "never seen hack","crazy life hack","hidden guide","top tricks","you won't believe this hack",
    NULL
};

// ========================= BOT PATTERNS =========================
static const pattern_t bot_patterns[] = {
    "check this out","hello everyone","click the link","visit our page","automated message","bot account",
    "follow us","like and share","join our group","retweet","subscribe now","auto reply","spam account",
    "bot message","follow for more","auto comment","mass message","bot posting","automated post",
    "generic message","bot content","click link","follow now","auto follow","bot comment","spam bot",
    "automated reply","bot activity","subscribe bot","join now bot","bot interaction","auto like",
    "bot mention","click bot","bot follower","bot spam","automated message bot","follow bot","auto message",
    "bot alert","bot post","bot share","bot tag","bot follow","automated comment","bot response",
    "spam message","bot notification","auto post","bot link","bot account message","bot content share",
    "bot thread","bot account post","automated mention","bot reply","bot tweet","bot email",
    "auto message bot","bot notification message","bot generated","bot spam message","bot interaction message",
    "bot comment post","bot reaction","bot activity log","bot message alert","bot system message",
    NULL
};

// ========================= MARKETING PATTERNS =========================
static const pattern_t marketing_patterns[] = {
    "special promotion","exclusive offer","sign up now","limited time","discount","save big","exclusive deal",
    "buy one get one","offer ends soon","act fast","register today","free trial","limited quantity",
    "best price","deal of the day","new product","promotion code","mega sale","limited stock","hot deal",
    "exclusive bonus","bonus gift","super deal","offer expires soon","cash reward","earn money","work from home",
    "fast cash","profit now","exclusive membership","reward points","free download","instant win","prize inside",
    "register free","money saving","one-time offer","bonus reward","top offer","mega offer","discount coupon",
    "free trial offer","secret offer","one day only","hurry","final offer","special discount","cash prize",
    "exclusive chance","register now","big bonus","limited availability","exclusive access code","free access",
    "hot sale","get it now","special offer code","exclusive bonus deal","fast track","instant access code",
    "limited edition gift","exclusive prize","amazing deal","cash back","super offer","exclusive membership access",
    "hot discount","grab now","limited promotion","today only offer","exclusive opportunity","special reward",
    "claim your bonus","urgent deal","biggest discount","free gift card","final chance offer","exclusive voucher",
    "super discount","coupon code","mega promotion","instant reward","limited offer code","special giveaway",
    "exclusive promotion code","bonus access","instant coupon","reward offer","limited edition offer","early bird",
    "VIP access","priority access","premium offer","early access","exclusive preview","bonus points","special deal",
    NULL
};

// ========================= TECHNOBABBLE PATTERNS =========================
static const pattern_t technobabble_patterns[] = {
    "AI-driven","blockchain-enabled","synergy","paradigm shift","cutting-edge","next-gen","disruptive",
    "innovative","cloud-native","hyperconverged","deep learning","IoT-enabled","quantum-ready","machine learning",
    "big data","digital transformation","smart tech","next generation","autonomous systems","AI-powered",
    "edge computing","neural network","predictive analytics","cloud computing","AI solution","data-driven",
    "automation","smart devices","advanced analytics","AI model","ML model","cybersecurity","blockchain technology",
    "digital ecosystem","tech stack","hybrid cloud","AI framework","cognitive computing","predictive modeling",
    "data pipeline","deep neural network","intelligent system","AI assistant","robotic process","IoT platform",
    "distributed ledger","automation framework","quantum computing","data science","AI agent","AI architecture",
    "cloud platform","AI algorithm","neural processing","smart system","machine intelligence","AI integration",
    "digital twin","AI strategy","AI implementation","tech innovation","cloud services","AI capabilities","edge AI",
    "next-gen platform","intelligent automation","advanced computing","AI research","AI technology","smart network",
    "AI optimization","ML pipeline","autonomous AI","AI deployment","cognitive system","AI-powered solution",
    "intelligent platform","AI-powered system","AI-driven analytics","machine learning pipeline","deep learning model",
    "predictive AI","AI-enabled platform","intelligent solution","AI innovation","AI-driven framework","AI-powered analytics",
    "AI-based system","AI-powered model","machine intelligence platform","automation system","AI research project",
    "AI cloud","AI-powered platform","AI assistant system","AI integration framework","intelligent AI","autonomous technology",
    "AI-powered assistant","digital automation","AI-powered automation","AI workflow","AI decision-making","AI-enabled solution",
    "AI optimization platform","intelligent analytics","AI-enhanced system","AI-powered analytics platform",
    NULL
};

// ========================= HYPE PATTERNS =========================
static const pattern_t hype_patterns[] = {
    "revolutionary","next level","game-changing","amazing results","must try","incredible","unstoppable",
    "breakthrough","epic","legendary","unprecedented","mind-blowing","next big thing","innovative","cutting-edge",
    "trailblazing","world-class","record-breaking","ultimate","next generation","astonishing","powerful","groundbreaking",
    "transformative","unmatched","elite","unbelievable","fantastic","unparalleled","sensational","amazing discovery",
    "next frontier","exciting","remarkable","next evolution","exclusive","outstanding","phenomenal","amazing opportunity",
    "top-tier","exceptional","spectacular","breathtaking","superior","next-level experience","game-changer","extreme",
    "supercharged","amazing innovation","inspiring","legendary performance","extraordinary","remarkable achievement",
    "epic results","transformational","amazing breakthrough","cutting-edge technology","elite performance",
    "groundbreaking discovery","unprecedented results","next-level innovation","record-shattering","astonishing results",
    "world-class achievement","mind-blowing experience","ultimate performance","exceptional results","revolutionary idea",
    "next big innovation","amazing journey","unmatched experience","fantastic results","powerful performance",
    "unparalleled achievement","sensational results","remarkable discovery","incredible results","ultimate success",
    "epic innovation","outstanding achievement","next frontier technology","astonishing breakthrough","next-gen tech",
    "revolutionary discovery","extraordinary performance","phenomenal results","superior innovation","mind-blowing breakthrough",
    "game-changing idea","amazing transformation","next evolution technology","legendary innovation","record-breaking results",
    "unprecedented discovery","cutting-edge performance","next-level breakthrough","elite innovation","ultimate technology",
    "transformative results","astonishing achievement","spectacular innovation","next-level success","epic breakthrough",
    "remarkable innovation","amazing performance","revolutionary tech","next frontier innovation","supercharged results",
    NULL
};

// ========================= POLITICAL PATTERNS =========================
static const pattern_t political_patterns[] = {
    "vote for","government","policy change","election","political debate","candidate","party platform","legislation",
    "bill passed","congress","senate","democracy","campaign","political rally","government official","policy proposal",
    "voting rights","political agenda","political party","political issue","campaign trail","government policy",
    "political leader","executive order","public office","political reform","political campaign","political news",
    "political statement","political controversy","political election","voter turnout","political debate","parliament",
    "government election","legislative session","political discussion","government reform","policy decision",
    "political movement","political strategy","government regulation","election results","political endorsement",
    "political scandal","candidate statement","political platform","political action","political pressure","government action",
    "policy initiative","political negotiation","political discourse","party leader","election campaign","public policy",
    "political influence","political leadership","political ideology","government funding","political appointment",
    "political reform movement","campaign strategy","political protest","legislative process","political advocacy",
    "government decision","policy implementation","political analysis","political debate series","political opinion",
    "political funding","political race","government election results","political endorsement","political meeting",
    "policy change proposal","government policy initiative","political conference","political forum","political summit",
    "political discussion panel","campaign advertisement","political fundraiser","political debate stage","political campaign speech",
    "government announcement","political campaign event","political decision making","political action plan","political reform agenda",
    "political legislation","political pressure group","voting campaign","political activism","government reform plan",
    "political accountability","election campaign strategy","political debate event","political speech","political initiative",
    "public policy debate","political candidate statement","legislative reform","political controversy discussion","political campaign ad",
    NULL
};

// ========================= OFFENSIVE PATTERNS =========================
static const pattern_t offensive_patterns[] = {
    "slur1","slur2","slur3","insult1","insult2","insult3","derogatory1","derogatory2","derogatory3",
    "offensive1","offensive2","offensive3","hate1","hate2","hate3","mock1","mock2","mock3",
    "demean1","demean2","demean3","ridicule1","ridicule2","ridicule3","abuse1","abuse2","abuse3",
    "curse1","curse2","curse3","obscene1","obscene2","obscene3","vulgar1","vulgar2","vulgar3",
    "racial1","racial2","racial3","sexist1","sexist2","sexist3","homophobic1","homophobic2","homophobic3",
    "slurword1","slurword2","slurword3","offensiveword1","offensiveword2","offensiveword3","insultword1","insultword2","insultword3",
    "derogword1","derogword2","derogword3","curseword1","curseword2","curseword3","abuseword1","abuseword2","abuseword3",
    "vulgarword1","vulgarword2","vulgarword3","mockword1","mockword2","mockword3","demeanword1","demeanword2","demeanword3",
    "hateword1","hateword2","hateword3","ridiculeword1","ridiculeword2","ridiculeword3","obsceneword1","obsceneword2","obsceneword3",
    "offensivephrase1","offensivephrase2","offensivephrase3","slurphrase1","slurphrase2","slurphrase3","insultphrase1","insultphrase2","insultphrase3",
    "derogphrase1","derogphrase2","derogphrase3","cursephrase1","cursephrase2","cursephrase3","abusephrase1","abusephrase2","abusephrase3",
    "vulgarphrase1","vulgarphrase2","vulgarphrase3","mockphrase1","mockphrase2","mockphrase3","demeanphrase1","demeanphrase2","demeanphrase3",
    NULL
};

// ========================= PROPAGANDA PATTERNS =========================
static const pattern_t propaganda_patterns[] = {
    "must believe","hidden agenda","our way or","truth about","patriot","loyal citizen","for the greater good",
    "support the cause","national pride","heroic act","propaganda campaign","we stand united","join the movement",
    "fight for freedom","for our country","true leader","loyal supporter","government approved","official statement",
    "authoritative source","mandatory belief","join the cause","follow the leader","citizens must act",
    "united we stand","official message","approved content","trusted source","mandatory compliance","loyal citizenry",
    "national interest","propaganda message","approved narrative","official propaganda","follow the rules","government narrative",
    "state-approved","patriotic duty","join the ranks","official decree","propaganda material","official announcement",
    "mandated belief","trusted source statement","official directive","loyal followers","government-endorsed","official guidance",
    "propaganda statement","mandatory participation","state-sanctioned","official narrative","approved information",
    "national duty","citizen responsibility","loyalty pledge","approved message","official media","trusted government source",
    "state message","mandatory allegiance","national pride message","official policy","propaganda broadcast","follow official guidance",
    "government-issued","official instructions","mandatory support","state-endorsed","official platform","propaganda content",
    "national loyalty","join official program","officially mandated","approved content distribution","government propaganda",
    NULL
};

// ========================= MISINFORMATION PATTERNS =========================
static const pattern_t misinformation_patterns[] = {
    "cure for cancer","miracle","hoax","false claims","fake news","unproven","conspiracy theory","secret formula",
    "miracle cure","medical hoax","false report","debunked claim","myth","unverified","fraudulent","false information",
    "fake cure","false story","misleading","hoax report","bogus claim","unverified story","fake discovery","pseudoscience",
    "false narrative","deceptive","fraud","unfounded","unsubstantiated","false advice","mythical","fabricated","misleading claim",
    "hoax alert","fake report","debunked theory","unconfirmed","false statement","bogus news","false rumor","unverified claim",
    "untrue","incorrect","misreported","fake discovery","false evidence","fraudulent claim","unverified rumor","misleading story",
    "hoax alert news","unconfirmed claim","fabricated story","false explanation","bogus statement","fake finding","pseudoscientific claim",
    "false allegation","unsubstantiated claim","misleading narrative","fake announcement","false assertion","untrue claim",
    "debunked story","fake report alert","misinformation campaign","bogus discovery","false guidance","fraudulent information",
    "unverified report","hoax message","misleading info","fake news report","false claim news","unproven assertion","bogus alert",
    "pseudoscience alert","fake study","misinformation report","unverified info","false research","fake evidence","fraud alert",
    NULL
};

// ========================= CONSPIRACY PATTERNS =========================
static const pattern_t conspiracy_patterns[] = {
    "secret government","they don't want you to know","cover-up","hidden truth","shadow government","elite control",
    "deep state","hidden agenda","classified information","conspiracy theory","underground network","top secret",
    "secret society","government cover-up","hidden plan","covert operation","secret files","insider information",
    "classified report","elite conspiracy","hidden documents","suppressed info","secret project","undercover agenda",
    "covert plan","hidden agenda revealed","government secrets","classified files","secret operation","hidden truth exposed",
    "underground conspiracy","shadow organization","secret meeting","concealed agenda","covert activity","secret knowledge",
    "hidden scheme","classified evidence","secret society plan","conspiracy uncovered","hidden network","elite manipulation",
    "government deception","secret directive","underground plot","covert conspiracy","hidden motive","secret files revealed",
    "shadow operation","secret agenda","hidden agenda documents","covert investigation","top secret project","classified information leak",
    "elite control plan","government cover-up revealed","secret plans","hidden facts","covert manipulation","underground network exposed",
    "secret truth","shadow agenda","hidden conspiracy","government deception plan","classified agenda","secret manipulation",
    "concealed plan","elite conspiracy revealed","covert files","secret government files","hidden operations","classified operation",
    "underground society","secret plot","hidden influence","covert agenda","government secret files","classified directive",
    NULL
};

static const pattern_t formal_patterns[] = {
    "therefore","hence","thus","moreover","furthermore","in conclusion","as a result",
    "consequently","accordingly","subsequently","as demonstrated","it follows that",
    "in summary","henceforth","by virtue of","in accordance with","pursuant to","notwithstanding",
    "as evidenced","insofar as","given that","insofar","whereas","hereby","as outlined",
    "with regard to","as stated","as indicated","provided that","inasmuch as",
    "for the purpose of","in light of","as previously mentioned","it can be observed",
    "it is therefore","taking into account","as per","in view of","in respect of",
    "following","subject to","under the circumstances","as has been noted",
    NULL
};

static const pattern_t casual_patterns[] = {
    "lol","bruh","hey","yo","dude","omg","haha","yo bro",
    "what's up","sup","man","buddy","y'all","gonna","wanna","cool","awesome",
    "nah","yep","hey there","chill","broski","yo dude","hey guys","totally",
    "whatever","no way","sounds good","catch you","later","bye","see ya",
    "yep yep","woah","oh man","sweet","dang","for real","right on","heck yeah",
    "gotcha","you bet","my bad","oh snap","haha lol","lmao","haha wow","peace out",
    NULL
};

static const pattern_t sarcasm_patterns[] = {
    "yeah right","sure","as if","totally","obviously","not really","wow great",
    "nice try","good one","oh fantastic","perfect timing","just what I needed",
    "how original","brilliant idea","love that","great job","oh really","wonderful",
    "superb","yeah sure","can't wait","fascinating","as expected","way to go",
    "just lovely","amazing work","how convenient","oh joy","well done",
    "wow, amazing","oh perfect","fantastic job","great, just great","lovely timing",
    "right, brilliant","oh sure, absolutely","how surprising","well, isn't that nice",
    "oh, wonderful timing","just perfect","marvelous","spectacular","oh, fabulous",
    NULL
};

static const pattern_t neutral_patterns[] = {
    "this","is","a","sentence","it","contains","words","normal",
    "statement","information","text","paragraph","example","data","object",
    "element","value","context","description","point","fact","case","item",
    "example text","written content","document","letter","report","entry",
    "note","message","record","section","section of text",
    "plain","simple","average","basic","standard","common","typical",
    "general","unremarkable","ordinary","regular","factual","matter-of-fact",
    "neutral tone","objective","informational","descriptive","explanatory",
    NULL
};

static const pattern_t aggressive_patterns[] = {
    "stupid","idiot","hate","kill","fool","loser","shut up","annoying",
    "moron","dumb","screw you","jerk","ridiculous","garbage","pathetic",
    "disgusting","worthless","attack","insult","retard","losing","destroy",
    "suck","failure","trash","punish","angry","fight","threat","rage",
    "obnoxious","hate you","get out","pissed","kill yourself","idiotic",
    "destroyed","broken","ruined","trash talk","violent","vicious","nasty",
    "hostile","harass","attack","fuming","disrespect","smash","wreck","crap",
    "awful","blame","anger","malicious","ferocious","outrage","hate-filled",
    NULL
};

static const pattern_t emotional_patterns[] = {
    "happy","sad","angry","excited","frustrated","joy","fear","love","hate",
    "anxious","bored","confused","relieved","nervous","proud","disappointed",
    "elated","ecstatic","content","upset","cheerful","gloomy","hopeful",
    "lonely","surprised","shocked","terrified","overjoyed","miserable",
    "joyful","angsty","in love","heartbroken","ecstatic","grief","pleased",
    "stressed","excitedly","enthusiastic","motivated","sentimental",
    "curious","overwhelmed","fearful","desperate","regretful","panicked",
    "optimistic","conflicted","relaxed","tense","melancholy","nervousness",
    "elation","distressed","apprehensive","joyous","disheartened",
    NULL
};

static const pattern_t passive_aggressive_patterns[] = {
    "sure","fine","whatever","if you say so","interesting","ok then","noted",
    "thanks a lot","good for you","no offense","whatever you say","appreciate it",
    "right, sure","as you wish","yeah, okay","not my problem","whatever works",
    "I guess","fine then","cool story","nice of you","way to go","oh great",
    "well, thanks","thanks anyway","glad to hear","if that makes you happy",
    "noted with thanks","oh, wonderful","wonderful job","guess that's fine",
    "thanks, I suppose","oh, fantastic","good for you, I guess","how nice",
    "noted, thanks","oh, amazing","sounds fun","well, alright","interesting take",
    NULL
};

/* ============================================================================
 * BrainRot patterns table (expanded to cover maximal internet casual/slang terms)
 * ============================================================================ */
static const char *brain_rot_patterns[] = {
    "lol","lmao","rofl","bruh","wtf","omg","haha","hehe","xd","xdxd",
    "yo","hey","sup","what's up","man","dude","bro","broski","homie","fam",
    "yolo","smh","fml","idk","ikr","tbh","ily","ily2","omfg","wtfv",
    "haha","heh","meh","ugh","aww","yay","yayyy","yesss","nope","nah",
    "yep","bruhh","brooo","duuuude","lolz","lols","lul","lulz","hahaaha",
    "roflmao","lmfao","kek","pog","poggers","pogchamp","rip","gg","ggwp",
    "rekt","owned","clap","clapclap","smh","facepalm","yikes","oops","oopsie",
    "ayyy","ayyy lmao","hehehe","yayyyyy","nooooo","yaaaas","bruh moment","oh no",
    "ikr","omgosh","lmaoed","lmaoing","haha lol","lol haha","xd lol","lol xd",
    "fml lol","smdh","sksksk","and i oop","yeet","yeeted","yeeting","uwu","owo",
    "uwu uwu","owo owo","rawr","rawr x3",">:(","<3",":3","-_-","^_^","T_T",";-;",
    ">:O","-.-",">:(","D:","XD","xD","XD XD",">:|","-.-'","-__-","o.O","O.o",
    "lolwut","lold","lolol","lololol","haha lmao","roflol","roflolmao","wtf lol",
    NULL
};

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
    for (int i=0; brain_rot_patterns[i]; i++) {
        if (strstr(word, brain_rot_patterns[i])) return 1;
    }
    return 0;
}

/* ============================================================================
 * Refactored fossil_io_soap_detect with Morse, BrainRot, Leet, and Structural
 * ============================================================================ */
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
    char **sentences = fossil_io_soap_split(text, "sentences");
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
    char **words = fossil_io_soap_split(text, "words");
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
    if (sentences) {
        for (size_t i = 0; sentences[i]; i++) free(sentences[i]);
        free(sentences);
    }

    if (words) {
        for (size_t i = 0; words[i]; i++) free(words[i]);
        free(words);
    }

    return result;
}

/* ============================================================================
 * Split / Reflow / Capitalize
 * ============================================================================ */

char **fossil_io_soap_split(const char *text, const char *flow_type){
    if(!text) return NULL;
    size_t count=0;
    const char *p=text;
    while(*p){
        if(!strcmp(flow_type,"words")){
            if(isspace((unsigned char)*p)) count++;
        }else{
            if(*p=='.'||*p=='!'||*p=='?') count++;
        }
        p++;
    }
    char **arr = (char**)malloc(sizeof(char*)*(count+2));
    size_t idx=0;
    const char *start=text;
    p=text;
    while(*p){
        if((!strcmp(flow_type,"words") && isspace((unsigned char)*p)) ||
           (!strcmp(flow_type,"sentences") && (*p=='.'||*p=='!'||*p=='?'))){
            size_t len=p-start;
            char *s=(char*)malloc(len+1);
            strncpy(s,start,len); s[len]=0;
            arr[idx++]=s;
            start=p+1;
        }
        p++;
    }
    if(*start){
        arr[idx++]=dupstr(start);
    }
    arr[idx]=NULL;
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
    char **sentences=fossil_io_soap_split(text,"sentences");
    if(!sentences) return dupstr(text);
    char *out=malloc(1024);
    out[0]=0;
    int i;
    for(i=0;i<2 && sentences[i];i++){ strcat(out,sentences[i]); strcat(out," "); }
    return out;
}

/* ============================================================================
 * High-level process with flow-type dispatch
 * ============================================================================ */

typedef struct {
    char *processed_text;                  // sanitized, normalized, grammar-corrected
    char *summary;                         // short summary (if requested)
    fossil_io_soap_scores_t scores;        // readability, clarity, quality
    fossil_io_soap_grammar_style_t style;  // style analysis
    struct {
        int brain_rot;
        int leet;
        int spam;
        int ragebait;
        int propaganda;
        int conspiracy;
        int clickbait;
        int bot;
        int marketing;
        int technobabble;
        int hype;
        int political;
        int offensive;
        int misinformation;
        int formal;
        int casual;
        int sarcasm;
        int neutral;
        int aggressive;
        int emotional;
        int passive_aggressive;
    } flags;
} fossil_io_soap_result_t;

static int detect_flag(const char *text, const char *detector_id, const char *flow_type) {
    if (!text || !detector_id) return 0;
    int res = fossil_io_soap_detect(text, detector_id, flow_type);
    return res;
}

static fossil_io_soap_result_t *
soap_process_internal(const char *text,
                      const char *flow_type,
                      const fossil_io_soap_options_t *options)
{
    if (!text) return NULL;

    fossil_io_soap_result_t *r = calloc(1, sizeof(*r));
    r->processed_text = dupstr(text);

    /* Step 0: implicit morse detection (heuristic, not option-gated) */
    if (fossil_io_soap_detect(r->processed_text, "morse", flow_type)) {
        char *tmp = decode_morse(r->processed_text);
        if (tmp) {
            free(r->processed_text);
            r->processed_text = tmp;
        }
    }

    /* Step 1: normalization pipeline */
    if (options) {
        if (options->apply_sanitization) {
            char *tmp = fossil_io_soap_sanitize(r->processed_text);
            free(r->processed_text); r->processed_text = tmp;
        }
        if (options->apply_normalization) {
            char *tmp = fossil_io_soap_normalize(r->processed_text);
            free(r->processed_text); r->processed_text = tmp;
        }
        if (options->apply_grammar_correction) {
            char *tmp = fossil_io_soap_correct_grammar(r->processed_text);
            free(r->processed_text); r->processed_text = tmp;
        }
    }

    /* Step 2: flow dispatch */
    if (flow_type && options) {

        if (strcmp(flow_type, "words") == 0) {
            char **tokens = fossil_io_soap_split(r->processed_text, "words");
            for (size_t i = 0; tokens && tokens[i]; i++) {
                if (options->detect_brain_rot)
                    r->flags.brain_rot |= detect_flag(tokens[i],"brain_rot","words");
                if (options->detect_quality)
                    r->flags.leet |= detect_flag(tokens[i],"leet","words");
                if (options->detect_formal)
                    r->flags.formal |= detect_flag(tokens[i],"formal","words");
                if (options->detect_casual)
                    r->flags.casual |= detect_flag(tokens[i],"casual","words");
                if (options->detect_sarcasm)
                    r->flags.sarcasm |= detect_flag(tokens[i],"sarcasm","words");
                if (options->detect_neutral)
                    r->flags.neutral |= detect_flag(tokens[i],"neutral","words");
                if (options->detect_aggressive)
                    r->flags.aggressive |= detect_flag(tokens[i],"aggressive","words");
                if (options->detect_emotional)
                    r->flags.emotional |= detect_flag(tokens[i],"emotional","words");
                if (options->detect_passive_aggressive)
                    r->flags.passive_aggressive |= detect_flag(tokens[i],"passive_aggressive","words");
            }
            for (size_t i = 0; tokens && tokens[i]; i++) free(tokens[i]);
            free(tokens);
        }

        else if (strcmp(flow_type, "sentences") == 0) {
            char **sentences = fossil_io_soap_split(r->processed_text, "sentences");
            for (size_t i = 0; sentences && sentences[i]; i++) {
                if (options->analyze_grammar || options->include_style)
                    r->style = fossil_io_soap_analyze_grammar_style(sentences[i]);

                if (options->detect_spam)
                    r->flags.spam |= detect_flag(sentences[i],"spam","sentences");
                if (options->detect_ragebait)
                    r->flags.ragebait |= detect_flag(sentences[i],"ragebait","sentences");
                if (options->detect_clickbait)
                    r->flags.clickbait |= detect_flag(sentences[i],"clickbait","sentences");
                if (options->detect_bot)
                    r->flags.bot |= detect_flag(sentences[i],"bot","sentences");
                if (options->detect_marketing)
                    r->flags.marketing |= detect_flag(sentences[i],"marketing","sentences");
                if (options->detect_technobabble)
                    r->flags.technobabble |= detect_flag(sentences[i],"technobabble","sentences");
                if (options->detect_hype)
                    r->flags.hype |= detect_flag(sentences[i],"hype","sentences");
                if (options->detect_political)
                    r->flags.political |= detect_flag(sentences[i],"political","sentences");
                if (options->detect_offensive)
                    r->flags.offensive |= detect_flag(sentences[i],"offensive","sentences");
                if (options->detect_misinformation)
                    r->flags.misinformation |= detect_flag(sentences[i],"misinformation","sentences");
            }
            for (size_t i = 0; sentences && sentences[i]; i++) free(sentences[i]);
            free(sentences);
        }

        else if (strcmp(flow_type, "documents") == 0) {
            if (options->include_scores)
                r->scores = fossil_io_soap_score(r->processed_text);

            if (options->detect_propaganda)
                r->flags.propaganda |= detect_flag(r->processed_text,"propaganda","documents");
            if (options->detect_conspiracy)
                r->flags.conspiracy |= detect_flag(r->processed_text,"conspiracy","documents");

            if (options->include_summary)
                r->summary = fossil_io_soap_summarize(r->processed_text);
        }
    }

    return r;
}

static char *
soap_result_to_string(const fossil_io_soap_result_t *r,
                      const fossil_io_soap_options_t *options)
{
    if (!r) return dupstr("");

    /* default behavior: return processed text only */
    if (!options || !options->include_summary)
        return dupstr(r->processed_text);

    /* simple annotated output */
    size_t len = strlen(r->processed_text) + 128;
    if (r->summary) len += strlen(r->summary);

    char *out = calloc(1, len);
    strcat(out, r->processed_text);

    if (r->summary) {
        strcat(out, "\n\nSummary:\n");
        strcat(out, r->summary);
    }

    return out;
}

char *
fossil_io_soap_process(const char *text,
                       const char *flow_type,
                       const fossil_io_soap_options_t *options)
{
    fossil_io_soap_result_t *r =
        soap_process_internal(text, flow_type, options);

    char *out = soap_result_to_string(r, options);

    /* cleanup */
    if (r) {
        free(r->processed_text);
        free(r->summary);
        free(r);
    }

    return out;
}
