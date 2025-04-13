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
#include <fossil/test/framework.h>

#include "fossil/io/framework.h"


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilities
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_SUITE(cpp_device_suite);

FOSSIL_SETUP(cpp_device_suite) {
    // Setup for all device tests (keyboard, mouse, touch)
}

FOSSIL_TEARDOWN(cpp_device_suite) {
    // Teardown for device tests
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases — Keyboard Class
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_CASE(cpp_test_keyboard_register_and_unregister) {
    fossil::io::Keyboard keyboard;
    fossil_io_keyboard_event_t event = { 'a', 0, 0, 0 };
    fossil_io_keyboard_callback_t callback = (fossil_io_keyboard_callback_t)0x1;

    keyboard.register_binding(event, callback);
    ASSUME_NOT_CNULL(callback);

    keyboard.unregister_binding(event);
    ASSUME_NOT_CNULL(callback);
}

FOSSIL_TEST_CASE(cpp_test_keyboard_clear_bindings) {
    fossil::io::Keyboard keyboard;
    fossil_io_keyboard_event_t event = { 'b', 0, 0, 0 };
    fossil_io_keyboard_callback_t callback = (fossil_io_keyboard_callback_t)0x1;

    keyboard.register_binding(event, callback);
    keyboard.clear_bindings();
    ASSUME_NOT_CNULL(callback);
}

FOSSIL_TEST_CASE(cpp_test_keyboard_poll_events) {
    fossil::io::Keyboard keyboard;
    fossil_io_keyboard_event_t event = { 'c', 0, 0, 0 };
    fossil_io_keyboard_callback_t callback = (fossil_io_keyboard_callback_t)0x1;

    keyboard.register_binding(event, callback);
    keyboard.poll_events();
    keyboard.unregister_binding(event);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases — Mouse Class
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_CASE(cpp_test_mouse_register_and_unregister) {
    fossil::io::Mouse mouse;

    fossil_io_mouse_event_t event = {
        .x = 100,
        .y = 200,
        .button = 1,
        .shift = 1,
        .ctrl = 0,
        .alt = 1
    };

    fossil_io_mouse_callback_t callback = (fossil_io_mouse_callback_t)0x2;

    mouse.register_binding(event, callback);
    ASSUME_NOT_CNULL(callback);

    mouse.unregister_binding(event);
    ASSUME_NOT_CNULL(callback);
}

FOSSIL_TEST_CASE(cpp_test_mouse_clear_bindings) {
    fossil::io::Mouse mouse;

    fossil_io_mouse_event_t event = {
        .x = 150,
        .y = 250,
        .button = 2,
        .shift = 0,
        .ctrl = 1,
        .alt = 0
    };

    fossil_io_mouse_callback_t callback = (fossil_io_mouse_callback_t)0x2;

    mouse.register_binding(event, callback);
    mouse.clear_bindings();
    ASSUME_NOT_CNULL(callback);
}

FOSSIL_TEST_CASE(cpp_test_mouse_poll_events) {
    fossil::io::Mouse mouse;

    fossil_io_mouse_event_t event = {
        .x = 120,
        .y = 180,
        .button = 0,
        .shift = 0,
        .ctrl = 0,
        .alt = 0
    };

    fossil_io_mouse_callback_t callback = (fossil_io_mouse_callback_t)0x2;

    mouse.register_binding(event, callback);
    mouse.poll_events();
    mouse.unregister_binding(event);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases — Touch Class
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_CASE(cpp_test_touch_register_and_unregister) {
    fossil::io::Touch touch;

    fossil_io_touch_event_t event = {
        .x = 400,
        .y = 300,
        .touch_id = 1,
        .action = 0,
        .shift = 0,
        .ctrl = 1,
        .alt = 0
    };

    fossil_io_touch_callback_t callback = (fossil_io_touch_callback_t)0x3;

    touch.register_binding(event, callback);
    ASSUME_NOT_CNULL(callback);

    touch.unregister_binding(event);
    ASSUME_NOT_CNULL(callback);
}

FOSSIL_TEST_CASE(cpp_test_touch_clear_bindings) {
    fossil::io::Touch touch;

    fossil_io_touch_event_t event = {
        .x = 500,
        .y = 600,
        .touch_id = 2,
        .action = 1,
        .shift = 1,
        .ctrl = 0,
        .alt = 1
    };

    fossil_io_touch_callback_t callback = (fossil_io_touch_callback_t)0x3;

    touch.register_binding(event, callback);
    touch.clear_bindings();
    ASSUME_NOT_CNULL(callback);
}

FOSSIL_TEST_CASE(cpp_test_touch_poll_events) {
    fossil::io::Touch touch;

    fossil_io_touch_event_t event = {
        .x = 700,
        .y = 800,
        .touch_id = 3,
        .action = 2,
        .shift = 1,
        .ctrl = 1,
        .alt = 0
    };

    fossil_io_touch_callback_t callback = (fossil_io_touch_callback_t)0x3;

    touch.register_binding(event, callback);
    touch.poll_events();
    touch.unregister_binding(event);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_device_tests) {
    // Keyboard
    FOSSIL_TEST_ADD(cpp_device_suite, cpp_test_keyboard_register_and_unregister);
    FOSSIL_TEST_ADD(cpp_device_suite, cpp_test_keyboard_clear_bindings);
    FOSSIL_TEST_ADD(cpp_device_suite, cpp_test_keyboard_poll_events);

    // Mouse
    FOSSIL_TEST_ADD(cpp_device_suite, cpp_test_mouse_register_and_unregister);
    FOSSIL_TEST_ADD(cpp_device_suite, cpp_test_mouse_clear_bindings);
    FOSSIL_TEST_ADD(cpp_device_suite, cpp_test_mouse_poll_events);

    // Touch
    FOSSIL_TEST_ADD(cpp_device_suite, cpp_test_touch_register_and_unregister);
    FOSSIL_TEST_ADD(cpp_device_suite, cpp_test_touch_clear_bindings);
    FOSSIL_TEST_ADD(cpp_device_suite, cpp_test_touch_poll_events);

    // Register the test suite
    FOSSIL_TEST_REGISTER(cpp_device_suite);
}
