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
#include <fossil/maip/framework.h>

#include "fossil/io/framework.h"

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilites
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_SUITE(c_cube_suite);

FOSSIL_SETUP(c_cube_suite)

{

    ASSUME_ITS_EQUAL_I32(0, fossil_io_cube_init());

}

FOSSIL_TEARDOWN(c_cube_suite)

{

    fossil_io_cube_shutdown();
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

/*----------------------------------------------------------

    Initialization

----------------------------------------------------------*/

FOSSIL_TEST(c_test_cube_initialize)

{

    ASSUME_ITS_EQUAL_I32(0, fossil_io_cube_init());

    fossil_io_cube_shutdown();

}

/*----------------------------------------------------------

    Context

----------------------------------------------------------*/

FOSSIL_TEST(c_test_cube_context_create_use_destroy)

{

    ASSUME_ITS_EQUAL_I32(0,

        fossil_io_cube_context_create("main"));

    ASSUME_ITS_EQUAL_I32(0,

        fossil_io_cube_context_use("main"));

    ASSUME_ITS_EQUAL_I32(0,

        fossil_io_cube_context_destroy("main"));

}

/*----------------------------------------------------------

    Object Creation

----------------------------------------------------------*/

FOSSIL_TEST(c_test_cube_create_find_destroy)

{

    fossil_io_cube_object_t *obj;

    ASSUME_ITS_EQUAL_I32(0,

        fossil_io_cube_create(

            "window",

            "main_window",

            "Main"));

    obj = fossil_io_cube_find("main_window");

    ASSUME_ITS_TRUE(obj != NULL);

    ASSUME_ITS_EQUAL_CSTR("main_window", obj->id);

    ASSUME_ITS_EQUAL_CSTR("Main", obj->name);

    ASSUME_ITS_EQUAL_CSTR("window", obj->type);

    ASSUME_ITS_EQUAL_I32(0,

        fossil_io_cube_destroy("main_window"));

    ASSUME_ITS_TRUE(

        fossil_io_cube_find("main_window") == NULL);

}

/*----------------------------------------------------------

    Clone

----------------------------------------------------------*/

FOSSIL_TEST(c_test_cube_clone)

{

    fossil_io_cube_object_t *obj;

    fossil_io_cube_create(

        "label",

        "label1",

        "Label");

    fossil_io_cube_set_text(

        "label1",

        "Hello");

    ASSUME_ITS_EQUAL_I32(

        0,

        fossil_io_cube_clone(

            "label1",

            "label2"));

    obj = fossil_io_cube_find("label2");

    ASSUME_ITS_TRUE(obj != NULL);

    ASSUME_ITS_EQUAL_CSTR("Hello", obj->text);

}

/*----------------------------------------------------------

    Parent

----------------------------------------------------------*/

FOSSIL_TEST(c_test_cube_parent)

{

    fossil_io_cube_create(

        "window",

        "parent",

        "Parent");

    fossil_io_cube_create(

        "button",

        "child",

        "Child");

    ASSUME_ITS_EQUAL_I32(

        0,

        fossil_io_cube_set_parent(

            "child",

            "parent"));

    ASSUME_ITS_EQUAL_CSTR(

        "parent",

        fossil_io_cube_get_parent("child"));

}

/*----------------------------------------------------------

    Text

----------------------------------------------------------*/

FOSSIL_TEST(c_test_cube_text)

{

    fossil_io_cube_create(

        "label",

        "label",

        "Label");

    fossil_io_cube_set_text(

        "label",

        "Hello");

    fossil_io_cube_append_text(

        "label",

        " World");

    ASSUME_ITS_EQUAL_CSTR(

        "Hello World",

        fossil_io_cube_get_text("label"));

}

/*----------------------------------------------------------

    Style

----------------------------------------------------------*/

FOSSIL_TEST(c_test_cube_style)

{

    fossil_io_cube_object_t *obj;

    fossil_io_cube_create(

        "button",

        "btn",

        "Button");

    fossil_io_cube_set_style(

        "btn",

        "{blue,bold}");

    obj = fossil_io_cube_find("btn");

    ASSUME_ITS_EQUAL_CSTR(

        "{blue,bold}",

        obj->style);

}

/*----------------------------------------------------------

    Geometry

----------------------------------------------------------*/

FOSSIL_TEST(c_test_cube_geometry)

{

    fossil_io_cube_object_t *obj;

    fossil_io_cube_create(

        "panel",

        "panel",

        "Panel");

    fossil_io_cube_set_position(

        "panel",

        10,

        20);

    fossil_io_cube_set_size(

        "panel",

        80,

        25);

    obj = fossil_io_cube_find("panel");

    ASSUME_ITS_EQUAL_I32(10, obj->x);

    ASSUME_ITS_EQUAL_I32(20, obj->y);

    ASSUME_ITS_EQUAL_I32(80, obj->width);

    ASSUME_ITS_EQUAL_I32(25, obj->height);

}

/*----------------------------------------------------------

    Visibility

----------------------------------------------------------*/

FOSSIL_TEST(c_test_cube_visibility)

{

    fossil_io_cube_object_t *obj;

    fossil_io_cube_create(

        "panel",

        "panel",

        "Panel");

    fossil_io_cube_hide("panel");

    obj = fossil_io_cube_find("panel");

    ASSUME_ITS_FALSE(obj->visible);

    fossil_io_cube_show("panel");

    ASSUME_ITS_TRUE(obj->visible);

}

/*----------------------------------------------------------

    Enable / Disable

----------------------------------------------------------*/

FOSSIL_TEST(c_test_cube_enable_disable)

{

    fossil_io_cube_object_t *obj;

    fossil_io_cube_create(

        "button",

        "btn",

        "Button");

    fossil_io_cube_disable("btn");

    obj = fossil_io_cube_find("btn");

    ASSUME_ITS_FALSE(obj->enabled);

    fossil_io_cube_enable("btn");

    ASSUME_ITS_TRUE(obj->enabled);

}

/*----------------------------------------------------------

    Focus

----------------------------------------------------------*/

FOSSIL_TEST(c_test_cube_focus)

{

    fossil_io_cube_object_t *obj;

    fossil_io_cube_create(

        "textbox",

        "txt",

        "Textbox");

    fossil_io_cube_focus("txt");

    obj = fossil_io_cube_find("txt");

    ASSUME_ITS_TRUE(obj->focused);

    fossil_io_cube_blur("txt");

    ASSUME_ITS_FALSE(obj->focused);

}

/*----------------------------------------------------------

    Filesystem Binding

----------------------------------------------------------*/

FOSSIL_TEST(c_test_cube_bind_file)

{

    fossil_io_cube_create(

        "editor",

        "editor",

        "Editor");

    ASSUME_ITS_EQUAL_I32(

        0,

        fossil_io_cube_bind_file(

            "editor",

            "."));

    ASSUME_ITS_EQUAL_I32(

        0,

        fossil_io_cube_refresh_object("editor"));

}

/*----------------------------------------------------------

    Transactions

----------------------------------------------------------*/

FOSSIL_TEST(c_test_cube_transactions)

{

    ASSUME_ITS_EQUAL_I32(

        0,

        fossil_io_cube_transaction_begin());

    fossil_io_cube_create(

        "window",

        "win",

        "Window");

    ASSUME_ITS_EQUAL_I32(

        0,

        fossil_io_cube_transaction_commit())
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_cube_tests)
{
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_initialize);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_context_create_use_destroy);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_create_find_destroy);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_clone);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_parent);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_text);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_style);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_geometry);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_visibility);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_enable_disable);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_focus);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_bind_file);
    FOSSIL_ADD_TEST(c_cube_suite, c_test_cube_transactions);
    
    FOSSIL_ADD_SUITE(c_cube_suite);

}
