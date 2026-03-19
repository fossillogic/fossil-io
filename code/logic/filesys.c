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
#include "fossil/io/filesys.h"

#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h> // malloc, free, realpath
#include <string.h>
#include <time.h>
#include <unistd.h>    // truncate, ftruncate, getcwd, chdir
#include <limits.h>    // PATH_MAX
#include <sys/stat.h>  // stat, mkdir
#include <sys/types.h> // sometimes needed for mode_t
#include <errno.h>     // optional, if checking errors
#include <fcntl.h>     // open, O_* flags

/* Ensure realpath is available on all POSIX systems */
#ifndef _WIN32
#define _DEFAULT_SOURCE
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#ifdef _WIN32
#include <windows.h>
#include <fileapi.h>
#include <sys/stat.h>
#else
#include <sys/stat.h>
#include <libgen.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#endif

#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#include <dirent.h>
#endif

#if defined(_WIN32)
#define PATH_SEP '\\'
#else
#define PATH_SEP '/'
#endif

#define FOSSIL_MERGE_CONCAT 1
#define FOSSIL_MERGE_INTERLEAVE 2

/* ------------------------------------------------------------
 * Internal hash algorithms
 * ------------------------------------------------------------ */

static inline uint64_t rotl64(uint64_t x, unsigned int r)
{
    return (x << r) | (x >> (64 - r));
}

static uint64_t fossil_hash_file(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f)
        return 0;

    uint64_t hash = 14695981039346656037ULL; // FNV offset basis
    const uint64_t prime = 1099511628211ULL;

    unsigned char buf[8192];
    size_t n;

    while ((n = fread(buf, 1, sizeof(buf), f)) > 0)
    {
        for (size_t i = 0; i < n; ++i)
        {
            hash ^= buf[i];
            hash *= prime;
            hash ^= (hash >> 33);
            hash = rotl64(hash, 27);
        }
    }

    fclose(f);

    // final avalanche
    hash ^= hash >> 33;
    hash *= 0xff51afd7ed558ccdULL;
    hash ^= hash >> 33;

    return hash;
}

static uint64_t hash_fnv1a(FILE *f)
{
    if (!f)
        return 0;

    uint64_t hash = 14695981039346656037ULL; // FNV offset basis
    const uint64_t prime = 1099511628211ULL;

    unsigned char buf[8192];
    size_t n;

    while ((n = fread(buf, 1, sizeof(buf), f)) > 0)
    {
        for (size_t i = 0; i < n; i++)
        {
            hash ^= buf[i];
            hash *= prime;
            hash ^= (hash >> 33);
            hash = rotl64(hash, 27);
        }
    }

    // final avalanche
    hash ^= hash >> 33;
    hash *= 0xff51afd7ed558ccdULL;
    hash ^= hash >> 33;

    return hash;
}

/* ------------------------------------------------------------
 * Internal utilites and platform abstractions
 * ------------------------------------------------------------ */

int32_t fossil_path_resolve(const char *path, char *out, size_t max_len)
{
    if (!path || !out) return -1;

    // Absolute path → just copy
    if (path[0] == '/') {
        strncpy(out, path, max_len - 1);
        out[max_len - 1] = '\0';
        return 0;
    }

    // Relative path → prepend cwd
    char cwd[PATH_MAX];
    if (!getcwd(cwd, sizeof(cwd)))
        return -1;

    snprintf(out, max_len, "%s/%s", cwd, path);
    return 0;
}

static int copy_file_stream(const char *src, const char *dest)
{
    FILE *in = fopen(src, "rb");
    if (!in)
        return -1;

    FILE *out = fopen(dest, "wb");
    if (!out)
    {
        fclose(in);
        return -1;
    }

    unsigned char buf[8192];
    size_t n;

    while ((n = fread(buf, 1, sizeof(buf), in)) > 0)
    {
        if (fwrite(buf, 1, n, out) != n)
        {
            fclose(in);
            fclose(out);
            return -1;
        }
    }

    fclose(in);
    fclose(out);
    return 0;
}

static int file_needs_update(const char *src, const char *dest)
{
#if defined(_WIN32)

    WIN32_FILE_ATTRIBUTE_DATA s, d;

    if (!GetFileAttributesExA(src, GetFileExInfoStandard, &s))
        return 1;

    if (!GetFileAttributesExA(dest, GetFileExInfoStandard, &d))
        return 1;

    uint64_t ssize = ((uint64_t)s.nFileSizeHigh << 32) | s.nFileSizeLow;
    uint64_t dsize = ((uint64_t)d.nFileSizeHigh << 32) | d.nFileSizeLow;

    if (ssize != dsize)
        return 1;

    return (CompareFileTime(&s.ftLastWriteTime, &d.ftLastWriteTime) != 0);

#else

    struct stat s, d;

    if (stat(src, &s) != 0)
        return 1;

    if (stat(dest, &d) != 0)
        return 1;

    if (s.st_size != d.st_size)
        return 1;

    return (s.st_mtime != d.st_mtime);

#endif
}

static int remove_path(const char *path)
{
#if defined(_WIN32)
    WIN32_FILE_ATTRIBUTE_DATA attr;
    if (!GetFileAttributesExA(path, GetFileExInfoStandard, &attr))
        return -1;

    if (attr.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        char search[FOSSIL_FILESYS_MAX_PATH];
        snprintf(search, sizeof(search), "%s\\*", path);

        WIN32_FIND_DATAA fd;
        HANDLE h = FindFirstFileA(search, &fd);

        if (h != INVALID_HANDLE_VALUE)
        {
            do
            {
                if (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, ".."))
                    continue;

                char full[FOSSIL_FILESYS_MAX_PATH];
                snprintf(full, sizeof(full), "%s\\%s", path, fd.cFileName);
                remove_path(full);

            } while (FindNextFileA(h, &fd));

            FindClose(h);
        }

        return RemoveDirectoryA(path);
    }
    else
    {
        return DeleteFileA(path);
    }

#else

    struct stat st;
    if (lstat(path, &st) != 0)
        return -1;

    if (S_ISDIR(st.st_mode))
    {
        DIR *dir = opendir(path);
        if (!dir)
            return -1;

        struct dirent *entry;
        char full[512];

        while ((entry = readdir(dir)))
        {
            if (!strcmp(entry->d_name, ".") ||
                !strcmp(entry->d_name, ".."))
                continue;

            snprintf(full, sizeof(full), "%s/%s", path, entry->d_name);
            remove_path(full);
        }

        closedir(dir);
        return rmdir(path);
    }
    else
    {
        return unlink(path);
    }

