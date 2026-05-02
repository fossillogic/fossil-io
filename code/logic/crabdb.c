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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =========================================================
 * Internal structures
 * ========================================================= */

typedef struct {
    char* name;
    fossil_io_crabdb_type_t type;
    int is_primary_key;
    int is_nullable;
} crabdb_column_t;

struct fossil_io_crabdb_table {
    char* name;

    crabdb_column_t* columns;
    size_t column_count;

    crabdb_row_t* rows;
    size_t row_count;

    char* parent;
    char** children;
    size_t child_count;
};

struct fossil_io_crabdb_handle {
    char* path;
    FILE* fp;

    crabdb_table_t* tables;
    size_t table_count;
};

struct fossil_io_crabdb_txn {
    crabdb_handle_t* db;
    crabdb_table_t* snapshot_tables;
    size_t snapshot_count;
};

/* =========================================================
 * Utilities
 * ========================================================= */

static int write_u32(FILE* f, uint32_t v) {
    return fwrite(&v, sizeof(v), 1, f) == 1;
}

static int read_u32(FILE* f, uint32_t* v) {
    return fread(v, sizeof(*v), 1, f) == 1;
}

static int write_u64(FILE* f, uint64_t v) {
    return fwrite(&v, sizeof(v), 1, f) == 1;
}

static int read_u64(FILE* f, uint64_t* v) {
    return fread(v, sizeof(*v), 1, f) == 1;
}

static int write_string(FILE* f, const char* s) {
    uint32_t len = s ? (uint32_t)strlen(s) : 0;
    if (!write_u32(f, len)) return 0;
    if (len && fwrite(s, 1, len, f) != len) return 0;
    return 1;
}

static char* read_string(FILE* f) {
    uint32_t len;
    if (!read_u32(f, &len)) return NULL;

    char* s = malloc(len + 1);
    if (!s) return NULL;

    if (len && fread(s, 1, len, f) != len) {
        free(s);
        return NULL;
    }

    s[len] = '\0';
    return s;
}

static char* crabdb_strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s);
    char* out = (char*)malloc(len + 1);
    if (!out) return NULL;
    memcpy(out, s, len + 1);
    return out;
}

static crabdb_table_t* find_table(crabdb_handle_t* db, const char* name) {
    for (size_t i = 0; i < db->table_count; i++) {
        if (strcmp(db->tables[i].name, name) == 0)
            return &db->tables[i];
    }
    return NULL;
}

static crabdb_column_t* find_column(crabdb_table_t* table, const char* name) {
    for (size_t i = 0; i < table->column_count; i++) {
        if (strcmp(table->columns[i].name, name) == 0)
            return &table->columns[i];
    }
    return NULL;
}

static int write_value(FILE* f, crabdb_value_t* v) {
    if (!write_u32(f, (uint32_t)v->type)) return 0;

    switch (v->type) {
        case FOSSIL_IO_CRABDB_TYPE_I32:
            return write_u32(f, (uint32_t)v->data.i32);

        case FOSSIL_IO_CRABDB_TYPE_I64:
            return write_u64(f, (uint64_t)v->data.i64);

        case FOSSIL_IO_CRABDB_TYPE_CSTR:
            return write_string(f, v->data.cstr);

        case FOSSIL_IO_CRABDB_TYPE_BOOL:
            return write_u32(f, v->data.boolean);

        case FOSSIL_IO_CRABDB_TYPE_F64:
            return fwrite(&v->data.f64, sizeof(double), 1, f) == 1;

        case FOSSIL_IO_CRABDB_TYPE_NULL:
            return 1;

        default:
            /* fallback raw blob */
            if (!write_u64(f, v->size)) return 0;
            return fwrite(v->data.any, 1, v->size, f) == v->size;
    }
}

