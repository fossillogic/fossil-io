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
#include "fossil/io/tape.h"
#include "fossil/io/output.h"
#include <errno.h>
#include <sys/types.h> // for ssize_t
#include <inttypes.h>
#include <stdarg.h>

#ifndef FOSSIL_IO_TAPE_INTERNAL_ERROR_LEN
#define FOSSIL_IO_TAPE_INTERNAL_ERROR_LEN 256
#endif

struct fossil_io_tape {
    FILE *fp;
    fossil_io_tape_config_t cfg;
    size_t current_pos;     // bytes from start
    bool eot;
    char last_error[FOSSIL_IO_TAPE_INTERNAL_ERROR_LEN];
};

struct fossil_io_tape_multi {
    fossil_io_tape_t **tapes;
    size_t capacity;
    size_t count;
    bool stopped;
};

/* ---------------------------
 * Utility: set last error
 * --------------------------- */
static void tape_set_error(fossil_io_tape_t *t, const char *fmt, ...) {
    if (!t) return;
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(t->last_error, sizeof(t->last_error), fmt, ap);
    va_end(ap);
}

/* ---------------------------
 * Simple checksum (rolling CRC-like)
 * --------------------------- */
static uint32_t tape_compute_checksum(const void *buf, size_t n) {
    // small, fast non-cryptographic checksum (FNV-1a like)
    const uint8_t *b = (const uint8_t*)buf;
    uint32_t h = 2166136261u;
    for (size_t i = 0; i < n; ++i) {
        h ^= b[i];
        h *= 16777619u;
    }
    return h;
}

/* ---------------------------
 * Open / Close
 * --------------------------- */
fossil_io_tape_t *fossil_io_tape_open(const fossil_io_tape_config_t *cfg) {
    if (!cfg || !cfg->device_path) return NULL;

    FILE *fp = fopen(cfg->device_path, "r+b");
    if (!fp) {
        // try create
        fp = fopen(cfg->device_path, "w+b");
        if (!fp) {
            return NULL;
        }
    }

    fossil_io_tape_t *t = (fossil_io_tape_t*)calloc(1, sizeof(*t));
    if (!t) {
        fclose(fp);
        return NULL;
    }

    // copy config
    t->cfg.device_path = NULL;
    t->cfg.block_size = (cfg->block_size == 0 ? 1024 : cfg->block_size);
    t->cfg.auto_rewind = cfg->auto_rewind;
    t->cfg.verify_checksum = cfg->verify_checksum;

    // duplicate device_path string
    t->cfg.device_path = strdup(cfg->device_path);
    t->fp = fp;
    t->current_pos = 0;
    t->eot = false;
    t->last_error[0] = '\0';

    // position at beginning
    if (fseek(fp, 0, SEEK_SET) != 0) {
        tape_set_error(t, "seek failed: %s", strerror(errno));
    }

    return t;
}

void fossil_io_tape_close(fossil_io_tape_t *tape) {
    if (!tape) return;
    if (tape->fp) {
        fclose(tape->fp);
        tape->fp = NULL;
    }
    free((char*)tape->cfg.device_path);
    tape->cfg.device_path = NULL;
    free(tape);
}

/* ---------------------------
 * Read / Write (block-based)
 * --------------------------- */

ssize_t fossil_io_tape_read(fossil_io_tape_t *tape, void *buffer, size_t size) {
    if (!tape || !buffer || size == 0) return -1;
    if (!tape->fp) {
        tape_set_error(tape, "invalid file pointer");
        return -1;
    }

    // read up to 'size' bytes
    size_t r = fread(buffer, 1, size, tape->fp);
    if (r < size) {
        if (feof(tape->fp)) {
            tape->eot = true;
        } else if (ferror(tape->fp)) {
            tape_set_error(tape, "read error: %s", strerror(errno));
            clearerr(tape->fp);
            return -1;
        }
    }

    tape->current_pos += r;

    if (tape->cfg.verify_checksum && r > 0) {
        // stored checksum convention:
        // After each block (of exactly tape->cfg.block_size bytes when writing),
        // this simple implementation does not read a trailing checksum from the file
        // because writing side must include it. Here we only compute and ignore,
        // but leave hook for verification in future.
        (void)tape_compute_checksum(buffer, r);
    }

    // optionally auto-rewind if configured and EOT
    if (tape->eot && tape->cfg.auto_rewind) {
        if (fseek(tape->fp, 0, SEEK_SET) == 0) {
            tape->current_pos = 0;
            tape->eot = false;
        }
    }

    return (ssize_t)r;
}

