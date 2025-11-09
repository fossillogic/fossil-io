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


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilites
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Define the test suite and add test cases
FOSSIL_SUITE(c_parser_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_parser_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_parser_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_create_palette) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    FOSSIL_TEST_ASSUME(strcmp(palette->name, "test_palette") == 0, "Palette name should be 'test_palette'");
    FOSSIL_TEST_ASSUME(strcmp(palette->description, "Test Description") == 0, "Palette description should be 'Test Description'");
    FOSSIL_TEST_ASSUME(palette->commands == NULL, "Palette commands should be NULL");
    fossil_io_parser_free(palette);
    palette = NULL;
} // end case

FOSSIL_TEST(c_add_command) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    FOSSIL_TEST_ASSUME(strcmp(command->name, "test_command") == 0, "Command name should be 'test_command'");
    FOSSIL_TEST_ASSUME(strcmp(command->description, "Test Command Description") == 0, "Command description should be 'Test Command Description'");
    FOSSIL_TEST_ASSUME(command->arguments == NULL, "Command arguments should be NULL");
    FOSSIL_TEST_ASSUME(palette->commands == command, "Palette commands should include the new command");
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_add_argument) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(command, "test_arg", "ta", FOSSIL_IO_PARSER_STRING, NULL, 0);
    FOSSIL_TEST_ASSUME(argument != NULL, "Argument should be added");
    FOSSIL_TEST_ASSUME(strcmp(argument->name, "test_arg") == 0, "Argument name should be 'test_arg'");
    FOSSIL_TEST_ASSUME(argument->type == FOSSIL_IO_PARSER_STRING, "Argument type should be STRING");
    FOSSIL_TEST_ASSUME(argument->value == NULL, "Argument value should be NULL");
    FOSSIL_TEST_ASSUME(command->arguments == argument, "Command arguments should include the new argument");
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_parse_command) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(command, "test_arg", "ta", FOSSIL_IO_PARSER_STRING, NULL, 0);
    FOSSIL_TEST_ASSUME(argument != NULL, "Argument should be added");

    char *argv[] = {"program", "test_command", "test_arg", "test_value"};
    fossil_io_parser_parse(palette, 4, argv);

    FOSSIL_TEST_ASSUME(command->arguments->value != NULL, "Argument value should be set");
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_free_palette) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_free(palette);
    // Test passes if no crash occurs during free
    FOSSIL_TEST_ASSUME(1, "Free operation completed without crash");
} // end case

