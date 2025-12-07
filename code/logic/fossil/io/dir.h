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
 * @brief Represents a directory entry with extended metadata.
 */
typedef struct {
    char name[512];             ///< Entry name (basename only)
    char path[1024];            ///< Full resolved path

    int32_t type;               ///< 0=file, 1=dir, 2=symlink, 3=pipe, 4=sock, 5=blockdev, 6=chardev, 7=other

    uint64_t size;              ///< File size in bytes (0 for dirs)
    uint64_t allocated_size;    ///< Allocated size on disk (blocks * blocksize)

    uint64_t modified;          ///< Last modified timestamp (UTC epoch)
    uint64_t accessed;          ///< Last access timestamp
    uint64_t changed;           ///< Metadata change timestamp
    uint64_t created;           ///< Creation timestamp (UTC epoch; 0 if unknown)

    int32_t permissions;        ///< Platform-specific permissions/mode bits
    int32_t owner_uid;          ///< Owner user ID (POSIX), -1 if not available
    int32_t owner_gid;          ///< Owner group ID (POSIX), -1 if not available

    int32_t is_hidden;          ///< 1=hidden, 0=not hidden
    int32_t is_readonly;        ///< 1=readonly, 0=not readonly
    int32_t is_system;          ///< Windows system attribute (1=yes), otherwise 0
    int32_t is_executable;      ///< Executable bit inferred from mode or extension

    uint32_t hash_crc32;        ///< Optional: CRC32 of file (0 if skipped)
    uint64_t hash_murmur64;     ///< Optional: Murmur64 hash
    uint8_t hash_sha1[20];      ///< Optional: SHA-1 hash (zeroed if unused)

    uint8_t attributes;         ///< Portable attribute bitfield (cross-platform)
    /*
        Bit 0 = Hidden
        Bit 1 = Readonly
        Bit 2 = Executable
        Bit 3 = Symlink
        Bit 4 = System
        Bit 5 = Archive (Win)
        Bit 6 = Compressed
        Bit 7 = Encrypted
    */

    void *platform_data;        ///< OS-specific data (FILE_ATTRIBUTE_xxx or struct stat)
} fossil_io_dir_entry_t;


// ============================================================================
// Directory Iterator
// ============================================================================

/**
 * @brief Directory iterator with support for filtering, sorting, and recursion.
 */
typedef struct {
    void *handle;               ///< Platform-specific DIR* / HANDLE / etc.
    char basepath[1024];        ///< The directory being iterated

    fossil_io_dir_entry_t current; ///< Current entry metadata

    int32_t active;             ///< 1=active, 0=end or closed
    size_t index;               ///< Current entry index
    size_t total;               ///< Total entries known (0=unknown)

    int32_t recursive;          ///< 1=recursive, 0=flat
    int32_t follow_symlinks;    ///< 1=follow symlinks, 0=ignore

    int32_t include_hidden;     ///< 1=list hidden files, 0=skip
    int32_t include_system;     ///< 1=list system files (Windows), 0=skip

    int32_t sort_mode;          ///< 0=none, 1=name, 2=size, 3=date, 4=type
    int32_t sort_descending;    ///< 1=reverse order

    const char *filter_glob;    ///< Optional: "*.c", "*.txt", etc. (NULL=none)
    const char *filter_regex;   ///< Optional regex filtering (NULL=none)

    void *platform_data;        ///< Extra platform-specific state (WIN32_FIND_DATAA, etc.)
} fossil_io_dir_iter_t;

// ============================================================================
// Basic Directory Operations
// ============================================================================

/**
 * @brief Checks if a directory exists at the given path.
 * @param path The directory path to check.
 * @return 1 if exists, 0 if not, negative on error.
 */
int32_t fossil_io_dir_exists(const char *path);

