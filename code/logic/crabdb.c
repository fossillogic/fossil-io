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
#include "fossil/io/crabdb.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ============================================================
 * Portable Threading Layer
 * ============================================================ */

#if defined(_WIN32)

#include <windows.h>

typedef CRITICAL_SECTION crabdb_mutex_t;

static void crabdb_mutex_init(crabdb_mutex_t *m)
{
    InitializeCriticalSection(m);
}

static void crabdb_mutex_lock(crabdb_mutex_t *m)
{
    EnterCriticalSection(m);
}

static void crabdb_mutex_unlock(crabdb_mutex_t *m)
{
    LeaveCriticalSection(m);
}

static void crabdb_mutex_destroy(crabdb_mutex_t *m)
{
    DeleteCriticalSection(m);
}

#elif defined(__unix__) || defined(__APPLE__)

#include <pthread.h>

typedef pthread_mutex_t crabdb_mutex_t;

static void crabdb_mutex_init(crabdb_mutex_t *m)
{
    pthread_mutex_init(m, NULL);
}

static void crabdb_mutex_lock(crabdb_mutex_t *m)
{
    pthread_mutex_lock(m);
}

static void crabdb_mutex_unlock(crabdb_mutex_t *m)
{
    pthread_mutex_unlock(m);
}

static void crabdb_mutex_destroy(crabdb_mutex_t *m)
{
    pthread_mutex_destroy(m);
}

#else

/* Fallback: single-thread no-op */
typedef int crabdb_mutex_t;

static void crabdb_mutex_init(crabdb_mutex_t *m) { (void)m; }
static void crabdb_mutex_lock(crabdb_mutex_t *m) { (void)m; }
static void crabdb_mutex_unlock(crabdb_mutex_t *m) { (void)m; }
static void crabdb_mutex_destroy(crabdb_mutex_t *m) { (void)m; }

#endif

/* ============================================================
 * Internal Structures
 * ============================================================ */

struct crabdb_handle
{
    crabdb_config_t config;
    char *path;
    char errmsg[256];

    crabdb_mutex_t mutex;
};

struct crabdb_stmt
{
    crabdb_handle_t *db;
    char *query;

    crabdb_value_t bindings[32];
    int binding_count;

    crabdb_value_t result_row[16];
    int column_count;

    int step_state;
};

struct crabdb_txn
{
    crabdb_handle_t *db;
    int active;
};

/* ============================================================
 * Helpers
 * ============================================================ */

static int crabdb_is_threaded(crabdb_handle_t *db)
{
    return db && db->config.thread_mode != CRABDB_THREAD_SINGLE;
}

static void crabdb_lock(crabdb_handle_t *db)
{
    if (crabdb_is_threaded(db))
    {
        crabdb_mutex_lock(&db->mutex);
    }
}

static void crabdb_unlock(crabdb_handle_t *db)
{
    if (crabdb_is_threaded(db))
    {
        crabdb_mutex_unlock(&db->mutex);
    }
}

static void crabdb_set_error(crabdb_handle_t *db, const char *msg)
{
    if (db && msg)
    {
        snprintf(db->errmsg, sizeof(db->errmsg), "%s", msg);
    }
}

const char *custom_strdup(const char *s)
{
    if (!s)
        return NULL;

    size_t len = strlen(s);
    char *copy = malloc(len + 1);
    if (copy)
        strcpy(copy, s);
    return copy;
}

/* ============================================================
 * Lifecycle
 * ============================================================ */

int fossil_io_crabdb_open(
    const char *path,
    const crabdb_config_t *config,
    crabdb_handle_t **out_db)
{
    if (!path || !out_db)
        return CRABDB_INVALID;

    crabdb_handle_t *db = calloc(1, sizeof(*db));
    if (!db)
        return CRABDB_NOMEM;

    db->path = custom_strdup(path);
    if (!db->path)
    {
        free(db);
        return CRABDB_NOMEM;
    }

    if (config)
        db->config = *config;
    else
    {
        db->config.thread_mode = CRABDB_THREAD_SINGLE;
        db->config.page_size = 4096;
        db->config.cache_size = 1024;
    }

    crabdb_mutex_init(&db->mutex);

    *out_db = db;
    return CRABDB_OK;
}

