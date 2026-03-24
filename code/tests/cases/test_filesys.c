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
#include <fossil/pizza/framework.h>

#include "fossil/io/framework.h"
#include <stdio.h> // for fpos_t or fpos64_t if needed
#include <string.h>

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilites
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Define the test suite and add test cases
FOSSIL_SUITE(c_filesys_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_filesys_suite)
{
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_filesys_suite)
{
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Test filesystem object initialization and metadata
FOSSIL_TEST(c_test_filesys_init_file)
{
    fossil_io_filesys_obj_t obj;
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_init(&obj, "C:\\test_file.txt");
#else
    int32_t result = fossil_io_filesys_init(&obj, "/tmp/test_file.txt");
#endif
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(FOSSIL_FILESYS_TYPE_FILE, obj.type);
}

FOSSIL_TEST(c_test_filesys_init_directory)
{
    fossil_io_filesys_obj_t obj;
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_init(&obj, "C:\\temp");
#else
    int32_t result = fossil_io_filesys_init(&obj, "/tmp");
#endif
    ASSUME_ITS_EQUAL_I32(0, result);
    ASSUME_ITS_EQUAL_I32(FOSSIL_FILESYS_TYPE_DIR, obj.type);
}

FOSSIL_TEST(c_test_filesys_init_nonexistent)
{
    fossil_io_filesys_obj_t obj;
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_init(&obj, "C:\\nonexistent\\path\\file.txt");
#else
    int32_t result = fossil_io_filesys_init(&obj, "/nonexistent/path/file.txt");
#endif
    ASSUME_ITS_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_filesys_refresh)
{
    fossil_io_filesys_obj_t obj;
#if defined(_WIN32) || defined(_WIN64)
    int32_t init_result = fossil_io_filesys_init(&obj, "C:\\temp");
#else
    int32_t init_result = fossil_io_filesys_init(&obj, "/tmp");
#endif
    ASSUME_ITS_EQUAL_I32(0, init_result);
    int32_t refresh_result = fossil_io_filesys_refresh(&obj);
    ASSUME_ITS_EQUAL_I32(0, refresh_result);
}

FOSSIL_TEST(c_test_filesys_exists_true)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_exists("C:\\temp");
#else
    int32_t result = fossil_io_filesys_exists("/tmp");
#endif
    ASSUME_ITS_EQUAL_I32(1, result);
}

FOSSIL_TEST(c_test_filesys_exists_false)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_exists("C:\\nonexistent\\path\\xyz");
#else
    int32_t result = fossil_io_filesys_exists("/nonexistent/path/xyz");
#endif
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST(c_test_filesys_file_open_write)
{
    fossil_io_filesys_file_t file;
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_file_open(&file, "C:\\temp\\test_write.txt", "w");
#else
    int32_t result = fossil_io_filesys_file_open(&file, "/tmp/test_write.txt", "w");
#endif
    ASSUME_NOT_EQUAL_I32(result, -1);
    if (result == 0) {
        fossil_io_filesys_file_close(&file);
    }
}

FOSSIL_TEST(c_test_filesys_file_write_read)
{
    fossil_io_filesys_file_t file;
#if defined(_WIN32) || defined(_WIN64)
    int32_t open_result = fossil_io_filesys_file_open(&file, "C:\\temp\\test_rw.txt", "w+");
#else
    int32_t open_result = fossil_io_filesys_file_open(&file, "/tmp/test_rw.txt", "w+");
#endif
    if (open_result == 0) {
        const char *data = "Hello World";
        size_t written = fossil_io_filesys_file_write(&file, data, 1, strlen(data));
        ASSUME_ITS_EQUAL_SIZE(written, strlen(data));
        fossil_io_filesys_file_close(&file);
    }
}

