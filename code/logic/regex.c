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
#include "fossil/io/regex.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ============================================================================
 * Internal Option Resolution (String IDs)
 * ============================================================================
 */

typedef unsigned int fossil_rx_optmask_t;

#define RX_OPT_ICASE      (1u << 0)
#define RX_OPT_MULTILINE  (1u << 1)
#define RX_OPT_DOTALL     (1u << 2)
#define RX_OPT_UNGREEDY   (1u << 3)
#define RX_OPT_ANCHORED   (1u << 4)

static const struct {
    const char *id;
    fossil_rx_optmask_t mask;
} fossil_rx_options[] = {
    { "icase",     RX_OPT_ICASE     },
    { "multiline", RX_OPT_MULTILINE },
    { "dotall",    RX_OPT_DOTALL    },
    { "ungreedy",  RX_OPT_UNGREEDY  },
    { "anchored",  RX_OPT_ANCHORED  },
    { NULL, 0 }
};

static fossil_rx_optmask_t fossil_io_regex_resolve_options(const char **ids) {
    fossil_rx_optmask_t mask = 0;

    if (!ids)
        return 0;

    for (; *ids; ids++) {
        for (int i = 0; fossil_rx_options[i].id; i++) {
            if (strcmp(*ids, fossil_rx_options[i].id) == 0) {
                mask |= fossil_rx_options[i].mask;
                break;
            }
        }
    }
    return mask;
}

/* ============================================================================
 * Bytecode VM
 * ============================================================================
 */

typedef enum {
    RX_OP_CHAR,
    RX_OP_ANY,
    RX_OP_JUMP,
    RX_OP_SPLIT,
    RX_OP_MATCH,
    RX_OP_SAVE,
    RX_OP_ASSERT_BEGIN,
    RX_OP_ASSERT_END
} fossil_rx_opcode_t;

typedef struct {
    fossil_rx_opcode_t op;
    int x;
    int y;
    int c;
} fossil_rx_inst_t;

/* ============================================================================
 * Compiled Regex Object
 * ============================================================================
 */

struct fossil_io_regex {
    fossil_rx_inst_t *prog;
    int prog_len;
    int cap_count;
    fossil_rx_optmask_t options;
};

/* ============================================================================
 * Match Object
 * ============================================================================
 */

struct fossil_io_regex_match {
    int matched;
    const char *start;
    const char *end;

    int group_count;
    const char **groups;
};

/* ============================================================================
 * Utility
 * ============================================================================
 */

static char *fossil_rx_strdup(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = (char *)malloc(n);
    if (p)
        memcpy(p, s, n);
    return p;
}

/* ============================================================================
 * Enhanced Compiler (literal + . ^ $)
 * ============================================================================
 */

static fossil_rx_inst_t *fossil_rx_compile_basic(
    const char *pattern,
    int *out_len) {
    int len = (int)strlen(pattern);
    fossil_rx_inst_t *prog;
    int pc = 0;

    prog = (fossil_rx_inst_t *)
        calloc((size_t)len + 2, sizeof(*prog));

    if (!prog)
        return NULL;

    for (int i = 0; i < len; i++) {
        char ch = pattern[i];

        if (ch == '.' ) {
            prog[pc++].op = RX_OP_ANY;
        }
        else if (ch == '^' && i == 0) {
            prog[pc++].op = RX_OP_ASSERT_BEGIN;
        }
        else if (ch == '$' && i == len - 1) {
            prog[pc++].op = RX_OP_ASSERT_END;
        }
        else {
            prog[pc].op = RX_OP_CHAR;
            prog[pc].c  = (unsigned char)ch;
            pc++;
        }
    }

    prog[pc++].op = RX_OP_MATCH;
    *out_len = pc;
    return prog;
}

/* ============================================================================
 * VM Execution
 * ============================================================================
 */

static int fossil_rx_vm_exec(
    const fossil_rx_inst_t *prog,
    int pc,
    const char *sp,
    fossil_io_regex_match_t *m,
    fossil_rx_optmask_t opts) {
    for (;;) {
        const fossil_rx_inst_t *ins = &prog[pc];

        switch (ins->op) {

        case RX_OP_CHAR: {
            char a = *sp;
            char b = (char)ins->c;

            if (!a)
                return 0;

            if (opts & RX_OPT_ICASE) {
                a = (char)tolower((unsigned char)a);
                b = (char)tolower((unsigned char)b);
            }

            if (a != b)
                return 0;

            sp++;
            pc++;
            break;
        }

        case RX_OP_ANY:
            if (*sp == '\0')
                return 0;
            sp++;
            pc++;
            break;

        case RX_OP_JUMP:
            pc = ins->x;
            break;

        case RX_OP_SPLIT:
            if (fossil_rx_vm_exec(prog, ins->x, sp, m, opts))
                return 1;
            pc = ins->y;
            break;

        case RX_OP_ASSERT_BEGIN:
            if (sp != m->start)
                return 0;
            pc++;
            break;

        case RX_OP_ASSERT_END:
            if (*sp != '\0')
                return 0;
            pc++;
            break;

        case RX_OP_MATCH:
            m->matched = 1;
            m->end = sp;
            return 1;

        default:
            return -1;
        }
    }
}

/* ============================================================================
 * Public API
 * ============================================================================
 */

fossil_io_regex_t *fossil_io_regex_compile(
    const char *pattern,
    const char **options,
    char **error_out) {
    fossil_io_regex_t *re;

    if (!pattern) {
        if (error_out)
            *error_out = fossil_rx_strdup("null pattern");
        return NULL;
    }

    re = (fossil_io_regex_t *)calloc(1, sizeof(*re));
    if (!re)
        return NULL;

    re->options = fossil_io_regex_resolve_options(options);

    re->prog = fossil_rx_compile_basic(pattern, &re->prog_len);
    if (!re->prog) {
        if (error_out)
            *error_out = fossil_rx_strdup("compile failed");
        free(re);
        return NULL;
    }

    return re;
}

void fossil_io_regex_free(fossil_io_regex_t *re) {
    if (!re)
        return;

    free(re->prog);
    free(re);
}

int fossil_io_regex_match(
    const fossil_io_regex_t *re,
    const char *text,
    fossil_io_regex_match_t **out_match) {
    fossil_io_regex_match_t *m;
    int rc = 0;

    if (!re || !text)
        return -1;

    m = (fossil_io_regex_match_t *)
        calloc(1, sizeof(*m));

    if (!m)
        return -1;

    if (re->options & RX_OPT_ANCHORED) {
        m->start = text;
        rc = fossil_rx_vm_exec(
            re->prog, 0, text, m, re->options);
    } else {
        for (const char *p = text; *p && !rc; p++) {
            m->start = p;
            rc = fossil_rx_vm_exec(
                re->prog, 0, p, m, re->options);
        }
    }

    if (!rc) {
        fossil_io_regex_match_free(m);
        if (out_match)
            *out_match = NULL;
        return 0;
    }

    if (out_match)
        *out_match = m;
    else
        fossil_io_regex_match_free(m);

    return 1;
}

void fossil_io_regex_match_free(fossil_io_regex_match_t *m) {
    if (!m)
        return;

    free(m->groups);
    free(m);
}

int fossil_io_regex_group_count(const fossil_io_regex_match_t *m) {
    return m ? m->group_count : 0;
}

const char *fossil_io_regex_group(
    const fossil_io_regex_match_t *m,
    int index) {
    if (!m || index < 0 || index >= m->group_count)
        return NULL;
    return m->groups[index];
}
