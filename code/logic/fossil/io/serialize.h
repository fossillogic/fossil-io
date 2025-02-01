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
#ifndef FOSSIL_IO_SERIALIZE_H
#define FOSSIL_IO_SERIALIZE_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t *buffer;
    size_t size;
    size_t capacity;
} fossil_io_serialize_buffer_t;

/**
 * @brief Initialize serialization buffer
 *
 * This function initializes a serialization buffer with the specified capacity.
 *
 * @param buf Pointer to the serialization buffer to initialize
 * @param capacity The capacity of the buffer
 * @return 0 on success, non-zero on failure
 */
int fossil_io_serialize_create(fossil_io_serialize_buffer_t *buf, size_t capacity);

/**
 * @brief Free serialization buffer
 *
 * This function frees the resources associated with a serialization buffer.
 *
 * @param buf Pointer to the serialization buffer to free
 */
void fossil_io_serialize_destroy(fossil_io_serialize_buffer_t *buf);

/**
 * @brief Expand serialization buffer
 *
 * This function expands the capacity of a serialization buffer by the specified amount.
 *
 * @param buf Pointer to the serialization buffer to expand
 * @param extra The amount to expand the buffer by
 * @return 0 on success, non-zero on failure
 */
int fossil_io_serialize_expand(fossil_io_serialize_buffer_t *buf, size_t extra);

/**
 * @brief Serialize an 8-bit integer
 *
 * This function serializes an 8-bit integer into the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param value The 8-bit integer value to serialize
 * @return 0 on success, non-zero on failure
 */
int fossil_io_serialize_i8(fossil_io_serialize_buffer_t *buf, int8_t value);

/**
 * @brief Serialize a 16-bit integer
 *
 * This function serializes a 16-bit integer into the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param value The 16-bit integer value to serialize
 * @return 0 on success, non-zero on failure
 */
int fossil_io_serialize_i16(fossil_io_serialize_buffer_t *buf, int16_t value);

/**
 * @brief Serialize a 32-bit integer
 *
 * This function serializes a 32-bit integer into the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param value The 32-bit integer value to serialize
 * @return 0 on success, non-zero on failure
 */
int fossil_io_serialize_i32(fossil_io_serialize_buffer_t *buf, int32_t value);

/**
 * @brief Serialize a 64-bit integer
 *
 * This function serializes a 64-bit integer into the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param value The 64-bit integer value to serialize
 * @return 0 on success, non-zero on failure
 */
int fossil_io_serialize_i64(fossil_io_serialize_buffer_t *buf, int64_t value);

/**
 * @brief Serialize an unsigned 8-bit integer
 *
 * This function serializes an unsigned 8-bit integer into the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param value The unsigned 8-bit integer value to serialize
 * @return 0 on success, non-zero on failure
 */
int fossil_io_serialize_u8(fossil_io_serialize_buffer_t *buf, uint8_t value);

/**
 * @brief Serialize an unsigned 16-bit integer
 *
 * This function serializes an unsigned 16-bit integer into the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param value The unsigned 16-bit integer value to serialize
 * @return 0 on success, non-zero on failure
 */
int fossil_io_serialize_u16(fossil_io_serialize_buffer_t *buf, uint16_t value);

/**
 * @brief Serialize an unsigned 32-bit integer
 *
 * This function serializes an unsigned 32-bit integer into the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param value The unsigned 32-bit integer value to serialize
 * @return 0 on success, non-zero on failure
 */
int fossil_io_serialize_u32(fossil_io_serialize_buffer_t *buf, uint32_t value);

/**
 * @brief Serialize an unsigned 64-bit integer
 *
 * This function serializes an unsigned 64-bit integer into the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param value The unsigned 64-bit integer value to serialize
 * @return 0 on success, non-zero on failure
 */
int fossil_io_serialize_u64(fossil_io_serialize_buffer_t *buf, uint64_t value);

/**
 * @brief Serialize a float
 *
 * This function serializes a float value into the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param value The float value to serialize
 * @return 0 on success, non-zero on failure
 */
