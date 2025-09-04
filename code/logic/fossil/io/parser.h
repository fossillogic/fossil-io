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
#ifndef FOSSIL_IO_PARSER_H
#define FOSSIL_IO_PARSER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Types of command argument
typedef enum {
    FOSSIL_IO_PARSER_BOOL,     // Boolean (enable/disable)
    FOSSIL_IO_PARSER_STRING,   // String argument
    FOSSIL_IO_PARSER_INT,      // Integer argument
    FOSSIL_IO_PARSER_FLOAT,    // Floating-point argument
    FOSSIL_IO_PARSER_DATE,     // Date argument
    FOSSIL_IO_PARSER_ARRAY,    // Array of values
    FOSSIL_IO_PARSER_FEATURE,  // Feature flag
    FOSSIL_IO_PARSER_COMBO,    // Combo (select from predefined options)
    FOSSIL_IO_PARSER_INVALID   // Invalid argument type
} fossil_io_parser_arg_type_t;

// Structure to represent each argument in the command
typedef struct fossil_io_parser_argument_s {
    char *name;                                   // Long argument name (e.g., --help)
    char short_name;                              // Short argument name (e.g., -h, '\0' if unused)
    fossil_io_parser_arg_type_t type;             // Argument type
    char *value;                                  // Parsed value
    char **combo_options;                         // Valid options for COMBO type
    int combo_count;                              // Number of valid options
    struct fossil_io_parser_argument_s *next;     // Next argument in the list
} fossil_io_parser_argument_t;

// Structure for a command
typedef struct fossil_io_parser_command_s {
    char *name;                                   // Command name
    char *description;                            // Command description
    fossil_io_parser_argument_t *arguments;       // List of arguments
    struct fossil_io_parser_command_s *prev;      // Previous command in the list
    struct fossil_io_parser_command_s *next;      // Next command in the list
} fossil_io_parser_command_t;

// Structure for the command palette
typedef struct fossil_io_parser_palette_s {
    char *name;                                   // Palette name
    char *description;                            // Palette description
    fossil_io_parser_command_t *commands;         // List of commands
} fossil_io_parser_palette_t;

// ==================================================================
// Global Variables
// ==================================================================

extern int FOSSIL_CLI_TOGGLE_DRY_RUN;
extern int FOSSIL_CLI_TOGGLE_VERBOSE;

extern char *FOSSIL_CLI_TOOL_NAME;
extern char *FOSSIL_CLI_TOOL_VERSION;

// ==================================================================
// Functions
// ==================================================================

/**
 * @brief Creates a new parser palette.
 *
 * @param name The name of the palette.
 * @param description A description of the palette.
 * @return A pointer to the newly created parser palette.
 */
fossil_io_parser_palette_t *fossil_io_parser_create_palette(const char *name, const char *description);

/**
 * @brief Adds a command to the parser palette.
 *
 * @param palette The parser palette to which the command will be added.
 * @param command_name The name of the command.
 * @param description A description of the command.
 * @return A pointer to the newly added command.
 */
fossil_io_parser_command_t *fossil_io_parser_add_command(fossil_io_parser_palette_t *palette, const char *command_name, const char *description);

/**
 * @brief Adds an argument to a command.
 *
 * @param command The command to which the argument will be added.
 * @param arg_name The name of the argument.
 * @param short_name The short name of the argument.
 * @param arg_type The type of the argument.
 * @param combo_options (Optional) Array of valid options for COMBO type.
 * @param combo_count (Optional) Number of options for COMBO type.
 * @return A pointer to the newly added argument.
 */
fossil_io_parser_argument_t *fossil_io_parser_add_argument(
    fossil_io_parser_command_t *command,
    const char *arg_name,
    char short_name,   // NEW
    fossil_io_parser_arg_type_t arg_type,
    char **combo_options,
    int combo_count
)

/**
 * @brief Parses the command-line arguments using the parser palette.
 *
 * @param palette The parser palette to use for parsing.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 */
void fossil_io_parser_parse(fossil_io_parser_palette_t *palette, int argc, char **argv);

/**
 * @brief Frees the memory allocated for the parser palette.
 *
 * @param palette The parser palette to be freed.
 */
void fossil_io_parser_free(fossil_io_parser_palette_t *palette);

#ifdef __cplusplus
}

/**
 * C++ wrapper for the Parser API.
 */
namespace fossil {

    /**
     * Namespace for I/O operations.
     */
    namespace io {
        /**
         * Parser API for sanitizing strings.
         */
        class Parser {
        public:
            /**
             * Creates a new parser palette.
             *
             * @param name The name of the palette.
             * @param description A description of the palette.
             * @return A pointer to the newly created parser palette.
             */
            static fossil_io_parser_palette_t *create_palette(const char *name, const char *description) {
                return fossil_io_parser_create_palette(name, description);
            }

            /**
             * Adds a command to the parser palette.
             *
             * @param palette The parser palette to which the command will be added.
             * @param command_name The name of the command.
             * @param description A description of the command.
             * @return A pointer to the newly added command.
             */
            static fossil_io_parser_command_t *add_command(fossil_io_parser_palette_t *palette, const char *command_name, const char *description) {
                return fossil_io_parser_add_command(palette, command_name, description);
            }

            /**
             * Adds an argument to a command.
             *
             * @param command The command to which the argument will be added.
             * @param arg_name The name of the argument.
             * @param arg_type The type of the argument.
             * @param combo_options (Optional) Array of valid options for COMBO type.
             * @param combo_count (Optional) Number of options for COMBO type.
             * @return A pointer to the newly added argument.
             */
            static fossil_io_parser_argument_t *add_argument(fossil_io_parser_command_t *command, const char *arg_name, fossil_io_parser_arg_type_t arg_type, char **combo_options, int combo_count) {
                return fossil_io_parser_add_argument(command, arg_name, arg_type, combo_options, combo_count);
            }

            /**
             * Parses the command-line arguments using the parser palette.
             *
             * @param palette The parser palette to use for parsing.
             * @param argc The number of command-line arguments.
             * @param argv The command-line arguments.
             */
            static void parse(fossil_io_parser_palette_t *palette, int argc, char **argv) {
                fossil_io_parser_parse(palette, argc, argv);
            }

            /**
             * Frees the memory allocated for the parser palette.
             *
             * @param palette The parser palette to be freed.
             */
            static void free(fossil_io_parser_palette_t *palette) {
                fossil_io_parser_free(palette);
            }

        };
    }
}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