#endif
}

static int mirror_recursive(
    const char *src,
    const char *dest,
    bool delete_extras)
{
    /* ensure destination exists */
    fossil_io_filesys_dir_create(dest, true);

#if defined(_WIN32)

    WIN32_FIND_DATAA fd;
    char search[FOSSIL_FILESYS_MAX_PATH];
    snprintf(search, sizeof(search), "%s\\*", src);

    HANDLE h = FindFirstFileA(search, &fd);
    if (h == INVALID_HANDLE_VALUE)
        return -1;

    do
    {
        if (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, ".."))
            continue;

        char sfull[FOSSIL_FILESYS_MAX_PATH], dfull[FOSSIL_FILESYS_MAX_PATH];
        snprintf(sfull, sizeof(sfull), "%s\\%s", src, fd.cFileName);
        snprintf(dfull, sizeof(dfull), "%s\\%s", dest, fd.cFileName);

        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            mirror_recursive(sfull, dfull, delete_extras);
        }
        else
        {
            if (file_needs_update(sfull, dfull))
                copy_file_stream(sfull, dfull);
        }

    } while (FindNextFileA(h, &fd));

    FindClose(h);

#else

    DIR *dir = opendir(src);
    if (!dir)
        return -1;

    struct dirent *entry;
    char sfull[512], dfull[512];

    while ((entry = readdir(dir)))
    {
        if (!strcmp(entry->d_name, ".") ||
            !strcmp(entry->d_name, ".."))
            continue;

        snprintf(sfull, sizeof(sfull), "%s/%s", src, entry->d_name);
        snprintf(dfull, sizeof(dfull), "%s/%s", dest, entry->d_name);

        struct stat st;
        if (lstat(sfull, &st) != 0)
            continue;

        if (S_ISDIR(st.st_mode))
        {
            mirror_recursive(sfull, dfull, delete_extras);
        }
        else if (S_ISREG(st.st_mode))
        {
            if (file_needs_update(sfull, dfull))
                copy_file_stream(sfull, dfull);
        }
    }

    closedir(dir);

#endif

    /* delete extras */
    if (delete_extras)
    {
#if defined(_WIN32)

        char search[FOSSIL_FILESYS_MAX_PATH];
        snprintf(search, sizeof(search), "%s\\*", dest);

        WIN32_FIND_DATAA fd;
        HANDLE h = FindFirstFileA(search, &fd);

        if (h != INVALID_HANDLE_VALUE)
        {
            do
            {
                if (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, ".."))
                    continue;

                char dfull[FOSSIL_FILESYS_MAX_PATH], sfull[FOSSIL_FILESYS_MAX_PATH];
                snprintf(dfull, sizeof(dfull), "%s\\%s", dest, fd.cFileName);
                snprintf(sfull, sizeof(sfull), "%s\\%s", src, fd.cFileName);

                if (GetFileAttributesA(sfull) == INVALID_FILE_ATTRIBUTES)
                    remove_path(dfull);

            } while (FindNextFileA(h, &fd));

            FindClose(h);
        }

#else

        DIR *dir2 = opendir(dest);
        if (dir2)
        {
            struct dirent *entry;
            char dfull[512], sfull[512];

            while ((entry = readdir(dir2)))
            {
                if (!strcmp(entry->d_name, ".") ||
                    !strcmp(entry->d_name, ".."))
                    continue;

                snprintf(dfull, sizeof(dfull), "%s/%s", dest, entry->d_name);
                snprintf(sfull, sizeof(sfull), "%s/%s", src, entry->d_name);

                if (access(sfull, F_OK) != 0)
                    remove_path(dfull);
            }

            closedir(dir2);
        }

#endif
    }

    return 0;
}

typedef struct dedup_entry
{
    uint64_t hash;
    size_t size;
    char path[FOSSIL_FILESYS_MAX_PATH];
} dedup_entry_t;

#define DEDUP_MAX 8192

static int dedup_walk(
    const char *path,
    bool recursive,
    dedup_entry_t *table,
    size_t *count)
{
#if defined(_WIN32)

    WIN32_FIND_DATAA fd;
    char search[FOSSIL_FILESYS_MAX_PATH];
    snprintf(search, sizeof(search), "%s\\*", path);

    HANDLE h = FindFirstFileA(search, &fd);
    if (h == INVALID_HANDLE_VALUE)
        return -1;

    do
    {
        if (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, ".."))
            continue;

        char full[FOSSIL_FILESYS_MAX_PATH];
        snprintf(full, sizeof(full), "%s\\%s", path, fd.cFileName);

        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (recursive)
                dedup_walk(full, true, table, count);
            continue;
        }

        size_t size =
            ((size_t)fd.nFileSizeHigh << 32) | fd.nFileSizeLow;

#else

    DIR *dir = opendir(path);
    if (!dir)
        return -1;

    struct dirent *entry;
    char full[512];

    while ((entry = readdir(dir)))
    {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        snprintf(full, sizeof(full), "%s/%s", path, entry->d_name);

        struct stat st;
        if (lstat(full, &st) != 0)
            continue;

        if (S_ISDIR(st.st_mode))
        {
            if (recursive)
                dedup_walk(full, true, table, count);
            continue;
        }

        if (!S_ISREG(st.st_mode))
            continue;

        size_t size = st.st_size;

#endif

        /* compute hash */
        uint64_t hash = fossil_hash_file(full);

        /* check duplicates */
        for (size_t i = 0; i < *count; ++i)
        {
            if (table[i].hash == hash && table[i].size == size)
            {
                /* duplicate → remove */
#if defined(_WIN32)
                DeleteFileA(full);
#else
                unlink(full);
#endif
                return 1; /* one removed */
            }
        }

        /* insert */
        if (*count < DEDUP_MAX)
        {
            table[*count].hash = hash;
            table[*count].size = size;
            snprintf(table[*count].path, sizeof(table[*count].path), "%s", full);
            (*count)++;
        }

#if defined(_WIN32)
    } while (FindNextFileA(h, &fd));

    FindClose(h);

#else
    }

    closedir(dir);

