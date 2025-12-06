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
FOSSIL_SUITE(cpp_dir_suite);

// Setup function for the test suite
FOSSIL_SETUP(cpp_dir_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_dir_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(cpp_test_dir_create_and_exists) {
    using fossil::io::Dir;
    const std::string dirname = "test_dir_create";
    Dir::remove_recursive(dirname);
    ASSUME_ITS_EQUAL_I32(0, Dir::exists(dirname));
    ASSUME_ITS_EQUAL_I32(0, Dir::create(dirname));
    ASSUME_ITS_EQUAL_I32(1, Dir::exists(dirname));
    Dir::remove_recursive(dirname);
}

FOSSIL_TEST(cpp_test_dir_remove_and_remove_recursive) {
    using fossil::io::Dir;
    const std::string dirname = "test_dir_remove";
    Dir::remove_recursive(dirname);
    Dir::create(dirname);
    ASSUME_ITS_EQUAL_I32(0, Dir::remove(dirname));
    ASSUME_ITS_EQUAL_I32(0, Dir::exists(dirname));
    Dir::create(dirname);
    char subdir[128];
    Dir::join(dirname, "sub", subdir, sizeof(subdir));
    Dir::create(subdir);
    ASSUME_ITS_EQUAL_I32(0, Dir::remove_recursive(dirname));
    ASSUME_ITS_EQUAL_I32(0, Dir::exists(dirname));
}

FOSSIL_TEST(cpp_test_dir_clear) {
    using fossil::io::Dir;
    const std::string dirname = "test_dir_clear";
    Dir::remove_recursive(dirname);
    Dir::create(dirname);
    char fname[128];
    Dir::join(dirname, "file.txt", fname, sizeof(fname));
    FILE *f = fopen(fname, "w");
    fprintf(f, "data");
    fclose(f);
    ASSUME_ITS_EQUAL_I32(0, Dir::clear(dirname));
    size_t count = 0;
    Dir::count(dirname, &count);
    ASSUME_ITS_EQUAL_I32(0, count);
    Dir::remove_recursive(dirname);
}

FOSSIL_TEST(cpp_test_dir_copy_and_copy_recursive) {
    using fossil::io::Dir;
    const std::string src = "test_dir_src";
    const std::string dst = "test_dir_dst";
    Dir::remove_recursive(src);
    Dir::remove_recursive(dst);
    Dir::create(src);
    char fname[128];
    Dir::join(src, "file.txt", fname, sizeof(fname));
    FILE *f = fopen(fname, "w");
    fprintf(f, "copytest");
    fclose(f);
    ASSUME_ITS_EQUAL_I32(0, Dir::copy(src, dst));
    size_t count = 0;
    Dir::count(dst, &count);
    ASSUME_ITS_TRUE(count > 0);
    Dir::remove_recursive(src);
    Dir::remove_recursive(dst);
}

FOSSIL_TEST(cpp_test_dir_move_and_rename) {
    const char *src = "test_dir_move_src";
    const char *dst = "test_dir_move_dst";
    fossil_io_dir_remove_recursive(src);
    fossil_io_dir_remove_recursive(dst);
    fossil_io_dir_create(src);

    // Ensure src is empty before moving
    size_t count = 0;
    fossil_io_dir_count(src, &count);
    ASSUME_ITS_EQUAL_I32(0, count);

    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_move(src, dst));
    ASSUME_ITS_EQUAL_I32(1, fossil_io_dir_exists(dst));
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_exists(src));
    const char *renamed = "test_dir_renamed";
    ASSUME_ITS_EQUAL_I32(0, fossil_io_dir_rename(dst, renamed));
    ASSUME_ITS_EQUAL_I32(1, fossil_io_dir_exists(renamed));
    fossil_io_dir_remove_recursive(renamed);
}

FOSSIL_TEST(cpp_test_dir_iter_and_list) {
    using fossil::io::Dir;
    const std::string dirname = "test_dir_iter";
    Dir::remove_recursive(dirname);
    Dir::create(dirname);

    char fname[128];
    Dir::join(dirname, "file.txt", fname, sizeof(fname));
    FILE *f = fopen(fname, "w");
    fprintf(f, "entry");
    fclose(f);

    fossil_io_dir_iter_t it;
    ASSUME_ITS_EQUAL_I32(0, Dir::iter_open(&it, dirname));
    int found = 0;
    while (Dir::iter_next(&it)) {
        if (strcmp(it.current.name, "file.txt") == 0) {
            found = 1;
            ASSUME_ITS_EQUAL_I32(0, it.current.type);
            ASSUME_ITS_TRUE(it.current.size > 0);
            ASSUME_ITS_TRUE(strlen(it.current.path) > 0);
        }
    }
    Dir::iter_close(&it);
    ASSUME_ITS_TRUE(found);

    fossil_io_dir_entry_t entries[10];
    size_t count = 0;
    ASSUME_ITS_EQUAL_I32(0, Dir::list(dirname, entries, &count, 10));
    ASSUME_ITS_TRUE(count > 0);
    int listed = 0;
    for (size_t i = 0; i < count; ++i) {
        if (strcmp(entries[i].name, "file.txt") == 0) {
            listed = 1;
            ASSUME_ITS_EQUAL_I32(0, entries[i].type);
            ASSUME_ITS_TRUE(entries[i].size > 0);
            ASSUME_ITS_TRUE(strlen(entries[i].path) > 0);
        }
    }
    ASSUME_ITS_TRUE(listed);

    Dir::remove_recursive(dirname);
}

