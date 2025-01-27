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
FOSSIL_TEST_SUITE(c_stream_suite);
fossil_fstream_t c_stream;

// Setup function for the test suite
FOSSIL_SETUP(c_stream_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_stream_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_CASE(c_test_stream_let_write_and_read_file) {
    const char *filename = "testfile.txt";
    const char *content = "This is a test.";

    // Write data to the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&c_stream, filename, "w"));
    fossil_fstream_write(&c_stream, content, strlen(content), 1);
    fossil_fstream_close(&c_stream);

    // Read data from the file
    char buffer[1024];
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&c_stream, filename, "r"));
    fossil_fstream_read(&c_stream, buffer, sizeof(buffer), 1);
    fossil_fstream_close(&c_stream);
}

FOSSIL_TEST_CASE(c_test_stream_let_open_and_close_file) {
    const char *filename = "testfile.txt";

    // Open the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&c_stream, filename, "w"));
    fossil_fstream_close(&c_stream);
}

FOSSIL_TEST_CASE(c_test_stream_multiple_files) {
    const char *filename1 = "testfile1.txt";
    const char *filename2 = "testfile2.txt";

    // Open the first file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&c_stream, filename1, "w"));
    fossil_fstream_close(&c_stream);

    // Open the second file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&c_stream, filename2, "w"));
    fossil_fstream_close(&c_stream);
}

FOSSIL_TEST_CASE(c_test_stream_seek_and_tell) {
    const char *filename = "testfile.txt";
    const char *content = "This is a test.";

    // Write data to the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&c_stream, filename, "w"));
    fossil_fstream_write(&c_stream, content, strlen(content), 1);
    fossil_fstream_close(&c_stream);

    // Open the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&c_stream, filename, "r"));

    // Seek to the end of the file
    fossil_fstream_seek(&c_stream, 0, SEEK_END);

    // Get the current position
    long position = fossil_fstream_tell(&c_stream);

    ASSUME_ITS_TRUE(position > 0);

    // Close the file
    fossil_fstream_close(&c_stream);
}

FOSSIL_TEST_CASE(c_test_stream_get_type) {
    const char *filename = "testfile_type.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&c_stream, filename, "w"));
    fossil_fstream_close(&c_stream);

    // Check the file type
    ASSUME_ITS_EQUAL_I32(2, fossil_fstream_get_type(filename));  // Regular file
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_file_tests) {
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_let_write_and_read_file);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_let_open_and_close_file);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_multiple_files);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_seek_and_tell);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_get_type);

    FOSSIL_TEST_REGISTER(c_stream_suite);
}
