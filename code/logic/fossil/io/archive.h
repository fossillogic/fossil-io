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
    FOSSIL_IO_ARCHIVE_TARBZ2,
    FOSSIL_IO_ARCHIVE_TARXZ,
    FOSSIL_IO_ARCHIVE_TARLZ4,
    FOSSIL_IO_ARCHIVE_TARZST,
    FOSSIL_IO_ARCHIVE_RAR,
    FOSSIL_IO_ARCHIVE_RAR5,
    FOSSIL_IO_ARCHIVE_7Z,
    FOSSIL_IO_ARCHIVE_CAB,
    FOSSIL_IO_ARCHIVE_ACE,
    FOSSIL_IO_ARCHIVE_ISO,
    FOSSIL_IO_ARCHIVE_BZ2,
    FOSSIL_IO_ARCHIVE_GZ,
    FOSSIL_IO_ARCHIVE_XZ,
    FOSSIL_IO_ARCHIVE_LZ4,
    FOSSIL_IO_ARCHIVE_ZSTD,
    FOSSIL_IO_ARCHIVE_ALL // wildcard for detection
} fossil_io_archive_type_t;

/**
 * Archive compression levels.
 */
typedef enum fossil_io_archive_compression {
    FOSSIL_IO_COMPRESSION_NONE = 0,
    FOSSIL_IO_COMPRESSION_FASTEST = 1,
    FOSSIL_IO_COMPRESSION_FAST = 3,
    FOSSIL_IO_COMPRESSION_NORMAL = 6,
    FOSSIL_IO_COMPRESSION_MAXIMUM = 9
} fossil_io_archive_compression_t;

/**
 * Archive operation modes.
 */
typedef enum fossil_io_archive_mode {
    FOSSIL_IO_ARCHIVE_READ = 1,
    FOSSIL_IO_ARCHIVE_WRITE = 2,
    FOSSIL_IO_ARCHIVE_APPEND = 4
} fossil_io_archive_mode_t;

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
    size_t compressed_size;  // Compressed size in archive
    bool is_directory;       // True if directory
    bool is_encrypted;       // True if encrypted
    uint64_t modified_time;  // Epoch timestamp
    uint64_t created_time;   // Creation timestamp
    uint32_t crc32;          // CRC32 checksum
    uint32_t permissions;    // File permissions (Unix style)
} fossil_io_archive_entry_t;

/**
 * Archive statistics.
 */
typedef struct fossil_io_archive_stats {
    size_t total_entries;
    size_t total_size;
    size_t compressed_size;
    double compression_ratio;
} fossil_io_archive_stats_t;

// ======================================================
// Initialization & Cleanup
// ======================================================

/**
 * Initialize an archive handle with specified mode and compression level.
 * 
 * This function creates and initializes an archive handle for reading, writing,
 * or appending to archive files. The archive type determines the format (ZIP, TAR, etc.),
 * mode specifies the access pattern (read/write/append), and compression sets the
 * compression level for write operations.
 * 
 * @param path The file system path to the archive file
 * @param type The archive format type (ZIP, TAR, 7Z, etc.)
 * @param mode The access mode (read, write, or append)
 * @param compression The compression level (none to maximum)
 * @return Pointer to initialized archive handle, or NULL on failure
 * 
 * @note The caller is responsible for closing the archive with fossil_io_archive_close()
 * @see fossil_io_archive_close()
 */
fossil_io_archive_t* fossil_io_archive_open(const char *path, fossil_io_archive_type_t type, fossil_io_archive_mode_t mode, fossil_io_archive_compression_t compression);

/**
 * Close and free an archive handle.
 * 
 * This function properly closes an opened archive, flushes any pending operations,
 * and releases all associated memory and system resources. After calling this
 * function, the archive handle becomes invalid and should not be used.
 * 
 * @param archive Pointer to the archive handle to close
 * 
 * @note Passing NULL is safe and will be ignored
 * @note All pending write operations are flushed before closing
 */
