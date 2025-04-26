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
FOSSIL_TEST_SUITE(cpp_network_suite);

// Setup function for the test suite
FOSSIL_SETUP(cpp_network_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_network_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_CASE(cpp_test_nstream_open) {
    fossil_nstream_t *ns = fossil_nstream_open("tcp", "127.0.0.1", "8080", NULL);
    ASSUME_NOT_CNULL(ns);
    fossil_nstream_close(ns);
}

FOSSIL_TEST_CASE(cpp_test_nstream_send_recv) {
    fossil_nstream_t *ns = fossil_nstream_open("tcp", "127.0.0.1", "8080", NULL);
    ASSUME_NOT_CNULL(ns);

    const char *message = "Hello, Fossil!";
    ssize_t bytes_sent = fossil_nstream_send(ns, message, strlen(message));
    ASSUME_ITS_EQUAL_I32((int)strlen(message), bytes_sent);

    char buffer[128];
    ssize_t bytes_received = fossil_nstream_recv(ns, buffer, sizeof(buffer));
    ASSUME_ITS_TRUE(bytes_received > 0);

    fossil_nstream_close(ns);
}

FOSSIL_TEST_CASE(cpp_test_nstream_listen_accept) {
    fossil_nstream_t *server = fossil_nstream_open("tcp", "127.0.0.1", "8080", NULL);
    ASSUME_NOT_CNULL(server);

    int result = fossil_nstream_listen(server, 5);
    ASSUME_ITS_EQUAL_I32(0, result);

    fossil_nstream_t *client = fossil_nstream_accept(server);
    ASSUME_NOT_CNULL(client);

    fossil_nstream_close(client);
    fossil_nstream_close(server);
}

FOSSIL_TEST_CASE(cpp_test_nstream_set_nonblocking) {
    fossil_nstream_t *ns = fossil_nstream_open("tcp", "127.0.0.1", "8080", NULL);
    ASSUME_NOT_CNULL(ns);

    int result = fossil_nstream_set_nonblocking(ns, 1);
    ASSUME_ITS_EQUAL_I32(0, result);

    fossil_nstream_close(ns);
}

FOSSIL_TEST_CASE(cpp_test_nstream_wait_readable_writable) {
    fossil_nstream_t *ns = fossil_nstream_open("tcp", "127.0.0.1", "8080", NULL);
    ASSUME_NOT_CNULL(ns);

    int result = fossil_nstream_wait_readable(ns, 1000);
    ASSUME_ITS_TRUE(result == 0 || result == 1);

    result = fossil_nstream_wait_writable(ns, 1000);
    ASSUME_ITS_TRUE(result == 0 || result == 1);

    fossil_nstream_close(ns);
}

FOSSIL_TEST_CASE(cpp_test_nstream_connect_timeout) {
    fossil_nstream_t *ns = fossil_nstream_open("tcp", NULL, NULL, NULL);
    ASSUME_NOT_CNULL(ns);

    int result = fossil_nstream_connect_timeout(ns, "127.0.0.1", "8080", 1000);
    ASSUME_ITS_EQUAL_I32(0, result);

    fossil_nstream_close(ns);
}

FOSSIL_TEST_CASE(cpp_test_nstream_get_peer_info) {
    fossil_nstream_t *ns = fossil_nstream_open("tcp", "127.0.0.1", "8080", NULL);
    ASSUME_NOT_CNULL(ns);

    char ip_str[64];
    uint16_t port;
    int result = fossil_nstream_get_peer_info(ns, ip_str, sizeof(ip_str), &port);
    ASSUME_ITS_EQUAL_I32(0, result);

    fossil_nstream_close(ns);
}

FOSSIL_TEST_CASE(cpp_test_nstream_send_recv_line) {
    fossil_nstream_t *ns = fossil_nstream_open("tcp", "127.0.0.1", "8080", NULL);
    ASSUME_NOT_CNULL(ns);

    const char *line = "Hello, Fossil Logic!\n";
    ssize_t bytes_sent = fossil_nstream_send_line(ns, line);
    ASSUME_ITS_EQUAL_I32((int)strlen(line), bytes_sent);

    char buffer[128];
    ssize_t bytes_received = fossil_nstream_recv_line(ns, buffer, sizeof(buffer));
    ASSUME_ITS_TRUE(bytes_received > 0);

    fossil_nstream_close(ns);
}

FOSSIL_TEST_CASE(cpp_test_nstream_ssl_send_recv) {
    fossil_nstream_t *ns = fossil_nstream_open("tls", "127.0.0.1", "8080", NULL);
    ASSUME_NOT_CNULL(ns);

    const char *message = "Secure Hello!";
    ssize_t bytes_sent = fossil_nstream_ssl_send(ns, message, strlen(message));
    ASSUME_ITS_EQUAL_I32((int)strlen(message), bytes_sent);

    char buffer[128];
    ssize_t bytes_received = fossil_nstream_ssl_recv(ns, buffer, sizeof(buffer));
    ASSUME_ITS_TRUE(bytes_received > 0);

    fossil_nstream_close(ns);
}

/**
 * Test cases for the C++ NStream wrapper class.
 */

FOSSIL_TEST_CASE(cpp_test_nstream_class_send_recv) {
    fossil::io::NStream ns("tcp", "127.0.0.1", "8080", "");

    const std::string message = "Hello, Fossil!";
    ssize_t bytes_sent = ns.send(message.c_str(), message.size());
    ASSUME_ITS_EQUAL_I32((int)message.size(), bytes_sent);

    std::string buffer;
    buffer.resize(128);
    ssize_t bytes_received = ns.recv(&buffer[0], buffer.size());
    ASSUME_ITS_TRUE(bytes_received > 0);
}

FOSSIL_TEST_CASE(cpp_test_nstream_class_listen_accept) {
    fossil::io::NStream server("tcp", "127.0.0.1", "8080", "");

    int result = server.listen(5);
    ASSUME_ITS_EQUAL_I32(0, result);

    fossil::io::NStream *client = server.accept();

    delete client;
}

FOSSIL_TEST_CASE(cpp_test_nstream_class_set_nonblocking) {
    fossil::io::NStream ns("tcp", "127.0.0.1", "8080", "");

    int result = ns.set_non_blocking(1);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST_CASE(cpp_test_nstream_class_wait_readable_writable) {
    fossil::io::NStream ns("tcp", "127.0.0.1", "8080", "");

    int result = ns.wait_readable(1000);
    ASSUME_ITS_TRUE(result == 0 || result == 1);

    result = ns.wait_writable(1000);
    ASSUME_ITS_TRUE(result == 0 || result == 1);
}

FOSSIL_TEST_CASE(cpp_test_nstream_class_connect_timeout) {
    fossil::io::NStream ns("tcp", "", "", "");

    int result = ns.connect_timeout("127.0.0.1", "8080", 1000);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST_CASE(cpp_test_nstream_class_get_peer_info) {
    fossil::io::NStream ns("tcp", "127.0.0.1", "8080", "");

    std::string ip_str;
    uint16_t port;
    int result = ns.get_peer_info(ip_str, port);
    ASSUME_ITS_EQUAL_I32(0, result);
}

FOSSIL_TEST_CASE(cpp_test_nstream_class_send_recv_line) {
    fossil::io::NStream ns("tcp", "127.0.0.1", "8080", "");

    const std::string line = "Hello, Fossil Logic!\n";
    ssize_t bytes_sent = ns.send_line(line);
    ASSUME_ITS_EQUAL_I32((int)line.size(), bytes_sent);

    std::string buffer;
    ssize_t bytes_received = ns.recv_line(buffer, 128);
    ASSUME_ITS_TRUE(bytes_received > 0);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_network_tests) {
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_open);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_send_recv);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_listen_accept);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_set_nonblocking);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_wait_readable_writable);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_connect_timeout);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_get_peer_info);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_send_recv_line);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_ssl_send_recv);

    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_class_send_recv);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_class_listen_accept);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_class_set_nonblocking);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_class_wait_readable_writable);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_class_connect_timeout);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_class_get_peer_info);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_class_send_recv_line);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_class_ssl_send_recv);

    FOSSIL_TEST_REGISTER(cpp_network_suite);
}
