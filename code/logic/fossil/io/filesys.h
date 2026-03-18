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
#ifndef FOSSIL_IO_FILESYS_H
#define FOSSIL_IO_FILESYS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* ------------------------------------------------------------
    * Core Definitions
    * ------------------------------------------------------------ */

#define FOSSIL_FILESYS_MAX_PATH 512
#define FOSSIL_FILESYS_MAX_ID 128
#define FOSSIL_FILESYS_MAX_TAGS 16

/* ------------------------------------------------------------
    * Filesystem Object Types
    * ------------------------------------------------------------ */

typedef enum
{
    FOSSIL_FILESYS_TYPE_UNKNOWN = 0,
    FOSSIL_FILESYS_TYPE_FILE,
    FOSSIL_FILESYS_TYPE_DIR,
    FOSSIL_FILESYS_TYPE_LINK
} fossil_io_filesys_type_t;

/* ------------------------------------------------------------
    * Permissions (cross-platform abstraction)
    * ------------------------------------------------------------ */

typedef struct
{
    bool read;
    bool write;
    bool execute;
} fossil_io_filesys_perms_t;

/* ------------------------------------------------------------
    * Thread / Locking Support
    * ------------------------------------------------------------ */

typedef struct
{
    void *handle; /* platform-specific mutex / lock */
    bool locked;
} fossil_io_filesys_lock_t;

/* ------------------------------------------------------------
    * Base Filesystem Object (shared by file/dir/link)
    * ------------------------------------------------------------ */

/**
 * @brief Base structure representing any filesystem object.
 *
 * This structure provides a unified abstraction over files, directories,
 * and symbolic/hard links. It is designed for cross-platform compatibility,
 * high-performance operations, and optional AI-driven metadata processing.
 *
 * Members:
 *  - char id[]: Unique string ID for referencing object (stable across operations).
 *  - char path[]: Full path to the object.
 *  - fossil_io_filesys_type_t type: Object type (file, dir, link).
 *  - fossil_io_filesys_perms_t perms: Access permissions.
 *  - uint32_t mode: Raw platform permission bits.
 *  - char owner[], group[]: Ownership metadata.
 *  - size_t size: Size (files) or entry count hint (dirs).
 *  - timestamps: created, modified, accessed.
 *  - lock: Thread-safe locking primitive.
 *  - flags: internal state flags.
 *
 * AI Fields:
 *  - indexed / analyzed
 *  - tags / embeddings
 */
typedef struct
{
    char id[FOSSIL_FILESYS_MAX_ID];
    char path[FOSSIL_FILESYS_MAX_PATH];

    fossil_io_filesys_type_t type;

    fossil_io_filesys_perms_t perms;
    uint32_t mode;

    char owner[64];
    char group[64];

    size_t size;

    time_t created_at;
    time_t modified_at;
    time_t accessed_at;

    fossil_io_filesys_lock_t lock;

    uint32_t flags;

} fossil_io_filesys_obj_t;

/* ------------------------------------------------------------
    * File Object
    * ------------------------------------------------------------ */

typedef struct
{
    fossil_io_filesys_obj_t base;

    void *handle; /* FILE* or platform handle */
    int fd;

    bool is_open;
    bool append_mode;
    bool temporary;

    size_t position;

    void *buffer;
    size_t buffer_size;

} fossil_io_filesys_file_t;

/* ------------------------------------------------------------
    * Directory Object
    * ------------------------------------------------------------ */

typedef struct
{
    fossil_io_filesys_obj_t base;

    size_t entry_count;

    bool recursive;
    bool mounted;

} fossil_io_filesys_dir_t;

/* ------------------------------------------------------------
    * Link Object
    * ------------------------------------------------------------ */

typedef struct
{
    fossil_io_filesys_obj_t base;

    char target[FOSSIL_FILESYS_MAX_PATH];

    bool symbolic;

} fossil_io_filesys_link_t;

/* ------------------------------------------------------------
    * General Filesystem Operations
    * ------------------------------------------------------------ */

/**
 * Initialize or refresh a filesystem object from a given path.
 *
 * Populates metadata (type, size, permissions, timestamps).
 *
 * @param obj Pointer to filesystem object
 * @param path Path to object
 * @return 0 on success, negative on failure
 */
int32_t fossil_io_filesys_init(fossil_io_filesys_obj_t *obj, const char *path);
int32_t fossil_io_filesys_refresh(fossil_io_filesys_obj_t *obj);

/**
 * Check if a filesystem object exists.
 *
 * @param path Path to check
 * @return 1 if exists, 0 if not, negative on error
 */
int32_t fossil_io_filesys_exists(const char *path);

/**
 * Remove a filesystem object (file, dir, link).
 *
 * @param path Path to remove
 * @param recursive Recursively remove contents if directory
 * @return 0 on success, negative on failure
 */
int32_t fossil_io_filesys_remove(const char *path, bool recursive);

/**
 * Rename or move a filesystem object.
 *
 * Handles cross-directory moves, atomic if possible.
 *
 * @param src Source path
 * @param dest Destination path
 * @return 0 on success, negative on failure
 */
int32_t fossil_io_filesys_move(const char *src, const char *dest);

/**
 * Copy a filesystem object.
 *
 * Copies files or directories. Can preserve metadata.
 *
 * @param src Source path
 * @param dest Destination path
 * @param preserve_meta Preserve permissions, timestamps
 * @return 0 on success, negative on failure
 */
