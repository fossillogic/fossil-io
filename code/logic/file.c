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
#include "fossil/io/file.h"
#include "fossil/io/soap.h"
#include "fossil/io/output.h"
#include "fossil/io/cstring.h"
#include "fossil/io/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#endif
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

fossil_io_file_t *FOSSIL_STDIN;
fossil_io_file_t *FOSSIL_STDOUT;
fossil_io_file_t *FOSSIL_STDERR;

typedef enum {
    FOSSIL_BUFFER_SMALL  = 100,
    FOSSIL_BUFFER_MEDIUM = 500,
    FOSSIL_BUFFER_LARGE  = 1000,
    FOSSIL_BUFFER_HUGE   = 5000,
    FOSSIL_BUFFER_GIANT  = 10000
} fossil_limit_t;

typedef struct {
    const char *keyword;
    const char *mode;
} fossil_io_file_mode_entry_t;

static const fossil_io_file_mode_entry_t fossil_io_file_mode_table[] = {
    // Classic C modes (standard fopen strings)
    { "r",     "r"   }, { "rb",    "rb"  },
    { "w",     "w"   }, { "wb",    "wb"  },
    { "a",     "a"   }, { "ab",    "ab"  },
    { "r+",    "r+"  }, { "rb+",   "r+b" }, { "r+b", "r+b" },
    { "w+",    "w+"  }, { "wb+",   "w+b" }, { "w+b", "w+b" },
    { "a+",    "a+"  }, { "ab+",   "a+b" }, { "a+b", "a+b" },

    // Extended readable modes
    { "read",          "r"   },
    { "readb",         "rb"  },
    { "write",         "w"   },
    { "writeb",        "wb"  },
    { "append",        "a"   },
    { "appendb",       "ab"  },
    { "read+write",    "r+"  },
    { "read+writeb",   "r+b" },
    { "write+read",    "w+"  },
    { "write+readb",   "w+b" },
    { "append+read",   "a+"  },
    { "append+readb",  "a+b" },
    { "read+t",        "rt"  },
    { "write+t",       "wt"  },
    { "read+write+t",  "r+t" },

    // Optional end-of-table sentinel
    { NULL, NULL }
};

static const char *fossil_io_file_mode_from_keyword(const char *keyword) {
    if (keyword == NULL) return NULL;
    for (int i = 0; fossil_io_file_mode_table[i].keyword != NULL; i++) {
        if (strcmp(keyword, fossil_io_file_mode_table[i].keyword) == 0) {
            return fossil_io_file_mode_table[i].mode;
        }
    }
    return NULL;
}

/**
 * Open a stream for file operations, populating extended file metadata.
 */