int fossil_io_crabdb_close(crabdb_handle_t *db)
{
    if (!db)
        return CRABDB_INVALID;

    crabdb_mutex_destroy(&db->mutex);

    free(db->path);
    free(db);

    return CRABDB_OK;
}

const char *fossil_io_crabdb_errmsg(crabdb_handle_t *db)
{
    return db ? db->errmsg : "no db";
}

/* ============================================================
 * Execution
 * ============================================================ */

/* ============================================================
 * Execution (Tokenizer + Parser + Eval)
 * ============================================================ */

typedef enum
{
    TOK_EOF = 0,
    TOK_IDENT,
    TOK_NUMBER,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_PLUS,
    TOK_MINUS,
    TOK_EQUAL,
    TOK_SEMI,
    TOK_KW_SELECT,
    TOK_KW_LET
} crabdb_token_type_t;

typedef struct
{
    crabdb_token_type_t type;
    const char *start;
    int length;
} crabdb_token_t;

typedef struct
{
    const char *input;
    size_t pos;
} crabdb_lexer_t;

/* ---------- Lexer helpers ---------- */

static int is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c == '_');
}

static int is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

static char lex_peek(crabdb_lexer_t *l)
{
    return l->input[l->pos];
}

static char lex_advance(crabdb_lexer_t *l)
{
    return l->input[l->pos++];
}

static void lex_skip_ws(crabdb_lexer_t *l)
{
    while (lex_peek(l) == ' ' || lex_peek(l) == '\n' || lex_peek(l) == '\t')
        lex_advance(l);
}

static crabdb_token_type_t keyword_type(const char *s, int len)
{
    if (len == 6 && strncmp(s, "select", 6) == 0)
        return TOK_KW_SELECT;
    if (len == 3 && strncmp(s, "let", 3) == 0)
        return TOK_KW_LET;
    return TOK_IDENT;
}

/* ---------- Tokenizer ---------- */

static crabdb_token_t next_token(crabdb_lexer_t *l)
{
    lex_skip_ws(l);

    const char *start = l->input + l->pos;
    char c = lex_advance(l);

    crabdb_token_t tok = {0};
    tok.start = start;
    tok.length = 1;

    switch (c)
    {
    case '\0':
        tok.type = TOK_EOF;
        return tok;
    case '(':
        tok.type = TOK_LPAREN;
        return tok;
    case ')':
        tok.type = TOK_RPAREN;
        return tok;
    case '+':
        tok.type = TOK_PLUS;
        return tok;
    case '-':
        tok.type = TOK_MINUS;
        return tok;
    case '=':
        tok.type = TOK_EQUAL;
        return tok;
    case ';':
        tok.type = TOK_SEMI;
        return tok;
    }

    if (is_digit(c))
    {
        while (is_digit(lex_peek(l)))
            lex_advance(l);
        tok.type = TOK_NUMBER;
        tok.length = (int)((l->input + l->pos) - start);
        return tok;
    }

    if (is_alpha(c))
    {
        while (is_alpha(lex_peek(l)) || is_digit(lex_peek(l)))
            lex_advance(l);

        tok.length = (int)((l->input + l->pos) - start);
        tok.type = keyword_type(start, tok.length);
        return tok;
    }

    tok.type = TOK_EOF;
    return tok;
}

/* ============================================================
 * AST
 * ============================================================ */

typedef enum
{
    CRABDB_EXPR_LITERAL,
    CRABDB_EXPR_ADD,
    CRABDB_EXPR_SUB
} crabdb_expr_kind_t;

typedef struct crabdb_expr
{
    crabdb_expr_kind_t kind;
    crabdb_value_t value;
    struct crabdb_expr *left;
    struct crabdb_expr *right;
} crabdb_expr_t;

/* ============================================================
 * Parser
 * ============================================================ */

typedef struct
{
    crabdb_lexer_t lexer;
    crabdb_token_t current;
} crabdb_parser_t;

