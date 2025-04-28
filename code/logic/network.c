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
#include <errno.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
typedef SOCKET socket_t;
#else
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
typedef int socket_t;
#endif

struct fossil_nstream_t {
    socket_t socket_fd;
    fossil_protocol_t protocol;
    fossil_client_type_t client_type;
    char protocol_flag[32];
    char client_type_flag[32];
    int is_connected;
    int is_server;
};

static char fossil_last_error[256] = {0};

static void fossil_set_last_error(const char *msg) {
    snprintf(fossil_last_error, sizeof(fossil_last_error), "%s", msg);
}

const char *fossil_nstream_last_error(void) {
    return fossil_last_error;
}

static int fossil_socket_init() {
#ifdef _WIN32
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2,2), &wsa);
#else
    return 0;
#endif
}

static void fossil_socket_cleanup() {
#ifdef _WIN32
    WSACleanup();
#endif
}

typedef struct {
    const char *name;
    fossil_protocol_t proto;
} proto_entry_t;

typedef struct {
    const char *name;
    fossil_client_type_t type;
} client_entry_t;

static const proto_entry_t proto_table[] = {
    {"tcp", FOSSIL_PROTO_TCP},
    {"udp", FOSSIL_PROTO_UDP},
    {"raw", FOSSIL_PROTO_RAW},
    {"icmp", FOSSIL_PROTO_ICMP},
    {"sctp", FOSSIL_PROTO_SCTP},
    {"http", FOSSIL_PROTO_HTTP},
    {"https", FOSSIL_PROTO_HTTPS},
    {"ftp", FOSSIL_PROTO_FTP},
    {"ssh", FOSSIL_PROTO_SSH},
    {"dns", FOSSIL_PROTO_DNS},
    {"ntp", FOSSIL_PROTO_NTP},
    {"smtp", FOSSIL_PROTO_SMTP},
    {"pop3", FOSSIL_PROTO_POP3},
    {"imap", FOSSIL_PROTO_IMAP},
    {"ldap", FOSSIL_PROTO_LDAP},
    {"mqtt", FOSSIL_PROTO_MQTT},
    {NULL, FOSSIL_PROTO_UNKNOWN}
};

static const client_entry_t client_table[] = {
    {"mail-server", FOSSIL_CLIENT_MAIL_SERVER},
    {"server", FOSSIL_CLIENT_SERVER},
    {"mail-client", FOSSIL_CLIENT_MAIL_CLIENT},
    {"client", FOSSIL_CLIENT_CLIENT},
    {"mail-bot", FOSSIL_CLIENT_MAIL_BOT},
    {"bot", FOSSIL_CLIENT_BOT},
    {"multicast", FOSSIL_CLIENT_MULTICAST},
    {"broadcast", FOSSIL_CLIENT_BROADCAST},
    {NULL, FOSSIL_CLIENT_UNKNOWN}
};

fossil_protocol_t fossil_protocol_from_string(const char *str) {
    if (!str) return FOSSIL_PROTO_UNKNOWN;
    for (int i = 0; proto_table[i].name; ++i) {
        if (strncmp(str, proto_table[i].name, strlen(proto_table[i].name)) == 0)
            return proto_table[i].proto;
    }
    return FOSSIL_PROTO_UNKNOWN;
}

fossil_client_type_t fossil_client_type_from_string(const char *str) {
    if (!str) return FOSSIL_CLIENT_UNKNOWN;
    for (int i = 0; client_table[i].name; ++i) {
        if (strncmp(str, client_table[i].name, strlen(client_table[i].name)) == 0)
            return client_table[i].type;
    }
    return FOSSIL_CLIENT_UNKNOWN;
}

const char *fossil_protocol_to_string(fossil_protocol_t proto) {
    for (int i = 0; proto_table[i].name; ++i) {
        if (proto_table[i].proto == proto)
            return proto_table[i].name;
    }
    return "unknown";
}

const char *fossil_client_type_to_string(fossil_client_type_t type) {
    for (int i = 0; client_table[i].name; ++i) {
        if (client_table[i].type == type)
            return client_table[i].name;
    }
    return "unknown";
}

