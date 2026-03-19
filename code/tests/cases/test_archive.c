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

FOSSIL_TEST(c_test_archive_create_and_close)
{
    const char *archive_path = "test_archive.zip";
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    fossil_io_archive_close(archive);
}

FOSSIL_TEST(c_test_archive_get_type_zip)
{
    const char *archive_path = "test_archive.zip";
    fossil_io_archive_type_t type = fossil_io_archive_get_type(archive_path);
    ASSUME_ITS_EQUAL_I32(type, FOSSIL_IO_ARCHIVE_ZIP);
}

FOSSIL_TEST(c_test_archive_add_file_and_list)
{
    const char *archive_path = "test_add_file.zip";
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
    bool added = fossil_io_archive_add_file(archive, "test_file.txt", "internal/test_file.txt");
    ASSUME_ITS_TRUE(added);
    
    fossil_io_archive_entry_t *entries = NULL;
    ssize_t count = fossil_io_archive_list(archive, &entries);
    ASSUME_ITS_MORE_THAN_I64(count, 0);
    
    fossil_io_archive_free_entries(entries, count);
    fossil_io_archive_close(archive);
}

FOSSIL_TEST(c_test_archive_exists_entry)
{
    const char *archive_path = "test_exists.zip";
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
    bool added = fossil_io_archive_add_file(archive, "test_file.txt", "myfile.txt");
    ASSUME_ITS_TRUE(added);
    
    bool exists = fossil_io_archive_exists(archive, "myfile.txt");
    ASSUME_ITS_TRUE(exists);
    
    bool not_exists = fossil_io_archive_exists(archive, "nonexistent.txt");
    ASSUME_ITS_FALSE(not_exists);
    
    fossil_io_archive_close(archive);
}

FOSSIL_TEST(c_test_archive_entry_size)
{
    const char *archive_path = "test_size.zip";
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
    bool added = fossil_io_archive_add_file(archive, "test_file.txt", "sized_file.txt");
    ASSUME_ITS_TRUE(added);
    
    ssize_t size = fossil_io_archive_entry_size(archive, "sized_file.txt");
    ASSUME_ITS_MORE_OR_EQUAL_I64(size, 0);
    
    fossil_io_archive_close(archive);
}

FOSSIL_TEST(c_test_archive_extract_file)
{
    const char *archive_path = "test_extract_file.zip";
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
    bool added = fossil_io_archive_add_file(archive, "test_file.txt", "extract_me.txt");
    ASSUME_ITS_TRUE(added);
    fossil_io_archive_close(archive);
    
    archive = fossil_io_archive_open(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    
    bool extracted = fossil_io_archive_extract_file(archive, "extract_me.txt", "extracted_file.txt");
    ASSUME_ITS_TRUE(extracted);
    
    fossil_io_archive_close(archive);
}

FOSSIL_TEST(c_test_archive_extract_all)
{
    const char *archive_path = "test_extract_all.zip";
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
    bool added = fossil_io_archive_add_file(archive, "test_file.txt", "file1.txt");
    ASSUME_ITS_TRUE(added);
    fossil_io_archive_close(archive);
    
    archive = fossil_io_archive_open(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    
    bool extracted = fossil_io_archive_extract_all(archive, "extracted_dir");
    ASSUME_ITS_TRUE(extracted);
    
    fossil_io_archive_close(archive);
}

FOSSIL_TEST(c_test_archive_get_stats)
{
    const char *archive_path = "test_stats.zip";
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
    bool added = fossil_io_archive_add_file(archive, "test_file.txt", "stat_file.txt");
    ASSUME_ITS_TRUE(added);
    
    fossil_io_archive_stats_t stats;
    bool got_stats = fossil_io_archive_get_stats(archive, &stats);
    ASSUME_ITS_TRUE(got_stats);
    ASSUME_ITS_MORE_THAN_I64(stats.total_entries, 0);
    
    fossil_io_archive_close(archive);
}

FOSSIL_TEST(c_test_archive_remove_entry)
{
    const char *archive_path = "test_remove.zip";
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
    bool added = fossil_io_archive_add_file(archive, "test_file.txt", "to_remove.txt");
    ASSUME_ITS_TRUE(added);
    
    bool removed = fossil_io_archive_remove(archive, "to_remove.txt");
    ASSUME_ITS_TRUE(removed);
    
    bool exists = fossil_io_archive_exists(archive, "to_remove.txt");
    ASSUME_ITS_FALSE(exists);
    
    fossil_io_archive_close(archive);
}

FOSSIL_TEST(c_test_archive_add_directory)
{
    const char *archive_path = "test_add_dir.zip";
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
    bool added = fossil_io_archive_add_directory(archive, "test_directory", "archive_dir");
    ASSUME_ITS_TRUE(added);
    
    fossil_io_archive_close(archive);
}

FOSSIL_TEST(c_test_archive_compression_levels)
{
    const char *archive_path = "test_compression.zip";
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_MAXIMUM);
    ASSUME_NOT_CNULL(archive);
    
    bool added = fossil_io_archive_add_file(archive, "test_file.txt", "compressed.txt");
    ASSUME_ITS_TRUE(added);
    
    fossil_io_archive_close(archive);
}

FOSSIL_TEST(c_test_archive_print)
{
    const char *archive_path = "test_print.zip";
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
    bool added = fossil_io_archive_add_file(archive, "test_file.txt", "print_file.txt");
    ASSUME_ITS_TRUE(added);
    
    fossil_io_archive_print(archive);
    
    fossil_io_archive_close(archive);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_archive_tests)
{
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_create_and_close);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_get_type_zip);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_add_file_and_list);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_exists_entry);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_entry_size);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_extract_file);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_extract_all);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_get_stats);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_remove_entry);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_add_directory);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_compression_levels);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_print);

    FOSSIL_TEST_REGISTER(c_archive_suite);
}