int32_t fossil_io_file_open(fossil_io_file_t *stream, const char *filename, const char *mode) {
    if (stream == NULL || filename == NULL || mode == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    if (strlen(filename) >= sizeof(stream->filename)) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Limit reached\n");
        return FOSSIL_ERROR_LIMIT_REACHED;
    }

    memset(stream, 0, sizeof(fossil_io_file_t));
    stream->file = fopen(filename, fossil_io_file_mode_from_keyword(mode));
    if (stream->file == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: File not found - %s\n", filename);
        stream->is_open = false;
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    // Set filename
    strncpy(stream->filename, filename, sizeof(stream->filename) - 1);
    stream->filename[sizeof(stream->filename) - 1] = '\0';

    // Set file descriptor
    stream->fd = fileno(stream->file);

    // Set file type and is_binary
    if (strchr(mode, 'b')) {
        strncpy(stream->type, "binary", sizeof(stream->type) - 1);
        stream->is_binary = true;
    } else {
        strncpy(stream->type, "text", sizeof(stream->type) - 1);
        stream->is_binary = false;
    }
    stream->type[sizeof(stream->type) - 1] = '\0';

    // Set append_mode
    stream->append_mode = (strchr(mode, 'a') != NULL);

    // Set flags (basic, not all possible flags)
    stream->flags = 0;
    if (strchr(mode, 'r')) stream->flags |= O_RDONLY;
    if (strchr(mode, 'w')) stream->flags |= O_WRONLY;
    if (strchr(mode, '+')) stream->flags |= O_RDWR;
    if (stream->append_mode) stream->flags |= O_APPEND;

    // Set readable/writable/executable
    stream->readable = fossil_io_file_is_readable(filename);
    stream->writable = fossil_io_file_is_writable(filename);
    stream->executable = fossil_io_file_is_executable(filename);

    // Set is_open
    stream->is_open = true;

    // Set position
    stream->position = ftell(stream->file);

    // Set size
    fseek(stream->file, 0, SEEK_END);
    stream->size = (size_t)ftell(stream->file);
    fseek(stream->file, stream->position, SEEK_SET);

    // Set timestamps and permissions if possible
#ifdef _WIN32
    stream->owner[0] = '\0';
    stream->group[0] = '\0';
    stream->mode = 0;
    stream->created_at = 0;
    stream->modified_at = 0;
    stream->accessed_at = 0;
#else
    struct stat st;
    if (stat(filename, &st) == 0) {
        stream->mode = st.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
        stream->created_at = st.st_ctime;
        stream->modified_at = st.st_mtime;
        stream->accessed_at = st.st_atime;
        struct passwd *pw = getpwuid(st.st_uid);
        struct group *gr = getgrgid(st.st_gid);
        if (pw) {
            strncpy(stream->owner, pw->pw_name, sizeof(stream->owner) - 1);
            stream->owner[sizeof(stream->owner) - 1] = '\0';
        } else {
            stream->owner[0] = '\0';
        }
        if (gr) {
            strncpy(stream->group, gr->gr_name, sizeof(stream->group) - 1);
            stream->group[sizeof(stream->group) - 1] = '\0';
        } else {
            stream->group[0] = '\0';
        }
    } else {
        stream->mode = 0;
        stream->owner[0] = '\0';
        stream->group[0] = '\0';
        stream->created_at = 0;
        stream->modified_at = 0;
        stream->accessed_at = 0;
    }
#endif

    // Buffering
    stream->buffer = NULL;
    stream->buffer_size = 0;

    // Temporary, locked, etc.
    stream->temporary = false;
    stream->locked = false;

    // AI/Intelligent processing members
    stream->indexed = false;
    stream->analyzed = false;
    stream->language[0] = '\0';
    stream->sentiment = 0.0f;
    for (int i = 0; i < 16; ++i) stream->tags[i] = NULL;
    stream->tag_count = 0;
    stream->compressed = false;
    stream->embedding = NULL;
    stream->embedding_size = 0;

    return FOSSIL_ERROR_OK;
}

// Close an open stream
void fossil_io_file_close(fossil_io_file_t *stream) {
    if (stream == NULL || stream->file == NULL) {
        return;
    }

    fclose(stream->file);

    stream->file = NULL;
    stream->fd = -1;
    stream->is_open = false;
    stream->size = 0;
    stream->position = 0;
    stream->flags = 0;
    stream->readable = false;
    stream->writable = false;
    stream->executable = false;
    stream->append_mode = false;
    stream->temporary = false;
    stream->locked = false;
    stream->buffer = NULL;
    stream->buffer_size = 0;
    stream->created_at = 0;
    stream->modified_at = 0;
    stream->accessed_at = 0;
    stream->type[0] = '\0';
    stream->owner[0] = '\0';
    stream->group[0] = '\0';
    stream->filename[0] = '\0';
    stream->mode = 0;
    stream->indexed = false;
    stream->analyzed = false;
    stream->language[0] = '\0';
    stream->sentiment = 0.0f;
    for (int i = 0; i < 16; ++i) stream->tags[i] = NULL;
    stream->tag_count = 0;
    stream->compressed = false;
    stream->embedding = NULL;
    stream->embedding_size = 0;
    stream->is_binary = false;
}

int32_t fossil_io_file_redirect_to_devnull(fossil_io_file_t *stream) {
    if (!stream) {
        return -1; // Invalid stream
    }

#ifdef _WIN32
    FILE *devnull = fopen("NUL", "w");
#else
    FILE *devnull = fopen("/dev/null", "w");
#endif

    if (!devnull) {
        return -1; // Failed to open null device
    }

    if (stream->file) {
        fclose(stream->file);
    }

    // Reset all fields to default/zero
    memset(stream, 0, sizeof(fossil_io_file_t));

    stream->file = devnull;
    stream->fd = fileno(devnull);
#ifdef _WIN32
    strncpy(stream->filename, "NUL", sizeof(stream->filename) - 1);
#else
    strncpy(stream->filename, "/dev/null", sizeof(stream->filename) - 1);
#endif
    stream->filename[sizeof(stream->filename) - 1] = '\0';
    strncpy(stream->type, "text", sizeof(stream->type) - 1);
    stream->type[sizeof(stream->type) - 1] = '\0';
    stream->mode = 0666;
    stream->owner[0] = '\0';
    stream->group[0] = '\0';
    stream->flags = O_WRONLY;
    stream->is_open = true;
    stream->readable = false;
    stream->writable = true;
    stream->executable = false;
    stream->append_mode = false;
    stream->temporary = false;
    stream->locked = false;
    stream->size = 0;
    stream->position = 0;
    stream->buffer = NULL;
    stream->buffer_size = 0;
    stream->created_at = 0;
    stream->modified_at = 0;
    stream->accessed_at = 0;
    stream->indexed = false;
    stream->analyzed = false;
    stream->language[0] = '\0';
    stream->sentiment = 0.0f;
    for (int i = 0; i < 16; ++i) stream->tags[i] = NULL;
    stream->tag_count = 0;
    stream->compressed = false;
    stream->embedding = NULL;
    stream->embedding_size = 0;
    stream->is_binary = false;

    return 0;
}

int32_t fossil_io_file_freopen(fossil_io_file_t *stream, const char *filename, const char *mode, FILE *file) {
    if (stream == NULL || filename == NULL || mode == NULL || file == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    FILE *new_file = freopen(filename, fossil_io_file_mode_from_keyword(mode), file);
    if (new_file == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: File not found - %s\n", filename);
        stream->is_open = false;
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    // Reset structure
    memset(stream, 0, sizeof(fossil_io_file_t));
    stream->file = new_file;
    stream->fd = fileno(new_file);

    // Set filename
    strncpy(stream->filename, filename, sizeof(stream->filename) - 1);
    stream->filename[sizeof(stream->filename) - 1] = '\0';

    // Set file type and is_binary
    if (strchr(mode, 'b')) {
        strncpy(stream->type, "binary", sizeof(stream->type) - 1);
        stream->is_binary = true;
    } else {
        strncpy(stream->type, "text", sizeof(stream->type) - 1);
        stream->is_binary = false;
    }
    stream->type[sizeof(stream->type) - 1] = '\0';

    // Set append_mode
    stream->append_mode = (strchr(mode, 'a') != NULL);

    // Set flags
    stream->flags = 0;
    if (strchr(mode, 'r')) stream->flags |= O_RDONLY;
    if (strchr(mode, 'w')) stream->flags |= O_WRONLY;
    if (strchr(mode, '+')) stream->flags |= O_RDWR;
    if (stream->append_mode) stream->flags |= O_APPEND;

    // Set readable/writable/executable
    stream->readable = fossil_io_file_is_readable(filename);
    stream->writable = fossil_io_file_is_writable(filename);
    stream->executable = fossil_io_file_is_executable(filename);

    // Set is_open
    stream->is_open = true;

    // Set position
    stream->position = ftell(new_file);

    // Set size
    fseek(new_file, 0, SEEK_END);
    stream->size = (size_t)ftell(new_file);
    fseek(new_file, stream->position, SEEK_SET);

    // Set timestamps and permissions if possible
#ifdef _WIN32
    stream->owner[0] = '\0';
    stream->group[0] = '\0';
    stream->mode = 0;
    stream->created_at = 0;
    stream->modified_at = 0;
    stream->accessed_at = 0;
#else
    struct stat st;
    if (stat(filename, &st) == 0) {
        stream->mode = st.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
        stream->created_at = st.st_ctime;
        stream->modified_at = st.st_mtime;
        stream->accessed_at = st.st_atime;
        struct passwd *pw = getpwuid(st.st_uid);
        struct group *gr = getgrgid(st.st_gid);
        if (pw) {
            strncpy(stream->owner, pw->pw_name, sizeof(stream->owner) - 1);
            stream->owner[sizeof(stream->owner) - 1] = '\0';
        } else {
            stream->owner[0] = '\0';
        }
        if (gr) {
            strncpy(stream->group, gr->gr_name, sizeof(stream->group) - 1);
            stream->group[sizeof(stream->group) - 1] = '\0';
        } else {
            stream->group[0] = '\0';
        }
    } else {
        stream->mode = 0;
        stream->owner[0] = '\0';
        stream->group[0] = '\0';
        stream->created_at = 0;
        stream->modified_at = 0;
        stream->accessed_at = 0;
    }
#endif

    // Buffering
    stream->buffer = NULL;
    stream->buffer_size = 0;

    // Temporary, locked, etc.
    stream->temporary = false;
    stream->locked = false;

    // AI/Intelligent processing members
    stream->indexed = false;
    stream->analyzed = false;
    stream->language[0] = '\0';
    stream->sentiment = 0.0f;
    for (int i = 0; i < 16; ++i) stream->tags[i] = NULL;
    stream->tag_count = 0;
    stream->compressed = false;
    stream->embedding = NULL;
    stream->embedding_size = 0;

    return FOSSIL_ERROR_OK;
}

// Read data from an open stream
size_t fossil_io_file_read(fossil_io_file_t *stream, void *buffer, size_t size, size_t count) {
    if (stream == NULL || buffer == NULL || stream->file == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    // Only allow reading if the stream is marked readable
    if (!stream->readable) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: File is not readable - %s\n", stream->filename);
        return FOSSIL_ERROR_IO;
    }

    size_t items_read = fread(buffer, size, count, stream->file);

    if (items_read == 0 && ferror(stream->file)) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: File corruption - %s\n", stream->filename);
        return FOSSIL_ERROR_FILE_CORRUPTION;
    }

    // Update file position and size in the stream struct
    stream->position = ftell(stream->file);
    fseek(stream->file, 0, SEEK_END);
    stream->size = (size_t)ftell(stream->file);
    fseek(stream->file, stream->position, SEEK_SET);

    return items_read;
}

// Write data to an open stream
size_t fossil_io_file_write(fossil_io_file_t *stream, const void *buffer, size_t size, size_t count) {
    if (stream == NULL || buffer == NULL || stream->file == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    // Only allow writing if the stream is marked writable
    if (!stream->writable) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: File is not writable - %s\n", stream->filename);
        return FOSSIL_ERROR_IO;
    }

    size_t bytes_written = fwrite(buffer, size, count, stream->file);

    if (bytes_written == 0 && ferror(stream->file)) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: File write failed - %s\n", strerror(errno));
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    // Update file position and size in the stream struct
    stream->position = ftell(stream->file);

    // Save current position, seek to end to get size, then restore position
    int64_t current_pos = stream->position;
    if (fseek(stream->file, 0, SEEK_END) == 0) {
        stream->size = (size_t)ftell(stream->file);
        fseek(stream->file, current_pos, SEEK_SET);
    }

    return bytes_written;
}

// Append data to the end of an open stream
int32_t fossil_io_file_append(fossil_io_file_t *stream, const void *buffer, size_t size, int32_t count) {
    if (stream == NULL || buffer == NULL || stream->file == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    // Only allow writing if the stream is marked writable
    if (!stream->writable) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: File is not writable - %s\n", stream->filename);
        return FOSSIL_ERROR_IO;
    }

    // Seek to end and update position
    if (fseek(stream->file, 0, SEEK_END) != 0) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Failed to seek to end of file\n");
        return FOSSIL_ERROR_IO;
    }
    stream->position = ftell(stream->file);

    size_t written = fwrite(buffer, size, count, stream->file);

    if (written != (size_t)count) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Failed to append data to file - %s\n", stream->filename);
        return FOSSIL_ERROR_IO;
    }

    // Update file size and position in the struct
    stream->position = ftell(stream->file);

    // Save current position, seek to end to get size, then restore position
    int64_t current_pos = stream->position;
    if (fseek(stream->file, 0, SEEK_END) == 0) {
        stream->size = (size_t)ftell(stream->file);
        fseek(stream->file, current_pos, SEEK_SET);
    }

    return FOSSIL_ERROR_OK;
}