ssize_t fossil_io_tape_write(fossil_io_tape_t *tape, const void *buffer, size_t size) {
    if (!tape || !buffer || size == 0) return -1;
    if (!tape->fp) {
        tape_set_error(tape, "invalid file pointer");
        return -1;
    }

    // write
    size_t w = fwrite(buffer, 1, size, tape->fp);
    if (w != size) {
        tape_set_error(tape, "write error: %s", strerror(errno));
        clearerr(tape->fp);
        return -1;
    }

    // flush so data persists (tape-like semantics)
    if (fflush(tape->fp) != 0) {
        tape_set_error(tape, "fflush failed: %s", strerror(errno));
        return -1;
    }

    tape->current_pos += w;
    tape->eot = false;

    if (tape->cfg.verify_checksum) {
        uint32_t c = tape_compute_checksum(buffer, w);
        // For a file-backed emulation we'd append the checksum as a 4-byte trailer.
        // But to keep simple & compatible with reads, we do NOT implicitly append a
        // checksum here (that can be added as an option). We compute it so the caller
        // can choose to write the checksum explicitly if desired.
        (void)c;
    }

    return (ssize_t)w;
}

/* ---------------------------
 * Rewind / Forward / Position / EOT
 * --------------------------- */

bool fossil_io_tape_rewind(fossil_io_tape_t *tape) {
    if (!tape || !tape->fp) return false;
    if (fseek(tape->fp, 0, SEEK_SET) != 0) {
        tape_set_error(tape, "rewind failed: %s", strerror(errno));
        return false;
    }
    tape->current_pos = 0;
    tape->eot = false;
    return true;
}

bool fossil_io_tape_forward(fossil_io_tape_t *tape, size_t blocks) {
    if (!tape || !tape->fp) return false;
    size_t step = tape->cfg.block_size ? tape->cfg.block_size : 1;
    // compute new offset, check bounds
    long new_off = (long)tape->current_pos + (long)blocks * (long)step;
    if (new_off < 0) new_off = 0;
    if (fseek(tape->fp, new_off, SEEK_SET) != 0) {
        tape_set_error(tape, "forward failed: %s", strerror(errno));
        return false;
    }
    tape->current_pos = (size_t)new_off;
    tape->eot = false;
    return true;
}

size_t fossil_io_tape_position(const fossil_io_tape_t *tape) {
    if (!tape) return 0;
    return tape->current_pos;
}

bool fossil_io_tape_eot(const fossil_io_tape_t *tape) {
    if (!tape) return true;
    return tape->eot || feof(tape->fp);
}

/* ---------------------------
 * Multi-tape functions
 * --------------------------- */

fossil_io_tape_multi_t *fossil_io_tape_multi_create(size_t count) {
    if (count == 0) return NULL;
    fossil_io_tape_multi_t *m = (fossil_io_tape_multi_t*)calloc(1, sizeof(*m));
    if (!m) return NULL;
    m->tapes = (fossil_io_tape_t**)calloc(count, sizeof(fossil_io_tape_t*));
    if (!m->tapes) {
        free(m);
        return NULL;
    }
    m->capacity = count;
    m->count = 0;
    m->stopped = false;
    return m;
}

bool fossil_io_tape_multi_add(fossil_io_tape_multi_t *multi, fossil_io_tape_t *tape) {
    if (!multi || !tape) return false;
    if (multi->count >= multi->capacity) return false;
    multi->tapes[multi->count++] = tape;
    return true;
}

/**
 * fossil_io_tape_multi_read:
 *   buffers is an array of pointers with at least multi->count elements.
 *   Each buffer should be preallocated with 'size' bytes (caller responsibility).
 * Returns number of tapes successfully read (>0), or 0 on none.
 */
size_t fossil_io_tape_multi_read(fossil_io_tape_multi_t *multi, void **buffers, size_t size) {
    if (!multi || !buffers || size == 0) return 0;
    if (multi->stopped) return 0;

    size_t success = 0;
    for (size_t i = 0; i < multi->count; ++i) {
        fossil_io_tape_t *t = multi->tapes[i];
        if (!t) continue;
        ssize_t r = fossil_io_tape_read(t, buffers[i], size);
        if (r > 0) {
            (void)r;
            ++success;
        } else {
            // if a tape failed read, set its error (already done in read)
            // but continue others (best-effort synchronous read)
        }
    }
    return success;
}

void fossil_io_tape_multi_stop(fossil_io_tape_multi_t *multi) {
    if (!multi) return;
    multi->stopped = true;
}

void fossil_io_tape_multi_destroy(fossil_io_tape_multi_t *multi) {
    if (!multi) return;
    free(multi->tapes);
    multi->tapes = NULL;
    free(multi);
}

/* ---------------------------
 * Utility and diagnostics
 * --------------------------- */

void fossil_io_tape_status(const fossil_io_tape_t *tape) {
    if (!tape) {
        fprintf(stderr, "[tape] (null)\n");
        return;
    }
    fprintf(stderr, "[tape] device='%s' pos=%zu block=%zu eot=%d verify_checksum=%d\n",
            tape->cfg.device_path ? tape->cfg.device_path : "(unknown)",
            tape->current_pos,
            tape->cfg.block_size,
            tape->eot ? 1 : 0,
            tape->cfg.verify_checksum ? 1 : 0);
}

const char *fossil_io_tape_device_name(const fossil_io_tape_t *tape) {
    if (!tape) return NULL;
    return tape->cfg.device_path;
}

const char *fossil_io_tape_last_error(const fossil_io_tape_t *tape) {
    if (!tape) return "invalid tape";
    return tape->last_error[0] ? tape->last_error : NULL;
}