static int read_value(FILE* f, crabdb_value_t* v) {
    uint32_t type;
    if (!read_u32(f, &type)) return 0;

    v->type = (fossil_io_crabdb_type_t)type;

    switch (v->type) {
        case FOSSIL_IO_CRABDB_TYPE_I32: {
            uint32_t tmp;
            if (!read_u32(f, &tmp)) return 0;
            v->data.i32 = (int32_t)tmp;
            break;
        }

        case FOSSIL_IO_CRABDB_TYPE_I64: {
            uint64_t tmp;
            if (!read_u64(f, &tmp)) return 0;
            v->data.i64 = (int64_t)tmp;
            break;
        }

        case FOSSIL_IO_CRABDB_TYPE_CSTR:
            v->data.cstr = read_string(f);
            break;

        case FOSSIL_IO_CRABDB_TYPE_BOOL: {
            uint32_t tmp;
            if (!read_u32(f, &tmp)) return 0;
            v->data.boolean = (uint8_t)tmp;
            break;
        }

        case FOSSIL_IO_CRABDB_TYPE_F64:
            if (fread(&v->data.f64, sizeof(double), 1, f) != 1)
                return 0;
            break;

        case FOSSIL_IO_CRABDB_TYPE_NULL:
            break;

        default: {
            uint64_t size;
            if (!read_u64(f, &size)) return 0;
            v->size = (size_t)size;
            v->data.any = malloc(v->size);
            if (!v->data.any) return 0;
            if (fread(v->data.any, 1, v->size, f) != v->size)
                return 0;
        }
    }

    return 1;
}

static int crabdb_save(crabdb_handle_t* db) {
    FILE* f = fopen(db->path, "wb");
    if (!f) return 0;

    fwrite(CRABDB_MAGIC, 1, 6, f);
    write_u32(f, CRABDB_VERSION);

    write_u32(f, (uint32_t)db->table_count);

    for (size_t i = 0; i < db->table_count; i++) {
        crabdb_table_t* t = &db->tables[i];

        write_string(f, t->name);
        write_string(f, t->parent);

        write_u32(f, (uint32_t)t->child_count);
        for (size_t c = 0; c < t->child_count; c++)
            write_string(f, t->children[c]);

        /* columns */
        write_u32(f, (uint32_t)t->column_count);
        for (size_t c = 0; c < t->column_count; c++) {
            crabdb_column_t* col = &t->columns[c];
            write_string(f, col->name);
            write_u32(f, col->type);
            write_u32(f, col->is_primary_key);
            write_u32(f, col->is_nullable);
        }

        /* rows */
        write_u32(f, (uint32_t)t->row_count);
        for (size_t r = 0; r < t->row_count; r++) {
            crabdb_row_t* row = &t->rows[r];
            for (size_t v = 0; v < row->column_count; v++) {
                write_value(f, &row->values[v]);
            }
        }
    }

    fclose(f);
    return 1;
}

static int crabdb_load(crabdb_handle_t* db) {
    FILE* f = fopen(db->path, "rb");
    if (!f) return 1; /* new db */

    char magic[7] = {0};
    fread(magic, 1, 6, f);

    if (strcmp(magic, CRABDB_MAGIC) != 0) {
        fclose(f);
        return 0;
    }

    uint32_t version;
    read_u32(f, &version);

    uint32_t table_count;
    read_u32(f, &table_count);

    db->tables = calloc(table_count, sizeof(*db->tables));
    db->table_count = table_count;

    for (uint32_t i = 0; i < table_count; i++) {
        crabdb_table_t* t = &db->tables[i];

        t->name = read_string(f);
        t->parent = read_string(f);

        uint32_t child_count;
        read_u32(f, &child_count);

        t->children = malloc(sizeof(char*) * child_count);
        t->child_count = child_count;

        for (uint32_t c = 0; c < child_count; c++)
            t->children[c] = read_string(f);

        uint32_t col_count;
        read_u32(f, &col_count);

        t->columns = calloc(col_count, sizeof(*t->columns));
        t->column_count = col_count;

        for (uint32_t c = 0; c < col_count; c++) {
            crabdb_column_t* col = &t->columns[c];
            col->name = read_string(f);

            uint32_t tmp;
            read_u32(f, &tmp); col->type = tmp;
            read_u32(f, &tmp); col->is_primary_key = tmp;
            read_u32(f, &tmp); col->is_nullable = tmp;
        }

        uint32_t row_count;
        read_u32(f, &row_count);

        t->rows = calloc(row_count, sizeof(*t->rows));
        t->row_count = row_count;

        for (uint32_t r = 0; r < row_count; r++) {
            crabdb_row_t* row = &t->rows[r];
            row->column_count = col_count;
            row->values = calloc(col_count, sizeof(crabdb_value_t));

            for (uint32_t v = 0; v < col_count; v++) {
                read_value(f, &row->values[v]);
            }
        }
    }

    fclose(f);
    return 1;
}

