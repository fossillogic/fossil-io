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
#ifndef FOSSIL_IO_DIR_H
#define FOSSIL_IO_DIR_H

#include "file.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Directory Entry Structure
// ============================================================================

/**
 * @brief Represents a directory entry (file, directory, symlink, etc.)
 */
typedef struct fossil_io_dir_entry_t {
    char name[512];     ///< Entry name
    char path[1024];    ///< Full resolved path
    int32_t type;       ///< 0=file, 1=dir, 2=symlink, 3=other
    uint64_t size;      ///< File size in bytes (0 for dirs)
    uint64_t modified;  ///< Last modified timestamp (UTC epoch)
} fossil_io_dir_entry_t;


// ============================================================================
// Directory Iterator
// ============================================================================

/**
 * @brief Represents an open directory iterator.
 */
typedef struct fossil_io_dir_iter_t {
    void *handle;           ///< Platform-specific DIR*/HANDLE
    char basepath[1024];    ///< The directory being iterated
    fossil_io_dir_entry_t current;
    int32_t active;         ///< 1=active, 0=end or closed
} fossil_io_dir_iter_t;


// ============================================================================
// Basic Directory Operations
// ============================================================================

int32_t fossil_io_dir_exists(const char *path);
int32_t fossil_io_dir_create(const char *path);
int32_t fossil_io_dir_remove(const char *path);
int32_t fossil_io_dir_remove_recursive(const char *path);
int32_t fossil_io_dir_clear(const char *path);

int32_t fossil_io_dir_copy(const char *src, const char *dst);
int32_t fossil_io_dir_copy_recursive(const char *src, const char *dst);

int32_t fossil_io_dir_move(const char *src, const char *dst);
int32_t fossil_io_dir_rename(const char *oldpath, const char *newpath);


// ============================================================================
// Directory Iteration API
// ============================================================================

int32_t fossil_io_dir_iter_open(fossil_io_dir_iter_t *it, const char *path);
int32_t fossil_io_dir_iter_next(fossil_io_dir_iter_t *it);   ///< Returns 1 if valid, 0 if end
void    fossil_io_dir_iter_close(fossil_io_dir_iter_t *it);

int32_t fossil_io_dir_list(const char *path,
                           fossil_io_dir_entry_t *entries,
                           size_t *count,
                           size_t max_entries);


// ============================================================================
// Path Utilities
// ============================================================================

int32_t fossil_io_dir_is_absolute(const char *path);
int32_t fossil_io_dir_join(const char *base, const char *name, char *out, size_t outsz);
int32_t fossil_io_dir_basename(const char *path, char *out, size_t outsz);
int32_t fossil_io_dir_dirname(const char *path, char *out, size_t outsz);
int32_t fossil_io_dir_normalize(const char *path, char *out, size_t outsz);
int32_t fossil_io_dir_realpath(const char *path, char *out, size_t outsz);


// ============================================================================
// Directory Inspection Queries
// ============================================================================

int32_t fossil_io_dir_is_empty(const char *path);
int32_t fossil_io_dir_count(const char *path, size_t *count);
int32_t fossil_io_dir_size(const char *path, uint64_t *bytes); // recursive sum

// ============================================================================
// Filtering + Scanning
// ============================================================================

/**
 * @brief Callback used for scanning files.
 * Return 1 to continue, 0 to stop traversal.
 */
typedef int32_t (*fossil_io_dir_scan_callback)(
    const fossil_io_dir_entry_t *entry,
    void *userdata);

int32_t fossil_io_dir_scan(const char *path, fossil_io_dir_scan_callback cb, void *userdata);
int32_t fossil_io_dir_scan_recursive(const char *path, fossil_io_dir_scan_callback cb, void *userdata);


// ============================================================================
// Permissions (platform supported)
// ============================================================================

int32_t fossil_io_dir_set_permissions(const char *path, int32_t mode);
int32_t fossil_io_dir_get_permissions(const char *path, int32_t *mode);


// ============================================================================
// Temp Directory Utilities
// ============================================================================

int32_t fossil_io_dir_temp(char *out, size_t outsz);
int32_t fossil_io_dir_create_temp(char *out, size_t outsz);


// ============================================================================
// Metadata Queries
// ============================================================================

int32_t fossil_io_dir_get_modified(const char *path, uint64_t *timestamp);
int32_t fossil_io_dir_get_created(const char *path, uint64_t *timestamp);


// ============================================================================
// Directory Sync / Backup
// ============================================================================

int32_t fossil_io_dir_backup(const char *src, const char *suffix);
int32_t fossil_io_dir_mirror(const char *src, const char *dst);
int32_t fossil_io_dir_sync(const char *src, const char *dst, int32_t delete_extraneous);


// ============================================================================
// Low-Level Helpers
// ============================================================================

int32_t fossil_io_dir_is_directory(const char *path);
int32_t fossil_io_dir_is_file(const char *path);
int32_t fossil_io_dir_is_symlink(const char *path);

#ifdef __cplusplus
}
#include <string>

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
