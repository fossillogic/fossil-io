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
#ifndef FOSSIL_IO_FILE_H
#define FOSSIL_IO_FILE_H

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <fcntl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Structure representing a file stream with extended metadata and AI processing support.
 *
 * This structure encapsulates all relevant information and state required to manage a file stream
 * in the Fossil Logic framework. It includes standard file stream and descriptor members, as well as
 * extended metadata (such as filename, type, permissions, ownership, and timestamps), state flags,
 * buffering, and advanced fields for AI-driven content analysis (including language detection,
 * sentiment, tags, and embeddings).
 *
 * The structure is designed to support high-performance, cross-platform file operations, and to
 * enable intelligent processing and indexing of file content for advanced applications.
 *
 * Members:
 *   - FILE *file: Pointer to the standard C FILE structure for high-level stream operations.
 *   - int fd: File descriptor for low-level POSIX operations.
 *   - char filename[500]: Buffer for the file's name or path.
 *   - char type[32]: String describing the file type (e.g., "text", "binary").
 *   - int32_t mode: POSIX-style permission bits (e.g., 0644).
 *   - char owner[128]: Username of the file's owner (optional).
 *   - char group[128]: Group name of the file's owner (optional).
 *   - int flags: Flags used when opening the file (e.g., O_RDONLY, O_WRONLY).
 *   - bool is_open: Indicates if the file is currently open.
 *   - bool readable, writable, executable: Access permission flags.
 *   - bool append_mode: True if file is opened in append mode.
 *   - bool temporary: True if the file is temporary (auto-delete on close).
 *   - bool locked: True if the file is locked for writing.
 *   - size_t size: Cached file size, if known.
 *   - int64_t position: Current file position.
 *   - void *buffer: Optional pointer to a custom buffer for read/write caching.
 *   - size_t buffer_size: Size of the custom buffer.
 *   - time_t created_at, modified_at, accessed_at: File timestamps.
 *   - bool indexed: True if the file content has been indexed for search/analysis.
 *   - bool analyzed: True if content has been processed by AI (e.g., NLP, classification).
 *   - char language[32]: Detected language of the text content (if applicable).
 *   - float sentiment: Sentiment score (-1.0 to 1.0) for text content.
 *   - char *tags[16]: Array of AI-generated tags or categories for the file.
 *   - int tag_count: Number of tags generated.
 *   - bool compressed: True if the file is AI-optimized for storage (e.g., vector embeddings).
 *   - void *embedding: Pointer to an AI embedding vector (if processed).
 *   - size_t embedding_size: Size of the embedding vector.
 *   - bool is_binary: True if AI processing detected non-text content.
 */
typedef struct {
    // Core file stream and descriptor
    FILE *file;             // Pointer to the FILE structure for the stream
    int fd;                 // File descriptor for low-level operations

    // File identification and metadata
    char filename[500];     // Array to store the filename
    char type[32];          // Type of the file (e.g., "text", "binary")
    int32_t mode;           // POSIX-style permission bits (e.g., 0644)
    char owner[128];        // Owner username (optional)
    char group[128];        // Group name (optional)

    // File state and flags
    int flags;              // Flags used when opening the file
    bool is_open;           // Indicates if the file is currently open
    bool readable;          // True if the file is readable
    bool writable;          // True if the file is writable
    bool executable;        // True if the file is executable
    bool append_mode;       // True if file is opened in append mode
    bool temporary;         // True if the file is temporary (auto-delete on close)
    bool locked;            // True if the file is locked for writing

    // File size and position
    size_t size;            // Cached file size, if known
    int64_t position;       // Current file position

    // Buffering
    void *buffer;           // Optional buffer for custom read/write caching
    size_t buffer_size;     // Size of the buffer

    // Timestamps
    time_t created_at;      // Timestamp when the file was created
    time_t modified_at;     // Timestamp of last modification
    time_t accessed_at;     // Timestamp of last access

    // AI / intelligent processing members
    bool indexed;           // True if the file content has been indexed for search/analysis
    bool analyzed;          // True if content has been processed by AI (e.g., NLP, classification)
    char language[32];      // Detected language of the text content (if applicable)
    float sentiment;        // Sentiment score (-1.0 to 1.0) for text content
    char *tags[16];         // AI-generated tags or categories for the file
    int tag_count;          // Number of tags generated
    bool compressed;        // True if the file is AI-optimized for storage (e.g., vector embeddings)
    void *embedding;        // Pointer to an AI embedding vector (if processed)
    size_t embedding_size;  // Size of the embedding vector
    bool is_binary;         // True if AI processing detected non-text content
} fossil_io_file_t;

typedef fpos_t fossil_io_pos_t;

extern fossil_io_file_t *FOSSIL_STDIN;
extern fossil_io_file_t *FOSSIL_STDOUT;
extern fossil_io_file_t *FOSSIL_STDERR;

