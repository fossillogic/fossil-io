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
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @file network.c
 * @brief Network logic implementation for handling communication protocols and client types.
 *
 * This file contains the core logic for managing network communication in the application.
 * It is responsible for implementing various protocols and handling different types of clients
 * that interact with the system. The following details outline the key aspects of the network
 * logic:
 *
 * ## Supported Protocols:
 * 1. **TCP (Transmission Control Protocol):**
 *    - Provides reliable, ordered, and error-checked delivery of data.
 *    - Used for persistent connections and critical data exchange.
 *    - Ensures data integrity and retransmission in case of packet loss.
 *
 * 2. **UDP (User Datagram Protocol):**
 *    - Provides connectionless communication with minimal overhead.
 *    - Suitable for real-time applications where speed is prioritized over reliability.
 *    - Commonly used for streaming, gaming, and other low-latency scenarios.
 *
 * 3. **HTTP/HTTPS:**
 *    - Supports web-based communication using the Hypertext Transfer Protocol.
 *    - HTTPS ensures secure communication via SSL/TLS encryption.
 *    - Used for RESTful APIs and web client interactions.
 *
 * 4. **WebSocket:**
 *    - Enables full-duplex communication channels over a single TCP connection.
 *    - Ideal for real-time applications such as chat systems and live updates.
 *
 * ## Client Types:
 * 1. **Standard Clients:**
 *    - General-purpose clients that use standard protocols like HTTP or WebSocket.
 *    - Typically include web browsers, mobile apps, and desktop applications.
 *
 * 2. **Embedded Clients:**
 *    - Lightweight clients running on embedded systems or IoT devices.
 *    - Often use UDP or custom lightweight protocols for communication.
 *
 * 3. **Service Clients:**
 *    - Backend services or microservices that interact with the system.
 *    - May use HTTP APIs, gRPC, or other service-to-service communication protocols.
 *
 * 4. **Real-Time Clients:**
 *    - Clients requiring low-latency communication, such as gaming or streaming applications.
 *    - Often rely on WebSocket or UDP for real-time data exchange.
 *
 * ## Supported Flags:
 * - **Protocol Flags:**
 *   - `tcp`: Specifies the use of the TCP protocol.
 *   - `udp`: Specifies the use of the UDP protocol.
 *   - `http`: Specifies the use of the HTTP protocol.
 *   - `https`: Specifies the use of the HTTPS protocol.
 *   - `raw`: Specifies the use of raw sockets.
 *   - `icmp`: Specifies the use of ICMP protocol.
 *   - `sctp`: Specifies the use of SCTP protocol.
 *   - `ftp`: Specifies the use of the FTP protocol.
 *   - `ssh`: Specifies the use of the SSH protocol.
 *   - `dns`: Specifies the use of the DNS protocol.
 *   - `ntp`: Specifies the use of the NTP protocol.
 *   - `smtp`: Specifies the use of the SMTP protocol.
 *   - `pop3`: Specifies the use of the POP3 protocol.
 *   - `imap`: Specifies the use of the IMAP protocol.
 *   - `ldap`: Specifies the use of the LDAP protocol.
 *   - `mqtt`: Specifies the use of the MQTT protocol.
 *
 * - **Client Type Flags:**
 *   - `mail-server`: Represents a mail server client.
 *   - `server`: Represents a general server client.
 *   - `mail-client`: Represents a mail client.
 *   - `client`: Represents a general client.
 *   - `mail-bot`: Represents an automated mail bot client.
 *   - `bot`: Represents a general bot client.
 *   - `multicast`: Represents a multicast client.
 *   - `broadcast`: Represents a broadcast client.
 *
 * ## Additional Notes:
 * - The implementation ensures scalability and fault tolerance for handling multiple clients
 *   simultaneously.
 * - Security measures, such as encryption and authentication, are integrated to protect
 *   communication channels.
 * - The file is designed to be modular, allowing easy extension for new protocols or client types.
 */

typedef struct fossil_nstream_t fossil_nstream_t;

