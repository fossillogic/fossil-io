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
#include "fossil/io/cube.h"
#include <string.h>
#include <stdlib.h>

/*-----------------------------------------------------------------------------
    Internal Limits
-----------------------------------------------------------------------------*/

#define FOSSIL_IO_CUBE_MAX_OBJECTS   1024
#define FOSSIL_IO_CUBE_MAX_EVENTS     256
#define FOSSIL_IO_CUBE_MAX_CONTEXTS    32

/*-----------------------------------------------------------------------------
    Internal Types
-----------------------------------------------------------------------------*/

typedef struct
{
    char id[FOSSIL_FILESYS_MAX_ID];
    bool active;

} fossil_io_cube_context_t;

typedef struct
{
    bool initialized;

    fossil_io_cube_object_t objects[FOSSIL_IO_CUBE_MAX_OBJECTS];
    size_t object_count;

    fossil_io_cube_event_t events[FOSSIL_IO_CUBE_MAX_EVENTS];
    size_t event_count;

    fossil_io_cube_context_t contexts[FOSSIL_IO_CUBE_MAX_CONTEXTS];
    size_t context_count;

    char current_context[FOSSIL_FILESYS_MAX_ID];

} fossil_io_cube_state_t;

/*-----------------------------------------------------------------------------
    Global State
-----------------------------------------------------------------------------*/

static fossil_io_cube_state_t g_cube;

/*-----------------------------------------------------------------------------
    Internal Helpers
-----------------------------------------------------------------------------*/

static fossil_io_cube_object_t *
cube_find_object(const char *id)
{
    size_t i;

    if (!id)
        return NULL;

    for (i = 0; i < g_cube.object_count; ++i)
    {
        if (strcmp(g_cube.objects[i].id, id) == 0)
            return &g_cube.objects[i];
    }

    return NULL;
}

static fossil_io_cube_object_t *
cube_new_object(void)
{
    if (g_cube.object_count >= FOSSIL_IO_CUBE_MAX_OBJECTS)
        return NULL;

    return &g_cube.objects[g_cube.object_count++];
}

/*-----------------------------------------------------------------------------
    Lifecycle
-----------------------------------------------------------------------------*/

int32_t fossil_io_cube_init(void)
{
    memset(&g_cube, 0, sizeof(g_cube));

    g_cube.initialized = true;

    return 0;
}

void fossil_io_cube_shutdown(void)
{
    memset(&g_cube, 0, sizeof(g_cube));
}

void fossil_io_cube_begin(void)
{
    if (!g_cube.initialized)
        return;

    fossil_io_clear_screen();
}

void fossil_io_cube_end(void)
{
    fossil_io_flush();
}

void fossil_io_cube_present(void)
{
    fossil_io_flush();
}

/*-----------------------------------------------------------------------------
    Contexts
-----------------------------------------------------------------------------*/

int32_t fossil_io_cube_context_create(const char *id)
{
    if (!id)
        return -1;

    if (g_cube.context_count >= FOSSIL_IO_CUBE_MAX_CONTEXTS)
        return -1;

    strcpy(g_cube.contexts[g_cube.context_count].id, id);
    g_cube.contexts[g_cube.context_count].active = true;

    g_cube.context_count++;

    return 0;
}

int32_t fossil_io_cube_context_destroy(const char *id)
{
    size_t i;

    for (i = 0; i < g_cube.context_count; i++)
    {
        if (!strcmp(g_cube.contexts[i].id, id))
        {
            g_cube.contexts[i].active = false;
            return 0;
        }
    }

    return -1;
}

int32_t fossil_io_cube_context_use(const char *id)
{
    if (!id)
        return -1;

    strncpy(g_cube.current_context,
            id,
            sizeof(g_cube.current_context));

    return 0;
}

/*-----------------------------------------------------------------------------
    Objects
-----------------------------------------------------------------------------*/

int32_t fossil_io_cube_create(
    const char *type,
    const char *id,
    const char *name)
{
    fossil_io_cube_object_t *obj;

    if (!type || !id)
        return -1;

    if (cube_find_object(id))
        return -1;

    obj = cube_new_object();

    if (!obj)
        return -1;

    memset(obj, 0, sizeof(*obj));

    strcpy(obj->id, id);
    strcpy(obj->type, type);

    if (name)
        strcpy(obj->name, name);

    obj->visible = true;
    obj->enabled = true;

    return 0;
}

int32_t fossil_io_cube_destroy(const char *id)
{
    fossil_io_cube_object_t *obj;

    obj = cube_find_object(id);

    if (!obj)
        return -1;

    memset(obj, 0, sizeof(*obj));

    return 0;
}