FOSSIL_TEST(c_test_filesys_file_seek)
{
    fossil_io_filesys_file_t file;
#if defined(_WIN32) || defined(_WIN64)
    int32_t open_result = fossil_io_filesys_file_open(&file, "C:\\temp\\test_seek.txt", "w+");
#else
    int32_t open_result = fossil_io_filesys_file_open(&file, "/tmp/test_seek.txt", "w+");
#endif
    if (open_result == 0) {
        fossil_io_filesys_file_seek(&file, 0, SEEK_SET);
        int64_t pos = fossil_io_filesys_file_tell(&file);
        ASSUME_ITS_EQUAL_I64(pos, 0);
        fossil_io_filesys_file_close(&file);
    }
}

FOSSIL_TEST(c_test_filesys_file_flush)
{
    fossil_io_filesys_file_t file;
#if defined(_WIN32) || defined(_WIN64)
    int32_t open_result = fossil_io_filesys_file_open(&file, "C:\\temp\\test_flush.txt", "w");
#else
    int32_t open_result = fossil_io_filesys_file_open(&file, "/tmp/test_flush.txt", "w");
#endif
    if (open_result == 0) {
        int32_t flush_result = fossil_io_filesys_file_flush(&file);
        ASSUME_ITS_EQUAL_I32(flush_result, 0);
        fossil_io_filesys_file_close(&file);
    }
}

FOSSIL_TEST(c_test_filesys_file_size)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t size = fossil_io_filesys_file_size("C:\\temp\\test.txt");
#else
    int32_t size = fossil_io_filesys_file_size("/tmp/test.txt");
#endif
    ASSUME_NOT_LESS_THAN_I32(size, -1);
}

FOSSIL_TEST(c_test_filesys_file_truncate)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_file_truncate("C:\\temp\\test_truncate.txt", 100);
#else
    int32_t result = fossil_io_filesys_file_truncate("/tmp/test_truncate.txt", 100);
#endif
    ASSUME_NOT_EQUAL_I32(result, -2);
}

// Test directory operations
FOSSIL_TEST(c_test_filesys_dir_create_simple)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_dir_create("C:\\temp\\test_dir_simple", true);
#else
    int32_t result = fossil_io_filesys_dir_create("/tmp/test_dir_simple", true);
#endif
    ASSUME_NOT_EQUAL_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_dir_create_recursive)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_dir_create("C:\\temp\\test_dir_a\\b\\c", true);
#else
    int32_t result = fossil_io_filesys_dir_create("/tmp/test_dir_a/b/c", true);
#endif
    ASSUME_NOT_EQUAL_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_dir_list)
{
    fossil_io_filesys_obj_t entries[256];
    size_t out_count = 0;
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_dir_list("C:\\temp", entries, 256, &out_count);
#else
    int32_t result = fossil_io_filesys_dir_list("/tmp", entries, 256, &out_count);
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_dir_walk)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_dir_walk("C:\\temp", NULL, NULL);
#else
    int32_t result = fossil_io_filesys_dir_walk("/tmp", NULL, NULL);
#endif
    ASSUME_NOT_EQUAL_I32(result, -2);
}

// Test path operations
FOSSIL_TEST(c_test_filesys_getcwd)
{
    char cwd[512];
    int32_t result = fossil_io_filesys_getcwd(cwd, 512);
    ASSUME_ITS_EQUAL_I32(result, 0);
    ASSUME_NOT_CNULL(cwd);
}

FOSSIL_TEST(c_test_filesys_abspath)
{
    char abs_path[512];
    int32_t result = fossil_io_filesys_abspath(".", abs_path, 512);
    ASSUME_ITS_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_filesys_dirname)
{
    char dir[256];
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_dirname("C:\\home\\user\\file.txt", dir, 256);
    ASSUME_ITS_EQUAL_I32(result, 0);
    ASSUME_ITS_CSTR_CONTAINS(dir, "C:\\home\\user");
#else
    int32_t result = fossil_io_filesys_dirname("/home/user/file.txt", dir, 256);
    ASSUME_ITS_EQUAL_I32(result, 0);
    ASSUME_ITS_CSTR_CONTAINS(dir, "/home/user");
#endif
}

