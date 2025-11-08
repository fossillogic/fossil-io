/**
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * This file is part of the Fossil Logic project, which aims to develop
 * high-performance, cross-platform applications and libraries. The code
 * contained herein is licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * Author: Michael Gene Brockus (Dreamer)
 * Date: 04/05/2014
 *
 * Copyright (C) 2014-2025 Fossil Logic. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#ifndef FOSSIL_IO_TAPE_H
#define FOSSIL_IO_TAPE_H

#include "stream.h"

#ifdef __cplusplus
extern "C" {
#endif

// ======================================================
// Fossil IO Tape — Type Definitions
// ======================================================

typedef enum fossil_io_tape_mode {
    FOSSIL_IO_TAPE_MODE_READ,
    FOSSIL_IO_TAPE_MODE_WRITE,
    FOSSIL_IO_TAPE_MODE_REWIND,
    FOSSIL_IO_TAPE_MODE_FORWARD,
    FOSSIL_IO_TAPE_MODE_STOP
} fossil_io_tape_mode_t;

typedef struct fossil_io_tape fossil_io_tape_t;
typedef struct fossil_io_tape_multi fossil_io_tape_multi_t;

// ======================================================
// Tape Device Configuration
// ======================================================

typedef struct fossil_io_tape_config {
    const char *device_path;   // Physical or virtual device
    size_t block_size;         // Default read/write block size
    bool auto_rewind;          // Auto rewind after read
    bool verify_checksum;      // Enable CRC or checksum validation
} fossil_io_tape_config_t;

// ======================================================
// Single Tape Reader API
// ======================================================

/**
 * @brief Open a single tape device.
 */
fossil_io_tape_t *fossil_io_tape_open(const fossil_io_tape_config_t *cfg);

/**
 * @brief Close a tape device.
 */
void fossil_io_tape_close(fossil_io_tape_t *tape);

/**
 * @brief Read a block of data from the tape.
 */
ssize_t fossil_io_tape_read(fossil_io_tape_t *tape, void *buffer, size_t size);

/**
 * @brief Write a block of data to the tape.
 */
ssize_t fossil_io_tape_write(fossil_io_tape_t *tape, const void *buffer, size_t size);

/**
 * @brief Rewind the tape to the beginning.
 */
bool fossil_io_tape_rewind(fossil_io_tape_t *tape);

/**
 * @brief Move forward N blocks.
 */
bool fossil_io_tape_forward(fossil_io_tape_t *tape, size_t blocks);

/**
 * @brief Get current tape position.
 */
size_t fossil_io_tape_position(const fossil_io_tape_t *tape);

/**
 * @brief Check if end-of-tape (EOT) reached.
 */
bool fossil_io_tape_eot(const fossil_io_tape_t *tape);

// ======================================================
// Multi-Tape Reader API
// ======================================================

/**
 * @brief Create a multi-tape reader pool.
 */
fossil_io_tape_multi_t *fossil_io_tape_multi_create(size_t count);

/**
 * @brief Add a tape reader to a multi-reader instance.
 */
bool fossil_io_tape_multi_add(fossil_io_tape_multi_t *multi, fossil_io_tape_t *tape);

/**
 * @brief Read synchronously from all active tapes.
 * Returns number of tapes successfully read.
 */
size_t fossil_io_tape_multi_read(fossil_io_tape_multi_t *multi, void **buffers, size_t size);

/**
 * @brief Stop all tapes in a multi-reader instance.
 */
void fossil_io_tape_multi_stop(fossil_io_tape_multi_t *multi);

/**
 * @brief Destroy a multi-tape reader instance.
 */
void fossil_io_tape_multi_destroy(fossil_io_tape_multi_t *multi);

// ======================================================
// Utility and Diagnostics
// ======================================================

/**
 * @brief Print device status or diagnostics.
 */
void fossil_io_tape_status(const fossil_io_tape_t *tape);

/**
 * @brief Return device name/path.
 */
const char *fossil_io_tape_device_name(const fossil_io_tape_t *tape);

/**
 * @brief Return last error or system message.
 */
const char *fossil_io_tape_last_error(const fossil_io_tape_t *tape);

#ifdef __cplusplus
}
#include <string>

/**
 * Namespace for the Fossil Logic I/O library.
 */
namespace fossil {

    /**
     * Namespace for the I/O utilities.
     */
    namespace io {



    }

}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