typedef enum {
    FOSSIL_PROTO_TCP,
    FOSSIL_PROTO_UDP,
    FOSSIL_PROTO_RAW,
    FOSSIL_PROTO_ICMP,
    FOSSIL_PROTO_SCTP,
    FOSSIL_PROTO_HTTP,
    FOSSIL_PROTO_HTTPS,
    FOSSIL_PROTO_FTP,
    FOSSIL_PROTO_SSH,
    FOSSIL_PROTO_DNS,
    FOSSIL_PROTO_NTP,
    FOSSIL_PROTO_SMTP,
    FOSSIL_PROTO_POP3,
    FOSSIL_PROTO_IMAP,
    FOSSIL_PROTO_LDAP,
    FOSSIL_PROTO_MQTT,
    FOSSIL_PROTO_UNKNOWN
} fossil_protocol_t;

typedef enum {
    FOSSIL_CLIENT_MAIL_SERVER,
    FOSSIL_CLIENT_SERVER,
    FOSSIL_CLIENT_MAIL_CLIENT,
    FOSSIL_CLIENT_CLIENT,
    FOSSIL_CLIENT_MAIL_BOT,
    FOSSIL_CLIENT_BOT,
    FOSSIL_CLIENT_MULTICAST,
    FOSSIL_CLIENT_BROADCAST,
    FOSSIL_CLIENT_UNKNOWN
} fossil_client_type_t;

/**
 * Create a new network stream.
 *
 * @param protocol_flag The protocol to use (e.g., "tcp", "udp").
 * @param client_type_flag The type of client (e.g., "server", "client").
 * @return A pointer to the newly created network stream, or NULL on failure.
 */
fossil_nstream_t *fossil_nstream_create(const char *protocol_flag, const char *client_type_flag);

/**
 * Connect a network stream to a remote host.
 *
 * @param stream The network stream to connect.
 * @param host The hostname or IP address of the remote host.
 * @param port The port number to connect to.
 * @return 0 on success, or -1 on failure.
 */
int fossil_nstream_connect(fossil_nstream_t *stream, const char *host, int port);

/**
 * Set up a network stream to listen for incoming connections.
 *
 * @param stream The network stream to set up.
 * @param host The hostname or IP address to bind to.
 * @param port The port number to listen on.
 * @return 0 on success, or -1 on failure.
 */
int fossil_nstream_listen(fossil_nstream_t *stream, const char *host, int port);

/**
 * Accept a new incoming connection on a listening network stream.
 *
 * @param server The listening network stream.
 * @return A pointer to the new network stream for the accepted connection, or NULL on failure.
 */
fossil_nstream_t *fossil_nstream_accept(fossil_nstream_t *server);

/**
 * Send data through a network stream.
 *
 * @param stream The network stream to send data through.
 * @param buffer The buffer containing the data to send.
 * @param size The size of the data to send, in bytes.
 * @return The number of bytes sent, or -1 on failure.
 */
ssize_t fossil_nstream_send(fossil_nstream_t *stream, const void *buffer, size_t size);

/**
 * Set the SO_REUSEADDR option on a network stream's underlying socket.
 *
 * @param stream The network stream to configure.
 * @param enable Nonzero to enable SO_REUSEADDR, zero to disable.
 * @return 0 on success, or -1 on failure.
 */
int fossil_nstream_set_reuseaddr(fossil_nstream_t *stream, int enable);

/**
 * Receive data through a network stream.
 *
 * @param stream The network stream to receive data from.
 * @param buffer The buffer to store the received data.
 * @param size The maximum size of the buffer, in bytes.
 * @return The number of bytes received, or -1 on failure.
 */
ssize_t fossil_nstream_recv(fossil_nstream_t *stream, void *buffer, size_t size);

/**
 * Close a network stream.
 *
 * @param stream The network stream to close.
 */
void fossil_nstream_close(fossil_nstream_t *stream);

/**
 * Destroy a network stream and free its resources.
 *
 * @param stream The network stream to destroy.
 */
