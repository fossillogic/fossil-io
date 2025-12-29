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

static const pattern_t spam_patterns[] = {
    {"buy now"}, {"click here"}, {"free gift"}, {"subscribe"}, {"limited offer"}, {NULL}
};

static const pattern_t ragebait_patterns[] = {
    {"you won't believe"}, {"shocking"}, {"outrageous"}, {"unbelievable"}, {NULL}
};

static const pattern_t clickbait_patterns[] = {
    {"this one trick"}, {"what happened next"}, {"will blow your mind"}, {"you need to see"}, {NULL}
};

static const pattern_t bot_patterns[] = {
    {"check this out"}, {"hello everyone"}, {"click the link"}, {"visit our page"}, {NULL}
};

static const pattern_t marketing_patterns[] = {
    {"special promotion"}, {"exclusive offer"}, {"sign up now"}, {"limited time"}, {NULL}
};

static const pattern_t technobabble_patterns[] = {
    {"AI-driven"}, {"blockchain-enabled"}, {"synergy"}, {"paradigm shift"}, {NULL}
};

static const pattern_t hype_patterns[] = {
    {"revolutionary"}, {"next level"}, {"game-changing"}, {"amazing results"}, {NULL}
};

static const pattern_t political_patterns[] = {
    {"vote for"}, {"government"}, {"policy change"}, {"election"}, {NULL}
};

static const pattern_t offensive_patterns[] = {
    {"slur1"}, {"slur2"}, {"slur3"}, {NULL} // replace with actual offensive words
};

static const pattern_t propaganda_patterns[] = {
    {"must believe"}, {"hidden agenda"}, {"our way or"}, {"truth about"}, {NULL}
};

static const pattern_t misinformation_patterns[] = {
    {"cure for cancer"}, {"miracle"}, {"hoax"}, {"false claims"}, {NULL}
};

static const pattern_t conspiracy_patterns[] = {
    {"secret government"}, {"they don't want you to know"}, {"cover-up"}, {"hidden truth"}, {NULL}
};

static int match_patterns(const char *text, const pattern_t *patterns) {
    if (!text || !patterns) return 0;
    for (int i = 0; patterns[i].pattern; i++) {
        if (strstr(text, patterns[i].pattern)) return 1;
    }
    return 0;
}

int fossil_io_soap_detect(const char *text,
                          const char *detector_id,
                          const char *flow_type)
{
    if (!text || !detector_id) return 0;

    int result = 0;

    if (!flow_type || strcmp(flow_type,"documents")==0) {
        /* ================= Document-level detectors ================= */
        char *norm = dupstr(text);
        normalize_leet(norm);
        strtolower(norm);

        if (strcmp(detector_id,"propaganda")==0)
            result = match_patterns(norm, propaganda_patterns);
        else if (strcmp(detector_id,"conspiracy")==0)
            result = match_patterns(norm, conspiracy_patterns);
        /* optionally add document-wide checks here */

        free(norm);
    }
    else if (strcmp(flow_type,"sentences")==0) {
        /* Split text into sentences and run sentence-level detectors */
        char **sentences = fossil_io_soap_split(text,"sentences");
        for (size_t i=0; sentences && sentences[i]; i++) {
            char *norm = dupstr(sentences[i]);
            normalize_leet(norm);
            strtolower(norm);

            if (strcmp(detector_id,"spam")==0) result |= match_patterns(norm, spam_patterns);
            else if (strcmp(detector_id,"ragebait")==0) result |= match_patterns(norm, ragebait_patterns);
            else if (strcmp(detector_id,"clickbait")==0) result |= match_patterns(norm, clickbait_patterns);
            else if (strcmp(detector_id,"bot")==0) result |= match_patterns(norm, bot_patterns);
            else if (strcmp(detector_id,"marketing")==0) result |= match_patterns(norm, marketing_patterns);
            else if (strcmp(detector_id,"technobabble")==0) result |= match_patterns(norm, technobabble_patterns);
            else if (strcmp(detector_id,"hype")==0) result |= match_patterns(norm, hype_patterns);
            else if (strcmp(detector_id,"political")==0) result |= match_patterns(norm, political_patterns);
            else if (strcmp(detector_id,"offensive")==0) result |= match_patterns(norm, offensive_patterns);
            else if (strcmp(detector_id,"misinfo")==0) result |= match_patterns(norm, misinformation_patterns);

            free(norm);
        }

        for (size_t i=0; sentences && sentences[i]; i++) free(sentences[i]);
        free(sentences);
    }
    else if (strcmp(flow_type,"words")==0) {
        /* Split text into words and run word-level detectors */
        char **words = fossil_io_soap_split(text,"words");
        for (size_t i=0; words && words[i]; i++) {
            char *norm = dupstr(words[i]);
            normalize_leet(norm);
            strtolower(norm);

            if (strcmp(detector_id,"brain_rot")==0) {
                if (strstr(norm,"lol") || strstr(norm,"bro") || strstr(norm,"wtf") || strstr(norm,"bruh"))
                    result = 1;
            } else if (strcmp(detector_id,"leet")==0) {
                for (char *p=norm; *p; p++)
                    if (*p=='4'||*p=='3'||*p=='1'||*p=='0'||*p=='5'||*p=='7') { result = 1; break; }
            }

            free(norm);
        }

        for (size_t i=0; words && words[i]; i++) free(words[i]);
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