#define FOSSIL_STDIN  (FOSSIL_STDIN)
#define FOSSIL_STDOUT (FOSSIL_STDOUT)
#define FOSSIL_STDERR (FOSSIL_STDERR)

/**
 * Reopen a stream with a new file.
 *
 * This function reopens a stream with a new file.
 *
 * @param stream   Pointer to the fossil_io_file_t structure to reopen.
 * @param filename The name of the file to reopen.
 * @param mode     The mode in which to reopen the file.
 * @param file     Pointer to the FILE structure to reopen.
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_io_file_freopen(fossil_io_file_t *stream, const char *filename, const char *mode, FILE *file);

/**
 * Open a stream for file operations.
 *
 * This function opens a file stream, allowing read or write operations on the specified file.
 *
 * @param stream   Pointer to the fossil_io_file_t structure to store the opened stream.
 * @param filename The name of the file to be opened.
 * @param mode     The mode in which to open the file (e.g., "r" for read, "w" for write).
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_io_file_open(fossil_io_file_t *stream, const char *filename, const char *mode);

/**
 * Close an open stream.
 *
 * This function closes a previously opened stream, releasing associated resources.
 *
 * @param stream Pointer to the fossil_io_file_t structure to be closed.
 */
void fossil_io_file_close(fossil_io_file_t *stream);

/**
 * Redirect a stream to /dev/null.
 *
 * This function redirects the given stream to /dev/null, effectively discarding any data written to it.
 *
 * @param stream Pointer to the fossil_io_file_t structure to redirect.
 * @return       0 on success, non-zero on failure.
 */
int32_t fossil_io_file_redirect_to_devnull(fossil_io_file_t *stream);

/**
 * Check if a stream is open.
 *
 * This function checks if a stream is open.
 *
 * @param stream Pointer to the fossil_io_file_t structure to check.
 * @return       1 if the stream is open, 0 if not.
 */
int32_t fossil_io_file_is_open(const fossil_io_file_t *stream);

/**
 * Read data from an open stream.
 *
 * This function reads data from an open stream into a buffer.
 *
 * @param stream Pointer to the fossil_io_file_t structure from which to read.
 * @param buffer Pointer to the buffer to store the read data.
 * @param size   Size of each element to be read.
 * @param count  Number of elements to read.
 * @return       The total number of elements successfully read.
 */
size_t fossil_io_file_read(fossil_io_file_t *stream, void *buffer, size_t size, size_t count);

/**
 * Write data to an open stream.
 *
 * This function writes data from a buffer to an open stream.
 *
 * @param stream Pointer to the fossil_io_file_t structure to which to write.
 * @param buffer Pointer to the buffer containing the data to be written.
 * @param size   Size of each element to be written.
 * @param count  Number of elements to write.
 * @return       The total number of elements successfully written.
 */
size_t fossil_io_file_write(fossil_io_file_t *stream, const void *buffer, size_t size, size_t count);

/**
 * Append data to the end of an open stream.
 *
 * This function appends data from a buffer to the end of an open stream.
 *
 * @param stream Pointer to the fossil_io_file_t structure to which to append.
 * @param buffer Pointer to the buffer containing the data to be appended.
 * @param size   Size of each element to be appended.
 * @param count  Number of elements to append.
 * @return       0 on success, non-zero on failure.
 */
int32_t fossil_io_file_append(fossil_io_file_t *stream, const void *buffer, size_t size, int32_t count);

/**
 * Seek to a specified position in an open stream.
 *
 * This function moves the file pointer associated with the stream to a new position.
 *
 * @param stream Pointer to the fossil_io_file_t structure to seek.
 * @param offset The offset from the specified origin.
 * @param origin The starting position for the offset (SEEK_SET, SEEK_CUR, SEEK_END).
 * @return       0 on success, non-zero on failure.
 */
int32_t fossil_io_file_seek(fossil_io_file_t *stream, int64_t offset, int32_t origin);

/**
 * Get the current position of the file pointer in an open stream.
 *
 * This function retrieves the current position of the file pointer in an open stream.
 *
 * @param stream Pointer to the fossil_io_file_t structure to get the position of.
 * @return       The current position of the file pointer.
 */
int32_t fossil_io_file_tell(fossil_io_file_t *stream);

/**
 * Save an open stream to a new file.
 *
 * This function saves the contents of an open stream to a new file.
 *
 * @param stream       Pointer to the fossil_io_file_t structure to be saved.
 * @param new_filename The name of the new file to save to.
 * @return             0 on success, non-zero on failure.
 */
int32_t fossil_io_file_save(fossil_io_file_t *stream, const char *new_filename);

/**
 * Copy a file from the source to the destination.
 *
 * This function copies a file from a source file to a destination file.
 *
 * @param source_filename      The name of the source file.
 * @param destination_filename The name of the destination file.
 * @return                     0 on success, non-zero on failure.
 */
int32_t fossil_io_file_copy(const char *source_filename, const char *destination_filename);