#endif

    return 0;
}

static int remove_recursive(const char *path)
{
#if defined(_WIN32)
    WIN32_FIND_DATAA fd;
    char search[FOSSIL_FILESYS_MAX_PATH];
    snprintf(search, sizeof(search), "%s\\*", path);

    HANDLE h = FindFirstFileA(search, &fd);
    if (h == INVALID_HANDLE_VALUE)
        return -1;

    do
    {
        if (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, ".."))
            continue;

        char full[FOSSIL_FILESYS_MAX_PATH];
        snprintf(full, sizeof(full), "%s\\%s", path, fd.cFileName);

        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            remove_recursive(full);
            RemoveDirectoryA(full);
        }
        else
        {
            DeleteFileA(full);
        }

    } while (FindNextFileA(h, &fd));

    FindClose(h);
    return RemoveDirectoryA(path) ? 0 : -1;

#else

        DIR *dir = opendir(path);
        if (!dir)
            return -1;

        struct dirent *entry;
        char full[512];

        while ((entry = readdir(dir)))
        {
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
                continue;

            snprintf(full, sizeof(full), "%s/%s", path, entry->d_name);

            struct stat st;
            if (lstat(full, &st) != 0)
                continue;

            if (S_ISDIR(st.st_mode))
            {
                remove_recursive(full);
                rmdir(full);
            }
            else
            {
                unlink(full);
            }
        }

        closedir(dir);
        return rmdir(path);

#endif
}

/* ------------------------------------------------------------
 * Thread / Mutex Abstraction
 * ------------------------------------------------------------ */

#if defined(_WIN32)
#include <windows.h>
typedef HANDLE fossil_mutex_native_t;
#else
#include <pthread.h>
    typedef pthread_mutex_t fossil_mutex_native_t;
#endif

static int fossil_mutex_init(fossil_io_filesys_lock_t *lock)
{
    if (!lock)
        return -1;

#if defined(_WIN32)
    lock->handle = CreateMutex(NULL, FALSE, NULL);
    if (!lock->handle)
        return -1;
#else
        lock->handle = malloc(sizeof(pthread_mutex_t));
        if (!lock->handle)
            return -1;
        if (pthread_mutex_init((pthread_mutex_t *)lock->handle, NULL) != 0)
            return -1;
#endif

    lock->locked = false;
    return 0;
}

static int fossil_mutex_lock(fossil_io_filesys_lock_t *lock)
{
#if defined(_WIN32)
    WaitForSingleObject(lock->handle, INFINITE);
#else
        pthread_mutex_lock((pthread_mutex_t *)lock->handle);
#endif
    lock->locked = true;
    return 0;
}

static int fossil_mutex_unlock(fossil_io_filesys_lock_t *lock)
{
#if defined(_WIN32)
    ReleaseMutex(lock->handle);
#else
        pthread_mutex_unlock((pthread_mutex_t *)lock->handle);
#endif
    lock->locked = false;
    return 0;
}

/* ------------------------------------------------------------
 * General Filesystem Operations
 * ------------------------------------------------------------ */

static void fossil_generate_id(const char *path, char *out, size_t len)
{
    /* simple stable hash (FNV-1a) */
    uint32_t hash = 2166136261u;

    for (const char *p = path; *p; ++p)
    {
        hash ^= (unsigned char)(*p);
        hash *= 16777619;
    }

    snprintf(out, len, "%08x", hash);
}

int32_t fossil_io_filesys_init(fossil_io_filesys_obj_t *obj, const char *path)
{
    if (!obj || !path)
        return -1;

    memset(obj, 0, sizeof(*obj));

    /* copy path safely */
    strncpy(obj->path, path, FOSSIL_FILESYS_MAX_PATH - 1);

    /* init lock */
    if (fossil_mutex_init(&obj->lock) != 0)
        return -1;

    /* generate stable ID */
    fossil_generate_id(path, obj->id, sizeof(obj->id));

    /* initial metadata load */
    if (fossil_io_filesys_refresh(obj) != 0)
        return -1;

    return 0;
}

int32_t fossil_io_filesys_refresh(fossil_io_filesys_obj_t *obj)
{
    if (!obj || !obj->path[0])
        return -1;

    fossil_mutex_lock(&obj->lock);

#if defined(_WIN32)

    WIN32_FILE_ATTRIBUTE_DATA data;
    if (!GetFileAttributesExA(obj->path, GetFileExInfoStandard, &data))
    {
        fossil_mutex_unlock(&obj->lock);
        return -1;
    }

    obj->size =
        ((size_t)data.nFileSizeHigh << 32) | data.nFileSizeLow;

    obj->created_at = data.ftCreationTime.dwLowDateTime;
    obj->modified_at = data.ftLastWriteTime.dwLowDateTime;
    obj->accessed_at = data.ftLastAccessTime.dwLowDateTime;

    if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        obj->type = FOSSIL_FILESYS_TYPE_DIR;
    else
        obj->type = FOSSIL_FILESYS_TYPE_FILE;

#else

        struct stat st;
        if (lstat(obj->path, &st) != 0)
        {
            fossil_mutex_unlock(&obj->lock);
            return -1;
        }

        obj->size = st.st_size;
        obj->mode = st.st_mode;

        obj->created_at = st.st_ctime;
        obj->modified_at = st.st_mtime;
        obj->accessed_at = st.st_atime;

        if (S_ISREG(st.st_mode))
            obj->type = FOSSIL_FILESYS_TYPE_FILE;
        else if (S_ISDIR(st.st_mode))
            obj->type = FOSSIL_FILESYS_TYPE_DIR;
        else if (S_ISLNK(st.st_mode))
            obj->type = FOSSIL_FILESYS_TYPE_LINK;
        else
            obj->type = FOSSIL_FILESYS_TYPE_UNKNOWN;

        obj->perms.read = (st.st_mode & S_IRUSR) != 0;
        obj->perms.write = (st.st_mode & S_IWUSR) != 0;
        obj->perms.execute = (st.st_mode & S_IXUSR) != 0;

#endif

    fossil_mutex_unlock(&obj->lock);
    return 0;
}

int32_t fossil_io_filesys_exists(const char *path)
{
    if (!path)
        return -1;

#if defined(_WIN32)
    DWORD attr = GetFileAttributesA(path);
    if (attr == INVALID_FILE_ATTRIBUTES)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND ||
            GetLastError() == ERROR_PATH_NOT_FOUND)
            return 0;
        return -1;
    }
    return 1;
