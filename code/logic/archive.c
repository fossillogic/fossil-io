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
#include "fossil/io/archive.h"
#include "fossil/io/cstring.h"
#include "fossil/io/output.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <utime.h>
#ifndef AT_FDCWD
#define AT_FDCWD -100
#endif
#endif

// ======================================================
// Opaque archive handle definition
// ======================================================

struct fossil_io_archive
{
    char *path;
    fossil_io_archive_type_t type;
    fossil_io_archive_mode_t mode;
    fossil_io_archive_compression_t compression;
    // Internal storage for entries
    fossil_io_archive_entry_t *entries;
    size_t entry_count;
    size_t entry_capacity;
};

// ======================================================
// Helper: Create directory recursively
// ======================================================
static bool fossil_io_create_directories(const char *path)
{
    if (!path)
        return false;

    char *path_copy = fossil_io_cstring_dup(path);
    if (!path_copy)
        return false;

    char *p = path_copy;

    // Skip root slash on Unix or drive letter on Windows
    if (*p == '/')
        p++;
#ifdef _WIN32
    else if (strlen(p) >= 2 && p[1] == ':')
        p += 2;
    if (*p == '/' || *p == '\\')
        p++;
#endif

    while (*p)
    {
        while (*p && *p != '/' && *p != '\\')
            p++;
        if (*p)
        {
            char sep = *p;
            *p = '\0';

#ifdef _WIN32
            CreateDirectoryA(path_copy, NULL);
#else
            mkdir(path_copy, 0755);
#endif

            *p = sep;
            p++;
        }
    }

    // Create the final directory if it doesn't end with a separator
    if (strlen(path_copy) > 0)
    {
#ifdef _WIN32
        CreateDirectoryA(path_copy, NULL);
#else
        mkdir(path_copy, 0755);
#endif
    }

    fossil_io_cstring_free(path_copy);
    return true;
}

// ======================================================
// Archive type detection
// ======================================================
static bool ends_with(const char *str, const char *suffix)
{
    return fossil_io_cstring_ends_with(str, suffix) == 1;
}

fossil_io_archive_type_t fossil_io_archive_get_type(const char *path)
{
    if (!path) return FOSSIL_IO_ARCHIVE_UNKNOWN;

    if (ends_with(path, ".tar.gz") || ends_with(path, ".tgz"))
        return FOSSIL_IO_ARCHIVE_TARGZ;
    if (ends_with(path, ".tar.bz2") || ends_with(path, ".tbz2"))
        return FOSSIL_IO_ARCHIVE_TARBZ2;
    if (ends_with(path, ".tar.xz") || ends_with(path, ".txz"))
        return FOSSIL_IO_ARCHIVE_TARXZ;

    const char *ext = strrchr(path, '.');
    if (!ext) return FOSSIL_IO_ARCHIVE_UNKNOWN;
    ext++;

    if (fossil_io_cstring_icmp(ext, "zip") == 0) return FOSSIL_IO_ARCHIVE_ZIP;
    if (fossil_io_cstring_icmp(ext, "tar") == 0) return FOSSIL_IO_ARCHIVE_TAR;
    if (fossil_io_cstring_icmp(ext, "gz")  == 0) return FOSSIL_IO_ARCHIVE_GZ;

    return FOSSIL_IO_ARCHIVE_UNKNOWN;
}

// ======================================================
// Initialization & Cleanup
// ======================================================

fossil_io_archive_t *fossil_io_archive_open(const char *path, fossil_io_archive_type_t type, fossil_io_archive_mode_t mode, fossil_io_archive_compression_t compression)
{
    if (!path)
        return NULL;

    fossil_io_archive_t *archive = malloc(sizeof(fossil_io_archive_t));
    if (!archive)
        return NULL;

    archive->path = fossil_io_cstring_dup(path);
    archive->type = (type == FOSSIL_IO_ARCHIVE_UNKNOWN) ? fossil_io_archive_get_type(path) : type;
    archive->mode = mode;
    archive->compression = compression;
    archive->entries = NULL;
    archive->entry_count = 0;
    archive->entry_capacity = 0;

    if (!archive->path)
    {
        free(archive);
        return NULL;
    }

    return archive;
}

