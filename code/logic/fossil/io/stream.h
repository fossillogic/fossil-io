/*
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * This file is part of the Fossil Logic project, which aims to develop high-
 * performance, cross-platform applications and libraries. The code contained
 * herein is subject to the terms and conditions defined in the project license.
 *
 * Author: Michael Gene Brockus (Dreamer)
 *
 * Copyright (C) 2024 Fossil Logic. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#ifndef FOSSIL_IO_STREAM_H
#define FOSSIL_IO_STREAM_H

#include <stdio.h>
#include <stdint.h>

#define FOSSIL_STDIN  stdin
#define FOSSIL_STDOUT stdout
#define FOSSIL_STDERR stderr

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Structure representing a file stream.
 */
typedef struct {
    FILE *file;                                       // Pointer to the FILE structure for the stream
    char filename[500]; // Array to store the filename
} fossil_fstream_t;

/**
 * Reopen a stream with a new file.
 *
 * This function reopens a stream with a new file.
 *
 * @param stream   Pointer to the fossil_fstream_t structure to reopen.
 * @param filename The name of the file to reopen.
 * @param mode     The mode in which to reopen the file.
 * @param file     Pointer to the FILE structure to reopen.
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_fstream_freopen(fossil_fstream_t *stream, const char *filename, const char *mode, FILE *file);

/**
 * Open a stream for file operations.
 *
 * This function opens a file stream, allowing read or write operations on the specified file.
 *
 * @param stream   Pointer to the fossil_fstream_t structure to store the opened stream.
 * @param filename The name of the file to be opened.
 * @param mode     The mode in which to open the file (e.g., "r" for read, "w" for write).
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_fstream_open(fossil_fstream_t *stream, const char *filename, const char *mode);

/**
 * Close an open stream.
 *
 * This function closes a previously opened stream, releasing associated resources.
 *
 * @param stream Pointer to the fossil_fstream_t structure to be closed.
 */
void fossil_fstream_close(fossil_fstream_t *stream);

/**
 * Check if a stream is open.
 *
 * This function checks if a stream is open.
 *
 * @param stream Pointer to the fossil_fstream_t structure to check.
 * @return       1 if the stream is open, 0 if not.
 */
int32_t fossil_fstream_is_open(const fossil_fstream_t *stream);

/**
 * Read data from an open stream.
 *
 * This function reads data from an open stream into a buffer.
 *
 * @param stream Pointer to the fossil_fstream_t structure from which to read.
 * @param buffer Pointer to the buffer to store the read data.
 * @param size   Size of each element to be read.
 * @param count  Number of elements to read.
 * @return       The total number of elements successfully read.
 */
size_t fossil_fstream_read(fossil_fstream_t *stream, void *buffer, size_t size, size_t count);

/**
 * Write data to an open stream.
 *
 * This function writes data from a buffer to an open stream.
 *
 * @param stream Pointer to the fossil_fstream_t structure to which to write.
 * @param buffer Pointer to the buffer containing the data to be written.
 * @param size   Size of each element to be written.
 * @param count  Number of elements to write.
 * @return       The total number of elements successfully written.
 */
size_t fossil_fstream_write(fossil_fstream_t *stream, const void *buffer, size_t size, size_t count);

/**
 * Append data to the end of an open stream.
 *
 * This function appends data from a buffer to the end of an open stream.
 *
 * @param stream Pointer to the fossil_fstream_t structure to which to append.
 * @param buffer Pointer to the buffer containing the data to be appended.
 * @param size   Size of each element to be appended.
 * @param count  Number of elements to append.
 * @return       0 on success, non-zero on failure.
 */
int32_t fossil_fstream_append(fossil_fstream_t *stream, const void *buffer, size_t size, int32_t count);

/**
 * Seek to a specified position in an open stream.
 *
 * This function moves the file pointer associated with the stream to a new position.
 *
 * @param stream Pointer to the fossil_fstream_t structure to seek.
 * @param offset The offset from the specified origin.
 * @param origin The starting position for the offset (SEEK_SET, SEEK_CUR, SEEK_END).
 * @return       0 on success, non-zero on failure.
 */
int32_t fossil_fstream_seek(fossil_fstream_t *stream, int64_t offset, int32_t origin);

/**
 * Get the current position of the file pointer in an open stream.
 *
 * This function retrieves the current position of the file pointer in an open stream.
 *
 * @param stream Pointer to the fossil_fstream_t structure to get the position of.
 * @return       The current position of the file pointer.
 */