/**
 * @brief Creates a directory at the specified path.
 * @param path The directory path to create.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_create(const char *path);

/**
 * @brief Removes the directory at the specified path.
 * @param path The directory path to remove.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_remove(const char *path);

/**
 * @brief Recursively removes a directory and all its contents.
 * @param path The directory path to remove.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_remove_recursive(const char *path);

/**
 * @brief Clears all contents of a directory, but does not remove the directory itself.
 * @param path The directory path to clear.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_clear(const char *path);

/**
 * @brief Copies the contents of one directory to another.
 * @param src Source directory path.
 * @param dst Destination directory path.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_copy(const char *src, const char *dst);

/**
 * @brief Recursively copies the contents of one directory to another.
 * @param src Source directory path.
 * @param dst Destination directory path.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_copy_recursive(const char *src, const char *dst);

/**
 * @brief Moves a directory from source to destination.
 * @param src Source directory path.
 * @param dst Destination directory path.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_move(const char *src, const char *dst);

/**
 * @brief Renames a directory from oldpath to newpath.
 * @param oldpath The current directory path.
 * @param newpath The new directory path.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_rename(const char *oldpath, const char *newpath);


// ============================================================================
// Directory Iteration API
// ============================================================================

/**
 * @brief Opens a directory iterator for the specified path.
 * @param it Pointer to the iterator structure.
 * @param path Directory path to iterate.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_iter_open(fossil_io_dir_iter_t *it, const char *path);

/**
 * @brief Advances the iterator to the next directory entry.
 * @param it Pointer to the iterator structure.
 * @return 1 if valid entry, 0 if end of directory, negative on error.
 */
int32_t fossil_io_dir_iter_next(fossil_io_dir_iter_t *it);

/**
 * @brief Closes the directory iterator and releases resources.
 * @param it Pointer to the iterator structure.
 */
void fossil_io_dir_iter_close(fossil_io_dir_iter_t *it);

/**
 * @brief Lists directory entries in the specified path.
 * @param path Directory path to list.
 * @param entries Array to store directory entries.
 * @param count Pointer to store number of entries found.
 * @param max_entries Maximum number of entries to list.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_list(const char *path,
                           fossil_io_dir_entry_t *entries,
                           size_t *count,
                           size_t max_entries);


// ============================================================================
// Path Utilities
// ============================================================================

/**
 * @brief Checks if the given path is absolute.
 * @param path Path to check.
 * @return 1 if absolute, 0 if relative, negative on error.
 */
int32_t fossil_io_dir_is_absolute(const char *path);

/**
 * @brief Joins a base path and a name into a single path.
 * @param base Base directory path.
 * @param name Name to append.
 * @param out Output buffer for joined path.
 * @param outsz Size of output buffer.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_join(const char *base, const char *name, char *out, size_t outsz);

/**
 * @brief Extracts the basename (final component) from a path.
 * @param path Input path.
 * @param out Output buffer for basename.
 * @param outsz Size of output buffer.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_basename(const char *path, char *out, size_t outsz);

/**
 * @brief Extracts the directory name (parent path) from a path.
 * @param path Input path.
 * @param out Output buffer for dirname.
 * @param outsz Size of output buffer.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_dirname(const char *path, char *out, size_t outsz);

/**
 * @brief Normalizes a path, resolving redundant separators and up-level references.
 * @param path Input path.
 * @param out Output buffer for normalized path.
 * @param outsz Size of output buffer.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_normalize(const char *path, char *out, size_t outsz);

/**
 * @brief Resolves a path to its absolute, canonical form.
 * @param path Input path.
 * @param out Output buffer for real path.
 * @param outsz Size of output buffer.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_realpath(const char *path, char *out, size_t outsz);


// ============================================================================
// Directory Inspection Queries
// ============================================================================

/**
 * @brief Checks if a directory is empty.
 * @param path Directory path to check.
 * @return 1 if empty, 0 if not, negative on error.
 */
int32_t fossil_io_dir_is_empty(const char *path);

/**
 * @brief Counts the number of entries in a directory.
 * @param path Directory path to count.
 * @param count Pointer to store entry count.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_count(const char *path, size_t *count);

/**
 * @brief Calculates the total size of a directory (recursive sum).
 * @param path Directory path to calculate.
 * @param bytes Pointer to store total size in bytes.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_size(const char *path, uint64_t *bytes);


// ============================================================================
// Filtering + Scanning
// ============================================================================

/**
 * @brief Callback used for scanning files.
 * Return 1 to continue, 0 to stop traversal.
 * @param entry Pointer to the directory entry.
 * @param userdata User-defined data passed to the callback.
 * @return 1 to continue, 0 to stop traversal.
 */
typedef int32_t (*fossil_io_dir_scan_callback)(
    const fossil_io_dir_entry_t *entry,
    void *userdata);