fossil_nstream_t *fossil_nstream_create(const char *protocol_flag, const char *client_type_flag) {
    if (fossil_socket_init() != 0) {
        fossil_set_last_error("Socket system initialization failed");
        return NULL;
    }
    
    if (!protocol_flag || !client_type_flag) {
        fossil_set_last_error("Invalid protocol or client type flag");
        return NULL;
    }
    
    fossil_protocol_t protocol = fossil_protocol_from_string(protocol_flag);
    fossil_client_type_t client_type = fossil_client_type_from_string(client_type_flag);
    
    if (protocol == FOSSIL_PROTO_UNKNOWN) {
        fossil_set_last_error("Unsupported protocol");
        return NULL;
    }
    
    if (client_type == FOSSIL_CLIENT_UNKNOWN) {
        fossil_set_last_error("Unsupported client type");
        return NULL;
    }
    
    fossil_nstream_t *stream = (fossil_nstream_t *)calloc(1, sizeof(fossil_nstream_t));
    if (!stream) {
        fossil_set_last_error("Memory allocation failed");
        return NULL;
    }
    
    stream->protocol = protocol;
    stream->client_type = client_type;
    snprintf(stream->protocol_flag, sizeof(stream->protocol_flag), "%s", protocol_flag);
    snprintf(stream->client_type_flag, sizeof(stream->client_type_flag), "%s", client_type_flag);
    stream->socket_fd = -1;
    return stream;
}

static socket_t fossil_create_socket(fossil_protocol_t proto) {
    int type = SOCK_STREAM;
    int proto_num = IPPROTO_TCP;
    
    switch (proto) {
        case FOSSIL_PROTO_TCP: type = SOCK_STREAM; proto_num = IPPROTO_TCP; break;
        case FOSSIL_PROTO_UDP: type = SOCK_DGRAM; proto_num = IPPROTO_UDP; break;
        case FOSSIL_PROTO_RAW: type = SOCK_RAW; proto_num = IPPROTO_RAW; break;
        case FOSSIL_PROTO_ICMP: type = SOCK_RAW; proto_num = IPPROTO_ICMP; break;
        case FOSSIL_PROTO_SCTP: type = SOCK_STREAM; proto_num = IPPROTO_SCTP; break;
        case FOSSIL_PROTO_HTTP: type = SOCK_STREAM; proto_num = IPPROTO_TCP; break;
        case FOSSIL_PROTO_HTTPS: type = SOCK_STREAM; proto_num = IPPROTO_TCP; break;
        case FOSSIL_PROTO_FTP: type = SOCK_STREAM; proto_num = IPPROTO_TCP; break;
        case FOSSIL_PROTO_SSH: type = SOCK_STREAM; proto_num = IPPROTO_TCP; break;
        case FOSSIL_PROTO_DNS: type = SOCK_DGRAM; proto_num = IPPROTO_UDP; break;
        case FOSSIL_PROTO_NTP: type = SOCK_DGRAM; proto_num = IPPROTO_UDP; break;
        case FOSSIL_PROTO_SMTP: type = SOCK_STREAM; proto_num = IPPROTO_TCP; break;
        case FOSSIL_PROTO_POP3: type = SOCK_STREAM; proto_num = IPPROTO_TCP; break;
        case FOSSIL_PROTO_IMAP: type = SOCK_STREAM; proto_num = IPPROTO_TCP; break;
        case FOSSIL_PROTO_LDAP: type = SOCK_STREAM; proto_num = IPPROTO_TCP; break;
        case FOSSIL_PROTO_MQTT: type = SOCK_STREAM; proto_num = IPPROTO_TCP; break;
        default:
            fossil_set_last_error("Unsupported protocol for socket creation");
            return -1;
    }
    
    return socket(AF_INET, type, proto_num);
}

