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
