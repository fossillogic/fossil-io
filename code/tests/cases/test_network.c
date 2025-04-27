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

// Fossil Logic Test Cases

// Helper: Open connection or skip
static fossil_nstream_t *try_open_tcp(const char *addr, const char *port) {
    fossil_nstream_t *ns = fossil_nstream_open("tcp", addr, port, NULL);
    if (!ns) {
        FOSSIL_TEST_SKIP("Unable to open TCP connection to %s:%s", addr, port);
    }
    return ns;
}

static fossil_nstream_t *try_open_tls(const char *addr, const char *port) {
    fossil_nstream_t *ns = fossil_nstream_open("tls", addr, port, NULL);
    if (!ns) {
        FOSSIL_TEST_SKIP("Unable to open TLS connection to %s:%s", addr, port);
    }
    return ns;
}

FOSSIL_TEST_CASE(c_test_nstream_open) {
    fossil_nstream_t *ns = try_open_tcp("127.0.0.1", "8080");
    fossil_nstream_close(ns);
}

FOSSIL_TEST_CASE(c_test_nstream_send_recv) {
    fossil_nstream_t *ns = try_open_tcp("127.0.0.1", "8080");

    const char *message = "Hello, Fossil!";
    ssize_t bytes_sent = fossil_nstream_send(ns, message, strlen(message));
    ASSUME_ITS_TRUE(bytes_sent >= 0);

    char buffer[128];
    ssize_t bytes_received = fossil_nstream_recv(ns, buffer, sizeof(buffer));
    ASSUME_ITS_TRUE(bytes_received >= 0);

    fossil_nstream_close(ns);
}

FOSSIL_TEST_CASE(c_test_nstream_listen_accept) {
    fossil_nstream_t *server = try_open_tcp("127.0.0.1", "8080");

    int result = fossil_nstream_listen(server, 5);
    ASSUME_ITS_EQUAL_I32(0, result);

    // We don't expect actual client in this minimal test
    fossil_nstream_close(server);
}

FOSSIL_TEST_CASE(c_test_nstream_set_nonblocking) {
    fossil_nstream_t *ns = try_open_tcp("127.0.0.1", "8080");

    int result = fossil_nstream_set_nonblocking(ns, 1);
    ASSUME_ITS_EQUAL_I32(0, result);

    fossil_nstream_close(ns);
}

FOSSIL_TEST_CASE(c_test_nstream_wait_readable_writable) {
    fossil_nstream_t *ns = try_open_tcp("127.0.0.1", "8080");

    int readable = fossil_nstream_wait_readable(ns, 1000);
    int writable = fossil_nstream_wait_writable(ns, 1000);

    ASSUME_ITS_TRUE(readable >= 0);
    ASSUME_ITS_TRUE(writable >= 0);

    fossil_nstream_close(ns);
}

FOSSIL_TEST_CASE(c_test_nstream_connect_timeout) {
    fossil_nstream_t *ns = fossil_nstream_open("tcp", NULL, NULL, NULL);
    ASSUME_NOT_CNULL(ns);

    int result = fossil_nstream_connect_timeout(ns, "127.0.0.1", "8080", 1000);
    ASSUME_ITS_TRUE(result == 0 || result == -1); // Allow failure if no server.

    fossil_nstream_close(ns);
}

FOSSIL_TEST_CASE(c_test_nstream_get_peer_info) {
    fossil_nstream_t *ns = try_open_tcp("127.0.0.1", "8080");

    char ip_str[64];
    uint16_t port;
    int result = fossil_nstream_get_peer_info(ns, ip_str, sizeof(ip_str), &port);
    ASSUME_ITS_EQUAL_I32(0, result);

    fossil_nstream_close(ns);
}

FOSSIL_TEST_CASE(c_test_nstream_send_recv_line) {
    fossil_nstream_t *ns = try_open_tcp("127.0.0.1", "8080");

    const char *line = "Hello, Fossil Logic!";
    ssize_t bytes_sent = fossil_nstream_send_line(ns, line);
    ASSUME_ITS_TRUE(bytes_sent >= 0);

    char buffer[128];
    ssize_t bytes_received = fossil_nstream_recv_line(ns, buffer, sizeof(buffer));
    ASSUME_ITS_TRUE(bytes_received >= 0);

    fossil_nstream_close(ns);
}

FOSSIL_TEST_CASE(c_test_nstream_ssl_send_recv) {
    fossil_nstream_t *ns = try_open_tls("127.0.0.1", "8080");

    const char *message = "Secure Hello!";
    ssize_t bytes_sent = fossil_nstream_ssl_send(ns, message, strlen(message));
    ASSUME_ITS_TRUE(bytes_sent >= 0);

    char buffer[128];
    ssize_t bytes_received = fossil_nstream_ssl_recv(ns, buffer, sizeof(buffer));
    ASSUME_ITS_TRUE(bytes_received >= 0);

    fossil_nstream_close(ns);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_network_tests) {
    FOSSIL_TEST_ADD(c_network_tests, c_test_nstream_open);
    FOSSIL_TEST_ADD(c_network_tests, c_test_nstream_send_recv);
    FOSSIL_TEST_ADD(c_network_tests, c_test_nstream_listen_accept);
    FOSSIL_TEST_ADD(c_network_tests, c_test_nstream_set_nonblocking);
    FOSSIL_TEST_ADD(c_network_tests, c_test_nstream_wait_readable_writable);
    FOSSIL_TEST_ADD(c_network_tests, c_test_nstream_connect_timeout);
    FOSSIL_TEST_ADD(c_network_tests, c_test_nstream_get_peer_info);
    FOSSIL_TEST_ADD(c_network_tests, c_test_nstream_send_recv_line);
    FOSSIL_TEST_ADD(c_network_tests, c_test_nstream_ssl_send_recv);

    FOSSIL_TEST_REGISTER(c_network_tests);
}