fossil_io_cube_object_t *
fossil_io_cube_find(const char *id)
{
    return cube_find_object(id);
}

/*-----------------------------------------------------------------------------
    Properties
-----------------------------------------------------------------------------*/

int32_t fossil_io_cube_set_name(
    const char *id,
    const char *name)
{
    fossil_io_cube_object_t *obj = cube_find_object(id);

    if (!obj || !name)
        return -1;

    strcpy(obj->name, name);

    return 0;
}

int32_t fossil_io_cube_set_text(
    const char *id,
    const char *text)
{
    fossil_io_cube_object_t *obj = cube_find_object(id);

    if (!obj || !text)
        return -1;

    strcpy(obj->text, text);

    return 0;
}

const char *
fossil_io_cube_get_text(const char *id)
{
    fossil_io_cube_object_t *obj = cube_find_object(id);

    if (!obj)
        return NULL;

    return obj->text;
}

int32_t fossil_io_cube_set_style(
    const char *id,
    const char *style)
{
    fossil_io_cube_object_t *obj = cube_find_object(id);

    if (!obj || !style)
        return -1;

    strcpy(obj->style, style);

    return 0;
}

/*-----------------------------------------------------------------------------
    Geometry
-----------------------------------------------------------------------------*/

int32_t fossil_io_cube_set_position(
    const char *id,
    int32_t x,
    int32_t y)
{
    fossil_io_cube_object_t *obj = cube_find_object(id);

    if (!obj)
        return -1;

    obj->x = x;
    obj->y = y;

    return 0;
}

int32_t fossil_io_cube_set_size(
    const char *id,
    int32_t w,
    int32_t h)
{
    fossil_io_cube_object_t *obj = cube_find_object(id);

    if (!obj)
        return -1;

    obj->width = w;
    obj->height = h;

    return 0;
}

/*-----------------------------------------------------------------------------
    Drawing
-----------------------------------------------------------------------------*/

void fossil_io_cube_draw_text(const char *id)
{
    fossil_io_cube_object_t *obj = cube_find_object(id);

    if (!obj || !obj->visible)
        return;

    fossil_io_move_cursor(obj->y, obj->x);

    if (obj->style[0])
        fossil_io_printf("%s%s{reset}",
                         obj->style,
                         obj->text);
    else
        fossil_io_puts(obj->text);
}

void fossil_io_cube_draw(void)
{
    size_t i;

    for (i = 0; i < g_cube.object_count; i++)
    {
        fossil_io_cube_draw_text(g_cube.objects[i].id);
    }
}

/*-----------------------------------------------------------------------------
    Filesystem
-----------------------------------------------------------------------------*/

int32_t fossil_io_cube_bind_file(
    const char *id,
    const char *path)
{
    fossil_io_cube_object_t *obj = cube_find_object(id);

    if (!obj)
        return -1;

    return fossil_io_filesys_init(
        &obj->file,
        path);
}

int32_t fossil_io_cube_refresh(
    const char *id)
{
    fossil_io_cube_object_t *obj = cube_find_object(id);

    if (!obj)
        return -1;

    return fossil_io_filesys_refresh(
        &obj->file);
}

int32_t fossil_io_cube_open(
    const char *id,
    const char *path)
{
    fossil_io_cube_object_t *obj = cube_find_object(id);
    fossil_io_filesys_file_t file;
    char buffer[FOSSIL_IO_CUBE_MAX_TEXT];

    size_t bytes;

    if (!obj)
        return -1;

    if (fossil_io_filesys_file_open(
            &file,
            path,
            "r"))
        return -1;

    bytes = fossil_io_filesys_file_read(
        &file,
        buffer,
        1,
        sizeof(buffer) - 1);

    buffer[bytes] = '\0';

    strcpy(obj->text, buffer);

    fossil_io_filesys_file_close(&file);

    fossil_io_filesys_init(
        &obj->file,
        path);

    return 0;
}

/*-----------------------------------------------------------------------------
    Input
-----------------------------------------------------------------------------*/

int32_t fossil_io_cube_poll_input(void)
{
    return fossil_io_getc(FOSSIL_STDIN);
}

/*-----------------------------------------------------------------------------
    Terminal
-----------------------------------------------------------------------------*/

void fossil_io_cube_clear(void)
{
    fossil_io_clear_screen();
}

void fossil_io_cube_flush(void)
{
    fossil_io_flush();
}

void fossil_io_cube_cursor_show(void)
{
    fossil_io_show_cursor();
}

void fossil_io_cube_cursor_hide(void)
{
    fossil_io_hide_cursor();
}
