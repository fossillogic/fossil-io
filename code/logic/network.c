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
#include "fossil/io/network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>    // For struct timeval
#include <unistd.h>      // For close on POSIX systems

#ifdef _WIN32
#define close_socket(s) closesocket(s)
#else
#define close_socket(s) close(s)
#endif

// Helper function to resolve the port number
static int resolve_port(const char *port_str) {
    if (!port_str || !*port_str) return -1;

    // Make sure the string is numeric
    for (const char *p = port_str; *p; ++p) {
        if (!isdigit((unsigned char)*p)) return -1;
    }

    int port = atoi(port_str);
    if (port <= 0 || port > 65535) return -1;
    return port;
}

// Initialize socket (for both client and server)
static int init_socket(const char *protocol) {
    if (!protocol) return -1;

    struct {
        const char *name;
        int type;
        int proto;
    } protocols[] = {
        { "tcp",  SOCK_STREAM, 0 },
        { "udp",  SOCK_DGRAM,  0 },
        { "raw",  SOCK_RAW,    IPPROTO_RAW },
        { "icmp", SOCK_RAW,    IPPROTO_ICMP },
    };

    for (size_t i = 0; i < sizeof(protocols)/sizeof(protocols[0]); ++i) {
        if (strcmp(protocol, protocols[i].name) == 0) {
            int sock = socket(AF_INET, protocols[i].type, protocols[i].proto);
            return sock >= 0 ? sock : -1;
        }
    }

    return -1; // Unsupported protocol
}

// Open a new network stream (TCP/UDP)
fossil_nstream_t *fossil_nstream_open(const char *protocol, const char *host, const char *port, const char *flags) {
    fossil_nstream_t *ns = malloc(sizeof(fossil_nstream_t));
    if (!ns) return NULL;

    ns->socket = init_socket(protocol);
    if (ns->socket < 0) {
        free(ns);
        return NULL;
    }

    ns->is_tls = (flags && strstr(flags, "tls") != NULL);  // Check if we need TLS (simplified)

    memset(&ns->addr, 0, sizeof(ns->addr));
    ns->addr.sin_family = AF_INET;
    ns->addr.sin_port = htons(resolve_port(port));
#ifdef _WIN32
    ns->addr.sin_addr.s_addr = inet_addr(host);
#else
    if (inet_pton(AF_INET, host, &ns->addr.sin_addr) <= 0) {
        close_socket(ns->socket);
        free(ns);
        return NULL;
    }
#endif

    strncpy(ns->protocol, protocol, sizeof(ns->protocol) - 1);
    ns->protocol[sizeof(ns->protocol) - 1] = '\0';

    // Optionally bind socket for server
    if (flags && strstr(flags, "server") != NULL) {
        if (bind(ns->socket, (struct sockaddr *)&ns->addr, sizeof(ns->addr)) < 0) {
            close_socket(ns->socket);
            free(ns);
            return NULL;
        }
    }

    return ns;
}

// Send data through the network stream
ssize_t fossil_nstream_send(fossil_nstream_t *ns, const void *buf, size_t len) {
    if (!ns || ns->socket < 0 || !buf || len == 0) return -1;

    if (strcmp(ns->protocol, "udp") == 0 || strcmp(ns->protocol, "raw") == 0 || strcmp(ns->protocol, "icmp") == 0) {
        return sendto(ns->socket, buf, len, 0, (struct sockaddr *)&ns->addr, sizeof(ns->addr));
    }

    return send(ns->socket, buf, len, 0);
}

// Receive data through the network stream
ssize_t fossil_nstream_recv(fossil_nstream_t *ns, void *buf, size_t len) {
    if (strcmp(ns->protocol, "udp") == 0) {
        socklen_t addr_len = sizeof(ns->addr);
        return recvfrom(ns->socket, buf, len, 0, (struct sockaddr *)&ns->addr, &addr_len);
    } else if (strcmp(ns->protocol, "raw") == 0 || strcmp(ns->protocol, "icmp") == 0) {
        socklen_t addr_len = sizeof(ns->addr);
        return recvfrom(ns->socket, buf, len, 0, (struct sockaddr *)&ns->addr, &addr_len);
    }
    return recv(ns->socket, buf, len, 0);
}

// Listen on a network stream (for servers)
int fossil_nstream_listen(fossil_nstream_t *ns, int backlog) {
    if (strcmp(ns->protocol, "tcp") == 0) {
        return listen(ns->socket, backlog);
    }
    return -1;  // Listen only supports TCP
}

// Accept an incoming connection on a server
fossil_nstream_t *fossil_nstream_accept(fossil_nstream_t *ns) {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_sock = accept(ns->socket, (struct sockaddr *)&client_addr, &addr_len);
    
    if (client_sock < 0) return NULL;

    fossil_nstream_t *client_ns = calloc(1, sizeof(fossil_nstream_t));  // safer
    if (!client_ns) return NULL;

    client_ns->socket = client_sock;
    client_ns->addr = client_addr;
    strncpy(client_ns->protocol, ns->protocol, sizeof(client_ns->protocol) - 1);
    client_ns->protocol[sizeof(client_ns->protocol) - 1] = '\0';
    client_ns->is_tls = ns->is_tls;  // inherit TLS flag

    return client_ns;
}