void fossil_io_archive_close(fossil_io_archive_t *archive);

// ======================================================
// Archive Inspection
// ======================================================

/**
 * Get archive type (auto-detect if unknown)
 * 
 * This function examines an archive file and determines its format type by
 * analyzing the file headers and magic numbers. It supports detection of
 * all major archive formats including ZIP, TAR variants, RAR, 7Z, and others.
 * 
 * @param path The file system path to the archive file to analyze
 * @return The detected archive type, or FOSSIL_IO_ARCHIVE_UNKNOWN if unrecognized
 * 
 * @note The file must exist and be readable for detection to work
 * @note Detection is based on file content, not file extension
 */
fossil_io_archive_type_t fossil_io_archive_get_type(const char *path);

/**
 * Get archive statistics.
 * 
 * This function retrieves comprehensive statistics about an archive including
 * the total number of entries, uncompressed size, compressed size, and
 * calculated compression ratio. The statistics reflect the current state
 * of the archive at the time of the call.
 * 
 * @param archive Pointer to an opened archive handle
 * @param stats Pointer to structure that will receive the statistics
 * @return True if statistics were successfully retrieved, false on error
 * 
 * @note The archive must be opened in a readable mode
 * @note Compression ratio is calculated as (compressed_size / total_size)
 */
bool fossil_io_archive_get_stats(fossil_io_archive_t *archive, fossil_io_archive_stats_t *stats);

/**
 * List all entries in the archive.
 * 
 * This function enumerates all files and directories contained within an archive
 * and returns detailed information about each entry including names, sizes,
 * timestamps, and attributes. The returned array contains metadata for all
 * entries without extracting the actual file content.
 * 
 * @param archive Pointer to an opened archive handle
 * @param entries Pointer to receive allocated array of entry structures
 * @return Number of entries found, or -1 on error
 * 
 * @note Caller must free the entries array using fossil_io_archive_free_entries()
 * @note The archive must be opened in a readable mode
 * @note Entry names use forward slashes as path separators regardless of platform
 */
ssize_t fossil_io_archive_list(fossil_io_archive_t *archive, fossil_io_archive_entry_t **entries);

/**
 * Free memory allocated for entries.
 * 
 * This function properly releases all memory associated with an array of
 * archive entries returned by fossil_io_archive_list(). It handles both
 * the entry structures and any dynamically allocated strings within them.
 * 
 * @param entries Pointer to the entries array to free
 * @param count Number of entries in the array
 * 
 * @note Passing NULL for entries is safe and will be ignored
 * @note The count parameter must match the value returned by fossil_io_archive_list()
 */
void fossil_io_archive_free_entries(fossil_io_archive_entry_t *entries, size_t count);

// ======================================================
// Extraction
// ======================================================

/**
 * Extract a single file to the specified path.
 * 
 * This function extracts a specific file from an archive to a destination
 * path on the file system. The extraction preserves file attributes when
 * possible and handles decompression automatically. Directory structures
 * are created as needed for the destination path.
 * 
 * @param archive Pointer to an opened archive handle (read mode)
 * @param entry_name The name/path of the file within the archive
 * @param dest_path The destination file system path for extraction
 * @return True if extraction succeeded, false on error
 * 
 * @note The archive must be opened in read mode
 * @note Parent directories of dest_path will be created if they don't exist
 * @note Existing files at dest_path will be overwritten
 */
bool fossil_io_archive_extract_file(fossil_io_archive_t *archive, const char *entry_name, const char *dest_path);

/**
 * Extract all files in the archive to the specified directory.
 * 
 * This function performs a complete extraction of all files and directories
 * from an archive to a destination directory. It recreates the directory
 * structure as stored in the archive and preserves file attributes and
 * timestamps when supported by the file system.
 * 
 * @param archive Pointer to an opened archive handle (read mode)
 * @param dest_dir The destination directory for extraction
 * @return True if all files were extracted successfully, false on any error
 * 
 * @note The destination directory will be created if it doesn't exist
 * @note Existing files in dest_dir may be overwritten
 * @note The operation stops on the first error encountered
 */
