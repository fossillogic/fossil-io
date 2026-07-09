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
#ifndef FOSSIL_IO_CUBE_H
#define FOSSIL_IO_CUBE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "output.h"
#include "input.h"
#include "filesys.h"

/*----------------------------------------------------------------------------
    Limits
----------------------------------------------------------------------------*/

#define FOSSIL_IO_CUBE_MAX_NAME        128
#define FOSSIL_IO_CUBE_MAX_TEXT        4096
#define FOSSIL_IO_CUBE_MAX_STYLE       128
#define FOSSIL_IO_CUBE_MAX_TYPE         32
#define FOSSIL_IO_CUBE_MAX_LAYOUT       32
#define FOSSIL_IO_CUBE_MAX_EVENT        32

/*----------------------------------------------------------------------------
    Generic Object
----------------------------------------------------------------------------*/

typedef struct fossil_io_cube_object
{
    char id[FOSSIL_FILESYS_MAX_ID];
    char name[FOSSIL_IO_CUBE_MAX_NAME];
    char type[FOSSIL_IO_CUBE_MAX_TYPE];
    char parent[FOSSIL_FILESYS_MAX_ID];
    char style[FOSSIL_IO_CUBE_MAX_STYLE];
    char text[FOSSIL_IO_CUBE_MAX_TEXT];
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
    bool visible;
    bool enabled;
    bool focused;
    fossil_io_filesys_obj_t file;
    void *user_data;
} fossil_io_cube_object_t;

/*----------------------------------------------------------------------------
    Event
----------------------------------------------------------------------------*/

typedef struct
{
    char name[FOSSIL_IO_CUBE_MAX_EVENT];
    char source[FOSSIL_FILESYS_MAX_ID];
    void *data;
} fossil_io_cube_event_t;

typedef void (*fossil_io_cube_callback)(
    fossil_io_cube_event_t *event);

/*----------------------------------------------------------------------------
    Lifecycle
----------------------------------------------------------------------------*/

int32_t fossil_io_cube_init(void);
void fossil_io_cube_shutdown(void);
void fossil_io_cube_begin(void);
void fossil_io_cube_end(void);
void fossil_io_cube_present(void);

/*----------------------------------------------------------------------------
    Context
----------------------------------------------------------------------------*/

int32_t fossil_io_cube_context_create(
    const char *id);

int32_t fossil_io_cube_context_destroy(
    const char *id);

int32_t fossil_io_cube_context_use(
    const char *id);

/*----------------------------------------------------------------------------
    Object Management
----------------------------------------------------------------------------*/

int32_t fossil_io_cube_create(
    const char *type,
    const char *id,
    const char *name);

int32_t fossil_io_cube_destroy(
    const char *id);

fossil_io_cube_object_t *fossil_io_cube_find(
    const char *id);

int32_t fossil_io_cube_clone(
    const char *source,
    const char *dest);

/*----------------------------------------------------------------------------

    Hierarchy

----------------------------------------------------------------------------*/

int32_t fossil_io_cube_set_parent(
    const char *child,
    const char *parent);

const char *fossil_io_cube_get_parent(
    const char *id);

/*----------------------------------------------------------------------------
    Properties
----------------------------------------------------------------------------*/

int32_t fossil_io_cube_set_name(
    const char *id,
    const char *name);

int32_t fossil_io_cube_set_text(
    const char *id,
    const char *text);

int32_t fossil_io_cube_append_text(
    const char *id,
    const char *text);

const char *fossil_io_cube_get_text(
    const char *id);

int32_t fossil_io_cube_set_style(
    const char *id,
    const char *style);

int32_t fossil_io_cube_set_user_data(
    const char *id,
    void *data);

void *fossil_io_cube_get_user_data(
    const char *id);

/*----------------------------------------------------------------------------
    Geometry
----------------------------------------------------------------------------*/

int32_t fossil_io_cube_set_position(
    const char *id,
    int32_t x,
    int32_t y);

int32_t fossil_io_cube_move(
    const char *id,
    int32_t dx,
    int32_t dy);

int32_t fossil_io_cube_set_size(
    const char *id,
    int32_t width,
    int32_t height);

int32_t fossil_io_cube_resize(
    const char *id,
    int32_t width,
    int32_t height);

/*----------------------------------------------------------------------------
    Visibility
----------------------------------------------------------------------------*/

int32_t fossil_io_cube_show(
    const char *id);

int32_t fossil_io_cube_hide(
    const char *id);

int32_t fossil_io_cube_enable(
    const char *id);

int32_t fossil_io_cube_disable(
    const char *id);

int32_t fossil_io_cube_focus(
    const char *id);

int32_t fossil_io_cube_blur(
    const char *id);

/*----------------------------------------------------------------------------

    Drawing

----------------------------------------------------------------------------*/

void fossil_io_cube_draw(void);

void fossil_io_cube_draw_box(
    const char *id);

void fossil_io_cube_draw_frame(
    const char *id);

void fossil_io_cube_draw_line(
    int32_t x1,
    int32_t y1,
    int32_t x2,
    int32_t y2);

void fossil_io_cube_draw_text(
    const char *id);

void fossil_io_cube_draw_fill(
    const char *id);

/*----------------------------------------------------------------------------
    Layout
----------------------------------------------------------------------------*/

int32_t fossil_io_cube_layout_apply(
    const char *layout,
    const char *parent);

/*----------------------------------------------------------------------------
    Input
----------------------------------------------------------------------------*/

int32_t fossil_io_cube_poll_input(void);

int32_t fossil_io_cube_read_key(void);

int32_t fossil_io_cube_wait_event(void);

/*----------------------------------------------------------------------------
    Events
----------------------------------------------------------------------------*/

int32_t fossil_io_cube_bind(
    const char *id,
    const char *event,
    fossil_io_cube_callback callback);

int32_t fossil_io_cube_emit(
    const char *id,
    const char *event,
    void *data);

int32_t fossil_io_cube_dispatch(void);

/*----------------------------------------------------------------------------
    Filesystem Integration
----------------------------------------------------------------------------*/

int32_t fossil_io_cube_bind_file(
    const char *id,
    const char *path);

int32_t fossil_io_cube_bind_directory(
    const char *id,
    const char *path);

int32_t fossil_io_cube_refresh(
    const char *id);

int32_t fossil_io_cube_open(
    const char *id,
    const char *path);

int32_t fossil_io_cube_save(
    const char *id);

int32_t fossil_io_cube_save_as(
    const char *id,
    const char *path);

int32_t fossil_io_cube_close(
    const char *id);

/*----------------------------------------------------------------------------
    Transactions
----------------------------------------------------------------------------*/

int32_t fossil_io_cube_transaction_begin(void);

int32_t fossil_io_cube_transaction_commit(void);

int32_t fossil_io_cube_transaction_rollback(void);

/*----------------------------------------------------------------------------
    Terminal
----------------------------------------------------------------------------*/

void fossil_io_cube_clear(void);

void fossil_io_cube_refresh(void);

void fossil_io_cube_cursor_show(void);

void fossil_io_cube_cursor_hide(void);

void fossil_io_cube_set_title(
    const char *title);

#ifdef __cplusplus
}

#include <string>

namespace fossil::io
{



} // namespace fossil

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */