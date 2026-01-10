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
FOSSIL_SUITE(c_stream_suite);
fossil_io_file_t c_stream;

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

FOSSIL_TEST(c_test_stream_tempfile_creation) {
    // Create a temporary file
    fossil_io_file_t temp_stream = fossil_io_file_tempfile();

    // Check if the temporary file is open
    ASSUME_ITS_TRUE(fossil_io_file_is_open(&temp_stream));

    // Close the temporary file
    fossil_io_file_close(&temp_stream);
}

FOSSIL_TEST(c_test_stream_tempfile_cleanup) {
    // Create a temporary file
    fossil_io_file_t temp_stream = fossil_io_file_tempfile();

    // Get the temporary file name
    const char *temp_filename = temp_stream.filename;

    // Close the temporary file
    fossil_io_file_close(&temp_stream);

    // Verify the temporary file is deleted
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_file_exists(temp_filename));
}

FOSSIL_TEST(c_test_stream_let_write_and_read_file) {
    const char *filename = "testfile.txt";
    const char *content = "This is a test.";

    // Write data to the file
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "w"));
    fossil_io_file_write(&c_stream, content, strlen(content), 1);
    fossil_io_file_close(&c_stream);

    // Read data from the file
    char buffer[1024];
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "r"));
    fossil_io_file_read(&c_stream, buffer, sizeof(buffer), 1);
    fossil_io_file_close(&c_stream);
}

FOSSIL_TEST(c_test_stream_redirect_to_devnull) {
    const char *filename = "testfile_redirect.txt";
    const char *content = "This is a test.";

    // Create the file and write data to it
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "w"));
    fossil_io_file_write(&c_stream, content, strlen(content), 1);

    // Redirect the stream to /dev/null
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_redirect_to_devnull(&c_stream));

    // Attempt to write more data (should not affect the file)
    const char *extra_content = "This should not be written.";
    fossil_io_file_write(&c_stream, extra_content, strlen(extra_content), 1);

    // Close the stream
    fossil_io_file_close(&c_stream);

    // Reopen the file and verify its content remains unchanged
    char buffer[1024] = {0};
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "r"));
    fossil_io_file_read(&c_stream, buffer, sizeof(buffer), 1);
    fossil_io_file_close(&c_stream);

    ASSUME_ITS_EQUAL_CSTR(content, buffer);
}


FOSSIL_TEST(c_test_stream_let_open_and_close_file) {
    const char *filename = "testfile.txt";

    // Open the file
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "w"));
    fossil_io_file_close(&c_stream);
}

FOSSIL_TEST(c_test_stream_multiple_files) {
    const char *filename1 = "testfile1.txt";
    const char *filename2 = "testfile2.txt";

    // Open the first file
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename1, "w"));
    fossil_io_file_close(&c_stream);

    // Open the second file
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename2, "w"));
    fossil_io_file_close(&c_stream);
}

FOSSIL_TEST(c_test_stream_seek_and_tell) {
    const char *filename = "testfile.txt";
    const char *content = "This is a test.";

    // Write data to the file
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "w"));
    fossil_io_file_write(&c_stream, content, strlen(content), 1);
    fossil_io_file_close(&c_stream);

    // Open the file
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "r"));

    // Seek to the end of the file
    fossil_io_file_seek(&c_stream, 0, SEEK_END);

    // Get the current position
    long position = fossil_io_file_tell(&c_stream);

    ASSUME_ITS_TRUE(position > 0);

    // Close the file
    fossil_io_file_close(&c_stream);
}

FOSSIL_TEST(c_test_stream_get_type) {
    const char *filename = "testfile_type.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "w"));
    fossil_io_file_close(&c_stream);

    // Check the file type
    ASSUME_ITS_EQUAL_I32(2, fossil_io_file_get_type(filename));  // Regular file
}

FOSSIL_TEST(c_test_stream_is_readable) {
    const char *filename = "testfile_readable.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "w"));
    fossil_io_file_close(&c_stream);

    // Check if the file is readable
    ASSUME_ITS_EQUAL_I32(1, fossil_io_file_is_readable(filename));
}

FOSSIL_TEST(c_test_stream_is_writable) {
    const char *filename = "testfile_writable.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "w"));
    fossil_io_file_close(&c_stream);

    // Check if the file is writable
    ASSUME_ITS_EQUAL_I32(1, fossil_io_file_is_writable(filename));
}

FOSSIL_TEST(c_test_stream_is_executable) {
    const char *filename = "testfile_executable.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "w"));
    fossil_io_file_close(&c_stream);

    // Check if the file is executable
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_is_executable(filename));
}

FOSSIL_TEST(c_test_stream_set_permissions) {
    const char *filename = "testfile_permissions.txt";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "w"));
    fossil_io_file_close(&c_stream);

    // Set file permissions
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_set_permissions(filename, 0644));

    // Check if the file is readable and writable
    ASSUME_ITS_EQUAL_I32(1, fossil_io_file_is_readable(filename));
    ASSUME_ITS_EQUAL_I32(1, fossil_io_file_is_writable(filename));
}