FOSSIL_TEST(c_argument_types) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");

    // Test BOOL argument
    fossil_io_parser_argument_t *bool_arg = fossil_io_parser_add_argument(command, "bool_arg", "ba", FOSSIL_IO_PARSER_BOOL, NULL, 0);
    FOSSIL_TEST_ASSUME(bool_arg != NULL, "BOOL argument should be added");
    FOSSIL_TEST_ASSUME(bool_arg->type == FOSSIL_IO_PARSER_BOOL, "BOOL argument type should be correct");

    // Test STRING argument
    fossil_io_parser_argument_t *string_arg = fossil_io_parser_add_argument(command, "string_arg", "sa", FOSSIL_IO_PARSER_STRING, NULL, 0);
    FOSSIL_TEST_ASSUME(string_arg != NULL, "STRING argument should be added");
    FOSSIL_TEST_ASSUME(string_arg->type == FOSSIL_IO_PARSER_STRING, "STRING argument type should be correct");

    // Test INT argument
    fossil_io_parser_argument_t *int_arg = fossil_io_parser_add_argument(command, "int_arg", "ia", FOSSIL_IO_PARSER_INT, NULL, 0);
    FOSSIL_TEST_ASSUME(int_arg != NULL, "INT argument should be added");
    FOSSIL_TEST_ASSUME(int_arg->type == FOSSIL_IO_PARSER_INT, "INT argument type should be correct");

    // Test FLOAT argument
    fossil_io_parser_argument_t *float_arg = fossil_io_parser_add_argument(command, "float_arg", "fa", FOSSIL_IO_PARSER_FLOAT, NULL, 0);
    FOSSIL_TEST_ASSUME(float_arg != NULL, "FLOAT argument should be added");
    FOSSIL_TEST_ASSUME(float_arg->type == FOSSIL_IO_PARSER_FLOAT, "FLOAT argument type should be correct");

    // Test DATE argument
    fossil_io_parser_argument_t *date_arg = fossil_io_parser_add_argument(command, "date_arg", "da", FOSSIL_IO_PARSER_DATE, NULL, 0);
    FOSSIL_TEST_ASSUME(date_arg != NULL, "DATE argument should be added");
    FOSSIL_TEST_ASSUME(date_arg->type == FOSSIL_IO_PARSER_DATE, "DATE argument type should be correct");

    // Test ARRAY argument
    fossil_io_parser_argument_t *array_arg = fossil_io_parser_add_argument(command, "array_arg", "aa", FOSSIL_IO_PARSER_ARRAY, NULL, 0);
    FOSSIL_TEST_ASSUME(array_arg != NULL, "ARRAY argument should be added");
    FOSSIL_TEST_ASSUME(array_arg->type == FOSSIL_IO_PARSER_ARRAY, "ARRAY argument type should be correct");

    // Test FEATURE argument
    fossil_io_parser_argument_t *feature_arg = fossil_io_parser_add_argument(command, "feature_arg", "fea", FOSSIL_IO_PARSER_FEATURE, NULL, 0);
    FOSSIL_TEST_ASSUME(feature_arg != NULL, "FEATURE argument should be added");
    FOSSIL_TEST_ASSUME(feature_arg->type == FOSSIL_IO_PARSER_FEATURE, "FEATURE argument type should be correct");

    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_null_palette) {
    fossil_io_parser_palette_t *palette = NULL;
    FOSSIL_TEST_ASSUME(fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description") == NULL, "Adding command to NULL palette should return NULL");
    fossil_io_parser_parse(palette, 0, NULL);
    FOSSIL_TEST_ASSUME(1, "Parse with NULL palette completed without crash");
} // end case

FOSSIL_TEST(c_empty_command_name) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "", "e", "Empty Command Name Description");
    FOSSIL_TEST_ASSUME(command == NULL, "Command with empty name should not be added");
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_duplicate_command_name) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *first_command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(first_command != NULL, "First command should be added");
    fossil_io_parser_command_t *duplicate = fossil_io_parser_add_command(palette, "test_command", "tc2", "Duplicate Command Description");
    FOSSIL_TEST_ASSUME(duplicate == NULL, "Duplicate command name should not be allowed");
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_null_argument_name) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(command, NULL, "na", FOSSIL_IO_PARSER_STRING, NULL, 0);
    FOSSIL_TEST_ASSUME(argument == NULL, "Argument with NULL name should not be added");
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_invalid_argument_type) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(command, "invalid_arg", "iv", FOSSIL_IO_PARSER_INVALID, NULL, 0);
    FOSSIL_TEST_ASSUME(argument == NULL, "Argument with invalid type should not be added");
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_create_palette_null_inputs) {
    fossil_io_parser_palette_t *palette1 = fossil_io_parser_create_palette(NULL, "Test Description");
    FOSSIL_TEST_ASSUME(palette1 == NULL, "Palette should not be created with NULL name");
    
    fossil_io_parser_palette_t *palette2 = fossil_io_parser_create_palette("test_palette", NULL);
    FOSSIL_TEST_ASSUME(palette2 == NULL, "Palette should not be created with NULL description");
    
    fossil_io_parser_palette_t *palette3 = fossil_io_parser_create_palette(NULL, NULL);
    FOSSIL_TEST_ASSUME(palette3 == NULL, "Palette should not be created with NULL name and description");
} // end case

