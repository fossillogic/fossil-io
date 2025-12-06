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


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilites
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Define the test suite and add test cases
FOSSIL_SUITE(c_dir_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_dir_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_dir_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_dir_create_and_exists) {
    const char *dirname = "test_dir_create";
    // Ensure directory does not exist
    fossil_io_dir_remove_recursive(dirname);
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_exists(dirname));
    // Create directory
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_create(dirname));
    // Check existence
    ASSUME_ITS_EQUAL_I32(1, fossil_io_dir_exists(dirname));
    // Cleanup
    fossil_io_dir_remove_recursive(dirname);
}

FOSSIL_TEST(c_test_dir_remove_and_remove_recursive) {
    const char *dirname = "test_dir_remove";
    fossil_io_dir_remove_recursive(dirname);
    fossil_io_dir_create(dirname);
    // Remove directory
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_remove(dirname));
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_exists(dirname));
    // Create nested structure
    fossil_io_dir_create(dirname);
    char subdir[128];
    fossil_io_dir_join(dirname, "sub", subdir, sizeof(subdir));
    fossil_io_dir_create(subdir);
    // Remove recursively
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_remove_recursive(dirname));
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_exists(dirname));
}

FOSSIL_TEST(c_test_dir_clear) {
    const char *dirname = "test_dir_clear";
    fossil_io_dir_remove_recursive(dirname);
    fossil_io_dir_create(dirname);
    char fname[128];
    fossil_io_dir_join(dirname, "file.txt", fname, sizeof(fname));
    FILE *f = fopen(fname, "w");
    fprintf(f, "data");
    fclose(f);
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_clear(dirname));
    size_t count = 0;
    fossil_io_dir_count(dirname, &count);
    ASSUME_ITS_EQUAL_I32(0, count);
    fossil_io_dir_remove_recursive(dirname);
}

FOSSIL_TEST(c_test_dir_copy_and_copy_recursive) {
    const char *src = "test_dir_src";
    const char *dst = "test_dir_dst";
    fossil_io_dir_remove_recursive(src);
    fossil_io_dir_remove_recursive(dst);
    fossil_io_dir_create(src);
    char fname[128];
    fossil_io_dir_join(src, "file.txt", fname, sizeof(fname));
    FILE *f = fopen(fname, "w");
    fprintf(f, "copytest");
    fclose(f);
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_copy(src, dst));
    size_t count = 0;
    fossil_io_dir_count(dst, &count);
    ASSUME_ITS_TRUE(count > 0);
    fossil_io_dir_remove_recursive(src);
    fossil_io_dir_remove_recursive(dst);
}

FOSSIL_TEST(c_test_dir_move_and_rename) {
    const char *src = "test_dir_move_src";
    const char *dst = "test_dir_move_dst";
    fossil_io_dir_remove_recursive(src);
    fossil_io_dir_remove_recursive(dst);
    fossil_io_dir_create(src);

    // Ensure src exists before move
    ASSUME_ITS_EQUAL_I32(1, fossil_io_dir_exists(src));
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_exists(dst));

    // Move src to dst
    int move_result = fossil_io_dir_move(src, dst);
    ASSUME_ITS_EQUAL_I32(0, move_result);

    // After move, src should not exist, dst should exist
    ASSUME_ITS_EQUAL_I32(1, fossil_io_dir_exists(dst));
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_exists(src));

    const char *renamed = "test_dir_renamed";
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_rename(dst, renamed));
    ASSUME_ITS_EQUAL_I32(1, fossil_io_dir_exists(renamed));
    fossil_io_dir_remove_recursive(renamed);
}

FOSSIL_TEST(c_test_dir_iter_and_list) {
    const char *dirname = "test_dir_iter";
    fossil_io_dir_remove_recursive(dirname);
    fossil_io_dir_create(dirname);

    // Create a file in the directory
    char fname[128];
    fossil_io_dir_join(dirname, "file.txt", fname, sizeof(fname));
    FILE *f = fopen(fname, "w");
    fprintf(f, "entry");
    fclose(f);

    // Test directory iterator
    fossil_io_dir_iter_t it;
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_iter_open(&it, dirname));
    int found = 0;
    while (fossil_io_dir_iter_next(&it)) {
        // Check if the entry is "file.txt"
        if (strcmp(it.current.name, "file.txt") == 0) {
            found = 1;
            // Optionally, check entry fields
            ASSUME_ITS_EQUAL_I32(0, it.current.type); // 0=file
            ASSUME_ITS_TRUE(it.current.size > 0);
            ASSUME_ITS_TRUE(strlen(it.current.path) > 0);
        }
    }
    fossil_io_dir_iter_close(&it);
    ASSUME_ITS_TRUE(found);

    // Test directory listing
    fossil_io_dir_entry_t entries[10];
    size_t count = 0;
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_list(dirname, entries, &count, 10));
    ASSUME_ITS_TRUE(count > 0);
    int listed = 0;
    for (size_t i = 0; i < count; ++i) {
        if (strcmp(entries[i].name, "file.txt") == 0) {
            listed = 1;
            ASSUME_ITS_EQUAL_I32(0, entries[i].type); // 0=file
            ASSUME_ITS_TRUE(entries[i].size > 0);
            ASSUME_ITS_TRUE(strlen(entries[i].path) > 0);
        }
    }
    ASSUME_ITS_TRUE(listed);

    fossil_io_dir_remove_recursive(dirname);
}

