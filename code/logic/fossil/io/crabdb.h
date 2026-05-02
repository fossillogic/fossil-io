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

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* =========================================================
 * Versioning
 * ========================================================= */

#define FOSSIL_IO_CRABDB_VERSION_MAJOR 0
#define FOSSIL_IO_CRABDB_VERSION_MINOR 1
#define FOSSIL_IO_CRABDB_VERSION_PATCH 0

/* =========================================================
 * Error codes
 * ========================================================= */
typedef enum {
    FOSSIL_IO_CRABDB_OK = 0,
    FOSSIL_IO_CRABDB_ERR_GENERIC,
    FOSSIL_IO_CRABDB_ERR_IO,
    FOSSIL_IO_CRABDB_ERR_MEMORY,
    FOSSIL_IO_CRABDB_ERR_SCHEMA,
    FOSSIL_IO_CRABDB_ERR_TYPE_MISMATCH,
    FOSSIL_IO_CRABDB_ERR_NOT_FOUND,
    FOSSIL_IO_CRABDB_ERR_CONSTRAINT,
    FOSSIL_IO_CRABDB_ERR_TRANSACTION,
    FOSSIL_IO_CRABDB_ERR_QUERY,
    FOSSIL_IO_CRABDB_ERR_INVALID_ARG
} fossil_io_crabdb_error_t;

/* =========================================================
 * Forward declarations
 * ========================================================= */

typedef struct fossil_io_crabdb_handle crabdb_handle_t;
typedef struct fossil_io_crabdb_table  crabdb_table_t;
typedef struct fossil_io_crabdb_row    crabdb_row_t;
typedef struct fossil_io_crabdb_value  crabdb_value_t;
typedef struct fossil_io_crabdb_txn    crabdb_txn_t;

/* =========================================================
 * Supported types (logical schema layer)
 * ========================================================= */
typedef enum {
    FOSSIL_IO_CRABDB_TYPE_I8,
    FOSSIL_IO_CRABDB_TYPE_I16,
    FOSSIL_IO_CRABDB_TYPE_I32,
    FOSSIL_IO_CRABDB_TYPE_I64,

    FOSSIL_IO_CRABDB_TYPE_U8,
    FOSSIL_IO_CRABDB_TYPE_U16,
    FOSSIL_IO_CRABDB_TYPE_U32,
    FOSSIL_IO_CRABDB_TYPE_U64,

    FOSSIL_IO_CRABDB_TYPE_HEX,
    FOSSIL_IO_CRABDB_TYPE_OCT,
    FOSSIL_IO_CRABDB_TYPE_BIN,

    FOSSIL_IO_CRABDB_TYPE_F32,
    FOSSIL_IO_CRABDB_TYPE_F64,

    FOSSIL_IO_CRABDB_TYPE_CSTR,
    FOSSIL_IO_CRABDB_TYPE_CHAR,

    FOSSIL_IO_CRABDB_TYPE_BOOL,

    FOSSIL_IO_CRABDB_TYPE_SIZE,

    FOSSIL_IO_CRABDB_TYPE_DATETIME,
    FOSSIL_IO_CRABDB_TYPE_DURATION,

    FOSSIL_IO_CRABDB_TYPE_ANY,
    FOSSIL_IO_CRABDB_TYPE_NULL
} fossil_io_crabdb_type_t;

/* =========================================================
 * Value container
 * ========================================================= */

struct fossil_io_crabdb_value {
    const char* column;
    fossil_io_crabdb_type_t type;

    union {
        int8_t   i8;
        int16_t  i16;
        int32_t  i32;
        int64_t  i64;

        uint8_t  u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;

        float    f32;
        double   f64;

        const char* cstr;
        char        ch;
        uint8_t     boolean;

        size_t      size;
        int64_t     datetime;
        int64_t     duration;

        void* any;
    } data;

    size_t size; /* used for any/blob-like data */
};

/* =========================================================
 * Row container (opaque in implementation)
 * ========================================================= */

struct fossil_io_crabdb_row {
    size_t column_count;
    crabdb_value_t* values;
};

/* =========================================================
 * Database lifecycle
 * ========================================================= */

/**
 * Open database at given path. If the file does not exist, it will be created.
 * Returns NULL on failure (e.g. I/O error, permission issues).
 * 
 * @param path Filesystem path to the database file.
 * @return Pointer to crabdb_handle_t on success, NULL on failure.
 */
crabdb_handle_t* fossil_io_crabdb_open(const char* path);

/**
 * Close the database and free all associated resources. After this call, the db pointer is invalid.
 * 
 * @param db Pointer to the database handle to close.
 */
void fossil_io_crabdb_close(crabdb_handle_t* db);

/* =========================================================
 * Schema / Table management
 * ========================================================= */

/**
 * Create a new table with the specified name. Returns an error code indicating success or failure.
 * 
 * @param db Pointer to the database handle.
 * @param table_name Name of the table to create.
 * @return FOSSIL_IO_CRABDB_OK on success, or an appropriate error code on failure.
 */