FOSSIL_TEST(c_test_filesys_basename)
{
    char name[256];
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_basename("C:\\home\\user\\file.txt", name, 256);
#else
    int32_t result = fossil_io_filesys_basename("/home/user/file.txt", name, 256);
#endif
    ASSUME_ITS_EQUAL_I32(result, 0);
    ASSUME_ITS_EQUAL_CSTR(name, "file.txt");
}

FOSSIL_TEST(c_test_filesys_extension)
{
    char ext[64];
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_extension("C:\\home\\user\\file.txt", ext, 64);
#else
    int32_t result = fossil_io_filesys_extension("/home/user/file.txt", ext, 64);
#endif
    ASSUME_ITS_EQUAL_I32(result, 0);
    ASSUME_ITS_EQUAL_CSTR(ext, ".txt");
}

// Test utility functions
FOSSIL_TEST(c_test_filesys_type_string_file)
{
    const char *type_str = fossil_io_filesys_type_string(FOSSIL_FILESYS_TYPE_FILE);
    ASSUME_NOT_CNULL(type_str);
    ASSUME_ITS_CSTR_CONTAINS(type_str, "file");
}

FOSSIL_TEST(c_test_filesys_type_string_dir)
{
    const char *type_str = fossil_io_filesys_type_string(FOSSIL_FILESYS_TYPE_DIR);
    ASSUME_NOT_CNULL(type_str);
    ASSUME_ITS_CSTR_CONTAINS(type_str, "dir");
}

FOSSIL_TEST(c_test_filesys_type_string_link)
{
    const char *type_str = fossil_io_filesys_type_string(FOSSIL_FILESYS_TYPE_LINK);
    ASSUME_NOT_CNULL(type_str);
}

FOSSIL_TEST(c_test_filesys_type_string_unknown)
{
    const char *type_str = fossil_io_filesys_type_string(FOSSIL_FILESYS_TYPE_UNKNOWN);
    ASSUME_NOT_CNULL(type_str);
}

// Test stat operations
FOSSIL_TEST(c_test_filesys_stat_file)
{
    fossil_io_filesys_obj_t obj;
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_stat("C:\\temp\\test.txt", &obj);
#else
    int32_t result = fossil_io_filesys_stat("/tmp/test.txt", &obj);
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_stat_directory)
{
    fossil_io_filesys_obj_t obj;
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_stat("C:\\temp", &obj);
#else
    int32_t result = fossil_io_filesys_stat("/tmp", &obj);
#endif
    ASSUME_ITS_EQUAL_I32(result, 0);
    ASSUME_ITS_EQUAL_I32(obj.type, FOSSIL_FILESYS_TYPE_DIR);
}

// Test move and copy operations
FOSSIL_TEST(c_test_filesys_move)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_move("C:\\temp\\old_name.txt", "C:\\temp\\new_name.txt");
#else
    int32_t result = fossil_io_filesys_move("/tmp/old_name.txt", "/tmp/new_name.txt");
#endif
    ASSUME_NOT_EQUAL_I32(result, -2);
}

FOSSIL_TEST(c_test_filesys_copy)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_copy("C:\\temp\\source.txt", "C:\\temp\\dest.txt", true);
#else
    int32_t result = fossil_io_filesys_copy("/tmp/source.txt", "/tmp/dest.txt", true);
#endif
    ASSUME_NOT_EQUAL_I32(result, -2);
}

// Test remove operations
FOSSIL_TEST(c_test_filesys_remove_file)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_remove("C:\\temp\\removable.txt", false);
#else
    int32_t result = fossil_io_filesys_remove("/tmp/removable.txt", false);
#endif
    ASSUME_NOT_EQUAL_I32(result, -2);
}

