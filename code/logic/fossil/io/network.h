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
#ifndef FOSSIL_IO_NETWORK_H
#define FOSSIL_IO_NETWORK_H

#include <stdint.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    typedef SOCKET fossil_io_socket_t;
    #define FOSSIL_IO_INVALID_SOCKET INVALID_SOCKET
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    #define closesocket close
    typedef int fossil_io_socket_t;
    #define FOSSIL_IO_INVALID_SOCKET (-1)
#endif

#define FOSSIL_IO_SOCKET_TYPE_TCP SOCK_STREAM
#define FOSSIL_IO_SOCKET_TYPE_UDP SOCK_DGRAM

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the network stack (needed for Windows).
 * Returns 0 on success, non-zero on failure.
 */
int fossil_io_network_create(void);

/**
 * Clean up network stack (needed for Windows).
 */
void fossil_io_network_destroy(void);

/**
 * Create a new TCP socket.
 * Returns a valid socket on success or FOSSIL_IO_INVALID_SOCKET on failure.
 */
fossil_io_socket_t fossil_io_network_create_socket(int type);

/**
 * Bind a socket to a specific port (IPv4/IPv6).
 * Returns 0 on success, -1 on failure.
 */
int fossil_io_network_bind(fossil_io_socket_t sock, const char *ip, uint16_t port);

/**
 * Listen for incoming connections.
 * Returns 0 on success, -1 on failure.
 */
int fossil_io_network_listen(fossil_io_socket_t sock, int backlog);

/**
 * Accept a new connection.
 * Returns a valid socket on success, or FOSSIL_IO_INVALID_SOCKET on failure.
 */
fossil_io_socket_t fossil_io_network_accept(fossil_io_socket_t sock, char *client_ip, uint16_t *client_port);

/**
 * Connect to a remote server.
 * Returns 0 on success, -1 on failure.
 */
int fossil_io_network_connect(fossil_io_socket_t sock, const char *ip, uint16_t port);

/**
 * Send data over a socket.
 * Returns the number of bytes sent, or -1 on failure.
 */
int fossil_io_network_send(fossil_io_socket_t sock, const void *data, size_t len);

/**
 * Receive data from a socket.
 * Returns the number of bytes received, or -1 on failure.
 */
int fossil_io_network_receive(fossil_io_socket_t sock, void *buffer, size_t len);

/**
 * Close a socket.
 */
void fossil_io_network_close(fossil_io_socket_t sock);

/**
 * Send data to a specific IP address and port.
 * Returns the number of bytes sent, or -1 on failure.
 */
int fossil_io_network_sendto(fossil_io_socket_t sock, const void *data, size_t len, const char *ip, uint16_t port);

/**
 * Receive data from a specific IP address and port.
 * Returns the number of bytes received, or -1 on failure.
 */
int fossil_io_network_recvfrom(fossil_io_socket_t sock, void *buffer, size_t len, char *ip, uint16_t *port);

#ifdef __cplusplus
}

/**
 * C++ wrapper for the output functions.
 */
namespace fossil {
    /**
     * Namespace for input/output operations.
     */
    namespace io {
        /**
         * Class for network operations.
         */
        class Network {
        public:
            /**
             * Initialize the network stack (needed for Windows).
             * Returns 0 on success, non-zero on failure.
             */
            static int init(void) {
                return fossil_io_network_create();
            }

            /**
             * Clean up network stack (needed for Windows).
             */
            static void cleanup(void) {
                fossil_io_network_destroy();
            }

            /**
             * Create a new TCP socket.
             * Returns a valid socket on success or FOSSIL_IO_INVALID_SOCKET on failure.
             */
            static fossil_io_socket_t create_socket(int type) {
                return fossil_io_network_create_socket(type);
            }

            /**
             * Bind a socket to a specific port (IPv4/IPv6).
             * Returns 0 on success, -1 on failure.
             */
            static int bind(fossil_io_socket_t sock, const char *ip, uint16_t port) {
                return fossil_io_network_bind(sock, ip, port);
            }

            /**
             * Listen for incoming connections.
             * Returns 0 on success, -1 on failure.
             */
            static int listen(fossil_io_socket_t sock, int backlog) {
                return fossil_io_network_listen(sock, backlog);
            }

            /**
             * Accept a new connection.
             * Returns a valid socket on success, or FOSSIL_IO_INVALID_SOCKET on failure.
             */
            static fossil_io_socket_t accept(fossil_io_socket_t sock, char *client_ip, uint16_t *client_port) {
                return fossil_io_network_accept(sock, client_ip, client_port);
            }

            /**
             * Connect to a remote server.
             * Returns 0 on success, -1 on failure.
             */
            static int connect(fossil_io_socket_t sock, const char *ip, uint16_t port) {
                return fossil_io_network_connect(sock, ip, port);
            }

            /**
             * Send data over a socket.
             * Returns the number of bytes sent, or -1 on failure.
             */
            static int send(fossil_io_socket_t sock, const void *data, size_t len) {
                return fossil_io_network_send(sock, data, len);
            }

            /**
             * Receive data from a socket.
             * Returns the number of bytes received, or -1 on failure.
             */
            static int receive(fossil_io_socket_t sock, void *buffer, size_t len) {
                return fossil_io_network_receive(sock, buffer, len);
            }

            /**
             * Close a socket.
             */
            static void close(fossil_io_socket_t sock) {
                fossil_io_network_close(sock);
            }

            /**
             * Send data to a specific IP address and port.
             * Returns the number of bytes sent, or -1 on failure.
             */
            static int sendto(fossil_io_socket_t sock, const void *data, size_t len, const char *ip, uint16_t port) {
                return fossil_io_network_sendto(sock, data, len, ip, port);
            }

            /**
             * Receive data from a specific IP address and port.
             * Returns the number of bytes received, or -1 on failure.
             */
            static int recvfrom(fossil_io_socket_t sock, void *buffer, size_t len, char *ip, uint16_t *port) {
                return fossil_io_network_recvfrom(sock, buffer, len, ip, port);
            }

        };
    }
}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
