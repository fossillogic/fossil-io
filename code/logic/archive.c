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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h>

#ifdef _WIN32
    #include <windows.h>
    #include <io.h>
#else
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

struct fossil_io_archive {
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
// Utility: Duplicate string safely
// ======================================================
static char* fossil_io_archive_strdup(const char *s) {
    if (!s) return NULL;
    size_t len = strlen(s);
    char *copy = malloc(len + 1);
    if (copy) strcpy(copy, s);
    return copy;
}

// ======================================================
// Helper: Create directory recursively
// ======================================================
static bool fossil_io_create_directories(const char *path) {
    if (!path) return false;
    
    char *path_copy = fossil_io_archive_strdup(path);
    if (!path_copy) return false;
    
    char *p = path_copy;
    
    // Skip root slash on Unix or drive letter on Windows
    if (*p == '/') p++;
#ifdef _WIN32
    else if (strlen(p) >= 2 && p[1] == ':') p += 2;
    if (*p == '/' || *p == '\\') p++;
#endif
    
    while (*p) {
        while (*p && *p != '/' && *p != '\\') p++;
        if (*p) {
            *p = '\0';
#ifdef _WIN32
            CreateDirectoryA(path_copy, NULL);
#else
            mkdir(path_copy, 0755);
#endif
            *p = '/';
            p++;
        }
    }
    
    free(path_copy);
    return true;
}

// ======================================================
// Archive type detection
// ======================================================
fossil_io_archive_type_t fossil_io_archive_get_type(const char *path) {
    if (!path) return FOSSIL_IO_ARCHIVE_UNKNOWN;
    
    fossil_fstream_t stream;
    if (fossil_fstream_open(&stream, path, "rb") != 0) {
        return FOSSIL_IO_ARCHIVE_UNKNOWN;
    }
    
    unsigned char header[16];
    size_t read = fossil_fstream_read(&stream, header, 1, sizeof(header));
    fossil_fstream_close(&stream);
    
    if (read < 4) return FOSSIL_IO_ARCHIVE_UNKNOWN;
    
    // ZIP signature
    if (header[0] == 0x50 && header[1] == 0x4B) {
        return FOSSIL_IO_ARCHIVE_ZIP;
    }
    
    // TAR (check for ustar)
    if (read >= 8 && memcmp(header, "ustar", 5) == 0) {
        return FOSSIL_IO_ARCHIVE_TAR;
    }
    
    // GZIP
    if (header[0] == 0x1F && header[1] == 0x8B) {
        return FOSSIL_IO_ARCHIVE_GZ;
    }
    
    // BZIP2
    if (header[0] == 0x42 && header[1] == 0x5A && header[2] == 0x68) {
        return FOSSIL_IO_ARCHIVE_BZ2;
    }
    
    // 7Z
    if (read >= 6 && memcmp(header, "7z\xBC\xAF\x27\x1C", 6) == 0) {
        return FOSSIL_IO_ARCHIVE_7Z;
    }
    
    // RAR
    if (read >= 7 && memcmp(header, "Rar!\x1A\x07", 6) == 0) {
        return header[6] == 0x00 ? FOSSIL_IO_ARCHIVE_RAR : FOSSIL_IO_ARCHIVE_RAR5;
    }
    
    return FOSSIL_IO_ARCHIVE_UNKNOWN;
}

// ======================================================
// Initialization & Cleanup
// ======================================================

fossil_io_archive_t* fossil_io_archive_open(const char *path, fossil_io_archive_type_t type, fossil_io_archive_mode_t mode, fossil_io_archive_compression_t compression) {
    if (!path) return NULL;
    
    fossil_io_archive_t *archive = malloc(sizeof(fossil_io_archive_t));
    if (!archive) return NULL;
    
    archive->path = fossil_io_archive_strdup(path);
    archive->type = (type == FOSSIL_IO_ARCHIVE_UNKNOWN) ? fossil_io_archive_get_type(path) : type;
    archive->mode = mode;
    archive->compression = compression;
    archive->entries = NULL;
    archive->entry_count = 0;
    archive->entry_capacity = 0;
    
    if (!archive->path) {
        free(archive);
        return NULL;
    }
    
    return archive;
}

void fossil_io_archive_close(fossil_io_archive_t *archive) {
    if (!archive) return;
    
    free(archive->path);
    
    if (archive->entries) {
        for (size_t i = 0; i < archive->entry_count; i++) {
            free(archive->entries[i].name);
        }
        free(archive->entries);
    }
    
    free(archive);
}

fossil_io_archive_t* fossil_io_archive_create(const char *path, fossil_io_archive_type_t type, fossil_io_archive_compression_t compression) {
    if (!path || type == FOSSIL_IO_ARCHIVE_UNKNOWN) return NULL;
    
    // Create parent directories
    char *path_copy = fossil_io_archive_strdup(path);
    if (path_copy) {
        char *last_slash = strrchr(path_copy, '/');
        if (!last_slash) last_slash = strrchr(path_copy, '\\');
        if (last_slash) {
            *last_slash = '\0';
            fossil_io_create_directories(path_copy);
        }
        free(path_copy);
    }
    
    return fossil_io_archive_open(path, type, FOSSIL_IO_ARCHIVE_WRITE, compression);
}

// ======================================================
// Archive Statistics
// ======================================================

bool fossil_io_archive_get_stats(fossil_io_archive_t *archive, fossil_io_archive_stats_t *stats) {
    if (!archive || !stats) return false;
    
    stats->total_entries = archive->entry_count;
    stats->total_size = 0;
    stats->compressed_size = 0;
    
    for (size_t i = 0; i < archive->entry_count; i++) {
        stats->total_size += archive->entries[i].size;
        stats->compressed_size += archive->entries[i].compressed_size;
    }
    
    stats->compression_ratio = stats->total_size > 0 ? 
        (double)stats->compressed_size / stats->total_size : 0.0;
    
    return true;
}

// ======================================================
// Entry Management
// ======================================================

ssize_t fossil_io_archive_list(fossil_io_archive_t *archive, fossil_io_archive_entry_t **entries) {
    if (!archive || !entries) return -1;
    
    if (archive->entry_count == 0) {
        *entries = NULL;
        return 0;
    }
    
    fossil_io_archive_entry_t *entry_copy = malloc(sizeof(fossil_io_archive_entry_t) * archive->entry_count);
    if (!entry_copy) return -1;
    
    for (size_t i = 0; i < archive->entry_count; i++) {
        entry_copy[i] = archive->entries[i];
        entry_copy[i].name = fossil_io_archive_strdup(archive->entries[i].name);
        if (!entry_copy[i].name) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                free(entry_copy[j].name);
            }
            free(entry_copy);
            return -1;
        }
    }
    
    *entries = entry_copy;
    return archive->entry_count;
}