// Seek to a specified position in an open stream
int32_t fossil_io_file_seek(fossil_io_file_t *stream, int64_t offset, int32_t origin) {
    if (stream == NULL || stream->file == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    // Use fseek for standard FILE* streams
    int result = fseek(stream->file, (long)offset, origin);
    if (result != 0) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Seek failed - %s\n", strerror(errno));
        return FOSSIL_ERROR_IO;
    }

    // Update the cached position in the stream struct
    long pos = ftell(stream->file);
    if (pos == -1L && ferror(stream->file)) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: IO error from getting file position\n");
        return FOSSIL_ERROR_IO;
    }
    stream->position = (int64_t)pos;

    return FOSSIL_ERROR_OK;
}

// Get the current position of the file pointer in an open stream
int32_t fossil_io_file_tell(fossil_io_file_t *stream) {
    if (stream == NULL || stream->file == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    long position = ftell(stream->file);

    if (position == -1L && ferror(stream->file)) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: IO error from getting file position\n");
        return FOSSIL_ERROR_IO;
    }

    stream->position = (int64_t)position; // update struct's position field

    // Optionally update cached size as well
    long current_pos = position;
    if (fseek(stream->file, 0, SEEK_END) == 0) {
        stream->size = (size_t)ftell(stream->file);
        fseek(stream->file, current_pos, SEEK_SET);
    }

    return (int32_t)position;
}