bool fossil_io_archive_extract_all(fossil_io_archive_t *archive, const char *dest_dir);

// ======================================================
// Creation / Modification
// ======================================================

/**
 * Create a new archive at path with the given type and compression level.
 * 
 * This function creates a new, empty archive file at the specified path
 * with the given format type and compression settings. The archive is
 * opened in write mode and ready to accept new entries. Any existing
 * file at the path will be overwritten.
 * 
 * @param path The file system path where the new archive will be created
 * @param type The archive format type (ZIP, TAR, etc.)
 * @param compression The compression level to use for added files
 * @return Pointer to the new archive handle, or NULL on failure
 * 
 * @note The caller must close the archive with fossil_io_archive_close()
 * @note Parent directories will be created if they don't exist
 * @note Existing files at the path will be overwritten without warning
 */
fossil_io_archive_t* fossil_io_archive_create(const char *path, fossil_io_archive_type_t type, fossil_io_archive_compression_t compression);

/**
 * Add a file from disk to the archive.
 * 
 * This function reads a file from the file system and adds it to an archive
 * with the specified internal path name. The file is compressed according
 * to the archive's compression settings and file attributes are preserved
 * in the archive metadata.
 * 
 * @param archive Pointer to an opened archive handle (write or append mode)
 * @param src_path The source file path on the file system
 * @param archive_path The path name to use inside the archive
 * @return True if the file was added successfully, false on error
 * 
 * @note The archive must be opened in write or append mode
 * @note The source file must exist and be readable
 * @note Archive paths should use forward slashes regardless of platform
 */
bool fossil_io_archive_add_file(fossil_io_archive_t *archive, const char *src_path, const char *archive_path);

/**
 * Add a directory recursively to the archive.
 * 
 * This function recursively adds all files and subdirectories from a source
 * directory to an archive, preserving the directory structure. It creates
 * directory entries in the archive and processes all files within the
 * directory tree, maintaining relative paths.
 * 
 * @param archive Pointer to an opened archive handle (write or append mode)
 * @param src_dir The source directory path on the file system
 * @param archive_dir The base directory path to use inside the archive
 * @return True if all files were added successfully, false on any error
 * 
 * @note The archive must be opened in write or append mode
 * @note Symbolic links are followed and their targets are archived
 * @note The operation stops on the first error encountered
 */
bool fossil_io_archive_add_directory(fossil_io_archive_t *archive, const char *src_dir, const char *archive_dir);

/**
 * Remove a file from the archive.
 * 
 * This function removes a specific entry from an archive by marking it
 * as deleted or by rebuilding the archive without the specified entry,
 * depending on the archive format capabilities. The operation may require
 * rewriting the entire archive for some formats.
 * 
 * @param archive Pointer to an opened archive handle (write or append mode)
 * @param entry_name The name/path of the entry to remove from the archive
 * @return True if the entry was removed successfully, false on error
 * 
 * @note The archive must be opened in write or append mode
 * @note Some archive formats may require complete reconstruction
 * @note Removing non-existent entries returns false
 */
bool fossil_io_archive_remove(fossil_io_archive_t *archive, const char *entry_name);

// ======================================================
// Utility / Info
// ======================================================

/**
 * Check if an entry exists in the archive.
 * 
 * This function searches the archive's directory structure to determine
 * if a specific file or directory entry exists. It performs a quick
 * lookup without extracting or reading the actual file content, making
 * it efficient for existence checks.
 * 
 * @param archive Pointer to an opened archive handle
 * @param entry_name The name/path to search for within the archive
 * @return True if the entry exists, false if not found or on error
 * 
 * @note The search is case-sensitive and uses exact string matching
 * @note Directory entries should be searched with their full path
 * @note The archive must be opened in a readable mode
 */
