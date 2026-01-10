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
#include <sys/time.h>
#endif
#include <sys/stat.h>
#ifndef _WIN32
#include <pwd.h>
#include <grp.h>
#endif

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
size_t fossil_io_file_read(
    fossil_io_file_t *stream,
    void *buffer,
    size_t size,
    size_t count
) {
    /* Match fread(): undefined behavior on NULL is avoided,
       but no diagnostics are emitted */
    if (!stream || !buffer || !stream->file || size == 0 || count == 0) {
        return 0;
    }

    /* Enforce Fossil IO access policy, but preserve fread semantics */
    if (!stream->readable) {
        errno = EBADF;
        return 0;
    }

    /* Clear previous error state (optional but common) */
    clearerr(stream->file);

    size_t items_read = fread(buffer, size, count, stream->file);

    /* Update stream position if available */
    long pos = ftell(stream->file);
    if (pos >= 0) {
        stream->position = (size_t)pos;
    }

    /* Do NOT seek to compute size — fread never does this */

    return items_read;
}

// Write data to an open stream
size_t fossil_io_file_write(
    fossil_io_file_t *stream,
    const void *buffer,
    size_t size,
    size_t count
) {
    /* fread/fwrite return 0 on invalid input without diagnostics */
    if (!stream || !buffer || !stream->file || size == 0 || count == 0) {
        return 0;
    }

    /* Enforce Fossil IO policy without breaking fwrite semantics */
    if (!stream->writable) {
        errno = EBADF;
        return 0;
    }

    /* Clear prior error state (optional but conventional) */
    clearerr(stream->file);

    size_t items_written = fwrite(buffer, size, count, stream->file);

    /* Update position if seekable */
    long pos = ftell(stream->file);
    if (pos >= 0) {
        stream->position = (size_t)pos;
    }

    /* Do NOT seek to compute size — fwrite never does this */

    return items_written;
}

// Append data to the end of an open stream
int32_t fossil_io_file_append(
    fossil_io_file_t *stream,
    const void *buffer,
    size_t size,
    size_t count
) {
    if (!stream || !buffer || !stream->file || size == 0 || count == 0) {
        return 0;
    }

    if (!stream->writable) {
        errno = EBADF;
        return 0;
    }

    /* DO NOT SEEK — append mode guarantees EOF writes */
    clearerr(stream->file);

    size_t items_written = fwrite(buffer, size, count, stream->file);

    long pos = ftell(stream->file);
    if (pos >= 0) {
        stream->position = (size_t)pos;
    }

    return items_written;
}

// Seek to a specified position in an open stream
int32_t fossil_io_file_seek(fossil_io_file_t *stream, int64_t offset, int32_t origin) {
    if (!stream || !stream->file) {
        errno = EINVAL;
        return -1;
    }

    /* Clear error state (optional but conventional) */
    clearerr(stream->file);

    int rc = fseek(stream->file, offset, origin);

    /* Do NOT call ftell() — fseek does not */
    return rc;
}

// Get the current position of the file pointer in an open stream
int32_t fossil_io_file_tell(fossil_io_file_t *stream) {
    if (!stream || !stream->file) {
        errno = EINVAL;
        return -1L;
    }

    clearerr(stream->file);

    int32_t pos = ftell(stream->file);

    if (pos >= 0) {
        stream->position = (int32_t)pos;
    }

    /* No seeking, no printing, no size updates */
    return pos;
}

// Save an open stream to a new file
int32_t fossil_io_file_save(
    fossil_io_file_t *stream,
    const char *new_filename
) {
    if (!stream || !stream->file || !new_filename) {
        errno = EINVAL;
        return -1;
    }

    if (strlen(new_filename) >= sizeof(stream->filename)) {
        errno = ENAMETOOLONG;
        return -1;
    }

    /* Flush stdio buffers */
    if (fflush(stream->file) != 0) {
        return -1;
    }

    /* Preserve old name in case of failure */
    char old_name[sizeof(stream->filename)];
    strncpy(old_name, stream->filename, sizeof(old_name));

    /* Rename atomically */
    if (rename(old_name, new_filename) != 0) {
        return -1;
    }

    /* Update in-memory name only after success */
    strncpy(stream->filename, new_filename, sizeof(stream->filename) - 1);
    stream->filename[sizeof(stream->filename) - 1] = '\0';

    return 0;
}