int fossil_io_serialize_f32(fossil_io_serialize_buffer_t *buf, float value);

/**
 * @brief Serialize a double
 *
 * This function serializes a double value into the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param value The double value to serialize
 * @return 0 on success, non-zero on failure
 */
int fossil_io_serialize_f64(fossil_io_serialize_buffer_t *buf, double value);

/**
 * @brief Serialize a string
 *
 * This function serializes a null-terminated string into the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param str The null-terminated string to serialize
 * @return 0 on success, non-zero on failure
 */
int fossil_io_serialize_cstr(fossil_io_serialize_buffer_t *buf, const char *str);

/**
 * @brief Serialize a boolean value
 *
 * This function serializes a boolean value into the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param value The boolean value to serialize
 * @return 0 on success, non-zero on failure
 */
int fossil_io_serialize_bool(fossil_io_serialize_buffer_t *buf, int value);

/**
 * @brief Deserialize an 8-bit integer
 *
 * This function deserializes an 8-bit integer from the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param offset Pointer to the offset within the buffer to start deserialization
 * @param value Pointer to the 8-bit integer to store the deserialized value
 * @return 0 on success, non-zero on failure
 */
int fossil_io_deserialize_i8(fossil_io_serialize_buffer_t *buf, size_t *offset, int8_t *value);

/**
 * @brief Deserialize a 16-bit integer
 *
 * This function deserializes a 16-bit integer from the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param offset Pointer to the offset within the buffer to start deserialization
 * @param value Pointer to the 16-bit integer to store the deserialized value
 * @return 0 on success, non-zero on failure
 */
int fossil_io_deserialize_i16(fossil_io_serialize_buffer_t *buf, size_t *offset, int16_t *value);

/**
 * @brief Deserialize a 32-bit integer
 *
 * This function deserializes a 32-bit integer from the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param offset Pointer to the offset within the buffer to start deserialization
 * @param value Pointer to the 32-bit integer to store the deserialized value
 * @return 0 on success, non-zero on failure
 */
int fossil_io_deserialize_i32(fossil_io_serialize_buffer_t *buf, size_t *offset, int32_t *value);

/**
 * @brief Deserialize a 64-bit integer
 *
 * This function deserializes a 64-bit integer from the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param offset Pointer to the offset within the buffer to start deserialization
 * @param value Pointer to the 64-bit integer to store the deserialized value
 * @return 0 on success, non-zero on failure
 */
int fossil_io_deserialize_i64(fossil_io_serialize_buffer_t *buf, size_t *offset, int64_t *value);

/**
 * @brief Deserialize an unsigned 8-bit integer
 *
 * This function deserializes an unsigned 8-bit integer from the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param offset Pointer to the offset within the buffer to start deserialization
 * @param value Pointer to the unsigned 8-bit integer to store the deserialized value
 * @return 0 on success, non-zero on failure
 */
int fossil_io_deserialize_u8(fossil_io_serialize_buffer_t *buf, size_t *offset, uint8_t *value);

/**
 * @brief Deserialize an unsigned 16-bit integer
 *
 * This function deserializes an unsigned 16-bit integer from the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param offset Pointer to the offset within the buffer to start deserialization
 * @param value Pointer to the unsigned 16-bit integer to store the deserialized value
 * @return 0 on success, non-zero on failure
 */
int fossil_io_deserialize_u16(fossil_io_serialize_buffer_t *buf, size_t *offset, uint16_t *value);

/**
 * @brief Deserialize an unsigned 32-bit integer
 *
 * This function deserializes an unsigned 32-bit integer from the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param offset Pointer to the offset within the buffer to start deserialization
 * @param value Pointer to the unsigned 32-bit integer to store the deserialized value
 * @return 0 on success, non-zero on failure
 */
int fossil_io_deserialize_u32(fossil_io_serialize_buffer_t *buf, size_t *offset, uint32_t *value);

/**
 * @brief Deserialize an unsigned 64-bit integer
 *
 * This function deserializes an unsigned 64-bit integer from the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param offset Pointer to the offset within the buffer to start deserialization
 * @param value Pointer to the unsigned 64-bit integer to store the deserialized value
 * @return 0 on success, non-zero on failure
 */
