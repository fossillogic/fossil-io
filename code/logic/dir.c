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
#include "fossil/io/dir.h"
#include "fossil/io/cstring.h"
#include "fossil/io/output.h"

#include <time.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h> // for realpath, getenv, mkdtemp
#include <unistd.h> // for mkdtemp

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #include <sys/stat.h>
  #include <direct.h>
  #ifndef PATH_MAX
    #define PATH_MAX MAX_PATH
  #endif
  #define PATHSEP '\\'
  #define PATHSEP_STR "\\"
  #define mkdir_native(p,mode) _mkdir(p)
  #define rmdir_native(p) _rmdir(p)
  #define stat_native _stat
  typedef struct _stat stat_t;
#else
  #include <sys/stat.h>
  #include <unistd.h>
  #include <dirent.h>
  #include <limits.h>
  #include <fcntl.h>
  #ifndef PATH_MAX
    #define PATH_MAX 4096
  #endif
  #define PATHSEP '/'
  #define PATHSEP_STR "/"
  #define mkdir_native(p,mode) mkdir((p),(mode))
  #define rmdir_native(p) rmdir(p)
  #define stat_native stat
  typedef struct stat stat_t;
#endif

// ------------------------------------------------------------
// Helpers
// ------------------------------------------------------------
static void safe_strcpy(char *dst, const char *src, size_t dstlen){
    if (!dst || dstlen == 0) return;
    if (!src) { dst[0] = '\0'; return; }
    size_t srclen = strlen(src);
    if (srclen >= dstlen)
        srclen = dstlen - 1;
    memcpy(dst, src, srclen);
    dst[srclen] = '\0';
}

static int32_t join_paths(const char *a, const char *b, char *out, size_t outsz){
    if (!out || outsz == 0) return -1;
    if (!a || !a[0]) { safe_strcpy(out, b ? b : "", outsz); return 0; }
    if (!b || !b[0]) { safe_strcpy(out, a, outsz); return 0; }

    size_t la = strlen(a);
    size_t lb = strlen(b);
    int need_sep = 0;
#ifdef _WIN32
    need_sep = (a[la-1] != '\\' && a[la-1] != '/');
#else
    need_sep = (a[la-1] != '/' && a[la-1] != '\\');
#endif

    size_t total = la + (need_sep ? 1 : 0) + lb;
    if (total >= outsz) {
        // Truncate b if needed
        size_t copy_b = outsz - la - (need_sep ? 1 : 0) - 1;
        if (copy_b > lb) copy_b = lb;
        memcpy(out, a, la);
        if (need_sep && la < outsz - 1) {
            out[la] = PATHSEP;
            la++;
        }
        if (copy_b > 0 && la + copy_b < outsz) {
            memcpy(out + la, b, copy_b);
            la += copy_b;
        }
        out[la] = '\0';
    } else {
        memcpy(out, a, la);
        size_t pos = la;
        if (need_sep) {
            out[pos++] = PATHSEP;
        }
        memcpy(out + pos, b, lb);
        out[pos + lb] = '\0';
    }
    return 0;
}

static int32_t file_copy_internal(const char *src, const char *dst){
    if (!src || !dst) return -1;
    FILE *fin = fopen(src, "rb");
    if (!fin) return -1;
    FILE *fout = fopen(dst, "wb");
    if (!fout) { fclose(fin); return -1; }

    char buf[16384];
    size_t r;
    int err = 0;
    while ((r = fread(buf, 1, sizeof(buf), fin)) > 0){
        if (fwrite(buf, 1, r, fout) != r) { err = -1; break; }
    }

    fclose(fin);
    fclose(fout);
    return err;
}

static int32_t path_is_dot_or_dotdot(const char *p){
    if (!p) return 0;
    return (strcmp(p, ".") == 0 || strcmp(p, "..") == 0);
}

// ------------------------------------------------------------
// Low-Level Helpers
// ------------------------------------------------------------
int32_t fossil_io_dir_is_directory(const char *path){
    if (!path) return 0;
    stat_t st;
    if (stat_native(path, &st) != 0) return 0;
#ifdef _WIN32
    return (st.st_mode & _S_IFDIR) ? 1 : 0;
#else
    return S_ISDIR(st.st_mode) ? 1 : 0;
#endif
}

int32_t fossil_io_dir_is_file(const char *path){
    if (!path) return 0;
    stat_t st;
    if (stat_native(path, &st) != 0) return 0;
#ifdef _WIN32
    return (st.st_mode & _S_IFREG) ? 1 : 0;
#else
    return S_ISREG(st.st_mode) ? 1 : 0;
#endif
}

int32_t fossil_io_dir_is_symlink(const char *path){
    if (!path) return 0;
#ifndef _WIN32
    struct stat st;
    if (lstat(path, &st) != 0) return 0;
    return S_ISLNK(st.st_mode) ? 1 : 0;
#else
    // Windows: best-effort using attributes
    DWORD a = GetFileAttributesA(path);
    if (a == INVALID_FILE_ATTRIBUTES) return 0;
    return (a & FILE_ATTRIBUTE_REPARSE_POINT) ? 1 : 0;
#endif
}

// ------------------------------------------------------------
// Basic Operations
// ------------------------------------------------------------
int32_t fossil_io_dir_exists(const char *path){
    return fossil_io_dir_is_directory(path);
}