/**
 * Remove a file stream.
 *
 * This function removes a file stream.
 *
 * @param filename The name of the file to remove.
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_io_file_remove(const char *filename);

/**
 * Rename a file or directory.
 *
 * This function renames a file or directory.
 *
 * @param old_filename The current name of the file or directory.
 * @param new_filename The new name to assign to the file or directory.
 * @return             0 on success, non-zero on failure.
 */
int32_t fossil_io_file_rename(const char *old_filename, const char *new_filename);

/**
 * Flush the contents of an open stream.
 *
 * This function flushes the contents of an open stream.
 *
 * @param stream Pointer to the fossil_io_file_t structure to flush.
 * @return       0 on success, non-zero on failure.
 */
int32_t fossil_io_file_flush(fossil_io_file_t *stream);

/**
 * Seek to the beginning of an open stream.
 *
 * This function moves the file pointer associated with the stream to the beginning.
 *
 * @param stream Pointer to the fossil_io_file_t structure to seek.
 * @return       0 on success, non-zero on failure.
 */
int32_t fossil_io_file_setpos(fossil_io_file_t *stream, const fossil_io_pos_t *pos);

/**
 * Get the current position of the file pointer in an open stream.
 *
 * This function retrieves the current position of the file pointer in an open stream.
 *
 * @param stream Pointer to the fossil_io_file_t structure to get the position of.
 * @param pos    Pointer to store the current position of the file pointer.
 * @return       0 on success, non-zero on failure.
 */
int32_t fossil_io_file_getpos(fossil_io_file_t *stream, int64_t *pos);

/**
 * Rotate a file stream.
 *
 * This function rotates a file stream.
 * 
 * @param filename The name of the file to rotate.
 * @param n        The number of rotations to perform.
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_io_file_rotate(const char *filename, int32_t n);

/**
 * Create a backup of a file with a specified backup suffix.
 *
 * This function creates a backup of a file with the given suffix.
 *
 * @param filename      The name of the file to create a backup for.
 * @param backup_suffix The suffix to be appended to the backup file.
 * @return              0 on success, non-zero on failure.
 */
int32_t fossil_io_file_backup(const char *filename, const char *backup_suffix);

/**
 * Check if a file exists.
 *
 * This function checks if a file exists.
 *
 * @param filename The name of the file to check for existence.
 * @return         1 if the file exists, 0 if not.
 */
int32_t fossil_io_file_file_exists(const char *filename);

/**
 * Get the size of an open stream.
 *
 * This function retrieves the size of an open stream.
 *
 * @param stream Pointer to the fossil_io_file_t structure to get the size of.
 * @return       The size of the open stream.
 */
int32_t fossil_io_file_get_size(fossil_io_file_t *stream);

/**
 * Delete a file.
 *
 * This function deletes a file.
 *
 * @param filename The name of the file to be deleted.
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_io_file_delete(const char *filename);

/**
 * Get the type of a file stream.
 *
 * This function retrieves the type of a file stream.
 *
 * @param filename The name of the file to get the type of.
 * @return         The type of the file stream.
 */
int fossil_io_file_get_type(const char *filename);

/**
 * Create a temporary file.
 *
 * This function creates a temporary file and returns its name.
 *
 * @return A pointer to the name of the temporary file, or NULL on failure.
 */
fossil_io_file_t fossil_io_file_tempfile(void);

/**
 * Check if a file is readable.
 *
 * This function checks if a file has read permissions.
 *
 * @param filename The name of the file to check.
 * @return         1 if readable, 0 otherwise.
 */
int32_t fossil_io_file_is_readable(const char *filename);

/**
 * Check if a file is writable.
 *
 * This function checks if a file has write permissions.
 *
 * @param filename The name of the file to check.
 * @return         1 if writable, 0 otherwise.
 */
int32_t fossil_io_file_is_writable(const char *filename);

/**
 * Check if a file is executable.
 *
 * This function checks if a file has execute permissions.
 *
 * @param filename The name of the file to check.
 * @return         1 if executable, 0 otherwise.
 */
int32_t fossil_io_file_is_executable(const char *filename);

/**
 * Set file permissions.
 *
 * This function sets the permissions for a file.
 *
 * @param filename The name of the file to set permissions for.
 * @param mode     The permissions to set (POSIX: chmod-style).
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_io_file_set_permissions(const char *filename, int32_t mode);

/**
 * Get file permissions.
 *
 * This function retrieves the permissions of a file.
 *
 * @param filename The name of the file to retrieve permissions for.
 * @param mode     Pointer to store the retrieved permissions (POSIX style).
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_io_file_get_permissions(const char *filename, int32_t *mode);

/**
 * @brief Rewinds the file stream to the beginning.
 *
 * Resets the position of the file pointer in the given `fossil_io_file_t`
 * structure to the beginning of the file, if the stream and file are valid.
 *
 * @param stream Pointer to a `fossil_io_file_t` structure representing the file stream.
 */
void fossil_io_file_rewind(fossil_io_file_t *stream);