// Save an open stream to a new file
int32_t fossil_io_file_save(fossil_io_file_t *stream, const char *new_filename) {
    if (stream == NULL || stream->file == NULL || new_filename == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    if (strlen(new_filename) >= sizeof(stream->filename)) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Limit reached\n");
        return FOSSIL_ERROR_LIMIT_REACHED;
    }

    // Close the current file
    fclose(stream->file);

    // Rename the file
    if (rename(stream->filename, new_filename) != 0) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Failed to save %s\n", new_filename);
        stream->is_open = false;
        return FOSSIL_ERROR_IO;
    }

    // Determine mode string based on file type
    const char *mode_str = (strcmp(stream->type, "binary") == 0) ? "rb+" : "r+";

    // Reopen the file with the new name and update metadata
    int32_t result = fossil_io_file_open(stream, new_filename, mode_str);
    if (result != FOSSIL_ERROR_OK) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: File not found - %s\n", strerror(errno));
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    // Update the filename in the struct
    strncpy(stream->filename, new_filename, sizeof(stream->filename) - 1);
    stream->filename[sizeof(stream->filename) - 1] = '\0';

    // Update type field based on file content (already handled in fossil_io_file_open)
    // All other fields (mode, owner, group, flags, timestamps, etc.) are updated by fossil_io_file_open

    return FOSSIL_ERROR_OK;
}

// Copy a file from the source to the destination
int32_t fossil_io_file_copy(const char *source_filename, const char *destination_filename) {
    if (source_filename == NULL || destination_filename == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    FILE *source_file = fopen(source_filename, "rb");
    if (source_file == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: File not found - %s\n", source_filename);
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    FILE *destination_file = fopen(destination_filename, "wb");
    if (destination_file == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: File not found - %s\n", destination_filename);
        fclose(source_file);
        return FOSSIL_ERROR_FILE_NOT_FOUND;
    }

    char buffer[FOSSIL_BUFFER_MEDIUM];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, FOSSIL_BUFFER_MEDIUM, source_file)) > 0) {
        size_t bytesWritten = fwrite(buffer, 1, bytesRead, destination_file);
        if (bytesWritten != bytesRead) {
            fossil_io_fprintf(FOSSIL_STDERR, "Error: File not found\n");
            fclose(source_file);
            fclose(destination_file);
            return FOSSIL_ERROR_FILE_NOT_FOUND;
        }
    }

    fclose(source_file);
    fclose(destination_file);

    return FOSSIL_ERROR_OK;
}

int32_t fossil_io_file_remove(const char *filename) {
    if (filename == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    if (remove(filename) == 0) {
        return FOSSIL_ERROR_OK;  // File removed successfully
    } else {
        if (errno == ENOENT) {
            fossil_io_fprintf(FOSSIL_STDERR, "Error: File not found when removing file %s\n", filename);
            return FOSSIL_ERROR_FILE_NOT_FOUND;
        } else {
            fossil_io_fprintf(FOSSIL_STDERR, "Error: IO error when removing file %s\n", filename);
            return FOSSIL_ERROR_IO;
        }
    }
}

int32_t fossil_io_file_rename(const char *old_filename, const char *new_filename) {
    if (old_filename == NULL || new_filename == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    if (rename(old_filename, new_filename) != 0) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Failed to rename file %s\n", old_filename);
        return FOSSIL_ERROR_IO;
    }

    return FOSSIL_ERROR_OK;
}

int32_t fossil_io_file_flush(fossil_io_file_t *stream) {
    if (stream == NULL || stream->file == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    if (fflush(stream->file) != 0) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Failed to flush file - %s\n", stream->filename);
        return FOSSIL_ERROR_IO;
    }

    // Update file position after flush
    stream->position = ftell(stream->file);

#ifndef _WIN32
    // Update timestamps and permissions using updated struct fields
    struct stat st;
    if (stat(stream->filename, &st) == 0) {
        stream->mode = st.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
        stream->modified_at = st.st_mtime;
        stream->accessed_at = st.st_atime;
        struct passwd *pw = getpwuid(st.st_uid);
        struct group *gr = getgrgid(st.st_gid);
        if (pw) {
            strncpy(stream->owner, pw->pw_name, sizeof(stream->owner) - 1);
            stream->owner[sizeof(stream->owner) - 1] = '\0';
        }
        if (gr) {
            strncpy(stream->group, gr->gr_name, sizeof(stream->group) - 1);
            stream->group[sizeof(stream->group) - 1] = '\0';
        }
    }
#endif

    return FOSSIL_ERROR_OK;
}

int32_t fossil_io_file_setpos(fossil_io_file_t *stream, int64_t pos) {
    if (stream == NULL || stream->file == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    if (fseek(stream->file, (long)pos, SEEK_SET) != 0) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Failed to set file position\n");
        return FOSSIL_ERROR_IO;
    }

    long new_pos = ftell(stream->file);
    if (new_pos == -1L && ferror(stream->file)) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: IO error from getting file position\n");
        return FOSSIL_ERROR_IO;
    }
    stream->position = (int64_t)new_pos;

    return FOSSIL_ERROR_OK;
}

int32_t fossil_io_file_getpos(fossil_io_file_t *stream, int64_t *pos) {
    if (stream == NULL || stream->file == NULL || pos == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    long file_pos = ftell(stream->file);
    if (file_pos == -1L && ferror(stream->file)) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: IO error from getting file position\n");
        return FOSSIL_ERROR_IO;
    }

    // Update the struct's position field
    stream->position = (int64_t)file_pos;
    *pos = stream->position;

    // Optionally update cached size as well
    long current_pos = file_pos;
    if (fseek(stream->file, 0, SEEK_END) == 0) {
        stream->size = (size_t)ftell(stream->file);
        fseek(stream->file, current_pos, SEEK_SET);
    }

    return FOSSIL_ERROR_OK;
}

