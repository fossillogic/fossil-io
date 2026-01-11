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
#include "fossil/io/cipher.h"
#include "fossil/io/cstring.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

/* ------------------------------------------------------------------------- */
/* Cipher handler signature                                                   */
/* ------------------------------------------------------------------------- */

typedef char *(*cipher_fn)(
    const char *text,
    const char *params,
    int decode
);

/* ------------------------------------------------------------------------- */
/* Forward declarations for cipher implementations                            */
/* (actual cipher logic can live elsewhere or be filled in later)             */
/* ------------------------------------------------------------------------- */

static char *cipher_caesar(const char *text, const char *params, int decode);
static char *cipher_vigenere(const char *text, const char *params, int decode);
static char *cipher_base64(const char *text, const char *params, int decode);
static char *cipher_base32(const char *text, const char *params, int decode);
static char *cipher_binary(const char *text, const char *params, int decode);
static char *cipher_morse(const char *text, const char *params, int decode);
static char *cipher_baconian(const char *text, const char *params, int decode);
static char *cipher_railfence(const char *text, const char *params, int decode);
static char *cipher_haxor(const char *text, const char *params, int decode);

/* ------------------------------------------------------------------------- */
/* Cipher registry                                                            */
/* ------------------------------------------------------------------------- */

typedef struct {
    const char *id;
    cipher_fn   fn;
} cipher_entry;

static const cipher_entry cipher_table[] = {
    { "caesar",    cipher_caesar    },
    { "vigenere",  cipher_vigenere  },
    { "base64",    cipher_base64    },
    { "base32",    cipher_base32    },
    { "binary",    cipher_binary    },
    { "morse",     cipher_morse     },
    { "baconian",  cipher_baconian  },
    { "railfence", cipher_railfence },
    { "haxor",    cipher_haxor    },
    { NULL, NULL }
};

/* ------------------------------------------------------------------------- */
/* Helpers                                                                    */
/* ------------------------------------------------------------------------- */

static const cipher_entry *cipher_lookup(const char *name) {
    for (int i = 0; cipher_table[i].id; i++) {
        if (!strcasecmp(cipher_table[i].id, name))
            return &cipher_table[i];
    }
    return NULL;
}

/* Splits "name:param" into name + param pointer */
static void cipher_split(const char *cipher_id,
             char *name, size_t name_sz,
             const char **params) {
    const char *p = strchr(cipher_id, ':');
    if (!p) {
        strncpy(name, cipher_id, name_sz - 1);
        name[name_sz - 1] = 0;
        *params = NULL;
        return;
    }

    size_t len = (size_t)(p - cipher_id);
    if (len >= name_sz)
        len = name_sz - 1;

    memcpy(name, cipher_id, len);
    name[len] = 0;
    *params = p + 1;
}

/* ------------------------------------------------------------------------- */
/* Public API                                                                 */
/* ------------------------------------------------------------------------- */

char *fossil_io_cipher_encode(const char *text, const char *cipher_id) {
    if (!text || !cipher_id)
        return NULL;

    char name[32];
    const char *params;

    cipher_split(cipher_id, name, sizeof(name), &params);

    const cipher_entry *c = cipher_lookup(name);
    if (!c || !c->fn)
        return NULL;

    return c->fn(text, params, 0);
}

char *fossil_io_cipher_decode(const char *text, const char *cipher_id) {
    if (!text || !cipher_id)
        return NULL;

    char name[32];
    const char *params;

    cipher_split(cipher_id, name, sizeof(name), &params);

    const cipher_entry *c = cipher_lookup(name);
    if (!c || !c->fn)
        return NULL;

    return c->fn(text, params, 1);
}

/* ------------------------------------------------------------------------- */
/* Lookup tables and helpers for ciphers                                      */
/* ------------------------------------------------------------------------- */

/* Caesar helpers */
static int parse_int_param(const char *params, const char *key, int def) {
    if (!params) return def;
    const char *p = strstr(params, key);
    if (!p) return def;
    p += strlen(key);
    if (*p == '=') p++;
    return atoi(p);
}
static int parse_bool_param(const char *params, const char *key, int def) {
    int v = parse_int_param(params, key, def);
    return v ? 1 : 0;
}

/* Vigenère helpers */
static void parse_vigenere_params(const char *params, char *key, size_t key_sz, int *alpha, int *preserve) {
    *alpha = parse_bool_param(params, "alpha", 1);
    *preserve = parse_bool_param(params, "preserve", 1);
    if (!params) { key[0] = 0; return; }
    const char *k = strstr(params, "key=");
    if (k) {
        k += 4;
        size_t len = 0;
        while (k[len] && k[len] != ',' && len < key_sz - 1) len++;
        strncpy(key, k, len); key[len] = 0;
    } else {
        size_t len = 0;
        while (params[len] && params[len] != ',' && len < key_sz - 1) len++;
        strncpy(key, params, len); key[len] = 0;
    }
}