static void parser_init(crabdb_parser_t *p, const char *input)
{
    p->lexer.input = input;
    p->lexer.pos = 0;
    p->current = next_token(&p->lexer);
}

static void parser_advance(crabdb_parser_t *p)
{
    p->current = next_token(&p->lexer);
}

/* ---------- Literal ---------- */

static crabdb_expr_t *parse_literal(crabdb_parser_t *p)
{
    if (p->current.type == TOK_NUMBER)
    {
        crabdb_expr_t *e = calloc(1, sizeof(*e));
        e->kind = CRABDB_EXPR_LITERAL;

        e->value.type = CRABDB_TYPE_I64;
        e->value.as.i64 = strtoll(p->current.start, NULL, 10);

        parser_advance(p);
        return e;
    }

    /* typed literal: i32(123) */
    if (p->current.type == TOK_IDENT)
    {
        const char *name = p->current.start;
        int len = p->current.length;

        parser_advance(p);

        if (p->current.type == TOK_LPAREN)
        {
            parser_advance(p);

            if (p->current.type != TOK_NUMBER)
                return NULL;

            int64_t v = strtoll(p->current.start, NULL, 10);
            parser_advance(p);

            if (p->current.type != TOK_RPAREN)
                return NULL;

            parser_advance(p);

            crabdb_expr_t *e = calloc(1, sizeof(*e));
            e->kind = CRABDB_EXPR_LITERAL;

            if (len == 3 && strncmp(name, "i32", 3) == 0)
                e->value.type = CRABDB_TYPE_I32;
            else if (len == 3 && strncmp(name, "u64", 3) == 0)
                e->value.type = CRABDB_TYPE_U64;
            else
                e->value.type = CRABDB_TYPE_I64;

            e->value.as.i64 = v;
            return e;
        }
    }

    return NULL;
}

/* ---------- Expression ---------- */

static crabdb_expr_t *parse_expr(crabdb_parser_t *p)
{
    crabdb_expr_t *left = parse_literal(p);
    if (!left)
        return NULL;

    while (p->current.type == TOK_PLUS ||
           p->current.type == TOK_MINUS)
    {

        crabdb_token_type_t op = p->current.type;
        parser_advance(p);

        crabdb_expr_t *right = parse_literal(p);
        if (!right)
            return left;

        crabdb_expr_t *e = calloc(1, sizeof(*e));
        e->kind = (op == TOK_PLUS)
                      ? CRABDB_EXPR_ADD
                      : CRABDB_EXPR_SUB;

        e->left = left;
        e->right = right;

        left = e;
    }

    return left;
}

/* ============================================================
 * Evaluator
 * ============================================================ */

static crabdb_value_t eval_expr(crabdb_expr_t *e)
{
    crabdb_value_t out = {0};

    switch (e->kind)
    {
    case CRABDB_EXPR_LITERAL:
        return e->value;

    case CRABDB_EXPR_ADD:
    {
        crabdb_value_t a = eval_expr(e->left);
        crabdb_value_t b = eval_expr(e->right);
        out.type = CRABDB_TYPE_I64;
        out.as.i64 = a.as.i64 + b.as.i64;
        return out;
    }

    case CRABDB_EXPR_SUB:
    {
        crabdb_value_t a = eval_expr(e->left);
        crabdb_value_t b = eval_expr(e->right);
        out.type = CRABDB_TYPE_I64;
        out.as.i64 = a.as.i64 - b.as.i64;
        return out;
    }
    }

    return out;
}

/* ============================================================
 * Simple Symbol Table (LET)
 * ============================================================ */

typedef struct
{
    char name[32];
    crabdb_value_t value;
} crabdb_symbol_t;

static crabdb_symbol_t g_symbols[64];
static int g_symbol_count = 0;

/* ============================================================
 * Statements
 * ============================================================ */

static int parse_let(crabdb_parser_t *p)
{
    parser_advance(p); /* let */

    if (p->current.type != TOK_IDENT)
        return CRABDB_ERROR;

    char name[32] = {0};
    strncpy(name, p->current.start, p->current.length);

    parser_advance(p);

    if (p->current.type != TOK_EQUAL)
        return CRABDB_ERROR;

    parser_advance(p);

    crabdb_expr_t *expr = parse_expr(p);
    if (!expr)
        return CRABDB_ERROR;

    crabdb_value_t v = eval_expr(expr);

    strncpy(g_symbols[g_symbol_count].name, name, 31);
    g_symbols[g_symbol_count].value = v;
    g_symbol_count++;

    return CRABDB_OK;
}

