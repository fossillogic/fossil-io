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

// ===============================
// Error Codes
// ===============================
typedef enum {
    FOSSIL_IO_ARCHIVE_OK = 0,
    FOSSIL_IO_ARCHIVE_ERR_OPEN,
    FOSSIL_IO_ARCHIVE_ERR_READ,
    FOSSIL_IO_ARCHIVE_ERR_WRITE,
    FOSSIL_IO_ARCHIVE_ERR_FORMAT,
    FOSSIL_IO_ARCHIVE_ERR_MEMORY,
    FOSSIL_IO_ARCHIVE_ERR_UNSUPPORTED,
} fossil_io_archive_error_t;

// ===============================
// Archive Handle
// ===============================
typedef struct fossil_io_archive fossil_io_archive_t;

// ===============================
// File Info
// ===============================
typedef struct {
    char *name;         // File path inside archive
    size_t size;        // Uncompressed size
    uint32_t crc32;     // CRC32 checksum
    bool is_dir;        // Directory flag
} fossil_io_archive_file_t;

// ===============================
// Archive Operations
// ===============================

// Open archive for reading or writing
fossil_io_archive_t* fossil_io_archive_open(const char *path, const char *mode, fossil_io_archive_error_t *err);

// Close archive
void fossil_io_archive_close(fossil_io_archive_t *archive);

// List files
fossil_io_archive_file_t* fossil_io_archive_list(fossil_io_archive_t *archive, size_t *count, fossil_io_archive_error_t *err);

// Extract a file
fossil_io_archive_error_t fossil_io_archive_extract(fossil_io_archive_t *archive, const char *filename, const char *dest_path);

// Add a file to archive
fossil_io_archive_error_t fossil_io_archive_add(fossil_io_archive_t *archive, const char *filename, const void *data, size_t size);

// Remove a file from archive (optional, may not be supported in all formats)
fossil_io_archive_error_t fossil_io_archive_remove(fossil_io_archive_t *archive, const char *filename);

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