int32_t fossil_io_dir_create(const char *path){
    if (!path) return -1;
    // attempt to create parent directories as needed (mkdir -p style)
    char tmp[1024];
    safe_strcpy(tmp, path, sizeof(tmp));
    size_t len = strlen(tmp);
    if (len == 0) return -1;

    // Normalize separators to platform-specific
    for (size_t i = 0; i < len; ++i){
#ifdef _WIN32
        if (tmp[i] == '/') tmp[i] = '\\';
#else
        if (tmp[i] == '\\') tmp[i] = '/';
#endif
    }

    // create progressively
    for (size_t i = 1; i <= len; ++i){
        if (tmp[i] == '\0' || tmp[i] == PATHSEP){
            char save = tmp[i];
            tmp[i] = '\0';
            if (!fossil_io_dir_exists(tmp)){
                int rc;
#ifdef _WIN32
                rc = mkdir_native(tmp, 0);
#else
                rc = mkdir_native(tmp, 0755);
#endif
                if (rc != 0 && errno != EEXIST) {
                    tmp[i] = save;
                    return -1;
                }
            }
            tmp[i] = save;
        }
    }

    // final create if not exists
    if (!fossil_io_dir_exists(path)){
#ifdef _WIN32
        if (mkdir_native(path, 0) != 0 && errno != EEXIST) return -1;
#else
        if (mkdir_native(path, 0755) != 0 && errno != EEXIST) return -1;
#endif
    }
    return 0;
}

int32_t fossil_io_dir_remove(const char *path){
    if (!path) return -1;
    if (!fossil_io_dir_exists(path)) return 0;
#ifdef _WIN32
    return rmdir_native(path) == 0 ? 0 : -1;
#else
    return rmdir_native(path) == 0 ? 0 : -1;
#endif
}

static int32_t remove_recursive_internal(const char *path){
    if (!path) return -1;
    if (!fossil_io_dir_exists(path)) return 0;

#ifndef _WIN32
    DIR *d = opendir(path);
    if (!d) return -1;
    struct dirent *ent;
    char child[2048];
    while ((ent = readdir(d)) != NULL){
        if (path_is_dot_or_dotdot(ent->d_name)) continue;
        join_paths(path, ent->d_name, child, sizeof(child));
        if (fossil_io_dir_is_directory(child)){
            if (remove_recursive_internal(child) != 0) { closedir(d); return -1; }
        } else {
            if (remove(child) != 0) { closedir(d); return -1; }
        }
    }
    closedir(d);
    if (rmdir(path) != 0) return -1;
    return 0;
#else
    WIN32_FIND_DATAA fd;
    char search[MAX_PATH];
    snprintf(search, sizeof(search), "%s\\*", path);
    HANDLE h = FindFirstFileA(search, &fd);
    if (h == INVALID_HANDLE_VALUE) return -1;
    do {
        const char *name = fd.cFileName;
        if (path_is_dot_or_dotdot(name)) continue;
        char child[PATH_MAX * 2];
        snprintf(child, sizeof(child), "%s\\%s", path, name);
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            if (remove_recursive_internal(child) != 0) { FindClose(h); return -1; }
        } else {
            if (!DeleteFileA(child)) { FindClose(h); return -1; }
        }
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    if (!RemoveDirectoryA(path)) return -1;
    return 0;
#endif
}

int32_t fossil_io_dir_remove_recursive(const char *path){
    return remove_recursive_internal(path);
}

int32_t fossil_io_dir_clear(const char *path){
    // Remove contents but keep dir
    if (!path) return -1;
    if (!fossil_io_dir_exists(path)) return 0;

#ifndef _WIN32
    DIR *d = opendir(path);
    if (!d) return -1;
    struct dirent *ent;
    char child[2048];
    while ((ent = readdir(d)) != NULL){
        if (path_is_dot_or_dotdot(ent->d_name)) continue;
        join_paths(path, ent->d_name, child, sizeof(child));
        if (fossil_io_dir_is_directory(child)){
            if (remove_recursive_internal(child) != 0) { closedir(d); return -1; }
        } else {
            if (remove(child) != 0) { closedir(d); return -1; }
        }
    }
    closedir(d);
    return 0;
#else
    WIN32_FIND_DATAA fd;
    char search[PATH_MAX * 2];
    snprintf(search, sizeof(search), "%s\\*", path);
    HANDLE h = FindFirstFileA(search, &fd);
    if (h == INVALID_HANDLE_VALUE) return -1;
    do {
        const char *name = fd.cFileName;
        if (path_is_dot_or_dotdot(name)) continue;
        char child[PATH_MAX * 2];
        snprintf(child, sizeof(child), "%s\\%s", path, name);
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            if (remove_recursive_internal(child) != 0) { FindClose(h); return -1; }
        } else {
            if (!DeleteFileA(child)) { FindClose(h); return -1; }
        }
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    return 0;
#endif
}