void fossil_io_archive_close(fossil_io_archive_t *archive)
{
    if (!archive)
        return;

    fossil_io_cstring_free(archive->path);

    if (archive->entries)
    {
        for (size_t i = 0; i < archive->entry_count; i++)
        {
            fossil_io_cstring_free(archive->entries[i].name);
        }
        free(archive->entries);
    }

    free(archive);
}

fossil_io_archive_t *fossil_io_archive_create(const char *path, fossil_io_archive_type_t type, fossil_io_archive_compression_t compression)
{
    if (!path || type == FOSSIL_IO_ARCHIVE_UNKNOWN)
        return NULL;

    // Create parent directories
    char *path_copy = fossil_io_cstring_dup(path);
    if (path_copy)
    {
        char *last_slash = strrchr(path_copy, '/');
        if (!last_slash)
            last_slash = strrchr(path_copy, '\\');
        if (last_slash)
        {
            *last_slash = '\0';
            fossil_io_create_directories(path_copy);
        }
        fossil_io_cstring_free(path_copy);
    }

    return fossil_io_archive_open(path, type, FOSSIL_IO_ARCHIVE_WRITE, compression);
}

// ======================================================
// Archive Statistics
// ======================================================

bool fossil_io_archive_get_stats(fossil_io_archive_t *archive, fossil_io_archive_stats_t *stats)
{
    if (!archive || !stats)
        return false;

    stats->total_entries = archive->entry_count;
    stats->total_size = 0;
    stats->compressed_size = 0;

    for (size_t i = 0; i < archive->entry_count; i++)
    {
        stats->total_size += archive->entries[i].size;
        stats->compressed_size += archive->entries[i].compressed_size;
    }

    stats->compression_ratio = stats->total_size > 0 ? (double)stats->compressed_size / stats->total_size : 0.0;

    return true;
}

// ======================================================
// Entry Management
// ======================================================

ssize_t fossil_io_archive_list(fossil_io_archive_t *archive, fossil_io_archive_entry_t **entries)
{
    if (!archive || !entries)
        return -1;

    if (archive->entry_count == 0)
    {
        *entries = NULL;
        return 0;
    }

    // Count unique entries first
    size_t unique_count = 0;
    for (size_t i = 0; i < archive->entry_count; i++)
    {
        bool is_duplicate = false;
        for (size_t j = 0; j < i; j++)
        {
            if (archive->entries[i].name && archive->entries[j].name &&
                fossil_io_cstring_equals(archive->entries[i].name, archive->entries[j].name))
            {
                is_duplicate = true;
                break;
            }
        }
        if (!is_duplicate)
        {
            unique_count++;
        }
    }

    if (unique_count == 0)
    {
        *entries = NULL;
        return 0;
    }

    fossil_io_archive_entry_t *entry_copy = malloc(sizeof(fossil_io_archive_entry_t) * unique_count);
    if (!entry_copy)
        return -1;

    // Zero-initialize new allocation
    memset(entry_copy, 0, sizeof(fossil_io_archive_entry_t) * unique_count);

    size_t copy_index = 0;
    for (size_t i = 0; i < archive->entry_count; i++)
    {
        bool is_duplicate = false;
        for (size_t j = 0; j < i; j++)
        {
            if (archive->entries[i].name && archive->entries[j].name &&
                fossil_io_cstring_equals(archive->entries[i].name, archive->entries[j].name))
            {
                is_duplicate = true;
                break;
            }
        }

        if (!is_duplicate)
        {
            fossil_io_archive_entry_t *dst = &entry_copy[copy_index];
            fossil_io_archive_entry_t *src = &archive->entries[i];

            memcpy(dst, src, sizeof(*dst));
            dst->name = fossil_io_cstring_dup(src->name);
            if (!dst->name)
            {
                // Cleanup on failure
                for (size_t k = 0; k < copy_index; k++)
                {
                    fossil_io_cstring_free(entry_copy[k].name);
                }
                free(entry_copy);
                return -1;
            }
            copy_index++;
        }
    }

    *entries = entry_copy;
    return (ssize_t)unique_count;
}