static int parse_select(crabdb_parser_t *p, crabdb_stmt_t *stmt)
{
    parser_advance(p); /* select */

    crabdb_expr_t *expr = parse_expr(p);
    if (!expr)
        return CRABDB_ERROR;

    crabdb_value_t v = eval_expr(expr);

    stmt->column_count = 1;
    stmt->result_row[0] = v;

    return CRABDB_OK;
}

/* ============================================================
 * Execution
 * ============================================================ */

/* --- Internal helpers --- */

static void crabdb_expr_free(crabdb_expr_t *e) {
    if (!e) return;
    crabdb_expr_free(e->left);
    crabdb_expr_free(e->right);
    free(e);
}

/* --- exec --- */

int fossil_io_crabdb_exec(crabdb_handle_t *db, const char *query)
{
    if (!db || !query)
        return CRABDB_INVALID;

    crabdb_stmt_t *stmt = NULL;

    int rc = fossil_io_crabdb_prepare(db, query, &stmt);
    if (rc != CRABDB_OK)
        return rc;

    while ((rc = fossil_io_crabdb_step(stmt)) == CRABDB_ROW) {
        /* intentionally discard results */
    }

    fossil_io_crabdb_finalize(stmt);
    return (rc == CRABDB_DONE) ? CRABDB_OK : rc;
}

/* --- prepare --- */

int fossil_io_crabdb_prepare(
    crabdb_handle_t *db,
    const char *query,
    crabdb_stmt_t **out_stmt)
{
    if (!db || !query || !out_stmt)
        return CRABDB_INVALID;

    crabdb_stmt_t *stmt = calloc(1, sizeof(*stmt));
    if (!stmt)
        return CRABDB_NOMEM;

    stmt->db = db;
    stmt->query = custom_strdup(query);
    if (!stmt->query) {
        free(stmt);
        return CRABDB_NOMEM;
    }

    const char *s = skip_ws(query);

    /* --- SELECT expression --- */
    if (strncmp(s, "select", 6) == 0) {
        s += 6;

        crabdb_expr_t *expr = parse_expr(s);

        if (!expr) {
            crabdb_set_error(db, "parse error");
            free(stmt->query);
            free(stmt);
            return CRABDB_ERROR;
        }

        crabdb_value_t v = eval_expr(expr);
        crabdb_expr_free(expr);

        stmt->column_count = 1;
        stmt->result_row[0] = v;
    }
    else {
        crabdb_set_error(db, "unknown query");
        free(stmt->query);
        free(stmt);
        return CRABDB_ERROR;
    }

    *out_stmt = stmt;
    return CRABDB_OK;
}

/* --- step --- */

int fossil_io_crabdb_step(crabdb_stmt_t *stmt)
{
    if (!stmt)
        return CRABDB_INVALID;

    if (stmt->step_state++ == 0)
        return CRABDB_ROW;

    return CRABDB_DONE;
}

/* --- finalize --- */

int fossil_io_crabdb_finalize(crabdb_stmt_t *stmt)
{
    if (!stmt)
        return CRABDB_INVALID;

    free(stmt->query);
    free(stmt);
    return CRABDB_OK;
}

/* --- reset --- */

int fossil_io_crabdb_reset(crabdb_stmt_t *stmt)
{
    if (!stmt)
        return CRABDB_INVALID;

    stmt->step_state = 0;
    return CRABDB_OK;
}

/* ============================================================
 * Binding Parameters (Typed)
 * ============================================================ */

int fossil_io_crabdb_bind_i8(crabdb_stmt_t *stmt, int index, int8_t value)
{
    if (!stmt || index <= 0 || index > 32)
        return CRABDB_INVALID;

    stmt->bindings[index - 1].type = CRABDB_TYPE_I8;
    stmt->bindings[index - 1].as.i8 = value;
    if (index > stmt->binding_count)
        stmt->binding_count = index;

    return CRABDB_OK;
}

