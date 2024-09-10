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
    fossil_mockup_file_t *mock_file;
    fossil_fstream_t stream;
} io;

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(stream_let_open_and_close_file) {
    io.mock_file = fossil_mockup_file_create("testfile.txt", "");
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&io.stream, io.mock_file->filename, "w"));
    ASSUME_NOT_CNULL(io.stream.file);
    fossil_fstream_close(&io.stream);
    ASSUME_ITS_CNULL(io.stream.file);
    fossil_mockup_file_erase(io.mock_file);
}

FOSSIL_TEST(stream_let_write_and_read_file) {
    const char *filename = "testfile.txt";
    const char *content = "This is a test.";
    io.mock_file = fossil_mockup_file_create(filename, "");

    // Write data to the file
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&io.stream, filename, "w"));
    fossil_fstream_write(&io.stream, content, strlen(content), 1);
    fossil_fstream_close(&io.stream);

    // Read data from the file
    char buffer[1024];
    ASSUME_ITS_EQUAL_I32(0, fossil_fstream_open(&io.stream, filename, "r"));
    fossil_fstream_read(&io.stream, buffer, sizeof(buffer), 1);
    fossil_fstream_close(&io.stream);
    // ASSUME_ITS_EQUAL_CSTR(content, buffer); make issue ticket for Fossil Mock io issues

    fossil_mockup_file_erase(io.mock_file);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_file_tests) {
    ADD_TEST(stream_let_open_and_close_file);
    ADD_TEST(stream_let_write_and_read_file);
}