/* =========================================================
 * Type validation
 * ========================================================= */

static int validate_type(fossil_io_crabdb_type_t type, crabdb_value_t* v) {
    if (!v) return 0;
    if (v->type == FOSSIL_IO_CRABDB_TYPE_NULL) return 1;
    return v->type == type || type == FOSSIL_IO_CRABDB_TYPE_ANY;
}

/* =========================================================
 * Database lifecycle
 * ========================================================= */

crabdb_handle_t* fossil_io_crabdb_open(const char* path) {
    if (!path) return NULL;

    crabdb_handle_t* db = calloc(1, sizeof(*db));
    if (!db) return NULL;

    db->path = crabdb_strdup(path);

    if (!crabdb_load(db)) {
        free(db);
        return NULL;
    }

    return db;
}

void fossil_io_crabdb_close(crabdb_handle_t* db) {
    if (!db) return;

    crabdb_save(db);

    if (db->fp) fclose(db->fp);

    for (size_t i = 0; i < db->table_count; i++) {
        crabdb_table_t* t = &db->tables[i];
        free(t->name);

        for (size_t j = 0; j < t->column_count; j++)
            free(t->columns[j].name);

        free(t->columns);

        for (size_t r = 0; r < t->row_count; r++) {
            free(t->rows[r].values);
        }
        free(t->rows);

        free(t->parent);
        for (size_t c = 0; c < t->child_count; c++)
            free(t->children[c]);
        free(t->children);
    }

    free(db->tables);
    free(db->path);
    free(db);
}

/* =========================================================
 * Schema
 * ========================================================= */

fossil_io_crabdb_error_t fossil_io_crabdb_create_table(
    crabdb_handle_t* db,
    const char* table_name
) {
    if (!db || !table_name) return FOSSIL_IO_CRABDB_ERR_INVALID_ARG;
    if (find_table(db, table_name)) return FOSSIL_IO_CRABDB_ERR_CONSTRAINT;

    db->tables = realloc(db->tables, sizeof(*db->tables) * (db->table_count + 1));
    if (!db->tables) return FOSSIL_IO_CRABDB_ERR_MEMORY;

    crabdb_table_t* t = &db->tables[db->table_count++];
    memset(t, 0, sizeof(*t));

    t->name = crabdb_strdup(table_name);

    return FOSSIL_IO_CRABDB_OK;
}

fossil_io_crabdb_error_t fossil_io_crabdb_define_column(
    crabdb_handle_t* db,
    const char* table_name,
    const char* column_name,
    fossil_io_crabdb_type_t type,
    int is_primary_key,
    int is_nullable
) {
    if (!db || !table_name || !column_name)
        return FOSSIL_IO_CRABDB_ERR_INVALID_ARG;

    crabdb_table_t* t = find_table(db, table_name);
    if (!t) return FOSSIL_IO_CRABDB_ERR_NOT_FOUND;

    t->columns = realloc(t->columns, sizeof(*t->columns) * (t->column_count + 1));
    if (!t->columns) return FOSSIL_IO_CRABDB_ERR_MEMORY;

    crabdb_column_t* col = &t->columns[t->column_count++];
    col->name = crabdb_strdup(column_name);
    col->type = type;
    col->is_primary_key = is_primary_key;
    col->is_nullable = is_nullable;

    return FOSSIL_IO_CRABDB_OK;
}

/* =========================================================
 * Relations
 * ========================================================= */

