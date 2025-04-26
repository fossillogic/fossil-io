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

#include <stddef.h>
#include <stdint.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Network stream structure
typedef struct fossil_nstream_t {
    int socket;            // Socket file descriptor
    int is_tls;            // Whether the stream uses TLS
    struct sockaddr_in addr;  // Address for connections
    char protocol[16];         // Protocol type (TCP, UDP, etc.)
} fossil_nstream_t;

/**
 * Initialize the network stack. This function is necessary for Windows
 * platforms to set up the Winsock library. On other platforms, it may
 * perform other necessary initializations.
 * 
 * @return 0 on success, non-zero on failure.
 */
fossil_nstream_t *fossil_nstream_open(const char *protocol, const char *host, const char *port, const char *flags);

/**
 * Send data through the network stream.
 *
 * @param ns The network stream to send data through.
 * @param buf The buffer containing the data to send.
 * @param len The length of the data to send.
 * @return The number of bytes sent, or -1 on failure.
 */
ssize_t fossil_nstream_send(fossil_nstream_t *ns, const void *buf, size_t len);

/**
 * Receive data through the network stream.
 * 
 * @param ns The network stream to receive data from.
 * @param buf The buffer to store the received data.
 * @param len The maximum length of data to receive.
 * @return The number of bytes received, or -1 on failure.
 */
ssize_t fossil_nstream_recv(fossil_nstream_t *ns, void *buf, size_t len);

/**
 * Listen on a network stream (for servers).
 *
 * @param ns The network stream to listen on.
 * @param backlog The maximum length of the queue of pending connections.
 * @return 0 on success, -1 on failure.
 */
int fossil_nstream_listen(fossil_nstream_t *ns, int backlog);

/**
 * Accept a new incoming connection on a listening socket. This function
 * extracts the first connection request on the queue of pending connections
 * and creates a new socket for the connection.
 * 
 * @param ns The listening network stream.
 * @return A valid network stream for the new connection on success, or NULL on failure.
 */
fossil_nstream_t *fossil_nstream_accept(fossil_nstream_t *ns);

/**
 * Close the network stream. This function releases the resources associated with the
 * network stream and closes the connection.
 * 
 * @param ns The network stream to close.
 */
void fossil_nstream_close(fossil_nstream_t *ns);

/**
 * Set the socket to non-blocking mode.
 * 
 * @param ns The network stream to modify.
 * @param enable 1 to enable non-blocking mode, 0 to disable it.
 * @return 0 on success, -1 on failure.
 */
int fossil_nstream_set_nonblocking(fossil_nstream_t *ns, int enable);

/**
 * Wait for the socket to become readable.
 * 
 * @param ns The network stream to wait on.
 * @param timeout_ms The timeout in milliseconds.
 * @return 0 on success, -1 on failure, or 1 if the timeout occurred.
 */
int fossil_nstream_wait_readable(fossil_nstream_t *ns, int timeout_ms);

/**
 * Wait for the socket to become writable.
 * 
 * @param ns The network stream to wait on.
 * @param timeout_ms The timeout in milliseconds.
 * @return 0 on success, -1 on failure, or 1 if the timeout occurred.
 */
int fossil_nstream_wait_writable(fossil_nstream_t *ns, int timeout_ms);

/**
 * Connect to a remote server. This function establishes a connection to a
 * specified IP address and port.
 * 
 * @param ns The network stream to use for the connection.
 * @param host The hostname or IP address of the remote server.
 * @param port The port number of the remote server.
 * @param timeout_ms The timeout in milliseconds.
 * @return 0 on success, -1 on failure.
 */
int fossil_nstream_connect_timeout(fossil_nstream_t *ns, const char *host, const char *port, int timeout_ms);

/**
 * Get the IP address and port of the connected peer.
 * 
 * @param ns The network stream to query.
 * @param ip_str A buffer to store the IP address as a string.
 * @param ip_len The length of the IP address buffer.
 * @param port A pointer to store the port number.
 * @return 0 on success, -1 on failure.
 */
int fossil_nstream_get_peer_info(fossil_nstream_t *ns, char *ip_str, size_t ip_len, uint16_t *port);

/**
 * Get the local IP address and port of the socket.
 * 
 * @param ns The network stream to query.
 * @param ip_str A buffer to store the local IP address as a string.
 * @param ip_len The length of the IP address buffer.
 * @param port A pointer to store the local port number.
 * @return 0 on success, -1 on failure.
 */
ssize_t fossil_nstream_send_line(fossil_nstream_t *ns, const char *line);

/**
 * Receive a line of text from the network stream. This function reads data
 * until a newline character is encountered.
 * 
 * @param ns The network stream to receive data from.
 * @param buf The buffer to store the received line.
 * @param max_len The maximum length of the buffer.
 * @return The number of bytes received, or -1 on failure.
 */
ssize_t fossil_nstream_recv_line(fossil_nstream_t *ns, char *buf, size_t max_len);

/**
 * Send data over a TLS connection.
 * 
 * @param ns The network stream to send data through.
 * @param buf The buffer containing the data to send.
 * @param len The length of the data to send.
 * @return The number of bytes sent, or -1 on failure.
 */
ssize_t fossil_nstream_ssl_send(fossil_nstream_t *ns, const void *buf, size_t len);

/**
 * Receive data over a TLS connection.
 * 
 * @param ns The network stream to receive data from.
 * @param buf The buffer to store the received data.
 * @param len The maximum length of data to receive.
 * @return The number of bytes received, or -1 on failure.
 */
