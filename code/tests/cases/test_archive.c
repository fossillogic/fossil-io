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
FOSSIL_SUITE(c_archive_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_archive_suite)
{
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_archive_suite)
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

// Archive API tests

FOSSIL_TEST(c_test_archive_open_create)
{
    fossil_io_archive_t *archive = fossil_io_archive_open("test.zip", FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_WRITE, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    fossil_io_archive_close(archive);
}

FOSSIL_TEST(c_test_archive_create_new)
{
    fossil_io_archive_t *archive = fossil_io_archive_create("newarchive.tar", FOSSIL_IO_ARCHIVE_TAR, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    fossil_io_archive_close(archive);
}

FOSSIL_TEST(c_test_archive_get_type)
{
    fossil_io_archive_type_t type = fossil_io_archive_get_type("archive.zip");
    ASSUME_ITS_EQUAL_I32(type, FOSSIL_IO_ARCHIVE_ZIP);
    
    type = fossil_io_archive_get_type("archive.tar.gz");
    ASSUME_ITS_EQUAL_I32(type, FOSSIL_IO_ARCHIVE_TARGZ);
    
    type = fossil_io_archive_get_type("archive.unknown");
    ASSUME_ITS_EQUAL_I32(type, FOSSIL_IO_ARCHIVE_UNKNOWN);
}

FOSSIL_TEST(c_test_archive_add_file)
{
    FILE *file = fopen("test_file.txt", "w");
    ASSUME_NOT_CNULL(file);
    fwrite("Test content", 1, 12, file);
    fclose(file);
    
    fossil_io_archive_t *archive = fossil_io_archive_create("test_archive.zip", FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    bool result = fossil_io_archive_add_file(archive, "test_file.txt", "archived.txt");
    ASSUME_ITS_TRUE(result);
    fossil_io_archive_close(archive);
}

FOSSIL_TEST(c_test_archive_add_directory)
{
    fossil_io_archive_t *archive = fossil_io_archive_create("test_dir.tar", FOSSIL_IO_ARCHIVE_TAR, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    bool result = fossil_io_archive_add_directory(archive, "source_dir", "archived_dir");
    ASSUME_ITS_TRUE(result);
    fossil_io_archive_close(archive);
}

FOSSIL_TEST(c_test_archive_extract_all)
{
    fossil_io_archive_t *archive = fossil_io_archive_open("test_dir.tar", FOSSIL_IO_ARCHIVE_TAR, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    bool result = fossil_io_archive_extract_all(archive, "extracted_dir");
    ASSUME_ITS_TRUE(result);
    fossil_io_archive_close(archive);
}

FOSSIL_TEST(c_test_archive_entry_size)
{
    fossil_io_archive_t *archive = fossil_io_archive_open("test_archive.zip", FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    ssize_t size = fossil_io_archive_entry_size(archive, "archived.txt");
    ASSUME_ITS_MORE_OR_EQUAL_I64(size, -1);
    fossil_io_archive_close(archive);
}

FOSSIL_TEST(c_test_archive_get_stats)
{
    fossil_io_archive_t *archive = fossil_io_archive_open("test_archive.zip", FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    fossil_io_archive_stats_t stats;
    bool result = fossil_io_archive_get_stats(archive, &stats);
    ASSUME_ITS_TRUE(result);
    ASSUME_ITS_MORE_OR_EQUAL_I64(stats.total_entries, 0);
    fossil_io_archive_close(archive);
}

FOSSIL_TEST(c_test_archive_list)
{
    fossil_io_archive_t *archive = fossil_io_archive_open("test_archive.zip", FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    fossil_io_archive_entry_t *entries = NULL;
    ssize_t count = fossil_io_archive_list(archive, &entries);
    ASSUME_ITS_MORE_OR_EQUAL_I64(count, 0);
    if (count > 0) {
        fossil_io_archive_free_entries(entries, (size_t)count);
    }
    fossil_io_archive_close(archive);
}

FOSSIL_TEST(c_test_archive_invalid_path)
{
    fossil_io_archive_t *archive = fossil_io_archive_open(NULL, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_CNULL(archive);
}

FOSSIL_TEST(c_test_archive_targz_type)
{
    fossil_io_archive_type_t type = fossil_io_archive_get_type("archive.tar.gz");
    ASSUME_ITS_EQUAL_I32(type, FOSSIL_IO_ARCHIVE_TARGZ);
    
    type = fossil_io_archive_get_type("archive.tgz");
    ASSUME_ITS_EQUAL_I32(type, FOSSIL_IO_ARCHIVE_TARGZ);
}

FOSSIL_TEST(c_test_archive_tarbz2_type)
{
    fossil_io_archive_type_t type = fossil_io_archive_get_type("archive.tar.bz2");
    ASSUME_ITS_EQUAL_I32(type, FOSSIL_IO_ARCHIVE_TARBZ2);
    
    type = fossil_io_archive_get_type("archive.tbz2");
    ASSUME_ITS_EQUAL_I32(type, FOSSIL_IO_ARCHIVE_TARBZ2);
}

FOSSIL_TEST(c_test_archive_compression_levels)
{
    fossil_io_archive_t *archive = fossil_io_archive_create("test_fast.zip", FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_FASTEST);
    ASSUME_NOT_CNULL(archive);
    fossil_io_archive_close(archive);
    
    archive = fossil_io_archive_create("test_max.zip", FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_MAXIMUM);
    ASSUME_NOT_CNULL(archive);
    fossil_io_archive_close(archive);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_archive_tests)
{
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_open_create);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_create_new);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_get_type);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_add_file);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_add_directory);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_extract_all);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_entry_size);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_get_stats);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_list);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_invalid_path);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_targz_type);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_tarbz2_type);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_compression_levels);

    FOSSIL_TEST_REGISTER(c_archive_suite);
}
