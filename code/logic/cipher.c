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

static char *cipher_caesar(const char *, const char *, int);
static char *cipher_rot13(const char *, const char *, int);
static char *cipher_vigenere(const char *, const char *, int);
static char *cipher_base64(const char *, const char *, int);
static char *cipher_base32(const char *, const char *, int);
static char *cipher_binary(const char *, const char *, int);
static char *cipher_morse(const char *, const char *, int);
static char *cipher_atbash(const char *, const char *, int);
static char *cipher_baconian(const char *, const char *, int);
static char *cipher_railfence(const char *, const char *, int);
static char *cipher_haxsor(const char *, const char *, int);

/* ------------------------------------------------------------------------- */
/* Cipher registry                                                            */
/* ------------------------------------------------------------------------- */

typedef struct {
    const char *id;
    cipher_fn   fn;
} cipher_entry;

static const cipher_entry cipher_table[] = {
    { "caesar",    cipher_caesar    },
    { "rot13",     cipher_rot13     },
    { "vigenere",  cipher_vigenere  },
    { "base64",    cipher_base64    },
    { "base32",    cipher_base32    },
    { "binary",    cipher_binary    },
    { "morse",     cipher_morse     },
    { "atbash",    cipher_atbash    },
    { "baconian",  cipher_baconian  },
    { "railfence", cipher_railfence },
    { "haxsor",    cipher_haxsor    },
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
/* Minimal reference cipher implementations                                   */
/* (safe stubs where logic not yet provided)                                  */
/* ------------------------------------------------------------------------- */

static char *cipher_caesar(const char *text, const char *params, int decode) {
    int shift = params ? atoi(params) : 3;
    if (decode) shift = -shift;

    char *out = fossil_io_cstring_dup(text);
    if (!out) return NULL;

    for (char *p = out; *p; p++) {
        if (isalpha((unsigned char)*p)) {
            char base = islower(*p) ? 'a' : 'A';
            *p = (char)(base + ((*p - base + shift + 26) % 26));
        }
    }
    return out;
}

static char *cipher_rot13(const char *text, const char *params, int decode) {
    (void)params;
    (void)decode;
    return cipher_caesar(text, "13", 0);
}

static char *cipher_atbash(const char *text, const char *params, int decode) {
    (void)params;
    (void)decode;

    char *out = fossil_io_cstring_dup(text);
    if (!out) return NULL;

    for (char *p = out; *p; p++) {
        if (islower(*p))
            *p = 'z' - (*p - 'a');
        else if (isupper(*p))
            *p = 'Z' - (*p - 'A');
    }
    return out;
}

static char *cipher_vigenere(const char *text, const char *params, int decode) {
    if (!params || !*params)
        return fossil_io_cstring_dup(text);

    char *out = fossil_io_cstring_dup(text);
    if (!out) return NULL;

    size_t klen = strlen(params);
    size_t j = 0;

    for (size_t i = 0; out[i]; i++) {
        if (!isalpha(out[i]))
            continue;

        int k = tolower(params[j % klen]) - 'a';
        if (decode) k = -k;

        char base = islower(out[i]) ? 'a' : 'A';
        out[i] = (char)(base + ((out[i] - base + k + 26) % 26));
        j++;
    }
    return out;
}

static const char base64_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static char *cipher_base64(const char *text, const char *params, int decode) {
    (void)params;

    if (!text) return NULL;

    if (!decode) {
        size_t len = strlen(text);
        size_t outlen = 4 * ((len + 2) / 3) + 1;
        char *out = malloc(outlen);
        if (!out) return NULL;

        size_t i = 0, j = 0;
        while (i < len) {
            unsigned char a = i < len ? text[i++] : 0;
            unsigned char b = i < len ? text[i++] : 0;
            unsigned char c = i < len ? text[i++] : 0;

            out[j++] = base64_table[(a >> 2) & 0x3F];
            out[j++] = base64_table[((a & 0x3) << 4) | ((b >> 4) & 0xF)];
            out[j++] = (i - 1) > len ? '=' : base64_table[((b & 0xF) << 2) | ((c >> 6) & 0x3)];
            out[j++] = (i) > len ? '=' : base64_table[c & 0x3F];
        }
        out[j] = 0;
        char *dup = fossil_io_cstring_dup(out);
        free(out);
        return dup;
    } else {
        /* Decode simplified version */
        size_t len = strlen(text);
        size_t outlen = len / 4 * 3 + 1;
        char *out = malloc(outlen);
        if (!out) return NULL;

        size_t i = 0, j = 0;
        unsigned char buf[4];
        int buflen = 0;

        while (i < len) {
            char c = text[i++];
            if (c == '=') break;
            const char *p = strchr(base64_table, c);
            if (!p) continue;
            buf[buflen++] = (unsigned char)(p - base64_table);
            if (buflen == 4) {
                out[j++] = (buf[0] << 2) | (buf[1] >> 4);
                out[j++] = (buf[1] << 4) | (buf[2] >> 2);
                out[j++] = (buf[2] << 6) | buf[3];
                buflen = 0;
            }
        }
        if (buflen) {
            if (buflen > 1) out[j++] = (buf[0] << 2) | (buf[1] >> 4);
            if (buflen > 2) out[j++] = (buf[1] << 4) | (buf[2] >> 2);
        }
        out[j] = 0;
        char *dup = fossil_io_cstring_dup(out);
        free(out);
        return dup;
    }
}

static const char base32_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

static char *cipher_base32(const char *text, const char *params, int decode) {
    (void)params;

    if (!text) return NULL;

    if (!decode) {
        size_t len = strlen(text);
        size_t outlen = (len + 4) / 5 * 8 + 1;
        char *out = malloc(outlen);
        if (!out) return NULL;

        size_t i = 0, j = 0;
        while (i < len) {
            unsigned char bytes[5] = {0,0,0,0,0};
            int bcount = 0;
            for (; bcount < 5 && i < len; bcount++, i++)
                bytes[bcount] = (unsigned char)text[i];

            out[j++] = base32_table[(bytes[0] >> 3) & 0x1F];
            out[j++] = base32_table[((bytes[0] & 0x07) << 2) | ((bytes[1] >> 6) & 0x03)];
            out[j++] = bcount > 1 ? base32_table[(bytes[1] >> 1) & 0x1F] : '=';
            out[j++] = bcount > 1 ? base32_table[((bytes[1] & 0x01) << 4) | ((bytes[2] >> 4) & 0x0F)] : '=';
            out[j++] = bcount > 2 ? base32_table[((bytes[2] & 0x0F) << 1) | ((bytes[3] >> 7) & 0x01)] : '=';
            out[j++] = bcount > 3 ? base32_table[(bytes[3] >> 2) & 0x1F] : '=';
            out[j++] = bcount > 3 ? base32_table[((bytes[3] & 0x03) << 3) | ((bytes[4] >> 5) & 0x07)] : '=';
            out[j++] = bcount > 4 ? base32_table[bytes[4] & 0x1F] : '=';
        }
        out[j] = 0;
        char *dup = fossil_io_cstring_dup(out);
        free(out);
        return dup;
    } else {
        /* Decode stub: fallback passthrough */
        return fossil_io_cstring_dup(text);
    }
}

static char *cipher_binary(const char *text, const char *params, int decode) {
    if (!text) return NULL;
    int bits = params ? atoi(params) : 8;

    if (decode) {
        size_t len = strlen(text);
        size_t outlen = len / bits + 1;
        char *out = fossil_io_cstring_dup("");
        if (!out) return NULL;

        for (size_t i = 0; i + bits <= len; i += bits) {
            char byte[9] = {0};
            strncpy(byte, text + i, bits);
            char c = (char) strtol(byte, NULL, 2);
            size_t olen = strlen(out);
            out = realloc(out, olen + 2);
            out[olen] = c;
            out[olen + 1] = 0;
        }
        return out;
    }

    /* Encode */
    size_t len = strlen(text);
    size_t outlen = len * bits + 1;
    char *out = malloc(outlen);
    if (!out) return NULL;
    out[0] = 0;

    for (size_t i = 0; i < len; i++) {
        char byte[16] = {0};
        for (int b = bits - 1; b >= 0; b--) {
            byte[bits - 1 - b] = ((text[i] >> b) & 1) ? '1' : '0';
        }
        strcat(out, byte);
    }

    char *dup = fossil_io_cstring_dup(out);
    free(out);
    return dup;
}

static const char *morse_table[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
    "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
    "..-", "...-", ".--", "-..-", "-.--", "--..", // A-Z
    "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----." // 0-9
};

static char *cipher_morse(const char *text, const char *params, int decode) {
    (void)params;
    if (!text) return NULL;

    if (!decode) {
        /* Encode */
        char *out = fossil_io_cstring_dup("");
        for (size_t i = 0; text[i]; i++) {
            char c = toupper(text[i]);
            if (c >= 'A' && c <= 'Z') {
                const char *m = morse_table[c - 'A'];
                size_t len = strlen(out) + strlen(m) + 2;
                out = realloc(out, len);
                strcat(out, m);
                strcat(out, " ");
            } else if (c >= '0' && c <= '9') {
                const char *m = morse_table[c - '0' + 26];
                size_t len = strlen(out) + strlen(m) + 2;
                out = realloc(out, len);
                strcat(out, m);
                strcat(out, " ");
            } else if (c == ' ') {
                strcat(out, "/ ");
            }
        }
        return out;
    }

    /* Decode (simplified, supports letters/digits only) */
    char *out = fossil_io_cstring_dup("");
    char buffer[16];
    size_t bidx = 0;

    for (size_t i = 0; ; i++) {
        char c = text[i];
        if (c == '.' || c == '-') {
            buffer[bidx++] = c;
        } else if (c == ' ' || c == 0) {
            buffer[bidx] = 0;
            if (bidx > 0) {
                int found = 0;
                for (int j = 0; j < 36; j++) {
                    if (strcmp(buffer, morse_table[j]) == 0) {
                        char decoded = (j < 26) ? 'A' + j : '0' + (j - 26);
                        size_t len = strlen(out);
                        out = realloc(out, len + 2);
                        out[len] = decoded;
                        out[len + 1] = 0;
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    size_t len = strlen(out);
                    out = realloc(out, len + 2);
                    out[len] = '?';
                    out[len + 1] = 0;
                }
            }
            bidx = 0;
        } else if (c == '/') {
            size_t len = strlen(out);
            out = realloc(out, len + 2);
            out[len] = ' ';
            out[len + 1] = 0;
        }
        if (c == 0) break;
    }
    return out;
}

static char *cipher_baconian(const char *text, const char *params, int decode) {
    (void)params;
    if (!text) return NULL;

    const char *bacon[26] = {
        "AAAAA","AAAAB","AAABA","AAABB","AABAA","AABAB","AABBA","AABBB",
        "ABAAA","ABAAB","ABABA","ABABB","ABBAA","ABBAB","ABBBA","ABBBB",
        "BAAAA","BAAAB","BAABA","BAABB","BABAA","BABAB","BABBA","BABBB",
        "BBAAA","BBAAB"
    };

    if (!decode) {
        char *out = fossil_io_cstring_dup("");
        for (size_t i = 0; text[i]; i++) {
            char c = toupper(text[i]);
            if (c >= 'A' && c <= 'Z') {
                size_t len = strlen(out) + 6;
                out = realloc(out, len);
                strcat(out, bacon[c - 'A']);
            }
        }
        return out;
    } else {
        char *out = fossil_io_cstring_dup("");
        size_t len = strlen(text);
        for (size_t i = 0; i + 5 <= len; i += 5) {
            char block[6];
            strncpy(block, text + i, 5);
            block[5] = 0;
            int found = 0;
            for (int j = 0; j < 26; j++) {
                if (strcmp(block, bacon[j]) == 0) {
                    size_t olen = strlen(out);
                    out = realloc(out, olen + 2);
                    out[olen] = 'A' + j;
                    out[olen + 1] = 0;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                size_t olen = strlen(out);
                out = realloc(out, olen + 2);
                out[olen] = '?';
                out[olen + 1] = 0;
            }
        }
        return out;
    }
}

static char *cipher_railfence(const char *text, const char *params, int decode) {
    if (!text) return NULL;

    int rails = params ? atoi(params) : 3;
    if (rails < 2) rails = 2;

    size_t len = strlen(text);
    char *out = fossil_io_cstring_dup(text);
    if (!out) return NULL;

    if (!decode) {
        char *tmp = malloc(len + 1);
        size_t idx = 0;
        for (int r = 0; r < rails; r++) {
            int step1 = (rails - r - 1) * 2;
            int step2 = r * 2;
            size_t i = r;
            int toggle = 0;
            while (i < len) {
                tmp[idx++] = text[i];
                if (r == 0 || r == rails - 1) {
                    i += (rails - 1) * 2;
                } else {
                    i += toggle ? step1 : step2;
                    toggle = !toggle;
                }
            }
        }
        tmp[len] = 0;
        free(out);
        out = fossil_io_cstring_dup(tmp);
        free(tmp);
    } else {
        /* Decode simplified: reverse process */
        char *tmp = malloc(len + 1);
        memset(tmp, 0, len + 1);

        size_t idx = 0;
        for (int r = 0; r < rails; r++) {
            int step1 = (rails - r - 1) * 2;
            int step2 = r * 2;
            size_t i = r;
            int toggle = 0;
            while (i < len) {
                tmp[i] = text[idx++];
                if (r == 0 || r == rails - 1) {
                    i += (rails - 1) * 2;
                } else {
                    i += toggle ? step1 : step2;
                    toggle = !toggle;
                }
            }
        }
        free(out);
        out = fossil_io_cstring_dup(tmp);
        free(tmp);
    }

    return out;
}

static char *cipher_haxsor(const char *text, const char *params, int decode) {
    (void)decode;  // XOR is symmetric
    if (!text) return NULL;

    char key = params && *params ? params[0] : 0x5A; // default key
    size_t len = strlen(text);

    char *out = fossil_io_cstring_dup(text);
    if (!out) return NULL;

    for (size_t i = 0; i < len; i++) {
        out[i] ^= key;
    }

    return out;
}