fossil_io_crabdb_error_t fossil_io_crabdb_set_parent(
    crabdb_handle_t* db,
    const char* child_table,
    const char* parent_table
) {
    if (!db || !child_table || !parent_table)
        return FOSSIL_IO_CRABDB_ERR_INVALID_ARG;

    crabdb_table_t* child = find_table(db, child_table);
    crabdb_table_t* parent = find_table(db, parent_table);

    if (!child || !parent) return FOSSIL_IO_CRABDB_ERR_NOT_FOUND;

    free(child->parent);
    child->parent = crabdb_strdup(parent_table);

    return FOSSIL_IO_CRABDB_OK;
}

fossil_io_crabdb_error_t fossil_io_crabdb_set_child(
    crabdb_handle_t* db,
    const char* parent_table,
    const char* child_table
) {
    if (!db || !parent_table || !child_table)
        return FOSSIL_IO_CRABDB_ERR_INVALID_ARG;

    crabdb_table_t* parent = find_table(db, parent_table);
    if (!parent) return FOSSIL_IO_CRABDB_ERR_NOT_FOUND;

    parent->children = realloc(parent->children,
        sizeof(char*) * (parent->child_count + 1));

    parent->children[parent->child_count++] = crabdb_strdup(child_table);

    return FOSSIL_IO_CRABDB_OK;
}

/* =========================================================
 * WHERE parser (simple: col=value)
 * ========================================================= */

static int match_where(crabdb_row_t* row, crabdb_table_t* table, const char* clause) {
    if (!clause) return 1;

    char col[64], val[64];
    if (sscanf(clause, "%63s = %63s", col, val) != 2)
        return 0;

    for (size_t i = 0; i < table->column_count; i++) {
        if (strcmp(table->columns[i].name, col) == 0) {
            crabdb_value_t* v = &row->values[i];
            if (v->type == FOSSIL_IO_CRABDB_TYPE_I32)
                return atoi(val) == v->data.i32;
        }
    }
    return 0;
}

/* =========================================================
 * CRUD
 * ========================================================= */

fossil_io_crabdb_error_t fossil_io_crabdb_insert(
    crabdb_handle_t* db,
    const char* table,
    crabdb_value_t* values,
    size_t value_count
) {
    if (!db || !table || !values)
        return FOSSIL_IO_CRABDB_ERR_INVALID_ARG;

    crabdb_table_t* t = find_table(db, table);
    if (!t) return FOSSIL_IO_CRABDB_ERR_NOT_FOUND;

    if (value_count != t->column_count)
        return FOSSIL_IO_CRABDB_ERR_SCHEMA;

    crabdb_row_t row;
    row.column_count = value_count;
    row.values = malloc(sizeof(crabdb_value_t) * value_count);

    for (size_t i = 0; i < value_count; i++) {
        crabdb_column_t* col = &t->columns[i];

        if (!validate_type(col->type, &values[i]))
            return FOSSIL_IO_CRABDB_ERR_TYPE_MISMATCH;

        if (!col->is_nullable && values[i].type == FOSSIL_IO_CRABDB_TYPE_NULL)
            return FOSSIL_IO_CRABDB_ERR_CONSTRAINT;

        row.values[i] = values[i];
    }

    t->rows = realloc(t->rows, sizeof(*t->rows) * (t->row_count + 1));
    t->rows[t->row_count++] = row;

    return FOSSIL_IO_CRABDB_OK;
}

fossil_io_crabdb_error_t fossil_io_crabdb_update(
    crabdb_handle_t* db,
    const char* table,
    crabdb_value_t* values,
    const char* where_clause
) {
    if (!db || !table || !values)
        return FOSSIL_IO_CRABDB_ERR_INVALID_ARG;

    crabdb_table_t* t = find_table(db, table);
    if (!t) return FOSSIL_IO_CRABDB_ERR_NOT_FOUND;

    for (size_t r = 0; r < t->row_count; r++) {
        if (!match_where(&t->rows[r], t, where_clause))
            continue;

        for (size_t c = 0; c < t->column_count; c++) {
            if (strcmp(values[c].column, t->columns[c].name) == 0) {
                if (!validate_type(t->columns[c].type, &values[c]))
                    return FOSSIL_IO_CRABDB_ERR_TYPE_MISMATCH;

                t->rows[r].values[c] = values[c];
            }
        }
    }

    return FOSSIL_IO_CRABDB_OK;
}