// Copy directory shallow
int32_t fossil_io_dir_copy(const char *src, const char *dst){
    if (!src || !dst) return -1;
    if (!fossil_io_dir_exists(src)) return -1;
    fossil_io_dir_create(dst);

#ifndef _WIN32
    DIR *d = opendir(src);
    if (!d) return -1;
    struct dirent *ent;
    char schild[2048], dchild[2048];
    int rc = 0;
    while ((ent = readdir(d)) != NULL){
        if (path_is_dot_or_dotdot(ent->d_name)) continue;
        join_paths(src, ent->d_name, schild, sizeof(schild));
        join_paths(dst, ent->d_name, dchild, sizeof(dchild));
        if (fossil_io_dir_is_directory(schild)){
            // create subdir only (not recursive)
            fossil_io_dir_create(dchild);
        } else {
            if (file_copy_internal(schild, dchild) != 0) { rc = -1; break; }
        }
    }
    closedir(d);
    return rc;
#else
    WIN32_FIND_DATAA fd;
    char search[PATH_MAX * 2];
    snprintf(search, sizeof(search), "%s\\*", src);
    HANDLE h = FindFirstFileA(search, &fd);
    if (h == INVALID_HANDLE_VALUE) return -1;
    int rc = 0;
    do {
        const char *name = fd.cFileName;
        if (path_is_dot_or_dotdot(name)) continue;
        char schild[PATH_MAX * 2], dchild[PATH_MAX * 2];
        snprintf(schild, sizeof(schild), "%s\\%s", src, name);
        snprintf(dchild, sizeof(dchild), "%s\\%s", dst, name);
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            fossil_io_dir_create(dchild);
        } else {
            if (file_copy_internal(schild, dchild) != 0) { rc = -1; break; }
        }
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    return rc;
#endif
}

static int32_t copy_recursive_internal(const char *src, const char *dst){
    if (!src || !dst) return -1;
    if (!fossil_io_dir_exists(src)) return -1;
    fossil_io_dir_create(dst);

#ifndef _WIN32
    DIR *d = opendir(src);
    if (!d) return -1;
    struct dirent *ent;
    char schild[2048], dchild[2048];
    int rc = 0;
    while ((ent = readdir(d)) != NULL){
        if (path_is_dot_or_dotdot(ent->d_name)) continue;
        join_paths(src, ent->d_name, schild, sizeof(schild));
        join_paths(dst, ent->d_name, dchild, sizeof(dchild));
        if (fossil_io_dir_is_directory(schild)){
            if (copy_recursive_internal(schild, dchild) != 0) { rc = -1; break; }
        } else {
            if (file_copy_internal(schild, dchild) != 0) { rc = -1; break; }
        }
    }
    closedir(d);
    return rc;
#else
    WIN32_FIND_DATAA fd;
    char search[PATH_MAX * 2];
    snprintf(search, sizeof(search), "%s\\*", src);
    HANDLE h = FindFirstFileA(search, &fd);
    if (h == INVALID_HANDLE_VALUE) return -1;
    int rc = 0;
    do {
        const char *name = fd.cFileName;
        if (path_is_dot_or_dotdot(name)) continue;
        char schild[PATH_MAX * 2], dchild[PATH_MAX * 2];
        snprintf(schild, sizeof(schild), "%s\\%s", src, name);
        snprintf(dchild, sizeof(dchild), "%s\\%s", dst, name);
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            if (copy_recursive_internal(schild, dchild) != 0) { rc = -1; break; }
        } else {
            if (file_copy_internal(schild, dchild) != 0) { rc = -1; break; }
        }
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    return rc;
#endif
}

int32_t fossil_io_dir_copy_recursive(const char *src, const char *dst){
    return copy_recursive_internal(src, dst);
}

int32_t fossil_io_dir_move(const char *src, const char *dst){
    if (!src || !dst) return -1;
#ifdef _WIN32
    if (MoveFileA(src, dst)) return 0;
    // if MoveFileA failed, attempt copy+delete
    if (file_copy_internal(src, dst) == 0){
        if (DeleteFileA(src)) return 0;
        return -1;
    }
    return -1;
#else
    if (rename(src, dst) == 0) return 0;
    // fallback: copy then remove
    if (file_copy_internal(src, dst) == 0){
        if (remove(src) == 0) return 0;
        return -1;
    }
    return -1;
#endif
}

int32_t fossil_io_dir_rename(const char *oldpath, const char *newpath){
    if (!oldpath || !newpath) return -1;
    if (rename(oldpath, newpath) == 0) return 0;
    return -1;
}

// ------------------------------------------------------------
// Directory Iterator + Listing
// ------------------------------------------------------------
int32_t fossil_io_dir_iter_open(fossil_io_dir_iter_t *it, const char *path){
    if (!it || !path) return -1;
    memset(it, 0, sizeof(*it));
    safe_strcpy(it->basepath, path, sizeof(it->basepath));
    it->active = 0;
    it->index = 0;
    it->total = 0;
    it->recursive = 0;
    it->follow_symlinks = 0;
    it->include_hidden = 0;
    it->include_system = 0;
    it->sort_mode = 0;
    it->sort_descending = 0;
    it->filter_glob = NULL;
    it->filter_regex = NULL;
    it->platform_data = NULL;

#ifndef _WIN32
    DIR *d = opendir(path);
    if (!d) return -1;
    it->handle = d;
    it->active = 1;
    it->current.platform_data = NULL;
    return 0;
#else
    // Windows: store search handle in handle and preload first
    WIN32_FIND_DATAA *fd = (WIN32_FIND_DATAA*)malloc(sizeof(WIN32_FIND_DATAA));
    if (!fd) return -1;
    char search[MAX_PATH];
    snprintf(search, sizeof(search), "%s\\*", path);
    HANDLE h = FindFirstFileA(search, fd);
    if (h == INVALID_HANDLE_VALUE) { free(fd); return -1; }
    // store a small struct that includes handle and last data
    struct {
        HANDLE handle;
        WIN32_FIND_DATAA data;
        int has_current;
    } *wh = malloc(sizeof(*wh));
    if (!wh) { FindClose(h); free(fd); return -1; }
    wh->handle = h;
    wh->data = *fd;
    wh->has_current = 1;
    free(fd);
    it->handle = wh;
    it->active = 1;
    it->current.platform_data = NULL;
    return 0;
#endif
}