void fossil_io_archive_free_entries(fossil_io_archive_entry_t *entries, size_t count)
{
    if (!entries)
        return;

    for (size_t i = 0; i < count; i++)
    {
        fossil_io_cstring_free(entries[i].name);
    }
    free(entries);
}

// ======================================================
// Entry Operations
// ======================================================

bool fossil_io_archive_exists(fossil_io_archive_t *archive, const char *entry_name)
{
    if (!archive || !entry_name)
        return false;

    for (size_t i = 0; i < archive->entry_count; i++)
    {
        if (archive->entries[i].name && fossil_io_cstring_equals(archive->entries[i].name, entry_name))
        {
            return true;
        }
    }
    return false;
}

ssize_t fossil_io_archive_entry_size(fossil_io_archive_t *archive, const char *entry_name)
{
    if (!archive || !entry_name)
        return -1;

    for (size_t i = 0; i < archive->entry_count; i++)
    {
        if (archive->entries[i].name && fossil_io_cstring_equals(archive->entries[i].name, entry_name))
        {
            return (ssize_t)archive->entries[i].size;
        }
    }
    return -1;
}

// ======================================================
// File Operations (Simplified Implementation)
// ======================================================

bool fossil_io_archive_add_file(fossil_io_archive_t *archive, const char *src_path, const char *archive_path)
{
    if (!archive || !src_path || !archive_path)
        return false;
    if (!(archive->mode & (FOSSIL_IO_ARCHIVE_WRITE | FOSSIL_IO_ARCHIVE_APPEND)))
        return false;

    FILE *file = fopen(src_path, "rb");
    if (!file)
        return false;

    if (fseek(file, 0, SEEK_END) != 0)
    {
        fclose(file);
        return false;
    }

    long file_size = ftell(file);
    fclose(file);

    if (file_size < 0)
        return false;

    // Resize entries array if needed
    if (archive->entry_count >= archive->entry_capacity)
    {
        size_t old_capacity = archive->entry_capacity;
        size_t new_capacity = archive->entry_capacity == 0 ? 8 : archive->entry_capacity * 2;
        fossil_io_archive_entry_t *new_entries = realloc(archive->entries,
                                                         sizeof(fossil_io_archive_entry_t) * new_capacity);
        if (!new_entries)
            return false;

        // Zero-initialize new memory
        memset(new_entries + old_capacity, 0,
               (new_capacity - old_capacity) * sizeof(fossil_io_archive_entry_t));

        archive->entries = new_entries;
        archive->entry_capacity = new_capacity;
    }

    // Add entry
    fossil_io_archive_entry_t *entry = &archive->entries[archive->entry_count];
    memset(entry, 0, sizeof(fossil_io_archive_entry_t));

    entry->name = fossil_io_cstring_dup(archive_path);
    if (!entry->name)
        return false;

    time_t now = time(NULL);
    entry->size = (size_t)file_size;
    entry->compressed_size = (size_t)file_size;
    entry->is_directory = false;
    entry->is_encrypted = false;
    entry->modified_time = now;
    entry->created_time = now;
    entry->crc32 = 0;
    entry->permissions = 0644;

    archive->entry_count++;
    return true;
}

bool fossil_io_archive_add_directory(fossil_io_archive_t *archive, const char *src_dir, const char *archive_dir)
{
    if (!archive || !src_dir || !archive_dir)
        return false;

    // Resize entries array if needed
    if (archive->entry_count >= archive->entry_capacity)
    {
        size_t old_capacity = archive->entry_capacity;
        size_t new_capacity = archive->entry_capacity == 0 ? 8 : archive->entry_capacity * 2;
        fossil_io_archive_entry_t *new_entries = realloc(archive->entries,
                                                         sizeof(fossil_io_archive_entry_t) * new_capacity);
        if (!new_entries)
            return false;

        // Zero-initialize new memory
        memset(new_entries + old_capacity, 0,
               (new_capacity - old_capacity) * sizeof(fossil_io_archive_entry_t));

        archive->entries = new_entries;
        archive->entry_capacity = new_capacity;
    }

    // Add directory entry
    fossil_io_archive_entry_t *entry = &archive->entries[archive->entry_count];
    memset(entry, 0, sizeof(fossil_io_archive_entry_t));

    entry->name = fossil_io_cstring_dup(archive_dir);
    if (!entry->name)
        return false;

    time_t now = time(NULL);
    entry->size = 0;
    entry->compressed_size = 0;
    entry->is_directory = true;
    entry->is_encrypted = false;
    entry->modified_time = now;
    entry->created_time = now;
    entry->crc32 = 0;
    entry->permissions = 0755;

    archive->entry_count++;
    return true;
}