/**
 * @brief Scans directory entries, invoking a callback for each entry.
 * @param path Directory path to scan.
 * @param cb Callback function to invoke.
 * @param userdata User-defined data passed to callback.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_scan(const char *path, fossil_io_dir_scan_callback cb, void *userdata);

/**
 * @brief Recursively scans directory entries, invoking a callback for each entry.
 * @param path Directory path to scan.
 * @param cb Callback function to invoke.
 * @param userdata User-defined data passed to callback.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_scan_recursive(const char *path, fossil_io_dir_scan_callback cb, void *userdata);


// ============================================================================
// Permissions (platform supported)
// ============================================================================

/**
 * @brief Sets permissions for a directory.
 * @param path Directory path.
 * @param mode Permission mode to set.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_set_permissions(const char *path, int32_t mode);

/**
 * @brief Gets permissions for a directory.
 * @param path Directory path.
 * @param mode Pointer to store permission mode.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_get_permissions(const char *path, int32_t *mode);


// ============================================================================
// Temp Directory Utilities
// ============================================================================

/**
 * @brief Gets the system temporary directory path.
 * @param out Output buffer for temp path.
 * @param outsz Size of output buffer.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_temp(char *out, size_t outsz);

/**
 * @brief Creates a unique temporary directory.
 * @param out Output buffer for temp directory path.
 * @param outsz Size of output buffer.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_create_temp(char *out, size_t outsz);


// ============================================================================
// Metadata Queries
// ============================================================================

/**
 * @brief Gets the last modified timestamp of a directory.
 * @param path Directory path.
 * @param timestamp Pointer to store modified time (UTC epoch).
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_get_modified(const char *path, uint64_t *timestamp);

/**
 * @brief Gets the creation timestamp of a directory.
 * @param path Directory path.
 * @param timestamp Pointer to store creation time (UTC epoch).
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_get_created(const char *path, uint64_t *timestamp);


// ============================================================================
// Directory Sync / Backup
// ============================================================================

/**
 * @brief Creates a backup of a directory with the given suffix.
 * @param src Source directory path.
 * @param suffix Suffix to append to backup directory.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_backup(const char *src, const char *suffix);

/**
 * @brief Mirrors the contents of one directory to another.
 * @param src Source directory path.
 * @param dst Destination directory path.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_mirror(const char *src, const char *dst);

/**
 * @brief Synchronizes two directories, optionally deleting extraneous files.
 * @param src Source directory path.
 * @param dst Destination directory path.
 * @param delete_extraneous If non-zero, delete files in dst not in src.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_sync(const char *src, const char *dst, int32_t delete_extraneous);


// ============================================================================
// Low-Level Helpers
// ============================================================================

/**
 * @brief Checks if the path refers to a directory.
 * @param path Path to check.
 * @return 1 if directory, 0 if not, negative on error.
 */
int32_t fossil_io_dir_is_directory(const char *path);

/**
 * @brief Checks if the path refers to a regular file.
 * @param path Path to check.
 * @return 1 if file, 0 if not, negative on error.
 */
int32_t fossil_io_dir_is_file(const char *path);

/**
 * @brief Checks if the path refers to a symbolic link.
 * @param path Path to check.
 * @return 1 if symlink, 0 if not, negative on error.
 */
int32_t fossil_io_dir_is_symlink(const char *path);

// ------------------------------------------------------------
// Link and Symlink
// ------------------------------------------------------------

/**
 * @brief Creates a hard link at linkpath to the target file.
 * @param target The target file path.
 * @param linkpath The path where the hard link will be created.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_link(const char *target, const char *linkpath);

/**
 * @brief Creates a symbolic link at linkpath to the target file or directory.
 * @param target The target file or directory path.
 * @param linkpath The path where the symbolic link will be created.
 * @return 0 on success, negative on error.
 */