// Copy a file from the source to the destination
int32_t fossil_io_file_copy(const char *source_filename, const char *destination_filename) {
    if (!source_filename || !destination_filename) return FOSSIL_ERROR_CNULL_POINTER;

    FILE *in = fopen(source_filename, "rb");
    if (!in) return FOSSIL_ERROR_FILE_NOT_FOUND;

    FILE *out = fopen(destination_filename, "wb");
    if (!out) {
        fclose(in);
        return FOSSIL_ERROR_IO;
    }

    char buf[FOSSIL_BUFFER_MEDIUM];
    size_t n;
    int error = 0;

    while ((n = fread(buf, 1, sizeof buf, in)) > 0) {
        if (fwrite(buf, 1, n, out) != n) {
            error = 1;
            break;
        }
    }

    if (ferror(in)) error = 1;

    fclose(in);
    fclose(out);

    if (error) {
        remove(destination_filename); /* prevent partial copy */
        return FOSSIL_ERROR_IO;
    }

    return FOSSIL_ERROR_OK;
}

int32_t fossil_io_file_remove(const char *filename) {
    if (!filename) return FOSSIL_ERROR_CNULL_POINTER;

    if (remove(filename) != 0) {
        if (errno == ENOENT)
            return FOSSIL_ERROR_FILE_NOT_FOUND;
        return FOSSIL_ERROR_IO;
    }

    return FOSSIL_ERROR_OK;
}

int32_t fossil_io_file_rename(const char *old_filename, const char *new_filename) {
    if (!old_filename || !new_filename)
        return FOSSIL_ERROR_CNULL_POINTER;

    if (rename(old_filename, new_filename) != 0) {
        if (errno == ENOENT)
            return FOSSIL_ERROR_FILE_NOT_FOUND;
        return FOSSIL_ERROR_IO;
    }

    return FOSSIL_ERROR_OK;
}

int32_t fossil_io_file_flush(fossil_io_file_t *stream) {
    if (!stream || !stream->file) return -1;

    if (fflush(stream->file) != 0) return -1;

    long pos = ftell(stream->file);
    if (pos >= 0)
        stream->position = (int64_t)pos;

#ifndef _WIN32
    struct stat st;
    if (stat(stream->filename, &st) == 0) {
        stream->mode = st.st_mode & 0777;
        stream->modified_at = st.st_mtime;
        stream->accessed_at = st.st_atime;
    }
#endif

    return 0;
}

int32_t fossil_io_file_setpos(fossil_io_file_t *stream, const fossil_io_pos_t *pos) {
    if (!stream || !stream->file || !pos) {
        return FOSSIL_ERROR_CNULL_POINTER;
    }

    if (fsetpos(stream->file, pos) != 0) {
        return FOSSIL_ERROR_IO;
    }

    /* Update cached byte position if available */
    long tell = ftell(stream->file);
    if (tell >= 0) {
        stream->position = (int64_t)tell;
    }

    return FOSSIL_ERROR_OK;
}