#else
        struct stat st;
        if (lstat(path, &st) == 0)
            return 1;

        if (errno == ENOENT)
            return 0;

        return -1;
#endif
}

int32_t fossil_io_filesys_remove(const char *path, bool recursive)
{
    if (!path)
        return -1;

#if defined(_WIN32)

    DWORD attr = GetFileAttributesA(path);
    if (attr == INVALID_FILE_ATTRIBUTES)
        return -1;

    if (attr & FILE_ATTRIBUTE_DIRECTORY)
    {
        if (!recursive)
            return RemoveDirectoryA(path) ? 0 : -1;

        return remove_recursive(path);
    }
    else
    {
        return DeleteFileA(path) ? 0 : -1;
    }

#else

        struct stat st;
        if (lstat(path, &st) != 0)
            return -1;

        if (S_ISDIR(st.st_mode))
        {
            if (!recursive)
                return rmdir(path);

            return remove_recursive(path);
        }
        else
        {
            return unlink(path);
        }

#endif
}

int32_t fossil_io_filesys_move(const char *src, const char *dest)
{
    if (!src || !dest)
        return -1;

#if defined(_WIN32)
    if (MoveFileExA(src, dest, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
        return 0;
    return -1;
#else
        if (rename(src, dest) == 0)
            return 0;
        return -1;
#endif
}

int32_t fossil_io_filesys_copy(const char *src, const char *dest, bool preserve_meta)
{
    FILE *in = fopen(src, "rb");
    if (!in)
        return -1;

    FILE *out = fopen(dest, "wb");
    if (!out)
    {
        fclose(in);
        return -1;
    }

    char buffer[8192];
    size_t n;

    while ((n = fread(buffer, 1, sizeof(buffer), in)) > 0)
    {
        if (fwrite(buffer, 1, n, out) != n)
        {
            fclose(in);
            fclose(out);
            return -1;
        }
    }

    fclose(in);
    fclose(out);

    /* Optional metadata copy */
    if (preserve_meta)
    {
        struct stat st;
        if (stat(src, &st) == 0)
        {
            chmod(dest, st.st_mode);
        }
    }

    return 0;
}

int32_t fossil_io_filesys_swap(const char *path1, const char *path2)
{
    if (!path1 || !path2)
        return -1;

    char tmp[FOSSIL_FILESYS_MAX_PATH];
    snprintf(tmp, sizeof(tmp), "%s.swap.tmp", path1);

    /* Step 1: path1 -> tmp */
    if (rename(path1, tmp) != 0)
        return -1;

    /* Step 2: path2 -> path1 */
    if (rename(path2, path1) != 0)
    {
        rename(tmp, path1); /* rollback */
        return -1;
    }

    /* Step 3: tmp -> path2 */
    if (rename(tmp, path2) != 0)
    {
        /* catastrophic but recoverable */
        rename(path1, path2);
        rename(tmp, path1);
        return -1;
    }

    return 0;
}

int32_t fossil_io_filesys_deduplicate(const char *path, bool recursive)
{
    if (!path)
        return -1;

    dedup_entry_t table[DEDUP_MAX];
    size_t count = 0;
    int removed = 0;

#if defined(_WIN32)
    removed += dedup_walk(path, recursive, table, &count);
#else
        removed += dedup_walk(path, recursive, table, &count);
#endif

    return removed;
}

int32_t fossil_io_filesys_stat(const char *path, fossil_io_filesys_obj_t *obj)
{
    if (!path || !obj)
        return -1;

    /* If already initialized, just refresh */
    if (obj->path[0] != '\0')
    {
        if (strncmp(obj->path, path, FOSSIL_FILESYS_MAX_PATH) == 0)
            return fossil_io_filesys_refresh(obj);
    }

    /* Otherwise initialize fresh */
    return fossil_io_filesys_init(obj, path);
}

/* File Operations */

// feature from file stream preservide for filesys
typedef struct
{
    const char *keyword;
    const char *mode;
} fossil_io_file_mode_entry_t;

static const fossil_io_file_mode_entry_t fossil_io_file_mode_table[] = {
    // Classic C modes (standard fopen strings)
    {"r", "r"},
    {"rb", "rb"},
    {"w", "w"},
    {"wb", "wb"},
    {"a", "a"},
    {"ab", "ab"},
    {"r+", "r+"},
    {"rb+", "r+b"},
    {"r+b", "r+b"},
    {"w+", "w+"},
    {"wb+", "w+b"},
    {"w+b", "w+b"},
    {"a+", "a+"},
    {"ab+", "a+b"},
    {"a+b", "a+b"},

    // Extended readable modes
    {"read", "r"},
    {"readb", "rb"},
    {"write", "w"},
    {"writeb", "wb"},
    {"append", "a"},
    {"appendb", "ab"},
    {"read+write", "r+"},
    {"read+writeb", "r+b"},
    {"write+read", "w+"},
    {"write+readb", "w+b"},
    {"append+read", "a+"},
    {"append+readb", "a+b"},
    {"read+t", "rt"},
    {"write+t", "wt"},
    {"read+write+t", "r+t"},

    {NULL, NULL}};

int32_t fossil_io_filesys_file_open(
    fossil_io_filesys_file_t *f,
    const char *path,
    const char *mode)
{
    if (!f || !path || !mode)
        return -1;

    memset(f, 0, sizeof(*f));

    /* Translate mode keyword to standard fopen mode */
    const char *actual_mode = mode;
    for (size_t i = 0; fossil_io_file_mode_table[i].keyword != NULL; ++i)
    {
        if (strcmp(fossil_io_file_mode_table[i].keyword, mode) == 0)
        {
            actual_mode = fossil_io_file_mode_table[i].mode;
            break;
        }
    }

    /* init base object */
    if (fossil_io_filesys_init(&f->base, path) != 0)
        return -1;

    fossil_mutex_lock(&f->base.lock);

    FILE *fp = fopen(path, actual_mode);
    if (!fp)
    {
        fossil_mutex_unlock(&f->base.lock);
        return -1;
    }

    f->handle = fp;
    f->fd = fileno(fp);
    f->is_open = true;
    f->append_mode = (strchr(actual_mode, 'a') != NULL);
    f->position = 0;

    fossil_mutex_unlock(&f->base.lock);
    return 0;
}

int32_t fossil_io_filesys_file_close(fossil_io_filesys_file_t *f)
{
    if (!f || !f->is_open)
        return -1;

    fossil_mutex_lock(&f->base.lock);

    FILE *fp = (FILE *)f->handle;
    int rc = fclose(fp);

    f->handle = NULL;
    f->is_open = false;

    fossil_mutex_unlock(&f->base.lock);
    return (rc == 0) ? 0 : -1;
}

size_t fossil_io_filesys_file_read(
    fossil_io_filesys_file_t *f,
    void *buf,
    size_t size,
    size_t count)
{
    if (!f || !f->is_open || !buf)
        return 0;

    fossil_mutex_lock(&f->base.lock);

    FILE *fp = (FILE *)f->handle;
    size_t n = fread(buf, size, count, fp);

    f->position = ftell(fp);

    fossil_mutex_unlock(&f->base.lock);
    return n;
}

size_t fossil_io_filesys_file_write(
    fossil_io_filesys_file_t *f,
    const void *buf,
    size_t size,
    size_t count)
{
    if (!f || !f->is_open || !buf)
        return 0;

    fossil_mutex_lock(&f->base.lock);

    FILE *fp = (FILE *)f->handle;

    if (f->append_mode)
        fseek(fp, 0, SEEK_END);

    size_t n = fwrite(buf, size, count, fp);

    f->position = ftell(fp);

    fossil_mutex_unlock(&f->base.lock);
    return n;
}

int32_t fossil_io_filesys_file_seek(
    fossil_io_filesys_file_t *f,
    int64_t offset,
    int32_t origin)
{
    if (!f || !f->is_open)
        return -1;

    fossil_mutex_lock(&f->base.lock);

    FILE *fp = (FILE *)f->handle;

#if defined(_WIN32)
    int rc = _fseeki64(fp, offset, origin);
#else
        int rc = fseeko(fp, offset, origin);
#endif

    if (rc == 0)
        f->position = ftell(fp);

    fossil_mutex_unlock(&f->base.lock);
    return (rc == 0) ? 0 : -1;
}

int64_t fossil_io_filesys_file_tell(fossil_io_filesys_file_t *f)
{
    if (!f || !f->is_open)
        return -1;

    fossil_mutex_lock(&f->base.lock);

    FILE *fp = (FILE *)f->handle;

#if defined(_WIN32)
    int64_t pos = _ftelli64(fp);
#else
        int64_t pos = ftello(fp);
#endif

    f->position = pos;

    fossil_mutex_unlock(&f->base.lock);
    return pos;
}

int32_t fossil_io_filesys_file_flush(fossil_io_filesys_file_t *f)
{
    if (!f || !f->is_open)
        return -1;

    fossil_mutex_lock(&f->base.lock);

    FILE *fp = (FILE *)f->handle;
    int rc = fflush(fp);

#if defined(_WIN32)
    _commit(_fileno(fp));
#else
        fsync(fileno(fp));
#endif

    fossil_mutex_unlock(&f->base.lock);
    return (rc == 0) ? 0 : -1;
}

int32_t fossil_io_filesys_file_size(const char *path)
{
    if (!path)
        return -1;

#if defined(_WIN32)
    WIN32_FILE_ATTRIBUTE_DATA data;
    if (!GetFileAttributesExA(path, GetFileExInfoStandard, &data))
        return -1;

    return (int32_t)(((uint64_t)data.nFileSizeHigh << 32) | data.nFileSizeLow);
#else
        struct stat st;
        if (stat(path, &st) != 0)
            return -1;

        return (int32_t)st.st_size;
#endif
}

int32_t fossil_io_filesys_file_truncate(const char *path, size_t size)
{
    if (!path)
        return -1;

#if defined(_WIN32)
    HANDLE h = CreateFileA(path, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE)
        return -1;

    LARGE_INTEGER li;
    li.QuadPart = size;

    if (!SetFilePointerEx(h, li, NULL, FILE_BEGIN) ||
        !SetEndOfFile(h))
    {
        CloseHandle(h);
        return -1;
    }

    CloseHandle(h);
    return 0;
#else
        return truncate(path, size);
#endif
}

int32_t fossil_io_filesys_file_hash(
    const char *path,
    unsigned char *hash_out,
    size_t hash_size,
    const char *algorithm)
{
    if (!path || !hash_out || !algorithm)
        return -1;

    FILE *f = fopen(path, "rb");
    if (!f)
        return -1;

    if (strcmp(algorithm, "fnv1a") == 0)
    {
        uint64_t h = hash_fnv1a(f);

        if (hash_size < sizeof(uint64_t))
        {
            fclose(f);
            return -1;
        }

        memcpy(hash_out, &h, sizeof(uint64_t));
    }
    else
    {
        fclose(f);
        return -1; /* unsupported */
    }

    fclose(f);
    return 0;
}

int32_t fossil_io_filesys_file_merge(
    const char *src1,
    const char *src2,
    const char *dest,
    uint32_t strategy)
{
    FILE *a = fopen(src1, "rb");
    FILE *b = fopen(src2, "rb");
    FILE *out = fopen(dest, "wb");

    if (!a || !b || !out)
        return -1;

    char buf1[4096], buf2[4096];

    if (strategy == FOSSIL_MERGE_CONCAT)
    {
        size_t n;
        while ((n = fread(buf1, 1, sizeof(buf1), a)) > 0)
            fwrite(buf1, 1, n, out);

        while ((n = fread(buf2, 1, sizeof(buf2), b)) > 0)
            fwrite(buf2, 1, n, out);
    }
    else if (strategy == FOSSIL_MERGE_INTERLEAVE)
    {
        size_t n1, n2;
        do
        {
            n1 = fread(buf1, 1, sizeof(buf1), a);
            if (n1)
                fwrite(buf1, 1, n1, out);

            n2 = fread(buf2, 1, sizeof(buf2), b);
            if (n2)
                fwrite(buf2, 1, n2, out);

        } while (n1 || n2);
    }

    fclose(a);
    fclose(b);
    fclose(out);
    return 0;
}

int32_t fossil_io_filesys_file_split(
    const char *path,
    size_t part_size,
    const char *prefix)
{
    if (!path || !prefix || part_size == 0)
        return -1;

    FILE *in = fopen(path, "rb");
    if (!in)
        return -1;

    unsigned char *buffer = malloc(part_size);
    if (!buffer)
    {
        fclose(in);
        return -1;
    }

    size_t part = 0;
    size_t read_bytes;

    while ((read_bytes = fread(buffer, 1, part_size, in)) > 0)
    {
        char out_path[512];
        snprintf(out_path, sizeof(out_path),
                 "%s.part%03zu", prefix, part++);

        FILE *out = fopen(out_path, "wb");
        if (!out)
        {
            free(buffer);
            fclose(in);
            return -1;
        }

        fwrite(buffer, 1, read_bytes, out);
        fclose(out);
    }

    free(buffer);
    fclose(in);
    return (int32_t)part;
}

int32_t fossil_io_filesys_file_join(
    const char **parts,
    size_t count,
    const char *dest)
{
    if (!parts || count == 0 || !dest)
        return -1;

    FILE *out = fopen(dest, "wb");
    if (!out)
        return -1;

    unsigned char buffer[8192];

    for (size_t i = 0; i < count; ++i)
    {
        FILE *in = fopen(parts[i], "rb");
        if (!in)
        {
            fclose(out);
            return -1;
        }

        size_t n;
        while ((n = fread(buffer, 1, sizeof(buffer), in)) > 0)
        {
            fwrite(buffer, 1, n, out);
        }

        fclose(in);
    }

    fclose(out);
    return 0;
}

int32_t fossil_io_filesys_file_compress(
    const char *src,
    const char *dest,
    const char *algorithm)
{
    if (!src || !dest)
        return -1;

    FILE *in = fopen(src, "rb");
    FILE *out = fopen(dest, "wb");

    if (!in || !out)
    {
        if (in)
            fclose(in);
        if (out)
            fclose(out);
        return -1;
    }

    /* Only RLE for now */
    if (algorithm && strcmp(algorithm, "rle") != 0)
    {
        fclose(in);
        fclose(out);
        return -1;
    }

    int prev = fgetc(in);
    if (prev == EOF)
    {
        fclose(in);
        fclose(out);
        return 0;
    }

    uint8_t count = 1;
    int c;

    while ((c = fgetc(in)) != EOF)
    {
        if (c == prev && count < 255)
        {
            count++;
        }
        else
        {
            fputc(count, out);
            fputc(prev, out);

            prev = c;
            count = 1;
        }
    }

    /* flush last */
    fputc(count, out);
    fputc(prev, out);

    fclose(in);
    fclose(out);
    return 0;
}

int32_t fossil_io_filesys_file_decompress(
    const char *src,
    const char *dest)
{
    if (!src || !dest)
        return -1;

    FILE *in = fopen(src, "rb");
    FILE *out = fopen(dest, "wb");

    if (!in || !out)
    {
        if (in)
            fclose(in);
        if (out)
            fclose(out);
        return -1;
    }

    int count, value;

    while ((count = fgetc(in)) != EOF)
    {
        value = fgetc(in);
        if (value == EOF)
            break;

        for (int i = 0; i < count; ++i)
            fputc(value, out);
    }

    fclose(in);
    fclose(out);
    return 0;
}

/* Directory Operations */

int32_t fossil_io_filesys_dir_create(const char *path, bool recursive)
{
    if (!path || !*path)
        return -1;

    if (!recursive)
    {
#if defined(_WIN32)
        return (_mkdir(path) == 0) ? 0 : -1;
#else
        return (mkdir(path, 0755) == 0) ? 0 : -1;
#endif
    }

    char tmp[PATH_MAX];
    strncpy(tmp, path, sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';

    size_t len = strlen(tmp);
    if (len == 0)
        return -1;

    /* strip trailing slash */
    if (tmp[len - 1] == '/' || tmp[len - 1] == '\\')
        tmp[len - 1] = '\0';

    for (char *p = tmp + 1; *p; ++p)
    {
        if (*p == '/' || *p == '\\')
        {
            *p = '\0';
#if defined(_WIN32)
            _mkdir(tmp);
#else
            mkdir(tmp, 0755);
#endif
            *p = PATH_SEP;
        }
    }

#if defined(_WIN32)
    return (_mkdir(tmp) == 0) ? 0 : -1;
#else
    return (mkdir(tmp, 0755) == 0) ? 0 : -1;
#endif
}

int32_t fossil_io_filesys_dir_list(
    const char *path,
    fossil_io_filesys_obj_t *entries,
    size_t max_entries,
    size_t *out_count)
{
    if (!path || !entries || !out_count)
        return -1;

    *out_count = 0;

#if defined(_WIN32)

    WIN32_FIND_DATAA fd;
    char search[FOSSIL_FILESYS_MAX_PATH];
    snprintf(search, sizeof(search), "%s\\*", path);

    HANDLE h = FindFirstFileA(search, &fd);
    if (h == INVALID_HANDLE_VALUE)
        return -1;

    do
    {
        if (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, ".."))
            continue;

        if (*out_count >= max_entries)
            break;

        char full[FOSSIL_FILESYS_MAX_PATH];
        snprintf(full, sizeof(full), "%s\\%s", path, fd.cFileName);

        fossil_io_filesys_stat(full, &entries[*out_count]);
        (*out_count)++;

    } while (FindNextFileA(h, &fd));

    FindClose(h);

#else

        DIR *dir = opendir(path);
        if (!dir)
            return -1;

        struct dirent *entry;

        while ((entry = readdir(dir)))
        {
            if (!strcmp(entry->d_name, ".") ||
                !strcmp(entry->d_name, ".."))
                continue;

            if (*out_count >= max_entries)
                break;

            char full[512];
            snprintf(full, sizeof(full), "%s/%s", path, entry->d_name);

            fossil_io_filesys_stat(full, &entries[*out_count]);
            (*out_count)++;
        }

        closedir(dir);

#endif

    return 0;
}

static int dir_walk_internal(
    const char *path,
    int (*callback)(const fossil_io_filesys_obj_t *, void *),
    void *user_data)
{
    fossil_io_filesys_obj_t obj;
    memset(&obj, 0, sizeof(obj));

    if (fossil_io_filesys_stat(path, &obj) != 0)
        return -1;

    /* invoke callback */
    int rc = callback(&obj, user_data);
    if (rc != 0)
        return rc;

#if defined(_WIN32)

    if (obj.type != FOSSIL_FILESYS_TYPE_DIR)
        return 0;

    WIN32_FIND_DATAA fd;
    char search[FOSSIL_FILESYS_MAX_PATH];
    snprintf(search, sizeof(search), "%s\\*", path);

    HANDLE h = FindFirstFileA(search, &fd);
    if (h == INVALID_HANDLE_VALUE)
        return -1;

    do
    {
        if (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, ".."))
            continue;

        char full[FOSSIL_FILESYS_MAX_PATH];
        snprintf(full, sizeof(full), "%s\\%s", path, fd.cFileName);

        rc = dir_walk_internal(full, callback, user_data);
        if (rc != 0)
        {
            FindClose(h);
            return rc;
        }

    } while (FindNextFileA(h, &fd));

    FindClose(h);

#else

        if (!S_ISDIR(obj.mode))
            return 0;

        DIR *dir = opendir(path);
        if (!dir)
            return -1;

        struct dirent *entry;

        while ((entry = readdir(dir)))
        {
            if (!strcmp(entry->d_name, ".") ||
                !strcmp(entry->d_name, ".."))
                continue;

            char full[512];
            snprintf(full, sizeof(full), "%s/%s", path, entry->d_name);

            rc = dir_walk_internal(full, callback, user_data);
            if (rc != 0)
            {
                closedir(dir);
                return rc;
            }
        }

        closedir(dir);

#endif

    return 0;
}

int32_t fossil_io_filesys_dir_walk(
    const char *path,
    int (*callback)(const fossil_io_filesys_obj_t *, void *),
    void *user_data)
{
    if (!path || !callback)
        return -1;

    return dir_walk_internal(path, callback, user_data);
}

int32_t fossil_io_filesys_dir_merge(
    const char *src,
    const char *dest,
    bool overwrite,
    bool recursive)
{
#if defined(_WIN32)

    WIN32_FIND_DATAA fd;
    char search[FOSSIL_FILESYS_MAX_PATH];
    snprintf(search, sizeof(search), "%s\\*", src);

    HANDLE h = FindFirstFileA(search, &fd);
    if (h == INVALID_HANDLE_VALUE)
        return -1;

    char src_path[FOSSIL_FILESYS_MAX_PATH], dest_path[FOSSIL_FILESYS_MAX_PATH];

    do
    {
        if (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, ".."))
            continue;

        snprintf(src_path, sizeof(src_path), "%s\\%s", src, fd.cFileName);
        snprintf(dest_path, sizeof(dest_path), "%s\\%s", dest, fd.cFileName);

        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (recursive)
            {
                CreateDirectoryA(dest_path, NULL);
                fossil_io_filesys_dir_merge(src_path, dest_path, overwrite, true);
            }
        }
        else
        {
            if (!overwrite && GetFileAttributesA(dest_path) != INVALID_FILE_ATTRIBUTES)
                continue;

            fossil_io_filesys_copy(src_path, dest_path, true);
        }
    } while (FindNextFileA(h, &fd));

    FindClose(h);
    return 0;