int32_t fossil_fstream_tell(fossil_fstream_t *stream);

/**
 * Save an open stream to a new file.
 *
 * This function saves the contents of an open stream to a new file.
 *
 * @param stream       Pointer to the fossil_fstream_t structure to be saved.
 * @param new_filename The name of the new file to save to.
 * @return             0 on success, non-zero on failure.
 */
int32_t fossil_fstream_save(fossil_fstream_t *stream, const char *new_filename);

/**
 * Copy a file from the source to the destination.
 *
 * This function copies a file from a source file to a destination file.
 *
 * @param source_filename      The name of the source file.
 * @param destination_filename The name of the destination file.
 * @return                     0 on success, non-zero on failure.
 */
int32_t fossil_fstream_copy(const char *source_filename, const char *destination_filename);

/**
 * Move a file stream.
 *
 * This function moves a file stream.
 *
 * @param source_filename      The name of the source file.
 * @param destination_filename The name of the destination file.
 * @return                     0 on success, non-zero on failure.
 */
int32_t fossil_fstream_move(const char *source_filename, const char *destination_filename);

/**
 * Remove a file stream.
 *
 * This function removes a file stream.
 *
 * @param filename The name of the file to remove.
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_fstream_remove(const char *filename);

/**
 * Rename a file or directory.
 *
 * This function renames a file or directory.
 *
 * @param old_filename The current name of the file or directory.
 * @param new_filename The new name to assign to the file or directory.
 * @return             0 on success, non-zero on failure.
 */
int32_t fossil_fstream_rename(const char *old_filename, const char *new_filename);

/**
 * Flush the contents of an open stream.
 *
 * This function flushes the contents of an open stream.
 *
 * @param stream Pointer to the fossil_fstream_t structure to flush.
 * @return       0 on success, non-zero on failure.
 */
int32_t fossil_fstream_flush(fossil_fstream_t *stream);

/**
 * Seek to the beginning of an open stream.
 *
 * This function moves the file pointer associated with the stream to the beginning.
 *
 * @param stream Pointer to the fossil_fstream_t structure to seek.
 * @return       0 on success, non-zero on failure.
 */
int32_t fossil_fstream_setpos(fossil_fstream_t *stream, int32_t pos);

/**
 * Get the current position of the file pointer in an open stream.
 *
 * This function retrieves the current position of the file pointer in an open stream.
 *
 * @param stream Pointer to the fossil_fstream_t structure to get the position of.
 * @param pos    Pointer to store the current position of the file pointer.
 * @return       0 on success, non-zero on failure.
 */
int32_t fossil_fstream_getpos(fossil_fstream_t *stream, int32_t *pos);

/**
 * Rotate a file stream.
 *
 * This function rotates a file stream.
 * 
 * @param filename The name of the file to rotate.
 * @param n        The number of rotations to perform.
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_fstream_rotate(const char *filename, int32_t n);

/**
 * Create a backup of a file with a specified backup suffix.
 *
 * This function creates a backup of a file with the given suffix.
 *
 * @param filename      The name of the file to create a backup for.
 * @param backup_suffix The suffix to be appended to the backup file.
 * @return              0 on success, non-zero on failure.
 */
int32_t fossil_fstream_backup(const char *filename, const char *backup_suffix);

/**
 * Check if a file exists.
 *
 * This function checks if a file exists.
 *
 * @param filename The name of the file to check for existence.
 * @return         1 if the file exists, 0 if not.
 */
int32_t fossil_fstream_file_exists(const char *filename);

/**
 * Get the size of an open stream.
 *
 * This function retrieves the size of an open stream.
 *
 * @param stream Pointer to the fossil_fstream_t structure to get the size of.
 * @return       The size of the open stream.
 */
int32_t fossil_fstream_get_size(fossil_fstream_t *stream);

/**
 * Delete a file.
 *
 * This function deletes a file.
 *
 * @param filename The name of the file to be deleted.
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_fstream_delete(const char *filename);

/**
 * Rename a file or directory.
 *
 * This function renames a file or directory.
 *
 * @param old_filename The current name of the file or directory.
 * @param new_filename The new name to assign to the file or directory.
 * @return             0 on success, non-zero on failure.
 */
int32_t fossil_fstream_rename(const char *old_filename, const char *new_filename);