int32_t fossil_io_file_rotate(const char *filename, int32_t n) {
    if (filename == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    char old_filename[FOSSIL_BUFFER_MEDIUM];
    char new_filename[FOSSIL_BUFFER_MEDIUM];

    for (int32_t i = n; i > 0; i--) {
        if (i == 1) {
            snprintf(old_filename, FOSSIL_BUFFER_MEDIUM, "%s", filename);
        } else {
            snprintf(old_filename, FOSSIL_BUFFER_MEDIUM, "%s.%d", filename, i - 1);
        }

        snprintf(new_filename, FOSSIL_BUFFER_MEDIUM, "%s.%d", filename, i);
        if (fossil_io_file_rename(old_filename, new_filename) != FOSSIL_ERROR_OK) {
            fossil_io_fprintf(FOSSIL_STDERR, "Error: Failed to rotate file %s\n", filename);
            return FOSSIL_ERROR_IO;
        }
    }

    return FOSSIL_ERROR_OK;
}

// Create a backup of a file with a specified backup suffix
int32_t fossil_io_file_backup(const char *filename, const char *backup_suffix) {
    if (filename == NULL || backup_suffix == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    char backup_filename[FOSSIL_BUFFER_MEDIUM + 10];  // Length of backup_suffix + maximum integer length
    snprintf(backup_filename, FOSSIL_BUFFER_MEDIUM + 10, "%s%s", filename, backup_suffix);

    if (fossil_io_file_copy(filename, backup_filename) != FOSSIL_ERROR_OK) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Failed to create backup for %s\n", filename);
        return FOSSIL_ERROR_IO;
    }

    return FOSSIL_ERROR_OK;
}

// Check if a file exists
int32_t fossil_io_file_file_exists(const char *filename) {
    if (filename == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return FOSSIL_ERROR_FILE_NOT_FOUND;  // File exists
    }
    return FOSSIL_ERROR_OK;  // File does not exist
}

// Get the size of an open stream (updates stream->size)
int32_t fossil_io_file_get_size(fossil_io_file_t *stream) {
    if (stream == NULL || stream->file == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    // Save current position
    int64_t current_pos = stream->position;
    if (fseek(stream->file, 0, SEEK_END) != 0) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: IO error from seeking to end\n");
        return FOSSIL_ERROR_IO;
    }
    long size = ftell(stream->file);
    if (size == -1L && ferror(stream->file)) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: IO error from getting file size\n");
        return FOSSIL_ERROR_IO;
    }

    // Restore original position
    if (fseek(stream->file, (long)current_pos, SEEK_SET) != 0) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: IO error from restoring file position\n");
        return FOSSIL_ERROR_IO;
    }

    stream->size = (size_t)size;
    stream->position = current_pos;
    return (int32_t)size;
}

// Delete a file
int32_t fossil_io_file_delete(const char *filename) {
    if (filename == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    if (remove(filename) == 0) {
        return FOSSIL_ERROR_OK;  // File deleted successfully
    }

    fossil_io_fprintf(FOSSIL_STDERR, "Error: IO error when deleting file %s\n", filename);
    return FOSSIL_ERROR_IO;
}

// Detect file type (Regular file, Directory, Symbolic link)
int fossil_io_file_get_type(const char *filename) {
    if (filename == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Null pointer\n");
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

int32_t fossil_io_file_is_open(const fossil_io_file_t *stream) {
    return stream != NULL && stream->file != NULL && stream->is_open;
}

int32_t fossil_io_file_is_readable(const char *filename) {
#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(filename);
    return (attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_DIRECTORY));
#else
    return (access(filename, R_OK) == 0) ? 1 : 0;
#endif
}

fossil_io_file_t fossil_io_file_tempfile(void) {
    fossil_io_file_t temp_stream;
    memset(&temp_stream, 0, sizeof(fossil_io_file_t));
    char temp_filename[FOSSIL_BUFFER_MEDIUM] = {0};

#ifdef _WIN32
    // Use GetTempFileName for Windows
    if (GetTempFileNameA(".", "fossil", 0, temp_filename) == 0) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Failed to create temporary file\n");
        temp_stream.file = NULL;
        temp_stream.is_open = false;
        return temp_stream;
    }
#else
    // Use mkstemp for POSIX
    snprintf(temp_filename, FOSSIL_BUFFER_MEDIUM, "/tmp/fossil_tempfile_XXXXXX");
    int fd = mkstemp(temp_filename);
    if (fd == -1) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Failed to create temporary file\n");
        temp_stream.file = NULL;
        temp_stream.is_open = false;
        return temp_stream;
    }
    close(fd); // We'll reopen with our own mode
#endif

    // Open the temporary file
    if (fossil_io_file_open(&temp_stream, temp_filename, "wb+") != FOSSIL_ERROR_OK) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Failed to open temporary file - %s\n", temp_filename);
        temp_stream.file = NULL;
        temp_stream.is_open = false;
        return temp_stream;
    }

    // Set updated fields for the new file type
    strncpy(temp_stream.type, "binary", sizeof(temp_stream.type) - 1);
    temp_stream.type[sizeof(temp_stream.type) - 1] = '\0';
    temp_stream.temporary = true;
    temp_stream.is_binary = true;
    temp_stream.readable = true;
    temp_stream.writable = true;
    temp_stream.append_mode = false;
    temp_stream.locked = false;
    temp_stream.size = 0;
    temp_stream.position = 0;
    temp_stream.buffer = NULL;
    temp_stream.buffer_size = 0;
    temp_stream.created_at = time(NULL);
    temp_stream.modified_at = temp_stream.created_at;
    temp_stream.accessed_at = temp_stream.created_at;
    temp_stream.indexed = false;
    temp_stream.analyzed = false;
    temp_stream.language[0] = '\0';
    temp_stream.sentiment = 0.0f;
    for (int i = 0; i < 16; ++i) temp_stream.tags[i] = NULL;
    temp_stream.tag_count = 0;
    temp_stream.compressed = false;
    temp_stream.embedding = NULL;
    temp_stream.embedding_size = 0;