int32_t fossil_io_filesys_copy(const char *src, const char *dest, bool preserve_meta);

/**
 * Atomic swap of two filesystem objects.
 *
 * @param path1 First path
 * @param path2 Second path
 * @return 0 on success, negative on failure
 */
int32_t fossil_io_filesys_swap(const char *path1, const char *path2);

/**
 * Deduplicate files in a directory (optional recursive).
 *
 * @param path Directory path
 * @param recursive Scan subdirectories
 * @return Number of duplicates removed, negative on failure
 */
int32_t fossil_io_filesys_deduplicate(const char *path, bool recursive);

/**
 * Get filesystem object info (type, size, permissions, timestamps).
 *
 * @param path Path to object
 * @param obj Pointer to store metadata
 * @return 0 on success, negative on failure
 */
int32_t fossil_io_filesys_stat(const char *path, fossil_io_filesys_obj_t *obj);

/* ------------------------------------------------------------
    * File-Specific Operations
    * ------------------------------------------------------------ */

/**
 * @brief Open a file and initialize a file object.
 *
 * Opens a file at the specified path with the given access mode and initializes
 * the provided file object structure. The file object will contain handle, position,
 * and other metadata needed for subsequent file operations.
 *
 * @param f Pointer to the file object to initialize
 * @param path Path to the file to open
 * @param mode File access mode (e.g., "r", "w", "a", "rb", "wb")
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_file_open(fossil_io_filesys_file_t *f, const char *path, const char *mode);

/**
 * @brief Close an open file and release resources.
 *
 * Closes the file associated with the given file object, flushes any buffered data,
 * and releases platform-specific file handles and resources. The file object should
 * not be used after this call without reopening.
 *
 * @param f Pointer to the file object to close
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_file_close(fossil_io_filesys_file_t *f);

/**
 * @brief Read data from an open file.
 *
 * Reads up to count elements of size bytes from the current file position into the
 * provided buffer. Advances the file position by the number of bytes read.
 *
 * @param f Pointer to the open file object
 * @param buf Pointer to buffer where data will be read
 * @param size Size of each element in bytes
 * @param count Number of elements to read
 * @return Number of elements successfully read, 0 on EOF or error
 */
size_t fossil_io_filesys_file_read(fossil_io_filesys_file_t *f, void *buf, size_t size, size_t count);

/**
 * @brief Write data to an open file.
 *
 * Writes count elements of size bytes from the provided buffer to the file at the
 * current file position. May be buffered depending on the file object's buffer settings.
 *
 * @param f Pointer to the open file object
 * @param buf Pointer to buffer containing data to write
 * @param size Size of each element in bytes
 * @param count Number of elements to write
 * @return Number of elements successfully written, 0 on error
 */
size_t fossil_io_filesys_file_write(fossil_io_filesys_file_t *f, const void *buf, size_t size, size_t count);

/**
 * @brief Seek to a specific position in an open file.
 *
 * Changes the file position indicator to the specified offset relative to the given
 * origin point (beginning, current position, or end of file).
 *
 * @param f Pointer to the open file object
 * @param offset Byte offset from the origin
 * @param origin Reference point for offset (SEEK_SET, SEEK_CUR, SEEK_END)
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_file_seek(fossil_io_filesys_file_t *f, int64_t offset, int32_t origin);

/**
 * @brief Get the current position in an open file.
 *
 * Returns the current byte offset of the file position indicator from the start
 * of the file.
 *
 * @param f Pointer to the open file object
 * @return Current file position in bytes, or negative error code on failure
 */
int64_t fossil_io_filesys_file_tell(fossil_io_filesys_file_t *f);

/**
 * @brief Flush buffered data to the file.
 *
 * Forces any pending buffered write operations on the file to be written to the
 * underlying storage medium, ensuring data durability.
 *
 * @param f Pointer to the open file object
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_file_flush(fossil_io_filesys_file_t *f);

/**
 * @brief Get the size of a file.
 *
 * Returns the total size in bytes of the file at the specified path without
 * requiring the file to be opened.
 *
 * @param path Path to the file
 * @return File size in bytes, or negative error code on failure
 */
int32_t fossil_io_filesys_file_size(const char *path);

/**
 * @brief Truncate or extend a file to a specific size.
 *
 * Resizes the file at the specified path to the given size. If the file is larger,
 * it will be truncated. If smaller, it will be extended (typically with null bytes).
 *
 * @param path Path to the file
 * @param size New size in bytes
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_file_truncate(const char *path, size_t size);

/**
 * @brief Compute the cryptographic hash of a file.
 *
 * Reads the entire contents of a file at the given path and computes its hash
 * using the specified algorithm. The resulting hash is stored in the provided
 * output buffer. Supported algorithms include "sha256", "md5", "sha1", etc.
 *
 * @param path Path to the file to hash
 * @param hash_out Pointer to buffer where computed hash will be stored
 * @param hash_size Size of the hash_out buffer in bytes
 * @param algorithm Name of the hashing algorithm to use (e.g., "sha256")
 * @return 0 on success, negative error code on failure (invalid path, algorithm, etc.)
 */
int32_t fossil_io_filesys_file_hash(const char *path, unsigned char *hash_out, size_t hash_size, const char *algorithm);