/**
 * Get the type of a file stream.
 *
 * This function retrieves the type of a file stream.
 *
 * @param filename The name of the file to get the type of.
 * @return         The type of the file stream.
 */
int fossil_fstream_get_type(const char *filename);

/**
 * Check if a file is readable.
 *
 * This function checks if a file has read permissions.
 *
 * @param filename The name of the file to check.
 * @return         1 if readable, 0 otherwise.
 */
int32_t fossil_fstream_is_readable(const char *filename);

/**
 * Check if a file is writable.
 *
 * This function checks if a file has write permissions.
 *
 * @param filename The name of the file to check.
 * @return         1 if writable, 0 otherwise.
 */
int32_t fossil_fstream_is_writable(const char *filename);

/**
 * Check if a file is executable.
 *
 * This function checks if a file has execute permissions.
 *
 * @param filename The name of the file to check.
 * @return         1 if executable, 0 otherwise.
 */
int32_t fossil_fstream_is_executable(const char *filename);

/**
 * Set file permissions.
 *
 * This function sets the permissions for a file.
 *
 * @param filename The name of the file to set permissions for.
 * @param mode     The permissions to set (POSIX: chmod-style).
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_fstream_set_permissions(const char *filename, int32_t mode);

/**
 * Get file permissions.
 *
 * This function retrieves the permissions of a file.
 *
 * @param filename The name of the file to retrieve permissions for.
 * @param mode     Pointer to store the retrieved permissions (POSIX style).
 * @return         0 on success, non-zero on failure.
 */
