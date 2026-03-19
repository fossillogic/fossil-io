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
FOSSIL_SUITE(cpp_archive_suite);

// Setup function for the test suite
FOSSIL_SETUP(cpp_archive_suite)
{
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_archive_suite)
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

FOSSIL_TEST(cpp_test_archive_create_and_close)
{
    const char *archive_path = "test_archive.zip";
    fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
}

FOSSIL_TEST(cpp_test_archive_get_type_zip)
{
    const char *archive_path = "test_archive.zip";
    fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
    
    fossil_io_archive_type_t type = fossil_io_archive_get_type(archive_path);
    ASSUME_ITS_EQUAL_I32(type, FOSSIL_IO_ARCHIVE_ZIP);
}

FOSSIL_TEST(cpp_test_archive_add_file_and_list)
{
    const char *archive_path = "test_add_file.zip";
    fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
    
    bool added = archive.add_file("test_file.txt", "internal/test_file.txt");
    ASSUME_ITS_TRUE(added);
    
    auto entries = archive.list();
    ASSUME_ITS_MORE_THAN_SIZE(entries.size(), 0);
}

FOSSIL_TEST(cpp_test_archive_exists_entry)
{
    const char *archive_path = "test_exists.zip";
    fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
    
    bool added = archive.add_file("test_file.txt", "myfile.txt");
    ASSUME_ITS_TRUE(added);
    
    bool exists = archive.exists("myfile.txt");
    ASSUME_ITS_TRUE(exists);
    
    bool not_exists = archive.exists("nonexistent.txt");
    ASSUME_ITS_FALSE(not_exists);
}

FOSSIL_TEST(cpp_test_archive_entry_size)
{
    const char *archive_path = "test_size.zip";
    fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
    
    bool added = archive.add_file("test_file.txt", "sized_file.txt");
    ASSUME_ITS_TRUE(added);
    
    ssize_t size = archive.entry_size("sized_file.txt");
    ASSUME_ITS_MORE_OR_EQUAL_I64(size, 0);
}

FOSSIL_TEST(cpp_test_archive_extract_file)
{
    const char *archive_path = "test_extract_file.zip";
    {
        fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
        ASSUME_ITS_TRUE(archive.is_valid());
        
        bool added = archive.add_file("test_file.txt", "extract_me.txt");
        ASSUME_ITS_TRUE(added);
    }
    
    fossil::io::Archive archive(archive_path, FOSSIL_IO_ARCHIVE_UNKNOWN, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(archive.is_valid());
    
    bool extracted = archive.extract_file("extract_me.txt", "extracted_file.txt");
    ASSUME_ITS_TRUE(extracted);
}

FOSSIL_TEST(cpp_test_archive_extract_all)
{
    const char *archive_path = "test_extract_all.zip";
    {
        fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
        ASSUME_ITS_TRUE(archive.is_valid());
        
        bool added = archive.add_file("test_file.txt", "file1.txt");
        ASSUME_ITS_TRUE(added);
    }
    
    fossil::io::Archive archive(archive_path, FOSSIL_IO_ARCHIVE_UNKNOWN, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(archive.is_valid());
    
    bool extracted = archive.extract_all("extracted_dir");
    ASSUME_ITS_TRUE(extracted);
}

FOSSIL_TEST(cpp_test_archive_get_stats)
{
    const char *archive_path = "test_stats.zip";
    fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
    
    bool added = archive.add_file("test_file.txt", "stat_file.txt");
    ASSUME_ITS_TRUE(added);
    
    fossil_io_archive_stats_t stats;
    bool got_stats = archive.get_stats(stats);
    ASSUME_ITS_TRUE(got_stats);
    ASSUME_ITS_MORE_THAN_U32(stats.total_entries, 0);
}

FOSSIL_TEST(cpp_test_archive_remove_entry)
{
    const char *archive_path = "test_remove.zip";
    fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
    
    bool added = archive.add_file("test_file.txt", "to_remove.txt");
    ASSUME_ITS_TRUE(added);
    
    bool removed = archive.remove("to_remove.txt");
    ASSUME_ITS_TRUE(removed);
    
    bool exists = archive.exists("to_remove.txt");
    ASSUME_ITS_FALSE(exists);
}

FOSSIL_TEST(cpp_test_archive_add_directory)
{
    const char *archive_path = "test_add_dir.zip";
    fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
    
    bool added = archive.add_directory("test_directory", "archive_dir");
    ASSUME_ITS_TRUE(added);
}

FOSSIL_TEST(cpp_test_archive_compression_levels)
{
    const char *archive_path = "test_compression.zip";
    fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_MAXIMUM);
    ASSUME_ITS_TRUE(archive.is_valid());
    
    bool added = archive.add_file("test_file.txt", "compressed.txt");
    ASSUME_ITS_TRUE(added);
}

FOSSIL_TEST(cpp_test_archive_print)
{
    const char *archive_path = "test_print.zip";
    fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
    
    bool added = archive.add_file("test_file.txt", "print_file.txt");
    ASSUME_ITS_TRUE(added);
    
    archive.print();
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_archive_tests)
{
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_create_and_close);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_type_zip);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_add_file_and_list);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_exists_entry);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_entry_size);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_extract_file);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_extract_all);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_stats);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_remove_entry);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_add_directory);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_compression_levels);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_print);

    FOSSIL_TEST_REGISTER(cpp_archive_suite);
}