FOSSIL_TEST(c_test_filesys_remove_directory_recursive)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_remove("C:\\temp\\removable_dir", true);
#else
    int32_t result = fossil_io_filesys_remove("/tmp/removable_dir", true);
#endif
    ASSUME_NOT_EQUAL_I32(result, -2);
}

// Test swap operation
FOSSIL_TEST(c_test_filesys_swap)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_swap("C:\\temp\\file1.txt", "C:\\temp\\file2.txt");
#else
    int32_t result = fossil_io_filesys_swap("/tmp/file1.txt", "/tmp/file2.txt");
#endif
    ASSUME_NOT_EQUAL_I32(result, -2);
}

// Test transaction operations
FOSSIL_TEST(c_test_filesys_tx_begin_commit)
{
    int32_t begin_result = fossil_io_filesys_tx_begin();
    ASSUME_NOT_LESS_THAN_I32(begin_result, -1);
    int32_t commit_result = fossil_io_filesys_tx_commit();
    ASSUME_NOT_LESS_THAN_I32(commit_result, -1);
}

FOSSIL_TEST(c_test_filesys_tx_rollback)
{
    fossil_io_filesys_tx_begin();
    int32_t result = fossil_io_filesys_tx_rollback();
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

// Test link operations
FOSSIL_TEST(c_test_filesys_link_create_symbolic)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_link_create("C:\\temp\\original.txt", "C:\\temp\\link.txt", true);
#else
    int32_t result = fossil_io_filesys_link_create("/tmp/original.txt", "/tmp/link.txt", true);
#endif
    ASSUME_NOT_EQUAL_I32(result, -2);
}

FOSSIL_TEST(c_test_filesys_link_create_hard)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_link_create("C:\\temp\\original.txt", "C:\\temp\\hardlink.txt", false);
#else
    int32_t result = fossil_io_filesys_link_create("/tmp/original.txt", "/tmp/hardlink.txt", false);
#endif
    ASSUME_NOT_EQUAL_I32(result, -2);
}

FOSSIL_TEST(c_test_filesys_link_is_symbolic)
{
    bool is_sym = false;
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_link_is_symbolic("C:\\temp\\link.txt", &is_sym);
#else
    int32_t result = fossil_io_filesys_link_is_symbolic("/tmp/link.txt", &is_sym);
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_chdir)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_chdir("C:\\temp");
#else
    int32_t result = fossil_io_filesys_chdir("/tmp");
#endif
    ASSUME_ITS_EQUAL_I32(result, 0);
}

// Test file rewrite operations
static int test_rewrite_uppercase(void *buf, size_t *size, void *user_data)
{
    (void)user_data;
    if (!buf || !size) return -1;
    unsigned char *data = (unsigned char *)buf;
    for (size_t i = 0; i < *size; ++i) {
        if (data[i] >= 'a' && data[i] <= 'z') {
            data[i] = data[i] - 'a' + 'A';
        }
    }
    // Use user_data in a harmless way to avoid unused warning
    if (user_data) {
        volatile uintptr_t dummy = (uintptr_t)user_data;
        (void)dummy;
    }
    return 0;
}

static int test_rewrite_truncate(void *buf, size_t *size, void *user_data)
{
    // Use buf in a harmless way to avoid unused warning
    if (buf) {
        volatile unsigned char dummy = *((unsigned char *)buf);
        (void)dummy;
    }
    if (!size) return -1;
    size_t max_size = (size_t)(uintptr_t)user_data;
    if (*size > max_size) {
        *size = max_size;
    }
    return 0;
}

static int test_rewrite_fail(void *buf, size_t *size, void *user_data)
{
    (void)buf;
    (void)size;
    (void)user_data;
    return -1;
}

