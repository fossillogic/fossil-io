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
#include <fossil/unittest/framework.h>
#include <fossil/mockup/framework.h>
#include <fossil/xassume.h>

#include "fossil/io/framework.h"

FOSSIL_TEST_DATA(StreamTestData) {
    fossil_fstream_t stream;
} io;

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(stream_let_write_and_read_file) {
    const char *filename = "testfile.txt";
    const char *content = "This is a test.";

    // Write data to the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&io.stream, filename, "w"));
    fossil_fstream_write(&io.stream, content, strlen(content), 1);
    fossil_fstream_close(&io.stream);

    // Read data from the file
    char buffer[1024];
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&io.stream, filename, "r"));
    fossil_fstream_read(&io.stream, buffer, sizeof(buffer), 1);
    fossil_fstream_close(&io.stream);
}

FOSSIL_TEST(stream_let_open_and_close_file) {
    const char *filename = "testfile.txt";

    // Open the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&io.stream, filename, "w"));
    fossil_fstream_close(&io.stream);
}

FOSSIL_TEST(stream_multiple_files) {
    const char *filename1 = "testfile1.txt";
    const char *filename2 = "testfile2.txt";

    // Open the first file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&io.stream, filename1, "w"));
    fossil_fstream_close(&io.stream);

    // Open the second file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&io.stream, filename2, "w"));
    fossil_fstream_close(&io.stream);
}

FOSSIL_TEST(stream_seek_and_tell) {
    const char *filename = "testfile.txt";
    const char *content = "This is a test.";

    // Write data to the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&io.stream, filename, "w"));
    fossil_fstream_write(&io.stream, content, strlen(content), 1);
    fossil_fstream_close(&io.stream);

    // Open the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&io.stream, filename, "r"));

    // Seek to the end of the file
    fossil_fstream_seek(&io.stream, 0, SEEK_END);

    // Get the current position
    long position = fossil_fstream_tell(&io.stream);

    // Close the file
    fossil_fstream_close(&io.stream);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_file_tests) {
    ADD_TEST(stream_let_write_and_read_file);
    ADD_TEST(stream_let_open_and_close_file);
    ADD_TEST(stream_multiple_files);
}