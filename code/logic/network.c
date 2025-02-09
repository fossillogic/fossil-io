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
#include <errno.h>  // For POSIX error handling
#include <stdlib.h> // For exit()

#ifdef _WIN32
    static WSADATA wsa;
#endif

int fossil_io_network_create(void) {
#ifdef _WIN32
    return WSAStartup(MAKEWORD(2, 2), &wsa);
#else
    return 0; // No initialization needed on Unix-like systems
#endif
}

void fossil_io_network_destroy(void) {
#ifdef _WIN32
    WSACleanup();
#endif
}

fossil_io_socket_t fossil_io_network_create_socket(int type) {
    fossil_io_socket_t sock = socket(AF_INET, type, 0);
    if (sock == FOSSIL_IO_INVALID_SOCKET) {
#ifdef _WIN32
        fprintf(stderr, "Socket creation failed with error: %d\n", WSAGetLastError());
#else
        perror("Socket creation failed");
#endif
    }
    return sock;
}

int fossil_io_network_bind(fossil_io_socket_t sock, const char *ip, uint16_t port) {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = ip ? inet_addr(ip) : INADDR_ANY;

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
#ifdef _WIN32
        fprintf(stderr, "Bind failed with error: %d\n", WSAGetLastError());
#else
        perror("Bind failed");
#endif
        return -1;
    }
    return 0;
}

int fossil_io_network_listen(fossil_io_socket_t sock, int backlog) {
    if (listen(sock, backlog) == -1) {
#ifdef _WIN32
        fprintf(stderr, "Listen failed with error: %d\n", WSAGetLastError());
#else
        perror("Listen failed");
#endif
        return -1;
    }
    return 0;
}

fossil_io_socket_t fossil_io_network_accept(fossil_io_socket_t sock, char *client_ip, uint16_t *client_port) {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    fossil_io_socket_t client_sock = accept(sock, (struct sockaddr*)&client_addr, &addr_len);

    if (client_sock == FOSSIL_IO_INVALID_SOCKET) {
#ifdef _WIN32
        fprintf(stderr, "Accept failed with error: %d\n", WSAGetLastError());
#else
        perror("Accept failed");
#endif
    } else if (client_ip) {
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

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
#ifdef _WIN32
        fprintf(stderr, "Connect failed with error: %d\n", WSAGetLastError());
#else
        perror("Connect failed");
#endif
        return -1;
    }
    return 0;
}

int fossil_io_network_send(fossil_io_socket_t sock, const void *data, size_t len) {
    int bytes_sent = send(sock, data, (int)len, 0);
    if (bytes_sent == -1) {
#ifdef _WIN32
        fprintf(stderr, "Send failed with error: %d\n", WSAGetLastError());
#else
        perror("Send failed");
#endif
    }
    return bytes_sent;
}

int fossil_io_network_receive(fossil_io_socket_t sock, void *buffer, size_t len) {
    int bytes_received = recv(sock, buffer, (int)len, 0);
    if (bytes_received == -1) {
#ifdef _WIN32
        fprintf(stderr, "Receive failed with error: %d\n", WSAGetLastError());
#else
        perror("Receive failed");
#endif
    }
    return bytes_received;
}

int fossil_io_network_sendto(fossil_io_socket_t sock, const void *data, size_t len, const char *ip, uint16_t port) {
    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = inet_addr(ip);

    int bytes_sent = sendto(sock, data, (int)len, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
    if (bytes_sent == -1) {
#ifdef _WIN32
        fprintf(stderr, "Sendto failed with error: %d\n", WSAGetLastError());
#else
        perror("Sendto failed");
#endif
    }
    return bytes_sent;
}

int fossil_io_network_recvfrom(fossil_io_socket_t sock, void *buffer, size_t len, char *ip, uint16_t *port) {
    struct sockaddr_in src_addr;
    socklen_t addr_len = sizeof(src_addr);

    int bytes_received = recvfrom(sock, buffer, (int)len, 0, (struct sockaddr*)&src_addr, &addr_len);
    if (bytes_received == -1) {
#ifdef _WIN32
        fprintf(stderr, "Recvfrom failed with error: %d\n", WSAGetLastError());
#else
        perror("Recvfrom failed");
#endif
    } else if (ip) {
        strcpy(ip, inet_ntoa(src_addr.sin_addr));
        if (port) {
            *port = ntohs(src_addr.sin_port);
        }
    }
    return bytes_received;
}

void fossil_io_network_close(fossil_io_socket_t sock) {
#ifdef _WIN32
    if (closesocket(sock) == SOCKET_ERROR) {
        fprintf(stderr, "Close socket failed with error: %d\n", WSAGetLastError());
    }
#else
    if (close(sock) == -1) {
        perror("Close socket failed");
    }
#endif
}
