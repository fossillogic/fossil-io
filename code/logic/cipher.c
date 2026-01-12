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

static char *cipher_caesar(const char *text, int decode);
static char *cipher_vigenere(const char *text, int decode);
static char *cipher_base64(const char *text, int decode);
static char *cipher_base32(const char *text, int decode);
static char *cipher_binary(const char *text, int decode);
static char *cipher_morse(const char *text, int decode);
static char *cipher_baconian(const char *text, int decode);
static char *cipher_railfence(const char *text, int decode);
static char *cipher_haxor(const char *text, int decode);
static char *cipher_leet(const char *text, int decode);
static char *cipher_rot13(const char *text, int decode);
static char *cipher_atbash(const char *text, int decode);

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
    { "haxor",     cipher_haxor     },
    { "leet",      cipher_leet      },
    { "rot13",     cipher_rot13     },
    { "atbash",    cipher_atbash    },
    { NULL, NULL }
};

/* ------------------------------------------------------------------------- */
/* Helpers                                                                    */
/* ------------------------------------------------------------------------- */

static const cipher_entry *cipher_lookup(ccstring name) {
    for (int i = 0; cipher_table[i].id; i++) {
        if (fossil_io_cstring_iequals(cipher_table[i].id, name))
            return &cipher_table[i];
    }
    return NULL;
}

/* ------------------------------------------------------------------------- */
/* Public API                                                                 */
/* ------------------------------------------------------------------------- */

cstring fossil_io_cipher_encode(ccstring text, ccstring cipher_id) {
    if (!text || !cipher_id)
        return NULL;

    const cipher_entry *c = cipher_lookup(cipher_id);
    if (!c || !c->fn)
        return NULL;

    // The cipher functions return malloc'd char*, so cast to cstring
    return (cstring)c->fn(text, NULL, 0);
}

cstring fossil_io_cipher_decode(ccstring text, ccstring cipher_id) {
    if (!text || !cipher_id)
        return NULL;

    const cipher_entry *c = cipher_lookup(cipher_id);
    if (!c || !c->fn)
        return NULL;

    return (cstring)c->fn(text, NULL, 1);
}

/* ------------------------------------------------------------------------- */
/* Lookup tables and helpers for ciphers                                      */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* Caesar Cipher                                                             */
/* ------------------------------------------------------------------------- */
static cstring cipher_caesar(ccstring text, int decode) {
    int shift = 3;
    if (decode) shift = 26 - shift;
    size_t len = fossil_io_cstring_length(text);
    cstring out = malloc(len + 1);
    if (!out) return NULL;
    for (size_t i = 0; i < len; ++i) {
        char c = text[i];
        if (isupper(c))
            out[i] = ((c - 'A' + shift) % 26) + 'A';
        else if (islower(c))
            out[i] = ((c - 'a' + shift) % 26) + 'a';
        else
            out[i] = c;
    }
    out[len] = '\0';
    return out;
}

/* ------------------------------------------------------------------------- */
/* Vigenère Cipher                                                           */
/* ------------------------------------------------------------------------- */
static cstring cipher_vigenere(ccstring text, int decode) {
    const char *key = "KEY";
    size_t len = fossil_io_cstring_length(text);
    size_t keylen = fossil_io_cstring_length(key);
    cstring out = malloc(len + 1);
    if (!out) return NULL;
    for (size_t i = 0, j = 0; i < len; ++i) {
        char c = text[i];
        if (isalpha(c)) {
            int k = toupper(key[j % keylen]) - 'A';
            if (decode) k = 26 - k;
            if (isupper(c))
                out[i] = ((c - 'A' + k) % 26) + 'A';
            else
                out[i] = ((c - 'a' + k) % 26) + 'a';
            j++;
        } else {
            out[i] = c;
        }
    }
    out[len] = '\0';
    return out;
}