FOSSIL_TEST(c_add_command_null_inputs) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    
    fossil_io_parser_command_t *command1 = fossil_io_parser_add_command(palette, NULL, "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command1 == NULL, "Command should not be added with NULL name");
    
    fossil_io_parser_command_t *command2 = fossil_io_parser_add_command(palette, "test_command", "tc", NULL);
    FOSSIL_TEST_ASSUME(command2 == NULL, "Command should not be added with NULL description");
    
    fossil_io_parser_command_t *command3 = fossil_io_parser_add_command(NULL, "test_command", "tc", "Test Description");
    FOSSIL_TEST_ASSUME(command3 == NULL, "Command should not be added to NULL palette");
    
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_add_command_with_short_name) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added successfully");
    FOSSIL_TEST_ASSUME(strcmp(command->short_name, "tc") == 0, "Command short name should be 'tc'");
    
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_duplicate_command_short_name) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *first_command = fossil_io_parser_add_command(palette, "test_command1", "tc", "Test Command Description 1");
    FOSSIL_TEST_ASSUME(first_command != NULL, "First command should be added");
    fossil_io_parser_command_t *duplicate = fossil_io_parser_add_command(palette, "test_command2", "tc", "Test Command Description 2");
    
    FOSSIL_TEST_ASSUME(duplicate == NULL, "Duplicate command short name should not be allowed");
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_add_argument_null_command) {
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(NULL, "test_arg", "ta", FOSSIL_IO_PARSER_STRING, NULL, 0);
    FOSSIL_TEST_ASSUME(argument == NULL, "Argument should not be added to NULL command");
} // end case

FOSSIL_TEST(c_add_argument_empty_name) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(command, "", "ea", FOSSIL_IO_PARSER_STRING, NULL, 0);
    
    FOSSIL_TEST_ASSUME(argument == NULL, "Argument with empty name should not be added");
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_duplicate_argument_name) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_argument_t *first_arg = fossil_io_parser_add_argument(command, "test_arg", "ta1", FOSSIL_IO_PARSER_STRING, NULL, 0);
    FOSSIL_TEST_ASSUME(first_arg != NULL, "First argument should be added");
    fossil_io_parser_argument_t *duplicate = fossil_io_parser_add_argument(command, "test_arg", "ta2", FOSSIL_IO_PARSER_INT, NULL, 0);
    
    FOSSIL_TEST_ASSUME(duplicate == NULL, "Duplicate argument name should not be allowed");
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_duplicate_argument_short_name) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_argument_t *first_arg = fossil_io_parser_add_argument(command, "test_arg1", "ta", FOSSIL_IO_PARSER_STRING, NULL, 0);
    FOSSIL_TEST_ASSUME(first_arg != NULL, "First argument should be added");
    fossil_io_parser_argument_t *duplicate = fossil_io_parser_add_argument(command, "test_arg2", "ta", FOSSIL_IO_PARSER_INT, NULL, 0);
    
    FOSSIL_TEST_ASSUME(duplicate == NULL, "Duplicate argument short name should not be allowed");
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_parse_null_palette) {
    char *argv[] = {"program", "test_command"};
    fossil_io_parser_parse(NULL, 2, argv);
    FOSSIL_TEST_ASSUME(1, "Parse with NULL palette completed without crash");
} // end case

FOSSIL_TEST(c_parse_null_argv) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_parse(palette, 2, NULL);
    fossil_io_parser_free(palette);
    FOSSIL_TEST_ASSUME(1, "Parse with NULL argv completed without crash");
} // end case

FOSSIL_TEST(c_parse_zero_argc) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    char *argv[] = {"program"};
    fossil_io_parser_parse(palette, 1, argv);
    fossil_io_parser_free(palette);
    FOSSIL_TEST_ASSUME(1, "Parse with zero argc completed without crash");
} // end case

FOSSIL_TEST(c_parse_help_flag) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    
    char *argv[] = {"program", "--help"};
    fossil_io_parser_parse(palette, 2, argv);
    
    fossil_io_parser_free(palette);
    FOSSIL_TEST_ASSUME(1, "Help flag parsing completed without crash");
} // end case

FOSSIL_TEST(c_parse_version_flag) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    
    char *argv[] = {"program", "--version"};
    fossil_io_parser_parse(palette, 2, argv);
    
    fossil_io_parser_free(palette);
    FOSSIL_TEST_ASSUME(1, "Version flag parsing completed without crash");
} // end case