FOSSIL_TEST(c_test_filesys_file_rewrite_basic)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_file_rewrite("C:\\temp\\rewrite_test.txt", test_rewrite_uppercase, NULL);
#else
    int32_t result = fossil_io_filesys_file_rewrite("/tmp/rewrite_test.txt", test_rewrite_uppercase, NULL);
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_file_rewrite_with_userdata)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_file_rewrite("C:\\temp\\rewrite_test.txt", test_rewrite_truncate, (void *)50);
#else
    int32_t result = fossil_io_filesys_file_rewrite("/tmp/rewrite_test.txt", test_rewrite_truncate, (void *)50);
#endif
    ASSUME_NOT_EQUAL_I32(result, -2);
}

FOSSIL_TEST(c_test_filesys_file_rewrite_null_path)
{
    int32_t result = fossil_io_filesys_file_rewrite(NULL, test_rewrite_uppercase, NULL);
    ASSUME_NOT_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_filesys_file_rewrite_null_transform)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_file_rewrite("C:\\temp\\test.txt", NULL, NULL);
#else
    int32_t result = fossil_io_filesys_file_rewrite("/tmp/test.txt", NULL, NULL);
#endif
    ASSUME_NOT_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_filesys_file_rewrite_nonexistent)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_file_rewrite("C:\\temp\\nonexistent_rewrite.txt", test_rewrite_uppercase, NULL);
#else
    int32_t result = fossil_io_filesys_file_rewrite("/tmp/nonexistent_rewrite.txt", test_rewrite_uppercase, NULL);
#endif
    ASSUME_NOT_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_filesys_file_rewrite_transform_failure)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_file_rewrite("C:\\temp\\test.txt", test_rewrite_fail, NULL);
#else
    int32_t result = fossil_io_filesys_file_rewrite("/tmp/test.txt", test_rewrite_fail, NULL);
#endif
    ASSUME_NOT_EQUAL_I32(result, 0);
}

// Test file format detection
FOSSIL_TEST(c_test_filesys_file_format_null_path)
{
    char format[64];
    int32_t result = fossil_io_filesys_file_format(NULL, format, sizeof(format));
    ASSUME_NOT_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_filesys_file_format_null_output)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_file_format("C:\\temp\\test.txt", NULL, 64);
#else
    int32_t result = fossil_io_filesys_file_format("/tmp/test.txt", NULL, 64);
