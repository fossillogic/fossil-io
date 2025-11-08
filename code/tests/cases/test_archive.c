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
FOSSIL_SETUP(c_archive_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_archive_suite) {
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
FOSSIL_TEST(c_test_archive_open_and_close) {
    const char *archive_path = "test_archive.zip";
    
    // Create a new archive
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
    // Close the archive
    fossil_io_archive_close(archive);
    
    // Open existing archive for reading
    archive = fossil_io_archive_open(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    
    fossil_io_archive_close(archive);
}

FOSSIL_TEST(c_test_archive_create) {
    const char *archive_path = "test_create.tar";
    
    // Create a TAR archive
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_TAR, FOSSIL_IO_COMPRESSION_FAST);
    ASSUME_NOT_CNULL(archive);
    
    fossil_io_archive_close(archive);
    
    // Verify file exists
    ASSUME_ITS_TRUE(fossil_fstream_file_exists(archive_path) != FOSSIL_ERROR_OK);
}

// Test archive inspection functions
FOSSIL_TEST(c_test_archive_get_type) {
    const char *zip_path = "test_type.zip";
    
    // Create a ZIP archive
    fossil_io_archive_t *archive = fossil_io_archive_create(zip_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    fossil_io_archive_close(archive);
    
    // Detect archive type
    fossil_io_archive_type_t detected_type = fossil_io_archive_get_type(zip_path);
    ASSUME_ITS_EQUAL_I32(FOSSIL_IO_ARCHIVE_ZIP, detected_type);
}

FOSSIL_TEST(c_test_archive_get_stats) {
    const char *archive_path = "test_stats.zip";
    fossil_io_archive_stats_t stats;
    
    // Create archive and add a test file
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
    // Create a temporary test file
    const char *test_file = "temp_test_file.txt";
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&temp_stream, test_file, "w"));
    fossil_fstream_write(&temp_stream, "Test content", 12, 1);
    fossil_fstream_close(&temp_stream);
    
    // Add file to archive
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file, "test.txt"));
    fossil_io_archive_close(archive);
    
    // Reopen and get stats
    archive = fossil_io_archive_open(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    
    ASSUME_ITS_TRUE(fossil_io_archive_get_stats(archive, &stats));
    ASSUME_ITS_TRUE(stats.total_entries > 0);
    
    fossil_io_archive_close(archive);
    fossil_fstream_remove(test_file);
}

FOSSIL_TEST(c_test_archive_list_entries) {
    const char *archive_path = "test_list.zip";
    fossil_io_archive_entry_t *entries = NULL;
    
    // Create archive with test files
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
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
    
    // Add files to archive
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file1, "file1.txt"));
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file2, "file2.txt"));
    fossil_io_archive_close(archive);
    
    // List entries
    archive = fossil_io_archive_open(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    
    ssize_t count = fossil_io_archive_list(archive, &entries);
    ASSUME_ITS_TRUE(count >= 2);
    
    // Free entries
    fossil_io_archive_free_entries(entries, count);
    fossil_io_archive_close(archive);
    
    // Cleanup
    fossil_fstream_remove(test_file1);
    fossil_fstream_remove(test_file2);
}

// Test extraction functions
FOSSIL_TEST(c_test_archive_extract_file) {
    const char *archive_path = "test_extract.zip";
    const char *test_file = "temp_source.txt";
    const char *extracted_file = "temp_extracted.txt";
    const char *test_content = "Extract test content";
    
    // Create source file
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&temp_stream, test_file, "w"));
    fossil_fstream_write(&temp_stream, test_content, strlen(test_content), 1);
    fossil_fstream_close(&temp_stream);
    
    // Create archive and add file
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file, "source.txt"));
    fossil_io_archive_close(archive);
    
    // Extract file
    archive = fossil_io_archive_open(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_TRUE(fossil_io_archive_extract_file(archive, "source.txt", extracted_file));
    fossil_io_archive_close(archive);
    
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

FOSSIL_TEST(c_test_archive_extract_all) {
    const char *archive_path = "test_extract_all.zip";
    const char *extract_dir = "temp_extract_dir";
    
    // Create archive with multiple files
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    
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
    
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file1, "all1.txt"));
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file2, "all2.txt"));
    fossil_io_archive_close(archive);
    
    // Extract all files
    archive = fossil_io_archive_open(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_TRUE(fossil_io_archive_extract_all(archive, extract_dir));
    fossil_io_archive_close(archive);
    
    // Cleanup
    fossil_fstream_remove(test_file1);
    fossil_fstream_remove(test_file2);
}