int fossil_nstream_connect(fossil_nstream_t *stream, const char *host, int port) {
    if (!stream) {
        fossil_set_last_error("Stream is NULL");
        return -1;
    }
    
    stream->socket_fd = fossil_create_socket(stream->protocol);
    if (stream->socket_fd < 0) {
        return -1;
    }
    
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, host, &addr.sin_addr) <= 0) {
        fossil_set_last_error("Invalid address or address not supported");
        return -1;
    }
    
    if (connect(stream->socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        fossil_set_last_error("Connection to the server failed");
        return -1;
    }
    
    stream->is_connected = 1;
    return 0;
}

int fossil_nstream_listen(fossil_nstream_t *stream, const char *host, int port) {
    if (!stream) {
        fossil_set_last_error("Stream is NULL");
        return -1;
    }
    
    stream->socket_fd = fossil_create_socket(stream->protocol);
    if (stream->socket_fd < 0) {
        return -1;
    }
    
    int opt = 1;
    if (setsockopt(stream->socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        fossil_set_last_error("Failed to set socket options");
        return -1;
    }
    
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = (host == NULL) ? INADDR_ANY : inet_addr(host);
    
    if (bind(stream->socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        fossil_set_last_error("Bind failed");
        return -1;
    }
    
    if (listen(stream->socket_fd, SOMAXCONN) < 0) {
        fossil_set_last_error("Listen failed");
        return -1;
    }
    
    stream->is_server = 1;
    return 0;
}

fossil_nstream_t *fossil_nstream_accept(fossil_nstream_t *server) {
    if (!server || !server->is_server) {
        fossil_set_last_error("Invalid server stream");
        return NULL;
    }
    
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    socket_t client_fd = accept(server->socket_fd, (struct sockaddr *)&addr, &addrlen);
    if (client_fd < 0) {
#ifdef _WIN32
        if (WSAGetLastError() != WSAEWOULDBLOCK) {
#else
        if (errno != EWOULDBLOCK && errno != EAGAIN) {
#endif
            fossil_set_last_error("Accept failed");
        }
        return NULL;
    }
    
    fossil_nstream_t *client = (fossil_nstream_t *)calloc(1, sizeof(fossil_nstream_t));
    if (!client) {
        fossil_set_last_error("Memory allocation failed");
#ifdef _WIN32
        closesocket(client_fd);
#else
        close(client_fd);
#endif
        return NULL;
    }
    
    *client = *server;
    client->socket_fd = client_fd;
    client->is_server = 0;
    client->is_connected = 1;
    
    return client;
}

ssize_t fossil_nstream_send(fossil_nstream_t *stream, const void *buffer, size_t size) {
    if (!stream || stream->socket_fd < 0) {
        fossil_set_last_error("Invalid stream or socket");
        return -1;
    }
#ifdef _WIN32
    ssize_t sent_bytes = send(stream->socket_fd, (const char *)buffer, (int)size, 0);
#else
    ssize_t sent_bytes = send(stream->socket_fd, buffer, size, 0);
#endif
    if (sent_bytes < 0) {
        fossil_set_last_error("Failed to send data");
    }
    return sent_bytes;
}

ssize_t fossil_nstream_recv(fossil_nstream_t *stream, void *buffer, size_t size) {
    if (!stream || stream->socket_fd < 0) {
        fossil_set_last_error("Invalid stream or socket");
        return -1;
    }
#ifdef _WIN32
    ssize_t received_bytes = recv(stream->socket_fd, (char *)buffer, (int)size, 0);
#else
    ssize_t received_bytes = recv(stream->socket_fd, buffer, size, 0);
#endif
    if (received_bytes < 0) {
        fossil_set_last_error("Failed to receive data");
    }
    return received_bytes;
}

void fossil_nstream_close(fossil_nstream_t *stream) {
    if (!stream || stream->socket_fd < 0) return;
#ifdef _WIN32
    closesocket(stream->socket_fd);
#else
    close(stream->socket_fd);
#endif
    stream->socket_fd = -1;
    stream->is_connected = 0;
}

void fossil_nstream_destroy(fossil_nstream_t *stream) {
    if (!stream) return;
    fossil_nstream_close(stream);
    free(stream);
    fossil_socket_cleanup();
}