#endif
    ASSUME_NOT_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_filesys_file_format_zero_length)
{
#if defined(_WIN32) || defined(_WIN64)
    char format[64];
    int32_t result = fossil_io_filesys_file_format("C:\\temp\\test.txt", format, 0);
#else
    char format[64];
    int32_t result = fossil_io_filesys_file_format("/tmp/test.txt", format, 0);
#endif
    ASSUME_NOT_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_filesys_file_format_nonexistent)
{
#if defined(_WIN32) || defined(_WIN64)
    char format[64];
    int32_t result = fossil_io_filesys_file_format("C:\\temp\\nonexistent_format.xyz", format, sizeof(format));
#else
    char format[64];
    int32_t result = fossil_io_filesys_file_format("/tmp/nonexistent_format.xyz", format, sizeof(format));
#endif
    ASSUME_NOT_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_filesys_file_format_zip)
{
#if defined(_WIN32) || defined(_WIN64)
    const char *path = "C:\\temp\\archive.zip";
#else
    const char *path = "/tmp/archive.zip";
#endif
    // Write minimal ZIP header
    FILE *f = fopen(path, "wb");
    if (f) {
        unsigned char zip_hdr[] = {0x50, 0x4B, 0x03, 0x04};
        fwrite(zip_hdr, 1, sizeof(zip_hdr), f);
        fclose(f);
    }
    char format[64];
    int32_t result = fossil_io_filesys_file_format(path, format, sizeof(format));
    ASSUME_ITS_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_filesys_file_format_gzip)
{
#if defined(_WIN32) || defined(_WIN64)
    const char *path = "C:\\temp\\file.tar.gz";
#else
    const char *path = "/tmp/file.tar.gz";
#endif
    // Write minimal GZIP header
    FILE *f = fopen(path, "wb");
    if (f) {
        unsigned char gz_hdr[] = {0x1F, 0x8B, 0x08, 0x00};
        fwrite(gz_hdr, 1, sizeof(gz_hdr), f);
        fclose(f);
    }
    char format[64];
    int32_t result = fossil_io_filesys_file_format(path, format, sizeof(format));
    ASSUME_ITS_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_filesys_file_format_bzip2)
{
#if defined(_WIN32) || defined(_WIN64)
    const char *path = "C:\\temp\\file.tar.bz2";
#else
    const char *path = "/tmp/file.tar.bz2";
#endif
    // Write minimal BZIP2 header
    FILE *f = fopen(path, "wb");
    if (f) {
        unsigned char bz2_hdr[] = {'B', 'Z', 'h'};
        fwrite(bz2_hdr, 1, sizeof(bz2_hdr), f);
        fclose(f);
    }
    char format[64];
    int32_t result = fossil_io_filesys_file_format(path, format, sizeof(format));
    ASSUME_ITS_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_filesys_file_format_xz)
{
#if defined(_WIN32) || defined(_WIN64)
    const char *path = "C:\\temp\\file.tar.xz";
#else
    const char *path = "/tmp/file.tar.xz";
#endif
    // Write minimal XZ header
    FILE *f = fopen(path, "wb");
    if (f) {
        unsigned char xz_hdr[] = {0xFD, '7', 'z', 'X', 'Z', 0x00};
        fwrite(xz_hdr, 1, sizeof(xz_hdr), f);
        fclose(f);
    }
    char format[64];
    int32_t result = fossil_io_filesys_file_format(path, format, sizeof(format));
    ASSUME_ITS_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_filesys_file_format_png)
{
#if defined(_WIN32) || defined(_WIN64)
    const char *path = "C:\\temp\\image.png";
#else
    const char *path = "/tmp/image.png";
#endif
    // Write minimal PNG header
    FILE *f = fopen(path, "wb");
    if (f) {
        unsigned char png_hdr[] = {0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};
        fwrite(png_hdr, 1, sizeof(png_hdr), f);
        fclose(f);
    }
    char format[64];
    int32_t result = fossil_io_filesys_file_format(path, format, sizeof(format));
    ASSUME_ITS_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_filesys_file_format_jpeg)
{
#if defined(_WIN32) || defined(_WIN64)
    const char *path = "C:\\temp\\image.jpg";
#else
    const char *path = "/tmp/image.jpg";
#endif
    // Write minimal JPEG header
    FILE *f = fopen(path, "wb");
    if (f) {
        unsigned char jpg_hdr[] = {0xFF, 0xD8, 0xFF};
        fwrite(jpg_hdr, 1, sizeof(jpg_hdr), f);
        fclose(f);
    }
    char format[64];
    int32_t result = fossil_io_filesys_file_format(path, format, sizeof(format));
    ASSUME_ITS_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_filesys_file_format_gif)
{
#if defined(_WIN32) || defined(_WIN64)
    const char *path = "C:\\temp\\image.gif";
#else
    const char *path = "/tmp/image.gif";
#endif
    // Write minimal GIF header
    FILE *f = fopen(path, "wb");
    if (f) {
        unsigned char gif_hdr[] = {'G', 'I', 'F', '8', '9', 'a'};
        fwrite(gif_hdr, 1, sizeof(gif_hdr), f);
        fclose(f);
    }
    char format[64];
    int32_t result = fossil_io_filesys_file_format(path, format, sizeof(format));
    ASSUME_ITS_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_filesys_file_format_pdf)
{
#if defined(_WIN32) || defined(_WIN64)
    const char *path = "C:\\temp\\document.pdf";
#else
    const char *path = "/tmp/document.pdf";
#endif
    // Write minimal PDF header
    FILE *f = fopen(path, "wb");
    if (f) {
        unsigned char pdf_hdr[] = {'%', 'P', 'D', 'F', '-'};
        fwrite(pdf_hdr, 1, sizeof(pdf_hdr), f);
        fclose(f);
    }
    char format[64];
    int32_t result = fossil_io_filesys_file_format(path, format, sizeof(format));
    ASSUME_ITS_EQUAL_I32(result, 0);
}

