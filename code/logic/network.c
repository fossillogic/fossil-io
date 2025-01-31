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
#include <string.h>

#ifdef _WIN32
    static WSADATA wsa;
#endif

int fossil_io_network_init(void) {
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        fprintf(stderr, "WSAStartup failed with error: %d\n", WSAGetLastError());
        return -1;
    }
#endif
    return 0;
}

void fossil_io_network_cleanup(void) {
#ifdef _WIN32
    WSACleanup();
#endif
}

fossil_io_socket_t fossil_io_network_create_socket(void) {
    fossil_io_socket_t sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == FOSSIL_IO_INVALID_SOCKET) {
        fprintf(stderr, "Socket creation failed with error: %d\n", WSAGetLastError());
    }
    return sock;
}

int fossil_io_network_bind(fossil_io_socket_t sock, const char *ip, uint16_t port) {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = ip ? inet_addr(ip) : INADDR_ANY;

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Bind failed with error: %d\n", WSAGetLastError());
        return -1;
    }
    return 0;
}

int fossil_io_network_listen(fossil_io_socket_t sock, int backlog) {
    if (listen(sock, backlog) == SOCKET_ERROR) {
        fprintf(stderr, "Listen failed with error: %d\n", WSAGetLastError());
        return -1;
    }
    return 0;
}

fossil_io_socket_t fossil_io_network_accept(fossil_io_socket_t sock, char *client_ip, uint16_t *client_port) {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    fossil_io_socket_t client_sock = accept(sock, (struct sockaddr*)&client_addr, &addr_len);

    if (client_sock == FOSSIL_IO_INVALID_SOCKET) {
        fprintf(stderr, "Accept failed with error: %d\n", WSAGetLastError());
        return FOSSIL_IO_INVALID_SOCKET;
    }

    if (client_ip) {
        strcpy(client_ip, inet_ntoa(client_addr.sin_addr));
        if (client_port) {
            *client_port = ntohs(client_addr.sin_port);
        }
    }

    return client_sock;
}

int fossil_io_network_connect(fossil_io_socket_t sock, const char *ip, uint16_t port) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Connect failed with error: %d\n", WSAGetLastError());
        return -1;
    }
    return 0;
}

int fossil_io_network_send(fossil_io_socket_t sock, const void *data, size_t len) {
    int bytes_sent = send(sock, data, (int)len, 0);
    if (bytes_sent == SOCKET_ERROR) {
        fprintf(stderr, "Send failed with error: %d\n", WSAGetLastError());
        return -1;
    }
    return bytes_sent;
}

int fossil_io_network_receive(fossil_io_socket_t sock, void *buffer, size_t len) {
    int bytes_received = recv(sock, buffer, (int)len, 0);
    if (bytes_received == SOCKET_ERROR) {
        fprintf(stderr, "Receive failed with error: %d\n", WSAGetLastError());
        return -1;
    }
    return bytes_received;
}

void fossil_io_network_close(fossil_io_socket_t sock) {
    if (closesocket(sock) == SOCKET_ERROR) {
        fprintf(stderr, "Close socket failed with error: %d\n", WSAGetLastError());
    }
}