#ifndef _WIN32
    // Set permissions, owner, and group if possible
    struct stat st;
    if (stat(temp_filename, &st) == 0) {
        temp_stream.mode = st.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
        struct passwd *pw = getpwuid(st.st_uid);
        struct group *gr = getgrgid(st.st_gid);
        if (pw) {
            strncpy(temp_stream.owner, pw->pw_name, sizeof(temp_stream.owner) - 1);
            temp_stream.owner[sizeof(temp_stream.owner) - 1] = '\0';
        } else {
            temp_stream.owner[0] = '\0';
        }
        if (gr) {
            strncpy(temp_stream.group, gr->gr_name, sizeof(temp_stream.group) - 1);
            temp_stream.group[sizeof(temp_stream.group) - 1] = '\0';
        } else {
            temp_stream.group[0] = '\0';
        }
    } else {
        temp_stream.mode = 0600;
        temp_stream.owner[0] = '\0';
        temp_stream.group[0] = '\0';
    }
#else
    temp_stream.mode = 0600;
    temp_stream.owner[0] = '\0';
    temp_stream.group[0] = '\0';
#endif

    temp_stream.flags = O_RDWR | O_CREAT | O_EXCL;
    temp_stream.executable = false;

    return temp_stream;
}

int32_t fossil_io_file_is_writable(const char *filename) {
#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(filename);
    if (attrs == INVALID_FILE_ATTRIBUTES || (attrs & FILE_ATTRIBUTE_DIRECTORY)) {
        return 0;
    }
    return !(attrs & FILE_ATTRIBUTE_READONLY);
#else
    return (access(filename, W_OK) == 0) ? 1 : 0;
#endif
}

int32_t fossil_io_file_is_executable(const char *filename) {
#ifdef _WIN32
    // On Windows, executables typically have extensions like .exe, .bat, .cmd
    const char *ext = strrchr(filename, '.');
    return (ext && (_stricmp(ext, ".exe") == 0 || _stricmp(ext, ".bat") == 0 || _stricmp(ext, ".cmd") == 0)) ? 1 : 0;
#else
    return (access(filename, X_OK) == 0) ? 1 : 0;
#endif
}

int32_t fossil_io_file_set_permissions(const char *filename, int32_t mode) {
#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(filename);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return -1; // File not found or other error
    }

    if (mode & _S_IWRITE) {
        attrs &= ~FILE_ATTRIBUTE_READONLY; // Remove readonly
    } else {
        attrs |= FILE_ATTRIBUTE_READONLY; // Add readonly
    }

    return (SetFileAttributesA(filename, attrs) != 0) ? 0 : -1;
#else
    return chmod(filename, mode);
#endif
}

int32_t fossil_io_file_get_permissions(const char *filename, int32_t *mode) {
    if (!mode) {
        return -1; // Null pointer error
    }

#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(filename);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return -1; // File not found or other error
    }

    *mode = _S_IREAD;
    if (!(attrs & FILE_ATTRIBUTE_READONLY)) {
        *mode |= _S_IWRITE;
    }
    return 0;
#else
    struct stat st;
    if (stat(filename, &st) != 0) {
        return -1; // File not found or error
    }
    *mode = st.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO); // User, Group, Other permissions
    return 0;
#endif
}

void fossil_io_file_rewind(fossil_io_file_t *stream) {
    if (stream && stream->file) {
        rewind(stream->file);
        stream->position = 0;

        // Optionally update cached size as well
        long current_pos = ftell(stream->file);
        if (fseek(stream->file, 0, SEEK_END) == 0) {
            stream->size = (size_t)ftell(stream->file);
            fseek(stream->file, current_pos, SEEK_SET);
        }
    }
}

int fossil_io_file_ai_analyze(fossil_io_file_t *f)
{
    if (!f || !f->file || !f->readable) {
        return -1;
    }

    // Reset analysis state
    f->analyzed = false;
    f->language[0] = '\0';
    f->sentiment = 0.0f;

    // Only analyze text files
    if (f->is_binary) {
        strncpy(f->language, "binary", sizeof(f->language) - 1);
        f->language[sizeof(f->language) - 1] = '\0';
        f->analyzed = true;
        f->sentiment = 0.0f;
        return 0;
    }

    // Read file content (up to 4096 bytes for analysis)
    long orig_pos = ftell(f->file);
    fseek(f->file, 0, SEEK_SET);
    char buffer[4096 + 1] = {0};
    size_t bytes = fread(buffer, 1, sizeof(buffer) - 1, f->file);
    buffer[bytes] = '\0';
    fseek(f->file, orig_pos, SEEK_SET);

    // Use cstring API for language detection
    size_t ascii_count = 0, non_ascii_count = 0;
    for (size_t i = 0; i < bytes; ++i) {
        if ((unsigned char)buffer[i] < 128)
            ascii_count++;
        else
            non_ascii_count++;
    }
    if (ascii_count > non_ascii_count) {
        cstring lang = fossil_io_cstring_create("en");
        strncpy(f->language, lang, sizeof(f->language) - 1);
        fossil_io_cstring_free(lang);
    } else {
        cstring lang = fossil_io_cstring_create("unknown");
        strncpy(f->language, lang, sizeof(f->language) - 1);
        fossil_io_cstring_free(lang);
    }
    f->language[sizeof(f->language) - 1] = '\0';

    // Sentiment analysis using cstring API
    const char *positive[] = { "good", "great", "excellent", "happy", "love", "success", "positive" };
    const char *negative[] = { "bad", "poor", "sad", "hate", "fail", "negative", "terrible" };
    int pos_score = 0, neg_score = 0;
    for (size_t i = 0; i < sizeof(positive)/sizeof(positive[0]); ++i) {
        if (fossil_io_cstring_contains(buffer, positive[i])) pos_score++;
    }
    for (size_t i = 0; i < sizeof(negative)/sizeof(negative[0]); ++i) {
        if (fossil_io_cstring_contains(buffer, negative[i])) neg_score++;
    }
    if (pos_score + neg_score > 0)
        f->sentiment = (float)(pos_score - neg_score) / (pos_score + neg_score);
    else
        f->sentiment = 0.0f;

    // Mark as analyzed
    f->analyzed = true;

    return 0;
}