int32_t fossil_io_dir_symlink(const char *target, const char *linkpath);

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

        /**
         * @brief C++ wrapper for directory operations.
         * 
         * The Dir class provides static methods that wrap the C directory API
         * functions, allowing convenient usage from C++ code. It covers basic
         * directory management (existence, creation, removal), recursive operations,
         * iteration, listing, path utilities, inspection queries, filtering/scanning,
         * permissions, temporary directory utilities, metadata queries, sync/backup,
         * and low-level helpers for file type checks.
         * 
         * All methods are static and forward their arguments to the corresponding
         * C functions, converting std::string to const char* as needed.
         */
        class Dir {
        public:
            /**
             * @brief Checks if a directory exists at the given path.
             * @param path Directory path to check.
             * @return 1 if exists, 0 if not, negative on error.
             */
            static int32_t exists(const std::string& path) {
            return fossil_io_dir_exists(path.c_str());
            }

            /**
             * @brief Creates a directory at the specified path.
             * @param path Directory path to create.
             * @return 0 on success, negative on error.
             */
            static int32_t create(const std::string& path) {
            return fossil_io_dir_create(path.c_str());
            }

            /**
             * @brief Removes the directory at the specified path.
             * @param path Directory path to remove.
             * @return 0 on success, negative on error.
             */
            static int32_t remove(const std::string& path) {
            return fossil_io_dir_remove(path.c_str());
            }

            /**
             * @brief Recursively removes a directory and all its contents.
             * @param path Directory path to remove.
             * @return 0 on success, negative on error.
             */
            static int32_t remove_recursive(const std::string& path) {
            return fossil_io_dir_remove_recursive(path.c_str());
            }

            /**
             * @brief Clears all contents of a directory, but does not remove the directory itself.
             * @param path Directory path to clear.
             * @return 0 on success, negative on error.
             */
            static int32_t clear(const std::string& path) {
            return fossil_io_dir_clear(path.c_str());
            }

            /**
             * @brief Copies the contents of one directory to another.
             * @param src Source directory path.
             * @param dst Destination directory path.
             * @return 0 on success, negative on error.
             */
            static int32_t copy(const std::string& src, const std::string& dst) {
            return fossil_io_dir_copy(src.c_str(), dst.c_str());
            }

            /**
             * @brief Recursively copies the contents of one directory to another.
             * @param src Source directory path.
             * @param dst Destination directory path.
             * @return 0 on success, negative on error.
             */
            static int32_t copy_recursive(const std::string& src, const std::string& dst) {
            return fossil_io_dir_copy_recursive(src.c_str(), dst.c_str());
            }

            /**
             * @brief Moves a directory from source to destination.
             * @param src Source directory path.
             * @param dst Destination directory path.
             * @return 0 on success, negative on error.
             */
            static int32_t move(const std::string& src, const std::string& dst) {
            return fossil_io_dir_move(src.c_str(), dst.c_str());
            }

            /**
             * @brief Renames a directory from oldpath to newpath.
             * @param oldpath Current directory path.
             * @param newpath New directory path.
             * @return 0 on success, negative on error.
             */
            static int32_t rename(const std::string& oldpath, const std::string& newpath) {
            return fossil_io_dir_rename(oldpath.c_str(), newpath.c_str());
            }

            /**
             * @brief Opens a directory iterator for the specified path.
             * @param it Pointer to iterator structure.
             * @param path Directory path to iterate.
             * @return 0 on success, negative on error.
             */
            static int32_t iter_open(fossil_io_dir_iter_t* it, const std::string& path) {
            return fossil_io_dir_iter_open(it, path.c_str());
            }

            /**
             * @brief Advances the iterator to the next directory entry.
             * @param it Pointer to iterator structure.
             * @return 1 if valid entry, 0 if end, negative on error.
             */
            static int32_t iter_next(fossil_io_dir_iter_t* it) {
            return fossil_io_dir_iter_next(it);
            }

            /**
             * @brief Closes the directory iterator and releases resources.
             * @param it Pointer to iterator structure.
             */
            static void iter_close(fossil_io_dir_iter_t* it) {
            fossil_io_dir_iter_close(it);
            }

            /**
             * @brief Lists directory entries in the specified path.
             * @param path Directory path to list.
             * @param entries Array to store directory entries.
             * @param count Pointer to store number of entries found.
             * @param max_entries Maximum number of entries to list.
             * @return 0 on success, negative on error.
             */
            static int32_t list(const std::string& path,
                    fossil_io_dir_entry_t* entries,
                    size_t* count,
                    size_t max_entries) {
            return fossil_io_dir_list(path.c_str(), entries, count, max_entries);
            }

            /**
             * @brief Checks if the given path is absolute.
             * @param path Path to check.
             * @return 1 if absolute, 0 if relative, negative on error.
             */
            static int32_t is_absolute(const std::string& path) {
            return fossil_io_dir_is_absolute(path.c_str());
            }

            /**
             * @brief Joins a base path and a name into a single path.
             * @param base Base directory path.
             * @param name Name to append.
             * @param out Output buffer for joined path.
             * @param outsz Size of output buffer.
             * @return 0 on success, negative on error.
             */
            static int32_t join(const std::string& base, const std::string& name, char* out, size_t outsz) {
            return fossil_io_dir_join(base.c_str(), name.c_str(), out, outsz);
            }

            /**
             * @brief Extracts the basename (final component) from a path.
             * @param path Input path.
             * @param out Output buffer for basename.
             * @param outsz Size of output buffer.
             * @return 0 on success, negative on error.
             */
            static int32_t basename(const std::string& path, char* out, size_t outsz) {
            return fossil_io_dir_basename(path.c_str(), out, outsz);
            }

            /**
             * @brief Extracts the directory name (parent path) from a path.
             * @param path Input path.
             * @param out Output buffer for dirname.
             * @param outsz Size of output buffer.
             * @return 0 on success, negative on error.
             */
            static int32_t dirname(const std::string& path, char* out, size_t outsz) {
            return fossil_io_dir_dirname(path.c_str(), out, outsz);
            }

            /**
             * @brief Normalizes a path, resolving redundant separators and up-level references.
             * @param path Input path.
             * @param out Output buffer for normalized path.
             * @param outsz Size of output buffer.
             * @return 0 on success, negative on error.
             */
            static int32_t normalize(const std::string& path, char* out, size_t outsz) {
            return fossil_io_dir_normalize(path.c_str(), out, outsz);
            }

            /**
             * @brief Resolves a path to its absolute, canonical form.
             * @param path Input path.
             * @param out Output buffer for real path.
             * @param outsz Size of output buffer.
             * @return 0 on success, negative on error.
             */
            static int32_t realpath(const std::string& path, char* out, size_t outsz) {
            return fossil_io_dir_realpath(path.c_str(), out, outsz);
            }

            /**
             * @brief Checks if a directory is empty.
             * @param path Directory path to check.
             * @return 1 if empty, 0 if not, negative on error.
             */
            static int32_t is_empty(const std::string& path) {
            return fossil_io_dir_is_empty(path.c_str());
            }

            /**
             * @brief Counts the number of entries in a directory.
             * @param path Directory path to count.
             * @param count Pointer to store entry count.
             * @return 0 on success, negative on error.
             */
            static int32_t count(const std::string& path, size_t* count) {
            return fossil_io_dir_count(path.c_str(), count);
            }

            /**
             * @brief Calculates the total size of a directory (recursive sum).
             * @param path Directory path to calculate.
             * @param bytes Pointer to store total size in bytes.
             * @return 0 on success, negative on error.
             */
            static int32_t size(const std::string& path, uint64_t* bytes) {
            return fossil_io_dir_size(path.c_str(), bytes);
            }

            /**
             * @brief Scans directory entries, invoking a callback for each entry.
             * @param path Directory path to scan.
             * @param cb Callback function to invoke.
             * @param userdata User-defined data passed to callback.
             * @return 0 on success, negative on error.
             */
            static int32_t scan(const std::string& path, fossil_io_dir_scan_callback cb, void* userdata) {
            return fossil_io_dir_scan(path.c_str(), cb, userdata);
            }

            /**
             * @brief Recursively scans directory entries, invoking a callback for each entry.
             * @param path Directory path to scan.
             * @param cb Callback function to invoke.
             * @param userdata User-defined data passed to callback.
             * @return 0 on success, negative on error.
             */
            static int32_t scan_recursive(const std::string& path, fossil_io_dir_scan_callback cb, void* userdata) {
            return fossil_io_dir_scan_recursive(path.c_str(), cb, userdata);
            }

            /**
             * @brief Sets permissions for a directory.
             * @param path Directory path.
             * @param mode Permission mode to set.
             * @return 0 on success, negative on error.
             */
            static int32_t set_permissions(const std::string& path, int32_t mode) {
            return fossil_io_dir_set_permissions(path.c_str(), mode);
            }

            /**
             * @brief Gets permissions for a directory.
             * @param path Directory path.
             * @param mode Pointer to store permission mode.
             * @return 0 on success, negative on error.
             */
            static int32_t get_permissions(const std::string& path, int32_t* mode) {
            return fossil_io_dir_get_permissions(path.c_str(), mode);
            }

            /**
             * @brief Gets the system temporary directory path.
             * @param out Output buffer for temp path.
             * @param outsz Size of output buffer.
             * @return 0 on success, negative on error.
             */
            static int32_t temp(char* out, size_t outsz) {
            return fossil_io_dir_temp(out, outsz);
            }

            /**
             * @brief Creates a unique temporary directory.
             * @param out Output buffer for temp directory path.
             * @param outsz Size of output buffer.
             * @return 0 on success, negative on error.
             */
            static int32_t create_temp(char* out, size_t outsz) {
            return fossil_io_dir_create_temp(out, outsz);
            }

            /**
             * @brief Gets the last modified timestamp of a directory.
             * @param path Directory path.
             * @param timestamp Pointer to store modified time (UTC epoch).
             * @return 0 on success, negative on error.
             */
            static int32_t get_modified(const std::string& path, uint64_t* timestamp) {
            return fossil_io_dir_get_modified(path.c_str(), timestamp);
            }

            /**
             * @brief Gets the creation timestamp of a directory.
             * @param path Directory path.
             * @param timestamp Pointer to store creation time (UTC epoch).
             * @return 0 on success, negative on error.
             */
            static int32_t get_created(const std::string& path, uint64_t* timestamp) {
            return fossil_io_dir_get_created(path.c_str(), timestamp);
            }

            /**
             * @brief Creates a backup of a directory with the given suffix.
             * @param src Source directory path.
             * @param suffix Suffix to append to backup directory.
             * @return 0 on success, negative on error.
             */
            static int32_t backup(const std::string& src, const std::string& suffix) {
            return fossil_io_dir_backup(src.c_str(), suffix.c_str());
            }

            /**
             * @brief Mirrors the contents of one directory to another.
             * @param src Source directory path.
             * @param dst Destination directory path.
             * @return 0 on success, negative on error.
             */
            static int32_t mirror(const std::string& src, const std::string& dst) {
            return fossil_io_dir_mirror(src.c_str(), dst.c_str());
            }

            /**
             * @brief Synchronizes two directories, optionally deleting extraneous files.
             * @param src Source directory path.
             * @param dst Destination directory path.
             * @param delete_extraneous If non-zero, delete files in dst not in src.
             * @return 0 on success, negative on error.
             */
            static int32_t sync(const std::string& src, const std::string& dst, int32_t delete_extraneous) {
            return fossil_io_dir_sync(src.c_str(), dst.c_str(), delete_extraneous);
            }

            /**
             * @brief Checks if the path refers to a directory.
             * @param path Path to check.
             * @return 1 if directory, 0 if not, negative on error.
             */
            static int32_t is_directory(const std::string& path) {
            return fossil_io_dir_is_directory(path.c_str());
            }

            /**
             * @brief Checks if the path refers to a regular file.
             * @param path Path to check.
             * @return 1 if file, 0 if not, negative on error.
             */
            static int32_t is_file(const std::string& path) {
            return fossil_io_dir_is_file(path.c_str());
            }

            /**
             * @brief Checks if the path refers to a symbolic link.
             * @param path Path to check.
             * @return 1 if symlink, 0 if not, negative on error.
             */
            static int32_t is_symlink(const std::string& path) {
            return fossil_io_dir_is_symlink(path.c_str());
            }

            // ------------------------------------------------------------
            // Link and Symlink
            // ------------------------------------------------------------

            /**
             * @brief Creates a hard link at linkpath to the target file.
             * @param target The target file path.
             * @param linkpath The path where the hard link will be created.
             * @return 0 on success, negative on error.
             */
            static int32_t link(const std::string& target, const std::string& linkpath) {
                return fossil_io_dir_link(target.c_str(), linkpath.c_str());
            }

            /**
             * @brief Creates a symbolic link at linkpath to the target file or directory.
             * @param target The target file or directory path.
             * @param linkpath The path where the symbolic link will be created.
             * @return 0 on success, negative on error.
             */
            static int32_t symlink(const std::string& target, const std::string& linkpath) {
                return fossil_io_dir_symlink(target.c_str(), linkpath.c_str());
            }

        };

    }

}

#endif /* __cplusplus */

#endif /* FOSSIL_IO_DIR_H */
