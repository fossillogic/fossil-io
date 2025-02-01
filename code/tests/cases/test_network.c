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
FOSSIL_TEST_SUITE(c_network_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_network_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_network_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_CASE(c_test_network_init) {
    int result = fossil_io_network_create();
    ASSUME_ITS_EQUAL_I32(0, result);
    fossil_io_network_destroy();
}

FOSSIL_TEST_CASE(c_test_network_create_socket) {
    fossil_io_network_create();
    fossil_io_socket_t sock = fossil_io_network_create_socket();
    ASSUME_NOT_EQUAL_I32(FOSSIL_IO_INVALID_SOCKET, sock);
    fossil_io_network_close(sock);
    fossil_io_network_destroy();
}

FOSSIL_TEST_CASE(c_test_network_bind) {
    fossil_io_network_create();
    fossil_io_socket_t sock = fossil_io_network_create_socket();
    ASSUME_NOT_EQUAL_I32(FOSSIL_IO_INVALID_SOCKET, sock);
    int result = fossil_io_network_bind(sock, "127.0.0.1", 8080);
    ASSUME_ITS_EQUAL_I32(0, result);
    fossil_io_network_close(sock);
    fossil_io_network_destroy();
}

FOSSIL_TEST_CASE(c_test_network_listen) {
    fossil_io_network_create();
    fossil_io_socket_t sock = fossil_io_network_create_socket();
    ASSUME_NOT_EQUAL_I32(FOSSIL_IO_INVALID_SOCKET, sock);
    fossil_io_network_bind(sock, "127.0.0.1", 8080);
    int result = fossil_io_network_listen(sock, 5);
    ASSUME_ITS_EQUAL_I32(0, result);
    fossil_io_network_close(sock);
    fossil_io_network_destroy();
}

FOSSIL_TEST_CASE(c_test_network_close) {
    fossil_io_network_create();
    fossil_io_socket_t sock = fossil_io_network_create_socket();
    ASSUME_NOT_EQUAL_I32(FOSSIL_IO_INVALID_SOCKET, sock);
    fossil_io_network_close(sock);
    // No direct way to test close, but we assume no errors if it reaches here
    fossil_io_network_destroy();
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_network_tests) {
    FOSSIL_TEST_ADD(c_network_suite, c_test_network_init);
    FOSSIL_TEST_ADD(c_network_suite, c_test_network_create_socket);
    FOSSIL_TEST_ADD(c_network_suite, c_test_network_bind);
    FOSSIL_TEST_ADD(c_network_suite, c_test_network_listen);
    FOSSIL_TEST_ADD(c_network_suite, c_test_network_close);

    FOSSIL_TEST_REGISTER(c_network_suite);
}