ssize_t fossil_nstream_ssl_recv(fossil_nstream_t *ns, void *buf, size_t len);

/**
 * Send data over a UDP socket.
 * 
 * @param ns The network stream to send data through.
 * @param buf The buffer containing the data to send.
 * @param len The length of the data to send.
 * @param ip The IP address of the destination.
 * @param port The port number of the destination.
 * @return The number of bytes sent, or -1 on failure.
 */
const char *fossil_nstream_strerror(void);

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
        class NStream {
        public:

            /**
             * Constructor for NStream.
             * 
             * @param protocol The protocol to use (e.g., "tcp", "udp").
             * @param host The hostname or IP address.
             * @param port The port number.
             * @param flags Additional flags (e.g., "server").
             */
            NStream(const std::string &protocol, const std::string &host, const std::string &port, const std::string &flags) {
                ns = fossil_nstream_open(protocol.c_str(), host.c_str(), port.c_str(), flags.c_str());
            }

            /**
             * Constructor for NStream with an existing socket.
             * 
             * @param socket The existing socket file descriptor.
             */
            ~NStream() {
                if (ns) {
                    fossil_nstream_close(ns);
                }
            }

            /**
             * Send data through the network stream.
             *
             * @param buf The buffer containing the data to send.
             * @param len The length of the data to send.
             * @return The number of bytes sent, or -1 on failure.
             */
            ssize_t send(const void *buf, size_t len) {
                return fossil_nstream_send(ns, buf, len);
            }

            /**
             * Receive data through the network stream.
             * 
             * @param buf The buffer to store the received data.
             * @param len The maximum length of data to receive.
             * @return The number of bytes received, or -1 on failure.
             */
            ssize_t recv(void *buf, size_t len) {
                return fossil_nstream_recv(ns, buf, len);
            }

            /**
             * Listen on a network stream (for servers).
             *
             * @param backlog The maximum length of the queue of pending connections.
             * @return 0 on success, -1 on failure.
             */
            int listen(int backlog) {
                return fossil_nstream_listen(ns, backlog);
            }

            /**
             * Accept a new incoming connection on a listening socket.
             * 
             * @return A valid network stream for the new connection on success, or nullptr on failure.
             */
            NStream *accept() {
                fossil_nstream_t *new_ns = fossil_nstream_accept(ns);
                return new_ns ? new NStream(new_ns) : nullptr;
            }

            /**
             * Set the socket to non-blocking mode.
             * 
             * @param enable 1 to enable non-blocking mode, 0 to disable it.
             * @return 0 on success, -1 on failure.
             */
            int set_non_blocking(int enable) {
                return fossil_nstream_set_nonblocking(ns, enable);
            }

            /**
             * Wait for the socket to become readable.
             * 
             * @param timeout_ms The timeout in milliseconds.
             * @return 0 on success, -1 on failure, or 1 if the timeout occurred.
             */
            int wait_readable(int timeout_ms) {
                return fossil_nstream_wait_readable(ns, timeout_ms);
            }

            /**
             * Wait for the socket to become writable.
             * 
             * @param timeout_ms The timeout in milliseconds.
             * @return 0 on success, -1 on failure, or 1 if the timeout occurred.
             */
            int wait_writable(int timeout_ms) {
                return fossil_nstream_wait_writable(ns, timeout_ms);
            }

            /**
             * Connect to a remote server with a timeout.
             * 
             * @param host The hostname or IP address of the remote server.
             * @param port The port number of the remote server.
             * @param timeout_ms The timeout in milliseconds.
             * @return 0 on success, -1 on failure.
             */
            int connect_timeout(const std::string &host, const std::string &port, int timeout_ms) {
                return fossil_nstream_connect_timeout(ns, host.c_str(), port.c_str(), timeout_ms);
            }

            /**
             * Get the IP address and port of the connected peer.
             * 
             * @param ip_str A string to store the IP address.
             * @param port A reference to store the port number.
             * @return 0 on success, -1 on failure.
             */
            int get_peer_info(std::string &ip_str, uint16_t &port) {
                char ip_buf[INET_ADDRSTRLEN];
                int result = fossil_nstream_get_peer_info(ns, ip_buf, sizeof(ip_buf), &port);
                if (result == 0) {
                    ip_str = ip_buf;
                }
                return result;
            }

            /**
             * Send a line of text through the network stream.
             * 
             * @param line The line of text to send.
             * @return The number of bytes sent, or -1 on failure.
             */
            ssize_t send_line(const std::string &line) {
                return fossil_nstream_send_line(ns, line.c_str());
            }

            /**
             * Receive a line of text from the network stream.
             * 
             * @param buf A string to store the received line.
             * @param max_len The maximum length of the buffer.
             * @return The number of bytes received, or -1 on failure.
             */
            ssize_t recv_line(std::string &buf, size_t max_len) {
                char *temp_buf = new char[max_len];
                ssize_t result = fossil_nstream_recv_line(ns, temp_buf, max_len);
                if (result >= 0) {
                    buf.assign(temp_buf, result);
                }
                delete[] temp_buf;
                return result;
            }

            /**
             * Get a string describing the last error.
             * 
             * @return A string describing the last error.
             */
            const char *str_error() {
                return fossil_nstream_strerror();
            }

        private:
            fossil_nstream_t *ns;

            // Private constructor for internal use
            NStream(fossil_nstream_t *existing_ns) : ns(existing_ns) {}

        };
    }
}

#endif

#endif /* FOSSIL_IO_NETWORK_H */