FOSSIL_TEST(c_test_filesys_file_format_elf)
{
#if defined(_WIN32) || defined(_WIN64)
    char format[64];
    int32_t result = fossil_io_filesys_file_format("C:\\temp\\executable", format, sizeof(format));
#else
    char format[64];
    int32_t result = fossil_io_filesys_file_format("/tmp/executable", format, sizeof(format));
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_file_format_pe_executable)
{
#if defined(_WIN32) || defined(_WIN64)
    char format[64];
    int32_t result = fossil_io_filesys_file_format("C:\\temp\\program.exe", format, sizeof(format));
#else
    char format[64];
    int32_t result = fossil_io_filesys_file_format("/tmp/program.exe", format, sizeof(format));
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_file_format_utf16le)
{
#if defined(_WIN32) || defined(_WIN64)
    char format[64];
    int32_t result = fossil_io_filesys_file_format("C:\\temp\\text_utf16le.txt", format, sizeof(format));
#else
    char format[64];
    int32_t result = fossil_io_filesys_file_format("/tmp/text_utf16le.txt", format, sizeof(format));
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_file_format_utf16be)
{
#if defined(_WIN32) || defined(_WIN64)
    char format[64];
    int32_t result = fossil_io_filesys_file_format("C:\\temp\\text_utf16be.txt", format, sizeof(format));
#else
    char format[64];
    int32_t result = fossil_io_filesys_file_format("/tmp/text_utf16be.txt", format, sizeof(format));
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_file_format_utf8_bom)
{
#if defined(_WIN32) || defined(_WIN64)
    char format[64];
    int32_t result = fossil_io_filesys_file_format("C:\\temp\\text_utf8_bom.txt", format, sizeof(format));
#else
    char format[64];
    int32_t result = fossil_io_filesys_file_format("/tmp/text_utf8_bom.txt", format, sizeof(format));
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_file_format_java_class)
{
#if defined(_WIN32) || defined(_WIN64)
    char format[64];
    int32_t result = fossil_io_filesys_file_format("C:\\temp\\Main.class", format, sizeof(format));
#else
    char format[64];
    int32_t result = fossil_io_filesys_file_format("/tmp/Main.class", format, sizeof(format));
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_file_format_small_buffer)
{
#if defined(_WIN32) || defined(_WIN64)
    const char *path = "C:\\temp\\test.txt";
#else
    const char *path = "/tmp/test.txt";
#endif
    // Write minimal text file
    FILE *f = fopen(path, "w");
    if (f) {
        fputs("A", f);
        fclose(f);
    }
    char format[2];
    int32_t result = fossil_io_filesys_file_format(path, format, sizeof(format));
    ASSUME_ITS_EQUAL_I32(result, 0);
    ASSUME_ITS_EQUAL_I32(format[sizeof(format) - 1], '\0');
}