#else

    DIR *dir = opendir(src);
    if (!dir)
        return -1;

    struct dirent *entry;
    char src_path[512], dest_path[512];

    while ((entry = readdir(dir)) != NULL)
    {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest, entry->d_name);

        struct stat st;
        stat(src_path, &st);

        if (S_ISDIR(st.st_mode) && recursive)
        {
            mkdir(dest_path, 0755);
            fossil_io_filesys_dir_merge(src_path, dest_path, overwrite, true);
        }
        else
        {
            if (!overwrite && access(dest_path, F_OK) == 0)
                continue;

            fossil_io_filesys_copy(src_path, dest_path, true);
        }
    }

    closedir(dir);
    return 0;

#endif
}

int32_t fossil_io_filesys_dir_mirror(
    const char *src,
    const char *dest,
    bool delete_extras)
{
    if (!src || !dest)
        return -1;

    return mirror_recursive(src, dest, delete_extras);
}

/* Link Operations */

int32_t fossil_io_filesys_link_create(
    const char *target,
    const char *link_path,
    bool symbolic)
{
    if (!target || !link_path)
        return -1;

#if defined(_WIN32)

    if (symbolic)
    {
        DWORD flags = 0;

        /* detect directory target */
        DWORD attr = GetFileAttributesA(target);
        if (attr != INVALID_FILE_ATTRIBUTES &&
            (attr & FILE_ATTRIBUTE_DIRECTORY))
        {
            flags |= SYMBOLIC_LINK_FLAG_DIRECTORY;
        }

        /* allow unprivileged (Windows 10+) */
#ifdef SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE
        flags |= SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE;
#endif

        return CreateSymbolicLinkA(link_path, target, flags) ? 0 : -1;
    }
    else
    {
        return CreateHardLinkA(link_path, target, NULL) ? 0 : -1;
    }

#else

        if (symbolic)
            return symlink(target, link_path);
        else
            return link(target, link_path);

#endif
}

