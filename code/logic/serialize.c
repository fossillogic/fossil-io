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
#include "fossil/io/serialize.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#define fseeko _fseeki64  // Use Windows-specific fseek for large files

#else
#include <unistd.h>

#endif

// Custom endian handling functions
uint16_t fossil_io_htons(uint16_t hostshort) {
    uint16_t result = (hostshort << 8) | (hostshort >> 8);
    return result;
}

uint16_t fossil_io_ntohs(uint16_t netshort) {
    return fossil_io_htons(netshort);
}

uint32_t fossil_io_htonl(uint32_t hostlong) {
    uint32_t result = ((hostlong & 0xFF000000) >> 24) |
                      ((hostlong & 0x00FF0000) >> 8) |
                      ((hostlong & 0x0000FF00) << 8) |
                      ((hostlong & 0x000000FF) << 24);
    return result;
}

uint32_t fossil_io_ntohl(uint32_t netlong) {
    return fossil_io_htonl(netlong);
}

uint64_t fossil_io_htonll(uint64_t hostlonglong) {
    uint64_t result = ((hostlonglong & 0xFF00000000000000ULL) >> 56) |
                      ((hostlonglong & 0x00FF000000000000ULL) >> 40) |
                      ((hostlonglong & 0x0000FF0000000000ULL) >> 24) |
                      ((hostlonglong & 0x000000FF00000000ULL) >> 8) |
                      ((hostlonglong & 0x00000000FF000000ULL) << 8) |
                      ((hostlonglong & 0x0000000000FF0000ULL) << 24) |
                      ((hostlonglong & 0x000000000000FF00ULL) << 40) |
                      ((hostlonglong & 0x00000000000000FFULL) << 56);
    return result;
}

uint64_t fossil_io_ntohll(uint64_t netlonglong) {
    return fossil_io_htonll(netlonglong);
}

// Initialization and memory management
int fossil_io_serialize_create(fossil_io_serialize_buffer_t *buf, size_t capacity) {
    if (!buf) return -1;
    buf->buffer = (uint8_t *)malloc(capacity);
    if (!buf->buffer) return -1;
    buf->size = 0;
    buf->capacity = capacity;
    return 0;
}

void fossil_io_serialize_destroy(fossil_io_serialize_buffer_t *buf) {
    if (buf && buf->buffer) {
        free(buf->buffer);
        buf->buffer = NULL;
        buf->size = buf->capacity = 0;
    }
}

int fossil_io_serialize_expand(fossil_io_serialize_buffer_t *buf, size_t extra) {
    if (!buf) return -1;
    size_t new_capacity = buf->capacity * 2 + extra;
    uint8_t *new_buffer = (uint8_t *)realloc(buf->buffer, new_capacity);
    if (!new_buffer) return -1;
    
    buf->buffer = new_buffer;
    buf->capacity = new_capacity;
    return 0;
}

int fossil_io_serialize_int8(fossil_io_serialize_buffer_t *buf, int8_t value) {
    if (buf->size + sizeof(int8_t) > buf->capacity) {
        if (fossil_io_serialize_expand(buf, sizeof(int8_t)) != 0) return -1;
    }
    buf->buffer[buf->size++] = (uint8_t)value;
    return 0;
}

int fossil_io_serialize_int16(fossil_io_serialize_buffer_t *buf, int16_t value) {
    if (buf->size + sizeof(int16_t) > buf->capacity) {
        if (fossil_io_serialize_expand(buf, sizeof(int16_t)) != 0) return -1;
    }
    int16_t net_value = fossil_io_htons(value);
    memcpy(buf->buffer + buf->size, &net_value, sizeof(int16_t));
    buf->size += sizeof(int16_t);
    return 0;
}

int fossil_io_serialize_int32(fossil_io_serialize_buffer_t *buf, int32_t value) {
    if (buf->size + sizeof(int32_t) > buf->capacity) {
        if (fossil_io_serialize_expand(buf, sizeof(int32_t)) != 0) return -1;
    }
    int32_t net_value = fossil_io_htonl(value);
    memcpy(buf->buffer + buf->size, &net_value, sizeof(int32_t));
    buf->size += sizeof(int32_t);
    return 0;
}

int fossil_io_serialize_int64(fossil_io_serialize_buffer_t *buf, int64_t value) {
    if (buf->size + sizeof(int64_t) > buf->capacity) {
        if (fossil_io_serialize_expand(buf, sizeof(int64_t)) != 0) return -1;
    }
    int64_t net_value = fossil_io_htonll(value);
    memcpy(buf->buffer + buf->size, &net_value, sizeof(int64_t));
    buf->size += sizeof(int64_t);
    return 0;
}

int fossil_io_serialize_float(fossil_io_serialize_buffer_t *buf, float value) {
    if (buf->size + sizeof(float) > buf->capacity) {
        if (fossil_io_serialize_expand(buf, sizeof(float)) != 0) return -1;
    }
    memcpy(buf->buffer + buf->size, &value, sizeof(float));
    buf->size += sizeof(float);
    return 0;
}