// Test modification functions
FOSSIL_TEST(c_test_archive_add_file) {
    const char *archive_path = "test_add.zip";
    const char *test_file = "temp_add_file.txt";
    
    // Create test file
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&temp_stream, test_file, "w"));
    fossil_fstream_write(&temp_stream, "Add file test", 13, 1);
    fossil_fstream_close(&temp_stream);
    
    // Create archive and add file
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file, "added.txt"));
    fossil_io_archive_close(archive);
    
    // Verify file was added
    archive = fossil_io_archive_open(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_TRUE(fossil_io_archive_exists(archive, "added.txt"));
    fossil_io_archive_close(archive);
    
    fossil_fstream_remove(test_file);
}

FOSSIL_TEST(c_test_archive_remove_entry) {
    const char *archive_path = "test_remove.zip";
    const char *test_file = "temp_remove_file.txt";
    
    // Create test file
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&temp_stream, test_file, "w"));
    fossil_fstream_write(&temp_stream, "Remove test", 11, 1);
    fossil_fstream_close(&temp_stream);
    
    // Create archive and add file
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file, "to_remove.txt"));
    
    // Remove the entry
    ASSUME_ITS_TRUE(fossil_io_archive_remove(archive, "to_remove.txt"));
    fossil_io_archive_close(archive);
    
    // Verify file was removed
    archive = fossil_io_archive_open(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_FALSE(fossil_io_archive_exists(archive, "to_remove.txt"));
    fossil_io_archive_close(archive);
    
    fossil_fstream_remove(test_file);
}

// Test utility functions
FOSSIL_TEST(c_test_archive_exists) {
    const char *archive_path = "test_exists.zip";
    const char *test_file = "temp_exists_file.txt";
    
    // Create test file and archive
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&temp_stream, test_file, "w"));
    fossil_fstream_write(&temp_stream, "Exists test", 11, 1);
    fossil_fstream_close(&temp_stream);
    
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file, "exists.txt"));
    fossil_io_archive_close(archive);
    
    // Test existence
    archive = fossil_io_archive_open(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_TRUE(fossil_io_archive_exists(archive, "exists.txt"));
    ASSUME_ITS_FALSE(fossil_io_archive_exists(archive, "nonexistent.txt"));
    fossil_io_archive_close(archive);
    
    fossil_fstream_remove(test_file);
}

FOSSIL_TEST(c_test_archive_entry_size) {
    const char *archive_path = "test_size.zip";
    const char *test_file = "temp_size_file.txt";
    const char *test_content = "Size test content with known length";
    
    // Create test file
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&temp_stream, test_file, "w"));
    fossil_fstream_write(&temp_stream, test_content, strlen(test_content), 1);
    fossil_fstream_close(&temp_stream);
    
    // Create archive and add file
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file, "size_test.txt"));
    fossil_io_archive_close(archive);
    
    // Check entry size
    archive = fossil_io_archive_open(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    
    ssize_t size = fossil_io_archive_entry_size(archive, "size_test.txt");
    ASSUME_ITS_EQUAL_I32(strlen(test_content), size);
    
    fossil_io_archive_close(archive);
    fossil_fstream_remove(test_file);
}

FOSSIL_TEST(c_test_archive_print) {
    const char *archive_path = "test_print.zip";
    const char *test_file = "temp_print_file.txt";
    
    // Create test file and archive
    fossil_fstream_t temp_stream;
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&temp_stream, test_file, "w"));
    fossil_fstream_write(&temp_stream, "Print test", 10, 1);
    fossil_fstream_close(&temp_stream);
    
    fossil_io_archive_t *archive = fossil_io_archive_create(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_COMPRESSION_NORMAL);
    ASSUME_NOT_CNULL(archive);
    ASSUME_ITS_TRUE(fossil_io_archive_add_file(archive, test_file, "print.txt"));
    fossil_io_archive_close(archive);
    
    // Test print function (just ensure it doesn't crash)
    archive = fossil_io_archive_open(archive_path, FOSSIL_IO_ARCHIVE_ZIP, FOSSIL_IO_ARCHIVE_READ, FOSSIL_IO_COMPRESSION_NONE);
    ASSUME_NOT_CNULL(archive);
    fossil_io_archive_print(archive);
    fossil_io_archive_close(archive);
    
    fossil_fstream_remove(test_file);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_archive_tests) {
    // Add all the archive test cases to the test suite
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_open_and_close);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_create);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_get_type);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_get_stats);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_list_entries);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_extract_file);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_extract_all);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_add_file);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_remove_entry);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_exists);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_entry_size);
    FOSSIL_TEST_ADD(c_archive_suite, c_test_archive_print);

    // Register the test suite
    FOSSIL_TEST_REGISTER(c_archive_suite);
}