bool fossil_io_archive_exists(fossil_io_archive_t *archive, const char *entry_name);

/**
 * Get the size of an entry in bytes.
 * 
 * This function retrieves the uncompressed size of a specific entry
 * within an archive. It reads the metadata without extracting the file,
 * providing a quick way to determine storage requirements before extraction.
 * 
 * @param archive Pointer to an opened archive handle
 * @param entry_name The name/path of the entry to query
 * @return The uncompressed size in bytes, or -1 on error or if not found
 * 
 * @note Returns the original file size, not the compressed size in the archive
 * @note Directory entries typically return 0 size
 * @note The archive must be opened in a readable mode
 */
ssize_t fossil_io_archive_entry_size(fossil_io_archive_t *archive, const char *entry_name);

/**
 * Print archive contents to stdout.
 * 
 * This function generates a human-readable listing of all entries in an
 * archive, displaying information such as entry names, sizes, timestamps,
 * and permissions in a formatted table. It provides a quick overview
 * of archive contents similar to command-line archive tools.
 * 
 * @param archive Pointer to an opened archive handle
 * 
 * @note Output is sent to stdout and formatted for terminal display
 * @note The archive must be opened in a readable mode
 * @note Large archives may produce extensive output
 */
void fossil_io_archive_print(fossil_io_archive_t *archive);

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
         * @class Archive
         * @brief A modern C++ wrapper for the Fossil Logic archive functionality.
         * 
         * This class provides a comprehensive interface for creating, reading, and modifying
         * archive files in various formats including ZIP, TAR, 7Z, RAR, and others. It wraps
         * the C API with RAII semantics, automatic resource management, and STL container
         * integration for convenient use in modern C++ applications.
         * 
         * The class supports move semantics for efficient resource transfer and prevents
         * copying to avoid double-free issues. Archive operations include compression,
         * extraction, directory traversal, and metadata inspection.
         * 
         * @note This class is not thread-safe. Use external synchronization for concurrent access.
         * @see fossil_io_archive_t for the underlying C implementation
         */
        class Archive {
        private:
            fossil_io_archive_t* handle; ///< Handle to the underlying C archive structure
            
        public:
            /**
             * @brief Constructs an Archive object and opens an existing archive file.
             * 
             * Creates a new Archive instance by opening an archive file at the specified path
             * with the given format type, access mode, and compression level. The constructor
             * automatically handles format detection if needed and initializes the internal
             * archive handle for subsequent operations.
             * 
             * @param path The file system path to the archive file to open
             * @param type The expected archive format (ZIP, TAR, 7Z, etc.)
             * @param mode Access mode - read, write, or append operations
             * @param compression Compression level for write operations (ignored for read mode)
             * 
             * @throws May throw if path is invalid or file cannot be accessed
             * @note Use isValid() to check if the archive was successfully opened
             * @see isValid(), fossil_io_archive_open()
             */
            Archive(const std::string& path, fossil_io_archive_type_t type, 
               fossil_io_archive_mode_t mode, fossil_io_archive_compression_t compression)
            : handle(fossil_io_archive_open(path.c_str(), type, mode, compression)) {}
            
            /**
             * @brief Destructor that automatically closes the archive and releases resources.
             * 
             * Ensures proper cleanup by closing the underlying archive handle if it's valid.
             * This guarantees that all pending write operations are flushed and system
             * resources are properly released, preventing memory leaks and file handle
             * exhaustion.
             * 
             * @note All pending operations are automatically committed before destruction
             */
            ~Archive() {
                if (handle) {
                    fossil_io_archive_close(handle);
                }
            }
            
            /**
             * @brief Deleted copy constructor to prevent shallow copying.
             * 
             * Archives manage system resources and file handles that cannot be safely
             * shared between instances. Copying is explicitly disabled to prevent
             * double-free errors and resource conflicts.
             * 
             * @param other Another Archive instance (parameter ignored)
             */
            Archive(const Archive&) = delete;
            
            /**
             * @brief Deleted copy assignment operator to prevent shallow copying.
             * 
             * Similar to the copy constructor, assignment is disabled to maintain
             * exclusive ownership of the underlying archive handle and prevent
             * resource management issues.
             * 
             * @param other Another Archive instance (parameter ignored)
             * @return Reference to this instance (never reached due to deletion)
             */
            Archive& operator=(const Archive&) = delete;
            
            /**
             * @brief Move constructor for efficient resource transfer.
             * 
             * Transfers ownership of the archive handle from another Archive instance
             * to this one. The source object is left in a valid but empty state,
             * ensuring that only one instance manages the resource at any time.
             * 
             * @param other Archive instance to move from (will be invalidated)
             * @note The moved-from object should not be used except for destruction
             */
            Archive(Archive&& other) noexcept : handle(other.handle) {
                other.handle = nullptr;
            }
            
            /**
             * @brief Move assignment operator for efficient resource transfer.
             * 
             * Safely transfers ownership of an archive handle from another instance
             * while properly cleaning up any existing resources. This ensures proper
             * resource management during assignment operations.
             * 
             * @param other Archive instance to move from
             * @return Reference to this instance for method chaining
             * @note Self-assignment is safely handled
             */
            Archive& operator=(Archive&& other) noexcept {
                if (this != &other) {
                    if (handle) {
                    fossil_io_archive_close(handle);
                    }
                    handle = other.handle;
                    other.handle = nullptr;
                }
                return *this;
            }
            
            /**
             * @brief Static factory method for creating new archive files.
             * 
             * Creates a brand new archive file at the specified path with the given
             * format and compression settings. This is the preferred method for
             * creating archives as it properly initializes all internal structures
             * and handles format-specific requirements.
             * 
             * @param path File system path where the new archive will be created
             * @param type Archive format to use (ZIP, TAR, 7Z, etc.)
             * @param compression Compression level for the archive contents
             * @return New Archive instance ready for adding files
             * 
             * @note Any existing file at the path will be overwritten
             * @note Parent directories will be created if they don't exist
             * @see fossil_io_archive_create()
             */
            static Archive create(const std::string& path, fossil_io_archive_type_t type, 
                fossil_io_archive_compression_t compression) {
                Archive archive("", FOSSIL_IO_ARCHIVE_UNKNOWN, FOSSIL_IO_ARCHIVE_WRITE, FOSSIL_IO_COMPRESSION_NONE);
                archive.handle = fossil_io_archive_create(path.c_str(), type, compression);
                return archive;
            }
            
            /**
             * @brief Detects and returns the archive format type of a file.
             * 
             * Analyzes the file at the given path to automatically determine its
             * archive format by examining file headers, magic numbers, and structural
             * patterns. This is useful for processing archives of unknown format
             * or validating archive files.
             * 
             * @param path File system path to the archive file to analyze
             * @return Detected archive type, or FOSSIL_IO_ARCHIVE_UNKNOWN if unrecognized
             * 
             * @note Detection is based on file content, not filename extension
             * @note The file must exist and be readable for detection to work
             * @see fossil_io_archive_get_type()
             */
            fossil_io_archive_type_t get_type(const std::string& path) const {
                return fossil_io_archive_get_type(path.c_str());
            }
            
            /**
             * @brief Retrieves comprehensive statistics about the archive.
             * 
             * Collects and returns detailed information about the archive including
             * total number of entries, uncompressed and compressed sizes, and calculated
             * compression efficiency ratio. This information is useful for monitoring
             * archive performance and storage optimization.
             * 
             * @param stats Reference to structure that will receive the statistics data
             * @return True if statistics were successfully retrieved, false on error
             * 
             * @note The archive must be in a readable state
             * @note Statistics reflect the current state at the time of the call
             * @see fossil_io_archive_stats_t, fossil_io_archive_get_stats()
             */
            bool get_stats(fossil_io_archive_stats_t& stats) const {
                return fossil_io_archive_get_stats(handle, &stats);
            }
            
            /**
             * @brief Lists all entries in the archive as an STL vector.
             * 
             * Enumerates all files and directories contained within the archive and
             * returns their metadata as a convenient STL vector. Each entry includes
             * comprehensive information such as name, size, timestamps, permissions,
             * and compression details without extracting the actual file content.
             * 
             * @return Vector of archive entry structures, empty on error
             * 
             * @note Memory management is handled automatically by the vector
             * @note Entry paths use forward slashes regardless of platform
             * @note Large archives may require significant memory for the entry list
             * @see fossil_io_archive_entry_t, fossil_io_archive_list()
             */
            std::vector<fossil_io_archive_entry_t> list() const {
                fossil_io_archive_entry_t* entries;
                ssize_t count = fossil_io_archive_list(handle, &entries);
                
                std::vector<fossil_io_archive_entry_t> result;
                if (count > 0) {
                    result.assign(entries, entries + count);
                    fossil_io_archive_free_entries(entries, count);
                }
                return result;
            }
            
            /**
             * @brief Extracts a single file from the archive to the file system.
             * 
             * Retrieves and decompresses a specific file from the archive, writing it
             * to the specified destination path on the file system. The operation
             * preserves file attributes when possible and automatically creates parent
             * directories as needed for the destination path.
             * 
             * @param entry_name The path/name of the file within the archive to extract
             * @param dest_path The destination file system path for the extracted file
             * @return True if extraction completed successfully, false on error
             * 
             * @note Existing files at dest_path will be overwritten without warning
             * @note File permissions and timestamps are preserved when supported
             * @see fossil_io_archive_extract_file()
             */
            bool extract_file(const std::string& entry_name, const std::string& dest_path) const {
                return fossil_io_archive_extract_file(handle, entry_name.c_str(), dest_path.c_str());
            }
            
            /**
             * @brief Extracts all files and directories from the archive.
             * 
             * Performs a complete extraction of the entire archive contents to the
             * specified destination directory. This operation recreates the directory
             * structure as stored in the archive and processes all files while
             * preserving attributes, permissions, and timestamps when possible.
             * 
             * @param dest_dir The destination directory for extracting all archive contents
             * @return True if all files were extracted successfully, false on any error
             * 
             * @note The destination directory will be created if it doesn't exist
             * @note Existing files may be overwritten during extraction
             * @note The operation stops immediately on the first error encountered
             * @see fossil_io_archive_extract_all()
             */
            bool extract_all(const std::string& dest_dir) const {
                return fossil_io_archive_extract_all(handle, dest_dir.c_str());
            }
            
            /**
             * @brief Adds a file from the file system to the archive.
             * 
             * Reads a file from the local file system and adds it to the archive with
             * the specified internal path name. The file is compressed according to
             * the archive's compression settings and metadata such as timestamps and
             * permissions are preserved in the archive entry.
             * 
             * @param src_dir The source file path on the local file system
             * @param archive_path The path name to use for this file inside the archive
             * @return True if the file was successfully added, false on error
             * 
             * @note The source file must exist and be readable
             * @note Archive paths should use forward slashes for cross-platform compatibility
             * @note The archive must be opened in write or append mode
             * @see fossil_io_archive_add_file()
             */
            bool add_file(const std::string& src_dir, const std::string& archive_path) const {
                return fossil_io_archive_add_file(handle, src_dir.c_str(), archive_path.c_str());
            }
            
            /**
             * @brief Recursively adds a directory and all its contents to the archive.
             * 
             * Traverses a directory tree on the file system and adds all files and
             * subdirectories to the archive while preserving the directory structure.
             * The operation processes all nested files and creates appropriate directory
             * entries within the archive to maintain the original hierarchy.
             * 
             * @param src_dir The source directory path on the local file system
             * @param archive_dir The base directory path to use inside the archive
             * @return True if all files were successfully added, false on any error
             * 
             * @note Symbolic links are followed and their targets are archived
             * @note Hidden files and directories are included in the operation
             * @note The operation stops on the first error encountered
             * @see fossil_io_archive_add_directory()
             */
            bool add_directory(const std::string& src_dir, const std::string& archive_dir) const {
                return fossil_io_archive_add_directory(handle, src_dir.c_str(), archive_dir.c_str());
            }
            
            /**
             * @brief Removes a specific entry from the archive.
             * 
             * Deletes a file or directory entry from the archive by marking it for
             * removal or rebuilding the archive without the specified entry, depending
             * on the archive format's capabilities. Some formats may require complete
             * reconstruction of the archive file for this operation.
             * 
             * @param entry_name The path/name of the entry to remove from the archive
             * @return True if the entry was successfully removed, false on error
             * 
             * @note The archive must be opened in write or append mode
             * @note Some archive formats require rewriting the entire archive
             * @note Attempting to remove non-existent entries returns false
             * @see fossil_io_archive_remove()
             */
            bool remove(const std::string& entry_name) const {
                return fossil_io_archive_remove(handle, entry_name.c_str());
            }
            
            /**
             * @brief Checks if a specific entry exists in the archive.
             * 
             * Performs a quick lookup in the archive's directory structure to determine
             * whether a file or directory with the given name exists. This operation
             * is efficient as it only searches metadata without reading or extracting
             * any file content from the archive.
             * 
             * @param entry_name The path/name to search for within the archive
             * @return True if the entry exists, false if not found or on error
             * 
             * @note The search is case-sensitive and uses exact string matching
             * @note Directory entries should include their full path for accurate results
             * @note The archive must be in a readable state
             * @see fossil_io_archive_exists()
             */
            bool exists(const std::string& entry_name) const {
                return fossil_io_archive_exists(handle, entry_name.c_str());
            }
            
            /**
             * @brief Retrieves the uncompressed size of an archive entry.
             * 
             * Returns the original file size in bytes for a specific entry within
             * the archive. This information is read from the archive metadata without
             * extracting or decompressing the file, making it efficient for size
             * calculations and storage planning.
             * 
             * @param entry_name The path/name of the entry to query for size information
             * @return The uncompressed size in bytes, or -1 on error or if not found
             * 
             * @note Returns original file size, not the compressed size in the archive
             * @note Directory entries typically return 0 for their size
             * @note The archive must be in a readable state
             * @see fossil_io_archive_entry_size()
             */
            ssize_t entry_size(const std::string& entry_name) const {
                return fossil_io_archive_entry_size(handle, entry_name.c_str());
            }
            
            /**
             * @brief Prints a formatted listing of all archive contents to stdout.
             * 
             * Generates a comprehensive, human-readable table showing all entries
             * in the archive with their names, sizes, modification dates, permissions,
             * and other relevant metadata. The output format is similar to command-line
             * archive utilities and is suitable for terminal display and logging.
             * 
             * @note Output is sent directly to stdout with formatted columns
             * @note Large archives may produce extensive output
             * @note The archive must be in a readable state
             * @see fossil_io_archive_print()
             */
            void print() const {
                fossil_io_archive_print(handle);
            }
            
            /**
             * @brief Checks if the archive handle is valid and ready for operations.
             * 
             * Verifies that the internal archive handle was successfully initialized
             * and is available for archive operations. This method should be called
             * after construction or creation to ensure the archive was opened correctly
             * before attempting any file operations.
             * 
             * @return True if the archive is valid and operational, false otherwise
             * 
             * @note A valid handle doesn't guarantee the archive file is not corrupted
             * @note This check should be performed before any archive operations
             * @note Invalid archives should not be used for any operations
             */
            bool is_valid() const {
                return handle != nullptr;
            }
        };

    }

}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