FOSSIL_TEST(cpp_test_dir_path_utilities) {
    char out[256];
    ASSUME_ITS_EQUAL_I32(1, fossil_io_dir_is_absolute("/tmp"));
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

FOSSIL_TEST(cpp_test_dir_is_empty_and_count_and_size) {
    using fossil::io::Dir;
    const std::string dirname = "test_dir_empty";
    Dir::remove_recursive(dirname);
    Dir::create(dirname);
    ASSUME_ITS_EQUAL_I32(1, Dir::is_empty(dirname));
    char fname[128];
    Dir::join(dirname, "file.txt", fname, sizeof(fname));
    FILE *f = fopen(fname, "w");
    fprintf(f, "abc");
    fclose(f);
    ASSUME_ITS_EQUAL_I32(0, Dir::is_empty(dirname));
    size_t count = 0;
    ASSUME_ITS_EQUAL_I32(0, Dir::count(dirname, &count));
    ASSUME_ITS_EQUAL_I32(1, count);
    uint64_t bytes = 0;
    ASSUME_ITS_EQUAL_I32(0, Dir::size(dirname, &bytes));
    ASSUME_ITS_TRUE(bytes > 0);
    Dir::remove_recursive(dirname);
}

FOSSIL_TEST(cpp_test_dir_permissions) {
    using fossil::io::Dir;
    const std::string dirname = "test_dir_perm";
    Dir::remove_recursive(dirname);
    Dir::create(dirname);
    ASSUME_ITS_EQUAL_I32(0, Dir::set_permissions(dirname, 0755));
    int32_t mode = 0;
    ASSUME_ITS_EQUAL_I32(0, Dir::get_permissions(dirname, &mode));
    ASSUME_ITS_TRUE(mode != 0);
    Dir::remove_recursive(dirname);
}

FOSSIL_TEST(cpp_test_dir_temp_and_create_temp) {
    using fossil::io::Dir;
    char tempdir[256];
    ASSUME_ITS_EQUAL_I32(0, Dir::temp(tempdir, sizeof(tempdir)));
    ASSUME_ITS_TRUE(strlen(tempdir) > 0);
    char tempcreated[256];
    ASSUME_ITS_EQUAL_I32(0, Dir::create_temp(tempcreated, sizeof(tempcreated)));
    ASSUME_ITS_TRUE(Dir::exists(tempcreated));
    Dir::remove_recursive(tempcreated);
}

FOSSIL_TEST(cpp_test_dir_metadata) {
    using fossil::io::Dir;
    const std::string dirname = "test_dir_meta";
    Dir::remove_recursive(dirname);
    Dir::create(dirname);
    uint64_t mod = 0, created = 0;
    ASSUME_ITS_EQUAL_I32(0, Dir::get_modified(dirname, &mod));
    ASSUME_ITS_TRUE(mod > 0);
    ASSUME_ITS_EQUAL_I32(0, Dir::get_created(dirname, &created));
    ASSUME_ITS_TRUE(created > 0);
    Dir::remove_recursive(dirname);
}

FOSSIL_TEST(cpp_test_dir_backup_mirror_sync) {
    using fossil::io::Dir;
    const std::string src = "test_dir_syncpp_src";
    const std::string dst = "test_dir_syncpp_dst";
    Dir::remove_recursive(src);
    Dir::remove_recursive(dst);
    Dir::create(src);
    char fname[128];
    Dir::join(src, "file.txt", fname, sizeof(fname));
    FILE *f = fopen(fname, "w");
    fprintf(f, "sync");
    fclose(f);
    ASSUME_ITS_EQUAL_I32(0, Dir::backup(src, "_bak"));
    ASSUME_ITS_EQUAL_I32(0, Dir::mirror(src, dst));
    ASSUME_ITS_EQUAL_I32(0, Dir::sync(src, dst, 1));
    Dir::remove_recursive(src);
    Dir::remove_recursive(dst);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_dir_tests) {
    FOSSIL_TEST_ADD(cpp_dir_suite, cpp_test_dir_create_and_exists);
    FOSSIL_TEST_ADD(cpp_dir_suite, cpp_test_dir_remove_and_remove_recursive);
    FOSSIL_TEST_ADD(cpp_dir_suite, cpp_test_dir_clear);
    FOSSIL_TEST_ADD(cpp_dir_suite, cpp_test_dir_copy_and_copy_recursive);
    FOSSIL_TEST_ADD(cpp_dir_suite, cpp_test_dir_move_and_rename);
    FOSSIL_TEST_ADD(cpp_dir_suite, cpp_test_dir_iter_and_list);
    FOSSIL_TEST_ADD(cpp_dir_suite, cpp_test_dir_path_utilities);
    FOSSIL_TEST_ADD(cpp_dir_suite, cpp_test_dir_is_empty_and_count_and_size);
    FOSSIL_TEST_ADD(cpp_dir_suite, cpp_test_dir_permissions);
    FOSSIL_TEST_ADD(cpp_dir_suite, cpp_test_dir_temp_and_create_temp);
    FOSSIL_TEST_ADD(cpp_dir_suite, cpp_test_dir_metadata);
    FOSSIL_TEST_ADD(cpp_dir_suite, cpp_test_dir_backup_mirror_sync);

    FOSSIL_TEST_REGISTER(cpp_dir_suite);
}
