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
#include <fossil/pizza/framework.h>
#include <string.h>

#include "fossil/io/framework.h"

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilites
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Define the test suite and add test cases
FOSSIL_SUITE(c_output_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_output_suite)
{
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_output_suite)
{
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_output_color_flag_toggle)
{
    int32_t original_color = FOSSIL_IO_COLOR_ENABLE;

    FOSSIL_IO_COLOR_ENABLE = 0;
    ASSUME_ITS_EQUAL_I32(0, FOSSIL_IO_COLOR_ENABLE);

    FOSSIL_IO_COLOR_ENABLE = 1;
    ASSUME_ITS_EQUAL_I32(1, FOSSIL_IO_COLOR_ENABLE);

    FOSSIL_IO_COLOR_ENABLE = original_color;
}

FOSSIL_TEST(c_test_output_color_markup_disabled)
{
    int32_t original_color = FOSSIL_IO_COLOR_ENABLE;
    char buffer[128];

    FOSSIL_IO_COLOR_ENABLE = 0;
    int result = fossil_io_sprintf(buffer, sizeof(buffer), "Status: {green}OK{reset}");
    ASSUME_ITS_TRUE(result >= 0);
    ASSUME_ITS_EQUAL_CSTR("Status: OK", buffer);

    FOSSIL_IO_COLOR_ENABLE = original_color;
}

FOSSIL_TEST(c_test_output_color_markup_enabled)
{
    int32_t original_color = FOSSIL_IO_COLOR_ENABLE;
    char buffer[128];

    FOSSIL_IO_COLOR_ENABLE = 1;
    int result = fossil_io_sprintf(buffer, sizeof(buffer), "Status: {green}OK{reset}");
    ASSUME_ITS_TRUE(result >= 0);
    ASSUME_ITS_TRUE(strstr(buffer, "\x1b[") != NULL);
    ASSUME_ITS_TRUE(strstr(buffer, "OK") != NULL);

    FOSSIL_IO_COLOR_ENABLE = original_color;
}

FOSSIL_TEST(c_test_output_output_preserves_formatting_when_disabled)
{
    int32_t original_output = FOSSIL_IO_OUTPUT_ENABLE;
    char buffer[128];

    FOSSIL_IO_OUTPUT_ENABLE = 0;
    int result = fossil_io_sprintf(buffer, sizeof(buffer), "%s", "DryRun");
    ASSUME_ITS_TRUE(result >= 0);
    ASSUME_ITS_EQUAL_CSTR("DryRun", buffer);

    FOSSIL_IO_OUTPUT_ENABLE = original_output;
}

FOSSIL_TEST(c_test_output_enable_flag_toggle)
{
    int32_t original_output = FOSSIL_IO_OUTPUT_ENABLE;

    FOSSIL_IO_OUTPUT_ENABLE = 0;
    ASSUME_ITS_EQUAL_I32(0, FOSSIL_IO_OUTPUT_ENABLE);

    FOSSIL_IO_OUTPUT_ENABLE = 1;
    ASSUME_ITS_EQUAL_I32(1, FOSSIL_IO_OUTPUT_ENABLE);

    FOSSIL_IO_OUTPUT_ENABLE = original_output;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_output_tests)
{
    FOSSIL_TEST_ADD(c_output_suite, c_test_output_color_flag_toggle);
    FOSSIL_TEST_ADD(c_output_suite, c_test_output_color_markup_disabled);
    FOSSIL_TEST_ADD(c_output_suite, c_test_output_color_markup_enabled);
    FOSSIL_TEST_ADD(c_output_suite, c_test_output_output_preserves_formatting_when_disabled);
    FOSSIL_TEST_ADD(c_output_suite, c_test_output_enable_flag_toggle);

    FOSSIL_TEST_REGISTER(c_output_suite);
}