int fossil_io_file_ai_generate_tags(fossil_io_file_t *f)
{
    if (!f || !f->file || !f->readable)
        return -1;

    // Clear previous tags
    for (int i = 0; i < 16; ++i) {
        if (f->tags[i]) {
            fossil_io_cstring_free(f->tags[i]);
            f->tags[i] = NULL;
        }
    }
    f->tag_count = 0;

    // Only analyze text files
    if (f->is_binary) {
        f->tags[0] = fossil_io_cstring_create("binary");
        f->tag_count = 1;
        return 0;
    }

    // Read up to 4096 bytes for tag analysis
    long orig_pos = ftell(f->file);
    fseek(f->file, 0, SEEK_SET);
    char buffer[4096 + 1] = {0};
    size_t bytes = fread(buffer, 1, sizeof(buffer) - 1, f->file);
    buffer[bytes] = '\0';
    fseek(f->file, orig_pos, SEEK_SET);

    // Sanitize input before analysis
    char *sanitized = fossil_io_soap_sanitize(buffer);

    // Tag generation using fossil_io_soap_* detection functions
    int max_tags = 16;
    int tag_idx = 0;

    if (fossil_io_soap_detect_ragebait(sanitized) && tag_idx < max_tags)
        f->tags[tag_idx++] = fossil_io_cstring_create("ragebait");
    if (fossil_io_soap_detect_clickbait(sanitized) && tag_idx < max_tags)
        f->tags[tag_idx++] = fossil_io_cstring_create("clickbait");
    if (fossil_io_soap_detect_spam(sanitized) && tag_idx < max_tags)
        f->tags[tag_idx++] = fossil_io_cstring_create("spam");
    if (fossil_io_soap_detect_woke(sanitized) && tag_idx < max_tags)
        f->tags[tag_idx++] = fossil_io_cstring_create("woke");
    if (fossil_io_soap_detect_bot(sanitized) && tag_idx < max_tags)
        f->tags[tag_idx++] = fossil_io_cstring_create("bot");
    if (fossil_io_soap_detect_sarcasm(sanitized) && tag_idx < max_tags)
        f->tags[tag_idx++] = fossil_io_cstring_create("sarcasm");
    if (fossil_io_soap_detect_formal(sanitized) && tag_idx < max_tags)
        f->tags[tag_idx++] = fossil_io_cstring_create("formal");
    if (fossil_io_soap_detect_snowflake(sanitized) && tag_idx < max_tags)
        f->tags[tag_idx++] = fossil_io_cstring_create("snowflake");
    if (fossil_io_soap_detect_offensive(sanitized) && tag_idx < max_tags)
        f->tags[tag_idx++] = fossil_io_cstring_create("offensive");
    if (fossil_io_soap_detect_neutral(sanitized) && tag_idx < max_tags)
        f->tags[tag_idx++] = fossil_io_cstring_create("neutral");
    if (fossil_io_soap_detect_hype(sanitized) && tag_idx < max_tags)
        f->tags[tag_idx++] = fossil_io_cstring_create("hype");
    if (fossil_io_soap_detect_quality(sanitized) && tag_idx < max_tags)
        f->tags[tag_idx++] = fossil_io_cstring_create("quality");
    if (fossil_io_soap_detect_political(sanitized) && tag_idx < max_tags)
        f->tags[tag_idx++] = fossil_io_cstring_create("political");
    if (fossil_io_soap_detect_conspiracy(sanitized) && tag_idx < max_tags)
        f->tags[tag_idx++] = fossil_io_cstring_create("conspiracy");
    if (fossil_io_soap_detect_marketing(sanitized) && tag_idx < max_tags)
        f->tags[tag_idx++] = fossil_io_cstring_create("marketing");
    if (fossil_io_soap_detect_technobabble(sanitized) && tag_idx < max_tags)
        f->tags[tag_idx++] = fossil_io_cstring_create("technobabble");

    // Add readability and style tags
    if (tag_idx < max_tags) {
        const char *readability = fossil_io_soap_readability_label(sanitized);
        if (readability && readability[0])
            f->tags[tag_idx++] = fossil_io_cstring_create(readability);
    }
    if (tag_idx < max_tags) {
        const char *style = fossil_io_soap_analyze_style(sanitized);
        if (style && style[0])
            f->tags[tag_idx++] = fossil_io_cstring_create(style);
    }

    // If no tags found, add "untagged"
    if (tag_idx == 0) {
        f->tags[0] = fossil_io_cstring_create("untagged");
        tag_idx = 1;
    }

    f->tag_count = tag_idx;

    if (sanitized)
        free(sanitized);

    return 0;
}

