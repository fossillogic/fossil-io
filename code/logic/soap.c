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
#include "soap.h"
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

int fossil_io_soap_detect(const char *text,
                          const char *detector_id,
                          const char *flow_type)
{
    if (!text || !detector_id) return 0;

    int result = 0;

    /* Normalize for detection */
    char *norm = dupstr(text);
    for (char *p = norm; *p; p++) *p = (char)tolower((unsigned char)*p);

    /* ========================================================================
     * Flow-specific detection
     * ======================================================================== */
    if (flow_type && strcmp(flow_type, "") != 0) {
        if (strcmp(flow_type, "words") == 0) {
            /* Word-level: scan tokens individually */
            char *token = strtok(norm, " \t\n");
            while (token) {
                if (strcmp(detector_id, "brain_rot") == 0) {
                    if (strstr(token, "lol") || strstr(token, "bro")) { result = 1; break; }
                }
                if (strcmp(detector_id, "leet") == 0) {
                    for (char *c = token; *c; c++)
                        if (*c=='4'||*c=='3'||*c=='1'||*c=='0'||*c=='5'||*c=='7') { result=1; break; }
                }
                token = strtok(NULL, " \t\n");
            }
        }
        else if (strcmp(flow_type, "sentences") == 0) {
            /* Sentence-level: scan for tone / spam phrases */
            const char *p = norm;
            while (*p) {
                if (strcmp(detector_id, "spam") == 0) {
                    if (strstr(p,"buy now") || strstr(p,"click here")) { result=1; break; }
                }
                if (strcmp(detector_id, "ragebait") == 0) {
                    if (strstr(p,"you won't believe") || strstr(p,"shocking")) { result=1; break; }
                }
                if (strcmp(detector_id, "morse") == 0) {
                    if (strchr(p,'.') && strchr(p,'-')) { result=1; break; }
                }
                /* move to next sentence */
                while (*p && *p != '.' && *p != '!' && *p != '?') p++;
                if (*p) p++;
            }
        }
        else if (strcmp(flow_type, "documents") == 0) {
            /* Document-level: full text heuristics */
            if (strcmp(detector_id, "propaganda") == 0) {
                if (strstr(norm,"must believe") || strstr(norm,"agenda")) result=1;
            }
            if (strcmp(detector_id, "conspiracy") == 0) {
                if (strstr(norm,"secret") && strstr(norm,"government")) result=1;
            }
        }
    }
    else {
        /* Fallback: simple global detection */
        if (strcmp(detector_id, "spam") == 0) result = strstr(norm,"buy now")!=NULL;
        else if (strcmp(detector_id, "brain_rot") == 0) result = strstr(norm,"lol") || strstr(norm,"bro");
        else if (strcmp(detector_id, "morse") == 0) result = strchr(norm,'.') && strchr(norm,'-');
    }

    free(norm);
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

char *fossil_io_soap_process(const char *text,const char *flow_type,const fossil_io_soap_options_t *options){
    if(!text) return NULL;
    char *work=dupstr(text);

    /* decode Morse if detected */
    if(fossil_io_soap_detect(work,"morse","signal")){
        char *tmp=decode_morse(work);
        free(work); work=tmp;
    }

    /* normalization / sanitization / grammar correction */
    if(options){
        if(options->apply_sanitization){ char *tmp=fossil_io_soap_sanitize(work); free(work); work=tmp; }
        if(options->apply_normalization){ char *tmp=fossil_io_soap_normalize(work); free(work); work=tmp; }
        if(options->apply_grammar_correction){ char *tmp=fossil_io_soap_correct_grammar(work); free(work); work=tmp; }
    }

    /* flow-type dispatch */
    if(flow_type){
        if(!strcmp(flow_type,"words")){
            char **tokens=fossil_io_soap_split(work,"words");
            for(size_t i=0;tokens[i];i++){
                fossil_io_soap_detect(tokens[i],"brain_rot","words");
            }
        } else if(!strcmp(flow_type,"sentences")){
            char **sentences=fossil_io_soap_split(work,"sentences");
            for(size_t i=0;sentences[i];i++){
                fossil_io_soap_analyze_grammar_style(sentences[i]);
                fossil_io_soap_score(sentences[i]);
            }
        } else if(!strcmp(flow_type,"documents")){
            fossil_io_soap_score(work);
            fossil_io_soap_summarize(work);
        }
    }

    return work;
}