int32_t fossil_io_filesys_link_read(
    const char *link_path,
    char *target_out,
    size_t max_len)
{
    if (!link_path || !target_out || max_len == 0)
        return -1;

#if defined(_WIN32)

    HANDLE h = CreateFileA(
        link_path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL);

    if (h == INVALID_HANDLE_VALUE)
        return -1;

    DWORD len = GetFinalPathNameByHandleA(
        h,
        target_out,
        (DWORD)max_len,
        FILE_NAME_NORMALIZED);

    CloseHandle(h);

    if (len == 0 || len >= max_len)
        return -1;

    return 0;

#else

        ssize_t len = readlink(link_path, target_out, max_len - 1);
        if (len < 0)
            return -1;

        target_out[len] = '\0';
        return 0;

#endif
}

int32_t fossil_io_filesys_link_resolve(
    const char *link_path,
    char *target_out,
    size_t max_len)
{
    if (!link_path || !target_out || max_len == 0)
        return -1;

#if defined(_WIN32)

    HANDLE h = CreateFileA(
        link_path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL);

    if (h == INVALID_HANDLE_VALUE)
        return -1;

    DWORD len = GetFinalPathNameByHandleA(
        h,
        target_out,
        (DWORD)max_len,
        FILE_NAME_NORMALIZED);

    CloseHandle(h);

    if (len == 0 || len >= max_len)
        return -1;

    return 0;

#else

    char buf[PATH_MAX];

    ssize_t len = readlink(link_path, buf, sizeof(buf) - 1);

    if (len < 0)
    {
        // Not a symlink → fallback: just copy path
        strncpy(target_out, link_path, max_len - 1);
        target_out[max_len - 1] = '\0';
        return 0;
    }

    buf[len] = '\0';

    // If relative symlink → resolve against directory
    if (buf[0] != '/')
    {
        char dir[PATH_MAX];
        strncpy(dir, link_path, sizeof(dir) - 1);
        dir[sizeof(dir) - 1] = '\0';

        char *slash = strrchr(dir, '/');
        if (slash)
            *slash = '\0';
        else
            strcpy(dir, ".");

        snprintf(target_out, max_len, "%s/%s", dir, buf);
    }
    else
    {
        // Absolute target
        strncpy(target_out, buf, max_len - 1);
        target_out[max_len - 1] = '\0';
    }

    return 0;

#endif
}