FOSSIL_TEST(c_test_dir_path_utilities) {
    char out[256];
//    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_is_absolute("/tmp"));
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_is_absolute("relative/path"));
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_join("/tmp", "file.txt", out, sizeof(out)));
    ASSUME_ITS_TRUE(strstr(out, "file.txt") != NULL);
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_basename("/tmp/file.txt", out, sizeof(out)));
    ASSUME_ITS_EQUAL_CSTR("file.txt", out);
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_dirname("/tmp/file.txt", out, sizeof(out)));
    ASSUME_ITS_TRUE(strstr(out, "/tmp") != NULL);
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_normalize("/tmp//foo/", out, sizeof(out)));
    ASSUME_ITS_TRUE(strstr(out, "/tmp/foo") != NULL);
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_realpath(".", out, sizeof(out)));
    ASSUME_ITS_TRUE(strlen(out) > 0);
}

FOSSIL_TEST(c_test_dir_is_empty_and_count_and_size) {
    const char *dirname = "test_dir_empty";
    fossil_io_dir_remove_recursive(dirname);
    fossil_io_dir_create(dirname);
    ASSUME_ITS_EQUAL_I32(1, fossil_io_dir_is_empty(dirname));
    char fname[128];
    fossil_io_dir_join(dirname, "file.txt", fname, sizeof(fname));
    FILE *f = fopen(fname, "w");
    fprintf(f, "abc");
    fclose(f);
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_is_empty(dirname));
    size_t count = 0;
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_count(dirname, &count));
    ASSUME_ITS_EQUAL_I32(1, count);
    uint64_t bytes = 0;
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_size(dirname, &bytes));
    ASSUME_ITS_TRUE(bytes > 0);
    fossil_io_dir_remove_recursive(dirname);
}

FOSSIL_TEST(c_test_dir_permissions) {
    const char *dirname = "test_dir_perm";
    fossil_io_dir_remove_recursive(dirname);
    fossil_io_dir_create(dirname);
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_set_permissions(dirname, 0755));
    int32_t mode = 0;
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_get_permissions(dirname, &mode));
    ASSUME_ITS_TRUE(mode != 0);
    fossil_io_dir_remove_recursive(dirname);
}

FOSSIL_TEST(c_test_dir_temp_and_create_temp) {
    char tempdir[256];
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_temp(tempdir, sizeof(tempdir)));
    ASSUME_ITS_TRUE(strlen(tempdir) > 0);
    char tempcreated[256];
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_create_temp(tempcreated, sizeof(tempcreated)));
    ASSUME_ITS_TRUE(fossil_io_dir_exists(tempcreated));
    fossil_io_dir_remove_recursive(tempcreated);
}

FOSSIL_TEST(c_test_dir_metadata) {
    const char *dirname = "test_dir_meta";
    fossil_io_dir_remove_recursive(dirname);
    fossil_io_dir_create(dirname);
    uint64_t mod = 0, created = 0;
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_get_modified(dirname, &mod));
    ASSUME_ITS_TRUE(mod > 0);
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_get_created(dirname, &created));
    ASSUME_ITS_TRUE(created > 0);
    fossil_io_dir_remove_recursive(dirname);
}

FOSSIL_TEST(c_test_dir_backup_mirror_sync) {
    const char *src = "test_dir_sync_src";
    const char *dst = "test_dir_sync_dst";
    fossil_io_dir_remove_recursive(src);
    fossil_io_dir_remove_recursive(dst);
    fossil_io_dir_create(src);
    char fname[128];
    fossil_io_dir_join(src, "file.txt", fname, sizeof(fname));
    FILE *f = fopen(fname, "w");
    fprintf(f, "sync");
    fclose(f);
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_backup(src, "_bak"));
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_mirror(src, dst));
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_sync(src, dst, 1));
    fossil_io_dir_remove_recursive(src);
    fossil_io_dir_remove_recursive(dst);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_dir_tests) {
    FOSSIL_TEST_ADD(c_dir_suite, c_test_dir_create_and_exists);
    FOSSIL_TEST_ADD(c_dir_suite, c_test_dir_remove_and_remove_recursive);
    FOSSIL_TEST_ADD(c_dir_suite, c_test_dir_clear);
    FOSSIL_TEST_ADD(c_dir_suite, c_test_dir_copy_and_copy_recursive);
    FOSSIL_TEST_ADD(c_dir_suite, c_test_dir_move_and_rename);
    FOSSIL_TEST_ADD(c_dir_suite, c_test_dir_iter_and_list);
    FOSSIL_TEST_ADD(c_dir_suite, c_test_dir_path_utilities);
    FOSSIL_TEST_ADD(c_dir_suite, c_test_dir_is_empty_and_count_and_size);
    FOSSIL_TEST_ADD(c_dir_suite, c_test_dir_permissions);
    FOSSIL_TEST_ADD(c_dir_suite, c_test_dir_temp_and_create_temp);
    FOSSIL_TEST_ADD(c_dir_suite, c_test_dir_metadata);
    FOSSIL_TEST_ADD(c_dir_suite, c_test_dir_backup_mirror_sync);

    FOSSIL_TEST_REGISTER(c_dir_suite);
}
