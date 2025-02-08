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

FOSSIL_TEST_CASE(cpp_test_stream_get_type) {
    const char *filename = "testfile_type.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Check the file type
    ASSUME_ITS_EQUAL_I32(2, fossil_fstream_get_type(filename));  // Regular file
}

FOSSIL_TEST_CASE(cpp_test_stream_is_readable) {
    const char *filename = "testfile_readable.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Check if the file is readable
    ASSUME_ITS_EQUAL_I32(1, fossil_fstream_is_readable(filename));
}

FOSSIL_TEST_CASE(cpp_test_stream_is_writable) {
    const char *filename = "testfile_writable.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Check if the file is writable
    ASSUME_ITS_EQUAL_I32(1, fossil_fstream_is_writable(filename));
}

FOSSIL_TEST_CASE(cpp_test_stream_is_executable) {
    const char *filename = "testfile_executable.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Check if the file is executable
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_is_executable(filename));
}

FOSSIL_TEST_CASE(cpp_test_stream_set_permissions) {
    const char *filename = "testfile_permissions.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Set file permissions
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_set_permissions(filename, 0644));

    // Check if the file is readable and writable
    ASSUME_ITS_EQUAL_I32(1, fossil_fstream_is_readable(filename));
    ASSUME_ITS_EQUAL_I32(1, fossil_fstream_is_writable(filename));
}

FOSSIL_TEST_CASE(cpp_test_stream_get_permissions) {
    const char *filename = "testfile_get_permissions.txt";
    int32_t mode;

    // // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_close(&cpp_stream);

    // Set file permissions
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_set_permissions(filename, 0644));

    // Get file permissions
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_get_permissions(filename, &mode));
}

FOSSIL_TEST_CASE(cpp_test_stream_remove_file) {
    const char *filename = "testfile_remove.txt";
    const char *content = "This is a test.";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_write(&cpp_stream, content, strlen(content), 1);
    fossil_fstream_close(&cpp_stream);

    // Remove the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_remove(filename));

    // Check if the file does not exist
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_file_exists(filename));
}

FOSSIL_TEST_CASE(cpp_test_stream_flush_file) {
    const char *filename = "testfile_flush.txt";
    const char *content = "This is a test.";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_write(&cpp_stream, content, strlen(content), 1);

    // Flush the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_flush(&cpp_stream));
    fossil_fstream_close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_setpos_and_getpos) {
    const char *filename = "testfile_setpos_getpos.txt";
    const char *content = "This is a test.";
    int32_t pos;

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "w"));
    fossil_fstream_write(&cpp_stream, content, strlen(content), 1);
    fossil_fstream_close(&cpp_stream);

    // Open the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&cpp_stream, filename, "r"));

    // Set the file position
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_setpos(&cpp_stream, 5));

    // Get the file position
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_getpos(&cpp_stream, &pos));
    ASSUME_ITS_EQUAL_I32(5, pos);

    // Close the file
    fossil_fstream_close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_class_write_and_read_file) {
    const char *filename = "testfile.txt";
    const char *content = "This is a test.";

    // Write data to the file
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&cpp_stream, filename, "w"));
    fossil::io::Stream::write(&cpp_stream, content, strlen(content), 1);
    fossil::io::Stream::close(&cpp_stream);

    // Read data from the file
    char buffer[1024];
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&cpp_stream, filename, "r"));
    fossil::io::Stream::read(&cpp_stream, buffer, sizeof(buffer), 1);
    fossil::io::Stream::close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_class_open_and_close_file) {
    const char *filename = "testfile.txt";

    // Open the file
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&cpp_stream, filename, "w"));
    fossil::io::Stream::close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_class_multiple_files) {
    const char *filename1 = "testfile1.txt";
    const char *filename2 = "testfile2.txt";

    // Open the first file
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&cpp_stream, filename1, "w"));
    fossil::io::Stream::close(&cpp_stream);

    // Open the second file
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&cpp_stream, filename2, "w"));
    fossil::io::Stream::close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_class_seek_and_tell) {
    const char *filename = "testfile.txt";
    const char *content = "This is a test.";

    // Write data to the file
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&cpp_stream, filename, "w"));
    fossil::io::Stream::write(&cpp_stream, content, strlen(content), 1);
    fossil::io::Stream::close(&cpp_stream);

    // Open the file
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&cpp_stream, filename, "r"));

    // Seek to the end of the file
    fossil::io::Stream::seek(&cpp_stream, 0, SEEK_END);

    // Get the current position
    long position = fossil::io::Stream::tell(&cpp_stream);

    ASSUME_ITS_TRUE(position > 0);

    // Close the file
    fossil::io::Stream::close(&cpp_stream);
}

FOSSIL_TEST_CASE(cpp_test_stream_class_get_type) {
    const char *filename = "testfile_type.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&cpp_stream, filename, "w"));
    fossil::io::Stream::close(&cpp_stream);

    // Check the file type
    ASSUME_ITS_EQUAL_I32(2, fossil::io::Stream::get_type(filename));  // Regular file
}

FOSSIL_TEST_CASE(cpp_test_stream_class_is_readable) {
    const char *filename = "testfile_readable_class.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&cpp_stream, filename, "w"));
    fossil::io::Stream::close(&cpp_stream);

    // Check if the file is readable
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Stream::is_readable(filename));
}

FOSSIL_TEST_CASE(cpp_test_stream_class_is_writable) {
    const char *filename = "testfile_writable_class.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&cpp_stream, filename, "w"));
    fossil::io::Stream::close(&cpp_stream);

    // Check if the file is writable
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Stream::is_writable(filename));
}

FOSSIL_TEST_CASE(cpp_test_stream_class_is_executable) {
    const char *filename = "testfile_executable_class.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&cpp_stream, filename, "w"));
    fossil::io::Stream::close(&cpp_stream);

    // Check if the file is executable
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::is_executable(filename));
}

FOSSIL_TEST_CASE(cpp_test_stream_class_set_permissions) {
    const char *filename = "testfile_permissions_class.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&cpp_stream, filename, "w"));
    fossil::io::Stream::close(&cpp_stream);

    // Set file permissions
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::set_permissions(filename, 0644));

    // Check if the file is readable and writable
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Stream::is_readable(filename));
    ASSUME_ITS_EQUAL_I32(1, fossil::io::Stream::is_writable(filename));
}

FOSSIL_TEST_CASE(cpp_test_stream_class_get_permissions) {
    const char *filename = "testfile_get_permissions_class.txt";
    int32_t mode;

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::open(&cpp_stream, filename, "w"));
    fossil::io::Stream::close(&cpp_stream);

    // Set file permissions
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::set_permissions(filename, 0644));

    // Get file permissions
    ASSUME_ITS_EQUAL_I32(0, fossil::io::Stream::get_permissions(filename, &mode));
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_file_tests) {
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_let_write_and_read_file);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_let_open_and_close_file);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_multiple_files);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_seek_and_tell);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_get_type);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_is_readable);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_is_writable);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_is_executable);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_set_permissions);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_get_permissions);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_remove_file);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_flush_file);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_setpos_and_getpos);

    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_class_write_and_read_file);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_class_open_and_close_file);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_class_multiple_files);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_class_seek_and_tell);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_class_get_type);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_class_is_readable);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_class_is_writable);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_class_is_executable);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_class_set_permissions);
    FOSSIL_TEST_ADD(cpp_stream_suite, cpp_test_stream_class_get_permissions);

    FOSSIL_TEST_REGISTER(cpp_stream_suite);
}