fossil_io_crabdb_error_t fossil_io_crabdb_delete(
    crabdb_handle_t* db,
    const char* table,
    const char* where_clause
) {
    if (!db || !table)
        return FOSSIL_IO_CRABDB_ERR_INVALID_ARG;

    crabdb_table_t* t = find_table(db, table);
    if (!t) return FOSSIL_IO_CRABDB_ERR_NOT_FOUND;

    size_t write = 0;
    for (size_t read = 0; read < t->row_count; read++) {
        if (!match_where(&t->rows[read], t, where_clause)) {
            t->rows[write++] = t->rows[read];
        }
    }
    t->row_count = write;

    return FOSSIL_IO_CRABDB_OK;
}

/* =========================================================
 * Query (minimal SQL-like)
 * ========================================================= */

crabdb_row_t* fossil_io_crabdb_query(
    crabdb_handle_t* db,
    const char* query
) {
    if (!db || !query) return NULL;

    char table[64];
    if (sscanf(query, "SELECT * FROM %63s", table) != 1)
        return NULL;

    crabdb_table_t* t = find_table(db, table);
    if (!t) return NULL;

    return t->rows;
}

/* =========================================================
 * Transactions (snapshot copy)
 * ========================================================= */

crabdb_txn_t* fossil_io_crabdb_begin(crabdb_handle_t* db) {
    if (!db) return NULL;

    crabdb_txn_t* txn = calloc(1, sizeof(*txn));
    txn->db = db;

    txn->snapshot_count = db->table_count;
    txn->snapshot_tables = malloc(sizeof(*txn->snapshot_tables) * db->table_count);

    memcpy(txn->snapshot_tables, db->tables,
           sizeof(*txn->snapshot_tables) * db->table_count);

    return txn;
}

fossil_io_crabdb_error_t fossil_io_crabdb_commit(crabdb_txn_t* txn) {
    if (!txn) return FOSSIL_IO_CRABDB_ERR_INVALID_ARG;

    crabdb_save(txn->db);

    free(txn->snapshot_tables);
    free(txn);

    return FOSSIL_IO_CRABDB_OK;
}

fossil_io_crabdb_error_t fossil_io_crabdb_rollback(crabdb_txn_t* txn) {
    if (!txn) return FOSSIL_IO_CRABDB_ERR_INVALID_ARG;

    txn->db->tables = txn->snapshot_tables;
    txn->db->table_count = txn->snapshot_count;

    free(txn);
    return FOSSIL_IO_CRABDB_OK;
}

/* =========================================================
 * Utilities
 * ========================================================= */

const char* fossil_io_crabdb_error_string(fossil_io_crabdb_error_t err) {
    switch (err) {
        case FOSSIL_IO_CRABDB_OK: return "OK";
        case FOSSIL_IO_CRABDB_ERR_IO: return "IO error";
        case FOSSIL_IO_CRABDB_ERR_MEMORY: return "Memory error";
        case FOSSIL_IO_CRABDB_ERR_SCHEMA: return "Schema error";
        case FOSSIL_IO_CRABDB_ERR_TYPE_MISMATCH: return "Type mismatch";
        case FOSSIL_IO_CRABDB_ERR_NOT_FOUND: return "Not found";
        case FOSSIL_IO_CRABDB_ERR_CONSTRAINT: return "Constraint violation";
        case FOSSIL_IO_CRABDB_ERR_TRANSACTION: return "Transaction error";
        case FOSSIL_IO_CRABDB_ERR_QUERY: return "Query error";
        case FOSSIL_IO_CRABDB_ERR_INVALID_ARG: return "Invalid argument";
        default: return "Generic error";
    }
}

const char* fossil_io_crabdb_version(void) {
    return "0.1.0";
}