/**
 * @brief Merge two files into a single destination file.
 *
 * Combines the contents of two source files into a single destination file according
 * to the specified merge strategy (concatenation, interleave, etc.).
 *
 * @param src1 Path to the first source file
 * @param src2 Path to the second source file
 * @param dest Path to the destination file
 * @param strategy Merge strategy to apply
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_file_merge(const char *src1, const char *src2, const char *dest, uint32_t strategy);

/**
 * @brief Split a file into multiple parts of specified size.
 *
 * Divides a file into smaller chunks of the specified size, creating multiple output
 * files with names prefixed by the given prefix and numbered sequentially.
 *
 * @param path Path to the file to split
 * @param part_size Size of each part in bytes
 * @param prefix Prefix for generated part file names
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_file_split(const char *path, size_t part_size, const char *prefix);

/**
 * @brief Join multiple file parts into a single file.
 *
 * Combines multiple file parts (typically created by file_split) into a single
 * destination file in the order specified by the parts array.
 *
 * @param parts Array of pointers to part file paths
 * @param count Number of parts in the array
 * @param dest Path to the destination file
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_file_join(const char **parts, size_t count, const char *dest);

/**
 * @brief Compress a file using the specified algorithm.
 *
 * Compresses the source file using the provided compression algorithm (e.g., "gzip", "bzip2")
 * and writes the compressed data to the destination file.
 *
 * @param src Path to the source file to compress
 * @param dest Path to the destination compressed file
 * @param algorithm Compression algorithm to use (e.g., "gzip", "bzip2")
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_file_compress(const char *src, const char *dest, const char *algorithm);

/**
 * @brief Decompress a file to its original form.
 *
 * Decompresses a previously compressed file and writes the decompressed data to the
 * destination file. The decompression algorithm is typically detected automatically.
 *
 * @param src Path to the compressed source file
 * @param dest Path to the destination decompressed file
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_file_decompress(const char *src, const char *dest);

/* ------------------------------------------------------------
    * Directory-Specific Operations
    * ------------------------------------------------------------ */

/**
 * @brief Create a directory at the specified path.
 *
 * Creates a new directory at the given path. If recursive is true, it will create any
 * necessary parent directories as well.
 * @param path Path to the directory to create
 * @param recursive Whether to create parent directories if they do not exist
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_dir_create(const char *path, bool recursive);

/**
 * @brief List the contents of a directory.
 *
 * Retrieves a list of entries in the specified directory, including their metadata.
 *
 * @param path Path to the directory to list
 * @param entries Array to store directory entries (caller-allocated)
 * @param max_entries Maximum number of entries to store in the array
 * @param out_count Pointer to store the actual number of entries found
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_dir_list(const char *path, fossil_io_filesys_obj_t *entries, size_t max_entries, size_t *out_count);

/**
 * @brief Walk a directory tree and apply a callback to each entry.
 *
 * Recursively traverses a directory and its subdirectories, invoking the provided
 * callback function for each filesystem object encountered.
 *
 * @param path Path to the directory to walk
 * @param callback Function to call for each entry (returns 0 to continue, non-zero to stop)
 * @param user_data Pointer to user data to pass to the callback
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_dir_walk(const char *path, int (*callback)(const fossil_io_filesys_obj_t *, void *), void *user_data);

/**
 * @brief Merge the contents of one directory into another.
 *
 * Merges the contents of the source directory into the destination directory. If
 * overwrite is true, existing files in the destination will be overwritten. If
 * recursive is true, subdirectories will also be merged.
 *
 * @param src Path to the source directory
 * @param dest Path to the destination directory
 * @param overwrite Whether to overwrite existing files in the destination
 * @param recursive Whether to merge subdirectories recursively
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_dir_merge(const char *src, const char *dest, bool overwrite, bool recursive);

/**
 * @brief Mirror a directory to another location.
 *
 * Creates an exact mirror of the source directory at the destination path. If
 * delete_extras is true, any files in the destination that are not in the source
 * will be deleted.
 *
 * @param src Path to the source directory
 * @param dest Path to the destination directory
 * @param delete_extras Whether to delete files in the destination that are not in the source
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_dir_mirror(const char *src, const char *dest, bool delete_extras);

/* ------------------------------------------------------------
    * Link-Specific Operations
    * ------------------------------------------------------------ */

/**
 * @brief Create a hard or symbolic link to a file, with optional metadata propagation.
 *
 * This function creates a new link (hard or symbolic) from `src->filename` to `dest_path`.
 * It optionally populates a fossil_io_file_t structure for the destination link and can
 * propagate file metadata such as timestamps and mode.
 *
 * @param src        Pointer to source fossil_io_file_t representing the existing file.
 * @param dest       Pointer to destination fossil_io_file_t to populate (may be NULL).
 * @param dest_path  Path to the file to create.
 * @param symbolic   If true, create a symbolic link; else create a hard link.
 * @param copy_meta  If true, copy metadata (mode, timestamps).
 *
 * @return int       0 on success, negative errno-style value on error.
 */
int32_t fossil_io_filesys_link_create(const char *target, const char *link_path, bool symbolic);

/**
 * @brief Read the target of a symbolic link.
 *
 * This function reads the target path of a symbolic link at the specified path.
 *
 * @param link_path Path to the symbolic link
 * @param target_out Buffer to store the target path
 * @param max_len Maximum length of the target_out buffer
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_link_read(const char *link_path, char *target_out, size_t max_len);

/**
 * @brief Resolve a symbolic link to its ultimate target.
 *
 * This function resolves a symbolic link at the specified path to its final target,
 * following any intermediate links if necessary.
 *
 * @param link_path Path to the symbolic link
 * @param target_out Buffer to store the resolved target path
 * @param max_len Maximum length of the target_out buffer
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_link_resolve(const char *link_path, char *target_out, size_t max_len);

/**
 * @brief Check if a link is symbolic or hard.
 *
 * This function checks whether the link at the specified path is a symbolic link
 * or a hard link.
 *
 * @param link_path Path to the link to check
 * @param is_symbolic Pointer to store the result (true if symbolic, false if hard)
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_link_is_symbolic(const char *link_path, bool *is_symbolic);

/* ------------------------------------------------------------
    * Transaction Operations
    * ------------------------------------------------------------ */

