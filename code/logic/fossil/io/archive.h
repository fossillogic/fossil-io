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
#ifndef FOSSIL_IO_ARCHIVE_H
#define FOSSIL_IO_ARCHIVE_H

#include "stream.h"

#ifdef __cplusplus
extern "C" {
#endif

// ======================================================
// Fossil IO — Archive Sub-Library
// ======================================================

/**
 * Supported archive types.
 */
typedef enum fossil_io_archive_type {
    FOSSIL_IO_ARCHIVE_UNKNOWN = 0,
    FOSSIL_IO_ARCHIVE_ZIP,
    FOSSIL_IO_ARCHIVE_TAR,
    FOSSIL_IO_ARCHIVE_TARGZ,
    FOSSIL_IO_ARCHIVE_RAR,
    FOSSIL_IO_ARCHIVE_7Z,
    FOSSIL_IO_ARCHIVE_ALL // wildcard for detection
} fossil_io_archive_type_t;

/**
 * Opaque archive handle.
 */
typedef struct fossil_io_archive fossil_io_archive_t;

/**
 * File entry inside an archive.
 */
typedef struct fossil_io_archive_entry {
    char *name;              // Path/name inside archive
    size_t size;             // Size of the file
    bool is_directory;       // True if directory
    uint64_t modified_time;  // Epoch timestamp
} fossil_io_archive_entry_t;

// ======================================================
// Initialization & Cleanup
// ======================================================

/**
 * Initialize an archive handle.
 */
fossil_io_archive_t* fossil_io_archive_open(const char *path, fossil_io_archive_type_t type);

/**
 * Close and free an archive handle.
 */
void fossil_io_archive_close(fossil_io_archive_t *archive);

// ======================================================
// Archive Inspection
// ======================================================

/**
 * Get archive type (auto-detect if unknown)
 */
fossil_io_archive_type_t fossil_io_archive_get_type(const char *path);

/**
 * List all entries in the archive.
 * Returns number of entries, or -1 on error.
 * Caller must free entries array using fossil_io_archive_free_entries().
 */
ssize_t fossil_io_archive_list(fossil_io_archive_t *archive, fossil_io_archive_entry_t **entries);

/**
 * Free memory allocated for entries.
 */
void fossil_io_archive_free_entries(fossil_io_archive_entry_t *entries, size_t count);

// ======================================================
// Extraction
// ======================================================

/**
 * Extract a single file to the specified path.
 */
bool fossil_io_archive_extract_file(fossil_io_archive_t *archive, const char *entry_name, const char *dest_path);

/**
 * Extract all files in the archive to the specified directory.
 */
bool fossil_io_archive_extract_all(fossil_io_archive_t *archive, const char *dest_dir);

// ======================================================
// Creation / Modification
// ======================================================

/**
 * Create a new archive at path with the given type.
 */
fossil_io_archive_t* fossil_io_archive_create(const char *path, fossil_io_archive_type_t type);

/**
 * Add a file from disk to the archive.
 */
bool fossil_io_archive_add_file(fossil_io_archive_t *archive, const char *src_path, const char *archive_path);

/**
 * Add a directory recursively to the archive.
 */
bool fossil_io_archive_add_directory(fossil_io_archive_t *archive, const char *src_dir, const char *archive_dir);

/**
 * Remove a file from the archive.
 */
bool fossil_io_archive_remove(fossil_io_archive_t *archive, const char *entry_name);

// ======================================================
// Utility / Info
// ======================================================

/**
 * Check if an entry exists in the archive.
 */
bool fossil_io_archive_exists(fossil_io_archive_t *archive, const char *entry_name);

/**
 * Get the size of an entry in bytes.
 */
ssize_t fossil_io_archive_entry_size(fossil_io_archive_t *archive, const char *entry_name);

/**
 * Print archive contents to stdout.
 */
void fossil_io_archive_print(fossil_io_archive_t *archive);

#ifdef __cplusplus
}

/**
 * Namespace for the Fossil Logic I/O library.
 */
namespace fossil {

    /**
     * Namespace for the I/O utilities.
     */
    namespace io {



    }

}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
