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
 * Initialize the network stack. This function is necessary for Windows
 * platforms to set up the Winsock library. On other platforms, it may
 * perform other necessary initializations.
 * 
 * @return 0 on success, non-zero on failure.
 */
int fossil_io_network_create(void);

/**
 * Clean up the network stack. This function is necessary for Windows
 * platforms to clean up the Winsock library. On other platforms, it may
 * perform other necessary clean-up operations.
 */
void fossil_io_network_destroy(void);

/**
 * Create a new socket of the specified type (TCP or UDP).
 * 
 * @param type The type of socket to create (e.g., SOCK_STREAM for TCP, SOCK_DGRAM for UDP).
 * @return A valid socket on success, or FOSSIL_IO_INVALID_SOCKET on failure.
 */
fossil_io_socket_t fossil_io_network_create_socket(int type);

/**
 * Bind a socket to a specific IP address and port. This function associates
 * the socket with a local address so that it can listen for incoming connections
 * or send data.
 * 
 * @param sock The socket to bind.
 * @param ip The IP address to bind to (e.g., "127.0.0.1" for localhost).
 * @param port The port number to bind to.
 * @return 0 on success, -1 on failure.
 */
int fossil_io_network_bind(fossil_io_socket_t sock, const char *ip, uint16_t port);

/**
 * Listen for incoming connections on a bound socket. This function puts the
 * socket into a state where it can accept incoming connection requests.
 * 
 * @param sock The socket to listen on.
 * @param backlog The maximum length of the queue of pending connections.
 * @return 0 on success, -1 on failure.
 */
int fossil_io_network_listen(fossil_io_socket_t sock, int backlog);

/**
 * Accept a new incoming connection on a listening socket. This function
 * extracts the first connection request on the queue of pending connections
 * and creates a new socket for the connection.
 * 
 * @param sock The listening socket.
 * @param client_ip A buffer to store the IP address of the connecting client.
 * @param client_port A pointer to store the port number of the connecting client.
 * @return A valid socket for the new connection on success, or FOSSIL_IO_INVALID_SOCKET on failure.
 */
fossil_io_socket_t fossil_io_network_accept(fossil_io_socket_t sock, char *client_ip, uint16_t *client_port);

/**
 * Connect to a remote server. This function establishes a connection to a
 * specified IP address and port.
 * 
 * @param sock The socket to use for the connection.
 * @param ip The IP address of the remote server.
 * @param port The port number of the remote server.
 * @return 0 on success, -1 on failure.
 */
int fossil_io_network_connect(fossil_io_socket_t sock, const char *ip, uint16_t port);

/**
 * Send data over a connected socket. This function transmits data to the
 * remote peer connected to the socket.
 * 
 * @param sock The socket to use for sending data.
 * @param data A pointer to the data to be sent.
 * @param len The length of the data to be sent.
 * @return The number of bytes sent, or -1 on failure.
 */
int fossil_io_network_send(fossil_io_socket_t sock, const void *data, size_t len);

/**
 * Receive data from a connected socket. This function reads data from the
 * remote peer connected to the socket.
 * 
 * @param sock The socket to use for receiving data.
 * @param buffer A buffer to store the received data.
 * @param len The maximum length of data to be received.
 * @return The number of bytes received, or -1 on failure.
 */
int fossil_io_network_receive(fossil_io_socket_t sock, void *buffer, size_t len);

/**
 * Close a socket. This function releases the resources associated with the
 * socket and closes the connection.
 * 
 * @param sock The socket to be closed.
 */
void fossil_io_network_close(fossil_io_socket_t sock);

/**
 * Send data to a specific IP address and port using a UDP socket. This function
 * transmits data to the specified address and port without establishing a connection.
 * 
 * @param sock The socket to use for sending data.
 * @param data A pointer to the data to be sent.
 * @param len The length of the data to be sent.
 * @param ip The IP address of the destination.
 * @param port The port number of the destination.
 * @return The number of bytes sent, or -1 on failure.
 */
int fossil_io_network_sendto(fossil_io_socket_t sock, const void *data, size_t len, const char *ip, uint16_t port);

/**
 * Receive data from a specific IP address and port using a UDP socket. This function
 * reads data from the specified address and port without establishing a connection.
 * 
 * @param sock The socket to use for receiving data.
 * @param buffer A buffer to store the received data.
 * @param len The maximum length of data to be received.
 * @param ip A buffer to store the IP address of the sender.
 * @param port A pointer to store the port number of the sender.
 * @return The number of bytes received, or -1 on failure.
 */
int fossil_io_network_recvfrom(fossil_io_socket_t sock, void *buffer, size_t len, char *ip, uint16_t *port);

/**
 * Bridge function for network operations. This function can be used to
 * transfer data between two sockets, effectively bridging them.
 * 
 * @param sock1 The first socket.
 * @param sock2 The second socket.
 * @return 0 on success, -1 on failure.
 */