int32_t fossil_io_dir_iter_next(fossil_io_dir_iter_t *it){
    if (!it || !it->handle || !it->active) return 0;

#ifndef _WIN32
    DIR *d = (DIR*)it->handle;
    struct dirent *ent;
    while ((ent = readdir(d)) != NULL){
        if (path_is_dot_or_dotdot(ent->d_name)) continue;
        fossil_io_dir_entry_t *e = &it->current;
        memset(e, 0, sizeof(*e));
        safe_strcpy(e->name, ent->d_name, sizeof(e->name));
        join_paths(it->basepath, ent->d_name, e->path, sizeof(e->path));

        stat_t st;
        if (lstat(e->path, &st) == 0){
            // Type
            if (S_ISREG(st.st_mode))      e->type = 0;
            else if (S_ISDIR(st.st_mode)) e->type = 1;
            else if (S_ISLNK(st.st_mode)) e->type = 2;
            else if (S_ISFIFO(st.st_mode))e->type = 3;
            else if (S_ISSOCK(st.st_mode))e->type = 4;
            else if (S_ISBLK(st.st_mode)) e->type = 5;
            else if (S_ISCHR(st.st_mode)) e->type = 6;
            else                          e->type = 7;

            e->size = (uint64_t)st.st_size;
#ifdef __linux__
            e->allocated_size = (uint64_t)st.st_blocks * 512;
#else
            e->allocated_size = (uint64_t)st.st_blocks * (uint64_t)DEV_BSIZE;
#endif
            e->modified = (uint64_t)st.st_mtime;
            e->accessed = (uint64_t)st.st_atime;
            e->changed  = (uint64_t)st.st_ctime;
#ifdef __APPLE__
            e->created  = (uint64_t)st.st_birthtime;
#else
            e->created  = 0;
#endif
            e->permissions = (int32_t)(st.st_mode & 07777);
            e->owner_uid = (int32_t)st.st_uid;
            e->owner_gid = (int32_t)st.st_gid;
            // Hidden: dotfile
            e->is_hidden = (e->name[0] == '.') ? 1 : 0;
            // Readonly: no write bit for owner
            e->is_readonly = ((st.st_mode & S_IWUSR) == 0) ? 1 : 0;
            // System: not applicable on POSIX
            e->is_system = 0;
            // Executable: owner execute bit
            e->is_executable = ((st.st_mode & S_IXUSR) != 0) ? 1 : 0;
            // Attributes bitfield
            e->attributes = 0;
            if (e->is_hidden)     e->attributes |= (1 << 0);
            if (e->is_readonly)   e->attributes |= (1 << 1);
            if (e->is_executable) e->attributes |= (1 << 2);
            if (e->type == 2)     e->attributes |= (1 << 3);
            // platform_data
            e->platform_data = NULL;
        } else {
            // fallback: unknown type
            e->type = 7;
            e->size = 0;
            e->allocated_size = 0;
            e->modified = 0;
            e->accessed = 0;
            e->changed = 0;
            e->created = 0;
            e->permissions = 0;
            e->owner_uid = -1;
            e->owner_gid = -1;
            e->is_hidden = (e->name[0] == '.') ? 1 : 0;
            e->is_readonly = 0;
            e->is_system = 0;
            e->is_executable = 0;
            e->attributes = 0;
            if (e->is_hidden) e->attributes |= (1 << 0);
            e->platform_data = NULL;
        }
        // Hashes not computed here
        e->hash_crc32 = 0;
        e->hash_murmur64 = 0;
        memset(e->hash_sha1, 0, sizeof(e->hash_sha1));
        return 1;
    }
    return 0;
#else
    struct {
        HANDLE handle;
        WIN32_FIND_DATAA data;
        int has_current;
    } *wh = (void*)it->handle;

    while (wh && wh->has_current){
        const char *name = wh->data.cFileName;
        wh->has_current = 0; // consume current
        if (!path_is_dot_or_dotdot(name)){
            fossil_io_dir_entry_t *e = &it->current;
            memset(e, 0, sizeof(*e));
            safe_strcpy(e->name, name, sizeof(e->name));
            fossil_io_dir_join(it->basepath, name, e->path, sizeof(e->path));
            // Type
            if (wh->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                e->type = 1;
            else if (wh->data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
                e->type = 2;
            else
                e->type = 0;
            // Size
            e->size = ((uint64_t)wh->data.nFileSizeHigh << 32) | wh->data.nFileSizeLow;
            e->allocated_size = e->size; // Windows: no easy way to get allocated size
            // Modified
            FILETIME ft = wh->data.ftLastWriteTime;
            ULARGE_INTEGER ull;
            ull.LowPart = ft.dwLowDateTime;
            ull.HighPart = ft.dwHighDateTime;
            uint64_t windows_time = ull.QuadPart;
            uint64_t seconds = (windows_time / 10000000ULL) - 11644473600ULL;
            e->modified = seconds;
            // Accessed
            ft = wh->data.ftLastAccessTime;
            ull.LowPart = ft.dwLowDateTime;
            ull.HighPart = ft.dwHighDateTime;
            windows_time = ull.QuadPart;
            seconds = (windows_time / 10000000ULL) - 11644473600ULL;
            e->accessed = seconds;
            // Changed (Windows: use last write)
            e->changed = e->modified;
            // Created
            ft = wh->data.ftCreationTime;
            ull.LowPart = ft.dwLowDateTime;
            ull.HighPart = ft.dwHighDateTime;
            windows_time = ull.QuadPart;
            seconds = (windows_time / 10000000ULL) - 11644473600ULL;
            e->created = seconds;
            // Permissions: map readonly
            e->permissions = (wh->data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) ? 0444 : 0666;
            e->owner_uid = -1;
            e->owner_gid = -1;
            // Hidden
            e->is_hidden = (wh->data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) ? 1 : 0;
            // Readonly
            e->is_readonly = (wh->data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) ? 1 : 0;
            // System
            e->is_system = (wh->data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) ? 1 : 0;
            // Executable: guess by extension
            const char *ext = strrchr(e->name, '.');
            if (ext && (
                _stricmp(ext, ".exe") == 0 ||
                _stricmp(ext, ".bat") == 0 ||
                _stricmp(ext, ".cmd") == 0 ||
                _stricmp(ext, ".com") == 0
            )) e->is_executable = 1;
            else e->is_executable = 0;
            // Attributes bitfield
            e->attributes = 0;
            if (e->is_hidden)     e->attributes |= (1 << 0);
            if (e->is_readonly)   e->attributes |= (1 << 1);
            if (e->is_executable) e->attributes |= (1 << 2);
            if (e->type == 2)     e->attributes |= (1 << 3);
            if (e->is_system)     e->attributes |= (1 << 4);
            if (wh->data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)    e->attributes |= (1 << 5);
            if (wh->data.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED) e->attributes |= (1 << 6);
            if (wh->data.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)  e->attributes |= (1 << 7);
            // platform_data
            e->platform_data = NULL;
            // Hashes not computed here
            e->hash_crc32 = 0;
            e->hash_murmur64 = 0;
            memset(e->hash_sha1, 0, sizeof(e->hash_sha1));
            // Prepare next
            if (FindNextFileA(wh->handle, &wh->data)) wh->has_current = 1;
            return 1;
        }
        // get next
        if (FindNextFileA(wh->handle, &wh->data)) wh->has_current = 1;
    }
    return 0;
#endif
}

void fossil_io_dir_iter_close(fossil_io_dir_iter_t *it){
    if (!it || !it->handle) return;
#ifndef _WIN32
    DIR *d = (DIR*)it->handle;
    closedir(d);
    it->handle = NULL;
    it->active = 0;
    it->platform_data = NULL;
    memset(&it->current, 0, sizeof(it->current));
#else
    struct {
        HANDLE handle;
        WIN32_FIND_DATAA data;
        int has_current;
    } *wh = (void*)it->handle;
    if (wh){
        FindClose(wh->handle);
        free(wh);
    }
    it->handle = NULL;
    it->active = 0;
    it->platform_data = NULL;
    memset(&it->current, 0, sizeof(it->current));
#endif
}

int32_t fossil_io_dir_list(const char *path,
                           fossil_io_dir_entry_t *entries,
                           size_t *count,
                           size_t max_entries)
{
    if (!path || !entries || !count) return -1;
    fossil_io_dir_iter_t it;
    if (fossil_io_dir_iter_open(&it, path) != 0) return -1;
    size_t idx = 0;
    while (idx < max_entries && fossil_io_dir_iter_next(&it)) {
        // Copy all fields of the updated fossil_io_dir_entry_t
        entries[idx] = it.current;
        // If platform_data is a pointer, set to NULL to avoid dangling pointers
        entries[idx].platform_data = NULL;
        idx++;
    }
    fossil_io_dir_iter_close(&it);
    *count = idx;
    return 0;
}

// ------------------------------------------------------------
// Path utilities
// ------------------------------------------------------------
int32_t fossil_io_dir_is_absolute(const char *path){
    if (!path || !path[0]) return 0;
#ifdef _WIN32
    // e.g., C:\ or \\server\share
    if ((strlen(path) >= 2 && path[1] == ':') || (strlen(path) >= 2 && path[0] == '\\' && path[1] == '\\'))
        return 1;
    return 0;
#else
    return (path[0] == '/') ? 1 : 0;
#endif
}

int32_t fossil_io_dir_join(const char *base, const char *name, char *out, size_t outsz){
    if (!out || outsz == 0) return -1;
    return join_paths(base, name, out, outsz);
}

int32_t fossil_io_dir_basename(const char *path, char *out, size_t outsz){
    if (!path || !out) return -1;
    const char *p = path + strlen(path);
    while (p > path && (*p == '\\' || *p == '/')) --p;
    const char *end = p;
    while (p > path && *(p-1) != '\\' && *(p-1) != '/') --p;
    size_t len = end - p + 1;
    if (len >= outsz) len = outsz - 1;
    strncpy(out, p, len);
    out[len] = '\0';
    return 0;
}

int32_t fossil_io_dir_dirname(const char *path, char *out, size_t outsz){
    if (!path || !out) return -1;
    const char *p = path + strlen(path);
    while (p > path && (*p == '\\' || *p == '/')) --p;
    while (p > path && *(p-1) != '\\' && *(p-1) != '/') --p;
    if (p == path){
        // no directory portion
        safe_strcpy(out, ".", outsz);
        return 0;
    }
    size_t len = p - path - 1;
    if (len >= outsz) len = outsz - 1;
    strncpy(out, path, len);
    out[len] = '\0';
    return 0;
}

int32_t fossil_io_dir_normalize(const char *path, char *out, size_t outsz){
    if (!path || !out) return -1;
    // Very simple normalization: collapse duplicate separators and remove trailing separators (except root)
    size_t j = 0;
    int last_sep = 0;
    for (size_t i = 0; path[i] != '\0' && j + 1 < outsz; ++i){
        char c = path[i];
        if (c == '/' || c == '\\'){
            if (last_sep) continue;
            out[j++] = PATHSEP;
            last_sep = 1;
        } else {
            out[j++] = c;
            last_sep = 0;
        }
    }
    // remove trailing separator if not root
    if (j > 1 && out[j-1] == PATHSEP) j--;
    out[j] = '\0';
    return 0;
}

int32_t fossil_io_dir_realpath(const char *path, char *out, size_t outsz){
    if (!path || !out) return -1;
#ifdef _WIN32
    // use GetFullPathNameA
    DWORD r = GetFullPathNameA(path, (DWORD)outsz, out, NULL);
    return (r > 0 && r < outsz) ? 0 : -1;
#else
    // Alternative to realpath: resolve absolute path, but do not resolve symlinks
    if (fossil_io_dir_is_absolute(path)) {
        safe_strcpy(out, path, outsz);
        return 0;
    }
    char cwd[PATH_MAX];
    if (!getcwd(cwd, sizeof(cwd))) return -1;
    char abs[PATH_MAX * 2];
    if (snprintf(abs, sizeof(abs), "%s/%s", cwd, path) >= (int)sizeof(abs)) return -1;
    // Normalize the result
    fossil_io_dir_normalize(abs, out, outsz);
    return 0;
#endif
}

// ------------------------------------------------------------
// Inspection and counting
// ------------------------------------------------------------
int32_t fossil_io_dir_is_empty(const char *path){
    if (!path) return -1;
    if (!fossil_io_dir_exists(path)) return -1;
#ifndef _WIN32
    DIR *d = opendir(path);
    if (!d) return -1;
    struct dirent *ent;
    int empty = 1;
    while ((ent = readdir(d)) != NULL){
        if (path_is_dot_or_dotdot(ent->d_name)) continue;
        empty = 0; break;
    }
    closedir(d);
    return empty;
#else
    WIN32_FIND_DATAA fd;
    char search[PATH_MAX * 2];
    snprintf(search, sizeof(search), "%s\\*", path);
    HANDLE h = FindFirstFileA(search, &fd);
    if (h == INVALID_HANDLE_VALUE) return -1;
    int empty = 1;
    do {
        if (!path_is_dot_or_dotdot(fd.cFileName)) { empty = 0; break; }
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    return empty;
#endif
}

int32_t fossil_io_dir_count(const char *path, size_t *count){
    if (!path || !count) return -1;
    if (!fossil_io_dir_exists(path)) return -1;
    size_t c = 0;
#ifndef _WIN32
    DIR *d = opendir(path);
    if (!d) return -1;
    struct dirent *ent;
    while ((ent = readdir(d)) != NULL){
        if (path_is_dot_or_dotdot(ent->d_name)) continue;
        ++c;
    }
    closedir(d);
#else
    WIN32_FIND_DATAA fd;
    char search[PATH_MAX * 2];
    snprintf(search, sizeof(search), "%s\\*", path);
    HANDLE h = FindFirstFileA(search, &fd);
    if (h == INVALID_HANDLE_VALUE) return -1;
    do {
        if (!path_is_dot_or_dotdot(fd.cFileName)) ++c;
    } while (FindNextFileA(h, &fd));
    FindClose(h);
#endif
    *count = c;
    return 0;
}

static int32_t size_recursive_internal(const char *path, uint64_t *acc){
    if (!path || !acc) return -1;
#ifndef _WIN32
    DIR *d = opendir(path);
    if (!d) return -1;
    struct dirent *ent;
    char child[2048];
    while ((ent = readdir(d)) != NULL){
        if (path_is_dot_or_dotdot(ent->d_name)) continue;
        join_paths(path, ent->d_name, child, sizeof(child));
        if (fossil_io_dir_is_directory(child)){
            if (size_recursive_internal(child, acc) != 0) { closedir(d); return -1; }
        } else {
            stat_t st;
            if (stat_native(child, &st) == 0) *acc += (uint64_t)st.st_size;
        }
    }
    closedir(d);
    return 0;
#else
    WIN32_FIND_DATAA fd;
    char search[PATH_MAX * 2];
    snprintf(search, sizeof(search), "%s\\*", path);
    HANDLE h = FindFirstFileA(search, &fd);
    if (h == INVALID_HANDLE_VALUE) return -1;
    do {
        const char *name = fd.cFileName;
        if (path_is_dot_or_dotdot(name)) continue;
        char child[PATH_MAX * 2];
        snprintf(child, sizeof(child), "%s\\%s", path, name);
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            if (size_recursive_internal(child, acc) != 0) { FindClose(h); return -1; }
        } else {
            uint64_t size = ((uint64_t)fd.nFileSizeHigh << 32) | fd.nFileSizeLow;
            *acc += size;
        }
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    return 0;
#endif
}

int32_t fossil_io_dir_size(const char *path, uint64_t *bytes){
    if (!path || !bytes) return -1;
    *bytes = 0;
    if (fossil_io_dir_is_file(path)){
        stat_t st;
        if (stat_native(path, &st) != 0) return -1;
        *bytes = (uint64_t)st.st_size;
        return 0;
    }
    if (!fossil_io_dir_is_directory(path)) return -1;
    return size_recursive_internal(path, bytes);
}

// ------------------------------------------------------------
// Scan + recursive scan
// ------------------------------------------------------------
int32_t fossil_io_dir_scan(const char *path, fossil_io_dir_scan_callback cb, void *userdata){
    if (!path || !cb) return -1;
    fossil_io_dir_iter_t it;
    memset(&it, 0, sizeof(it));
    if (fossil_io_dir_iter_open(&it, path) != 0) return -1;
    while (fossil_io_dir_iter_next(&it)){
        // The callback receives a fully populated fossil_io_dir_entry_t
        if (!cb(&it.current, userdata)) break;
    }
    fossil_io_dir_iter_close(&it);
    return 0;
}

static int32_t scan_recursive_internal(const char *path, fossil_io_dir_scan_callback cb, void *userdata){
    if (!path || !cb) return -1;
    fossil_io_dir_iter_t it;
    if (fossil_io_dir_iter_open(&it, path) != 0) return -1;
    while (fossil_io_dir_iter_next(&it)){
        // invoke callback
        if (!cb(&it.current, userdata)) {
            fossil_io_dir_iter_close(&it);
            return 0;
        }
        // if entry is a directory (type == 1), recurse
        if (it.current.type == 1){
            if (scan_recursive_internal(it.current.path, cb, userdata) != 0) {
                fossil_io_dir_iter_close(&it);
                return -1;
            }
        }
    }
    fossil_io_dir_iter_close(&it);
    return 0;
}

int32_t fossil_io_dir_scan_recursive(const char *path, fossil_io_dir_scan_callback cb, void *userdata){
    return scan_recursive_internal(path, cb, userdata);
}

// ------------------------------------------------------------
// Permissions
// ------------------------------------------------------------
int32_t fossil_io_dir_set_permissions(const char *path, int32_t mode){
    if (!path) return -1;
#ifndef _WIN32
    if (chmod(path, (mode_t)mode) == 0) return 0;
    return -1;
#else
    // Windows: best-effort - set read-only attribute if write bit not provided
    DWORD attrs = GetFileAttributesA(path);
    if (attrs == INVALID_FILE_ATTRIBUTES) return -1;
    if ((mode & 0222) == 0) attrs |= FILE_ATTRIBUTE_READONLY;
    else attrs &= ~FILE_ATTRIBUTE_READONLY;
    if (!SetFileAttributesA(path, attrs)) return -1;
    return 0;
#endif
}

int32_t fossil_io_dir_get_permissions(const char *path, int32_t *mode){
    if (!path || !mode) return -1;
#ifndef _WIN32
    struct stat st;
    if (stat_native(path, &st) != 0) return -1;
    *mode = st.st_mode & 0777;
    return 0;
#else
    DWORD attrs = GetFileAttributesA(path);
    if (attrs == INVALID_FILE_ATTRIBUTES) return -1;
    // Map readonly attribute to POSIX-ish mode
    if (attrs & FILE_ATTRIBUTE_READONLY) *mode = 0444;
    else *mode = 0666;
    return 0;
#endif
}

// ------------------------------------------------------------
// Temp directory utilities
// ------------------------------------------------------------
int32_t fossil_io_dir_temp(char *out, size_t outsz){
    if (!out) return -1;
#ifdef _WIN32
    DWORD r = GetTempPathA((DWORD)outsz, out);
    return (r > 0 && r < outsz) ? 0 : -1;
#else
    const char *t = getenv("TMPDIR");
    if (!t) t = "/tmp";
    safe_strcpy(out, t, outsz);
    return 0;
#endif
}

int32_t fossil_io_dir_create_temp(char *out, size_t outsz){
    if (!out) return -1;
    char tmpdir[PATH_MAX * 2];
    if (fossil_io_dir_temp(tmpdir, sizeof(tmpdir)) != 0) return -1;

    char tpl[PATH_MAX * 4];
    snprintf(tpl, sizeof(tpl), "%s/fossil_tmp_%u_%u", tmpdir, (unsigned)getpid(), (unsigned)time(NULL));
    // Try to create a unique directory by appending a counter
    for (int i = 0; i < 1000; ++i) {
        char path[PATH_MAX * 4 + 8]; // Extra space for "_%03d" and null terminator
        int n = snprintf(path, sizeof(path), "%s_%03d", tpl, i);
        if (n < 0 || n >= (int)sizeof(path)) continue; // Skip if truncated
        if (mkdir_native(path, 0700) == 0) {
            safe_strcpy(out, path, outsz);
            return 0;
        }
        if (errno != EEXIST) break;
    }
    return -1;
}

// ------------------------------------------------------------
// Metadata
// ------------------------------------------------------------
int32_t fossil_io_dir_get_modified(const char *path, uint64_t *timestamp){
    if (!path || !timestamp) return -1;
    stat_t st;
    if (stat_native(path, &st) != 0) return -1;
    *timestamp = (uint64_t)st.st_mtime;
    return 0;
}

int32_t fossil_io_dir_get_created(const char *path, uint64_t *timestamp){
    if (!path || !timestamp) return -1;
#ifndef _WIN32
    // POSIX does not generally expose creation time; return ctime as approximation
    stat_t st;
    if (stat_native(path, &st) != 0) return -1;
#ifdef __APPLE__
    *timestamp = (uint64_t)st.st_birthtime;
#else
    *timestamp = (uint64_t)st.st_ctime;
#endif
    return 0;
#else
    WIN32_FILE_ATTRIBUTE_DATA fad;
    if (!GetFileAttributesExA(path, GetFileExInfoStandard, &fad)) return -1;
    ULARGE_INTEGER ull;
    ull.LowPart = fad.ftCreationTime.dwLowDateTime;
    ull.HighPart = fad.ftCreationTime.dwHighDateTime;
    uint64_t seconds = (ull.QuadPart / 10000000ULL) - 11644473600ULL;
    *timestamp = seconds;
    return 0;
#endif
}

// ------------------------------------------------------------
// Backup / mirror / sync (basic implementations)
// ------------------------------------------------------------
int32_t fossil_io_dir_backup(const char *src, const char *suffix){
    if (!src || !suffix) return -1;
    char dst[2048];
    snprintf(dst, sizeof(dst), "%s%s", src, suffix);
    return fossil_io_dir_copy_recursive(src, dst);
}

int32_t fossil_io_dir_mirror(const char *src, const char *dst){
    if (!src || !dst) return -1;
    // naive mirror: if dst exists remove and copy recursive
    if (fossil_io_dir_exists(dst)) {
        if (fossil_io_dir_remove_recursive(dst) != 0) return -1;
    }
    return fossil_io_dir_copy_recursive(src, dst);
}

static int32_t sync_internal_delete_extraneous(const char *src, const char *dst){
    // delete files in dst that are not in src
    if (!src || !dst) return -1;
    fossil_io_dir_iter_t it;
    if (fossil_io_dir_iter_open(&it, dst) != 0) return -1;
    char srcchild[2048], dstchild[2048];
    while (fossil_io_dir_iter_next(&it)){
        join_paths(src, it.current.name, srcchild, sizeof(srcchild));
        join_paths(dst, it.current.name, dstchild, sizeof(dstchild));
        // Use the new type field for entry type
        if (!fossil_io_dir_is_directory(srcchild) && !fossil_io_dir_is_file(srcchild) && !fossil_io_dir_is_symlink(srcchild)){
            // not present in src => remove from dst (recursively)
            if (it.current.type == 1) { // directory
                fossil_io_dir_remove_recursive(dstchild);
            } else if (it.current.type == 2) { // symlink
                remove(dstchild);
            } else if (it.current.type == 0) { // file
                remove(dstchild);
            } else {
                // other types: pipe, sock, blockdev, chardev, etc.
                remove(dstchild);
            }
        } else {
            // present, recurse for directories
            if (it.current.type == 1) sync_internal_delete_extraneous(srcchild, dstchild);
        }
    }
    fossil_io_dir_iter_close(&it);
    return 0;
}

// ------------------------------------------------------------
// Link and Symlink
// ------------------------------------------------------------
int32_t fossil_io_dir_link(const char *target, const char *linkpath){
    if (!target || !linkpath) return -1;
#ifdef _WIN32
    // On Windows, CreateHardLinkA only works for files, not directories
    // Try to create a hard link for files
    if (fossil_io_dir_is_file(target)) {
        if (CreateHardLinkA(linkpath, target, NULL)) return 0;
        return -1;
    }
    // Hard links to directories are not supported on Windows
    return -1;
#else
    // POSIX: use link()
    if (link(target, linkpath) == 0) return 0;
    return -1;
#endif
}

int32_t fossil_io_dir_symlink(const char *target, const char *linkpath){
    if (!target || !linkpath) return -1;
#ifdef _WIN32
    // On Windows, use CreateSymbolicLinkA
    DWORD flags = 0;
    if (fossil_io_dir_is_directory(target))
        flags = 0x1; // SYMBOLIC_LINK_FLAG_DIRECTORY
    // Since Windows 10 1703, unprivileged symlink creation is allowed in developer mode
    if (CreateSymbolicLinkA(linkpath, target, flags)) return 0;
    return -1;
#else
    // POSIX: use symlink()
    if (symlink(target, linkpath) == 0) return 0;
    return -1;
#endif
}

int32_t fossil_io_dir_sync(const char *src, const char *dst, int32_t delete_extraneous){
    if (!src || !dst) return -1;
    if (!fossil_io_dir_exists(src)) return -1;
    fossil_io_dir_create(dst);

    // copy entries from src to dst (overwrite)
    fossil_io_dir_iter_t it;
    if (fossil_io_dir_iter_open(&it, src) != 0) return -1;
    char srcchild[2048], dstchild[2048];
    while (fossil_io_dir_iter_next(&it)){
        join_paths(src, it.current.name, srcchild, sizeof(srcchild));
        join_paths(dst, it.current.name, dstchild, sizeof(dstchild));
        switch (it.current.type) {
            case 1: // directory
                fossil_io_dir_sync(srcchild, dstchild, delete_extraneous);
                break;
            case 2: // symlink
                // Copy symlink using fossil_io_dir_symlink
                // Remove any existing file/dir at dstchild first
                fossil_io_dir_remove_recursive(dstchild);
                fossil_io_dir_symlink(srcchild, dstchild);
                break;
            case 0: // file
            default: {
                // soon to handle hard-link creation
                break;
            }
        }
    }
    fossil_io_dir_iter_close(&it);

    if (delete_extraneous){
        sync_internal_delete_extraneous(src, dst);
    }
    return 0;
}