bool fossil_io_archive_extract_file(fossil_io_archive_t *archive, const char *entry_name, const char *dest_path)
{
    if (!archive || !entry_name || !dest_path)
        return false;
    if (!(archive->mode & FOSSIL_IO_ARCHIVE_READ))
        return false;

    if (!fossil_io_archive_exists(archive, entry_name))
        return false;

    // Create destination directory
    char *dest_copy = fossil_io_cstring_dup(dest_path);
    if (dest_copy)
    {
        char *last_slash = strrchr(dest_copy, '/');
        if (!last_slash)
            last_slash = strrchr(dest_copy, '\\');
        if (last_slash)
        {
            *last_slash = '\0';
            fossil_io_create_directories(dest_copy);
        }
        fossil_io_cstring_free(dest_copy);
    }

    // Simplified: create empty file
    FILE *file = fopen(dest_path, "wb");
    if (!file)
        return false;
    fclose(file);

    return true;
}

bool fossil_io_archive_extract_all(fossil_io_archive_t *archive, const char *dest_dir)
{
    if (!archive || !dest_dir)
        return false;

    fossil_io_create_directories(dest_dir);

    for (size_t i = 0; i < archive->entry_count; i++)
    {
        size_t path_len = strlen(dest_dir) + strlen(archive->entries[i].name) + 2;
        char *full_path = malloc(path_len);
        if (!full_path)
            return false;

        snprintf(full_path, path_len, "%s/%s", dest_dir, archive->entries[i].name);

        bool success = true;
        if (archive->entries[i].is_directory)
        {
            success = fossil_io_create_directories(full_path);
        }
        else
        {
            success = fossil_io_archive_extract_file(archive, archive->entries[i].name, full_path);
        }

        free(full_path);

        if (!success)
            return false;
    }

    return true;
}

bool fossil_io_archive_remove(fossil_io_archive_t *archive, const char *entry_name)
{
    if (!archive || !entry_name)
        return false;
    if (!(archive->mode & (FOSSIL_IO_ARCHIVE_WRITE | FOSSIL_IO_ARCHIVE_APPEND)))
        return false;

    for (size_t i = 0; i < archive->entry_count; i++)
    {
        if (archive->entries[i].name && fossil_io_cstring_equals(archive->entries[i].name, entry_name))
        {
            fossil_io_cstring_free(archive->entries[i].name);

            // Shift remaining entries
            for (size_t j = i; j < archive->entry_count - 1; j++)
            {
                archive->entries[j] = archive->entries[j + 1];
            }
            archive->entry_count--;
            return true;
        }
    }
    return false;
}

// ======================================================
// Utility Functions
// ======================================================

void fossil_io_archive_print(fossil_io_archive_t *archive)
{
    if (!archive)
        return;

    fossil_io_printf("{cyan}Archive:{reset} %s {yellow}(Type: %d, Entries: %zu){reset}\n",
                     archive->path, archive->type, archive->entry_count);
    fossil_io_printf("{bold,blue}%-40s %10s %10s %s{reset}\n", "Name", "Size", "Compressed", "Directory");
    fossil_io_printf("{bold,blue}%-40s %10s %10s %s{reset}\n", "----", "----", "----------", "---------");

    for (size_t i = 0; i < archive->entry_count; i++)
    {
        fossil_io_archive_entry_t *entry = &archive->entries[i];
        fossil_io_printf("{white}%-40s{reset} {green}%10zu{reset} {magenta}%10zu{reset} {yellow}%s{reset}\n",
                         entry->name ? entry->name : "(null)",
                         entry->size,
                         entry->compressed_size,
                         entry->is_directory ? "Yes" : "No");
    }
}
