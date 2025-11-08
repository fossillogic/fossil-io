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
FOSSIL_SETUP(cpp_archive_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_archive_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Archive API Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *

// Test archive initialization and cleanup
FOSSIL_TEST(cpp_test_archive_open_and_close) {
    const char *archive_path = "test_archive.zip";
    
    // Create a new archive using the C++ wrapper
    fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_ITS_TRUE(archive.is_valid());
    
    // Archive will be automatically closed when destructor is called
}

FOSSIL_TEST(cpp_test_archive_create) {
    const char *archive_path = "test_create.tar";
    
    // Create a TAR archive using the C++ wrapper
    fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_TAR, FOSSIL_IO_COMPRESSION_FAST);
    ASSUME_ITS_TRUE(archive.is_valid());
    
    // Verify file exists - note: archive automatically closes on destruction
    ASSUME_ITS_TRUE(fossil_fstream_file_exists(archive_path) != FOSSIL_ERROR_OK);
}

// Test archive inspection functions
FOSSIL_TEST(cpp_test_archive_get_type) {
    const char *zip_path = "test_type.zip";
    
    // Create a ZIP archive
    {
        fossil::io::Archive archive = fossil::io::Archive::create(zip_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
        ASSUME_ITS_TRUE(archive.is_valid());
    } // Archive automatically closed here
    
    // Detect archive type using static method
    fossil::io::Archive temp_archive("", FOSSIL_IO_ARCHIVE_UNKNOWN, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    fossil_io_archive_type_t detected_type = temp_archive.get_type(zip_path);
    ASSUME_ITS_EQUAL_I32(FOSSIL_IO_ARCHIVE_ZIP, detected_type);
}

FOSSIL_TEST(cpp_test_archive_get_stats) {
    const char *archive_path = "test_stats.zip";
    fossil_io_archive_stats_t stats;
    
    // Create archive and add a test file
    const char *test_file = "temp_test_file.txt";
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&temp_stream, test_file, "w"));
    fossil_fstream_write(&temp_stream, "Test content", 12, 1);
    fossil_fstream_close(&temp_stream);
    
    {
        fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
        ASSUME_ITS_TRUE(archive.is_valid());
        
        // Add file to archive
        ASSUME_ITS_TRUE(archive.add_file(test_file, "test.txt"));
    } // Archive automatically closed here
    
    // Reopen and get stats
    fossil::io::Archive archive(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(archive.is_valid());
    
    ASSUME_ITS_TRUE(archive.get_stats(stats));
    ASSUME_ITS_TRUE(stats.total_entries > 0);
    
    fossil_fstream_remove(test_file);
}

FOSSIL_TEST(cpp_test_archive_list_entries) {
    const char *archive_path = "test_list.zip";
    
    // Create test files
    const char *test_file1 = "temp_test1.txt";
    const char *test_file2 = "temp_test2.txt";
    
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&temp_stream, test_file1, "w"));
    fossil_fstream_write(&temp_stream, "Content 1", 9, 1);
    fossil_fstream_close(&temp_stream);
    
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&temp_stream, test_file2, "w"));
    fossil_fstream_write(&temp_stream, "Content 2", 9, 1);
    fossil_fstream_close(&temp_stream);
    
    {
        // Create archive with test files
        fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
        ASSUME_ITS_TRUE(archive.is_valid());
        
        // Add files to archive
        ASSUME_ITS_TRUE(archive.add_file(test_file1, "file1.txt"));
        ASSUME_ITS_TRUE(archive.add_file(test_file2, "file2.txt"));
    } // Archive automatically closed here
    
    // List entries using the C++ wrapper
    fossil::io::Archive archive(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(archive.is_valid());
    
    std::vector<fossil_io_archive_entry_t> entries = archive.list();
    ASSUME_ITS_TRUE(entries.size() >= 2);
    
    // Cleanup
    fossil_fstream_remove(test_file1);
    fossil_fstream_remove(test_file2);
}

// Test extraction functions
FOSSIL_TEST(cpp_test_archive_extract_file) {
    const char *archive_path = "test_extract.zip";
    const char *test_file = "temp_source.txt";
    const char *extracted_file = "temp_extracted.txt";
    const char *test_content = "Extract test content";
    
    // Create source file
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&temp_stream, test_file, "w"));
    fossil_fstream_write(&temp_stream, test_content, strlen(test_content), 1);
    fossil_fstream_close(&temp_stream);
    
    {
        // Create archive and add file
        fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
        ASSUME_ITS_TRUE(archive.is_valid());
        ASSUME_ITS_TRUE(archive.add_file(test_file, "source.txt"));
    } // Archive automatically closed here
    
    // Extract file
    fossil::io::Archive archive(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(archive.is_valid());
    ASSUME_ITS_TRUE(archive.extract_file("source.txt", extracted_file));
    
    // Verify extracted content
    char buffer[1024] = {0};
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&temp_stream, extracted_file, "r"));
    fossil_fstream_read(&temp_stream, buffer, sizeof(buffer), 1);
    fossil_fstream_close(&temp_stream);
    
    ASSUME_ITS_EQUAL_CSTR(test_content, buffer);
    
    // Cleanup
    fossil_fstream_remove(test_file);
    fossil_fstream_remove(extracted_file);
}

