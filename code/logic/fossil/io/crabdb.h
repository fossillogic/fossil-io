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

typedef enum
{
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

typedef enum
{
    CRABDB_THREAD_SINGLE = 0,
    CRABDB_THREAD_SERIALIZED = 1,
    CRABDB_THREAD_MULTI = 2
} crabdb_thread_mode_t;

/* ============================================================
    * Data Types (Extended Numeric System)
    * ============================================================ */

typedef enum
{
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
    * Forward Declarations
    * ============================================================ */

typedef struct crabdb_instr crabdb_instr_t;
typedef struct crabdb_program crabdb_program_t;

/* ============================================================
    * Numeric Range Metadata
    * ============================================================ */

typedef struct crabdb_numeric_range_signed_t
{
    int64_t min;
    int64_t max;
} crabdb_numeric_range_signed_t;

typedef struct crabdb_numeric_range_unsigned_t
{
    uint64_t min;
    uint64_t max;
} crabdb_numeric_range_unsigned_t;

typedef struct crabdb_numeric_range
{
    uint8_t bits;      /* 8, 16, 32, 64 */
    uint8_t is_signed; /* 1 = signed, 0 = unsigned */

    union
    {
        crabdb_numeric_range_signed_t s;
        crabdb_numeric_range_unsigned_t u;
    } range;

} crabdb_numeric_range_t;

/* ============================================================
    * Value Container
    * ============================================================ */

typedef struct crabdb_value
{
    crabdb_type_t type;
    union
    {
        int8_t i8;
        int16_t i16;
        int32_t i32;
        int64_t i64;
        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;
        double f64;
        float f32;
        char *s;
        struct
        {
            const void *data;
            size_t size;
        } blob;
        int b;
    } as;
} crabdb_value_t;

/* ============================================================
    * Configuration
    * ============================================================ */

typedef struct crabdb_config
{
    crabdb_thread_mode_t thread_mode;
    int enable_wal;
    int enable_foreign_keys;
    size_t page_size;
    size_t cache_size;
} crabdb_config_t;

/* ============================================================
    * Core Lifecycle
    * ============================================================ */

/**
 * Open a CrabDB database at the specified path with the given configuration.
 * On success, returns CRABDB_OK and sets *out_db to a new handle.
 * On failure, returns an error code and *out_db is set to NULL.
 *
 * The caller is responsible for calling fossil_io_crabdb_close() on the returned handle.
 * @param path The filesystem path to the database file.
 * @param config Optional configuration parameters (can be NULL for defaults).
 * @param out_db Output parameter for the database handle.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_open(
    const char *path,
    const crabdb_config_t *config,
    crabdb_handle_t **out_db);

/**
 * Close a CrabDB database handle, releasing all associated resources.
 * After this call, the handle is no longer valid and should not be used.
 * @param db The database handle to close.
 * @return CRABDB_OK on success, or an error code if the handle is invalid.
 */
int fossil_io_crabdb_close(crabdb_handle_t *db);

/**
 * Retrieve the last error message associated with the database handle.
 * If the handle is NULL, returns a generic "no db" message.
 * @param db The database handle to query for errors.
 * @return A string describing the last error, or "no db" if the handle is NULL.
 */
const char *
fossil_io_crabdb_errmsg(crabdb_handle_t *db);

/* ============================================================
    * Execution API
    * ============================================================ */

/**
 * Execute a SQL query directly on the database without preparing a statement.
 * @param db The database handle.
 * @param program The program to execute.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int
fossil_io_crabdb_exec(
    crabdb_handle_t *db,
    const crabdb_program_t *program);

/**
 * Prepare a SQL statement for execution.
 * @param db The database handle.
 * @param query The SQL query string to prepare.
 * @param out_stmt Output parameter for the prepared statement.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int
fossil_io_crabdb_prepare(
    crabdb_handle_t *db,
    const char *query,
    crabdb_stmt_t **out_stmt);

/**
 * Step through the results of a prepared statement.
 * @param stmt The prepared statement to step through.
 * @return CRABDB_ROW if a new row is available, CRABDB_DONE if execution is complete, or an error code on failure.
 */
int
fossil_io_crabdb_step(
    crabdb_stmt_t *stmt);

/**
 * Finalize a prepared statement, releasing all associated resources.
 * After this call, the statement is no longer valid and should not be used.
 * @param stmt The prepared statement to finalize.
 * @return CRABDB_OK on success, or an error code if the statement is invalid.
 */
int
fossil_io_crabdb_finalize(
    crabdb_stmt_t *stmt);

/**
 * Reset a prepared statement to its initial state, ready for re-execution.
 * @param stmt The prepared statement to reset.
 * @return CRABDB_OK on success, or an error code if the statement is invalid.
 */
int
fossil_io_crabdb_reset(
    crabdb_stmt_t *stmt);

/* ============================================================
    * Binding Parameters (Typed)
    * ============================================================ */

/**
 * Bind an int8_t value to a parameter in the prepared statement.
 * @param stmt The prepared statement.
 * @param index The 1-based index of the parameter to bind.
 * @param value The int8_t value to bind.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_bind_i8(crabdb_stmt_t *stmt, int index, int8_t value);

/**
 * Bind an int16_t value to a parameter in the prepared statement.
 * @param stmt The prepared statement.
 * @param index The 1-based index of the parameter to bind.
 * @param value The int16_t value to bind.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_bind_i16(crabdb_stmt_t *stmt, int index, int16_t value);

/**
 * Bind an int32_t value to a parameter in the prepared statement.
 * @param stmt The prepared statement.
 * @param index The 1-based index of the parameter to bind.
 * @param value The int32_t value to bind.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_bind_i32(crabdb_stmt_t *stmt, int index, int32_t value);

/**
 * Bind an int64_t value to a parameter in the prepared statement.
 * @param stmt The prepared statement.
 * @param index The 1-based index of the parameter to bind.
 * @param value The int64_t value to bind.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_bind_i64(crabdb_stmt_t *stmt, int index, int64_t value);

/**
 * Bind a uint8_t value to a parameter in the prepared statement.
 * @param stmt The prepared statement.
 * @param index The 1-based index of the parameter to bind.
 * @param value The uint8_t value to bind.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_bind_u8(crabdb_stmt_t *stmt, int index, uint8_t value);

/**
 * Bind a uint16_t value to a parameter in the prepared statement.
 * @param stmt The prepared statement.
 * @param index The 1-based index of the parameter to bind.
 * @param value The uint16_t value to bind.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_bind_u16(crabdb_stmt_t *stmt, int index, uint16_t value);

/**
 * Bind a uint32_t value to a parameter in the prepared statement.
 * @param stmt The prepared statement.
 * @param index The 1-based index of the parameter to bind.
 * @param value The uint32_t value to bind.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_bind_u32(crabdb_stmt_t *stmt, int index, uint32_t value);

/**
 * Bind a uint64_t value to a parameter in the prepared statement.
 * @param stmt The prepared statement.
 * @param index The 1-based index of the parameter to bind.
 * @param value The uint64_t value to bind.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_bind_u64(crabdb_stmt_t *stmt, int index, uint64_t value);

/**
 * Bind a float value to a parameter in the prepared statement.
 * @param stmt The prepared statement.
 * @param index The 1-based index of the parameter to bind.
 * @param value The float value to bind.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_bind_f32(crabdb_stmt_t *stmt, int index, float);

/**
 * Bind a double value to a parameter in the prepared statement.
 * @param stmt The prepared statement.
 * @param index The 1-based index of the parameter to bind.
 * @param value The double value to bind.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_bind_f64(crabdb_stmt_t *stmt, int index, double);

/**
 * Bind a text value to a parameter in the prepared statement.
 * @param stmt The prepared statement.
 * @param index The 1-based index of the parameter to bind.
 * @param value The text value to bind.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_bind_text(
    crabdb_stmt_t *stmt, int index, const char *value);

/**
 * Bind a blob value to a parameter in the prepared statement.
 * @param stmt The prepared statement.
 * @param index The 1-based index of the parameter to bind.
 * @param data The blob data to bind.
 * @param size The size of the blob data.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_bind_blob(
    crabdb_stmt_t *stmt, int index, const void *data, size_t size);

/**
 * Bind a NULL value to a parameter in the prepared statement.
 * @param stmt The prepared statement.
 * @param index The 1-based index of the parameter to bind.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_bind_null(
    crabdb_stmt_t *stmt, int index);

/* ============================================================
    * Column Access (Typed)
    * ============================================================ */

/**
 * Get the number of columns in the current result row of the prepared statement.
 * @param stmt The prepared statement.
 * @return The number of columns, or an error code if the statement is invalid.
 */
int fossil_io_crabdb_column_count(crabdb_stmt_t *);

/**
 * Get the data type of a column in the current result row of the prepared statement.
 * @param stmt The prepared statement.
 * @param col The 0-based index of the column to query.
 * @return The data type of the column, or an error code if the statement or column index is invalid.
 */
crabdb_type_t fossil_io_crabdb_column_type(crabdb_stmt_t *, int col);

/**
 * Get the value of a column in the current result row of the prepared statement as an int8_t.
 * @param stmt The prepared statement.
 * @param col The 0-based index of the column to retrieve.
 * @return The int8_t value of the column, or an error code if the statement, column index, or type is invalid.
 */
int8_t fossil_io_crabdb_column_i8(crabdb_stmt_t *stmt, int col);

/**
 * Get the value of a column in the current result row of the prepared statement as an int16_t.
 * @param stmt The prepared statement.
 * @param col The 0-based index of the column to retrieve.
 * @return The int16_t value of the column, or an error code if the statement, column index, or type is invalid.
 */
int16_t fossil_io_crabdb_column_i16(crabdb_stmt_t *stmt, int col);

/**
 * Get the value of a column in the current result row of the prepared statement as an int32_t.
 * @param stmt The prepared statement.
 * @param col The 0-based index of the column to retrieve.
 * @return The int32_t value of the column, or an error code if the statement, column index, or type is invalid.
 */
int32_t fossil_io_crabdb_column_i32(crabdb_stmt_t *stmt, int col);

/**
 * Get the value of a column in the current result row of the prepared statement as an int64_t.
 * @param stmt The prepared statement.
 * @param col The 0-based index of the column to retrieve.
 * @return The int64_t value of the column, or an error code if the statement, column index, or type is invalid.
 */
int64_t fossil_io_crabdb_column_i64(crabdb_stmt_t *stmt, int col);

/**
 * Get the value of a column in the current result row of the prepared statement as a uint8_t.
 * @param stmt The prepared statement.
 * @param col The 0-based index of the column to retrieve.
 * @return The uint8_t value of the column, or an error code if the statement, column index, or type is invalid.
 */
uint8_t fossil_io_crabdb_column_u8(crabdb_stmt_t *stmt, int col);

/**
 * Get the value of a column in the current result row of the prepared statement as a uint16_t.
 * @param stmt The prepared statement.
 * @param col The 0-based index of the column to retrieve.
 * @return The uint16_t value of the column, or an error code if the statement, column index, or type is invalid.
 */
uint16_t fossil_io_crabdb_column_u16(crabdb_stmt_t *stmt, int col);

/**
 * Get the value of a column in the current result row of the prepared statement as a uint32_t.
 * @param stmt The prepared statement.
 * @param col The 0-based index of the column to retrieve.
 * @return The uint32_t value of the column, or an error code if the statement, column index, or type is invalid.
 */
uint32_t fossil_io_crabdb_column_u32(crabdb_stmt_t *stmt, int col);

/**
 * Get the value of a column in the current result row of the prepared statement as a uint64_t.
 * @param stmt The prepared statement.
 * @param col The 0-based index of the column to retrieve.
 * @return The uint64_t value of the column, or an error code if the statement, column index, or type is invalid.
 */
uint64_t fossil_io_crabdb_column_u64(crabdb_stmt_t *stmt, int col);

/**
 * Get the value of a column in the current result row of the prepared statement as a float.
 * @param stmt The prepared statement.
 * @param col The 0-based index of the column to retrieve.
 * @return The float value of the column, or an error code if the statement, column index, or type is invalid.
 */
float fossil_io_crabdb_column_f32(crabdb_stmt_t *stmt, int col);

/**
 * Get the value of a column in the current result row of the prepared statement as a double.
 * @param stmt The prepared statement.
 * @param col The 0-based index of the column to retrieve.
 * @return The double value of the column, or an error code if the statement, column index, or type is invalid.
 */
double fossil_io_crabdb_column_f64(crabdb_stmt_t *stmt, int col);

/**
 * Get the value of a column in the current result row of the prepared statement as a double.
 * @param stmt The prepared statement.
 * @param col The 0-based index ofthe column to retrieve.
 * @return The double value ofthe column, or an error code ifthe statement, column index, or type is invalid.
 */
const char *fossil_io_crabdb_column_text(crabdb_stmt_t *stmt, int col);

/**
 * Get the value of a column in the current result row of the prepared statement as a blob.
 * @param stmt The prepared statement.
 * @param col The 0-based index of the column to retrieve.
 * @param out_size Output parameter for the size of the blob data.
 * @return A pointer to the blob data, or NULL if the statement, column index, or type is invalid.
 */
const void *fossil_io_crabdb_column_blob(crabdb_stmt_t *stmt, int index, size_t *out_size);

/* ============================================================
    * Transactions
    * ============================================================ */

/**
 * Begin a new transaction on the database.
 * @param db The database handle.
 * @param out Output parameter for the new transaction handle.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_begin(crabdb_handle_t *, crabdb_txn_t **);

/**
 * Commit a transaction.
 * @param txn The transaction handle.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_commit(crabdb_txn_t *);

/**
 * Rollback a transaction.
 * @param txn The transaction handle.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_rollback(crabdb_txn_t *);

/* ============================================================
    * Family Tree Model
    * ============================================================ */

/**
 * Create a new family in the database with the specified name and schema.
 * @param db The database handle.
 * @param name The name of the family to create.
 * @param schema The schema definition for the family (e.g., column definitions).
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_create_family(
    crabdb_handle_t *db,
    const char *name,
    const char *schema);

/**
 * Create a parent-child relationship between two records in the family tree.
 * @param db The database handle.
 * @param parent_family The name of the parent family.
 * @param parent_id The ID of the parent record.
 * @param child_family The name of the child family.
 * @param child_id The ID of the child record.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_relate(
    crabdb_handle_t *db,
    const char *parent_family,
    int64_t parent_id,
    const char *child_family,
    int64_t child_id);

/**
 * Query the lineage of a record in the family tree, returning a prepared statement that can be stepped through.
 * @param db The database handle.
 * @param family The name of the family to query.
 * @param id The ID of the record to query lineage for.
 * @param out_stmt Output parameter for the prepared statement containing the lineage results.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_query_lineage(
    crabdb_handle_t *db,
    const char *family,
    int64_t id,
    crabdb_stmt_t **out_stmt);

/* ============================================================
    * Type Introspection
    * ============================================================ */

/**
 * Get the number of bits used to represent a given data type.
 * @param type The data type to query.
 * @return The number of bits for the type, or 0 if the type is invalid.
 */
int fossil_io_crabdb_type_bits(crabdb_type_t type);

/**
 * Check if a given data type is signed.
 * @param type The data type to query.
 * @return 1 if the type is signed, 0 if it is unsigned, or -1 if the type is invalid.
 */
int fossil_io_crabdb_type_is_signed(crabdb_type_t type);

/* ============================================================
    * Utility
    * ============================================================ */

/**
 * Get the version string of the CrabDB library.
 * @return A string representing the version of the library.
 */
const char *fossil_io_crabdb_version(void);

/**
 * Initialize the CrabDB library. This should be called before any other functions are used.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_init(void);

/**
 * Shutdown the CrabDB library, releasing any global resources. After this call, no other functions should be used.
 * @return CRABDB_OK on success, or an error code on failure.
 */
int fossil_io_crabdb_shutdown(void);

#ifdef __cplusplus
}

#include <string>
#include <vector>
#include <stdexcept>

namespace fossil::io
{

    class CrabDB
    {
    public:
        /* =========================
         * Nested Statement Wrapper
         * ========================= */
        class statement
        {
        public:
            statement(crabdb_stmt_t *stmt = nullptr)
                : stmt_(stmt) {}

            ~statement()
            {
                if (stmt_)
                {
                    fossil_io_crabdb_finalize(stmt_);
                }
            }

            statement(const statement &) = delete;
            statement &operator=(const statement &) = delete;

            statement(statement &&other) noexcept
            {
                stmt_ = other.stmt_;
                other.stmt_ = nullptr;
            }

            statement &operator=(statement &&other) noexcept
            {
                if (this != &other)
                {
                    if (stmt_)
                    {
                        fossil_io_crabdb_finalize(stmt_);
                    }
                    stmt_ = other.stmt_;
                    other.stmt_ = nullptr;
                }
                return *this;
            }

            /**
             * Step through the results of the prepared statement. Returns true if a new row is available, false if execution is complete.
             * Throws an exception if an error occurs.
             */
            int step()
            {
                return fossil_io_crabdb_step(stmt_);
            }

            /**
             * Reset the prepared statement to its initial state.
             * Throws an exception if an error occurs.
             */
            void reset()
            {
                fossil_io_crabdb_reset(stmt_);
            }

            /* ---- Bindings ---- */

            /**
             * Bind an int8_t value to the prepared statement at the given index.
             * @param idx The 1-based index of the parameter to bind.
             * @param v The int8_t value to bind.
             * Throws an exception if an error occurs.
             */
            void bind_i8(int idx, int8_t v)
            {
                fossil_io_crabdb_bind_i8(stmt_, idx, v);
            }

            /**
             * Bind an int16_t value to the prepared statement at the given index.
             * @param idx The 1-based index of the parameter to bind.
             * @param v The int16_t value to bind.
             * Throws an exception if an error occurs.
             */
            void bind_i16(int idx, int16_t v)
            {
                fossil_io_crabdb_bind_i16(stmt_, idx, v);
            }

            /**
             * Bind an int32_t value to the prepared statement at the given index.
             * @param idx The 1-based index of the parameter to bind.
             * @param v The int32_t value to bind.
             * Throws an exception if an error occurs.
             */
            void bind_i32(int idx, int32_t v)
            {
                fossil_io_crabdb_bind_i32(stmt_, idx, v);
            }

            /**
             * Bind a 32-bit integer to the prepared statement at the given index.
             * @param idx The 1-based index of the parameter to bind.
             * @param v The 32-bit integer value to bind.
             * Throws an exception if an error occurs.
             */
            void bind_i32(int idx, int32_t v)
            {
                fossil_io_crabdb_bind_i32(stmt_, idx, v);
            }

            /**
             * Bind a 64-bit integer to the prepared statement at the given index.
             * @param idx The 1-based index of the parameter to bind.
             * @param v The 64-bit integer value to bind.
             * Throws an exception if an error occurs.
             */
            void bind_u8(int idx, uint8_t v)
            {
                fossil_io_crabdb_bind_u8(stmt_, idx, v);
            }

            /**
             * Bind a 16-bit unsigned integer to the prepared statement at the given index.
             * @param idx The 1-based index of the parameter to bind.
             * @param v The 16-bit unsigned integer value to bind.
             * Throws an exception if an error occurs.
             */
            void bind_u16(int idx, uint16_t v)
            {
                fossil_io_crabdb_bind_u16(stmt_, idx, v);
            }

            /**
             * Bind a 32-bit unsigned integer to the prepared statement at the given index.
             * @param idx The 1-based index of the parameter to bind.
             * @param v The 32-bit unsigned integer value to bind.
             * Throws an exception if an error occurs.
             */
            void bind_u32(int idx, uint32_t v)
            {
                fossil_io_crabdb_bind_u32(stmt_, idx, v);
            }

            /**
             * Bind a 64-bit unsigned integer to the prepared statement at the given index.
             * @param idx The 1-based index of the parameter to bind.
             * @param v The 64-bit unsigned integer value to bind.
             * Throws an exception if an error occurs.
             */
            void bind_u64(int idx, uint64_t v)
            {
                fossil_io_crabdb_bind_u64(stmt_, idx, v);
            }

            /**
             * Bind a double value to the prepared statement at the given index.
             * @param idx The 1-based index of the parameter to bind.
             * @param v The double value to bind.
             * Throws an exception if an error occurs.
             */
            void bind_f64(int idx, double v)
            {
                fossil_io_crabdb_bind_f64(stmt_, idx, v);
            }

            /**
             * Bind a text value to the prepared statement at the given index.
             * @param idx The 1-based index of the parameter to bind.
             * @param v The text value to bind.
             * Throws an exception if an error occurs.
             */
            void bind_text(int idx, const std::string &v)
            {
                fossil_io_crabdb_bind_text(stmt_, idx, v.c_str());
            }

            /**
             * Bind a NULL value to the prepared statement at the given index.
             * @param idx The 1-based index of the parameter to bind.
             * Throws an exception if an error occurs.
             */
            void bind_null(int idx)
            {
                fossil_io_crabdb_bind_null(stmt_, idx);
            }

            /* ---- Column access ---- */

            /**
             * Get the number of columns in the current result row of the prepared statement.
             * @return The number of columns, or an error code if the statement is invalid.
             */
            int column_count() const
            {
                return fossil_io_crabdb_column_count(stmt_);
            }

            /**
             * Get the data type of a column in the current result row of the prepared statement.
             * @param col The 0-based index of the column to query.
             * @return The data type of the column, or an error code if the statement or column index is invalid.
             */
            crabdb_type_t column_type(int col) const
            {
                return fossil_io_crabdb_column_type(stmt_, col);
            }

            /**
             * Get the value of a column in the current result row of the prepared statement as an int8_t.
             * @param col The 0-based index of the column to retrieve.
             * @return The int8_t value of the column, or an error code if the statement, column index, or type is invalid.
             */
            int8_t column_i8(int col) const
            {
                return fossil_io_crabdb_column_i8(stmt_, col);
            }

            /**
             * Get the value of a column in the current result row of the prepared statement as an int16_t.
             * @param col The 0-based index of the column to retrieve.
             * @return The int16_t value of the column, or an error code if the statement, column index, or type is invalid.
             */
            int16_t column_i16(int col) const
            {
                return fossil_io_crabdb_column_i16(stmt_, col);
            }

            /**
            * Get the value of a column in the current result row of the prepared statement as an int32_t.
            * @param col The 0-based index of the column to retrieve.
            * @return The int32_t value of the column, or an error code if the statement, column index, or type is invalid.
            */
            int32_t column_i32(int col) const
            {
                return fossil_io_crabdb_column_i32(stmt_, col);
            }

            /**
            * Get the value of a column in the current result row of the prepared statement as an int64_t.
            * @param col The 0-based index of the column to retrieve.
            * @return The int64_t value of the column, or an error code if the statement, column index, or type is invalid.
            */
            int64_t column_i64(int col) const
            {
                return fossil_io_crabdb_column_i64(stmt_, col);
            }

            /**
             * Get the value of a column in the current result row of the prepared statement as a uint8_t.
             * @param col The 0-based index of the column to retrieve.
             * @return The uint8_t value of the column, or an error code if the statement, column index, or type is invalid.
             */
            uint8_t column_u8(int col) const
            {
                return fossil_io_crabdb_column_u8(stmt_, col);
            }

            /**
             * Get the value of a column in the current result row of the prepared statement as a uint16_t.
             * @param col The 0-based index of the column to retrieve.
             * @return The uint16_t value of the column, or an error code if the statement, column index, or type is invalid.
             */
            uint16_t column_u16(int col) const
            {
                return fossil_io_crabdb_column_u16(stmt_, col);
            }

            /**
             * Get the value of a column in the current result row of the prepared statement as a uint32_t.
             * @param col The 0-based index of the column to retrieve.
             * @return The uint32_t value of the column, or an error code if the statement, column index, or type is invalid.
             */
            uint32_t column_u32(int col) const
            {
                return fossil_io_crabdb_column_u32(stmt_, col);
            }

            /**
             * Get the value of a column in the current result row of the prepared statement as a uint64_t.
             * @param col The 0-based index of the column to retrieve.
             * @return The uint64_t value of the column, or an error code if the statement, column index, or type is invalid.
             */
            uint64_t column_u64(int col) const
            {
                return fossil_io_crabdb_column_u64(stmt_, col);
            }

            /**
            * Get the value of a column in the current result row of the prepared statement as a float.
            * @param col The 0-based index of the column to retrieve.
            * @return The float value of the column, or an error code if the statement, column index, or type is invalid.
            */
            float column_f32(int col) const
            {
                return fossil_io_crabdb_column_f32(stmt_, col);
            }

            /**
            * Get the value of a column in the current result row of the prepared statement as a double.
            * @param col The 0-based index of the column to retrieve.
            * @return The double value of the column, or an error code if the statement, column index, or type is invalid.
            */
            double column_f64(int col) const
            {
                return fossil_io_crabdb_column_f64(stmt_, col);
            }

            /**
            * Get the value of a column in the current result row of the prepared statement as a text string.
            * @param col The 0-based index of the column to retrieve.
            * @return The text value of the column, or an empty string if the statement, column index, or type is invalid.
            */
            std::string column_text(int col) const
            {
                const char *s = fossil_io_crabdb_column_text(stmt_, col);
                return s ? s : "";
            }

            /**
            * Get the value of a column in the current result row of the prepared statement as a blob.
            * @param col The 0-based index of the column to retrieve.
            * @param out_size Output parameter for the size of the blob data.
            * @return A pointer to the blob data, or nullptr if the statement, column index, or type is invalid.
            */
            const void *column_blob(int col, size_t *out_size) const
            {
                return fossil_io_crabdb_column_blob(stmt_, col, out_size);
            }

            crabdb_stmt_t *raw() { return stmt_; }

        private:
            crabdb_stmt_t *stmt_;
        };

    public:
        /* =========================
         * Lifecycle
         * ========================= */

        CrabDB(const std::string &path,
               const crabdb_config_t *config = nullptr)
            : db_(nullptr)
        {
            int rc = fossil_io_crabdb_open(path.c_str(), config, &db_);
            if (rc != CRABDB_OK)
            {
                throw std::runtime_error("CrabDB open failed");
            }
        }

        ~CrabDB()
        {
            if (db_)
            {
                fossil_io_crabdb_close(db_);
            }
        }

        CrabDB(const CrabDB &) = delete;
        CrabDB &operator=(const CrabDB &) = delete;

        CrabDB(CrabDB &&other) noexcept
        {
            db_ = other.db_;
            other.db_ = nullptr;
        }

        CrabDB &operator=(CrabDB &&other) noexcept
        {
            if (this != &other)
            {
                if (db_)
                {
                    fossil_io_crabdb_close(db_);
                }
                db_ = other.db_;
                other.db_ = nullptr;
            }
            return *this;
        }

        /* =========================
         * Core API
         * ========================= */

        /**
         * Execute a SQL query or precompiled CrabDB bytecode program.
         * In the future, DSL will compile into bytecode before reaching this layer.
         *
         * @param query SQL string OR serialized bytecode program.
         * @return CRABDB_OK on success, or an error code on failure.
         * Throws an exception if an error occurs.
         */
        int exec(const std::string &query)
        {
            int rc = fossil_io_crabdb_exec(db_, query.c_str());

            if (rc != CRABDB_OK)
            {
                throw std::runtime_error(error_message());
            }

            return rc;
        }

        /**
         * Prepare a SQL statement for execution.
         * In the future, this will compile SQL/DSL into CrabDB bytecode.
         *
         * @param query SQL or DSL query string.
         * @return A statement object representing a compiled execution plan.
         * Throws an exception if an error occurs.
         */
        statement prepare(const std::string &query)
        {
            crabdb_stmt_t *stmt = nullptr;

            int rc = fossil_io_crabdb_prepare(db_, query.c_str(), &stmt);

            if (rc != CRABDB_OK)
            {
                throw std::runtime_error(error_message());
            }

            return statement(stmt);
        }

        /**
         * Get a string describing the last error that occurred on the database handle.
         * @return A string describing the last error, or "no db" if the handle is NULL.
         */
        std::string error_message() const
        {
            return fossil_io_crabdb_errmsg(db_);
        }

        /* =========================
         * Transactions
         * ========================= */

        class transaction
        {
        public:
            transaction(crabdb_txn_t *txn) : txn_(txn) {}

            ~transaction()
            {
                if (txn_)
                {
                    fossil_io_crabdb_rollback(txn_);
                }
            }

            void commit()
            {
                if (txn_)
                {
                    fossil_io_crabdb_commit(txn_);
                    txn_ = nullptr;
                }
            }

        private:
            crabdb_txn_t *txn_;
        };

        transaction begin()
        {
            crabdb_txn_t *txn = nullptr;

            int rc = fossil_io_crabdb_begin(db_, &txn);
            if (rc != CRABDB_OK)
            {
                throw std::runtime_error(error_message());
            }

            return transaction(txn);
        }

        /* =========================
         * Family Tree API
         * ========================= */

        int create_family(const std::string &name,
                          const std::string &schema)
        {
            return fossil_io_crabdb_create_family(
                db_, name.c_str(), schema.c_str());
        }

        int relate(const std::string &parent_family,
                   int64_t parent_id,
                   const std::string &child_family,
                   int64_t child_id)
        {
            return fossil_io_crabdb_relate(
                db_,
                parent_family.c_str(),
                parent_id,
                child_family.c_str(),
                child_id);
        }

        statement query_lineage(const std::string &family,
                                int64_t id)
        {
            crabdb_stmt_t *stmt = nullptr;

            int rc = fossil_io_crabdb_query_lineage(
                db_, family.c_str(), id, &stmt);

            if (rc != CRABDB_OK)
            {
                throw std::runtime_error(error_message());
            }

            return statement(stmt);
        }

        /* =========================
         * Static Utilities
         * ========================= */

        static std::string version()
        {
            return fossil_io_crabdb_version();
        }

        static void init()
        {
            fossil_io_crabdb_init();
        }

        static void shutdown()
        {
            fossil_io_crabdb_shutdown();
        }

    private:
        crabdb_handle_t *db_;
    };

} // namespace fossil::io

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
