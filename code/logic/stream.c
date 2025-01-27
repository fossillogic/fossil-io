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
#include "fossil/io/stream.h"
#include "fossil/io/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#endif

// Define portable permission flags
#define FOSSIL_FILE_READ   0x01  // Read permission
#define FOSSIL_FILE_WRITE  0x02  // Write permission
#define FOSSIL_FILE_EXEC   0x04  // Execute permission

typedef enum {
    FOSSIL_BUFFER_SMALL  = 100,
    FOSSIL_BUFFER_MEDIUM = 500,
    FOSSIL_BUFFER_LARGE  = 1000,
    FOSSIL_BUFFER_HUGE   = 5000,
    FOSSIL_BUFFER_GIANT  = 10000
} fossil_limit_t;

// Open a stream for file operations
int32_t fossil_fstream_open(fossil_fstream_t *stream, const char *filename, const char *mode) {
    if (stream == NULL || filename == NULL || mode == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    if (strlen(filename) >= FOSSIL_BUFFER_MEDIUM) {
        fprintf(stderr, "Error: Limit reached\n");
        return FOSSIL_ERROR_LIMIT_REACHED;
    }

    stream->file = fopen(filename, mode);
    if (stream->file == NULL) {
        fprintf(stderr, "Error: File not found - %s\n", filename);
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    strncpy(stream->filename, filename, FOSSIL_BUFFER_MEDIUM);

    return FOSSIL_ERROR_OK;
}

// Close an open stream
void fossil_fstream_close(fossil_fstream_t *stream) {
    if (stream != NULL && stream->file != NULL) {
        if (fclose(stream->file) != 0) {
            fprintf(stderr, "Error: Failed to close file - %s\n", stream->filename);
        }
        stream->file = NULL;
    }
}

// Read data from an open stream
size_t fossil_fstream_read(fossil_fstream_t *stream, void *buffer, size_t size, size_t count) {
    if (stream == NULL || buffer == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    size_t bytes_read = fread(buffer, size, count, stream->file);

    if (bytes_read == 0 && ferror(stream->file)) {
        fprintf(stderr, "Error: File corruption\n");
        return FOSSIL_ERROR_FILE_CORRUPTION;
    }

    return bytes_read;
}

// Write data to an open stream
size_t fossil_fstream_write(fossil_fstream_t *stream, const void *buffer, size_t size, size_t count) {
    if (stream == NULL || buffer == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    size_t bytes_written = fwrite(buffer, size, count, stream->file);

    if (bytes_written == 0 && ferror(stream->file)) {
        fprintf(stderr, "Error: File not found - %s\n", strerror(errno));
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    return bytes_written;
}

// Append data to the end of an open stream
int32_t fossil_fstream_append(fossil_fstream_t *stream, const void * restrict buffer, size_t size, int32_t count) {
    if (stream == NULL || buffer == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    fseek(stream->file, 0, SEEK_END);
    int32_t result = fwrite(buffer, size, count, stream->file);

    if (result != count) {
        fprintf(stderr, "Error: File not found\n");
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    return FOSSIL_ERROR_OK;
}

// Seek to a specified position in an open stream
int32_t fossil_fstream_seek(fossil_fstream_t *stream, int64_t offset, int32_t origin) {
    if (stream == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    int32_t result = fseek(stream->file, offset, origin);

    if (result != 0) {
        fprintf(stderr, "Error: File not found - %s\n", strerror(errno));
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    return FOSSIL_ERROR_OK;
}

// Get the current position of the file pointer in an open stream
int32_t fossil_fstream_tell(fossil_fstream_t *stream) {
    if (stream == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    long position = ftell(stream->file);

    if (position == -1L && ferror(stream->file)) {
        fprintf(stderr, "Error: IO error from getting file position\n");
        return FOSSIL_ERROR_IO;
    }

    return (int32_t)position;
}

// Save an open stream to a new file
int32_t fossil_fstream_save(fossil_fstream_t *stream, const char *new_filename) {
    if (stream == NULL || stream->file == NULL || new_filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    if (strlen(new_filename) >= FOSSIL_BUFFER_MEDIUM) {
        fprintf(stderr, "Error: Limit reached\n");
        return FOSSIL_ERROR_LIMIT_REACHED;
    }

    fclose(stream->file);

    if (rename(stream->filename, new_filename) != 0) {
        fprintf(stderr, "Error: Failed to save %s\n", new_filename);
        return FOSSIL_ERROR_NULL_POINTER;
    }

    // Reopen the file with the new name
    int32_t result = fossil_fstream_open(stream, new_filename, "r");

    if (result != FOSSIL_ERROR_OK) {
        fprintf(stderr, "Error: File not found - %s\n", strerror(errno));
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }
    return result;
}

// Copy a file from the source to the destination
int32_t fossil_fstream_copy(const char *source_filename, const char *destination_filename) {
    if (source_filename == NULL || destination_filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    FILE *source_file = fopen(source_filename, "rb");
    if (source_file == NULL) {
        fprintf(stderr, "Error: File not found - %s\n", source_filename);
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    FILE *destination_file = fopen(destination_filename, "wb");
    if (destination_file == NULL) {
        fprintf(stderr, "Error: File not found - %s\n", destination_filename);
        fclose(source_file);
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    char buffer[FOSSIL_BUFFER_MEDIUM];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, FOSSIL_BUFFER_MEDIUM, source_file)) > 0) {
        size_t bytesWritten = fwrite(buffer, 1, bytesRead, destination_file);
        if (bytesWritten != bytesRead) {
            fprintf(stderr, "Error: File not found\n");
            fclose(source_file);
            fclose(destination_file);
            return FOSSIL_ERROR_FILE_NOT_FOUND;
        }
    }

    fclose(source_file);
    fclose(destination_file);

    return FOSSIL_ERROR_OK;
}

// Create a backup of a file with a specified backup suffix
int32_t fossil_fstream_backup(const char *filename, const char *backup_suffix) {
    if (filename == NULL || backup_suffix == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    char backup_filename[FOSSIL_BUFFER_MEDIUM + 10];  // Length of backup_suffix + maximum integer length
    snprintf(backup_filename, FOSSIL_BUFFER_MEDIUM + 10, "%s%s", filename, backup_suffix);

    if (fossil_fstream_copy(filename, backup_filename) != FOSSIL_ERROR_OK) {
        fprintf(stderr, "Error: Failed to create backup for %s\n", filename);
        return FOSSIL_ERROR_IO;
    }

    return FOSSIL_ERROR_OK;
}

// Check if a file exists
int32_t fossil_fstream_file_exists(const char *filename) {
    if (filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return FOSSIL_ERROR_FILE_NOT_FOUND;  // File exists
    }
    return FOSSIL_ERROR_OK;  // File does not exist
}

// Get the size of an open stream
int32_t fossil_fstream_get_size(fossil_fstream_t *stream) {
    if (stream == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    fseek(stream->file, 0, SEEK_END);
    long size = ftell(stream->file);
    rewind(stream->file);

    if (size == -1L && ferror(stream->file)) {
        fprintf(stderr, "Error: IO error from getting file size\n");
        return FOSSIL_ERROR_OK;
    }

    return (int32_t)size;
}

// Delete a file
int32_t fossil_fstream_delete(const char *filename) {
    if (filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    if (remove(filename) == 0) {
        return FOSSIL_ERROR_OK;  // File deleted successfully
    }

    fprintf(stderr, "Error: IO error when deleting file %s\n", filename);
    return FOSSIL_ERROR_IO;
}

// Rename a file or directory
int32_t fossil_fstream_rename(const char *old_filename, const char *new_filename) {
    if (old_filename == NULL || new_filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    if (rename(old_filename, new_filename) != 0) {
        fprintf(stderr, "Error: Failed to rename file %s\n", old_filename);
        return FOSSIL_ERROR_IO;
    }

    return FOSSIL_ERROR_OK;
}

// Read file with buffering
int fossil_fstream_read_buffered(const char *filename, void *buffer, size_t size) {
    if (filename == NULL || buffer == NULL || size == 0) {
        fprintf(stderr, "Error: Invalid input\n");
        return -1;
    }

#ifdef _WIN32
    HANDLE hFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error opening file: %lu\n", GetLastError());
        return -1;
    }

    DWORD bytesRead;
    BOOL result = ReadFile(hFile, buffer, (DWORD)size, &bytesRead, NULL);
    CloseHandle(hFile);

    if (!result || bytesRead != size) {
        return -1;
    }

#else
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    size_t bytesRead = fread(buffer, 1, size, file);
    fclose(file);

    if (bytesRead != size) {
        return -1;
    }
#endif

    return 0;
}

// Write file with buffering
int fossil_fstream_write_buffered(const char *filename, const void *data, size_t size) {
    if (filename == NULL || data == NULL || size == 0) {
        fprintf(stderr, "Error: Invalid input\n");
        return -1;
    }

#ifdef _WIN32
    HANDLE hFile = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error opening file: %lu\n", GetLastError());
        return -1;
    }

    DWORD bytesWritten;
    BOOL result = WriteFile(hFile, data, (DWORD)size, &bytesWritten, NULL);
    CloseHandle(hFile);

    if (!result || bytesWritten != size) {
        return -1;
    }

#else
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    size_t bytesWritten = fwrite(data, 1, size, file);
    fclose(file);

    if (bytesWritten != size) {
        return -1;
    }
#endif

    return 0;
}

// Lock file (POSIX: fcntl, Windows: LockFile)
int fossil_fstream_lock(const char *filename) {
    if (filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return -1;
    }

#ifdef _WIN32
    HANDLE hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return -1;
    }

    if (!LockFile(hFile, 0, 0, MAXDWORD, MAXDWORD)) {
        CloseHandle(hFile);
        return -1;
    }

    CloseHandle(hFile);
#else
    int fd = open(filename, O_RDWR);
    if (fd == -1) {
        return -1;
    }

    struct flock fl = {0};
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &fl) == -1) {
        close(fd);
        return -1;
    }

    close(fd);
#endif

    return 0;
}

// Unlock file (POSIX: fcntl, Windows: UnlockFile)
int fossil_fstream_unlock(const char *filename) {
    if (filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return -1;
    }

#ifdef _WIN32
    HANDLE hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return -1;
    }

    if (!UnlockFile(hFile, 0, 0, MAXDWORD, MAXDWORD)) {
        CloseHandle(hFile);
        return -1;
    }

    CloseHandle(hFile);
#else
    int fd = open(filename, O_RDWR);
    if (fd == -1) {
        return -1;
    }

    struct flock fl = {0};
    fl.l_type = F_UNLCK;
    fl.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &fl) == -1) {
        close(fd);
        return -1;
    }

    close(fd);
#endif

    return 0;
}

// Detect file type (Regular file, Directory, Symbolic link)
int fossil_fstream_get_type(const char *filename) {
    if (filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return -1;
    }

#ifdef _WIN32
    DWORD attributes = GetFileAttributesA(filename);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        return -1;
    }

    if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
        return 1;  // Directory
    }

    if (attributes & FILE_ATTRIBUTE_REPARSE_POINT) {
        return 3;  // Symbolic Link (or Junction)
    }

    return 2;  // Regular File
#else
    struct stat file_stat;
    if (stat(filename, &file_stat) != 0) {
        return -1;
    }

    if (S_ISDIR(file_stat.st_mode)) return 1;  // Directory
    if (S_ISREG(file_stat.st_mode)) return 2;  // Regular file
    if (S_ISLNK(file_stat.st_mode)) return 3;  // Symbolic link

    return 0;  // Unknown
#endif
}

int32_t fossil_fstream_is_open(const fossil_fstream_t *stream) {
    return stream != NULL && stream->file != NULL;
}

int32_t fossil_fstream_read_all(fossil_fstream_t *stream, void **buffer, size_t *size) {
    if (stream == NULL || buffer == NULL || size == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    int32_t file_size = fossil_fstream_get_size(stream);
    if (file_size < 0) {
        fprintf(stderr, "Error: Failed to get file size\n");
        return FOSSIL_ERROR_IO;
    }

    *buffer = malloc(file_size);
    if (*buffer == NULL) {
        fprintf(stderr, "Error: Out of memory\n");
        return FOSSIL_ERROR_OUT_OF_MEMORY;
    }

    size_t bytes_read = fread(*buffer, 1, file_size, stream->file);
    if ((size_t)bytes_read != (size_t)file_size) {
        fprintf(stderr, "Error: File not found\n");
        free(*buffer);
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    *size = file_size;
    return FOSSIL_ERROR_OK;
}

int32_t fossil_fstream_write_all(fossil_fstream_t *stream, const void *buffer, size_t size) {
    if (stream == NULL || buffer == NULL || stream->file == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return FOSSIL_ERROR_NULL_POINTER;
    }

    size_t bytes_written = fwrite(buffer, 1, size, stream->file);
    if (bytes_written != size) {
        fprintf(stderr, "Error: File not found\n");
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    return FOSSIL_ERROR_OK;
}

// Check file permission (cross-platform)
int32_t fossil_fstream_check_permission(const char *filename, const char *mode) {
    if (filename == NULL || mode == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return 0;
    }

    int perm_flag = 0;
    if (strcmp(mode, "r") == 0) perm_flag = R_OK;
    else if (strcmp(mode, "w") == 0) perm_flag = W_OK;
    else if (strcmp(mode, "x") == 0) perm_flag = F_OK; // Windows doesn't support execution permissions directly.

#ifdef _WIN32
    return (_access(filename, perm_flag) == 0) ? 1 : 0;
#else
    return (access(filename, perm_flag) == 0) ? 1 : 0;
#endif
}

// Set file permissions (cross-platform)
int fossil_fstream_set_permissions(const char *filename, int permissions) {
    if (filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return -1;
    }

#ifdef _WIN32
    DWORD attr = GetFileAttributesA(filename);
    if (attr == INVALID_FILE_ATTRIBUTES) {
        return -1;  // Failed to get attributes
    }

    // Windows only supports read-only attribute
    if (permissions & FOSSIL_FILE_WRITE) {
        attr &= ~FILE_ATTRIBUTE_READONLY;  // Allow writing
    } else {
        attr |= FILE_ATTRIBUTE_READONLY;   // Set read-only
    }

    // Apply changes
    return (SetFileAttributesA(filename, attr) != 0) ? 0 : -1;
#else
    // POSIX: Use chmod to set permissions
    mode_t mode = 0;

    // Owner permissions
    if (permissions & FOSSIL_FILE_READ)  mode |= S_IRUSR;
    if (permissions & FOSSIL_FILE_WRITE) mode |= S_IWUSR;
    if (permissions & FOSSIL_FILE_EXEC)  mode |= S_IXUSR;

    // Group permissions (optional, can be added based on requirements)
    if (permissions & FOSSIL_FILE_READ)  mode |= S_IRGRP;
    if (permissions & FOSSIL_FILE_WRITE) mode |= S_IWGRP;
    if (permissions & FOSSIL_FILE_EXEC)  mode |= S_IXGRP;

    // Others permissions (optional, can be added based on requirements)
    if (permissions & FOSSIL_FILE_READ)  mode |= S_IROTH;
    if (permissions & FOSSIL_FILE_WRITE) mode |= S_IWOTH;
    if (permissions & FOSSIL_FILE_EXEC)  mode |= S_IXOTH;

    // Add sticky bit for directories (POSIX only)
    if (permissions & 0x100) {  // Assuming 0x100 flag represents sticky bit
        mode |= S_ISVTX;
    }

    return (chmod(filename, mode) == 0) ? 0 : -1;
#endif
}

// Get file permissions (cross-platform)
int fossil_fstream_get_permissions(const char *filename) {
    if (filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return -1;
    }

#ifdef _WIN32
    DWORD attr = GetFileAttributesA(filename);
    if (attr == INVALID_FILE_ATTRIBUTES) {
        return -1;  // Failed to get attributes
    }

    // Map Windows attributes to our portable permission flags
    int permissions = 0;
    if (attr & FILE_ATTRIBUTE_READONLY) {
        permissions |= FOSSIL_FILE_READ;
    } else {
        permissions |= FOSSIL_FILE_WRITE;
    }
    return permissions;
#else
    struct stat file_stat;
    if (stat(filename, &file_stat) != 0) {
        return -1;  // Failed to get permissions
    }

    // Map POSIX permissions to our portable permission flags
    int permissions = 0;
    if (file_stat.st_mode & S_IRUSR) permissions |= FOSSIL_FILE_READ;
    if (file_stat.st_mode & S_IWUSR) permissions |= FOSSIL_FILE_WRITE;
    if (file_stat.st_mode & S_IXUSR) permissions |= FOSSIL_FILE_EXEC;

    // Check for sticky bit in POSIX systems
    if (file_stat.st_mode & S_ISVTX) {
        permissions |= 0x100;  // Sticky bit flag
    }

    return permissions;
#endif
}

int32_t fossil_fstream_restrict(const char *filename) {
    if (filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return -1;
    }

#ifdef _WIN32
    // On Windows, restrict to owner-only permissions (Read/Write).
    // Windows doesn't natively support sticky bits, so we focus on file attributes.
    return fossil_fstream_set_permissions(filename, FOSSIL_FILE_READ | FOSSIL_FILE_WRITE);
#else
    // POSIX: Restrict to owner-only permissions, allowing read and write only to the owner
    mode_t mode = S_IRUSR | S_IWUSR;

    // Ensure sticky bit is supported on POSIX systems if needed.
    // Sticky bit is a special permission which can be used on directories to restrict file deletions
    // to the file's owner. It's typically used with directories like /tmp.
    // On non-directory files, it doesn't have any effect, but it's important for portability.
    mode |= S_ISVTX;  // Apply sticky bit if desired.

    // Apply the restricted permissions
    return (chmod(filename, mode) == 0) ? 0 : -1;
#endif
}

int32_t fossil_fstream_check_ownership(const char *filename) {
    if (filename == NULL) {
        fprintf(stderr, "Error: Null pointer\n");
        return 0;
    }

#ifdef _WIN32
    // Windows lacks direct user ownership checking in most environments.
    fprintf(stderr, "Error: Ownership check not implemented on Windows.\n");
    return 0;
#else
    struct stat file_stat;
    if (stat(filename, &file_stat) == 0) {
        return (file_stat.st_uid == getuid()) ? 1 : 0;
    }
    return 0;
#endif
}

int32_t fossil_fstream_compare_permissions(const char *file1, const char *file2) {
    int perm1 = fossil_fstream_get_permissions(file1);
    int perm2 = fossil_fstream_get_permissions(file2);

    if (perm1 == -1 || perm2 == -1) {
        return -1;  // Error reading permissions
    }

    return (perm1 == perm2) ? 1 : 0;  // Returns 1 if permissions are equal, 0 otherwise
}