int32_t fossil_io_filesys_link_is_symbolic(
    const char *link_path,
    bool *is_symbolic)
{
    if (!link_path || !is_symbolic)
        return -1;

#if defined(_WIN32)

    DWORD attr = GetFileAttributesA(link_path);
    if (attr == INVALID_FILE_ATTRIBUTES)
        return -1;

    *is_symbolic = (attr & FILE_ATTRIBUTE_REPARSE_POINT) != 0;
    return 0;

#else

        struct stat st;
        if (lstat(link_path, &st) != 0)
            return -1;

        *is_symbolic = S_ISLNK(st.st_mode);
        return 0;

#endif
}

/* Transaction Operations */

static fossil_io_filesys_lock_t g_tx_lock;

int32_t fossil_io_filesys_tx_init(void)
{
    return fossil_mutex_init(&g_tx_lock);
}

int32_t fossil_io_filesys_tx_begin(void)
{
    if (!g_tx_lock.handle)
        return fossil_io_filesys_tx_init();
    
    return fossil_mutex_lock(&g_tx_lock);
}

int32_t fossil_io_filesys_tx_commit(void)
{
    if (!g_tx_lock.handle || !g_tx_lock.locked)
        return -1;
    
    return fossil_mutex_unlock(&g_tx_lock);
}