int fossil_io_crabdb_bind_i16(crabdb_stmt_t *stmt, int index, int16_t value)
{
    if (!stmt || index <= 0 || index > 32)
        return CRABDB_INVALID;

    stmt->bindings[index - 1].type = CRABDB_TYPE_I16;
    stmt->bindings[index - 1].as.i16 = value;
    if (index > stmt->binding_count)
        stmt->binding_count = index;

    return CRABDB_OK;
}

int fossil_io_crabdb_bind_i32(crabdb_stmt_t *stmt, int index, int32_t value)
{
    if (!stmt || index <= 0 || index > 32)
        return CRABDB_INVALID;

    stmt->bindings[index - 1].type = CRABDB_TYPE_I32;
    stmt->bindings[index - 1].as.i32 = value;
    if (index > stmt->binding_count)
        stmt->binding_count = index;

    return CRABDB_OK;
}

int fossil_io_crabdb_bind_i64(crabdb_stmt_t *stmt, int index, int64_t value)
{
    if (!stmt || index <= 0 || index > 32)
        return CRABDB_INVALID;

    stmt->bindings[index - 1].type = CRABDB_TYPE_I64;
    stmt->bindings[index - 1].as.i64 = value;
    if (index > stmt->binding_count)
        stmt->binding_count = index;

    return CRABDB_OK;
}

int fossil_io_crabdb_bind_u8(crabdb_stmt_t *stmt, int index, uint8_t value)
{
    if (!stmt || index <= 0 || index > 32)
        return CRABDB_INVALID;

    stmt->bindings[index - 1].type = CRABDB_TYPE_U8;
    stmt->bindings[index - 1].as.u8 = value;
    if (index > stmt->binding_count)
        stmt->binding_count = index;

    return CRABDB_OK;
}

int fossil_io_crabdb_bind_u16(crabdb_stmt_t *stmt, int index, uint16_t value)
{
    if (!stmt || index <= 0 || index > 32)
        return CRABDB_INVALID;

    stmt->bindings[index - 1].type = CRABDB_TYPE_U16;
    stmt->bindings[index - 1].as.u16 = value;
    if (index > stmt->binding_count)
        stmt->binding_count = index;

    return CRABDB_OK;
}

int fossil_io_crabdb_bind_u32(crabdb_stmt_t *stmt, int index, uint32_t value)
{
    if (!stmt || index <= 0 || index > 32)
        return CRABDB_INVALID;

    stmt->bindings[index - 1].type = CRABDB_TYPE_U32;
    stmt->bindings[index - 1].as.u32 = value;
    if (index > stmt->binding_count)
        stmt->binding_count = index;

    return CRABDB_OK;
}

int fossil_io_crabdb_bind_u64(crabdb_stmt_t *stmt, int index, uint64_t value)
{
    if (!stmt || index <= 0 || index > 32)
        return CRABDB_INVALID;

    stmt->bindings[index - 1].type = CRABDB_TYPE_U64;
    stmt->bindings[index - 1].as.u64 = value;
    if (index > stmt->binding_count)
        stmt->binding_count = index;

    return CRABDB_OK;
}

int fossil_io_crabdb_bind_f32(crabdb_stmt_t *stmt, int index, float value)
{
    if (!stmt || index <= 0 || index > 32)
        return CRABDB_INVALID;

    stmt->bindings[index - 1].type = CRABDB_TYPE_F32;
    stmt->bindings[index - 1].as.f32 = value;
    if (index > stmt->binding_count)
        stmt->binding_count = index;

    return CRABDB_OK;
}

int fossil_io_crabdb_bind_f64(crabdb_stmt_t *stmt, int index, double value)
{
    if (!stmt || index <= 0 || index > 32)
        return CRABDB_INVALID;

    stmt->bindings[index - 1].type = CRABDB_TYPE_F64;
    stmt->bindings[index - 1].as.f64 = value;
    if (index > stmt->binding_count)
        stmt->binding_count = index;

    return CRABDB_OK;
}