FOSSIL_TEST(c_parse_dry_run_flag) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    
    char *argv[] = {"program", "--dry-run", "test_command"};
    fossil_io_parser_parse(palette, 3, argv);
    
    FOSSIL_TEST_ASSUME(FOSSIL_CLI_TOGGLE_DRY_RUN == 1, "Dry-run flag should be set");
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_parse_verbose_flag) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    
    char *argv[] = {"program", "--verbose", "test_command"};
    fossil_io_parser_parse(palette, 3, argv);
    
    FOSSIL_TEST_ASSUME(FOSSIL_CLI_TOGGLE_VERBOSE == 1, "Verbose flag should be set");
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_parse_unknown_command) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "valid_command", "vc", "Valid Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Valid command should be added");
    
    char *argv[] = {"program", "unknown_command"};
    fossil_io_parser_parse(palette, 2, argv);
    
    fossil_io_parser_free(palette);
    FOSSIL_TEST_ASSUME(1, "Unknown command parsing completed without crash");
} // end case

FOSSIL_TEST(c_argument_with_combo_options) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    
    char *combo_options[] = {"option1", "option2", "option3"};
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(command, "combo_arg", "ca", FOSSIL_IO_PARSER_STRING, combo_options, 3);
    
    FOSSIL_TEST_ASSUME(argument != NULL, "Argument with combo options should be added");
    FOSSIL_TEST_ASSUME(argument->combo_options == combo_options, "Combo options should be set correctly");
    FOSSIL_TEST_ASSUME(argument->combo_count == 3, "Combo count should be 3");
    
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_free_null_palette) {
    fossil_io_parser_free(NULL);
    FOSSIL_TEST_ASSUME(1, "Free NULL palette completed without crash");
} // end case