/**
 * @brief Begin a filesystem transaction.
 *
 * This function starts a new transaction, allowing multiple filesystem operations
 * to be performed atomically.
 *
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_tx_begin(void);

/**
 * @brief Commit a filesystem transaction.
 *
 * This function commits the current transaction, making all changes permanent.
 *
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_tx_commit(void);

/**
 * @brief Rollback a filesystem transaction.
 *
 * This function rolls back the current transaction, undoing all changes made
 * since the transaction began.
 *
 * @return 0 on success, negative error code on failure
 */
int32_t fossil_io_filesys_tx_rollback(void);

/* ------------------------------------------------------------
    * Path / Utility Operations
    * ------------------------------------------------------------ */

/**
 * @brief Get the current working directory.
 *
 * Retrieves the absolute path of the current working directory and stores it in the
 * provided buffer. The buffer must be large enough to hold the path plus null terminator.
 *
 * @param buf Pointer to buffer where the current working directory path will be stored
 * @param size Size of the buffer in bytes
 * @return 0 on success, negative error code on failure (buffer too small, permission denied, etc.)
 */
int32_t fossil_io_filesys_getcwd(char *buf, size_t size);

/**
 * @brief Change the current working directory.
 *
 * Changes the process's current working directory to the specified path. All relative
 * paths in subsequent operations will be resolved relative to this new directory.
 *
 * @param path Path to the directory to change to (absolute or relative)
 * @return 0 on success, negative error code on failure (path not found, permission denied, etc.)
 */
int32_t fossil_io_filesys_chdir(const char *path);

/**
 * @brief Convert a path to its absolute form.
 *
 * Resolves relative paths to absolute paths, expanding any relative path components
 * (., .., ~) and removing redundant separators. Symbolic links are not followed.
 *
 * @param path Input path (absolute or relative)
 * @param abs_path Buffer to store the resulting absolute path
 * @param max_len Maximum length of the abs_path buffer in bytes
 * @return 0 on success, negative error code on failure (buffer too small, invalid path, etc.)
 */
int32_t fossil_io_filesys_abspath(const char *path, char *abs_path, size_t max_len);

/**
 * @brief Extract the directory component from a path.
 *
 * Extracts the directory portion of a path string. For example, "/home/user/file.txt"
 * returns "/home/user". The trailing separator is not included in the output.
 *
 * @param path Input path string
 * @param dir_out Buffer to store the extracted directory name
 * @param max_len Maximum length of the dir_out buffer in bytes
 * @return 0 on success, negative error code on failure (buffer too small, invalid path, etc.)
 */
int32_t fossil_io_filesys_dirname(const char *path, char *dir_out, size_t max_len);

/**
 * @brief Extract the filename component from a path.
 *
 * Extracts the filename (basename) portion of a path string, removing all directory
 * components. For example, "/home/user/file.txt" returns "file.txt".
 *
 * @param path Input path string
 * @param name_out Buffer to store the extracted filename
 * @param max_len Maximum length of the name_out buffer in bytes
 * @return 0 on success, negative error code on failure (buffer too small, invalid path, etc.)
 */
int32_t fossil_io_filesys_basename(const char *path, char *name_out, size_t max_len);

/**
 * @brief Extract the file extension from a path.
 *
 * Extracts the file extension (suffix) from a filename, including the leading dot.
 * For example, "/home/user/file.tar.gz" may return ".gz" depending on implementation.
 * If no extension is present, an empty string is returned.
 *
 * @param path Input path string
 * @param ext_out Buffer to store the extracted file extension
 * @param max_len Maximum length of the ext_out buffer in bytes
 * @return 0 on success, negative error code on failure (buffer too small, invalid path, etc.)
 */
int32_t fossil_io_filesys_extension(const char *path, char *ext_out, size_t max_len);

/* ------------------------------------------------------------
    * Utility Functions
    * ------------------------------------------------------------ */

/**
 * @brief Convert a filesystem object type enum to a human-readable string.
 *
 * This function takes a fossil_io_filesys_type_t value and returns a corresponding
 * string representation (e.g., "file", "directory", "link", "unknown").
 *
 * @param type The filesystem object type to convert
 * @return A string representing the filesystem object type
 */
const char *fossil_io_filesys_type_string(fossil_io_filesys_type_t type);

#ifdef __cplusplus
}

#include "error.h"
#include <string>

namespace fossil::io
{

    class Filesys
    {
    public:
        // Constructor/Destructor
        Filesys() = default;
        ~Filesys() = default;

        /**
         * @brief Initialize a filesystem object from a given path.
         *
         * Populates metadata (type, size, permissions, timestamps) for the object
         * at the specified path. Reports errors via Error::report on failure.
         *
         * @param obj Pointer to filesystem object to initialize
         * @param path Path to the filesystem object
         * @return 0 on success, negative on failure
         */
        int32_t init(fossil_io_filesys_obj_t *obj, const std::string &path)
        {
            int32_t result = fossil_io_filesys_init(obj, path.c_str());
            if (result != 0)
            {
                Error::report("[%s] %s\n", "fs.not_found", Error::what("fs.not_found"));
            }
            return result;
        }