fossil_io_crabdb_error_t fossil_io_crabdb_create_table(
    crabdb_handle_t* db,
    const char* table_name
);

/**
 * Define a new column in the specified table with the given properties. Returns an error code indicating success or failure.
 * 
 * @param db Pointer to the database handle.
 * @param table_name Name of the table to modify.
 * @param column_name Name of the column to define.
 * @param type Data type of the column (from fossil_io_crabdb_type_t).
 * @param is_primary_key Non-zero if this column is a primary key, zero otherwise.
 * @param is_nullable Non-zero if this column can contain NULL values, zero otherwise.
 * @return FOSSIL_IO_CRABDB_OK on success, or an appropriate error code on failure.
 */
fossil_io_crabdb_error_t fossil_io_crabdb_define_column(
    crabdb_handle_t* db,
    const char* table_name,
    const char* column_name,
    fossil_io_crabdb_type_t type,
    int is_primary_key,
    int is_nullable
);

/* =========================================================
 * Family Tree relations (core innovation)
 * ========================================================= */

/**
 * Set the parent table for the specified child table.
 * 
 * @param db Pointer to the database handle.
 * @param child_table Name of the child table.
 * @param parent_table Name of the parent table.
 * @return FOSSIL_IO_CRABDB_OK on success, or an appropriate error code on failure.
 */
fossil_io_crabdb_error_t fossil_io_crabdb_set_parent(
    crabdb_handle_t* db,
    const char* child_table,
    const char* parent_table
);

/**
 * Set the child table for the specified parent table.
 * 
 * @param db Pointer to the database handle.
 * @param parent_table Name of the parent table.
 * @param child_table Name of the child table.
 * @return FOSSIL_IO_CRABDB_OK on success, or an appropriate error code on failure.
 */
fossil_io_crabdb_error_t fossil_io_crabdb_set_child(
    crabdb_handle_t* db,
    const char* parent_table,
    const char* child_table
);

/* =========================================================
 * CRUD operations
 * ========================================================= */

/**
 * Insert a new row into the specified table with the given values. Returns an error code indicating success or failure.
 * 
 * @param db Pointer to the database handle.
 * @param table Name of the table to insert into.
 * @param values Array of crabdb_value_t representing column-value pairs for the new row.
 * @param value_count Number of elements in the values array.
 * @return FOSSIL_IO_CRABDB_OK on success, or an appropriate error code on failure.
 */
fossil_io_crabdb_error_t fossil_io_crabdb_insert(
    crabdb_handle_t* db,
    const char* table,
    crabdb_value_t* values,
    size_t value_count
);

/**
 * Update existing rows in the specified table that match the where_clause with the given values. Returns an error code indicating success or failure.
 * 
 * @param db Pointer to the database handle.
 * @param table Name of the table to update.
 * @param values Array of crabdb_value_t representing column-value pairs for the update.
 * @param where_clause SQL-like WHERE clause to specify which rows to update (e.g. "id = 5").
 * @return FOSSIL_IO_CRABDB_OK on success, or an appropriate error code on failure.
 */
fossil_io_crabdb_error_t fossil_io_crabdb_update(
    crabdb_handle_t* db,
    const char* table,
    crabdb_value_t* values,
    const char* where_clause
);

/**
 * Delete rows from the specified table that match the where_clause. Returns an error code indicating success or failure.
 * 
 * @param db Pointer to the database handle.
 * @param table Name of the table to delete from.
 * @param where_clause SQL-like WHERE clause to specify which rows to delete (e.g. "id = 5").
 * @return FOSSIL_IO_CRABDB_OK on success, or an appropriate error code on failure.
 */
fossil_io_crabdb_error_t fossil_io_crabdb_delete(
    crabdb_handle_t* db,
    const char* table,
    const char* where_clause
);

/* =========================================================
 * Query engine
 * ========================================================= */

/**
 * Execute a query on the database and return the resulting rows.
 * 
 * @param db Pointer to the database handle.
 * @param query SQL-like query string to execute.
 * @return Pointer to an array of crabdb_row_t representing the query results.
 */
crabdb_row_t* fossil_io_crabdb_query(
    crabdb_handle_t* db,
    const char* query
);

/* =========================================================
 * Transactions
 * ========================================================= */

/**
 * Begin a new transaction. Returns a pointer to a crabdb_txn_t representing the transaction context, or NULL on failure.
 * 
 * @param db Pointer to the database handle.
 * @return Pointer to crabdb_txn_t on success, NULL on failure.
 */
crabdb_txn_t* fossil_io_crabdb_begin(crabdb_handle_t* db);

/**
 * Commit the specified transaction. Returns an error code indicating success or failure.
 * 
 * @param txn Pointer to the transaction context to commit.
 * @return FOSSIL_IO_CRABDB_OK on success, or an appropriate error code on failure.
 */