int fossil_io_file_ai_compute_embedding(fossil_io_file_t *f, const void *model, size_t model_size)
{
    if (!f || !f->file || !f->readable) {
        return -1;
    }

    // Free previous embedding if present
    if (f->embedding) {
        free(f->embedding);
        f->embedding = NULL;
        f->embedding_size = 0;
    }

    // Only process text files
    if (f->is_binary) {
        f->embedding = NULL;
        f->embedding_size = 0;
        return 0;
    }

    // Read up to 4096 bytes for embedding computation
    long orig_pos = ftell(f->file);
    fseek(f->file, 0, SEEK_SET);
    char buffer[4096 + 1] = {0};
    size_t bytes = fread(buffer, 1, sizeof(buffer) - 1, f->file);
    buffer[bytes] = '\0';
    fseek(f->file, orig_pos, SEEK_SET);

    // Dummy embedding: for demonstration, just hash the buffer and fill a float array
    size_t embedding_dim = 32; // Example embedding dimension
    float *embedding_vec = (float *)malloc(sizeof(float) * embedding_dim);
    if (!embedding_vec) {
        return -1;
    }
    for (size_t i = 0; i < embedding_dim; ++i) {
        unsigned int hash = 0;
        for (size_t j = i; j < bytes; j += embedding_dim) {
            hash += (unsigned char)buffer[j];
        }
        embedding_vec[i] = (float)(hash % 997) / 997.0f;
    }

    f->embedding = embedding_vec;
    f->embedding_size = embedding_dim * sizeof(float);

    // Mark as compressed for AI storage
    f->compressed = true;

    return 0;
}

bool fossil_io_file_ai_ready(fossil_io_file_t *f)
{
    if (!f)
        return false;
    if (!f->file)
        return false;
    if (!f->is_open)
        return false;
    if (!f->readable)
        return false;
    if (f->is_binary)
        return false;
    return true;
}

void fossil_io_file_ai_reset(fossil_io_file_t *f)
{
    if (!f) return;

    // Free AI-generated tags
    for (int i = 0; i < 16; ++i) {
        if (f->tags[i]) {
            fossil_io_cstring_free(f->tags[i]);
            f->tags[i] = NULL;
        }
    }
    f->tag_count = 0;

    // Free embedding vector
    if (f->embedding) {
        free(f->embedding);
        f->embedding = NULL;
        f->embedding_size = 0;
    }

    // Reset analysis state
    f->indexed = false;
    f->analyzed = false;
    f->language[0] = '\0';
    f->sentiment = 0.0f;
    f->compressed = false;
    f->is_binary = false;
}

int fossil_io_file_add_tag(fossil_io_file_t *f, const char *tag)
{
    if (!f || !tag)
        return -1;
    if (f->tag_count >= 16)
        return -2;
    f->tags[f->tag_count] = fossil_io_cstring_create(tag);
    if (!f->tags[f->tag_count])
        return -3;
    f->tag_count++;
    return 0;
}

void fossil_io_file_detect_binary(fossil_io_file_t *f)
{
    if (!f || !f->file) {
        return;
    }

    // Read up to 1024 bytes from the start of the file
    long orig_pos = ftell(f->file);
    fseek(f->file, 0, SEEK_SET);
    unsigned char buffer[1024];
    size_t bytes = fread(buffer, 1, sizeof(buffer), f->file);
    fseek(f->file, orig_pos, SEEK_SET);

    // Heuristic: if any byte is 0 or in the range of typical binary values, mark as binary
    size_t nontext = 0;
    for (size_t i = 0; i < bytes; ++i) {
        if (buffer[i] == 0 || (buffer[i] < 8 || (buffer[i] > 13 && buffer[i] < 32))) {
            nontext++;
        }
    }

    if (nontext > 0) {
        f->is_binary = true;
        cstring type = fossil_io_cstring_create("binary");
        strncpy(f->type, type, sizeof(f->type) - 1);
        fossil_io_cstring_free(type);
    } else {
        f->is_binary = false;
        cstring type = fossil_io_cstring_create("text");
        strncpy(f->type, type, sizeof(f->type) - 1);
        fossil_io_cstring_free(type);
    }
    f->type[sizeof(f->type) - 1] = '\0';
}

int fossil_io_file_compress(fossil_io_file_t *f)
{
    if (!f || !f->file || !f->readable)
        return -1;

    // Only compress text files
    if (f->is_binary) {
        f->compressed = false;
        return 0;
    }

    // Read up to 4096 bytes for compression
    long orig_pos = ftell(f->file);
    fseek(f->file, 0, SEEK_SET);
    char buffer[4096 + 1] = {0};
    size_t bytes = fread(buffer, 1, sizeof(buffer) - 1, f->file);
    buffer[bytes] = '\0';
    fseek(f->file, orig_pos, SEEK_SET);

    // Simple RLE compression (for demonstration)
    char *compressed = (char *)malloc(bytes * 2 + 1); // Worst case
    if (!compressed)
        return -2;

    size_t ci = 0;
    for (size_t i = 0; i < bytes;) {
        char c = buffer[i];
        size_t run = 1;
        while (i + run < bytes && buffer[i + run] == c && run < 255)
            run++;
        compressed[ci++] = c;
        compressed[ci++] = (char)run;
        i += run;
    }
    compressed[ci] = '\0';

    // Free previous embedding if present
    if (f->embedding) {
        free(f->embedding);
        f->embedding = NULL;
        f->embedding_size = 0;
    }

    f->embedding = compressed;
    f->embedding_size = ci;
    f->compressed = true;

    return 0;
}

int fossil_io_file_decompress(fossil_io_file_t *f)
{
    if (!f || !f->compressed || !f->embedding || f->embedding_size == 0)
        return -1;

    // Only decompress if embedding is a char* (from RLE compression)
    char *compressed = (char *)f->embedding;
    size_t ci = 0, out_size = 0;

    // Estimate decompressed size (worst case: each run is length 1)
    size_t max_out = f->embedding_size * 255;
    char *decompressed = (char *)malloc(max_out + 1);
    if (!decompressed)
        return -2;

    while (ci + 1 < f->embedding_size) {
        char c = compressed[ci];
        unsigned char run = (unsigned char)compressed[ci + 1];
        for (unsigned char j = 0; j < run; ++j) {
            decompressed[out_size++] = c;
        }
        ci += 2;
    }
    decompressed[out_size] = '\0';

    // Free previous embedding
    free(f->embedding);
    f->embedding = decompressed;
    f->embedding_size = out_size;
    f->compressed = false;

    return 0;
}