        /**
         * @brief Refresh the metadata of an existing filesystem object.
         *
         * Updates all metadata (size, permissions, timestamps) for an already
         * initialized filesystem object. Reports errors via Error::report on failure.
         *
         * @param obj Pointer to filesystem object to refresh
         * @return 0 on success, negative on failure
         */
        int32_t refresh(fossil_io_filesys_obj_t *obj)
        {
            int32_t result = fossil_io_filesys_refresh(obj);
            if (result != 0)
            {
                Error::report("[%s] %s\n", "fs.not_found", Error::what("fs.not_found"));
            }
            return result;
        }

        /**
         * @brief Check if a filesystem object exists at the given path.
         *
         * Determines whether a file, directory, or link exists at the specified path
         * without modifying any metadata or file access times.
         *
         * @param path Path to check for existence
         * @return 1 if exists, 0 if not, negative on error
         */
        int32_t exists(const std::string &path)
        {
            return fossil_io_filesys_exists(path.c_str());
        }

        /**
         * @brief Remove a filesystem object (file, directory, or link).
         *
         * Deletes the filesystem object at the specified path. If recursive is true
         * and the object is a directory, removes all contents recursively.
         * Reports permission errors via Error::report on failure.
         *
         * @param path Path to the object to remove
         * @param recursive If true, recursively remove directory contents
         * @return 0 on success, negative on failure
         */
        int32_t remove(const std::string &path, bool recursive = false)
        {
            int32_t result = fossil_io_filesys_remove(path.c_str(), recursive);
            if (result != 0)
            {
                Error::report("[%s] %s\n", "fs.permission", Error::what("fs.permission"));
            }
            return result;
        }

        /**
         * @brief Rename or move a filesystem object to a new location.
         *
         * Atomically moves (or renames) a filesystem object from src to dest.
         * Supports cross-directory moves when possible.
         *
         * @param src Source path of the object to move
         * @param dest Destination path for the object
         * @return 0 on success, negative on failure
         */
        int32_t move(const std::string &src, const std::string &dest)
        {
            return fossil_io_filesys_move(src.c_str(), dest.c_str());
        }

        /**
         * @brief Copy a filesystem object to a new location.
         *
         * Creates a copy of the file or directory at src to dest. If preserve_meta
         * is true, retains permissions and timestamps from the source.
         *
         * @param src Source path of the object to copy
         * @param dest Destination path for the copy
         * @param preserve_meta If true, preserve permissions and timestamps
         * @return 0 on success, negative on failure
         */
        int32_t copy(const std::string &src, const std::string &dest, bool preserve_meta = true)
        {
            return fossil_io_filesys_copy(src.c_str(), dest.c_str(), preserve_meta);
        }

        /**
         * @brief Atomically swap two filesystem objects.
         *
         * Exchanges the contents and metadata of two filesystem objects in an
         * atomic operation when supported by the platform.
         *
         * @param path1 Path to the first object
         * @param path2 Path to the second object
         * @return 0 on success, negative on failure
         */
        int32_t swap(const std::string &path1, const std::string &path2)
        {
            return fossil_io_filesys_swap(path1.c_str(), path2.c_str());
        }

        /**
         * @brief Deduplicate files in a directory by removing duplicates.
         *
         * Scans a directory and removes duplicate files, optionally scanning
         * subdirectories recursively. Uses hash-based comparison.
         *
         * @param path Directory path to scan for duplicates
         * @param recursive If true, scan subdirectories recursively
         * @return Number of duplicates removed, or negative on failure
         */
        int32_t deduplicate(const std::string &path, bool recursive = false)
        {
            return fossil_io_filesys_deduplicate(path.c_str(), recursive);
        }

        /**
         * @brief Get filesystem object metadata (type, size, permissions, timestamps).
         *
         * Retrieves complete metadata information about a filesystem object
         * without opening it or modifying access times.
         *
         * @param path Path to the object
         * @param obj Pointer to storage for metadata
         * @return 0 on success, negative on failure
         */
        int32_t stat(const std::string &path, fossil_io_filesys_obj_t *obj)
        {
            return fossil_io_filesys_stat(path.c_str(), obj);
        }

        /**
         * @brief Open a file and initialize a file object.
         *
         * Opens a file at the specified path with the given mode and initializes
         * the file object structure for subsequent read/write operations.
         *
         * @param f Pointer to file object to initialize
         * @param path Path to the file to open
         * @param mode File access mode (e.g., "r", "w", "a", "rb", "wb")
         * @return 0 on success, negative on failure
         */
        int32_t file_open(fossil_io_filesys_file_t *f, const std::string &path, const std::string &mode)
        {
            return fossil_io_filesys_file_open(f, path.c_str(), mode.c_str());
        }

        /**
         * @brief Close an open file and release resources.
         *
         * Closes the file, flushes any buffered data, and releases platform-specific
         * file handles. The file object should not be used after this call without reopening.
         *
         * @param f Pointer to the file object to close
         * @return 0 on success, negative on failure
         */
        int32_t file_close(fossil_io_filesys_file_t *f)
        {
            return fossil_io_filesys_file_close(f);
        }