void fossil_io_archive_free_entries(fossil_io_archive_entry_t *entries, size_t count) {
    if (!entries) return;
    
    for (size_t i = 0; i < count; i++) {
        free(entries[i].name);
    }
    free(entries);
}

// ======================================================
// Entry Operations
// ======================================================

bool fossil_io_archive_exists(fossil_io_archive_t *archive, const char *entry_name) {
    if (!archive || !entry_name) return false;
    
    for (size_t i = 0; i < archive->entry_count; i++) {
        if (archive->entries[i].name && strcmp(archive->entries[i].name, entry_name) == 0) {
            return true;
        }
    }
    return false;
}

ssize_t fossil_io_archive_entry_size(fossil_io_archive_t *archive, const char *entry_name) {
    if (!archive || !entry_name) return -1;
    
    for (size_t i = 0; i < archive->entry_count; i++) {
        if (archive->entries[i].name && strcmp(archive->entries[i].name, entry_name) == 0) {
            return archive->entries[i].size;
        }
    }
    return -1;
}

// ======================================================
// File Operations (Simplified Implementation)
// ======================================================

bool fossil_io_archive_add_file(fossil_io_archive_t *archive, const char *src_path, const char *archive_path) {
    if (!archive || !src_path || !archive_path) return false;
    if (!(archive->mode & (FOSSIL_IO_ARCHIVE_WRITE | FOSSIL_IO_ARCHIVE_APPEND))) return false;
    
    fossil_fstream_t stream;
    if (fossil_fstream_open(&stream, src_path, "rb") != 0) {
        return false;
    }
    
    // Get file size
    if (fossil_fstream_seek(&stream, 0, SEEK_END) != 0) {
        fossil_fstream_close(&stream);
        return false;
    }
    
    int32_t file_size = fossil_fstream_tell(&stream);
    fossil_fstream_close(&stream);
    
    if (file_size < 0) return false;
    
    // Resize entries array if needed
    if (archive->entry_count >= archive->entry_capacity) {
        size_t new_capacity = archive->entry_capacity == 0 ? 8 : archive->entry_capacity * 2;
        fossil_io_archive_entry_t *new_entries = realloc(archive->entries, 
            sizeof(fossil_io_archive_entry_t) * new_capacity);
        if (!new_entries) return false;
        
        archive->entries = new_entries;
        archive->entry_capacity = new_capacity;
    }
    
    // Add entry
    fossil_io_archive_entry_t *entry = &archive->entries[archive->entry_count];
    memset(entry, 0, sizeof(fossil_io_archive_entry_t));
    
    entry->name = fossil_io_archive_strdup(archive_path);
    if (!entry->name) return false;
    
    entry->size = file_size;
    entry->compressed_size = file_size; // Simplified: no actual compression
    entry->is_directory = false;
    entry->is_encrypted = false;
    entry->modified_time = time(NULL);
    entry->created_time = time(NULL);
    entry->crc32 = 0; // Simplified: no CRC calculation
    entry->permissions = 0644;
    
    archive->entry_count++;
    return true;
}