// Close the network stream
void fossil_nstream_close(fossil_nstream_t *ns) {
    close_socket(ns->socket);
    free(ns);
}

// Set socket to non-blocking mode
int fossil_nstream_set_nonblocking(fossil_nstream_t *ns, int enable) {
    if (!ns || ns->socket < 0) return -1;
#ifdef _WIN32
    u_long mode = enable ? 1 : 0;
    return ioctlsocket(ns->socket, FIONBIO, &mode);
#else
    int flags = fcntl(ns->socket, F_GETFL, 0);
    if (flags == -1) return -1;

    if (enable) flags |= O_NONBLOCK;
    else flags &= ~O_NONBLOCK;

    return fcntl(ns->socket, F_SETFL, flags);
#endif
}

// Wait for the socket to become readable
int fossil_nstream_wait_readable(fossil_nstream_t *ns, int timeout_ms) {
    struct timeval timeout;
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(ns->socket, &readfds);

    return select(ns->socket + 1, &readfds, NULL, NULL, &timeout);
}

// Wait for the socket to become writable
int fossil_nstream_wait_writable(fossil_nstream_t *ns, int timeout_ms) {
    struct timeval timeout;
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;

    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(ns->socket, &writefds);

    return select(ns->socket + 1, NULL, &writefds, NULL, &timeout);
}

// Connect with timeout
int fossil_nstream_connect_timeout(fossil_nstream_t *ns, const char *host, const char *port, int timeout_ms) {
    if (!ns || ns->socket < 0 || !host || !port) return -1;

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(resolve_port(port));

    in_addr_t addr = inet_addr(host);
    if (addr == INADDR_NONE) return -1;  // Invalid address

    server_addr.sin_addr.s_addr = addr;

    // Set non-blocking
    if (fossil_nstream_set_nonblocking(ns, 1) != 0) return -1;

    int result = connect(ns->socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (result < 0) {
#ifdef _WIN32
        int last_error = WSAGetLastError();
        if (last_error == WSAEWOULDBLOCK) {
#else
        if (errno == EINPROGRESS) {
#endif
            fd_set writefds;
            FD_ZERO(&writefds);
            FD_SET(ns->socket, &writefds);

            struct timeval timeout;
            timeout.tv_sec = timeout_ms / 1000;
            timeout.tv_usec = (timeout_ms % 1000) * 1000;

            result = select(ns->socket + 1, NULL, &writefds, NULL, &timeout);
            if (result <= 0) {
                fossil_nstream_set_nonblocking(ns, 0);
                return -1;  // Timeout or select error
            }

            // Check if there was a socket error
            int so_error = 0;
            socklen_t len = sizeof(so_error);
            getsockopt(ns->socket, SOL_SOCKET, SO_ERROR, (char *)&so_error, &len);
            if (so_error != 0) {
                fossil_nstream_set_nonblocking(ns, 0);
                return -1;
            }
        } else {
            fossil_nstream_set_nonblocking(ns, 0);
            return -1;
        }
    }

    // Set back to blocking
    fossil_nstream_set_nonblocking(ns, 0);
    return 0;
}

// Get peer information (client's IP and port)
int fossil_nstream_get_peer_info(fossil_nstream_t *ns, char *ip_str, size_t ip_len, uint16_t *port) {
    struct sockaddr_in peer_addr;
    socklen_t addr_len = sizeof(peer_addr);
    if (getpeername(ns->socket, (struct sockaddr *)&peer_addr, &addr_len) == -1) return -1;

    inet_ntop(AF_INET, &peer_addr.sin_addr, ip_str, ip_len);
    *port = ntohs(peer_addr.sin_port);
    return 0;
}

// Send a line (e.g., for SMTP/IMAP)
ssize_t fossil_nstream_send_line(fossil_nstream_t *ns, const char *line) {
    size_t len = strlen(line);
    char *buffer = malloc(len + 3);  // \r\n
    if (!buffer) return -1;

    sprintf(buffer, "%s\r\n", line);
    ssize_t result = fossil_nstream_send(ns, buffer, len + 2);

    free(buffer);
    return result;
}

// Receive a line (e.g., for SMTP/IMAP)
ssize_t fossil_nstream_recv_line(fossil_nstream_t *ns, char *buf, size_t max_len) {
    ssize_t bytes_received = 0;
    char ch;
    while ((size_t)bytes_received < max_len - 1) {
        if (fossil_nstream_recv(ns, &ch, 1) <= 0) return -1;

        buf[bytes_received++] = ch;
        if (ch == '\n') break;
    }

    buf[bytes_received] = '\0';
    return bytes_received;
}

// SSL/TLS send wrapper
ssize_t fossil_nstream_ssl_send(fossil_nstream_t *ns, const void *buf, size_t len) {
    // TLS handling code here (simplified for now)
    return send(ns->socket, buf, len, 0);
}

// SSL/TLS receive wrapper
ssize_t fossil_nstream_ssl_recv(fossil_nstream_t *ns, void *buf, size_t len) {
    // TLS handling code here (simplified for now)
    return recv(ns->socket, buf, len, 0);
}

// Get the string representation of the last error
const char *fossil_nstream_strerror(void) {
#ifdef _WIN32
    static char buf[64];
    snprintf(buf, sizeof(buf), "WSA Error: %d", WSAGetLastError());
    return buf;
#else
    return strerror(errno);
#endif
}
