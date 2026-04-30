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

/* =========================================================
 * Internal Structures
 * ========================================================= */

typedef struct crabdb_kv {
    char* key;
    crabdb_value_t value;
} crabdb_kv_t;

typedef struct crabdb_row_internal {
    crabdb_kv_t* cols;
    size_t count;
} crabdb_row_internal_t;

typedef struct crabdb_table {
    char* name;
    crabdb_row_internal_t* rows;
    size_t row_count;
} crabdb_table_t;

struct crabdb_handle {
    crabdb_table_t* tables;
    size_t table_count;

    char* last_error;

    int threading_enabled;
    size_t max_threads;
};

/* =========================================================
 * Utility Helpers
 * ========================================================= */

static void set_error(crabdb_handle_t* db, const char* msg) {
    if (!db) return;
    free(db->last_error);
    db->last_error = strdup(msg);
}

static crabdb_table_t* find_table(crabdb_handle_t* db, const char* name) {
    for (size_t i = 0; i < db->table_count; i++) {
        if (strcmp(db->tables[i].name, name) == 0) {
            return &db->tables[i];
        }
    }
    return NULL;
}

/* =========================================================
 * Database Lifecycle
 * ========================================================= */

crabdb_handle_t* fossil_io_crabdb_open(const char* path) {
    (void)path; /* no persistence in this version */

    crabdb_handle_t* db = (crabdb_handle_t*)calloc(1, sizeof(crabdb_handle_t));
    if (!db) return NULL;

    db->tables = NULL;
    db->table_count = 0;
    db->last_error = NULL;
    db->threading_enabled = 0;
    db->max_threads = 1;

    return db;
}

void fossil_io_crabdb_close(crabdb_handle_t* db) {
    if (!db) return;

    for (size_t t = 0; t < db->table_count; t++) {
        crabdb_table_t* table = &db->tables[t];

        free(table->name);

        for (size_t r = 0; r < table->row_count; r++) {
            free(table->rows[r].cols);
        }
        free(table->rows);
    }

    free(db->tables);
    free(db->last_error);
    free(db);
}

int fossil_io_crabdb_flush(crabdb_handle_t* db) {
    if (!db) return 1;
    return 0; /* no-op for in-memory version */
}

/* =========================================================
 * Query Engine (stub parser)
 * ========================================================= */

int fossil_io_crabdb_exec(crabdb_handle_t* db, const char* query) {
    if (!db || !query) return 1;

    /* minimal stub behavior */
    if (strstr(query, "create") != NULL) {
        return 0;
    }

    if (strstr(query, "insert") != NULL) {
        return 0;
    }

    return 0;
}

int fossil_io_crabdb_query(
    crabdb_handle_t* db,
    const char* query,
    crabdb_result_t* out_result
) {
    (void)db;
    (void)query;
    (void)out_result;

    /* future: D-inspired parser → AST → executor */
    return 0;
}

/* =========================================================
 * Typed Operations
 * ========================================================= */

int fossil_io_crabdb_insert(
    crabdb_handle_t* db,
    const char* table,
    const crabdb_value_t* values,
    size_t count
) {
    if (!db || !table || !values) return 1;

    crabdb_table_t* t = find_table(db, table);
    if (!t) {
        set_error(db, "table not found");
        return 1;
    }

    crabdb_row_internal_t* new_rows =
        (crabdb_row_internal_t*)realloc(t->rows, sizeof(crabdb_row_internal_t) * (t->row_count + 1));

    if (!new_rows) return 1;

    t->rows = new_rows;

    crabdb_kv_t* cols = (crabdb_kv_t*)calloc(count, sizeof(crabdb_kv_t));
    if (!cols) return 1;

    for (size_t i = 0; i < count; i++) {
        cols[i].value = values[i];
    }

    t->rows[t->row_count].cols = cols;
    t->rows[t->row_count].count = count;
    t->row_count++;

    return 0;
}

int fossil_io_crabdb_update(crabdb_handle_t* db, const char* query) {
    (void)db;
    (void)query;
    return 0;
}

int fossil_io_crabdb_delete(crabdb_handle_t* db, const char* query) {
    (void)db;
    (void)query;
    return 0;
}

/* =========================================================
 * Schema / Family Tree Model
 * ========================================================= */

int fossil_io_crabdb_create_table(
    crabdb_handle_t* db,
    const char* name
) {
    if (!db || !name) return 1;

    crabdb_table_t* new_tables =
        (crabdb_table_t*)realloc(db->tables, sizeof(crabdb_table_t) * (db->table_count + 1));

    if (!new_tables) return 1;

    db->tables = new_tables;

    crabdb_table_t* t = &db->tables[db->table_count];
    memset(t, 0, sizeof(*t));

    t->name = strdup(name);
    t->rows = NULL;
    t->row_count = 0;

    db->table_count++;

    return 0;
}

int fossil_io_crabdb_link_family(
    crabdb_handle_t* db,
    const char* parent_table,
    const char* child_table
) {
    (void)db;
    (void)parent_table;
    (void)child_table;

    /* future: adjacency list or closure table */
    return 0;
}

/* =========================================================
 * Threading Control
 * ========================================================= */

int fossil_io_crabdb_enable_threading(crabdb_handle_t* db, int enable) {
    if (!db) return 1;
    db->threading_enabled = enable;
    return 0;
}

int fossil_io_crabdb_set_max_threads(crabdb_handle_t* db, size_t threads) {
    if (!db || threads == 0) return 1;
    db->max_threads = threads;
    return 0;
}

/* =========================================================
 * Utilities
 * ========================================================= */

const char* fossil_io_crabdb_last_error(crabdb_handle_t* db) {
    if (!db) return NULL;
    return db->last_error;
}

void fossil_io_crabdb_free_result(crabdb_result_t* result) {
    if (!result) return;

    for (size_t i = 0; i < result->count; i++) {
        free(result->rows[i].values);
    }

    free(result->rows);
    result->rows = NULL;
    result->count = 0;
}
