/*
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * This file is part of the Fossil Logic project, which aims to develop high-
 * performance, cross-platform applications and libraries. The code contained
 * herein is subject to the terms and conditions defined in the project license.
 *
 * Author: Michael Gene Brockus (Dreamer)
 * Date: 07/01/2024
 *
 * Copyright (C) 2024 Fossil Logic. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include <fossil/pizza/framework.h>
#include <fossil/io/framework.h>
#include <vector>
#include <string>


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilites
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Define the test suite and add test cases
FOSSIL_SUITE(cpp_parser_suite);

// Setup function for the test suite
FOSSIL_SETUP(cpp_parser_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_parser_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(cpp_create_palette) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    FOSSIL_TEST_ASSUME(strcmp(palette->name, "test_palette") == 0, "Palette name should be 'test_palette'");
    FOSSIL_TEST_ASSUME(strcmp(palette->description, "Test Description") == 0, "Palette description should be 'Test Description'");
    FOSSIL_TEST_ASSUME(palette->commands == NULL, "Palette commands should be NULL");
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(cpp_add_command) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    FOSSIL_TEST_ASSUME(strcmp(command->name, "test_command") == 0, "Command name should be 'test_command'");
    FOSSIL_TEST_ASSUME(strcmp(command->description, "Test Command Description") == 0, "Command description should be 'Test Command Description'");
    FOSSIL_TEST_ASSUME(command->arguments == NULL, "Command arguments should be NULL");
    FOSSIL_TEST_ASSUME(palette->commands == command, "Palette commands should include the new command");
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(cpp_add_argument) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "Test Command Description");
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(command, "test_arg", FOSSIL_IO_PARSER_STRING, NULL, 0);
    FOSSIL_TEST_ASSUME(argument != NULL, "Argument should be added");
    FOSSIL_TEST_ASSUME(strcmp(argument->name, "test_arg") == 0, "Argument name should be 'test_arg'");
    FOSSIL_TEST_ASSUME(argument->type == FOSSIL_IO_PARSER_STRING, "Argument type should be STRING");
    FOSSIL_TEST_ASSUME(argument->value == NULL, "Argument value should be NULL");
    FOSSIL_TEST_ASSUME(command->arguments == argument, "Command arguments should include the new argument");
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(cpp_argument_types) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "Test Command Description");

    // Test BOOL argument
    fossil_io_parser_argument_t *bool_arg = fossil_io_parser_add_argument(command, "bool_arg", FOSSIL_IO_PARSER_BOOL, NULL, 0);
    FOSSIL_TEST_ASSUME(bool_arg != NULL, "BOOL argument should be added");
    FOSSIL_TEST_ASSUME(bool_arg->type == FOSSIL_IO_PARSER_BOOL, "BOOL argument type should be correct");

    // Test STRING argument
    fossil_io_parser_argument_t *string_arg = fossil_io_parser_add_argument(command, "string_arg", FOSSIL_IO_PARSER_STRING, NULL, 0);
    FOSSIL_TEST_ASSUME(string_arg != NULL, "STRING argument should be added");
    FOSSIL_TEST_ASSUME(string_arg->type == FOSSIL_IO_PARSER_STRING, "STRING argument type should be correct");

    // Test INT argument
    fossil_io_parser_argument_t *int_arg = fossil_io_parser_add_argument(command, "int_arg", FOSSIL_IO_PARSER_INT, NULL, 0);
    FOSSIL_TEST_ASSUME(int_arg != NULL, "INT argument should be added");
    FOSSIL_TEST_ASSUME(int_arg->type == FOSSIL_IO_PARSER_INT, "INT argument type should be correct");

    // Test FLOAT argument
    fossil_io_parser_argument_t *float_arg = fossil_io_parser_add_argument(command, "float_arg", FOSSIL_IO_PARSER_FLOAT, NULL, 0);
    FOSSIL_TEST_ASSUME(float_arg != NULL, "FLOAT argument should be added");
    FOSSIL_TEST_ASSUME(float_arg->type == FOSSIL_IO_PARSER_FLOAT, "FLOAT argument type should be correct");

    // Test DATE argument
    fossil_io_parser_argument_t *date_arg = fossil_io_parser_add_argument(command, "date_arg", FOSSIL_IO_PARSER_DATE, NULL, 0);
    FOSSIL_TEST_ASSUME(date_arg != NULL, "DATE argument should be added");
    FOSSIL_TEST_ASSUME(date_arg->type == FOSSIL_IO_PARSER_DATE, "DATE argument type should be correct");

    // Test ARRAY argument
    fossil_io_parser_argument_t *array_arg = fossil_io_parser_add_argument(command, "array_arg", FOSSIL_IO_PARSER_ARRAY, NULL, 0);
    FOSSIL_TEST_ASSUME(array_arg != NULL, "ARRAY argument should be added");
    FOSSIL_TEST_ASSUME(array_arg->type == FOSSIL_IO_PARSER_ARRAY, "ARRAY argument type should be correct");

    // Test FEATURE argument
    fossil_io_parser_argument_t *feature_arg = fossil_io_parser_add_argument(command, "feature_arg", FOSSIL_IO_PARSER_FEATURE, NULL, 0);
    FOSSIL_TEST_ASSUME(feature_arg != NULL, "FEATURE argument should be added");
    FOSSIL_TEST_ASSUME(feature_arg->type == FOSSIL_IO_PARSER_FEATURE, "FEATURE argument type should be correct");

    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(cpp_parse_command) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "Test Command Description");
    fossil_io_parser_add_argument(command, "test_arg", FOSSIL_IO_PARSER_STRING, NULL, 0);

    std::vector<std::string> argv = {"program", "test_command", "test_arg", "test_value"};
    std::vector<const char*> argv_cstr;
    for (const auto& arg : argv) {
        argv_cstr.push_back(arg.c_str());
    }
    fossil_io_parser_parse(palette, 4, const_cast<char**>(argv_cstr.data()));

    FOSSIL_TEST_ASSUME(command->arguments->value != NULL, "Argument value should be set");
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(cpp_free_palette) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");

    ASSUME_NOT_CNULL(palette);
    fossil_io_parser_add_command(palette, "test_command", "Test Command Description");
    fossil_io_parser_free(palette);
    // No explicit assumptions here, just ensuring no memory leaks or crashes
} // end case

FOSSIL_TEST(cpp_wrapper_create_palette) {
    fossil::io::Parser parser;
    fossil_io_parser_palette_t *palette = parser.create_palette("wrapper_palette", "Wrapper Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    FOSSIL_TEST_ASSUME(strcmp(palette->name, "wrapper_palette") == 0, "Palette name should be 'wrapper_palette'");
    FOSSIL_TEST_ASSUME(strcmp(palette->description, "Wrapper Test Description") == 0, "Palette description should be 'Wrapper Test Description'");
    FOSSIL_TEST_ASSUME(palette->commands == NULL, "Palette commands should be NULL");
    parser.free(palette);
} // end case

FOSSIL_TEST(cpp_wrapper_add_command) {
    fossil::io::Parser parser;
    fossil_io_parser_palette_t *palette = parser.create_palette("wrapper_palette", "Wrapper Test Description");
    fossil_io_parser_command_t *command = parser.add_command(palette, "wrapper_command", "Wrapper Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    FOSSIL_TEST_ASSUME(strcmp(command->name, "wrapper_command") == 0, "Command name should be 'wrapper_command'");
    FOSSIL_TEST_ASSUME(strcmp(command->description, "Wrapper Command Description") == 0, "Command description should be 'Wrapper Command Description'");
    FOSSIL_TEST_ASSUME(command->arguments == NULL, "Command arguments should be NULL");
    FOSSIL_TEST_ASSUME(palette->commands == command, "Palette commands should include the new command");
    parser.free(palette);
} // end case

FOSSIL_TEST(cpp_wrapper_add_argument) {
    fossil::io::Parser parser;
    fossil_io_parser_palette_t *palette = parser.create_palette("wrapper_palette", "Wrapper Test Description");
    fossil_io_parser_command_t *command = parser.add_command(palette, "wrapper_command", "Wrapper Command Description");
    fossil_io_parser_argument_t *argument = parser.add_argument(command, "wrapper_arg", FOSSIL_IO_PARSER_STRING, NULL, 0);
    FOSSIL_TEST_ASSUME(argument != NULL, "Argument should be added");
    FOSSIL_TEST_ASSUME(strcmp(argument->name, "wrapper_arg") == 0, "Argument name should be 'wrapper_arg'");
    FOSSIL_TEST_ASSUME(argument->type == FOSSIL_IO_PARSER_STRING, "Argument type should be STRING");
    FOSSIL_TEST_ASSUME(argument->value == NULL, "Argument value should be NULL");
    FOSSIL_TEST_ASSUME(command->arguments == argument, "Command arguments should include the new argument");
    parser.free(palette);
} // end case

FOSSIL_TEST(cpp_wrapper_parse_command) {
    fossil::io::Parser parser;
    fossil_io_parser_palette_t *palette = parser.create_palette("wrapper_palette", "Wrapper Test Description");
    fossil_io_parser_command_t *command = parser.add_command(palette, "wrapper_command", "Wrapper Command Description");
    parser.add_argument(command, "wrapper_arg", FOSSIL_IO_PARSER_STRING, NULL, 0);

    std::vector<std::string> argv = {"program", "wrapper_command", "wrapper_arg", "wrapper_value"};
    std::vector<const char*> argv_cstr;
    for (const auto& arg : argv) {
        argv_cstr.push_back(arg.c_str());
    }
    parser.parse(palette, 4, const_cast<char**>(argv_cstr.data()));

    FOSSIL_TEST_ASSUME(command->arguments->value != NULL, "Argument value should be set");
    parser.free(palette);
} // end case

FOSSIL_TEST(cpp_wrapper_free_palette) {
    fossil::io::Parser parser;
    fossil_io_parser_palette_t *palette = parser.create_palette("wrapper_palette", "Wrapper Test Description");

    ASSUME_NOT_CNULL(palette);
    parser.add_command(palette, "wrapper_command", "Wrapper Command Description");
    parser.free(palette);
    // No explicit assumptions here, just ensuring no memory leaks or crashes
} // end case

FOSSIL_TEST(cpp_wrapper_argument_types) {
    fossil::io::Parser parser;
    fossil_io_parser_palette_t *palette = parser.create_palette("wrapper_palette", "Wrapper Test Description");
    fossil_io_parser_command_t *command = parser.add_command(palette, "wrapper_command", "Wrapper Command Description");

    // Test BOOL argument
    fossil_io_parser_argument_t *bool_arg = parser.add_argument(command, "bool_arg", FOSSIL_IO_PARSER_BOOL, NULL, 0);
    FOSSIL_TEST_ASSUME(bool_arg != NULL, "BOOL argument should be added");
    FOSSIL_TEST_ASSUME(bool_arg->type == FOSSIL_IO_PARSER_BOOL, "BOOL argument type should be correct");

    // Test STRING argument
    fossil_io_parser_argument_t *string_arg = parser.add_argument(command, "string_arg", FOSSIL_IO_PARSER_STRING, NULL, 0);
    FOSSIL_TEST_ASSUME(string_arg != NULL, "STRING argument should be added");
    FOSSIL_TEST_ASSUME(string_arg->type == FOSSIL_IO_PARSER_STRING, "STRING argument type should be correct");

    // Test INT argument
    fossil_io_parser_argument_t *int_arg = parser.add_argument(command, "int_arg", FOSSIL_IO_PARSER_INT, NULL, 0);
    FOSSIL_TEST_ASSUME(int_arg != NULL, "INT argument should be added");
    FOSSIL_TEST_ASSUME(int_arg->type == FOSSIL_IO_PARSER_INT, "INT argument type should be correct");

    // Test FLOAT argument
    fossil_io_parser_argument_t *float_arg = parser.add_argument(command, "float_arg", FOSSIL_IO_PARSER_FLOAT, NULL, 0);
    FOSSIL_TEST_ASSUME(float_arg != NULL, "FLOAT argument should be added");
    FOSSIL_TEST_ASSUME(float_arg->type == FOSSIL_IO_PARSER_FLOAT, "FLOAT argument type should be correct");

    // Test DATE argument
    fossil_io_parser_argument_t *date_arg = parser.add_argument(command, "date_arg", FOSSIL_IO_PARSER_DATE, NULL, 0);
    FOSSIL_TEST_ASSUME(date_arg != NULL, "DATE argument should be added");
    FOSSIL_TEST_ASSUME(date_arg->type == FOSSIL_IO_PARSER_DATE, "DATE argument type should be correct");

    // Test ARRAY argument
    fossil_io_parser_argument_t *array_arg = parser.add_argument(command, "array_arg", FOSSIL_IO_PARSER_ARRAY, NULL, 0);
    FOSSIL_TEST_ASSUME(array_arg != NULL, "ARRAY argument should be added");
    FOSSIL_TEST_ASSUME(array_arg->type == FOSSIL_IO_PARSER_ARRAY, "ARRAY argument type should be correct");

    // Test FEATURE argument
    fossil_io_parser_argument_t *feature_arg = parser.add_argument(command, "feature_arg", FOSSIL_IO_PARSER_FEATURE, NULL, 0);
    FOSSIL_TEST_ASSUME(feature_arg != NULL, "FEATURE argument should be added");
    FOSSIL_TEST_ASSUME(feature_arg->type == FOSSIL_IO_PARSER_FEATURE, "FEATURE argument type should be correct");
    parser.free(palette);
} // end case

FOSSIL_TEST(cpp_wrapper_null_palette) {
    fossil::io::Parser parser;
    fossil_io_parser_palette_t *palette = NULL;
    FOSSIL_TEST_ASSUME(parser.add_command(palette, "test_command", "Test Command Description") == NULL, "Adding command to NULL palette should return NULL");
    parser.parse(palette, 0, NULL);
} // end case

FOSSIL_TEST(cpp_wrapper_empty_command_name) {
    fossil::io::Parser parser;
    fossil_io_parser_palette_t *palette = parser.create_palette("wrapper_palette", "Wrapper Test Description");
    fossil_io_parser_command_t *command = parser.add_command(palette, "", "Empty Command Name Description");
    FOSSIL_TEST_ASSUME(command == NULL, "Command with empty name should not be added");
    parser.free(palette);
} // end case

FOSSIL_TEST(cpp_wrapper_duplicate_command_name) {
    fossil::io::Parser parser;
    fossil_io_parser_palette_t *palette = parser.create_palette("wrapper_palette", "Wrapper Test Description");
    parser.add_command(palette, "wrapper_command", "Wrapper Command Description");
    fossil_io_parser_command_t *duplicate = parser.add_command(palette, "wrapper_command", "Duplicate Command Description");
    FOSSIL_TEST_ASSUME(duplicate == NULL, "Duplicate command name should not be allowed");
    parser.free(palette);
} // end case

FOSSIL_TEST(cpp_wrapper_null_argument_name) {
    fossil::io::Parser parser;
    fossil_io_parser_palette_t *palette = parser.create_palette("wrapper_palette", "Wrapper Test Description");
    fossil_io_parser_command_t *command = parser.add_command(palette, "wrapper_command", "Wrapper Command Description");
    fossil_io_parser_argument_t *argument = parser.add_argument(command, NULL, FOSSIL_IO_PARSER_STRING, NULL, 0);
    FOSSIL_TEST_ASSUME(argument == NULL, "Argument with NULL name should not be added");
    parser.free(palette);
} // end case

FOSSIL_TEST(cpp_wrapper_invalid_argument_type) {
    fossil::io::Parser parser;
    fossil_io_parser_palette_t *palette = parser.create_palette("wrapper_palette", "Wrapper Test Description");
    fossil_io_parser_command_t *command = parser.add_command(palette, "wrapper_command", "Wrapper Command Description");
    fossil_io_parser_argument_t *argument = parser.add_argument(command, "invalid_arg", FOSSIL_IO_PARSER_INVALID, NULL, 0);
    FOSSIL_TEST_ASSUME(argument == NULL, "Argument with invalid type should not be added");
    parser.free(palette);
} // end case

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_parser_test_cases) {
    FOSSIL_TEST_ADD(cpp_parser_suite, cpp_create_palette);
    FOSSIL_TEST_ADD(cpp_parser_suite, cpp_add_command);
    FOSSIL_TEST_ADD(cpp_parser_suite, cpp_add_argument);
    FOSSIL_TEST_ADD(cpp_parser_suite, cpp_parse_command);
    FOSSIL_TEST_ADD(cpp_parser_suite, cpp_free_palette);
    FOSSIL_TEST_ADD(cpp_parser_suite, cpp_argument_types);

    FOSSIL_TEST_ADD(cpp_parser_suite, cpp_wrapper_create_palette);
    FOSSIL_TEST_ADD(cpp_parser_suite, cpp_wrapper_add_command);
    FOSSIL_TEST_ADD(cpp_parser_suite, cpp_wrapper_add_argument);
    FOSSIL_TEST_ADD(cpp_parser_suite, cpp_wrapper_parse_command);
    FOSSIL_TEST_ADD(cpp_parser_suite, cpp_wrapper_free_palette);
    FOSSIL_TEST_ADD(cpp_parser_suite, cpp_wrapper_argument_types);
    FOSSIL_TEST_ADD(cpp_parser_suite, cpp_wrapper_null_palette);
    FOSSIL_TEST_ADD(cpp_parser_suite, cpp_wrapper_empty_command_name);
    FOSSIL_TEST_ADD(cpp_parser_suite, cpp_wrapper_duplicate_command_name);
    FOSSIL_TEST_ADD(cpp_parser_suite, cpp_wrapper_null_argument_name);
    FOSSIL_TEST_ADD(cpp_parser_suite, cpp_wrapper_invalid_argument_type);

    FOSSIL_TEST_REGISTER(cpp_parser_suite);
} // end of group