bool fossil_io_archive_add_directory(fossil_io_archive_t *archive, const char *src_dir, const char *archive_dir) {
    if (!archive || !src_dir || !archive_dir) return false;
    
    // Simplified implementation: just add directory entry
    return fossil_io_archive_add_file(archive, src_dir, archive_dir);
}

bool fossil_io_archive_extract_file(fossil_io_archive_t *archive, const char *entry_name, const char *dest_path) {
    if (!archive || !entry_name || !dest_path) return false;
    if (!(archive->mode & FOSSIL_IO_ARCHIVE_READ)) return false;
    
    if (!fossil_io_archive_exists(archive, entry_name)) return false;
    
    // Create destination directory
    char *dest_copy = fossil_io_archive_strdup(dest_path);
    if (dest_copy) {
        char *last_slash = strrchr(dest_copy, '/');
        if (!last_slash) last_slash = strrchr(dest_copy, '\\');
        if (last_slash) {
            *last_slash = '\0';
            fossil_io_create_directories(dest_copy);
        }
        free(dest_copy);
    }
    
    // Simplified: create empty file
    FILE *file = fopen(dest_path, "wb");
    if (!file) return false;
    fclose(file);
    
    return true;
}

bool fossil_io_archive_extract_all(fossil_io_archive_t *archive, const char *dest_dir) {
    if (!archive || !dest_dir) return false;
    
    fossil_io_create_directories(dest_dir);
    
    for (size_t i = 0; i < archive->entry_count; i++) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dest_dir, archive->entries[i].name);
        
        if (!fossil_io_archive_extract_file(archive, archive->entries[i].name, full_path)) {
            return false;
        }
    }
    
    return true;
}

bool fossil_io_archive_remove(fossil_io_archive_t *archive, const char *entry_name) {
    if (!archive || !entry_name) return false;
    if (!(archive->mode & (FOSSIL_IO_ARCHIVE_WRITE | FOSSIL_IO_ARCHIVE_APPEND))) return false;
    
    for (size_t i = 0; i < archive->entry_count; i++) {
        if (archive->entries[i].name && strcmp(archive->entries[i].name, entry_name) == 0) {
            free(archive->entries[i].name);
            
            // Shift remaining entries
            for (size_t j = i; j < archive->entry_count - 1; j++) {
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

void fossil_io_archive_print(fossil_io_archive_t *archive) {
    if (!archive) return;
    
    printf("Archive: %s (Type: %d, Entries: %zu)\n", 
           archive->path, archive->type, archive->entry_count);
    printf("%-40s %10s %10s %s\n", "Name", "Size", "Compressed", "Directory");
    printf("%-40s %10s %10s %s\n", "----", "----", "----------", "---------");
    
    for (size_t i = 0; i < archive->entry_count; i++) {
        fossil_io_archive_entry_t *entry = &archive->entries[i];
        printf("%-40s %10zu %10zu %s\n", 
               entry->name ? entry->name : "(null)",
               entry->size,
               entry->compressed_size,
               entry->is_directory ? "Yes" : "No");
    }
}