int fossil_io_network_bridge(fossil_io_socket_t sock1, fossil_io_socket_t sock2);

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
             * Initialize the network stack. This function is necessary for Windows
             * platforms to set up the Winsock library. On other platforms, it may
             * perform other necessary initializations.
             * 
             * @return 0 on success, non-zero on failure.
             */
            static int init(void) {
                return fossil_io_network_create();
            }

            /**
             * Clean up the network stack. This function is necessary for Windows
             * platforms to clean up the Winsock library. On other platforms, it may
             * perform other necessary clean-up operations.
             */
            static void cleanup(void) {
                fossil_io_network_destroy();
            }

            /**
             * Create a new socket of the specified type (TCP or UDP).
             * 
             * @param type The type of socket to create (e.g., SOCK_STREAM for TCP, SOCK_DGRAM for UDP).
             * @return A valid socket on success, or FOSSIL_IO_INVALID_SOCKET on failure.
             */
            static fossil_io_socket_t create_socket(int type) {
                return fossil_io_network_create_socket(type);
            }

            /**
             * Bind a socket to a specific IP address and port. This function associates
             * the socket with a local address so that it can listen for incoming connections
             * or send data.
             * 
             * @param sock The socket to bind.
             * @param ip The IP address to bind to (e.g., "127.0.0.1" for localhost).
             * @param port The port number to bind to.
             * @return 0 on success, -1 on failure.
             */
            static int bind(fossil_io_socket_t sock, const char *ip, uint16_t port) {
                return fossil_io_network_bind(sock, ip, port);
            }

            /**
             * Listen for incoming connections on a bound socket. This function puts the
             * socket into a state where it can accept incoming connection requests.
             * 
             * @param sock The socket to listen on.
             * @param backlog The maximum length of the queue of pending connections.
             * @return 0 on success, -1 on failure.
             */
            static int listen(fossil_io_socket_t sock, int backlog) {
                return fossil_io_network_listen(sock, backlog);
            }

            /**
             * Accept a new incoming connection on a listening socket. This function
             * extracts the first connection request on the queue of pending connections
             * and creates a new socket for the connection.
             * 
             * @param sock The listening socket.
             * @param client_ip A buffer to store the IP address of the connecting client.
             * @param client_port A pointer to store the port number of the connecting client.
             * @return A valid socket for the new connection on success, or FOSSIL_IO_INVALID_SOCKET on failure.
             */
            static fossil_io_socket_t accept(fossil_io_socket_t sock, char *client_ip, uint16_t *client_port) {
                return fossil_io_network_accept(sock, client_ip, client_port);
            }

            /**
             * Connect to a remote server. This function establishes a connection to a
             * specified IP address and port.
             * 
             * @param sock The socket to use for the connection.
             * @param ip The IP address of the remote server.
             * @param port The port number of the remote server.
             * @return 0 on success, -1 on failure.
             */
            static int connect(fossil_io_socket_t sock, const char *ip, uint16_t port) {
                return fossil_io_network_connect(sock, ip, port);
            }

            /**
             * Send data over a connected socket. This function transmits data to the
             * remote peer connected to the socket.
             * 
             * @param sock The socket to use for sending data.
             * @param data A pointer to the data to be sent.
             * @param len The length of the data to be sent.
             * @return The number of bytes sent, or -1 on failure.
             */
            static int send(fossil_io_socket_t sock, const void *data, size_t len) {
                return fossil_io_network_send(sock, data, len);
            }

            /**
             * Receive data from a connected socket. This function reads data from the
             * remote peer connected to the socket.
             * 
             * @param sock The socket to use for receiving data.
             * @param buffer A buffer to store the received data.
             * @param len The maximum length of data to be received.
             * @return The number of bytes received, or -1 on failure.
             */
            static int receive(fossil_io_socket_t sock, void *buffer, size_t len) {
                return fossil_io_network_receive(sock, buffer, len);
            }

            /**
             * Close a socket. This function releases the resources associated with the
             * socket and closes the connection.
             * 
             * @param sock The socket to be closed.
             */
            static void close(fossil_io_socket_t sock) {
                fossil_io_network_close(sock);
            }

            /**
             * Send data to a specific IP address and port using a UDP socket. This function
             * transmits data to the specified address and port without establishing a connection.
             * 
             * @param sock The socket to use for sending data.
             * @param data A pointer to the data to be sent.
             * @param len The length of the data to be sent.
             * @param ip The IP address of the destination.
             * @param port The port number of the destination.
             * @return The number of bytes sent, or -1 on failure.
             */
            static int sendto(fossil_io_socket_t sock, const void *data, size_t len, const char *ip, uint16_t port) {
                return fossil_io_network_sendto(sock, data, len, ip, port);
            }

            /**
             * Receive data from a specific IP address and port using a UDP socket. This function
             * reads data from the specified address and port without establishing a connection.
             * 
             * @param sock The socket to use for receiving data.
             * @param buffer A buffer to store the received data.
             * @param len The maximum length of data to be received.
             * @param ip A buffer to store the IP address of the sender.
             * @param port A pointer to store the port number of the sender.
             * @return The number of bytes received, or -1 on failure.
             */
            static int recvfrom(fossil_io_socket_t sock, void *buffer, size_t len, char *ip, uint16_t *port) {
                return fossil_io_network_recvfrom(sock, buffer, len, ip, port);
            }

            /**
             * Bridge function for network operations. This function can be used to
             * transfer data between two sockets, effectively bridging them.
             * 
             * @param sock1 The first socket.
             * @param sock2 The second socket.
             * @return 0 on success, -1 on failure.
             */
            static int bridge(fossil_io_socket_t sock1, fossil_io_socket_t sock2) {
                return fossil_io_network_bridge(sock1, sock2);
            }

        };
    }
}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