int fossil_io_deserialize_u64(fossil_io_serialize_buffer_t *buf, size_t *offset, uint64_t *value);

/**
 * @brief Deserialize a float
 *
 * This function deserializes a float value from the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param offset Pointer to the offset within the buffer to start deserialization
 * @param value Pointer to the float to store the deserialized value
 * @return 0 on success, non-zero on failure
 */
int fossil_io_deserialize_f32(fossil_io_serialize_buffer_t *buf, size_t *offset, float *value);

/**
 * @brief Deserialize a double
 *
 * This function deserializes a double value from the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param offset Pointer to the offset within the buffer to start deserialization
 * @param value Pointer to the double to store the deserialized value
 * @return 0 on success, non-zero on failure
 */
int fossil_io_deserialize_f64(fossil_io_serialize_buffer_t *buf, size_t *offset, double *value);

/**
 * @brief Deserialize a string
 *
 * This function deserializes a null-terminated string from the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param offset Pointer to the offset within the buffer to start deserialization
 * @param out Pointer to the buffer to store the deserialized string
 * @param max_len The maximum length of the output buffer
 * @return 0 on success, non-zero on failure
 */
int fossil_io_deserialize_cstr(fossil_io_serialize_buffer_t *buf, size_t *offset, char *out, size_t max_len);

/**
 * @brief Deserialize a boolean value
 *
 * This function deserializes a boolean value from the buffer.
 *
 * @param buf Pointer to the serialization buffer
 * @param offset Pointer to the offset within the buffer to start deserialization
 * @param value Pointer to the boolean value to store the deserialized value
 * @return 0 on success, non-zero on failure
 */
int fossil_io_deserialize_bool(fossil_io_serialize_buffer_t *buf, size_t *offset, int *value);

/**
 * @brief Serialize buffer to file
 *
 * This function serializes a buffer to a file.
 *
 * @param buf Pointer to the serialization buffer
 * @param filename The name of the file to write to
 * @return 0 on success, non-zero on failure
 */
int fossil_io_serialize_to_file(fossil_io_serialize_buffer_t *buf, const char *filename);

/**
 * @brief Deserialize buffer from file
 *
 * This function deserializes a buffer from a file.
 *
 * @param buf Pointer to the serialization buffer
 * @param filename The name of the file to read from
 * @return 0 on success, non-zero on failure
 */
int fossil_io_deserialize_from_file(fossil_io_serialize_buffer_t *buf, const char *filename);

#ifdef __cplusplus
}

/**
 * Namespace for the Fossil Logic I/O library.
 */
namespace fossil {

    /**
     * Namespace for the I/O utilities.
     */
    namespace io {

        /**
         * Class for handling operations.
         */
        class Serialize {
        public:
            /**
             * Constructor to initialize a serialization buffer with the specified capacity.
             *
             * @param capacity The capacity of the buffer
             */
            Serialize(size_t capacity) {
                fossil_io_serialize_create(&buffer, capacity);
            }

            /**
             * Destructor to free the resources associated with a serialization buffer.
             */
            ~Serialize() {
                fossil_io_serialize_destroy(&buffer);
            }

            /**
             * Expands the capacity of a serialization buffer by the specified amount.
             *
             * @param extra The amount to expand the buffer by
             * @return 0 on success, non-zero on failure
             */
            int expand(size_t extra) {
                return fossil_io_serialize_expand(&buffer, extra);
            }

            /**
             * Serializes an 8-bit integer into the buffer.
             *
             * @param value The 8-bit integer value to serialize
             * @return 0 on success, non-zero on failure
             */
            int serialize_i8(int8_t value) {
                return fossil_io_serialize_i8(&buffer, value);
            }

            /**
             * Serializes a 16-bit integer into the buffer.
             *
             * @param value The 16-bit integer value to serialize
             * @return 0 on success, non-zero on failure
             */
            int serialize_i16(int16_t value) {
                return fossil_io_serialize_i16(&buffer, value);
            }