/**
 * @brief Analyze file content using AI (text classification, sentiment, language detection).
 *
 * This function performs AI-driven analysis on the file content, including text classification,
 * sentiment analysis, and language detection. Results are stored in the fossil_io_file_t structure.
 *
 * @param f Pointer to the fossil_io_file_t structure representing the file.
 * @return 0 on success, non-zero on failure.
 */
int fossil_io_file_ai_analyze(fossil_io_file_t *f);

/**
 * @brief Compute and store embeddings for semantic search.
 *
 * This function computes AI embedding vectors for the file content using the provided model,
 * storing the result in the fossil_io_file_t structure for semantic search and analysis.
 *
 * @param f         Pointer to the fossil_io_file_t structure representing the file.
 * @param model     Pointer to the AI model used for embedding computation.
 * @param model_size Size of the AI model.
 * @return 0 on success, non-zero on failure.
 */
int fossil_io_file_ai_compute_embedding(fossil_io_file_t *f, const void *model, size_t model_size);

/**
 * @brief Check if file is ready for AI processing.
 *
 * This function checks if the file is in a valid state for AI processing (e.g., open, readable).
 *
 * @param f Pointer to the fossil_io_file_t structure representing the file.
 * @return true if ready, false otherwise.
 */
bool fossil_io_file_ai_ready(fossil_io_file_t *f);

/**
 * @brief Clear AI metadata (tags, embeddings, analysis state).
 *
 * This function resets AI-related metadata in the fossil_io_file_t structure, including tags,
 * embeddings, and analysis state.
 *
 * @param f Pointer to the fossil_io_file_t structure representing the file.
 */
void fossil_io_file_ai_reset(fossil_io_file_t *f);

/**
 * @brief Add a tag to the file (up to 16).
 *
 * This function adds a new AI-generated tag to the fossil_io_file_t structure, up to a maximum of 16 tags.
 *
 * @param f   Pointer to the fossil_io_file_t structure representing the file.
 * @param tag Pointer to the tag string to add.
 * @return 0 on success, non-zero on failure.
 */
int fossil_io_file_add_tag(fossil_io_file_t *f, const char *tag);

/**
 * @brief Detect if file is binary or text.
 *
 * This function analyzes the file content to determine if it is binary or text, updating the is_binary flag.
 *
 * @param f Pointer to the fossil_io_file_t structure representing the file.
 */
void fossil_io_file_detect_binary(fossil_io_file_t *f);

/**
 * @brief Compress file content for AI storage/embedding.
 *
 * This function compresses the file content for optimized AI storage or embedding, updating the compressed flag.
 *
 * @param f Pointer to the fossil_io_file_t structure representing the file.
 * @return 0 on success, non-zero on failure.
 */
int fossil_io_file_compress(fossil_io_file_t *f);

/**
 * @brief Decompress AI-compressed content.
 *
 * This function decompresses AI-compressed file content, restoring the original data in the fossil_io_file_t structure.
 *
 * @param f Pointer to the fossil_io_file_t structure representing the file.
 * @return 0 on success, non-zero on failure.
 */
int fossil_io_file_decompress(fossil_io_file_t *f);

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
int fossil_io_file_link(
        const fossil_io_file_t *src,
        fossil_io_file_t *dest,
        const char *dest_path,
        bool symbolic,
        bool copy_meta);

#ifdef __cplusplus
}
#include <string>

/**
 * C++ API namespace for Fossil Logic source code.
 */
namespace fossil {
    /**
     * IO namespace for io related code.
     */
    namespace io {

        /**
         * Class for file stream functions.
         */
        class Stream {
        public:

            /**
             * Reopen a stream with a new file.
             *
             * This function reopens a stream with a new file.
             *
             * @param stream   Pointer to the fossil_io_file_t structure to reopen.
             * @param filename The name of the file to reopen.
             * @param mode     The mode in which to reopen the file.
             * @param file     Pointer to the FILE structure to reopen.
             * @return         0 on success, non-zero on failure.
             */
            static int32_t freopen(fossil_io_file_t *stream, const char *filename, const char *mode, FILE *file) {
                return fossil_io_file_freopen(stream, filename, mode, file);
            }

            /**
             * Reopen a stream with a new file.
             *
             * This function reopens a stream with a new file.
             *
             * @param stream   Pointer to the fossil_io_file_t structure to reopen.
             * @param filename The name of the file to reopen.
             * @param mode     The mode in which to reopen the file.
             * @param file     Pointer to the FILE structure to reopen.
             * @return         0 on success, non-zero on failure.
             */
            static int32_t freopen(fossil_io_file_t *stream, const std::string &filename, const std::string &mode, FILE *file) {
                return fossil_io_file_freopen(stream, filename.c_str(), mode.c_str(), file);
            }

