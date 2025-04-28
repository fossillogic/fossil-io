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
// * Fossil Logic Test Utilities
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_SUITE(cpp_network_suite);

// Setup function for the test suite
FOSSIL_SETUP(cpp_network_suite) {
    // Setup code (if needed)
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_network_suite) {
    // Teardown code (if needed)
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_CASE(cpp_test_nstream_create_and_destroy) {
    const char *protocols[] = {"tcp", "udp", "raw", "icmp", "sctp", "http", "https", "ftp", "ssh", "dns", "ntp", "smtp", "pop3", "imap", "ldap", "mqtt"};
    const char *clients[] = {"mail-server", "server", "mail-client", "client", "mail-bot", "bot", "multicast", "broadcast"};

    for (size_t i = 0; i < sizeof(protocols) / sizeof(protocols[0]); i++) {
        for (size_t j = 0; j < sizeof(clients) / sizeof(clients[0]); j++) {
            fossil_nstream_t *stream = fossil_nstream_create(protocols[i], clients[j]);
            ASSUME_NOT_CNULL(stream);
            fossil_nstream_destroy(stream);
        }
    }
}

FOSSIL_TEST_CASE(cpp_test_nstream_connect_invalid_host) {
    fossil_nstream_t *stream = fossil_nstream_create("tcp", "client");
    ASSUME_NOT_CNULL(stream);

    // Attempt to connect to an invalid host
    ASSUME_ITS_EQUAL_I32(-1, fossil_nstream_connect(stream, "invalid_host", 12345));
    fossil_nstream_destroy(stream);
}

FOSSIL_TEST_CASE(cpp_test_nstream_listen_and_accept) {
    fossil_nstream_t *server = fossil_nstream_create("tcp", "server");
    ASSUME_NOT_CNULL(server);

    // Start listening on a local port
    ASSUME_ITS_EQUAL_I32(0, fossil_nstream_listen(server, "127.0.0.1", 12345));

    // Simulate a client connecting
    fossil_nstream_t *client = fossil_nstream_create("tcp", "client");
    ASSUME_NOT_CNULL(client);
    ASSUME_ITS_EQUAL_I32(0, fossil_nstream_connect(client, "127.0.0.1", 12345));

    // Accept the client connection
    fossil_nstream_t *accepted_client = fossil_nstream_accept(server);
    ASSUME_NOT_CNULL(accepted_client);

    // Cleanup
    fossil_nstream_destroy(client);
    fossil_nstream_destroy(accepted_client);
    fossil_nstream_destroy(server);
}

FOSSIL_TEST_CASE(cpp_test_nstream_send_and_receive) {
    fossil_nstream_t *server = fossil_nstream_create("tcp", "server");
    ASSUME_NOT_CNULL(server);

    // Start listening on a local port
    ASSUME_ITS_EQUAL_I32(0, fossil_nstream_listen(server, "127.0.0.1", 12345));

    // Simulate a client connecting
    fossil_nstream_t *client = fossil_nstream_create("tcp", "client");
    ASSUME_NOT_CNULL(client);
    ASSUME_ITS_EQUAL_I32(0, fossil_nstream_connect(client, "127.0.0.1", 12345));

    // Accept the client connection
    fossil_nstream_t *accepted_client = fossil_nstream_accept(server);
    ASSUME_NOT_CNULL(accepted_client);

    // Send and receive data
    const char *message = "Hello, Fossil!";
    ASSUME_ITS_EQUAL_I32(strlen(message), fossil_nstream_send(client, message, strlen(message)));

    char buffer[1024] = {0};
    ASSUME_ITS_EQUAL_I32(strlen(message), fossil_nstream_recv(accepted_client, buffer, sizeof(buffer)));
    ASSUME_ITS_EQUAL_CSTR(message, buffer);

    // Cleanup
    fossil_nstream_destroy(client);
    fossil_nstream_destroy(accepted_client);
    fossil_nstream_destroy(server);
}

FOSSIL_TEST_CASE(cpp_test_nstream_protocols) {
    const char *protocols[] = {"tcp", "udp", "raw", "icmp", "sctp", "http", "https", "ftp", "ssh", "dns", "ntp", "smtp", "pop3", "imap", "ldap", "mqtt"};

    for (size_t i = 0; i < sizeof(protocols) / sizeof(protocols[0]); i++) {
        fossil_nstream_t *stream = fossil_nstream_create(protocols[i], "client");
        ASSUME_NOT_CNULL(stream);
        fossil_nstream_destroy(stream);
    }
}

FOSSIL_TEST_CASE(cpp_test_nstream_client_types) {
    const char *clients[] = {"mail-server", "server", "mail-client", "client", "mail-bot", "bot", "multicast", "broadcast"};

    for (size_t i = 0; i < sizeof(clients) / sizeof(clients[0]); i++) {
        fossil_nstream_t *stream = fossil_nstream_create("tcp", clients[i]);
        ASSUME_NOT_CNULL(stream);
        fossil_nstream_destroy(stream);
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_network_tests) {
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_create_and_destroy);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_connect_invalid_host);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_listen_and_accept);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_send_and_receive);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_protocols);
    FOSSIL_TEST_ADD(cpp_network_suite, cpp_test_nstream_client_types);

    FOSSIL_TEST_REGISTER(cpp_network_suite);
}