int32_t fossil_fstream_get_permissions(const char *filename, int32_t *mode);

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
             * @param stream   Pointer to the fossil_fstream_t structure to reopen.
             * @param filename The name of the file to reopen.
             * @param mode     The mode in which to reopen the file.
             * @param file     Pointer to the FILE structure to reopen.
             * @return         0 on success, non-zero on failure.
             */
            static int32_t freopen(fossil_fstream_t *stream, const char *filename, const char *mode, FILE *file) {
                return fossil_fstream_freopen(stream, filename, mode, file);
            }

            /**
             * Open a stream for file operations.
             *
             * This function opens a file stream, allowing read or write operations on the specified file.
             *
             * @param stream   Pointer to the fossil_fstream_t structure to store the opened stream.
             * @param filename The name of the file to be opened.
             * @param mode     The mode in which to open the file (e.g., "r" for read, "w" for write).
             * @return         0 on success, non-zero on failure.
             */
            static int32_t open(fossil_fstream_t *stream, const char *filename, const char *mode) {
                return fossil_fstream_open(stream, filename, mode);
            }

            /**
             * Close an open stream.
             *
             * This function closes a previously opened stream, releasing associated resources.
             *
             * @param stream Pointer to the fossil_fstream_t structure to be closed.
             */
            static void close(fossil_fstream_t *stream) {
                fossil_fstream_close(stream);
            }

            /**
             * Check if a stream is open.
             *
             * This function checks if a stream is open.
             *
             * @param stream Pointer to the fossil_fstream_t structure to check.
             * @return       1 if the stream is open, 0 if not.
             */
            static int32_t is_open(const fossil_fstream_t *stream) {
                return fossil_fstream_is_open(stream);
            }

            /**
             * Read data from an open stream.
             *
             * This function reads data from an open stream into a buffer.
             *
             * @param stream Pointer to the fossil_fstream_t structure from which to read.
             * @param buffer Pointer to the buffer to store the read data.
             * @param size   Size of each element to be read.
             * @param count  Number of elements to read.
             * @return       The total number of elements successfully read.
             */
            static size_t read(fossil_fstream_t *stream, void *buffer, size_t size, size_t count) {
                return fossil_fstream_read(stream, buffer, size, count);
            }

            /**
             * Write data to an open stream.
             *
             * This function writes data from a buffer to an open stream.
             *
             * @param stream Pointer to the fossil_fstream_t structure to which to write.
             * @param buffer Pointer to the buffer containing the data to be written.
             * @param size   Size of each element to be written.
             * @param count  Number of elements to write.
             * @return       The total number of elements successfully written.
             */
            static size_t write(fossil_fstream_t *stream, const void *buffer, size_t size, size_t count) {
                return fossil_fstream_write(stream, buffer, size, count);
            }

            /**
             * Append data to the end of an open stream.
             *
             * This function appends data from a buffer to the end of an open stream.
             *
             * @param stream Pointer to the fossil_fstream_t structure to which to append.
             * @param buffer Pointer to the buffer containing the data to be appended.
             * @param size   Size of each element to be appended.
             * @param count  Number of elements to append.
             * @return       0 on success, non-zero on failure.
             */
            static int32_t append(fossil_fstream_t *stream, const void *buffer, size_t size, int32_t count) {
                return fossil_fstream_append(stream, buffer, size, count);
            }

            /**
             * Seek to a specified position in an open stream.
             *
             * This function moves the file pointer associated with the stream to a new position.
             *
             * @param stream Pointer to the fossil_fstream_t structure to seek.
             * @param offset The offset from the specified origin.
             * @param origin The starting position for the offset (SEEK_SET, SEEK_CUR, SEEK_END).
             * @return       0 on success, non-zero on failure.
             */
            static int32_t seek(fossil_fstream_t *stream, int64_t offset, int32_t origin) {
                return fossil_fstream_seek(stream, offset, origin);
            }

            /**
             * Get the current position of the file pointer in an open stream.
             *
             * This function retrieves the current position of the file pointer in an open stream.
             *
             * @param stream Pointer to the fossil_fstream_t structure to get the position of.
             * @return       The current position of the file pointer.
             */
            static int32_t tell(fossil_fstream_t *stream) {
                return fossil_fstream_tell(stream);
            }

            /**
             * Save an open stream to a new file.
             *
             * This function saves the contents of an open stream to a new file.
             *
             * @param stream       Pointer to the fossil_fstream_t structure to be saved.
             * @param new_filename The name of the new file to save to.
             * @return             0 on success, non-zero on failure.
             */
            static int32_t save(fossil_fstream_t *stream, const char *new_filename) {
                return fossil_fstream_save(stream, new_filename);
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
                return fossil_fstream_copy(source_filename, destination_filename);
            }

            /**
             * Move a file stream.
             *
             * This function moves a file stream.
             *
             * @param source_filename      The name of the source file.
             * @param destination_filename The name of the destination file.
             * @return                     0 on success, non-zero on failure.
             */
            static int32_t move(const char *source_filename, const char *destination_filename) {
                return fossil_fstream_move(source_filename, destination_filename);
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
                return fossil_fstream_remove(filename);
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
                return fossil_fstream_rename(old_filename, new_filename);
            }

            /**
             * Flush the contents of an open stream.
             *
             * This function flushes the contents of an open stream.
             *
             * @param stream Pointer to the fossil_fstream_t structure to flush.
             * @return       0 on success, non-zero on failure.
             */
            static int32_t flush(fossil_fstream_t *stream) {
                return fossil_fstream_flush(stream);
            }

            /**
             * Seek to the beginning of an open stream.
             *
             * This function moves the file pointer associated with the stream to the beginning.
             *
             * @param stream Pointer to the fossil_fstream_t structure to seek.
             * @return       0 on success, non-zero on failure.
             */
            static int32_t setpos(fossil_fstream_t *stream, int32_t pos) {
                return fossil_fstream_setpos(stream, pos);
            }

            /**
             * Get the current position of the file pointer in an open stream.
             *
             * This function retrieves the current position of the file pointer in an open stream.
             *
             * @param stream Pointer to the fossil_fstream_t structure to get the position of.
             * @param pos    Pointer to store the current position of the file pointer.
             * @return       0 on success, non-zero on failure.
             */
            static int32_t getpos(fossil_fstream_t *stream, int32_t *pos) {
                return fossil_fstream_getpos(stream, pos);
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
                return fossil_fstream_rotate(filename, n);
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
                return fossil_fstream_backup(filename, backup_suffix);
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
                return fossil_fstream_file_exists(filename);
            }

            /**
             * Get the size of an open stream.
             *
             * This function retrieves the size of an open stream.
             *
             * @param stream Pointer to the fossil_fstream_t structure to get the size of.
             * @return       The size of the open stream.
             */
            static int32_t get_size(fossil_fstream_t *stream) {
                return fossil_fstream_get_size(stream);
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
                return fossil_fstream_delete(filename);
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
                return fossil_fstream_get_type(filename);
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
                return fossil_fstream_is_readable(filename);
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
                return fossil_fstream_is_writable(filename);
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
                return fossil_fstream_is_executable(filename);
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
                return fossil_fstream_set_permissions(filename, mode);
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
                return fossil_fstream_get_permissions(filename, mode);
            }

        };

    } // namespace io

} // namespace fossil

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