            /**
             * Open a stream for file operations.
             *
             * This function opens a file stream, allowing read or write operations on the specified file.
             *
             * @param stream   Pointer to the fossil_io_file_t structure to store the opened stream.
             * @param filename The name of the file to be opened.
             * @param mode     The mode in which to open the file (e.g., "r" for read, "w" for write).
             * @return         0 on success, non-zero on failure.
             */
            static int32_t open(fossil_io_file_t *stream, const char *filename, const char *mode) {
                return fossil_io_file_open(stream, filename, mode);
            }

            /**
             * Open a stream for file operations.
             *
             * This function opens a file stream, allowing read or write operations on the specified file.
             *
             * @param stream   Pointer to the fossil_io_file_t structure to store the opened stream.
             * @param filename The name of the file to be opened.
             * @param mode     The mode in which to open the file (e.g., "r" for read, "w" for write).
             * @return         0 on success, non-zero on failure.
             */
            static int32_t open(fossil_io_file_t *stream, const std::string &filename, const std::string &mode) {
                return fossil_io_file_open(stream, filename.c_str(), mode.c_str());
            }

            /**
             * Close an open stream.
             *
             * This function closes a previously opened stream, releasing associated resources.
             *
             * @param stream Pointer to the fossil_io_file_t structure to be closed.
             */
            static void close(fossil_io_file_t *stream) {
                fossil_io_file_close(stream);
            }

            /**
             * Redirect a stream to /dev/null.
             *
             * This function redirects the given stream to /dev/null, effectively discarding any data written to it.
             *
             * @param stream Pointer to the fossil_io_file_t structure to redirect.
             * @return       0 on success, non-zero on failure.
             */
            static int32_t redirect_to_devnull(fossil_io_file_t *stream) {
                return fossil_io_file_redirect_to_devnull(stream);
            }

            /**
             * Check if a stream is open.
             *
             * This function checks if a stream is open.
             *
             * @param stream Pointer to the fossil_io_file_t structure to check.
             * @return       1 if the stream is open, 0 if not.
             */
            static int32_t is_open(const fossil_io_file_t *stream) {
                return fossil_io_file_is_open(stream);
            }

            /**
             * Read data from an open stream.
             *
             * This function reads data from an open stream into a buffer.
             *
             * @param stream Pointer to the fossil_io_file_t structure from which to read.
             * @param buffer Pointer to the buffer to store the read data.
             * @param size   Size of each element to be read.
             * @param count  Number of elements to read.
             * @return       The total number of elements successfully read.
             */
            static size_t read(fossil_io_file_t *stream, void *buffer, size_t size, size_t count) {
                return fossil_io_file_read(stream, buffer, size, count);
            }

            /**
             * Write data to an open stream.
             *
             * This function writes data from a buffer to an open stream.
             *
             * @param stream Pointer to the fossil_io_file_t structure to which to write.
             * @param buffer Pointer to the buffer containing the data to be written.
             * @param size   Size of each element to be written.
             * @param count  Number of elements to write.
             * @return       The total number of elements successfully written.
             */
            static size_t write(fossil_io_file_t *stream, const void *buffer, size_t size, size_t count) {
                return fossil_io_file_write(stream, buffer, size, count);
            }

            /**
             * Append data to the end of an open stream.
             *
             * This function appends data from a buffer to the end of an open stream.
             *
             * @param stream Pointer to the fossil_io_file_t structure to which to append.
             * @param buffer Pointer to the buffer containing the data to be appended.
             * @param size   Size of each element to be appended.
             * @param count  Number of elements to append.
             * @return       0 on success, non-zero on failure.
             */
            static int32_t append(fossil_io_file_t *stream, const void *buffer, size_t size, int32_t count) {
                return fossil_io_file_append(stream, buffer, size, count);
            }

            /**
             * Seek to a specified position in an open stream.
             *
             * This function moves the file pointer associated with the stream to a new position.
             *
             * @param stream Pointer to the fossil_io_file_t structure to seek.
             * @param offset The offset from the specified origin.
             * @param origin The starting position for the offset (SEEK_SET, SEEK_CUR, SEEK_END).
             * @return       0 on success, non-zero on failure.
             */
            static int32_t seek(fossil_io_file_t *stream, int64_t offset, int32_t origin) {
                return fossil_io_file_seek(stream, offset, origin);
            }

            /**
             * Get the current position of the file pointer in an open stream.
             *
             * This function retrieves the current position of the file pointer in an open stream.
             *
             * @param stream Pointer to the fossil_io_file_t structure to get the position of.
             * @return       The current position of the file pointer.
             */
            static int32_t tell(fossil_io_file_t *stream) {
                return fossil_io_file_tell(stream);
            }

            /**
             * Save an open stream to a new file.
             *
             * This function saves the contents of an open stream to a new file.
             *
             * @param stream       Pointer to the fossil_io_file_t structure to be saved.
             * @param new_filename The name of the new file to save to.
             * @return             0 on success, non-zero on failure.
             */
            static int32_t save(fossil_io_file_t *stream, const char *new_filename) {
                return fossil_io_file_save(stream, new_filename);
            }