FOSSIL_TEST(cpp_test_archive_extract_all) {
    const char *archive_path = "test_extract_all.zip";
    const char *extract_dir = "temp_extract_dir";
    
    // Create test files
    const char *test_file1 = "temp_all1.txt";
    const char *test_file2 = "temp_all2.txt";
    
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&temp_stream, test_file1, "w"));
    fossil_fstream_write(&temp_stream, "All test 1", 10, 1);
    fossil_fstream_close(&temp_stream);
    
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&temp_stream, test_file2, "w"));
    fossil_fstream_write(&temp_stream, "All test 2", 10, 1);
    fossil_fstream_close(&temp_stream);
    
    {
        // Create archive with multiple files
        fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
        ASSUME_ITS_TRUE(archive.is_valid());
        
        ASSUME_ITS_TRUE(archive.add_file(test_file1, "all1.txt"));
        ASSUME_ITS_TRUE(archive.add_file(test_file2, "all2.txt"));
    } // Archive automatically closed here
    
    // Extract all files
    fossil::io::Archive archive(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(archive.is_valid());
    ASSUME_ITS_TRUE(archive.extract_all(extract_dir));
    
    // Cleanup
    fossil_fstream_remove(test_file1);
    fossil_fstream_remove(test_file2);
}

// Test modification functions
FOSSIL_TEST(cpp_test_archive_add_file) {
    const char *archive_path = "test_add.zip";
    const char *test_file = "temp_add_file.txt";
    
    // Create test file
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&temp_stream, test_file, "w"));
    fossil_fstream_write(&temp_stream, "Add file test", 13, 1);
    fossil_fstream_close(&temp_stream);
    
    {
        // Create archive and add file
        fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
        ASSUME_ITS_TRUE(archive.is_valid());
        ASSUME_ITS_TRUE(archive.add_file(test_file, "added.txt"));
    } // Archive automatically closed here
    
    // Verify file was added
    fossil::io::Archive archive(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(archive.is_valid());
    ASSUME_ITS_TRUE(archive.exists("added.txt"));
    
    fossil_fstream_remove(test_file);
}

FOSSIL_TEST(cpp_test_archive_remove_entry) {
    const char *archive_path = "test_remove.zip";
    const char *test_file = "temp_remove_file.txt";
    
    // Create test file
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&temp_stream, test_file, "w"));
    fossil_fstream_write(&temp_stream, "Remove test", 11, 1);
    fossil_fstream_close(&temp_stream);
    
    {
        // Create archive and add file
        fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
        ASSUME_ITS_TRUE(archive.is_valid());
        ASSUME_ITS_TRUE(archive.add_file(test_file, "to_remove.txt"));
        
        // Remove the entry
        ASSUME_ITS_TRUE(archive.remove("to_remove.txt"));
    } // Archive automatically closed here
    
    // Verify file was removed
    fossil::io::Archive archive(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(archive.is_valid());
    ASSUME_ITS_FALSE(archive.exists("to_remove.txt"));
    
    fossil_fstream_remove(test_file);
}

// Test utility functions
FOSSIL_TEST(cpp_test_archive_exists) {
    const char *archive_path = "test_exists.zip";
    const char *test_file = "temp_exists_file.txt";
    
    // Create test file and archive
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&temp_stream, test_file, "w"));
    fossil_fstream_write(&temp_stream, "Exists test", 11, 1);
    fossil_fstream_close(&temp_stream);
    
    {
        fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
        ASSUME_ITS_TRUE(archive.is_valid());
        ASSUME_ITS_TRUE(archive.add_file(test_file, "exists.txt"));
    } // Archive automatically closed here
    
    // Test existence
    fossil::io::Archive archive(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(archive.is_valid());
    ASSUME_ITS_TRUE(archive.exists("exists.txt"));
    ASSUME_ITS_FALSE(archive.exists("nonexistent.txt"));
    
    fossil_fstream_remove(test_file);
}

FOSSIL_TEST(cpp_test_archive_entry_size) {
    const char *archive_path = "test_size.zip";
    const char *test_file = "temp_size_file.txt";
    const char *test_content = "Size test content with known length";
    
    // Create test file
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&temp_stream, test_file, "w"));
    fossil_fstream_write(&temp_stream, test_content, strlen(test_content), 1);
    fossil_fstream_close(&temp_stream);
    
    {
        // Create archive and add file
        fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
        ASSUME_ITS_TRUE(archive.is_valid());
        ASSUME_ITS_TRUE(archive.add_file(test_file, "size_test.txt"));
    } // Archive automatically closed here
    
    // Check entry size
    fossil::io::Archive archive(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(archive.is_valid());
    
    ssize_t size = archive.entry_size("size_test.txt");
    ASSUME_ITS_EQUAL_I32(strlen(test_content), size);
    
    fossil_fstream_remove(test_file);
}

FOSSIL_TEST(cpp_test_archive_print) {
    const char *archive_path = "test_print.zip";
    const char *test_file = "temp_print_file.txt";
    
    // Create test file and archive
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&temp_stream, test_file, "w"));
    fossil_fstream_write(&temp_stream, "Print test", 10, 1);
    fossil_fstream_close(&temp_stream);
    
    {
        fossil::io::Archive archive = fossil::io::Archive::create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
        ASSUME_ITS_TRUE(archive.is_valid());
        ASSUME_ITS_TRUE(archive.add_file(test_file, "print.txt"));
    } // Archive automatically closed here
    
    // Test print function (just ensure it doesn't crash)
    fossil::io::Archive archive(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_ITS_TRUE(archive.is_valid());
    archive.print();
    
    fossil_fstream_remove(test_file);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_archive_tests) {
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_open_and_close);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_create);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_type);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_get_stats);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_list_entries);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_extract_file);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_extract_all);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_add_file);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_remove_entry);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_exists);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_entry_size);
    FOSSIL_TEST_ADD(cpp_archive_suite, cpp_test_archive_print);

    // Register the test suite
    FOSSIL_TEST_REGISTER(cpp_archive_suite);
}