FOSSIL_TEST(c_test_stream_get_permissions) {
    const char *filename = "testfile_get_permissions.txt";
    int32_t mode;

    // // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "w"));
    fossil_io_file_close(&c_stream);

    // Set file permissions
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_set_permissions(filename, 0644));

    // Get file permissions
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_get_permissions(filename, &mode));
}

FOSSIL_TEST(c_test_stream_flush_file) {
    const char *filename = "testfile_flush.txt";
    const char *content = "This is a test.";

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "w"));
    fossil_io_file_write(&c_stream, content, strlen(content), 1);

    // Flush the file
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_flush(&c_stream));
    fossil_io_file_close(&c_stream);
}

FOSSIL_TEST(c_test_stream_setpos_and_getpos) {
    const char *filename = "testfile_setpos_getpos.txt";
    const char *content = "This is a test.";
    int64_t pos;

    // Create the file
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "w"));
    fossil_io_file_write(&c_stream, content, strlen(content), 1);
    fossil_io_file_close(&c_stream);

    // Open the file
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "r"));

    // Set the file position
    fossil_io_pos_t pos_set = 5;
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_setpos(&c_stream, &pos_set));

    // Get the file position
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_getpos(&c_stream, &pos));
    ASSUME_ITS_EQUAL_I32(5, pos);

    // Close the file
    fossil_io_file_close(&c_stream);
}

FOSSIL_TEST(c_test_stream_ai_analyze) {
    const char *filename = "testfile_ai.txt";
    const char *content = "This is a test for AI analysis.";

    // Write data to the file
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "w"));
    fossil_io_file_write(&c_stream, content, strlen(content), 1);
    fossil_io_file_close(&c_stream);

    // Open for reading and run AI analysis
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "r"));
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_ai_analyze(&c_stream));
    fossil_io_file_close(&c_stream);
}

FOSSIL_TEST(c_test_stream_ai_compute_embedding) {
    const char *filename = "testfile_ai_embed.txt";
    const char *content = "Embedding test content.";
    char dummy_model[16] = {0};

    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "w"));
    fossil_io_file_write(&c_stream, content, strlen(content), 1);
    fossil_io_file_close(&c_stream);

    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "r"));
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_ai_compute_embedding(&c_stream, dummy_model, sizeof(dummy_model)));
    fossil_io_file_close(&c_stream);
}

FOSSIL_TEST(c_test_stream_ai_ready_and_reset) {
    const char *filename = "testfile_ai_ready.txt";
    const char *content = "Ready/reset test.";

    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "w"));
    fossil_io_file_write(&c_stream, content, strlen(content), 1);
    fossil_io_file_close(&c_stream);

    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "r"));
    ASSUME_ITS_TRUE(fossil_io_file_ai_ready(&c_stream));
    fossil_io_file_ai_reset(&c_stream);
    fossil_io_file_close(&c_stream);
}

FOSSIL_TEST(c_test_stream_add_tag) {
    const char *filename = "testfile_add_tag.txt";
    const char *content = "Tag add test.";

    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "w"));
    fossil_io_file_write(&c_stream, content, strlen(content), 1);
    fossil_io_file_close(&c_stream);

    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "r"));
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_add_tag(&c_stream, "test-tag"));
    fossil_io_file_close(&c_stream);
}

FOSSIL_TEST(c_test_stream_detect_binary) {
    const char *filename = "testfile_detect_binary.txt";
    const char *content = "Binary detection test.";

    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "w"));
    fossil_io_file_write(&c_stream, content, strlen(content), 1);
    fossil_io_file_close(&c_stream);

    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "r"));
    fossil_io_file_detect_binary(&c_stream);
    fossil_io_file_close(&c_stream);
}

FOSSIL_TEST(c_test_stream_compress_and_decompress) {
    const char *filename = "testfile_compress.txt";
    const char *content = "Compression test.";

    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "w"));
    fossil_io_file_write(&c_stream, content, strlen(content), 1);
    fossil_io_file_close(&c_stream);

    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_open(&c_stream, filename, "r"));
    ASSUME_ITS_EQUAL_I32(0, fossil_io_file_compress(&c_stream));
    ASSUME_NOT_EQUAL_I32(0, fossil_io_file_decompress(&c_stream));
    fossil_io_file_close(&c_stream);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_file_tests) {
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_tempfile_creation);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_tempfile_cleanup);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_let_write_and_read_file);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_let_open_and_close_file);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_redirect_to_devnull);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_multiple_files);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_seek_and_tell);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_get_type);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_is_readable);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_is_writable);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_is_executable);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_set_permissions);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_get_permissions);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_flush_file);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_setpos_and_getpos);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_ai_analyze);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_ai_compute_embedding);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_ai_ready_and_reset);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_add_tag);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_detect_binary);
    FOSSIL_TEST_ADD(c_stream_suite, c_test_stream_compress_and_decompress);

    FOSSIL_TEST_REGISTER(c_stream_suite);
}