/* ------------------------------------------------------------------------- */
/* Base64                                                                    */
/* ------------------------------------------------------------------------- */
static const char b64_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static cstring cipher_base64(ccstring text, int decode) {
    size_t len = fossil_io_cstring_length(text);
    if (!decode) {
        size_t out_len = 4 * ((len + 2) / 3);
        cstring out = malloc(out_len + 1);
        if (!out) return NULL;
        size_t i, j;
        for (i = 0, j = 0; i < len;) {
            uint32_t octet_a = i < len ? (unsigned char)text[i++] : 0;
            uint32_t octet_b = i < len ? (unsigned char)text[i++] : 0;
            uint32_t octet_c = i < len ? (unsigned char)text[i++] : 0;
            uint32_t triple = (octet_a << 16) | (octet_b << 8) | octet_c;
            out[j++] = b64_table[(triple >> 18) & 0x3F];
            out[j++] = b64_table[(triple >> 12) & 0x3F];
            out[j++] = (i > len + 1) ? '=' : b64_table[(triple >> 6) & 0x3F];
            out[j++] = (i > len) ? '=' : b64_table[triple & 0x3F];
        }
        out[out_len] = '\0';
        return out;
    } else {
        int b64_index[256] = {0};
        for (int i = 0; i < 64; ++i) b64_index[(unsigned char)b64_table[i]] = i;
        size_t out_len = len / 4 * 3;
        if (text[len - 1] == '=') out_len--;
        if (text[len - 2] == '=') out_len--;
        cstring out = malloc(out_len + 1);
        if (!out) return NULL;
        size_t i, j;
        for (i = 0, j = 0; i < len;) {
            int sextet_a = text[i] == '=' ? 0 & i++ : b64_index[(unsigned char)text[i++]];
            int sextet_b = text[i] == '=' ? 0 & i++ : b64_index[(unsigned char)text[i++]];
            int sextet_c = text[i] == '=' ? 0 & i++ : b64_index[(unsigned char)text[i++]];
            int sextet_d = text[i] == '=' ? 0 & i++ : b64_index[(unsigned char)text[i++]];
            uint32_t triple = (sextet_a << 18) | (sextet_b << 12) | (sextet_c << 6) | sextet_d;
            if (j < out_len) out[j++] = (triple >> 16) & 0xFF;
            if (j < out_len) out[j++] = (triple >> 8) & 0xFF;
            if (j < out_len) out[j++] = triple & 0xFF;
        }
        out[out_len] = '\0';
        return out;
    }
}

/* ------------------------------------------------------------------------- */
/* Base32                                                                    */
/* ------------------------------------------------------------------------- */
static const char b32_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

static cstring cipher_base32(ccstring text, int decode) {
    size_t len = fossil_io_cstring_length(text);
    if (!decode) {
        size_t out_len = ((len + 4) / 5) * 8;
        cstring out = malloc(out_len + 1);
        if (!out) return NULL;
        size_t i, j;
        for (i = 0, j = 0; i < len;) {
            uint64_t buffer = 0;
            int bits = 0;
            for (int k = 0; k < 5; ++k) {
                buffer <<= 8;
                if (i < len) buffer |= (unsigned char)text[i++];
                bits += 8;
            }
            for (int k = 0; k < 8; ++k) {
                if (bits >= 5) {
                    out[j++] = b32_table[(buffer >> (bits - 5)) & 0x1F];
                    bits -= 5;
                } else {
                    out[j++] = '=';
                }
            }
        }
        out[out_len] = '\0';
        return out;
    } else {
        int b32_index[256] = {0};
        for (int i = 0; i < 32; ++i) b32_index[(unsigned char)b32_table[i]] = i;
        size_t out_len = len * 5 / 8;
        cstring out = malloc(out_len + 1);
        if (!out) return NULL;
        size_t i, j;
        for (i = 0, j = 0; i < len;) {
            uint64_t buffer = 0;
            int bits = 0;
            for (int k = 0; k < 8 && i < len; ++k) {
                char c = text[i++];
                if (c == '=') {
                    buffer <<= 5;
                    bits += 5;
                } else {
                    buffer = (buffer << 5) | b32_index[(unsigned char)c];
                    bits += 5;
                }
            }
            for (int k = 0; k < 5 && j < out_len; ++k) {
                if (bits >= 8) {
                    out[j++] = (buffer >> (bits - 8)) & 0xFF;
                    bits -= 8;
                }
            }
        }
        out[out_len] = '\0';
        return out;
    }
}