FOSSIL_TEST(c_multiple_commands_in_palette) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    
    fossil_io_parser_command_t *command1 = fossil_io_parser_add_command(palette, "command1", "c1", "First Command");
    fossil_io_parser_command_t *command2 = fossil_io_parser_add_command(palette, "command2", "c2", "Second Command");
    fossil_io_parser_command_t *command3 = fossil_io_parser_add_command(palette, "command3", "c3", "Third Command");
    
    FOSSIL_TEST_ASSUME(command1 != NULL, "First command should be added");
    FOSSIL_TEST_ASSUME(command2 != NULL, "Second command should be added");
    FOSSIL_TEST_ASSUME(command3 != NULL, "Third command should be added");
    
    // Verify the linked list structure
    FOSSIL_TEST_ASSUME(palette->commands == command3, "Latest command should be at the head");
    FOSSIL_TEST_ASSUME(command3->next == command2, "Command3 should point to command2");
    FOSSIL_TEST_ASSUME(command2->next == command1, "Command2 should point to command1");
    FOSSIL_TEST_ASSUME(command1->next == NULL, "Command1 should be the last");
    
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_parse_bool_argument_true) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(command, "bool_arg", "ba", FOSSIL_IO_PARSER_BOOL, NULL, 0);
    FOSSIL_TEST_ASSUME(argument != NULL, "Argument should be added");
    
    char *argv[] = {"program", "test_command", "bool_arg", "true"};
    fossil_io_parser_parse(palette, 4, argv);
    
    FOSSIL_TEST_ASSUME(command->arguments->value != NULL, "Boolean argument value should be set");
    FOSSIL_TEST_ASSUME(*(int*)command->arguments->value == 1, "Boolean argument should be true");
    
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_parse_bool_argument_false) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(command, "bool_arg", "ba", FOSSIL_IO_PARSER_BOOL, NULL, 0);
    FOSSIL_TEST_ASSUME(argument != NULL, "Argument should be added");
    
    char *argv[] = {"program", "test_command", "bool_arg", "false"};
    fossil_io_parser_parse(palette, 4, argv);
    
    FOSSIL_TEST_ASSUME(command->arguments->value != NULL, "Boolean argument value should be set");
    FOSSIL_TEST_ASSUME(*(int*)command->arguments->value == 0, "Boolean argument should be false");
    
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_parse_int_argument) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(command, "int_arg", "ia", FOSSIL_IO_PARSER_INT, NULL, 0);
    FOSSIL_TEST_ASSUME(argument != NULL, "Argument should be added");
    
    char *argv[] = {"program", "test_command", "int_arg", "42"};
    fossil_io_parser_parse(palette, 4, argv);
    
    FOSSIL_TEST_ASSUME(command->arguments->value != NULL, "Integer argument value should be set");
    FOSSIL_TEST_ASSUME(*(int*)command->arguments->value == 42, "Integer argument should be 42");
    
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_parse_uint_argument) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(command, "uint_arg", "ua", FOSSIL_IO_PARSER_UINT, NULL, 0);
    FOSSIL_TEST_ASSUME(argument != NULL, "Argument should be added");
    
    char *argv[] = {"program", "test_command", "uint_arg", "100"};
    fossil_io_parser_parse(palette, 4, argv);
    
    FOSSIL_TEST_ASSUME(command->arguments->value != NULL, "Unsigned integer argument value should be set");
    FOSSIL_TEST_ASSUME(*(unsigned int*)command->arguments->value == 100, "Unsigned integer argument should be 100");
    
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_parse_float_argument) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(command, "float_arg", "fa", FOSSIL_IO_PARSER_FLOAT, NULL, 0);
    FOSSIL_TEST_ASSUME(argument != NULL, "Argument should be added");
    
    char *argv[] = {"program", "test_command", "float_arg", "3.14"};
    fossil_io_parser_parse(palette, 4, argv);
    
    FOSSIL_TEST_ASSUME(command->arguments->value != NULL, "Float argument value should be set");
    FOSSIL_TEST_ASSUME(fabs(*(float*)command->arguments->value - 3.14f) < 0.001f, "Float argument should be approximately 3.14");
    
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_parse_hex_argument) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(command, "hex_arg", "ha", FOSSIL_IO_PARSER_HEX, NULL, 0);
    FOSSIL_TEST_ASSUME(argument != NULL, "Argument should be added");
    
    char *argv[] = {"program", "test_command", "hex_arg", "0xFF"};
    fossil_io_parser_parse(palette, 4, argv);
    
    FOSSIL_TEST_ASSUME(command->arguments->value != NULL, "Hex argument value should be set");
    FOSSIL_TEST_ASSUME(*(unsigned int*)command->arguments->value == 255, "Hex argument should be 255 (0xFF)");
    
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_parse_oct_argument) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(command, "oct_arg", "oa", FOSSIL_IO_PARSER_OCT, NULL, 0);
    FOSSIL_TEST_ASSUME(argument != NULL, "Argument should be added");
    
    char *argv[] = {"program", "test_command", "oct_arg", "0777"};
    fossil_io_parser_parse(palette, 4, argv);
    
    FOSSIL_TEST_ASSUME(command->arguments->value != NULL, "Octal argument value should be set");
    FOSSIL_TEST_ASSUME(*(unsigned int*)command->arguments->value == 511, "Octal argument should be 511 (0777)");
    
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_parse_help_with_specific_command) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    
    char *argv[] = {"program", "--help", "test_command"};
    fossil_io_parser_parse(palette, 3, argv);
    
    fossil_io_parser_free(palette);
    FOSSIL_TEST_ASSUME(1, "Help with specific command parsing completed without crash");
} // end case