            /**
             * Save an open stream to a new file.
             *
             * This function saves the contents of an open stream to a new file.
             *
             * @param stream       Pointer to the fossil_io_file_t structure to be saved.
             * @param new_filename The name of the new file to save to.
             * @return             0 on success, non-zero on failure.
             */
            static int32_t save(fossil_io_file_t *stream, const std::string &new_filename) {
                return fossil_io_file_save(stream, new_filename.c_str());
            }

            /**
             * Copy a file from the source to the destination.
             *
             * This function copies a file from a source file to a destination file.
             *
             * @param source_filename      The name of the source file.
             * @param destination_filename The name of the destination file.
             * @return                     0 on success, non-zero on failure.
             */
            static int32_t copy(const char *source_filename, const char *destination_filename) {
                return fossil_io_file_copy(source_filename, destination_filename);
            }

            /**
             * Copy a file from the source to the destination.
             *
             * This function copies a file from a source file to a destination file.
             *
             * @param source_filename      The name of the source file.
             * @param destination_filename The name of the destination file.
             * @return                     0 on success, non-zero on failure.
             */
            static int32_t copy(const std::string &source_filename, const std::string &destination_filename) {
                return fossil_io_file_copy(source_filename.c_str(), destination_filename.c_str());
            }

            /**
             * Remove a file stream.
             *
             * This function removes a file stream.
             *
             * @param filename The name of the file to remove.
             * @return         0 on success, non-zero on failure.
             */
            static int32_t remove(const char *filename) {
                return fossil_io_file_remove(filename);
            }

            /**
             * Remove a file stream.
             *
             * This function removes a file stream.
             *
             * @param filename The name of the file to remove.
             * @return         0 on success, non-zero on failure.
             */
            static int32_t remove(const std::string &filename) {
                return fossil_io_file_remove(filename.c_str());
            }

            /**
             * Rename a file or directory.
             *
             * This function renames a file or directory.
             *
             * @param old_filename The current name of the file or directory.
             * @param new_filename The new name to assign to the file or directory.
             * @return             0 on success, non-zero on failure.
             */
            static int32_t rename(const char *old_filename, const char *new_filename) {
                return fossil_io_file_rename(old_filename, new_filename);
            }

            /**
             * Rename a file or directory.
             *
             * This function renames a file or directory.
             *
             * @param old_filename The current name of the file or directory.
             * @param new_filename The new name to assign to the file or directory.
             * @return             0 on success, non-zero on failure.
             */
            static int32_t rename(const std::string &old_filename, const std::string &new_filename) {
                return fossil_io_file_rename(old_filename.c_str(), new_filename.c_str());
            }

            /**
             * Flush the contents of an open stream.
             *
             * This function flushes the contents of an open stream.
             *
             * @param stream Pointer to the fossil_io_file_t structure to flush.
             * @return       0 on success, non-zero on failure.
             */
            static int32_t flush(fossil_io_file_t *stream) {
                return fossil_io_file_flush(stream);
            }

            /**
             * Seek to the beginning of an open stream.
             *
             * This function moves the file pointer associated with the stream to the beginning.
             *
             * @param stream Pointer to the fossil_io_file_t structure to seek.
             * @return       0 on success, non-zero on failure.
             */
            static int32_t setpos(fossil_io_file_t *stream, const fossil_io_pos_t *pos) {
                return fossil_io_file_setpos(stream, pos);
            }

            /**
             * Get the current position of the file pointer in an open stream.
             *
             * This function retrieves the current position of the file pointer in an open stream.
             *
             * @param stream Pointer to the fossil_io_file_t structure to get the position of.
             * @param pos    Pointer to store the current position of the file pointer.
             * @return       0 on success, non-zero on failure.
             */
            static int32_t getpos(fossil_io_file_t *stream, int64_t *pos) {
                return fossil_io_file_getpos(stream, pos);
            }

            /**
             * Rotate a file stream.
             *
             * This function rotates a file stream.
             * 
             * @param filename The name of the file to rotate.
             * @param n        The number of rotations to perform.
             * @return         0 on success, non-zero on failure.
             */
            static int32_t rotate(const char *filename, int32_t n) {
                return fossil_io_file_rotate(filename, n);
            }

            /**
             * Rotate a file stream.
             *
             * This function rotates a file stream.
             * 
             * @param filename The name of the file to rotate.
             * @param n        The number of rotations to perform.
             * @return         0 on success, non-zero on failure.
             */
            static int32_t rotate(const std::string &filename, int32_t n) {
                return fossil_io_file_rotate(filename.c_str(), n);
            }

            /**
             * Create a backup of a file with a specified backup suffix.
             *
             * This function creates a backup of a file with the given suffix.
             *
             * @param filename      The name of the file to create a backup for.
             * @param backup_suffix The suffix to be appended to the backup file.
             * @return              0 on success, non-zero on failure.
             */
            static int32_t backup(const char *filename, const char *backup_suffix) {
                return fossil_io_file_backup(filename, backup_suffix);
            }