/* ------------------------------------------------------------------------- */
/* Binary (ASCII to binary string and back)                                  */
/* ------------------------------------------------------------------------- */
static cstring cipher_binary(ccstring text, int decode) {
    if (!decode) {
        size_t len = fossil_io_cstring_length(text);
        cstring out = malloc(len * 8 + 1);
        if (!out) return NULL;
        char *p = out;
        for (size_t i = 0; i < len; ++i) {
            for (int b = 7; b >= 0; --b)
                *p++ = ((text[i] >> b) & 1) ? '1' : '0';
        }
        *p = '\0';
        return out;
    } else {
        size_t len = fossil_io_cstring_length(text);
        if (len % 8 != 0) return NULL;
        size_t out_len = len / 8;
        cstring out = malloc(out_len + 1);
        if (!out) return NULL;
        for (size_t i = 0; i < out_len; ++i) {
            char c = 0;
            for (int b = 0; b < 8; ++b)
                c = (c << 1) | (text[i * 8 + b] == '1' ? 1 : 0);
            out[i] = c;
        }
        out[out_len] = '\0';
        return out;
    }
}

/* ------------------------------------------------------------------------- */
/* Morse Code                                                                */
/* ------------------------------------------------------------------------- */
static const char *morse_table[36] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", // A-J
    "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",   // K-T
    "..-", "...-", ".--", "-..-", "-.--", "--..",                         // U-Z
    "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----." // 0-9
};

static cstring cipher_morse(ccstring text, int decode) {
    if (!decode) {
        size_t len = fossil_io_cstring_length(text);
        cstring out = malloc(len * 5 * 4 + 1); // generous buffer
        if (!out) return NULL;
        char *p = out;
        for (size_t i = 0; i < len; ++i) {
            char c = toupper(text[i]);
            if (c >= 'A' && c <= 'Z') {
                const char *m = morse_table[c - 'A'];
                while (*m) *p++ = *m++;
                *p++ = ' ';
            } else if (c >= '0' && c <= '9') {
                const char *m = morse_table[26 + (c - '0')];
                while (*m) *p++ = *m++;
                *p++ = ' ';
            } else if (isspace(c)) {
                *p++ = '/';
                *p++ = ' ';
            }
        }
        if (p > out) *(p - 1) = '\0';
        else *p = '\0';
        return out;
    } else {
        size_t len = fossil_io_cstring_length(text);
        cstring out = malloc(len + 1);
        if (!out) return NULL;
        size_t o = 0;
        char buf[8] = {0};
        size_t b = 0;
        for (size_t i = 0; i <= len; ++i) {
            if (text[i] == '.' || text[i] == '-' || text[i] == '_') {
                if (b < sizeof(buf) - 1)
                    buf[b++] = text[i];
            } else if (text[i] == ' ' || text[i] == '\0' || text[i] == '/') {
                buf[b] = '\0';
                if (b > 0) {
                    int found = 0;
                    for (int j = 0; j < 36; ++j) {
                        if (strcmp(buf, morse_table[j]) == 0) {
                            if (j < 26)
                                out[o++] = 'A' + j;
                            else
                                out[o++] = '0' + (j - 26);
                            found = 1;
                            break;
                        }
                    }
                    if (!found) out[o++] = '?';
                }
                b = 0;
                if (text[i] == '/') out[o++] = ' ';
            }
        }
        out[o] = '\0';
        return out;
    }
}