            /**
             * Serializes a 32-bit integer into the buffer.
             *
             * @param value The 32-bit integer value to serialize
             * @return 0 on success, non-zero on failure
             */
            int serialize_i32(int32_t value) {
                return fossil_io_serialize_i32(&buffer, value);
            }

            /**
             * Serializes a 64-bit integer into the buffer.
             *
             * @param value The 64-bit integer value to serialize
             * @return 0 on success, non-zero on failure
             */
            int serialize_i64(int64_t value) {
                return fossil_io_serialize_i64(&buffer, value);
            }

            /**
             * Serializes an unsigned 8-bit integer into the buffer.
             *
             * @param value The unsigned 8-bit integer value to serialize
             * @return 0 on success, non-zero on failure
             */
            int serialize_u8(uint8_t value) {
                return fossil_io_serialize_u8(&buffer, value);
            }

            /**
             * Serializes an unsigned 16-bit integer into the buffer.
             *
             * @param value The unsigned 16-bit integer value to serialize
             * @return 0 on success, non-zero on failure
             */
            int serialize_u16(uint16_t value) {
                return fossil_io_serialize_u16(&buffer, value);
            }

            /**
             * Serializes an unsigned 32-bit integer into the buffer.
             *
             * @param value The unsigned 32-bit integer value to serialize
             * @return 0 on success, non-zero on failure
             */
            int serialize_u32(uint32_t value) {
                return fossil_io_serialize_u32(&buffer, value);
            }

            /**
             * Serializes an unsigned 64-bit integer into the buffer.
             *
             * @param value The unsigned 64-bit integer value to serialize
             * @return 0 on success, non-zero on failure
             */
            int serialize_u64(uint64_t value) {
                return fossil_io_serialize_u64(&buffer, value);
            }

            /**
             * Serializes a float value into the buffer.
             *
             * @param value The float value to serialize
             * @return 0 on success, non-zero on failure
             */
            int serialize_f32(float value) {
                return fossil_io_serialize_f32(&buffer, value);
            }

            /**
             * Serializes a double value into the buffer.
             *
             * @param value The double value to serialize
             * @return 0 on success, non-zero on failure
             */
            int serialize_f64(double value) {
                return fossil_io_serialize_f64(&buffer, value);
            }

            /**
             * Serializes a null-terminated string into the buffer.
             *
             * @param str The null-terminated string to serialize
             * @return 0 on success, non-zero on failure
             */
            int serialize_cstr(const char *str) {
                return fossil_io_serialize_cstr(&buffer, str);
            }

            /**
             * Serializes a boolean value into the buffer.
             *
             * @param value The boolean value to serialize
             * @return 0 on success, non-zero on failure
             */
            int serialize_bool(int value) {
                return fossil_io_serialize_bool(&buffer, value);
            }

            /**
             * Deserializes an 8-bit integer from the buffer.
             *
             * @param offset Pointer to the offset within the buffer to start deserialization
             * @param value Pointer to the 8-bit integer to store the deserialized value
             * @return 0 on success, non-zero on failure
             */
            int deserialize_i8(size_t *offset, int8_t *value) {
                return fossil_io_deserialize_i8(&buffer, offset, value);
            }

            /**
             * Deserializes a 16-bit integer from the buffer.
             *
             * @param offset Pointer to the offset within the buffer to start deserialization
             * @param value Pointer to the 16-bit integer to store the deserialized value
             * @return 0 on success, non-zero on failure
             */
            int deserialize_i16(size_t *offset, int16_t *value) {
                return fossil_io_deserialize_i16(&buffer, offset, value);
            }

            /**
             * Deserializes a 32-bit integer from the buffer.
             *
             * @param offset Pointer to the offset within the buffer to start deserialization
             * @param value Pointer to the 32-bit integer to store the deserialized value
             * @return 0 on success, non-zero on failure
             */
            int deserialize_i32(size_t *offset, int32_t *value) {
                return fossil_io_deserialize_i32(&buffer, offset, value);
            }