            /**
             * Create a backup of a file with a specified backup suffix.
             *
             * This function creates a backup of a file with the given suffix.
             *
             * @param filename      The name of the file to create a backup for.
             * @param backup_suffix The suffix to be appended to the backup file.
             * @return              0 on success, non-zero on failure.
             */
            static int32_t backup(const std::string &filename, const std::string &backup_suffix) {
                return fossil_io_file_backup(filename.c_str(), backup_suffix.c_str());
            }

            /**
             * Check if a file exists.
             *
             * This function checks if a file exists.
             *
             * @param filename The name of the file to check for existence.
             * @return         1 if the file exists, 0 if not.
             */
            static int32_t file_exists(const char *filename) {
                return fossil_io_file_file_exists(filename);
            }

            /**
             * Check if a file exists.
             *
             * This function checks if a file exists.
             *
             * @param filename The name of the file to check for existence.
             * @return         1 if the file exists, 0 if not.
             */
            static int32_t file_exists(const std::string &filename) {
                return fossil_io_file_file_exists(filename.c_str());
            }

            /**
             * Get the size of an open stream.
             *
             * This function retrieves the size of an open stream.
             *
             * @param stream Pointer to the fossil_io_file_t structure to get the size of.
             * @return       The size of the open stream.
             */
            static int32_t get_size(fossil_io_file_t *stream) {
                return fossil_io_file_get_size(stream);
            }

            /**
             * Delete a file.
             *
             * This function deletes a file.
             *
             * @param filename The name of the file to be deleted.
             * @return         0 on success, non-zero on failure.
             */
            static int32_t delete_file(const char *filename) {
                return fossil_io_file_delete(filename);
            }

            /**
             * Delete a file.
             *
             * This function deletes a file.
             *
             * @param filename The name of the file to be deleted.
             * @return         0 on success, non-zero on failure.
             */
            static int32_t delete_file(const std::string &filename) {
                return fossil_io_file_delete(filename.c_str());
            }

            /**
             * Get the type of a file stream.
             *
             * This function retrieves the type of a file stream.
             *
             * @param filename The name of the file to get the type of.
             * @return         The type of the file stream.
             */
            static int get_type(const char *filename) {
                return fossil_io_file_get_type(filename);
            }

            /**
             * Get the type of a file stream.
             *
             * This function retrieves the type of a file stream.
             *
             * @param filename The name of the file to get the type of.
             * @return         The type of the file stream.
             */
            static int get_type(const std::string &filename) {
                return fossil_io_file_get_type(filename.c_str());
            }

            /**
             * Create a temporary file.
             *
             * This function creates a temporary file and returns its name.
             *
             * @return A pointer to the name of the temporary file, or NULL on failure.
             */
            static fossil_io_file_t tempfile() {
                return fossil_io_file_tempfile();
            }

            /**
             * Check if a file is readable.
             *
             * This function checks if a file has read permissions.
             *
             * @param filename The name of the file to check.
             * @return         1 if readable, 0 otherwise.
             */
            static int32_t is_readable(const char *filename) {
                return fossil_io_file_is_readable(filename);
            }

            /**
             * Check if a file is readable.
             *
             * This function checks if a file has read permissions.
             *
             * @param filename The name of the file to check.
             * @return         1 if readable, 0 otherwise.
             */
            static int32_t is_readable(const std::string &filename) {
                return fossil_io_file_is_readable(filename.c_str());
            }

            /**
             * Check if a file is writable.
             *
             * This function checks if a file has write permissions.
             *
             * @param filename The name of the file to check.
             * @return         1 if writable, 0 otherwise.
             */
            static int32_t is_writable(const char *filename) {
                return fossil_io_file_is_writable(filename);
            }

            /**
             * Check if a file is writable.
             *
             * This function checks if a file has write permissions.
             *
             * @param filename The name of the file to check.
             * @return         1 if writable, 0 otherwise.
             */
            static int32_t is_writable(const std::string &filename) {
                return fossil_io_file_is_writable(filename.c_str());
            }

            /**
             * Check if a file is executable.
             *
             * This function checks if a file has execute permissions.
             *
             * @param filename The name of the file to check.
             * @return         1 if executable, 0 otherwise.
             */
            static int32_t is_executable(const char *filename) {
                return fossil_io_file_is_executable(filename);
            }

            /**
             * Check if a file is executable.
             *
             * This function checks if a file has execute permissions.
             *
             * @param filename The name of the file to check.
             * @return         1 if executable, 0 otherwise.
             */
            static int32_t is_executable(const std::string &filename) {
                return fossil_io_file_is_executable(filename.c_str());
            }

            /**
             * Set file permissions.
             *
             * This function sets the permissions for a file.
             *
             * @param filename The name of the file to set permissions for.
             * @param mode     The permissions to set (POSIX: chmod-style).
             * @return         0 on success, non-zero on failure.
             */
            static int32_t set_permissions(const char *filename, int32_t mode) {
                return fossil_io_file_set_permissions(filename, mode);
            }