int fossil_io_crabdb_bind_text(
    crabdb_stmt_t *stmt, int index, const char *value)
{
    if (!stmt || index <= 0 || index > 32)
        return CRABDB_INVALID;

    stmt->bindings[index - 1].type = CRABDB_TYPE_TEXT;
    strcpy(stmt->bindings[index - 1].as.s, value);
    if (index > stmt->binding_count)
        stmt->binding_count = index;

    return CRABDB_OK;
}

int fossil_io_crabdb_bind_blob(
    crabdb_stmt_t *stmt, int index, const void *data, size_t size)
{
    if (!stmt || index <= 0 || index > 32)
        return CRABDB_INVALID;

    stmt->bindings[index - 1].type = CRABDB_TYPE_BLOB;
    stmt->bindings[index - 1].as.blob.data = data;
    stmt->bindings[index - 1].as.blob.size = size;
    if (index > stmt->binding_count)
        stmt->binding_count = index;

    return CRABDB_OK;
}

int fossil_io_crabdb_bind_null(
    crabdb_stmt_t *stmt, int index)
{
    if (!stmt || index <= 0 || index > 32)
        return CRABDB_INVALID;

    stmt->bindings[index - 1].type = CRABDB_TYPE_NULL;
    if (index > stmt->binding_count)
        stmt->binding_count = index;

    return CRABDB_OK;
}

/* ============================================================
 * Column Access (Typed)
 * ============================================================ */

int fossil_io_crabdb_column_count(crabdb_stmt_t *stmt)
{
    if (!stmt)
        return CRABDB_INVALID;

    return stmt->column_count;
}

crabdb_type_t fossil_io_crabdb_column_type(crabdb_stmt_t *stmt, int col)
{
    if (!stmt || col < 0 || col >= stmt->column_count)
        return CRABDB_TYPE_NULL;

    return stmt->result_row[col].type;
}

int8_t fossil_io_crabdb_column_i8(crabdb_stmt_t *stmt, int col)
{
    if (!stmt || col < 0 || col >= stmt->column_count)
        return 0;

    crabdb_value_t *val = &stmt->result_row[col];
    if (val->type != CRABDB_TYPE_I8)
        return 0;

    return val->as.i8;
}

int16_t fossil_io_crabdb_column_i16(crabdb_stmt_t *stmt, int col)
{
    if (!stmt || col < 0 || col >= stmt->column_count)
        return 0;

    crabdb_value_t *val = &stmt->result_row[col];
    if (val->type != CRABDB_TYPE_I16)
        return 0;

    return val->as.i16;
}

int32_t fossil_io_crabdb_column_i32(crabdb_stmt_t *stmt, int col)
{
    if (!stmt || col < 0 || col >= stmt->column_count)
        return 0;

    crabdb_value_t *val = &stmt->result_row[col];
    if (val->type != CRABDB_TYPE_I32)
        return 0;

    return val->as.i32;
}

int64_t fossil_io_crabdb_column_i64(crabdb_stmt_t *stmt, int col)
{
    if (!stmt || col < 0 || col >= stmt->column_count)
        return 0;

    crabdb_value_t *val = &stmt->result_row[col];
    if (val->type != CRABDB_TYPE_I64)
        return 0;

    return val->as.i64;
}

uint8_t fossil_io_crabdb_column_u8(crabdb_stmt_t *stmt, int col)
{
    if (!stmt || col < 0 || col >= stmt->column_count)
        return 0;

    crabdb_value_t *val = &stmt->result_row[col];
    if (val->type != CRABDB_TYPE_U8)
        return 0;

    return val->as.u8;
}

uint16_t fossil_io_crabdb_column_u16(crabdb_stmt_t *stmt, int col)
{
    if (!stmt || col < 0 || col >= stmt->column_count)
        return 0;

    crabdb_value_t *val = &stmt->result_row[col];
    if (val->type != CRABDB_TYPE_U16)
        return 0;

    return val->as.u16;
}