int fossil_io_serialize_double(fossil_io_serialize_buffer_t *buf, double value) {
    if (buf->size + sizeof(double) > buf->capacity) {
        if (fossil_io_serialize_expand(buf, sizeof(double)) != 0) return -1;
    }
    memcpy(buf->buffer + buf->size, &value, sizeof(double));
    buf->size += sizeof(double);
    return 0;
}

int fossil_io_serialize_string(fossil_io_serialize_buffer_t *buf, const char *str) {
    size_t len = strlen(str) + 1;  // Include null-terminator
    if (buf->size + len > buf->capacity) {
        if (fossil_io_serialize_expand(buf, len) != 0) return -1;
    }
    memcpy(buf->buffer + buf->size, str, len);
    buf->size += len;
    return 0;
}

int fossil_io_serialize_bool(fossil_io_serialize_buffer_t *buf, int value) {
    if (buf->size + sizeof(int) > buf->capacity) {
        if (fossil_io_serialize_expand(buf, sizeof(int)) != 0) return -1;
    }
    buf->buffer[buf->size++] = (uint8_t)value;
    return 0;
}

int fossil_io_deserialize_int8(fossil_io_serialize_buffer_t *buf, size_t *offset, int8_t *value) {
    if (*offset + sizeof(int8_t) > buf->size) return -1;
    *value = buf->buffer[(*offset)++];
    return 0;
}

int fossil_io_deserialize_int16(fossil_io_serialize_buffer_t *buf, size_t *offset, int16_t *value) {
    if (*offset + sizeof(int16_t) > buf->size) return -1;
    int16_t net_value;
    memcpy(&net_value, buf->buffer + *offset, sizeof(int16_t));
    *value = fossil_io_ntohs(net_value);
    *offset += sizeof(int16_t);
    return 0;
}

int fossil_io_deserialize_int32(fossil_io_serialize_buffer_t *buf, size_t *offset, int32_t *value) {
    if (*offset + sizeof(int32_t) > buf->size) return -1;
    int32_t net_value;
    memcpy(&net_value, buf->buffer + *offset, sizeof(int32_t));
    *value = fossil_io_ntohl(net_value);
    *offset += sizeof(int32_t);
    return 0;
}

int fossil_io_deserialize_int64(fossil_io_serialize_buffer_t *buf, size_t *offset, int64_t *value) {
    if (*offset + sizeof(int64_t) > buf->size) return -1;
    int64_t net_value;
    memcpy(&net_value, buf->buffer + *offset, sizeof(int64_t));
    *value = fossil_io_ntohll(net_value);
    *offset += sizeof(int64_t);
    return 0;
}

int fossil_io_deserialize_float(fossil_io_serialize_buffer_t *buf, size_t *offset, float *value) {
    if (*offset + sizeof(float) > buf->size) return -1;
    memcpy(value, buf->buffer + *offset, sizeof(float));
    *offset += sizeof(float);
    return 0;
}

int fossil_io_deserialize_double(fossil_io_serialize_buffer_t *buf, size_t *offset, double *value) {
    if (*offset + sizeof(double) > buf->size) return -1;
    memcpy(value, buf->buffer + *offset, sizeof(double));
    *offset += sizeof(double);
    return 0;
}

int fossil_io_deserialize_string(fossil_io_serialize_buffer_t *buf, size_t *offset, char *out, size_t max_len) {
    size_t len = strnlen((char *)(buf->buffer + *offset), max_len);
    if (*offset + len + 1 > buf->size) return -1;
    memcpy(out, buf->buffer + *offset, len + 1);
    *offset += len + 1;
    return 0;
}

int fossil_io_deserialize_bool(fossil_io_serialize_buffer_t *buf, size_t *offset, int *value) {
    if (*offset + 1 > buf->size) return -1;
    *value = buf->buffer[(*offset)++];
    return 0;
}

// File-based serialization functions
int fossil_io_serialize_to_file(fossil_io_serialize_buffer_t *buf, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) return -1;
    size_t written = fwrite(buf->buffer, 1, buf->size, file);
    if (written != buf->size) {
        fclose(file);
        return -1;
    }
    fclose(file);
    return 0;
}

int fossil_io_deserialize_from_file(fossil_io_serialize_buffer_t *buf, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) return -1;

    // Seek to the end of the file to get its size
    fseeko(file, 0, SEEK_END);
    size_t file_size = ftello(file);
    rewind(file);

    if (fossil_io_serialize_create(buf, file_size) != 0) {
        fclose(file);
        return -1;
    }

    // Read the file content into the buffer
    size_t read = fread(buf->buffer, 1, file_size, file);
    if (read != file_size) {
        fossil_io_serialize_destroy(buf);
        fclose(file);
        return -1;
    }
    
    buf->size = file_size;
    fclose(file);
    return 0;
}
