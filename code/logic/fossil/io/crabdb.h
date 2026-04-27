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
#ifndef FOSSIL_IO_CRABDB_H
#define FOSSIL_IO_CRABDB_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>

#include <stdint.h>

/* ============================================================

 * Visibility / API macro

 * ============================================================ */

#ifndef FOSSIL_IO_CRABDB_API

#define FOSSIL_IO_CRABDB_API

#endif

/* ============================================================

 * Opaque Types

 * ============================================================ */

typedef struct crabdb_handle crabdb_handle_t;

typedef struct crabdb_stmt crabdb_stmt_t;

typedef struct crabdb_txn crabdb_txn_t;

/* ============================================================

 * Result Codes

 * ============================================================ */

typedef enum {

    CRABDB_OK = 0,

    CRABDB_ERROR = -1,

    CRABDB_BUSY = -2,

    CRABDB_NOMEM = -3,

    CRABDB_INVALID = -4,

    CRABDB_NOTFOUND = -5,

    CRABDB_DONE = 1,

    CRABDB_ROW = 2

} crabdb_result_t;

/* ============================================================

 * Threading Modes

 * ============================================================ */

typedef enum {

    CRABDB_THREAD_SINGLE = 0,

    CRABDB_THREAD_SERIALIZED = 1,

    CRABDB_THREAD_MULTI = 2

} crabdb_thread_mode_t;

/* ============================================================

 * Data Types (Extended Numeric System)

 * ============================================================ */

typedef enum {

    CRABDB_TYPE_NULL = 0,

    /* Signed integers */

    CRABDB_TYPE_I8,

    CRABDB_TYPE_I16,

    CRABDB_TYPE_I32,

    CRABDB_TYPE_I64,

    /* Unsigned integers */

    CRABDB_TYPE_U8,

    CRABDB_TYPE_U16,

    CRABDB_TYPE_U32,

    CRABDB_TYPE_U64,

    /* Floating point */

    CRABDB_TYPE_F32,

    CRABDB_TYPE_F64,

    /* Other */

    CRABDB_TYPE_BOOL,

    CRABDB_TYPE_TEXT,

    CRABDB_TYPE_BLOB,

    CRABDB_TYPE_DATE,

    CRABDB_TYPE_JSON,

    CRABDB_TYPE_UUID

} crabdb_type_t;

/* ============================================================
 * Numeric Range Metadata
 * ============================================================ */

typedef struct crabdb_numeric_range_signed_t {
    int64_t min;
    int64_t max;
} crabdb_numeric_range_signed_t;

typedef struct crabdb_numeric_range_unsigned_t {
    uint64_t min;
    uint64_t max;
} crabdb_numeric_range_unsigned_t;

typedef struct crabdb_numeric_range {
    uint8_t bits;        /* 8, 16, 32, 64 */
    uint8_t is_signed;   /* 1 = signed, 0 = unsigned */

    union {
        crabdb_numeric_range_signed_t s;
        crabdb_numeric_range_unsigned_t u;
    } range;

} crabdb_numeric_range_t;

/* ============================================================

 * Value Container

 * ============================================================ */

typedef struct crabdb_value {

    crabdb_type_t type;

    union {

        int64_t     i64;

        uint64_t    u64;

        double      f64;

        float       f32;

        const char *s;

        struct {

            const void *data;

            size_t size;

        } blob;

        int         b;

    } as;

} crabdb_value_t;

/* ============================================================

 * Configuration

 * ============================================================ */

typedef struct crabdb_config {

    crabdb_thread_mode_t thread_mode;

    int enable_wal;

    int enable_foreign_keys;

    size_t page_size;

    size_t cache_size;

} crabdb_config_t;

/* ============================================================

 * Core Lifecycle

 * ============================================================ */

FOSSIL_IO_CRABDB_API int

fossil_io_crabdb_open(

    const char *path,

    const crabdb_config_t *config,

    crabdb_handle_t **out_db

);

FOSSIL_IO_CRABDB_API int

fossil_io_crabdb_close(

    crabdb_handle_t *db

);

FOSSIL_IO_CRABDB_API const char *

fossil_io_crabdb_errmsg(

    crabdb_handle_t *db

);

/* ============================================================

 * Execution API

 * ============================================================ */

FOSSIL_IO_CRABDB_API int

fossil_io_crabdb_exec(

    crabdb_handle_t *db,

    const char *query

);

FOSSIL_IO_CRABDB_API int

fossil_io_crabdb_prepare(

    crabdb_handle_t *db,

    const char *query,

    crabdb_stmt_t **out_stmt

);

FOSSIL_IO_CRABDB_API int

fossil_io_crabdb_step(

    crabdb_stmt_t *stmt

);

FOSSIL_IO_CRABDB_API int

fossil_io_crabdb_finalize(

    crabdb_stmt_t *stmt

);

FOSSIL_IO_CRABDB_API int

fossil_io_crabdb_reset(

    crabdb_stmt_t *stmt

);

/* ============================================================

 * Binding Parameters (Typed)

 * ============================================================ */

FOSSIL_IO_CRABDB_API int fossil_io_crabdb_bind_i8  (crabdb_stmt_t*, int, int8_t);

FOSSIL_IO_CRABDB_API int fossil_io_crabdb_bind_i16 (crabdb_stmt_t*, int, int16_t);