/* ------------------------------------------------------------------------- */
/* Baconian Cipher                                                           */
/* ------------------------------------------------------------------------- */
static const char *baconian_table[26] = {
    "AAAAA","AAAAB","AAABA","AAABB","AABAA","AABAB","AABBA","AABBB","ABAAA","ABAAB",
    "ABABA","ABABB","ABBAA","ABBAB","ABBBA","ABBBB","BAAAA","BAAAB","BAABA","BAABB",
    "BABAA","BABAB","BABBA","BABBB","BBAAA","BBAAB"
};

static cstring cipher_baconian(ccstring text, int decode) {
    if (!decode) {
        size_t len = fossil_io_cstring_length(text);
        cstring out = malloc(len * 5 + 1);
        if (!out) return NULL;
        char *p = out;
        for (size_t i = 0; i < len; ++i) {
            char c = toupper(text[i]);
            if (c >= 'A' && c <= 'Z') {
                const char *b = baconian_table[c - 'A'];
                for (int j = 0; j < 5; ++j) *p++ = b[j];
            } else if (isspace(c)) {
                *p++ = ' ';
            }
        }
        *p = '\0';
        return out;
    } else {
        size_t len = fossil_io_cstring_length(text);
        cstring out = malloc(len / 5 + 2);
        if (!out) return NULL;
        size_t o = 0;
        for (size_t i = 0; i + 4 < len;) {
            if (isspace(text[i])) {
                out[o++] = ' ';
                ++i;
                continue;
            }
            char buf[6] = {0};
            for (int j = 0; j < 5 && i < len; ++j, ++i)
                buf[j] = toupper(text[i]);
            int found = 0;
            for (int j = 0; j < 26; ++j) {
                if (fossil_io_cstring_compare_safe(buf, baconian_table[j], 5) == 0) {
                    out[o++] = 'A' + j;
                    found = 1;
                    break;
                }
            }
            if (!found) out[o++] = '?';
        }
        out[o] = '\0';
        return out;
    }
}

/* ------------------------------------------------------------------------- */
/* Rail Fence Cipher                                                         */
/* ------------------------------------------------------------------------- */
static cstring cipher_railfence(ccstring text, int decode) {
    int rails = 3;
    size_t len = fossil_io_cstring_length(text);
    if (rails < 2 || len < 2) return fossil_io_cstring_dup(text);
    cstring out = malloc(len + 1);
    if (!out) return NULL;
    if (!decode) {
        size_t pos = 0;
        for (int r = 0; r < rails; ++r) {
            int down = 1, row = 0;
            for (size_t i = 0; i < len; ++i) {
                if (row == r) out[pos++] = text[i];
                if (row == 0) down = 1;
                else if (row == rails - 1) down = 0;
                row += down ? 1 : -1;
            }
        }
        out[pos] = '\0';
    } else {
        int *rail_map = malloc(len * sizeof(int));
        if (!rail_map) { free(out); return NULL; }
        int down = 1, row = 0;
        for (size_t i = 0; i < len; ++i) {
            rail_map[i] = row;
            if (row == 0) down = 1;
            else if (row == rails - 1) down = 0;
            row += down ? 1 : -1;
        }
        size_t *rail_pos = calloc(rails, sizeof(size_t));
        size_t *rail_len = calloc(rails, sizeof(size_t));
        for (size_t i = 0; i < len; ++i) rail_len[rail_map[i]]++;
        size_t idx = 0;
        char **rails_arr = malloc(rails * sizeof(char *));
        for (int r = 0; r < rails; ++r) {
            rails_arr[r] = malloc(rail_len[r] + 1);
            memcpy(rails_arr[r], text + idx, rail_len[r]);
            rails_arr[r][rail_len[r]] = '\0';
            idx += rail_len[r];
        }
        for (size_t i = 0; i < len; ++i) {
            int r = rail_map[i];
            out[i] = rails_arr[r][rail_pos[r]++];
        }
        out[len] = '\0';
        for (int r = 0; r < rails; ++r) free(rails_arr[r]);
        free(rails_arr);
        free(rail_map);
        free(rail_pos);
        free(rail_len);
    }
    return out;
}