void fossil_nstream_destroy(fossil_nstream_t *stream);

/**
 * Get a string describing the last error that occurred.
 *
 * @return A string describing the last error.
 */
const char *fossil_nstream_last_error(void);

#ifdef __cplusplus
}
#include <stdexcept>
#include <string>

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
        /**
         * A C++ wrapper class for the Fossil network stream API.
         * Provides an object-oriented interface for managing network streams.
         */
        class NStream {
        public:
            /**
             * Constructor to create a new network stream.
             *
             * @param protocol_flag The protocol to use (e.g., "tcp", "udp").
             * @param client_type_flag The type of client (e.g., "server", "client").
             * @throws std::runtime_error If the stream creation fails.
             */
            NStream(const std::string &protocol_flag, const std::string &client_type_flag) {
                stream_ = fossil_nstream_create(protocol_flag.c_str(), client_type_flag.c_str());
                if (!stream_) {
                    throw std::runtime_error(fossil_nstream_last_error());
                }
            }

            /**
             * Destructor to destroy the network stream and free its resources.
             */
            ~NStream() {
                if (stream_) {
                    fossil_nstream_destroy(stream_);
                }
            }

            /**
             * Connect the network stream to a remote host.
             *
             * @param host The hostname or IP address of the remote host.
             * @param port The port number to connect to.
             * @throws std::runtime_error If the connection fails.
             */
            void connect(const std::string &host, int port) {
                if (fossil_nstream_connect(stream_, host.c_str(), port) != 0) {
                    throw std::runtime_error(fossil_nstream_last_error());
                }
            }

            /**
             * Set up the network stream to listen for incoming connections.
             *
             * @param host The hostname or IP address to bind to.
             * @param port The port number to listen on.
             * @throws std::runtime_error If the setup fails.
             */
            void listen(const std::string &host, int port) {
                if (fossil_nstream_listen(stream_, host.c_str(), port) != 0) {
                    throw std::runtime_error(fossil_nstream_last_error());
                }
            }

            /**
             * Accept a new incoming connection on a listening network stream.
             *
             * @return A pointer to a new NStream object for the accepted connection.
             * @throws std::runtime_error If accepting the connection fails.
             */
            NStream *accept() {
                fossil_nstream_t *accepted_stream = fossil_nstream_accept(stream_);
                if (!accepted_stream) {
                    throw std::runtime_error(fossil_nstream_last_error());
                }
                return new NStream(accepted_stream);
            }

            /**
             * Send data through the network stream.
             *
             * @param buffer The buffer containing the data to send.
             * @param size The size of the data to send, in bytes.
             * @return The number of bytes sent.
             * @throws std::runtime_error If sending the data fails.
             */
            ssize_t send(const void *buffer, size_t size) {
                ssize_t bytes_sent = fossil_nstream_send(stream_, buffer, size);
                if (bytes_sent < 0) {
                    throw std::runtime_error(fossil_nstream_last_error());
                }
                return bytes_sent;
            }

            /**
             * Receive data through the network stream.
             *
             * @param buffer The buffer to store the received data.
             * @param size The maximum size of the buffer, in bytes.
             * @return The number of bytes received.
             * @throws std::runtime_error If receiving the data fails.
             */
            ssize_t recv(void *buffer, size_t size) {
                ssize_t bytes_received = fossil_nstream_recv(stream_, buffer, size);
                if (bytes_received < 0) {
                    throw std::runtime_error(fossil_nstream_last_error());
                }
                return bytes_received;
            }

            /**
             * Close the network stream.
             */
            void close() {
                fossil_nstream_close(stream_);
            }

        private:
            /**
             * Private constructor to wrap an existing fossil_nstream_t object.
             *
             * @param stream A pointer to an existing fossil_nstream_t object.
             */
            NStream(fossil_nstream_t *stream) : stream_(stream) {}

            fossil_nstream_t *stream_; /**< Pointer to the underlying network stream. */
        };
    }
}

#endif

#endif /* FOSSIL_IO_NETWORK_H */