int32_t fossil_io_file_getpos(fossil_io_file_t *stream, int64_t *pos) {
    if (!stream || !stream->file || !pos) {
        errno = EINVAL;
        return -1;
    }

    long local_pos = ftell(stream->file);
    if (local_pos >= 0) {
        stream->position = (int64_t)local_pos;
        *pos = (int64_t)local_pos;
        return 0;
    }

    return -1;
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
    FILE *tmp_fp = fopen(temp_filename, "wb+");
    int fd = tmp_fp ? fileno(tmp_fp) : -1;
    if (tmp_fp) fclose(tmp_fp);
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

int fossil_io_file_ai_compute_embedding(fossil_io_file_t *f, const void *model, size_t model_size)
{
    if (!f || !f->file || !f->readable) {
        return -1;
    }

    // Use model and model_size to influence embedding dimension (dummy usage)
    size_t embedding_dim = 32;
    if (model && model_size > 0) {
        // For demonstration, use the first byte of model (if available) to alter embedding_dim
        const unsigned char *model_bytes = (const unsigned char *)model;
        embedding_dim = 16 + (model_bytes[0] % 48); // Range: 16-63
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
    float *embedding_vec = (float *)malloc(sizeof(float) * embedding_dim);
    if (!embedding_vec) {
        return -1;
    }
    for (size_t i = 0; i < embedding_dim; ++i) {
        unsigned int hash = 0;
        for (size_t j = i; j < bytes; j += embedding_dim) {
            hash += (unsigned char)buffer[j];
        }
        // Mix in model data if available
        if (model && model_size > 0) {
            hash ^= ((const unsigned char *)model)[i % model_size];
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

// ------------------------------------------------------------
// Internal helpers
// ------------------------------------------------------------
static int fossil_copy_file_metadata(const char *src, const char *dest) {
#ifndef _WIN32
    struct stat st;
    if (stat(src, &st) != 0)
        return -errno;

    // chmod
    if (chmod(dest, st.st_mode) != 0)
        return -errno;

    // timestamps
    struct timeval times[2];
    times[0].tv_sec  = st.st_atime;
    times[0].tv_usec = 0;
    times[1].tv_sec  = st.st_mtime;
    times[1].tv_usec = 0;

    if (utimes(dest, times) != 0)
        return -errno;

    return 0;

#else
    // Windows does not support portable chmod/timestamp copying in this mode.
    // Explicitly mark parameters as used to avoid -Werror=unused-parameter.
    (void)src;
    (void)dest;

    return 0;
#endif
}

static void fossil_io_file_reset(fossil_io_file_t *f, const char *path) {
    memset(f, 0, sizeof(*f));
    f->fd = -1;
    f->file = NULL;
    strncpy(f->filename, path, sizeof(f->filename) - 1);
}

// ------------------------------------------------------------
// Main function
// ------------------------------------------------------------
int fossil_io_file_link(
        const fossil_io_file_t *src,
        fossil_io_file_t *dest,
        const char *dest_path,
        bool symbolic,
        bool copy_meta)
{
    if (!src || !dest_path)
        return -EINVAL;

    int rc = 0;

#ifdef _WIN32
    if (symbolic) {
        DWORD flags = 0;   // no SYMBOLIC_LINK_FLAG_FILE — safe for Windows 7–11
        if (!CreateSymbolicLinkA(dest_path, src->filename, flags))
            return -(int)GetLastError();
    } else {
        if (!CreateHardLinkA(dest_path, src->filename, NULL))
            return -(int)GetLastError();
    }
#else
    if (symbolic) {
        if (symlink(src->filename, dest_path) != 0)
            return -errno;
    } else {
        if (link(src->filename, dest_path) != 0)
            return -errno;
    }
#endif

    // --------------------------------------------------------
    // Populate destination structure (optional)
    // --------------------------------------------------------
    if (dest != NULL) {
        fossil_io_file_reset(dest, dest_path);

        // Hard link → real file, Symlink → metadata follows actual target
        strncpy(dest->type, src->type, sizeof(dest->type));
        dest->mode = src->mode;
        dest->readable  = src->readable;
        dest->writable  = src->writable;
        dest->executable = src->executable;

        dest->is_open = false;
        dest->indexed = false;
        dest->analyzed = false;

        dest->size = src->size;
        dest->created_at  = src->created_at;
        dest->modified_at = src->modified_at;
        dest->accessed_at = src->accessed_at;

        dest->is_binary = src->is_binary;

        // Reset AI fields; these are content-specific and should not be inherited
        memset(dest->tags, 0, sizeof(dest->tags));
        dest->tag_count = 0;
        dest->embedding = NULL;
        dest->embedding_size = 0;
        dest->sentiment = 0.0f;
        memset(dest->language, 0, sizeof(dest->language));
    }

    // --------------------------------------------------------
    // Metadata propagation (optional)
    // --------------------------------------------------------
    if (copy_meta) {
        rc = fossil_copy_file_metadata(src->filename, dest_path);
        if (rc < 0)
            return rc;
    }

    return 0;
}