fossil_io_crabdb_error_t fossil_io_crabdb_commit(crabdb_txn_t* txn);

/**
 * Rollback the specified transaction. Returns an error code indicating success or failure.
 * 
 * @param txn Pointer to the transaction context to rollback.
 * @return FOSSIL_IO_CRABDB_OK on success, or an appropriate error code on failure.
 */
fossil_io_crabdb_error_t fossil_io_crabdb_rollback(crabdb_txn_t* txn);

/* =========================================================
 * Utility helpers
 * ========================================================= */

/**
 * Get a human-readable error message for the specified error code.
 * 
 * @param err The error code to get the message for.
 * @return A string describing the error.
 */
const char* fossil_io_crabdb_error_string(fossil_io_crabdb_error_t err);

/**
 * Get the version string of the CrabDB library.
 * 
 * @return A string representing the version of the library (e.g. "0.1.0").
 */
const char* fossil_io_crabdb_version(void);

#ifdef __cplusplus
}

#include <string>
#include <vector>
#include <stdexcept>

namespace fossil::io
{

    /* =========================================================
     * Value Wrapper (C++ friendly)
     * ========================================================= */

    class Value
    {
    public:
        crabdb_value_t raw;

        Value()
        {
            raw.type = CRAB_TYPE_NULL;
            raw.as.any = nullptr;
        }

        static Value from_int64(int64_t v)
        {
            Value val;
            val.raw.type = CRAB_TYPE_I64;
            val.raw.as.i64 = v;
            return val;
        }

        static Value from_string(const std::string &s)
        {
            Value val;
            val.raw.type = CRAB_TYPE_CSTR;
            val.raw.as.cstr = s.c_str(); // NOTE: lifetime must be managed by caller
            return val;
        }
    };

    /* =========================================================
     * Result Wrapper
     * ========================================================= */

    class Result
    {
    public:
        crabdb_result_t raw;

        Result()
        {
            raw.rows = nullptr;
            raw.count = 0;
        }

        size_t size() const { return raw.count; }
    };

    /* =========================================================
     * Core Database Class
     * ========================================================= */

    class CrabDB
    {
    public:
        CrabDB() : db_(nullptr) {}

        explicit CrabDB(const std::string &path)
        {
            open(path);
        }

        ~CrabDB()
        {
            close();
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
                close();
                db_ = other.db_;
                other.db_ = nullptr;
            }
            return *this;
        }

        /* =========================================================
         * Lifecycle
         * ========================================================= */

        bool open(const std::string &path)
        {
            db_ = fossil_io_crabdb_open(path.c_str());
            return db_ != nullptr;
        }

        void close()
        {
            if (db_)
            {
                fossil_io_crabdb_close(db_);
                db_ = nullptr;
            }
        }

        bool flush()
        {
            return fossil_io_crabdb_flush(db_) == 0;
        }

        /* =========================================================
         * Query Engine
         * ========================================================= */

        bool exec(const std::string &query)
        {
            return fossil_io_crabdb_exec(db_, query.c_str()) == 0;
        }

        bool query(const std::string &query, Result &out)
        {
            return fossil_io_crabdb_query(db_, query.c_str(), &out.raw) == 0;
        }

        /* =========================================================
         * Typed Operations
         * ========================================================= */

        bool insert(const std::string &table,
                    const std::vector<Value> &values)
        {

            std::vector<crabdb_value_t> raw;
            raw.reserve(values.size());

            for (const auto &v : values)
            {
                raw.push_back(v.raw);
            }

            return fossil_io_crabdb_insert(
                       db_,
                       table.c_str(),
                       raw.data(),
                       raw.size()) == 0;
        }

        bool update(const std::string &query)
        {
            return fossil_io_crabdb_update(db_, query.c_str()) == 0;
        }

        bool remove(const std::string &query)
        {
            return fossil_io_crabdb_delete(db_, query.c_str()) == 0;
        }

        /* =========================================================
         * Schema / Family Tree
         * ========================================================= */

        bool create_table(const std::string &name)
        {
            return fossil_io_crabdb_create_table(db_, name.c_str()) == 0;
        }

        bool link_family(const std::string &parent,
                         const std::string &child)
        {

            return fossil_io_crabdb_link_family(
                       db_,
                       parent.c_str(),
                       child.c_str()) == 0;
        }

        /* =========================================================
         * Threading
         * ========================================================= */

        bool enable_threading(bool enable)
        {
            return fossil_io_crabdb_enable_threading(db_, enable ? 1 : 0) == 0;
        }

        bool set_max_threads(std::size_t threads)
        {
            return fossil_io_crabdb_set_max_threads(db_, threads) == 0;
        }

        /* =========================================================
         * Errors
         * ========================================================= */

        std::string last_error() const
        {
            const char *err = fossil_io_crabdb_last_error(db_);
            return err ? std::string(err) : std::string();
        }

    private:
        crabdb_handle_t *db_;
    };

} // namespace fossil::io

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