FOSSIL_TEST(c_parse_color_enable) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    
    char *argv[] = {"program", "color=enable"};
    fossil_io_parser_parse(palette, 2, argv);
    
    FOSSIL_TEST_ASSUME(FOSSIL_IO_COLOR_ENABLE == 1, "Color should be enabled");
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_parse_color_disable) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    
    char *argv[] = {"program", "color=disable"};
    fossil_io_parser_parse(palette, 2, argv);
    
    FOSSIL_TEST_ASSUME(FOSSIL_IO_COLOR_ENABLE == 0, "Color should be disabled");
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_command_with_null_short_name) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", NULL, "Test Command Description");
    
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added with NULL short name");
    FOSSIL_TEST_ASSUME(command->short_name == NULL, "Command short name should be NULL");
    
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_argument_with_null_short_name) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(command, "test_arg", NULL, FOSSIL_IO_PARSER_STRING, NULL, 0);
    
    FOSSIL_TEST_ASSUME(argument != NULL, "Argument should be added with NULL short name");
    FOSSIL_TEST_ASSUME(argument->short_name == NULL, "Argument short name should be NULL");
    
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_multiple_arguments_per_command) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    
    fossil_io_parser_argument_t *arg1 = fossil_io_parser_add_argument(command, "arg1", "a1", FOSSIL_IO_PARSER_STRING, NULL, 0);
    fossil_io_parser_argument_t *arg2 = fossil_io_parser_add_argument(command, "arg2", "a2", FOSSIL_IO_PARSER_INT, NULL, 0);
    fossil_io_parser_argument_t *arg3 = fossil_io_parser_add_argument(command, "arg3", "a3", FOSSIL_IO_PARSER_BOOL, NULL, 0);
    
    FOSSIL_TEST_ASSUME(arg1 != NULL, "First argument should be added");
    FOSSIL_TEST_ASSUME(arg2 != NULL, "Second argument should be added");
    FOSSIL_TEST_ASSUME(arg3 != NULL, "Third argument should be added");
    
    // Verify linked list structure
    FOSSIL_TEST_ASSUME(command->arguments == arg3, "Latest argument should be at the head");
    FOSSIL_TEST_ASSUME(arg3->next == arg2, "Arg3 should point to arg2");
    FOSSIL_TEST_ASSUME(arg2->next == arg1, "Arg2 should point to arg1");
    FOSSIL_TEST_ASSUME(arg1->next == NULL, "Arg1 should be the last");
    
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_parse_missing_bool_value) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(command, "bool_arg", "ba", FOSSIL_IO_PARSER_BOOL, NULL, 0);
    FOSSIL_TEST_ASSUME(argument != NULL, "Argument should be added");
    
    char *argv[] = {"program", "test_command", "bool_arg"};
    fossil_io_parser_parse(palette, 3, argv);
    
    fossil_io_parser_free(palette);
    FOSSIL_TEST_ASSUME(1, "Parse with missing bool value completed without crash");
} // end case

FOSSIL_TEST(c_parse_missing_string_value) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(command, "string_arg", "sa", FOSSIL_IO_PARSER_STRING, NULL, 0);
    FOSSIL_TEST_ASSUME(argument != NULL, "Argument should be added");
    
    char *argv[] = {"program", "test_command", "string_arg"};
    fossil_io_parser_parse(palette, 3, argv);
    
    fossil_io_parser_free(palette);
    FOSSIL_TEST_ASSUME(1, "Parse with missing string value completed without crash");
} // end case

FOSSIL_TEST(c_parse_missing_int_value) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(command, "int_arg", "ia", FOSSIL_IO_PARSER_INT, NULL, 0);
    FOSSIL_TEST_ASSUME(argument != NULL, "Argument should be added");
    
    char *argv[] = {"program", "test_command", "int_arg"};
    fossil_io_parser_parse(palette, 3, argv);
    
    fossil_io_parser_free(palette);
    FOSSIL_TEST_ASSUME(1, "Parse with missing int value completed without crash");
} // end case

FOSSIL_TEST(c_parse_bool_yes_no) {
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    fossil_io_parser_argument_t *argument = fossil_io_parser_add_argument(command, "bool_arg", "ba", FOSSIL_IO_PARSER_BOOL, NULL, 0);
    FOSSIL_TEST_ASSUME(argument != NULL, "Argument should be added");
    
    char *argv[] = {"program", "test_command", "bool_arg", "yes"};
    fossil_io_parser_parse(palette, 4, argv);
    
    FOSSIL_TEST_ASSUME(command->arguments->value != NULL, "Boolean argument value should be set");
    FOSSIL_TEST_ASSUME(*(int*)command->arguments->value == 1, "Boolean argument should be true for 'yes'");
    
    fossil_io_parser_free(palette);
} // end case