FOSSIL_IO_CRABDB_API int fossil_io_crabdb_bind_i32 (crabdb_stmt_t*, int, int32_t);

FOSSIL_IO_CRABDB_API int fossil_io_crabdb_bind_i64 (crabdb_stmt_t*, int, int64_t);

FOSSIL_IO_CRABDB_API int fossil_io_crabdb_bind_u8  (crabdb_stmt_t*, int, uint8_t);

FOSSIL_IO_CRABDB_API int fossil_io_crabdb_bind_u16 (crabdb_stmt_t*, int, uint16_t);

FOSSIL_IO_CRABDB_API int fossil_io_crabdb_bind_u32 (crabdb_stmt_t*, int, uint32_t);

FOSSIL_IO_CRABDB_API int fossil_io_crabdb_bind_u64 (crabdb_stmt_t*, int, uint64_t);

FOSSIL_IO_CRABDB_API int fossil_io_crabdb_bind_f32 (crabdb_stmt_t*, int, float);

FOSSIL_IO_CRABDB_API int fossil_io_crabdb_bind_f64 (crabdb_stmt_t*, int, double);

FOSSIL_IO_CRABDB_API int fossil_io_crabdb_bind_text(

    crabdb_stmt_t *stmt, int index, const char *value);

FOSSIL_IO_CRABDB_API int fossil_io_crabdb_bind_blob(

    crabdb_stmt_t *stmt, int index, const void *data, size_t size);

FOSSIL_IO_CRABDB_API int fossil_io_crabdb_bind_null(

    crabdb_stmt_t *stmt, int index);

/* ============================================================

 * Column Access (Typed)

 * ============================================================ */

FOSSIL_IO_CRABDB_API int fossil_io_crabdb_column_count(crabdb_stmt_t*);

FOSSIL_IO_CRABDB_API crabdb_type_t

fossil_io_crabdb_column_type(crabdb_stmt_t*, int col);

FOSSIL_IO_CRABDB_API int8_t   fossil_io_crabdb_column_i8 (crabdb_stmt_t*, int);

FOSSIL_IO_CRABDB_API int16_t  fossil_io_crabdb_column_i16(crabdb_stmt_t*, int);

FOSSIL_IO_CRABDB_API int32_t  fossil_io_crabdb_column_i32(crabdb_stmt_t*, int);

FOSSIL_IO_CRABDB_API int64_t  fossil_io_crabdb_column_i64(crabdb_stmt_t*, int);

FOSSIL_IO_CRABDB_API uint8_t  fossil_io_crabdb_column_u8 (crabdb_stmt_t*, int);

FOSSIL_IO_CRABDB_API uint16_t fossil_io_crabdb_column_u16(crabdb_stmt_t*, int);

FOSSIL_IO_CRABDB_API uint32_t fossil_io_crabdb_column_u32(crabdb_stmt_t*, int);

FOSSIL_IO_CRABDB_API uint64_t fossil_io_crabdb_column_u64(crabdb_stmt_t*, int);

FOSSIL_IO_CRABDB_API float    fossil_io_crabdb_column_f32(crabdb_stmt_t*, int);

FOSSIL_IO_CRABDB_API double   fossil_io_crabdb_column_f64(crabdb_stmt_t*, int);

FOSSIL_IO_CRABDB_API const char *

fossil_io_crabdb_column_text(crabdb_stmt_t*, int);

FOSSIL_IO_CRABDB_API const void *

fossil_io_crabdb_column_blob(crabdb_stmt_t*, int, size_t *out_size);

/* ============================================================

 * Transactions

 * ============================================================ */

FOSSIL_IO_CRABDB_API int

fossil_io_crabdb_begin(crabdb_handle_t*, crabdb_txn_t**);

FOSSIL_IO_CRABDB_API int

fossil_io_crabdb_commit(crabdb_txn_t*);

FOSSIL_IO_CRABDB_API int

fossil_io_crabdb_rollback(crabdb_txn_t*);

/* ============================================================

 * Family Tree Model

 * ============================================================ */

FOSSIL_IO_CRABDB_API int

fossil_io_crabdb_create_family(

    crabdb_handle_t *db,

    const char *name,

    const char *schema

);

FOSSIL_IO_CRABDB_API int

fossil_io_crabdb_relate(

    crabdb_handle_t *db,

    const char *parent_family,

    int64_t parent_id,

    const char *child_family,

    int64_t child_id

);

FOSSIL_IO_CRABDB_API int

fossil_io_crabdb_query_lineage(

    crabdb_handle_t *db,

    const char *family,

    int64_t id,

    crabdb_stmt_t **out_stmt

);

/* ============================================================

 * Type Introspection

 * ============================================================ */

FOSSIL_IO_CRABDB_API int

fossil_io_crabdb_type_bits(crabdb_type_t type);

FOSSIL_IO_CRABDB_API int

fossil_io_crabdb_type_is_signed(crabdb_type_t type);

/* ============================================================

 * Utility

 * ============================================================ */

FOSSIL_IO_CRABDB_API const char *

fossil_io_crabdb_version(void);

FOSSIL_IO_CRABDB_API int fossil_io_crabdb_init(void);

FOSSIL_IO_CRABDB_API int fossil_io_crabdb_shutdown(void);

#ifdef __cplusplus
}

#include <string>

namespace fossil::io
{



} // namespace fossil

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
