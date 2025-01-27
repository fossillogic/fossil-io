/*
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * This file is part of the Fossil Logic project, which aims to develop high-
 * performance, cross-platform applications and libraries. The code contained
 * herein is subject to the terms and conditions defined in the project license.
 *
 * Author: Michael Gene Brockus (Dreamer)
 *
 * Copyright (C) 2024 Fossil Logic. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include <fossil/test/framework.h>

#include "fossil/io/framework.h"


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilites
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Define the test suite and add test cases
FOSSIL_TEST_SUITE(cpp_stream_suite);
fossil_fstream_t cpp_stream;

// Setup function for the test suite
FOSSIL_SETUP(cpp_stream_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_stream_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_CASE(cpp_test_stream_let_write_and_read_file) {
    const char *filename = "testfile.txt";
    const char *content = "This is a test.";

    // Write data to the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_write(&cpp_stream, content, strlen(content), 1);
    fossil_fstream_close(&cpp_stream);

    // Read data from the file
    char buffer[1024];
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "r"));
    fossil_fstream_read(&cpp_stream, buffer, sizeof(buffer), 1);
    fossil_fstream_close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_let_open_and_close_file) {
    const char *filename = "testfile.txt";

    // Open the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_multiple_files) {
    const char *filename1 = "testfile1.txt";
    const char *filename2 = "testfile2.txt";

    // Open the first file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename1, "w"));
    fossil_fstream_close(&cpp_stream);

    // Open the second file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename2, "w"));
    fossil_fstream_close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_seek_and_tell) {
    const char *filename = "testfile.txt";
    const char *content = "This is a test.";

    // Write data to the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_write(&cpp_stream, content, strlen(content), 1);
    fossil_fstream_close(&cpp_stream);

    // Open the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "r"));

    // Seek to the end of the file
    fossil_fstream_seek(&cpp_stream, 0, SEEK_END);

    // Get the current position
    long position = fossil_fstream_tell(&cpp_stream);

    ASSUME_ITS_TRUE(position > 0);

    // Close the file
    fossil_fstream_close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_read_buffered) {
    const char *filename = "testfile_buffered.txt";
    const char *content = "Buffered read test content.";
    char buffer[1024];

    // Write data to the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_write(&cpp_stream, content, strlen(content), 1);
    fossil_fstream_close(&cpp_stream);

    // Read data from the file with buffering
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_read_buffered(filename, buffer, strlen(content)));
    ASSUME_ITS_EQUAL_CSTR(content, buffer);
}

FOSSIL_TEST_CASE(cpp_test_stream_write_buffered) {
    const char *filename = "testfile_buffered_write.txt";
    const char *content = "Buffered write test content.";

    // Write data to the file with buffering
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_write_buffered(filename, content, strlen(content)));

    // Read data from the file to verify
    char buffer[1024];
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "r"));
    fossil_fstream_read(&cpp_stream, buffer, sizeof(buffer), 1);
    fossil_fstream_close(&cpp_stream);

    ASSUME_ITS_EQUAL_CSTR(content, buffer);
}

FOSSIL_TEST_CASE(cpp_test_stream_lock_and_unlock) {
    const char *filename = "testfile_lock.txt";

    // Create and lock the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_lock(filename));
    fossil_fstream_close(&cpp_stream);

    // Unlock the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_unlock(filename));
}

FOSSIL_TEST_CASE(cpp_test_stream_get_type) {
    const char *filename = "testfile_type.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Check the file type
    ASSUME_ITS_EQUAL_I32(2, fossil_fstream_get_type(filename));  // Regular file
}

FOSSIL_TEST_CASE(cpp_test_stream_check_permission) {
    const char *filename = "testfile_permission.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Check read and write permissions
    ASSUME_ITS_EQUAL_I32(1, fossil_fstream_check_permission(filename, "r"));
    ASSUME_ITS_EQUAL_I32(1, fossil_fstream_check_permission(filename, "w"));
}

FOSSIL_TEST_CASE(cpp_test_stream_set_and_get_permissions) {
    const char *filename = "testfile_set_permissions.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Set and get permissions
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_set_permissions(filename, 0644));
    ASSUME_ITS_EQUAL_I32(0644, fossil_fstream_get_permissions(filename));
}

FOSSIL_TEST_CASE(cpp_test_stream_restrict_permissions) {
    const char *filename = "testfile_restrict.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Restrict permissions
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_restrict(filename));
    ASSUME_ITS_EQUAL_I32(0600, fossil_fstream_get_permissions(filename));
}

FOSSIL_TEST_CASE(cpp_test_stream_check_ownership) {
    const char *filename = "testfile_ownership.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Check ownership
    ASSUME_ITS_EQUAL_I32(1, fossil_fstream_check_ownership(filename));
}

FOSSIL_TEST_CASE(cpp_test_stream_compare_permissions) {
    const char *filename1 = "testfile_compare1.txt";
    const char *filename2 = "testfile_compare2.txt";

    // Create the files
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename1, "w"));
    fossil_fstream_close(&cpp_stream);
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename2, "w"));
    fossil_fstream_close(&cpp_stream);

    // Set permissions
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_set_permissions(filename1, 0644));
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_set_permissions(filename2, 0644));

    // Compare permissions
    ASSUME_ITS_EQUAL_I32(1, fossil_fstream_compare_permissions(filename1, filename2));
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_file_tests) {
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_let_write_and_read_file);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_let_open_and_close_file);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_multiple_files);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_seek_and_tell);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_read_buffered);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_write_buffered);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_lock_and_unlock);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_get_type);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_check_permission);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_set_and_get_permissions);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_restrict_permissions);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_check_ownership);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_compare_permissions);

    FOSSIL_TEST_REGISTER(cpp_stream_suite);
}