/* Base64 helpers */
static const char base64_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char base64_url_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

/* Base32 helpers */
static const char base32_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
static const char base32hex_table[] =
    "0123456789ABCDEFGHIJKLMNOPQRSTUV";

/* Morse code table */
static const struct { char ch; const char *code; } morse_table[] = {
    {'A', ".-"},    {'B', "-..."},  {'C', "-.-."}, {'D', "-.."},   {'E', "."},
    {'F', "..-."},  {'G', "--."},   {'H', "...."}, {'I', ".."},    {'J', ".---"},
    {'K', "-.-"},   {'L', ".-.."},  {'M', "--"},   {'N', "-."},    {'O', "---"},
    {'P', ".--."},  {'Q', "--.-"},  {'R', ".-."},  {'S', "..."},   {'T', "-"},
    {'U', "..-"},   {'V', "...-"},  {'W', ".--"},  {'X', "-..-"},  {'Y', "-.--"},
    {'Z', "--.."},  {'0', "-----"}, {'1', ".----"},{'2', "..---"}, {'3', "...--"},
    {'4', "....-"}, {'5', "....."}, {'6', "-...."},{'7', "--..."}, {'8', "---.."},
    {'9', "----."}, {' ', "/"},     {0, NULL}
};

/* Baconian helpers */
static const char *baconian_classic[] = {
    "AAAAA","AAAAB","AAABA","AAABB","AABAA","AABAB","AABBA","AABBB","ABAAA","ABAAB",
    "ABABA","ABABB","ABBAA","ABBAB","ABBBA","ABBBB","BAAAA","BAAAB","BAABA","BAABB",
    "BABAA","BABAB","BABBA","BABBB","BBAAA","BBAAB"
};

/* Rail Fence helpers */
static void railfence_params(const char *params, int *rails, int *offset, int *preserve) {
    *rails = parse_int_param(params, "rails", 2);
    *offset = parse_int_param(params, "offset", 0);
    *preserve = parse_bool_param(params, "preserve", 1);
    if (!params) return;
    int v = atoi(params);
    if (v > 1) *rails = v;
}

/* Haxor helpers */
static const struct { char ch; const char *leet; } haxor_1337[] = {
    {'A', "4"}, {'B', "8"}, {'C', "("}, {'D', "|)"}, {'E', "3"}, {'F', "|="},
    {'G', "6"}, {'H', "#"}, {'I', "1"}, {'J', "_|"}, {'K', "|<"}, {'L', "1"},
    {'M', "/\\/\\",}, {'N', "|\\|"}, {'O', "0"}, {'P', "|>"}, {'Q', "0,"},
    {'R', "|2"}, {'S', "5"}, {'T', "7"}, {'U', "(_)"}, {'V', "\\/"}, {'W', "\\/\\/"},
    {'X', "><"}, {'Y', "`/"}, {'Z', "2"}, {0, NULL}
};

/* ------------------------------------------------------------------------- */
/* Cipher Implementations                                                     */
/* ------------------------------------------------------------------------- */

/* 1. Caesar Cipher */
static char *cipher_caesar(const char *text, const char *params, int decode) {
    int shift = parse_int_param(params, "shift", 3);
    int wrap = parse_bool_param(params, "wrap", 1);
    int alpha = parse_bool_param(params, "alpha", 1);
    if (!params && isdigit(text[0])) shift = atoi(text);

    if (decode) shift = 26 - (shift % 26);
    size_t len = strlen(text);
    char *out = malloc(len + 1);
    if (!out) return NULL;
    for (size_t i = 0; i < len; i++) {
        char c = text[i];
        if (alpha && isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            int off = (c - base + shift) % 26;
            if (!wrap && (c - base + shift) >= 26) out[i] = c;
            else out[i] = base + off;
        } else out[i] = c;
    }
    out[len] = 0;
    return out;
}