        /**
         * @brief Read data from an open file.
         *
         * Reads up to count elements of size bytes from the current file position
         * into the provided buffer. Advances the position by bytes read.
         *
         * @param f Pointer to the open file object
         * @param buf Pointer to buffer for read data
         * @param size Size of each element in bytes
         * @param count Number of elements to read
         * @return Number of elements successfully read, 0 on EOF or error
         */
        size_t file_read(fossil_io_filesys_file_t *f, void *buf, size_t size, size_t count)
        {
            return fossil_io_filesys_file_read(f, buf, size, count);
        }

        /**
         * @brief Write data to an open file.
         *
         * Writes count elements of size bytes from the buffer to the file at the
         * current position. May be buffered depending on file object settings.
         *
         * @param f Pointer to the open file object
         * @param buf Pointer to buffer containing data to write
         * @param size Size of each element in bytes
         * @param count Number of elements to write
         * @return Number of elements successfully written, 0 on error
         */
        size_t file_write(fossil_io_filesys_file_t *f, const void *buf, size_t size, size_t count)
        {
            return fossil_io_filesys_file_write(f, buf, size, count);
        }

        /**
         * @brief Seek to a specific position in an open file.
         *
         * Changes the file position indicator to the specified offset relative to
         * the given origin (SEEK_SET, SEEK_CUR, SEEK_END).
         *
         * @param f Pointer to the open file object
         * @param offset Byte offset from the origin
         * @param origin Reference point: SEEK_SET, SEEK_CUR, or SEEK_END
         * @return 0 on success, negative on failure
         */
        int32_t file_seek(fossil_io_filesys_file_t *f, int64_t offset, int32_t origin)
        {
            return fossil_io_filesys_file_seek(f, offset, origin);
        }

        /**
         * @brief Get the current position in an open file.
         *
         * Returns the current byte offset of the file position indicator from the
         * start of the file.
         *
         * @param f Pointer to the open file object
         * @return Current file position in bytes, or negative on failure
         */
        int64_t file_tell(fossil_io_filesys_file_t *f)
        {
            return fossil_io_filesys_file_tell(f);
        }

        /**
         * @brief Flush buffered data to the file.
         *
         * Forces any pending buffered write operations on the file to be written
         * to the underlying storage medium, ensuring data durability.
         *
         * @param f Pointer to the open file object
         * @return 0 on success, negative on failure
         */
        int32_t file_flush(fossil_io_filesys_file_t *f)
        {
            return fossil_io_filesys_file_flush(f);
        }

        /**
         * @brief Get the size of a file.
         *
         * Returns the total size in bytes of the file at the specified path
         * without requiring the file to be opened.
         *
         * @param path Path to the file
         * @return File size in bytes, or negative on failure
         */
        int32_t file_size(const std::string &path)
        {
            return fossil_io_filesys_file_size(path.c_str());
        }

        /**
         * @brief Truncate or extend a file to a specific size.
         *
         * Resizes the file to the given size. If larger, it will be truncated.
         * If smaller, it will be extended (typically with null bytes).
         *
         * @param path Path to the file
         * @param size New size in bytes
         * @return 0 on success, negative on failure
         */
        int32_t file_truncate(const std::string &path, size_t size)
        {
            return fossil_io_filesys_file_truncate(path.c_str(), size);
        }

        /**
         * @brief Compute the cryptographic hash of a file.
         *
         * Reads the entire contents of a file and computes its hash using the
         * specified algorithm (e.g., "sha256", "md5", "sha1").
         *
         * @param path Path to the file to hash
         * @param hash_out Pointer to buffer for the computed hash
         * @param hash_size Size of the hash_out buffer in bytes
         * @param algorithm Name of the hashing algorithm to use
         * @return 0 on success, negative on failure
         */
        int32_t file_hash(const std::string &path, unsigned char *hash_out, size_t hash_size, const std::string &algorithm)
        {
            return fossil_io_filesys_file_hash(path.c_str(), hash_out, hash_size, algorithm.c_str());
        }

        /**
         * @brief Merge two files into a single destination file.
         *
         * Combines the contents of two source files into a single destination file
         * according to the specified merge strategy.
         *
         * @param src1 Path to the first source file
         * @param src2 Path to the second source file
         * @param dest Path to the destination file
         * @param strategy Merge strategy to apply (concatenation, interleave, etc.)
         * @return 0 on success, negative on failure
         */
        int32_t file_merge(const std::string &src1, const std::string &src2, const std::string &dest, uint32_t strategy)
        {
            return fossil_io_filesys_file_merge(src1.c_str(), src2.c_str(), dest.c_str(), strategy);
        }

        /**
         * @brief Split a file into multiple parts of specified size.
         *
         * Divides a file into smaller chunks, creating multiple output files with
         * names prefixed by the given prefix and numbered sequentially.
         *
         * @param path Path to the file to split
         * @param part_size Size of each part in bytes
         * @param prefix Prefix for generated part file names
         * @return 0 on success, negative on failure
         */
        int32_t file_split(const std::string &path, size_t part_size, const std::string &prefix)
        {
            return fossil_io_filesys_file_split(path.c_str(), part_size, prefix.c_str());
        }

        /**
         * @brief Join multiple file parts into a single file.
         *
         * Combines multiple file parts (typically created by file_split) into a
         * single destination file in the specified order.
         *
         * @param parts Array of pointers to part file paths
         * @param count Number of parts in the array
         * @param dest Path to the destination file
         * @return 0 on success, negative on failure
         */
        int32_t file_join(const char **parts, size_t count, const std::string &dest)
        {
            return fossil_io_filesys_file_join(parts, count, dest.c_str());
        }