FOSSIL_TEST(c_parse_combined_flags) {
    // Reset global flags
    FOSSIL_CLI_TOGGLE_DRY_RUN = 0;
    FOSSIL_CLI_TOGGLE_VERBOSE = 0;
    
    fossil_io_parser_palette_t *palette = fossil_io_parser_create_palette("test_palette", "Test Description");
    FOSSIL_TEST_ASSUME(palette != NULL, "Palette should be created");
    fossil_io_parser_command_t *command = fossil_io_parser_add_command(palette, "test_command", "tc", "Test Command Description");
    FOSSIL_TEST_ASSUME(command != NULL, "Command should be added");
    
    char *argv[] = {"program", "--dry-run", "--verbose", "test_command"};
    fossil_io_parser_parse(palette, 4, argv);
    
    FOSSIL_TEST_ASSUME(FOSSIL_CLI_TOGGLE_DRY_RUN == 1, "Dry-run flag should be set");
    FOSSIL_TEST_ASSUME(FOSSIL_CLI_TOGGLE_VERBOSE == 1, "Verbose flag should be set");
    
    fossil_io_parser_free(palette);
} // end case

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *
FOSSIL_TEST_GROUP(c_parser_test_cases) {
    FOSSIL_TEST_ADD(c_parser_suite, c_create_palette);
    FOSSIL_TEST_ADD(c_parser_suite, c_add_command);
    FOSSIL_TEST_ADD(c_parser_suite, c_add_argument);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_command);
    FOSSIL_TEST_ADD(c_parser_suite, c_free_palette);
    FOSSIL_TEST_ADD(c_parser_suite, c_argument_types);
    FOSSIL_TEST_ADD(c_parser_suite, c_null_palette);
    FOSSIL_TEST_ADD(c_parser_suite, c_empty_command_name);
    FOSSIL_TEST_ADD(c_parser_suite, c_duplicate_command_name);
    FOSSIL_TEST_ADD(c_parser_suite, c_null_argument_name);
    FOSSIL_TEST_ADD(c_parser_suite, c_invalid_argument_type);
    FOSSIL_TEST_ADD(c_parser_suite, c_create_palette_null_inputs);
    FOSSIL_TEST_ADD(c_parser_suite, c_add_command_null_inputs);
    FOSSIL_TEST_ADD(c_parser_suite, c_add_command_with_short_name);
    FOSSIL_TEST_ADD(c_parser_suite, c_duplicate_command_short_name);
    FOSSIL_TEST_ADD(c_parser_suite, c_add_argument_null_command);
    FOSSIL_TEST_ADD(c_parser_suite, c_add_argument_empty_name);
    FOSSIL_TEST_ADD(c_parser_suite, c_duplicate_argument_name);
    FOSSIL_TEST_ADD(c_parser_suite, c_duplicate_argument_short_name);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_null_palette);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_null_argv);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_zero_argc);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_help_flag);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_version_flag);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_dry_run_flag);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_verbose_flag);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_unknown_command);
    FOSSIL_TEST_ADD(c_parser_suite, c_argument_with_combo_options);
    FOSSIL_TEST_ADD(c_parser_suite, c_free_null_palette);
    FOSSIL_TEST_ADD(c_parser_suite, c_multiple_commands_in_palette);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_bool_argument_true);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_bool_argument_false);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_int_argument);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_uint_argument);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_float_argument);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_hex_argument);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_oct_argument);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_help_with_specific_command);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_color_enable);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_color_disable);
    FOSSIL_TEST_ADD(c_parser_suite, c_command_with_null_short_name);
    FOSSIL_TEST_ADD(c_parser_suite, c_argument_with_null_short_name);
    FOSSIL_TEST_ADD(c_parser_suite, c_multiple_arguments_per_command);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_missing_bool_value);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_missing_string_value);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_missing_int_value);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_bool_yes_no);
    FOSSIL_TEST_ADD(c_parser_suite, c_parse_combined_flags);

    FOSSIL_TEST_REGISTER(c_parser_suite);
} // end of group