/* 3. Vigenère */
static char *cipher_vigenere(const char *text, const char *params, int decode) {
    char key[64] = {0};
    int alpha, preserve;
    parse_vigenere_params(params, key, sizeof(key), &alpha, &preserve);
    if (!key[0]) return NULL;
    size_t len = strlen(text), klen = strlen(key);
    char *out = malloc(len + 1);
    if (!out) return NULL;
    for (size_t i = 0, j = 0; i < len; i++) {
        char c = text[i];
        if (isalpha(c) && alpha) {
            char base = isupper(c) ? 'A' : 'a';
            char k = toupper(key[j % klen]) - 'A';
            int off = decode ? (26 + (c - base) - k) % 26 : (c - base + k) % 26;
            out[i] = base + off;
            j++;
        } else if (preserve) out[i] = c;
        else out[i] = c;
    }
    out[len] = 0;
    return out;
}

/* 4. Base64 */
static char *cipher_base64(const char *text, const char *params, int decode) {
    int url = parse_bool_param(params, "url", 0);
    int pad = parse_bool_param(params, "pad", 1);
    int wrap = parse_int_param(params, "wrap", 0);
    const char *table = url ? base64_url_table : base64_table;
    if (!decode) {
        size_t len = strlen(text);
        size_t olen = ((len + 2) / 3) * 4;
        char *out = malloc(olen + 2);
        if (!out) return NULL;
        size_t i, j = 0;
        for (i = 0; i < len; i += 3) {
            int v = (text[i] << 16) | ((i+1 < len ? text[i+1] : 0) << 8) | (i+2 < len ? text[i+2] : 0);
            out[j++] = table[(v >> 18) & 0x3F];
            out[j++] = table[(v >> 12) & 0x3F];
            out[j++] = (i+1 < len) ? table[(v >> 6) & 0x3F] : (pad ? '=' : 0);
            out[j++] = (i+2 < len) ? table[v & 0x3F] : (pad ? '=' : 0);
        }
        out[j] = 0;
        return out;
    } else {
        // Simple base64 decode (no error checking)
        size_t len = strlen(text);
        char *out = malloc(len + 1);
        if (!out) return NULL;
        int val = 0, valb = -8, j = 0;
        for (size_t i = 0; i < len; i++) {
            char c = text[i];
            if (c == '=' || c == '\n' || c == '\r') continue;
            const char *p = strchr(table, c);
            if (!p) continue;
            val = (val << 6) + (p - table);
            valb += 6;
            if (valb >= 0) {
                out[j++] = (char)((val >> valb) & 0xFF);
                valb -= 8;
            }
        }
        out[j] = 0;
        return out;
    }
}

/* 5. Base32 */
static char *cipher_base32(const char *text, const char *params, int decode) {
    int hex = parse_bool_param(params, "hex", 0);
    int pad = parse_bool_param(params, "pad", 1);
    int upper = parse_bool_param(params, "upper", 1);
    const char *table = hex ? base32hex_table : base32_table;
    if (!decode) {
        size_t len = strlen(text);
        size_t olen = ((len + 4) / 5) * 8;
        char *out = malloc(olen + 2);
        if (!out) return NULL;
        size_t i, j = 0;
        for (i = 0; i < len; i += 5) {
            unsigned int v = 0;
            for (int k = 0; k < 5; k++)
                v |= (i+k < len ? (unsigned char)text[i+k] : 0) << (8*(4-k));
            for (int k = 0; k < 8; k++) {
                if (i*8/5 + k < olen)
                    out[j++] = table[(v >> (35-5*k)) & 0x1F];
            }
        }
        if (pad) while (j % 8) out[j++] = '=';
        out[j] = 0;
        if (!upper) for (size_t k = 0; k < j; k++) out[k] = tolower(out[k]);
        return out;
    } else {
        // Simple base32 decode (no error checking)
        size_t len = strlen(text);
        char *out = malloc(len + 1);
        if (!out) return NULL;
        int val = 0, valb = -5, j = 0;
        for (size_t i = 0; i < len; i++) {
            char c = toupper(text[i]);
            if (c == '=' || c == '\n' || c == '\r') continue;
            const char *p = strchr(table, c);
            if (!p) continue;
            val = (val << 5) + (p - table);
            valb += 5;
            if (valb >= 0) {
                out[j++] = (char)((val >> valb) & 0xFF);
                valb -= 8;
            }
        }
        out[j] = 0;
        return out;
    }
}

