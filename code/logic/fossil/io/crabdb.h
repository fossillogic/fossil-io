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
 * Core Handle
 * ========================================================= */

typedef struct crabdb_handle crabdb_handle_t;

/* =========================================================
 * Type System
 * ========================================================= */

typedef enum
{
    CRAB_TYPE_I8,
    CRAB_TYPE_I16,
    CRAB_TYPE_I32,
    CRAB_TYPE_I64,

    CRAB_TYPE_U8,
    CRAB_TYPE_U16,
    CRAB_TYPE_U32,
    CRAB_TYPE_U64,

    CRAB_TYPE_F32,
    CRAB_TYPE_F64,

    CRAB_TYPE_BOOL,
    CRAB_TYPE_CHAR,
    CRAB_TYPE_CSTR,

    CRAB_TYPE_HEX,
    CRAB_TYPE_OCT,
    CRAB_TYPE_BIN,

    CRAB_TYPE_SIZE,
    CRAB_TYPE_DATETIME,
    CRAB_TYPE_DURATION,

    CRAB_TYPE_ANY,
    CRAB_TYPE_NULL

} crabdb_type_t;

/* =========================================================
 * Value Container
 * ========================================================= */

typedef struct
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

        float f32;
        double f64;

        uint8_t b;
        char c;

        const char *cstr;

        uint64_t size;
        uint64_t datetime;
        uint64_t duration;

        void *any;

    } as;

} crabdb_value_t;

/* =========================================================
 * Row / Result Set
 * ========================================================= */

typedef struct
{
    crabdb_value_t *values;
    size_t count;
} crabdb_row_t;

typedef struct
{
    crabdb_row_t *rows;
    size_t count;
} crabdb_result_t;

/* =========================================================
 * Database Lifecycle
 * ========================================================= */

/**
 * Opens a connection to the CrabDB database.
 *
 * @param path The path to the database file.
 * @return A pointer to the database handle, or NULL on failure.
 */
crabdb_handle_t *fossil_io_crabdb_open(const char *path);

/**
 * Closes the connection to the CrabDB database.
 *
 * @param db A pointer to the database handle.
 */
void fossil_io_crabdb_close(crabdb_handle_t *db);

/**
 * Flushes any pending changes to the CrabDB database.
 *
 * @param db A pointer to the database handle.
 * @return 0 on success, or a non-zero error code on failure.
 */
int fossil_io_crabdb_flush(crabdb_handle_t *db);

/* =========================================================
 * Query Engine (D-inspired)
 * ========================================================= */

/**
 * Executes a query against the CrabDB database.
 *
 * @param db A pointer to the database handle.
 * @param query The query string to execute.
 * @param out_result A pointer to a result structure to receive the query results.
 * @return 0 on success, or a non-zero error code on failure.
 */
int fossil_io_crabdb_exec(
    crabdb_handle_t *db,
    const char *query);

/**
 * Executes a query against the CrabDB database and retrieves results.
 *
 * @param db A pointer to the database handle.
 * @param query The query string to execute.
 * @param out_result A pointer to a result structure to receive the query results.
 * @return 0 on success, or a non-zero error code on failure.
 */
int fossil_io_crabdb_query(
    crabdb_handle_t *db,
    const char *query,
    crabdb_result_t *out_result);

/* =========================================================
 * Typed Operations (Safer API Layer)
 * ========================================================= */

/**
 * Inserts a new record into the specified table.
 *
 * @param db A pointer to the database handle.
 * @param table The name of the table to insert into.
 * @param values An array of values to insert, corresponding to the table schema.
 * @param count The number of values in the array.
 * @return 0 on success, or a non-zero error code on failure.
 */
int fossil_io_crabdb_insert(
    crabdb_handle_t *db,
    const char *table,
    const crabdb_value_t *values,
    size_t count);

/**
 * Updates records in the specified table based on a condition.
 *
 * @param db A pointer to the database handle.
 * @param table The name of the table to update.
 * @param values An array of values to update, corresponding to the table schema.
 * @param count The number of values in the array.
 * @param condition A query string specifying the condition for which records to update.
 * @return 0 on success, or a non-zero error code on failure.
 */
int fossil_io_crabdb_update(
    crabdb_handle_t *db,
    const char *query);

/**
 * Deletes records from the specified table based on a condition.
 *
 * @param db A pointer to the database handle.
 * @param table The name of the table to delete from.
 * @param condition A query string specifying the condition for which records to delete.
 * @return 0 on success, or a non-zero error code on failure.
 */
int fossil_io_crabdb_delete(
    crabdb_handle_t *db,
    const char *query);

/* =========================================================
 * Schema / Family Tree Model
 * ========================================================= */

/**
 * Creates a new family (table) in the CrabDB database.
 * @param db A pointer to the database handle.
 * @param name The name of the family (table) to create.
 * @param schema The schema definition for the family (table).
 * @return 0 on success, or a non-zero error code on failure.
 */
int fossil_io_crabdb_create_table(
    crabdb_handle_t *db,
    const char *name);

/**
 * Establishes a parent-child relationship between two families (tables).
 *
 * @param db A pointer to the database handle.
 * @param parent_table The name of the parent family (table).
 * @param child_table The name of the child family (table).
 * @return 0 on success, or a non-zero error code on failure.
 */
int fossil_io_crabdb_link_family(
    crabdb_handle_t *db,
    const char *parent_table,
    const char *child_table);

/* =========================================================
 * Threading Control
 * ========================================================= */

/**
 * Enables or disables threading support in the CrabDB database.
 *
 * @param db A pointer to the database handle.
 * @param enable Non-zero to enable threading, zero to disable.
 * @return 0 on success, or a non-zero error code on failure.
 */
int fossil_io_crabdb_enable_threading(crabdb_handle_t *db, int enable);

/**
 * Sets the maximum number of threads that CrabDB can utilize for concurrent operations.
 *
 * @param db A pointer to the database handle.
 * @param threads The maximum number of threads to allow (must be greater than 0).
 * @return 0 on success, or a non-zero error code on failure.
 */
int fossil_io_crabdb_set_max_threads(crabdb_handle_t *db, size_t threads);

/* =========================================================
 * Utilities
 * ========================================================= */

/**
 * Retrieves the last error message from the CrabDB database.
 *
 * @param db A pointer to the database handle.
 * @return A string containing the last error message, or NULL if no error has occurred.
 */
const char *fossil_io_crabdb_last_error(crabdb_handle_t *db);

/**
 * Frees the memory associated with a CrabDB result set.
 *
 * @param result A pointer to the result set to free.
 */
void fossil_io_crabdb_free_result(crabdb_result_t *result);

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
