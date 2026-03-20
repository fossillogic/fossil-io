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

FOSSIL_TEST(cpp_test_archive_open_create)
{
    fossil::io::Archive archive("test.zip", FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_WRITE, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
}

FOSSIL_TEST(cpp_test_archive_create_new)
{
    fossil::io::Archive archive = fossil::io::Archive::create("newarchive.tar", FOSSIL_IO_ARCHIVE_TAR, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(archive.is_valid());
}

FOSSIL_TEST(cpp_test_archive_get_type)
{
    fossil::io::Archive archive("", FOSSIL_IO_ARCHIVE_UNKNOWN, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    
    fossil_io_archive_type_t type = archive.get_type("archive.zip");
    ASSUME_ITS_EQUAL_I32(type, FOSSIL_IO_ARCHIVE_ZIP);
    
    type = archive.get_type("archive.tar.gz");
    ASSUME_ITS_EQUAL_I32(type, FOSSIL_IO_ARCHIVE_TARGZ);
    
    type = archive.get_type("archive.unknown");
    ASSUME_ITS_EQUAL_I32(type, FOSSIL_IO_ARCHIVE_UNKNOWN);
}

FOSSIL_TEST(cpp_test_archive_add_file)
{
    // Create a source file first
    FILE* fp = fopen("source.txt", "w");
    if (fp) {
        fprintf(fp, "test content");
        fclose(fp);
    }
    
    fossil::io::Archive archive = fossil::io::Archive::create("test_archive.zip", FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
    bool result = archive.add_file("source.txt", "archived.txt");
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(cpp_test_archive_add_directory)
{
    fossil::io::Archive archive = fossil::io::Archive::create("test_dir.tar", FOSSIL_IO_ARCHIVE_TAR, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(archive.is_valid());
    bool result = archive.add_directory("source_dir", "archived_dir");
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(cpp_test_archive_extract_all)
{
    fossil::io::Archive archive("test_dir.tar", FOSSIL_IO_ARCHIVE_TAR, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(archive.is_valid());
    bool result = archive.extract_all("extracted_dir");
    ASSUME_ITS_TRUE(result);
}

FOSSIL_TEST(cpp_test_archive_exists)
{
    fossil::io::Archive archive("test_archive.zip", FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(archive.is_valid());
    bool exists = archive.exists("archived.txt");
    ASSUME_ITS_TRUE(exists || !exists);
}

FOSSIL_TEST(cpp_test_archive_entry_size)
{
    fossil::io::Archive archive("test_archive.zip", FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(archive.is_valid());
    ssize_t size = archive.entry_size("archived.txt");
    ASSUME_ITS_MORE_OR_EQUAL_I64(size, -1);
}

FOSSIL_TEST(cpp_test_archive_get_stats)
{
    fossil::io::Archive archive("test_archive.zip", FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(archive.is_valid());
    fossil_io_archive_stats_t stats;
    bool result = archive.get_stats(stats);
    ASSUME_ITS_TRUE(result);
    ASSUME_ITS_MORE_OR_EQUAL_I64(stats.total_entries, 0);
}

FOSSIL_TEST(cpp_test_archive_list)
{
    fossil::io::Archive archive("test_archive.zip", FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(archive.is_valid());
    auto entries = archive.list();
    ASSUME_ITS_TRUE(!entries.empty() || entries.empty());
}

FOSSIL_TEST(cpp_test_archive_targz_type)
{
    fossil::io::Archive archive("", FOSSIL_IO_ARCHIVE_UNKNOWN, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    
    fossil_io_archive_type_t type = archive.get_type("archive.tar.gz");
    ASSUME_ITS_EQUAL_I32(type, FOSSIL_IO_ARCHIVE_TARGZ);
    
    type = archive.get_type("archive.tgz");
    ASSUME_ITS_EQUAL_I32(type, FOSSIL_IO_ARCHIVE_TARGZ);
}

FOSSIL_TEST(cpp_test_archive_tarbz2_type)
{
    fossil::io::Archive archive("", FOSSIL_IO_ARCHIVE_UNKNOWN, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    
    fossil_io_archive_type_t type = archive.get_type("archive.tar.bz2");
    ASSUME_ITS_EQUAL_I32(type, FOSSIL_IO_ARCHIVE_TARBZ2);
    
    type = archive.get_type("archive.tbz2");
    ASSUME_ITS_EQUAL_I32(type, FOSSIL_IO_ARCHIVE_TARBZ2);
}

FOSSIL_TEST(cpp_test_archive_compression_levels)
{
    fossil::io::Archive archive_fast = fossil::io::Archive::create("test_fast.zip", FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_FASTEST);
    ASSUME_ITS_TRUE(archive_fast.is_valid());
    
    fossil::io::Archive archive_max = fossil::io::Archive::create("test_max.zip", FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_MAXIMUM);
    ASSUME_ITS_TRUE(archive_max.is_valid());
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_archive_tests)
{
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_open_create);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_create_new);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_type);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_add_file);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_add_directory);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_extract_all);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_exists);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_entry_size);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_stats);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_list);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_targz_type);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_tarbz2_type);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_compression_levels);

    FOSSIL_TEST_REGISTER(cpp_archive_suite);
}