            /**
             * Set file permissions.
             *
             * This function sets the permissions for a file.
             *
             * @param filename The name of the file to set permissions for.
             * @param mode     The permissions to set (POSIX: chmod-style).
             * @return         0 on success, non-zero on failure.
             */
            static int32_t set_permissions(const std::string &filename, int32_t mode) {
                return fossil_io_file_set_permissions(filename.c_str(), mode);
            }

            /**
             * Get file permissions.
             *
             * This function retrieves the permissions of a file.
             *
             * @param filename The name of the file to retrieve permissions for.
             * @param mode     Pointer to store the retrieved permissions (POSIX style).
             * @return         0 on success, non-zero on failure.
             */
            static int32_t get_permissions(const char *filename, int32_t *mode) {
                return fossil_io_file_get_permissions(filename, mode);
            }

            /**
             * Get file permissions.
             *
             * This function retrieves the permissions of a file.
             *
             * @param filename The name of the file to retrieve permissions for.
             * @param mode     Pointer to store the retrieved permissions (POSIX style).
             * @return         0 on success, non-zero on failure.
             */
            static int32_t get_permissions(const std::string &filename, int32_t *mode) {
                return fossil_io_file_get_permissions(filename.c_str(), mode);
            }

            /**
             * Analyze file content using AI (text classification, sentiment, language detection).
             *
             * This function performs AI-driven analysis on the file content, including text classification,
             * sentiment analysis, and language detection. Results are stored in the fossil_io_file_t structure.
             *
             * @param f Pointer to the fossil_io_file_t structure representing the file.
             * @return 0 on success, non-zero on failure.
             */
            static int ai_analyze(fossil_io_file_t *f) {
                return fossil_io_file_ai_analyze(f);
            }

            /**
             * Compute and store embeddings for semantic search.
             *
             * This function computes AI embedding vectors for the file content using the provided model,
             * storing the result in the fossil_io_file_t structure for semantic search and analysis.
             *
             * @param f         Pointer to the fossil_io_file_t structure representing the file.
             * @param model     Pointer to the AI model used for embedding computation.
             * @param model_size Size of the AI model.
             * @return 0 on success, non-zero on failure.
             */
            static int ai_compute_embedding(fossil_io_file_t *f, const void *model, size_t model_size) {
                return fossil_io_file_ai_compute_embedding(f, model, model_size);
            }

            /**
             * Check if file is ready for AI processing.
             *
             * This function checks if the file is in a valid state for AI processing (e.g., open, readable).
             *
             * @param f Pointer to the fossil_io_file_t structure representing the file.
             * @return true if ready, false otherwise.
             */
            static bool ai_ready(fossil_io_file_t *f) {
                return fossil_io_file_ai_ready(f);
            }

            /**
             * Clear AI metadata (tags, embeddings, analysis state).
             *
             * This function resets AI-related metadata in the fossil_io_file_t structure, including tags,
             * embeddings, and analysis state.
             *
             * @param f Pointer to the fossil_io_file_t structure representing the file.
             */
            static void ai_reset(fossil_io_file_t *f) {
                fossil_io_file_ai_reset(f);
            }

            /**
             * Add a tag to the file (up to 16).
             *
             * This function adds a new AI-generated tag to the fossil_io_file_t structure, up to a maximum of 16 tags.
             *
             * @param f   Pointer to the fossil_io_file_t structure representing the file.
             * @param tag Pointer to the tag string to add.
             * @return 0 on success, non-zero on failure.
             */
            static int add_tag(fossil_io_file_t *f, const char *tag) {
                return fossil_io_file_add_tag(f, tag);
            }

            /**
             * Detect if file is binary or text.
             *
             * This function analyzes the file content to determine if it is binary or text, updating the is_binary flag.
             *
             * @param f Pointer to the fossil_io_file_t structure representing the file.
             */
            static void detect_binary(fossil_io_file_t *f) {
                fossil_io_file_detect_binary(f);
            }

            /**
             * Compress file content for AI storage/embedding.
             *
             * This function compresses the file content for optimized AI storage or embedding, updating the compressed flag.
             *
             * @param f Pointer to the fossil_io_file_t structure representing the file.
             * @return 0 on success, non-zero on failure.
             */
            static int compress(fossil_io_file_t *f) {
                return fossil_io_file_compress(f);
            }

            /**
             * Decompress AI-compressed content.
             *
             * This function decompresses AI-compressed file content, restoring the original data in the fossil_io_file_t structure.
             *
             * @param f Pointer to the fossil_io_file_t structure representing the file.
             * @return 0 on success, non-zero on failure.
             */
            static int decompress(fossil_io_file_t *f) {
                return fossil_io_file_decompress(f);
            }

        };

    } // namespace io

} // namespace fossil

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