/* 6. Binary */
static char *cipher_binary(const char *text, const char *params, int decode) {
    int bits = parse_int_param(params, "bits", 8);
    char sep = ' ';
    if (params) {
        const char *p = strstr(params, "sep=");
        if (p) sep = p[4];
        else if (isdigit(params[0])) bits = atoi(params);
    }
    int signed_mode = parse_bool_param(params, "signed", 0);
    if (!decode) {
        size_t len = strlen(text);
        char *out = malloc(len * (bits + 2) + 1);
        if (!out) return NULL;
        size_t j = 0;
        for (size_t i = 0; i < len; i++) {
            unsigned char c = text[i];
            for (int b = bits - 1; b >= 0; b--)
                out[j++] = ((c >> b) & 1) ? '1' : '0';
            out[j++] = sep;
        }
        out[j ? j-1 : 0] = 0;
        return out;
    } else {
        // Decode binary string to text
        size_t len = strlen(text);
        char *out = malloc(len / bits + 2);
        if (!out) return NULL;
        size_t j = 0, i = 0;
        while (i < len) {
            int v = 0, b = 0;
            while (b < bits && i < len) {
                if (text[i] == '0' || text[i] == '1') {
                    v = (v << 1) | (text[i] - '0');
                    b++;
                }
                i++;
            }
            if (b == bits) out[j++] = (char)v;
            while (i < len && text[i] == sep) i++;
        }
        out[j] = 0;
        return out;
    }
}

/* 7. Morse Code */
static char *cipher_morse(const char *text, const char *params, int decode) {
    char dash = '-';
    char dot = '.';
    const char *charsep = " ";
    const char *wordsep = "/";
    int strict = parse_bool_param(params, "strict", 0);
    if (params) {
        const char *p;
        if ((p = strstr(params, "dash="))) dash = p[5];
        if ((p = strstr(params, "dot="))) dot = p[4];
        if ((p = strstr(params, "charsep="))) charsep = p + 8;
        if ((p = strstr(params, "wordsep="))) wordsep = p + 8;
    }
    if (!decode) {
        size_t len = strlen(text);
        char *out = malloc(len * 8 + 1);
        if (!out) return NULL;
        size_t j = 0;
        size_t cslen = strlen(charsep);
        for (size_t i = 0; i < len; i++) {
            char c = toupper(text[i]);
            if (c == ' ') {
                size_t wslen = strlen(wordsep);
                memcpy(out + j, wordsep, wslen); j += wslen;
            } else {
                int found = 0;
                for (int k = 0; morse_table[k].ch; k++) {
                    if (morse_table[k].ch == c) {
                        for (size_t m = 0; morse_table[k].code[m]; m++)
                            out[j++] = morse_table[k].code[m] == '.' ? dot : dash;
                        found = 1;
                        break;
                    }
                }
                if (!found && strict) continue;
                memcpy(out + j, charsep, cslen); j += cslen;
            }
        }
        out[j ? j-cslen : 0] = 0;
        return out;
    } else {
        // Morse decode (simple, not strict)
        char *out = malloc(strlen(text) + 1);
        if (!out) return NULL;
        size_t j = 0;
        char buf[8] = {0};
        size_t bi = 0;
        for (size_t i = 0; text[i]; i++) {
            if (strncmp(text + i, wordsep, strlen(wordsep)) == 0) {
                out[j++] = ' ';
                i += strlen(wordsep) - 1;
                bi = 0;
            } else if (strncmp(text + i, charsep, strlen(charsep)) == 0) {
                buf[bi] = 0;
                for (int k = 0; morse_table[k].ch; k++) {
                    int match = 1;
                    for (size_t m = 0; morse_table[k].code[m]; m++) {
                        char sym = morse_table[k].code[m] == '.' ? dot : dash;
                        if (buf[m] != sym) { match = 0; break; }
                    }
                    if (match && strlen(morse_table[k].code) == strlen(buf)) {
                        out[j++] = morse_table[k].ch;
                        break;
                    }
                }
                bi = 0;
                i += strlen(charsep) - 1;
            } else if (text[i] == dot || text[i] == dash) {
                if (bi < sizeof(buf)-1) buf[bi++] = text[i];
            }
        }
        out[j] = 0;
        return out;
    }
}