FOSSIL_TEST(c_test_filesys_file_format_large_buffer)
{
#if defined(_WIN32) || defined(_WIN64)
    char format[1024];
    int32_t result = fossil_io_filesys_file_format("C:\\temp\\test.txt", format, sizeof(format));
#else
    char format[1024];
    int32_t result = fossil_io_filesys_file_format("/tmp/test.txt", format, sizeof(format));
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

// Permission-related tests for file, directory, and link operations

FOSSIL_TEST(c_test_filesys_file_is_readable)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_file_is_readable("C:\\temp\\test.txt");
#else
    int32_t result = fossil_io_filesys_file_is_readable("/tmp/test.txt");
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_file_is_writable)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_file_is_writable("C:\\temp\\test.txt");
#else
    int32_t result = fossil_io_filesys_file_is_writable("/tmp/test.txt");
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_file_set_perms)
{
    fossil_io_filesys_perms_t perms = {1, 1, 0};
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_file_set_perms("C:\\temp\\test.txt", perms);
#else
    int32_t result = fossil_io_filesys_file_set_perms("/tmp/test.txt", perms);
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_file_set_owner)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_file_set_owner("C:\\temp\\test.txt", "Administrator", "Administrators");
#else
    int32_t result = fossil_io_filesys_file_set_owner("/tmp/test.txt", "root", "root");
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_dir_is_readable)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_dir_is_readable("C:\\temp");
#else
    int32_t result = fossil_io_filesys_dir_is_readable("/tmp");
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_dir_is_writable)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_dir_is_writable("C:\\temp");
#else
    int32_t result = fossil_io_filesys_dir_is_writable("/tmp");
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_dir_set_perms)
{
    fossil_io_filesys_perms_t perms = {1, 1, 1};
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_dir_set_perms("C:\\temp", perms);
#else
    int32_t result = fossil_io_filesys_dir_set_perms("/tmp", perms);
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_dir_set_owner)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_dir_set_owner("C:\\temp", "Administrator", "Administrators");
#else
    int32_t result = fossil_io_filesys_dir_set_owner("/tmp", "root", "root");
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_link_set_perms)
{
    fossil_io_filesys_perms_t perms = {1, 0, 0};
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_link_set_perms("C:\\temp\\link.txt", perms);
#else
    int32_t result = fossil_io_filesys_link_set_perms("/tmp/link.txt", perms);
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

FOSSIL_TEST(c_test_filesys_link_set_owner)
{
#if defined(_WIN32) || defined(_WIN64)
    int32_t result = fossil_io_filesys_link_set_owner("C:\\temp\\link.txt", "Administrator", "Administrators");
#else
    int32_t result = fossil_io_filesys_link_set_owner("/tmp/link.txt", "root", "root");
#endif
    ASSUME_NOT_LESS_THAN_I32(result, -1);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_filesys_tests)
{
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_init_file);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_init_directory);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_init_nonexistent);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_refresh);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_exists_true);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_exists_false);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_open_write);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_write_read);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_seek);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_flush);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_size);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_truncate);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_dir_create_simple);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_dir_create_recursive);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_dir_list);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_dir_walk);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_getcwd);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_abspath);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_dirname);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_basename);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_extension);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_type_string_file);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_type_string_dir);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_type_string_link);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_type_string_unknown);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_stat_file);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_stat_directory);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_move);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_copy);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_remove_file);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_remove_directory_recursive);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_swap);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_tx_begin_commit);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_tx_rollback);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_link_create_symbolic);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_link_create_hard);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_link_is_symbolic);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_chdir);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_rewrite_basic);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_rewrite_with_userdata);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_rewrite_null_path);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_rewrite_null_transform);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_rewrite_nonexistent);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_rewrite_transform_failure);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_null_path);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_null_output);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_zero_length);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_nonexistent);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_zip);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_gzip);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_bzip2);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_xz);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_png);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_jpeg);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_gif);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_pdf);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_elf);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_pe_executable);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_utf16le);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_utf16be);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_utf8_bom);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_java_class);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_small_buffer);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_format_large_buffer);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_is_readable);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_is_writable);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_set_perms);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_file_set_owner);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_dir_is_readable);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_dir_is_writable);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_dir_set_perms);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_dir_set_owner);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_link_set_perms);
    FOSSIL_TEST_ADD(c_filesys_suite, c_test_filesys_link_set_owner);

    FOSSIL_TEST_REGISTER(c_filesys_suite);
}