            /**
             * Deserializes a 64-bit integer from the buffer.
             *
             * @param offset Pointer to the offset within the buffer to start deserialization
             * @param value Pointer to the 64-bit integer to store the deserialized value
             * @return 0 on success, non-zero on failure
             */
            int deserialize_i64(size_t *offset, int64_t *value) {
                return fossil_io_deserialize_i64(&buffer, offset, value);
            }

            /**
             * Deserializes an unsigned 8-bit integer from the buffer.
             *
             * @param offset Pointer to the offset within the buffer to start deserialization
             * @param value Pointer to the unsigned 8-bit integer to store the deserialized value
             * @return 0 on success, non-zero on failure
             */
            int deserialize_u8(size_t *offset, uint8_t *value) {
                return fossil_io_deserialize_u8(&buffer, offset, value);
            }

            /**
             * Deserializes an unsigned 16-bit integer from the buffer.
             *
             * @param offset Pointer to the offset within the buffer to start deserialization
             * @param value Pointer to the unsigned 16-bit integer to store the deserialized value
             * @return 0 on success, non-zero on failure
             */
            int deserialize_u16(size_t *offset, uint16_t *value) {
                return fossil_io_deserialize_u16(&buffer, offset, value);
            }

            /**
             * Deserializes an unsigned 32-bit integer from the buffer.
             *
             * @param offset Pointer to the offset within the buffer to start deserialization
             * @param value Pointer to the unsigned 32-bit integer to store the deserialized value
             * @return 0 on success, non-zero on failure
             */
            int deserialize_u32(size_t *offset, uint32_t *value) {
                return fossil_io_deserialize_u32(&buffer, offset, value);
            }

            /**
             * Deserializes an unsigned 64-bit integer from the buffer.
             *
             * @param offset Pointer to the offset within the buffer to start deserialization
             * @param value Pointer to the unsigned 64-bit integer to store the deserialized value
             * @return 0 on success, non-zero on failure
             */
            int deserialize_u64(size_t *offset, uint64_t *value) {
                return fossil_io_deserialize_u64(&buffer, offset, value);
            }

            /**
             * Deserializes a float value from the buffer.
             *
             * @param offset Pointer to the offset within the buffer to start deserialization
             * @param value Pointer to the float to store the deserialized value
             * @return 0 on success, non-zero on failure
             */
            int deserialize_f32(size_t *offset, float *value) {
                return fossil_io_deserialize_f32(&buffer, offset, value);
            }

            /**
             * Deserializes a double value from the buffer.
             *
             * @param offset Pointer to the offset within the buffer to start deserialization
             * @param value Pointer to the double to store the deserialized value
             * @return 0 on success, non-zero on failure
             */
            int deserialize_f64(size_t *offset, double *value) {
                return fossil_io_deserialize_f64(&buffer, offset, value);
            }

            /**
             * Deserializes a null-terminated string from the buffer.
             *
             * @param offset Pointer to the offset within the buffer to start deserialization
             * @param out Pointer to the buffer to store the deserialized string
             * @param max_len The maximum length of the output buffer
             * @return 0 on success, non-zero on failure
             */
            int deserialize_cstr(size_t *offset, char *out, size_t max_len) {
                return fossil_io_deserialize_cstr(&buffer, offset, out, max_len);
            }

            /**
             * Deserializes a boolean value from the buffer.
             *
             * @param offset Pointer to the offset within the buffer to start deserialization
             * @param value Pointer to the boolean value to store the deserialized value
             * @return 0 on success, non-zero on failure
             */
            int deserialize_bool(size_t *offset, int *value) {
                return fossil_io_deserialize_bool(&buffer, offset, value);
            }

            /**
             * Serializes a buffer to a file.
             *
             * @param filename The name of the file to write to
             * @return 0 on success, non-zero on failure
             */
            int serialize_to_file(const char *filename) {
                return fossil_io_serialize_to_file(&buffer, filename);
            }

            /**
             * Deserializes a buffer from a file.
             *
             * @param filename The name of the file to read from
             * @return 0 on success, non-zero on failure
             */
            int deserialize_from_file(const char *filename) {
                return fossil_io_deserialize_from_file(&buffer, filename);
            }

    }

}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
