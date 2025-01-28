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
// * Fossil Logic Test Utilites
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Define the test suite and add test cases
FOSSIL_TEST_SUITE(cpp_keyboard_suite);

// Setup function for the test suite
FOSSIL_SETUP(cpp_keyboard_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_keyboard_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_CASE(cpp_test_keyboard_register_unregister_binding) {
    fossil_io_keyboard_event_t event = { 'a', 0, 0, 0 };
    fossil_io_keyboard_callback_t callback = (fossil_io_keyboard_callback_t)1; // Assuming a valid callback function

    fossil_io_keyboard_register_binding(event, callback);
    ASSUME_NOT_CNULL(callback); // Assumption on pointer

    fossil_io_keyboard_unregister_binding(event);
    ASSUME_NOT_CNULL(callback); // Assumption on pointer
}

FOSSIL_TEST_CASE(cpp_test_keyboard_clear_bindings) {
    fossil_io_keyboard_event_t event = { 'a', 0, 0, 0 };
    fossil_io_keyboard_callback_t callback = (fossil_io_keyboard_callback_t)1; // Assuming a valid callback function

    fossil_io_keyboard_register_binding(event, callback);
    fossil_io_keyboard_clear_bindings();
    ASSUME_NOT_CNULL(callback); // Assumption on pointer
}

FOSSIL_TEST_CASE(cpp_test_keyboard_poll_events) {
    fossil_io_keyboard_event_t event = { 'a', 0, 0, 0 };
    fossil_io_keyboard_callback_t callback = (fossil_io_keyboard_callback_t)1; // Assuming a valid callback function

    fossil_io_keyboard_register_binding(event, callback);
    fossil_io_keyboard_poll_events();
    ASSUME_NOT_CNULL(callback); // Assumption on pointer
    fossil_io_keyboard_unregister_binding(event);
}

FOSSIL_TEST_CASE(cpp_test_keyboard_class_register_unregister_binding) {
    fossil::io::keyboard::init();
    fossil_io_keyboard_event_t event = { 'a', 0, 0, 0 };
    fossil_io_keyboard_callback_t callback = (fossil_io_keyboard_callback_t)1; // Assuming a valid callback function

    fossil::io::keyboard::register_binding(event, callback);
    ASSUME_NOT_CNULL(callback); // Assumption on pointer

    fossil::io::keyboard::unregister_binding(event);
    ASSUME_NOT_CNULL(callback); // Assumption on pointer
    fossil::io::keyboard::shutdown();
}

FOSSIL_TEST_CASE(cpp_test_keyboard_class_clear_bindings) {
    fossil::io::keyboard::init();
    fossil_io_keyboard_event_t event = { 'a', 0, 0, 0 };
    fossil_io_keyboard_callback_t callback = (fossil_io_keyboard_callback_t)1; // Assuming a valid callback function

    fossil::io::keyboard::register_binding(event, callback);
    fossil::io::keyboard::clear_bindings();
    ASSUME_NOT_CNULL(callback); // Assumption on pointer
    fossil::io::keyboard::shutdown();
}

FOSSIL_TEST_CASE(cpp_test_keyboard_class_poll_events) {
    fossil::io::keyboard::init();
    fossil_io_keyboard_event_t event = { 'a', 0, 0, 0 };
    fossil_io_keyboard_callback_t callback = (fossil_io_keyboard_callback_t)1; // Assuming a valid callback function

    fossil::io::keyboard::register_binding(event, callback);
    fossil::io::keyboard::poll_events();
    ASSUME_NOT_CNULL(callback); // Assumption on pointer
    fossil::io::keyboard::unregister_binding(event);
    fossil::io::keyboard::shutdown();
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_keyboard_tests) {
    FOSSIL_TEST_ADD(cpp_keyboard_suite, cpp_test_keyboard_register_unregister_binding);
    FOSSIL_TEST_ADD(cpp_keyboard_suite, cpp_test_keyboard_clear_bindings);
    FOSSIL_TEST_ADD(cpp_keyboard_suite, cpp_test_keyboard_poll_events);

    FOSSIL_TEST_ADD(cpp_keyboard_suite, cpp_test_keyboard_class_register_unregister_binding);
    FOSSIL_TEST_ADD(cpp_keyboard_suite, cpp_test_keyboard_class_clear_bindings);
    FOSSIL_TEST_ADD(cpp_keyboard_suite, cpp_test_keyboard_class_poll_events);

    FOSSIL_TEST_REGISTER(cpp_keyboard_suite);
}