/* ------------------------------------------------------------------------- */
/* Haxor Cipher (hex encoding/decoding)                                      */
/* ------------------------------------------------------------------------- */
static cstring cipher_haxor(ccstring text, int decode) {
    if (!decode) {
        size_t len = fossil_io_cstring_length(text);
        cstring out = malloc(len * 2 + 1);
        if (!out) return NULL;
        for (size_t i = 0; i < len; ++i)
            sprintf(out + i * 2, "%02X", (unsigned char)text[i]);
        out[len * 2] = '\0';
        return out;
    } else {
        size_t len = fossil_io_cstring_length(text);
        if (len % 2 != 0) return NULL;
        size_t out_len = len / 2;
        cstring out = malloc(out_len + 1);
        if (!out) return NULL;
        for (size_t i = 0; i < out_len; ++i) {
            unsigned int val;
            sscanf(text + i * 2, "%2X", &val);
            out[i] = (char)val;
        }
        out[out_len] = '\0';
        return out;
    }
}

/* ------------------------------------------------------------------------- */
/* Leet Speak                                                                */
/* ------------------------------------------------------------------------- */
static const char *leet_table[26] = {
    "4", "8", "(", "|)", "3", "|=", "6", "#", "!", "_|", "|<", "1", "|\\/|", "|\\|", "0", "|*", "0_", "|2", "5", "7", "|_|", "\\/", "\\/\\/", "><", "`/", "2"
};

static cstring cipher_leet(ccstring text, int decode) {
    size_t len = fossil_io_cstring_length(text);
    cstring out = malloc(len * 4 + 1);
    if (!out) return NULL;
    char *p = out;
    if (!decode) {
        for (size_t i = 0; i < len; ++i) {
            char c = toupper(text[i]);
            if (c >= 'A' && c <= 'Z') {
                const char *l = leet_table[c - 'A'];
                while (*l) *p++ = *l++;
            } else {
                *p++ = text[i];
            }
        }
        *p = '\0';
    } else {
        // Simple decode: map leet to letter if exact match
        size_t i = 0;
        while (i < len) {
            int found = 0;
            for (int l = 0; l < 26; ++l) {
                size_t leet_len = fossil_io_cstring_length(leet_table[l]);
                if (leet_len > 0 && fossil_io_cstring_compare_safe(text + i, leet_table[l], leet_len) == 0) {
                    *p++ = 'A' + l;
                    i += leet_len;
                    found = 1;
                    break;
                }
            }
            if (!found) *p++ = text[i++];
        }
        *p = '\0';
    }
    return out;
}

/* ------------------------------------------------------------------------- */
/* ROT13                                                                     */
/* ------------------------------------------------------------------------- */
static cstring cipher_rot13(ccstring text, int decode) {
    size_t len = fossil_io_cstring_length(text);
    cstring out = malloc(len + 1);
    if (!out) return NULL;
    for (size_t i = 0; i < len; ++i) {
        char c = text[i];
        if (isupper(c))
            out[i] = ((c - 'A' + 13) % 26) + 'A';
        else if (islower(c))
            out[i] = ((c - 'a' + 13) % 26) + 'a';
        else
            out[i] = c;
    }
    out[len] = '\0';
    return out;
}

/* ------------------------------------------------------------------------- */
/* Atbash                                                                    */
/* ------------------------------------------------------------------------- */
static cstring cipher_atbash(ccstring text, int decode) {
    size_t len = fossil_io_cstring_length(text);
    cstring out = malloc(len + 1);
    if (!out) return NULL;
    for (size_t i = 0; i < len; ++i) {
        char c = text[i];
        if (isupper(c))
            out[i] = 'Z' - (c - 'A');
        else if (islower(c))
            out[i] = 'z' - (c - 'a');
        else
            out[i] = c;
    }
    out[len] = '\0';
    return out;
}