uint32_t fossil_io_crabdb_column_u32(crabdb_stmt_t *stmt, int col)
{
    if (!stmt || col < 0 || col >= stmt->column_count)
        return 0;

    crabdb_value_t *val = &stmt->result_row[col];
    if (val->type != CRABDB_TYPE_U32)
        return 0;

    return val->as.u32;
}

uint64_t fossil_io_crabdb_column_u64(crabdb_stmt_t *stmt, int col)
{
    if (!stmt || col < 0 || col >= stmt->column_count)
        return 0;

    crabdb_value_t *val = &stmt->result_row[col];
    if (val->type != CRABDB_TYPE_U64)
        return 0;

    return val->as.u64;
}

float fossil_io_crabdb_column_f32(crabdb_stmt_t *stmt, int col)
{
    if (!stmt || col < 0 || col >= stmt->column_count)
        return 0.0f;

    crabdb_value_t *val = &stmt->result_row[col];
    if (val->type != CRABDB_TYPE_F32)
        return 0.0f;

    return val->as.f32;
}

double fossil_io_crabdb_column_f64(crabdb_stmt_t *stmt, int col)
{
    if (!stmt || col < 0 || col >= stmt->column_count)
        return 0.0f;

    crabdb_value_t *val = &stmt->result_row[col];
    if (val->type != CRABDB_TYPE_F32)
        return 0.0f;

    return val->as.f32;
}

const char *fossil_io_crabdb_column_text(crabdb_stmt_t *stmt, int col)
{
    if (!stmt || col < 0 || col >= stmt->column_count)
        return NULL;

    crabdb_value_t *val = &stmt->result_row[col];
    if (val->type != CRABDB_TYPE_TEXT)
        return NULL;

    return val->as.s;
}

const void *fossil_io_crabdb_column_blob(crabdb_stmt_t *stmt, size_t *out_size)
{
    if (!stmt || !out_size)
        return NULL;

    *out_size = 0;
    return NULL;
}

/* ============================================================
 * Transactions
 * ============================================================ */

int fossil_io_crabdb_begin(crabdb_handle_t *db, crabdb_txn_t **out)
{
    if (!db || !out)
        return CRABDB_INVALID;

    crabdb_txn_t *txn = calloc(1, sizeof(*txn));
    txn->db = db;
    txn->active = 1;

    *out = txn;
    return CRABDB_OK;
}

int fossil_io_crabdb_commit(crabdb_txn_t *txn)
{
    if (!txn || !txn->active)
        return CRABDB_INVALID;

    txn->active = 0;
    free(txn);

    return CRABDB_OK;
}

int fossil_io_crabdb_rollback(crabdb_txn_t *txn)
{
    if (!txn || !txn->active)
        return CRABDB_INVALID;

    txn->active = 0;
    free(txn);

    return CRABDB_OK;
}

/* ============================================================
 * Family Tree (unchanged logic, safe)
 * ============================================================ */

int fossil_io_crabdb_create_family(crabdb_handle_t *db,
                                   const char *name, const char *schema)
{
    if (!db || !name || !schema)
        return CRABDB_INVALID;
    return CRABDB_OK;
}

int fossil_io_crabdb_relate(crabdb_handle_t *db,
                            const char *pf, int64_t pid,
                            const char *cf, int64_t cid)
{

    if (!db || !pf || !cf)
        return CRABDB_INVALID;

    if (pid == cid)
    {
        crabdb_set_error(db, "self relation");
    }

    return CRABDB_OK;
}

int fossil_io_crabdb_query_lineage(
    crabdb_handle_t *db,
    const char *family,
    int64_t id,
    crabdb_stmt_t **out_stmt)
{
    if (!db || !family || !out_stmt)
        return CRABDB_INVALID;

    char buf[64];
    snprintf(buf, sizeof(buf), "lineage:%s:%lld", family, (long long)id);

    return fossil_io_crabdb_prepare(db, buf, out_stmt);
}

/* ============================================================
 * Utility
 * ============================================================ */

const char *fossil_io_crabdb_version(void)
{
    return "CrabDB 0.1.0";
}

int fossil_io_crabdb_init(void)
{
    return CRABDB_OK;
}

int fossil_io_crabdb_shutdown(void)
{
    return CRABDB_OK;
}