        /**
         * @brief Compress a file using the specified algorithm.
         *
         * Compresses the source file using the provided compression algorithm
         * (e.g., "gzip", "bzip2") and writes compressed data to the destination.
         *
         * @param src Path to the source file to compress
         * @param dest Path to the destination compressed file
         * @param algorithm Compression algorithm to use
         * @return 0 on success, negative on failure
         */
        int32_t file_compress(const std::string &src, const std::string &dest, const std::string &algorithm)
        {
            return fossil_io_filesys_file_compress(src.c_str(), dest.c_str(), algorithm.c_str());
        }

        /**
         * @brief Decompress a file to its original form.
         *
         * Decompresses a previously compressed file and writes the decompressed
         * data to the destination. Algorithm is typically detected automatically.
         *
         * @param src Path to the compressed source file
         * @param dest Path to the destination decompressed file
         * @return 0 on success, negative on failure
         */
        int32_t file_decompress(const std::string &src, const std::string &dest)
        {
            return fossil_io_filesys_file_decompress(src.c_str(), dest.c_str());
        }

        /**
         * @brief Create a directory at the specified path.
         *
         * Creates a new directory. If recursive is true, also creates any necessary
         * parent directories as needed.
         *
         * @param path Path to the directory to create
         * @param recursive If true, create parent directories if they do not exist
         * @return 0 on success, negative on failure
         */
        int32_t dir_create(const std::string &path, bool recursive = false)
        {
            return fossil_io_filesys_dir_create(path.c_str(), recursive);
        }

        /**
         * @brief List the contents of a directory.
         *
         * Retrieves a list of entries in the specified directory, including their
         * metadata. Results are stored in the provided array up to max_entries.
         *
         * @param path Path to the directory to list
         * @param entries Array to store directory entries (caller-allocated)
         * @param max_entries Maximum number of entries to store
         * @param out_count Pointer to store the actual number of entries found
         * @return 0 on success, negative on failure
         */
        int32_t dir_list(const std::string &path, fossil_io_filesys_obj_t *entries, size_t max_entries, size_t *out_count)
        {
            return fossil_io_filesys_dir_list(path.c_str(), entries, max_entries, out_count);
        }

        /**
         * @brief Walk a directory tree and apply a callback to each entry.
         *
         * Recursively traverses a directory and subdirectories, invoking the provided
         * callback function for each filesystem object encountered.
         *
         * @param path Path to the directory to walk
         * @param callback Function to call for each entry (return 0 to continue, non-zero to stop)
         * @param user_data Pointer to user data to pass to the callback
         * @return 0 on success, negative on failure
         */
        int32_t dir_walk(const std::string &path, int (*callback)(const fossil_io_filesys_obj_t *, void *), void *user_data)
        {
            return fossil_io_filesys_dir_walk(path.c_str(), callback, user_data);
        }

        /**
         * @brief Merge the contents of one directory into another.
         *
         * Merges the contents of the source directory into the destination directory.
         * If overwrite is true, existing files in the destination will be replaced.
         * If recursive is true, subdirectories will also be merged.
         *
         * @param src Path to the source directory
         * @param dest Path to the destination directory
         * @param overwrite If true, overwrite existing files in destination
         * @param recursive If true, merge subdirectories recursively
         * @return 0 on success, negative on failure
         */
        int32_t dir_merge(const std::string &src, const std::string &dest, bool overwrite = false, bool recursive = true)
        {
            return fossil_io_filesys_dir_merge(src.c_str(), dest.c_str(), overwrite, recursive);
        }

        /**
         * @brief Mirror a directory to another location.
         *
         * Creates an exact mirror of the source directory at the destination path.
         * If delete_extras is true, any files in the destination that are not in
         * the source will be deleted.
         *
         * @param src Path to the source directory
         * @param dest Path to the destination directory
         * @param delete_extras If true, delete files in destination not in source
         * @return 0 on success, negative on failure
         */
        int32_t dir_mirror(const std::string &src, const std::string &dest, bool delete_extras = false)
        {
            return fossil_io_filesys_dir_mirror(src.c_str(), dest.c_str(), delete_extras);
        }

        /**
         * @brief Create a hard or symbolic link to a file.
         *
         * Creates a new link (hard or symbolic) from the target file to the link path.
         * Symbolic links point to the target; hard links are direct references.
         *
         * @param target Path to the target file
         * @param link_path Path where the link will be created
         * @param symbolic If true, create a symbolic link; else create a hard link
         * @return 0 on success, negative on failure
         */
        int32_t link_create(const std::string &target, const std::string &link_path, bool symbolic = true)
        {
            return fossil_io_filesys_link_create(target.c_str(), link_path.c_str(), symbolic);
        }

        /**
         * @brief Read the target of a symbolic link.
         *
         * Reads the target path of a symbolic link at the specified path,
         * without following intermediate links.
         *
         * @param link_path Path to the symbolic link
         * @param target_out Buffer to store the target path
         * @param max_len Maximum length of the target_out buffer
         * @return 0 on success, negative on failure
         */
        int32_t link_read(const std::string &link_path, char *target_out, size_t max_len)
        {
            return fossil_io_filesys_link_read(link_path.c_str(), target_out, max_len);
        }