int32_t fossil_io_filesys_tx_rollback(void)
{
    if (!g_tx_lock.handle || !g_tx_lock.locked)
        return -1;
    
    return fossil_mutex_unlock(&g_tx_lock);
}

int32_t fossil_io_filesys_tx_is_active(void)
{
    return (g_tx_lock.locked) ? 1 : 0;
}

/* Path / Utility Operations */

int32_t fossil_io_filesys_getcwd(char *buf, size_t size)
{
    if (!buf || size == 0)
        return -1;

#if defined(_WIN32)
    if (!_getcwd(buf, (int)size))
        return -1;
#else
        if (!getcwd(buf, size))
            return -1;
#endif
    return 0;
}

int32_t fossil_io_filesys_chdir(const char *path)
{
    if (!path)
        return -1;

#if defined(_WIN32)
    return _chdir(path);
#else
        return chdir(path);
#endif
}

int32_t fossil_io_filesys_abspath(const char *path, char *abs_path, size_t max_len)
{
    if (!path || !abs_path || max_len == 0)
        return -1;

#if defined(_WIN32)

    if (!_fullpath(abs_path, path, max_len))
        return -1;

#else

    // Already absolute
    if (path[0] == '/')
    {
        strncpy(abs_path, path, max_len - 1);
        abs_path[max_len - 1] = '\0';
        return 0;
    }

    // Relative → prepend cwd
    char cwd[PATH_MAX];
    if (!getcwd(cwd, sizeof(cwd)))
        return -1;

    int written = snprintf(abs_path, max_len, "%s/%s", cwd, path);
    if (written < 0 || (size_t)written >= max_len)
        return -1;

#endif

    return 0;
}

int32_t fossil_io_filesys_dirname(const char *path, char *dir_out, size_t max_len)
{
    if (!path || !dir_out || max_len == 0)
        return -1;

    char tmp[PATH_MAX];
    strncpy(tmp, path, sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';

#if defined(_WIN32)
    char *last_sep = strrchr(tmp, '\\');
#else
        char *last_sep = strrchr(tmp, '/');
#endif

    if (!last_sep)
    {
        /* no separator, return "." */
        strncpy(dir_out, ".", max_len);
        dir_out[max_len - 1] = '\0';
    }
    else
    {
        size_t len = (size_t)(last_sep - tmp);
        if (len >= max_len)
            len = max_len - 1;
        strncpy(dir_out, tmp, len);
        dir_out[len] = '\0';
    }

    return 0;
}

int32_t fossil_io_filesys_basename(const char *path, char *name_out, size_t max_len)
{
    if (!path || !name_out || max_len == 0)
        return -1;

    const char *base = path;

#if defined(_WIN32)
    const char *last_sep = strrchr(path, '\\');
#else
        const char *last_sep = strrchr(path, '/');
#endif
    if (last_sep)
        base = last_sep + 1;

    strncpy(name_out, base, max_len);
    name_out[max_len - 1] = '\0';
    return 0;
}

int32_t fossil_io_filesys_extension(const char *path, char *ext_out, size_t max_len)
{
    if (!path || !ext_out || max_len == 0)
        return -1;

    const char *base = path;
#if defined(_WIN32)
    const char *last_sep = strrchr(path, '\\');
#else
        const char *last_sep = strrchr(path, '/');
#endif
    if (last_sep)
        base = last_sep + 1;

    const char *dot = strrchr(base, '.');
    if (!dot || dot == base)
    {
        ext_out[0] = '\0';
    }
    else
    {
        strncpy(ext_out, dot, max_len);
        ext_out[max_len - 1] = '\0';
    }

    return 0;
}

const char *fossil_io_filesys_type_string(fossil_io_filesys_type_t type)
{
    switch (type)
    {
    case FOSSIL_FILESYS_TYPE_FILE:
        return "file";
    case FOSSIL_FILESYS_TYPE_DIR:
        return "directory";
    case FOSSIL_FILESYS_TYPE_LINK:
        return "link";
    default:
        return "unknown";
    }
}