/* 9. Baconian */
static char *cipher_baconian(const char *text, const char *params, int decode) {
    const char *variant = "classic";
    char a = 'A', b = 'B';
    int group = 5;
    if (params) {
        const char *p;
        if ((p = strstr(params, "variant="))) variant = p + 8;
        if ((p = strstr(params, "a="))) a = p[2];
        if ((p = strstr(params, "b="))) b = p[2];
        group = parse_int_param(params, "group", 5);
    }
    if (!decode) {
        size_t len = strlen(text);
        char *out = malloc(len * group + 2);
        if (!out) return NULL;
        size_t j = 0;
        for (size_t i = 0; i < len; i++) {
            char c = toupper(text[i]);
            if (c >= 'A' && c <= 'Z') {
                const char *code = baconian_classic[c - 'A'];
                for (int k = 0; k < group; k++)
                    out[j++] = code[k] == 'A' ? a : b;
            } else out[j++] = c;
        }
        out[j] = 0;
        return out;
    } else {
        // Baconian decode
        size_t len = strlen(text);
        char *out = malloc(len / group + 2);
        if (!out) return NULL;
        size_t j = 0, i = 0;
        while (i + group <= len) {
            char buf[6] = {0};
            for (int k = 0; k < group; k++)
                buf[k] = (text[i + k] == a) ? 'A' : (text[i + k] == b) ? 'B' : 0;
            i += group;
            for (int k = 0; k < 26; k++) {
                if (strncmp(buf, baconian_classic[k], group) == 0) {
                    out[j++] = 'A' + k;
                    break;
                }
            }
        }
        out[j] = 0;
        return out;
    }
}

/* 10. Rail Fence */
static char *cipher_railfence(const char *text, const char *params, int decode) {
    int rails, offset, preserve;
    railfence_params(params, &rails, &offset, &preserve);
    size_t len = strlen(text);
    char *out = malloc(len + 1);
    if (!out || rails < 2) return NULL;
    if (!decode) {
        int rail = 0, dir = 1;
        size_t j = 0;
        char **rows = malloc(rails * sizeof(char *));
        size_t *idx = calloc(rails, sizeof(size_t));
        for (int r = 0; r < rails; r++) rows[r] = malloc(len + 1);
        for (size_t i = 0; i < len; i++) {
            rows[rail][idx[rail]++] = text[i];
            rail += dir;
            if (rail == rails - 1 || rail == 0) dir = -dir;
        }
        for (int r = 0; r < rails; r++) {
            memcpy(out + j, rows[r], idx[r]);
            j += idx[r];
            free(rows[r]);
        }
        free(rows); free(idx);
        out[j] = 0;
        return out;
    } else {
        // Rail fence decode
        size_t *pos = calloc(rails, sizeof(size_t));
        size_t *lenr = calloc(rails, sizeof(size_t));
        int rail = 0, dir = 1;
        for (size_t i = 0; i < len; i++) {
            lenr[rail]++;
            rail += dir;
            if (rail == rails - 1 || rail == 0) dir = -dir;
        }
        char **rows = malloc(rails * sizeof(char *));
        size_t idx = 0;
        for (int r = 0; r < rails; r++) {
            rows[r] = malloc(lenr[r]);
            memcpy(rows[r], text + idx, lenr[r]);
            idx += lenr[r];
        }
        rail = 0; dir = 1;
        idx = 0;
        for (size_t i = 0; i < len; i++) {
            out[i] = rows[rail][pos[rail]++];
            rail += dir;
            if (rail == rails - 1 || rail == 0) dir = -dir;
        }
        for (int r = 0; r < rails; r++) free(rows[r]);
        free(rows); free(pos); free(lenr);
        out[len] = 0;
        return out;
    }
}

/* 11. Haxor */
static char *cipher_haxor(const char *text, const char *params, int decode) {
    const char *mode = "1337";
    int preserve_case = parse_bool_param(params, "case", 0);
    int reverse = parse_bool_param(params, "reverse", 0);
    if (params) {
        const char *p;
        if ((p = strstr(params, "mode="))) mode = p + 5;
    }
    size_t len = strlen(text);
    char *out = malloc(len * 8 + 1);
    if (!out) return NULL;
    size_t j = 0;
    if (!decode || !reverse) {
        for (size_t i = 0; i < len; i++) {
            char c = toupper(text[i]);
            int found = 0;
            for (int k = 0; haxor_1337[k].ch; k++) {
                if (haxor_1337[k].ch == c) {
                    size_t l = strlen(haxor_1337[k].leet);
                    memcpy(out + j, haxor_1337[k].leet, l);
                    j += l;
                    found = 1;
                    break;
                }
            }
            if (!found) out[j++] = text[i];
        }
        out[j] = 0;
        return out;
    } else {
        // Reverse mapping (simple, only for 1337)
        size_t i = 0;
        while (i < len) {
            int matched = 0;
            for (int k = 0; haxor_1337[k].ch; k++) {
                size_t l = strlen(haxor_1337[k].leet);
                if (strncmp(text + i, haxor_1337[k].leet, l) == 0) {
                    out[j++] = preserve_case ? haxor_1337[k].ch : tolower(haxor_1337[k].ch);
                    i += l;
                    matched = 1;
                    break;
                }
            }
            if (!matched) out[j++] = text[i++];
        }
        out[j] = 0;
        return out;
    }
}