        /**
         * @brief Resolve a symbolic link to its ultimate target.
         *
         * Resolves a symbolic link at the specified path to its final target,
         * following any intermediate links if necessary.
         *
         * @param link_path Path to the symbolic link
         * @param target_out Buffer to store the resolved target path
         * @param max_len Maximum length of the target_out buffer
         * @return 0 on success, negative on failure
         */
        int32_t link_resolve(const std::string &link_path, char *target_out, size_t max_len)
        {
            return fossil_io_filesys_link_resolve(link_path.c_str(), target_out, max_len);
        }

        /**
         * @brief Check if a link is symbolic or hard.
         *
         * Determines whether the link at the specified path is a symbolic link
         * or a hard link.
         *
         * @param link_path Path to the link to check
         * @param is_symbolic Pointer to store the result (true if symbolic, false if hard)
         * @return 0 on success, negative on failure
         */
        int32_t link_is_symbolic(const std::string &link_path, bool *is_symbolic)
        {
            return fossil_io_filesys_link_is_symbolic(link_path.c_str(), is_symbolic);
        }

        /**
         * @brief Begin a filesystem transaction.
         *
         * Starts a new transaction, allowing multiple filesystem operations
         * to be performed atomically. Use commit() or rollback() to finalize.
         *
         * @return 0 on success, negative on failure
         */
        int32_t tx_begin()
        {
            return fossil_io_filesys_tx_begin();
        }

        /**
         * @brief Commit a filesystem transaction.
         *
         * Commits the current transaction, making all changes permanent.
         * All operations since tx_begin() are applied atomically.
         *
         * @return 0 on success, negative on failure
         */
        int32_t tx_commit()
        {
            return fossil_io_filesys_tx_commit();
        }

        /**
         * @brief Rollback a filesystem transaction.
         *
         * Rolls back the current transaction, undoing all changes made
         * since tx_begin() was called.
         *
         * @return 0 on success, negative on failure
         */
        int32_t tx_rollback()
        {
            return fossil_io_filesys_tx_rollback();
        }

        /**
         * @brief Get the current working directory.
         *
         * Retrieves the absolute path of the current working directory and stores
         * it in the provided buffer. Buffer must be large enough for path + null terminator.
         *
         * @param buf Pointer to buffer for the current working directory path
         * @param size Size of the buffer in bytes
         * @return 0 on success, negative on failure
         */
        int32_t getcwd(char *buf, size_t size)
        {
            return fossil_io_filesys_getcwd(buf, size);
        }

        /**
         * @brief Change the current working directory.
         *
         * Changes the process's current working directory to the specified path.
         * All relative paths in subsequent operations are resolved relative to this directory.
         *
         * @param path Path to the directory to change to (absolute or relative)
         * @return 0 on success, negative on failure
         */
        int32_t chdir(const std::string &path)
        {
            return fossil_io_filesys_chdir(path.c_str());
        }

        /**
         * @brief Convert a path to its absolute form.
         *
         * Resolves relative paths to absolute paths, expanding relative components
         * (., .., ~) and removing redundant separators. Symbolic links are not followed.
         *
         * @param path Input path (absolute or relative)
         * @param abs_path Buffer to store the resulting absolute path
         * @param max_len Maximum length of the abs_path buffer in bytes
         * @return 0 on success, negative on failure
         */
        int32_t abspath(const std::string &path, char *abs_path, size_t max_len)
        {
            return fossil_io_filesys_abspath(path.c_str(), abs_path, max_len);
        }

        /**
         * @brief Extract the directory component from a path.
         *
         * Extracts the directory portion of a path string. For example, "/home/user/file.txt"
         * returns "/home/user" without the trailing separator.
         *
         * @param path Input path string
         * @param dir_out Buffer to store the extracted directory name
         * @param max_len Maximum length of the dir_out buffer in bytes
         * @return 0 on success, negative on failure
         */
        int32_t dirname(const std::string &path, char *dir_out, size_t max_len)
        {
            return fossil_io_filesys_dirname(path.c_str(), dir_out, max_len);
        }

        /**
         * @brief Extract the filename component from a path.
         *
         * Extracts the filename (basename) portion of a path string, removing all directory
         * components. For example, "/home/user/file.txt" returns "file.txt".
         *
         * @param path Input path string
         * @param name_out Buffer to store the extracted filename
         * @param max_len Maximum length of the name_out buffer in bytes
         * @return 0 on success, negative on failure
         */
        int32_t basename(const std::string &path, char *name_out, size_t max_len)
        {
            return fossil_io_filesys_basename(path.c_str(), name_out, max_len);
        }

        /**
         * @brief Extract the file extension from a path.
         *
         * Extracts the file extension (suffix) from a filename, including the leading dot.
         * For example, might return ".gz" from "file.tar.gz". If no extension is present,
         * returns an empty string.
         *
         * @param path Input path string
         * @param ext_out Buffer to store the extracted file extension
         * @param max_len Maximum length of the ext_out buffer in bytes
         * @return 0 on success, negative on failure
         */
        int32_t extension(const std::string &path, char *ext_out, size_t max_len)
        {
            return fossil_io_filesys_extension(path.c_str(), ext_out, max_len);
        }

        /**
         * @brief Convert a filesystem object type enum to a human-readable string.
         *
         * Takes a fossil_io_filesys_type_t value and returns a corresponding string
         * representation (e.g., "file", "directory", "link", "unknown").
         *
         * @param type The filesystem object type to convert
         * @return A string representing the filesystem object type
         */
        const char *type_string(fossil_io_filesys_type_t type)
        {
            return fossil_io_filesys_type_string(type);
        }
    };

} // namespace fossil

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
