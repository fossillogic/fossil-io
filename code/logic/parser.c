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
#include "fossil/io/parser.h"
#include "fossil/io/output.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int FOSSIL_CLI_TOGGLE_DRY_RUN = 0;
int FOSSIL_CLI_TOGGLE_VERBOSE = 0;
int FOSSIL_CLI_TOGGLE_SANITY = 0;

char *FOSSIL_CLI_TOOL_NAME = "fossil-cli";
char *FOSSIL_CLI_TOOL_VERSION = "1.0.0";
char *FOSSIL_CLI_TOOL_AUTHOR = "Fossil Logic";
char *FOSSIL_CLI_TOOL_LICENSE = "Mozilla Public License 2.0";
char *FOSSIL_CLI_TOOL_DESCRIPTION = "A command-line interface for Fossil Logic.";
char *FOSSIL_CLI_TOOL_WEBSITE = "https://fossil-logic.com";


extern char *_custom_strdup(const char *str) {
    if (!str) return NULL;
    size_t len = strlen(str) + 1;
    char *dup = malloc(len);
    if (dup) {
        memcpy(dup, str, len);
    }
    return dup;
}

// ==================================================================
// AI magic tricks
// ==================================================================

// Function to calculate Levenshtein Distance
int levenshtein_distance(const char *s1, const char *s2) {
    int len1 = strlen(s1), len2 = strlen(s2);
    int dp[len1 + 1][len2 + 1];

    for (int i = 0; i <= len1; i++) dp[i][0] = i;
    for (int j = 0; j <= len2; j++) dp[0][j] = j;

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            dp[i][j] = fmin(fmin(dp[i - 1][j] + 1, dp[i][j - 1] + 1), dp[i - 1][j - 1] + cost);
        }
    }
    return dp[len1][len2];
}

// Function to find the closest command match
const char* suggest_command(const char *input, fossil_io_parser_palette_t *palette) {
    fossil_io_parser_command_t *current = palette->commands;
    const char *best_match = NULL;
    int min_distance = INT_MAX;

    while (current) {
        int distance = levenshtein_distance(input, current->name);
        if (distance < min_distance) {
            min_distance = distance;
            best_match = current->name;
        }
        current = current->next;
    }
    return (min_distance <= 3) ? best_match : NULL; // Suggest only if close enough
}

// ==================================================================
// Functions
// ==================================================================

void show_this(void) {
    fossil_io_printf("{blue}About this tool:{reset}\n");
    fossil_io_printf("{cyan}  Name:{reset} %s\n", FOSSIL_CLI_TOOL_NAME);
    fossil_io_printf("{cyan}  Version:{reset} %s\n", FOSSIL_CLI_TOOL_VERSION);
    fossil_io_printf("{cyan}  Author:{reset} %s\n", FOSSIL_CLI_TOOL_AUTHOR);
    fossil_io_printf("{cyan}  License:{reset} %s\n", FOSSIL_CLI_TOOL_LICENSE);
    fossil_io_printf("{cyan}  Description:{reset} %s\n", FOSSIL_CLI_TOOL_DESCRIPTION);
    fossil_io_printf("{cyan}  Website:{reset} %s\n", FOSSIL_CLI_TOOL_WEBSITE);

    fossil_io_printf("{blue}Operating System:{reset} %s\n", 
#if defined(_WIN32) || defined(_WIN64)
        "Windows"
#elif defined(__linux__)
        "Linux"
#elif defined(__APPLE__) && defined(__MACH__)
        "macOS"
#elif defined(__unix__)
        "Unix"
#else
        "Unknown"
#endif
    );

    fossil_io_printf("{blue}Endianness:{reset} %s\n", 
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        "Little Endian"
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        "Big Endian"
#else
        "Unknown"
#endif
    );
}

void show_version(void) {
    fossil_io_printf("{blue}Version:{reset} %s\n", FOSSIL_CLI_TOOL_VERSION);
}

void show_help(const char *command_name, const fossil_io_parser_palette_t *palette) {
    fossil_io_parser_command_t *command = palette->commands;

    // If no specific command is provided, show all commands
    if (!command_name) {
        fossil_io_printf("{blue}Available commands:{reset}\n");
        while (command) {
            fossil_io_printf("{cyan}  %s: %s{reset}\n", command->name, command->description);
            command = command->next;
        }
        fossil_io_printf("\n{blue}Use '--help <command>' for details on a specific command.{reset}\n");
        return;
    }

    // Search for the specific command
    while (command) {
        if (strcmp(command->name, command_name) == 0) {
            fossil_io_printf("{blue}Command: %s\nDescription: %s{reset}\n", command->name, command->description);
            fossil_io_printf("{blue}Arguments:{reset}\n");
            fossil_io_parser_argument_t *arg = command->arguments;
            while (arg) {
                fossil_io_printf("{cyan}  --%s (%s): %s{reset}\n", 
                       arg->name, 
                       arg->type == FOSSIL_IO_PARSER_BOOL ? "bool" :
                       arg->type == FOSSIL_IO_PARSER_STRING ? "string" :
                       arg->type == FOSSIL_IO_PARSER_INT ? "int" :
                       arg->type == FOSSIL_IO_PARSER_FLOAT ? "float" :
                       arg->type == FOSSIL_IO_PARSER_DATE ? "date" :
                       arg->type == FOSSIL_IO_PARSER_ARRAY ? "array" :
                       arg->type == FOSSIL_IO_PARSER_FEATURE ? "feature" :
                       arg->type == FOSSIL_IO_PARSER_INVALID ? "invalid" :
                       "unknown", 
                       arg->value ? arg->value : "No default value");
                arg = arg->next;
            }

            // Handle specific flags
            fossil_io_printf("{blue}Built-in:{reset}\n");
            fossil_io_printf("{cyan}  --help: Show help information for this command.{reset}\n");
            fossil_io_printf("{cyan}  --version: Display the version of the application.{reset}\n");
            fossil_io_printf("{cyan}  --this: Perform the operation on the current context.{reset}\n");
            fossil_io_printf("{cyan}  --dry-run: Simulate the operation without making changes.{reset}\n");
            fossil_io_printf("{cyan}  --verbose: Provide detailed output during execution.{reset}\n");
            fossil_io_printf("{cyan}  --sanity: Perform sanity checks before execution.{reset}\n");
            return;
        }
        command = command->next;
    }

    // If the command is not found
    fossil_io_fprintf(FOSSIL_STDERR, "{red}Unknown command '%s'. Use '--help' to see available commands.{reset}\n", command_name);
}

void show_usage(const char *command_name, const fossil_io_parser_palette_t *palette) {
    fossil_io_parser_command_t *command = palette->commands;

    // Search for the specific command
    while (command) {
        if (strcmp(command->name, command_name) == 0) {
            fossil_io_printf("{blue}Usage example for '%s':{reset}\n", command->name);
            fossil_io_printf("{cyan}  %s{reset}", command->name);

            fossil_io_parser_argument_t *arg = command->arguments;
            while (arg) {
                fossil_io_printf("{cyan} --%s {reset}", arg->name);
                switch (arg->type) {
                    case FOSSIL_IO_PARSER_STRING:
                        fossil_io_printf("{cyan}<string>{reset}");
                        break;
                    case FOSSIL_IO_PARSER_INT:
                        fossil_io_printf("{cyan}<int>{reset}");
                        break;
                    case FOSSIL_IO_PARSER_BOOL:
                        fossil_io_printf("{cyan}<true/false>{reset}");
                        break;
                    case FOSSIL_IO_PARSER_FLOAT:
                        fossil_io_printf("{cyan}<float>{reset}");
                        break;
                    case FOSSIL_IO_PARSER_DATE:
                        fossil_io_printf("{cyan}<YYYY-MM-DD>{reset}");
                        break;
                    case FOSSIL_IO_PARSER_ARRAY:
                        fossil_io_printf("{cyan}<value1,value2,...>{reset}");
                        break;
                    case FOSSIL_IO_PARSER_FEATURE:
                        fossil_io_printf("{cyan}<enable/disable>{reset}");
                        break;
                    default:
                        fossil_io_printf("{cyan}<unknown>{reset}");
                        break;
                }
                arg = arg->next;
            }

            // Add built-in flags to the usage
            fossil_io_printf("{cyan} [--this] [--dry-run] [--verbose] [--sanity] [--version] [--help]{reset}");
            fossil_io_printf("\n");
            return;
        }
        command = command->next;
    }

    // If the command is not found
    fossil_io_fprintf(FOSSIL_STDERR, "{red}Unknown command '%s'. Use '--help' to see available commands.{reset}\n", command_name);
}

fossil_io_parser_palette_t *fossil_io_parser_create_palette(const char *name, const char *description) {
    if (!name || !description) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Palette name and description cannot be NULL.{reset}\n");
        return NULL;
    }

    fossil_io_parser_palette_t *palette = malloc(sizeof(fossil_io_parser_palette_t));
    if (!palette) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Memory allocation failed for palette.{reset}\n");
        return NULL;
    }

    palette->name = _custom_strdup(name);
    if (!palette->name) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Memory allocation failed for palette name.{reset}\n");
        free(palette);
        return NULL;
    }

    palette->description = _custom_strdup(description);
    if (!palette->description) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Memory allocation failed for palette description.{reset}\n");
        free(palette->name);
        free(palette);
        return NULL;
    }

    palette->commands = NULL;
    return palette;
}

fossil_io_parser_command_t *fossil_io_parser_add_command(fossil_io_parser_palette_t *palette, const char *command_name, const char *description) {
    if (!palette || !command_name || !description) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Palette, command name, and description cannot be NULL.{reset}\n");
        return NULL;
    }

    if (strlen(command_name) == 0) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Command name cannot be empty.{reset}\n");
        return NULL;
    }

    // Check for duplicate command name
    fossil_io_parser_command_t *current = palette->commands;
    while (current) {
        if (strcmp(current->name, command_name) == 0) {
            fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Command with name '%s' already exists.{reset}\n", command_name);
            return NULL;
        }
        current = current->next;
    }

    fossil_io_parser_command_t *command = malloc(sizeof(fossil_io_parser_command_t));
    if (!command) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Memory allocation failed for command.{reset}\n");
        return NULL;
    }

    command->name = _custom_strdup(command_name);
    if (!command->name) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Memory allocation failed for command name.{reset}\n");
        free(command);
        return NULL;
    }

    command->description = _custom_strdup(description);
    if (!command->description) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Memory allocation failed for command description.{reset}\n");
        free(command->name);
        free(command);
        return NULL;
    }

    command->arguments = NULL;
    command->prev = NULL;
    command->next = palette->commands;
    if (palette->commands) {
        palette->commands->prev = command;
    }
    palette->commands = command;
    return command;
}

fossil_io_parser_argument_t *fossil_io_parser_add_argument(fossil_io_parser_command_t *command, const char *arg_name, fossil_io_parser_arg_type_t arg_type, char **combo_options, int combo_count) {
    if (!command || !arg_name) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Command and argument name cannot be NULL.{reset}\n");
        return NULL;
    }

    fossil_io_parser_argument_t *argument = malloc(sizeof(fossil_io_parser_argument_t));
    if (!argument) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Memory allocation failed for argument.{reset}\n");
        return NULL;
    }

    argument->name = _custom_strdup(arg_name);
    if (!argument->name) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Memory allocation failed for argument name.{reset}\n");
        free(argument);
        return NULL;
    }

    if (arg_type < FOSSIL_IO_PARSER_BOOL || arg_type > FOSSIL_IO_PARSER_FEATURE) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Invalid argument type for '%s'.{reset}\n", arg_name);
        free(argument->name);
        free(argument);
        return NULL;
    }
    argument->type = arg_type;
    argument->value = NULL;
    argument->combo_options = combo_options;
    argument->combo_count = combo_count;
    argument->next = command->arguments;
    command->arguments = argument;
    return argument;
}

// Updated parse function with error checking
void fossil_io_parser_parse(fossil_io_parser_palette_t *palette, int argc, char **argv) {
    if (!palette || !argv) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Invalid arguments passed to parser.{reset}\n");
        return;
    }

    if (argc < 2) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}No command provided.{reset}\n");
        return;
    }

    const char *command_name = NULL;
    int global_flags_processed = 0;

    // Scan for global flags and commands
    for (int i = 1; i < argc; i++) {
        const char *arg = argv[i];

        if (!arg) {
            fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: NULL argument encountered.{reset}\n");
            return;
        }

        if (strcmp(arg, "--help") == 0) {
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                show_help(argv[++i], palette); // Show help for a specific command
            } else {
                show_help(NULL, palette);     // Show general help
            }
            global_flags_processed = 1;
            break;
        }

        if (strcmp(arg, "--version") == 0) {
            show_version();
            global_flags_processed = 1;
            break;
        }

        if (strcmp(arg, "--this") == 0) {
            show_this();
            global_flags_processed = 1;
            break;
        }

        if (strcmp(arg, "--dry-run") == 0) {
            FOSSIL_CLI_TOGGLE_DRY_RUN = 1;
            fossil_io_printf("{blue}Dry-run mode enabled.{reset}\n");
            continue;
        }

        if (strcmp(arg, "--verbose") == 0) {
            FOSSIL_CLI_TOGGLE_VERBOSE = 1;
            fossil_io_printf("{blue}Verbose mode enabled.{reset}\n");
            continue;
        }

        if (strcmp(arg, "--sanity") == 0) {
            FOSSIL_CLI_TOGGLE_SANITY = 1;
            fossil_io_printf("{blue}Sanity checks enabled.{reset}\n");
            continue;
        }

        // Assume the first non-flag argument is the command
        if (arg[0] != '-' && !command_name) {
            command_name = arg;
        }
    }

    if (global_flags_processed) {
        return;
    }

    if (!command_name) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}No command provided. Type '--help' to see available commands.{reset}\n");
        return;
    }

    // Process specific commands
    fossil_io_parser_command_t *command = palette->commands;
    while (command) {
        if (strcmp(command->name, command_name) == 0) {
            break;
        }
        command = command->next;
    }

    if (!command) {
        // Suggest a similar command or show an error
        const char *suggestion = suggest_command(command_name, palette);
        if (suggestion) {
            fossil_io_fprintf(FOSSIL_STDERR, "{red}Unknown command: '%s'. Did you mean '%s'?{reset}\n", command_name, suggestion);
        } else {
            fossil_io_fprintf(FOSSIL_STDERR, "{red}Unknown command: '%s'. Type '--help' to see available commands.{reset}\n", command_name);
        }
        return;
    }

    // Process command arguments
    for (int i = 1; i < argc; i++) {
        const char *arg_value = argv[i];
        if (!arg_value) {
            fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: NULL argument encountered.{reset}\n");
            return;
        }

        if (arg_value[0] == '-') {
            continue; // Skip flags already processed
        }

        fossil_io_parser_argument_t *argument = command->arguments;
        while (argument) {
            if (strcmp(argument->name, arg_value) == 0) {
                switch (argument->type) {
                    case FOSSIL_IO_PARSER_BOOL:
                        argument->value = malloc(sizeof(int));
                        if (!argument->value) {
                            fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Memory allocation failed for boolean argument.{reset}\n");
                            return;
                        }
                        if (i + 1 < argc && (strcmp(argv[i + 1], "true") == 0 || strcmp(argv[i + 1], "yes") == 0)) {
                            *(int *)argument->value = 1; // Enable
                            i++;
                        } else if (i + 1 < argc && (strcmp(argv[i + 1], "false") == 0 || strcmp(argv[i + 1], "no") == 0)) {
                            *(int *)argument->value = 0; // Disable
                            i++;
                        } else {
                            fossil_io_fprintf(FOSSIL_STDERR, "{red}Invalid value for boolean argument: %s{reset}\n", argv[i + 1]);
                            free(argument->value);
                            argument->value = NULL;
                        }
                        break;
                    case FOSSIL_IO_PARSER_STRING:
                        if (i + 1 < argc) {
                            argument->value = _custom_strdup(argv[++i]);
                            if (!argument->value) {
                                fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Memory allocation failed for string argument.{reset}\n");
                                return;
                            }
                        } else {
                            fossil_io_fprintf(FOSSIL_STDERR, "{red}Missing value for string argument: %s{reset}\n", arg_value);
                        }
                        break;
                    case FOSSIL_IO_PARSER_INT:
                        if (i + 1 < argc) {
                            argument->value = malloc(sizeof(int));
                            if (!argument->value) {
                                fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Memory allocation failed for integer argument.{reset}\n");
                                return;
                            }
                            *(int *)argument->value = atoi(argv[++i]);
                        } else {
                            fossil_io_fprintf(FOSSIL_STDERR, "{red}Missing value for integer argument: %s{reset}\n", arg_value);
                        }
                        break;
                    case FOSSIL_IO_PARSER_FLOAT:
                        if (i + 1 < argc) {
                            argument->value = malloc(sizeof(float));
                            if (!argument->value) {
                                fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Memory allocation failed for float argument.{reset}\n");
                                return;
                            }
                            *(float *)argument->value = atof(argv[++i]);
                        } else {
                            fossil_io_fprintf(FOSSIL_STDERR, "{red}Missing value for float argument: %s{reset}\n", arg_value);
                        }
                        break;
                    case FOSSIL_IO_PARSER_DATE:
                        if (i + 1 < argc) {
                            argument->value = _custom_strdup(argv[++i]);
                            if (!argument->value) {
                                fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Memory allocation failed for date argument.{reset}\n");
                                return;
                            }
                        } else {
                            fossil_io_fprintf(FOSSIL_STDERR, "{red}Missing value for date argument: %s{reset}\n", arg_value);
                        }
                        break;
                    case FOSSIL_IO_PARSER_ARRAY: {
                        if (i + 1 < argc) {
                            char *array_copy = _custom_strdup(argv[++i]);
                            if (!array_copy) {
                                fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Memory allocation failed for array argument.{reset}\n");
                                return;
                            }
                            char *token = strtok(array_copy, ",");
                            char **array_values = NULL;
                            int count = 0;

                            while (token) {
                                char **temp = realloc(array_values, sizeof(char *) * (count + 1));
                                if (!temp) {
                                    fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Memory allocation failed for array values.{reset}\n");
                                    free(array_copy);
                                    for (int j = 0; j < count; j++) {
                                        free(array_values[j]);
                                    }
                                    free(array_values);
                                    return;
                                }
                                array_values = temp;
                                array_values[count++] = _custom_strdup(token);
                                token = strtok(NULL, ",");
                            }

                            argument->value = (char *)array_values;
                            free(array_copy);
                        } else {
                            fossil_io_fprintf(FOSSIL_STDERR, "{red}Missing value for array argument: %s{reset}\n", arg_value);
                        }
                        break;
                    }
                    case FOSSIL_IO_PARSER_FEATURE:
                        if (i + 1 < argc) {
                            argument->value = malloc(sizeof(int));
                            if (!argument->value) {
                                fossil_io_fprintf(FOSSIL_STDERR, "{red}Error: Memory allocation failed for feature argument.{reset}\n");
                                return;
                            }
                            if (strcmp(argv[i + 1], "enable") == 0) {
                                *(int *)argument->value = 1; // Enable
                                i++;
                            } else if (strcmp(argv[i + 1], "disable") == 0) {
                                *(int *)argument->value = 0; // Disable
                                i++;
                            } else if (strcmp(argv[i + 1], "auto") == 0) {
                                *(int *)argument->value = 2; // Auto
                                i++;
                            } else {
                                fossil_io_fprintf(FOSSIL_STDERR, "{red}Invalid value for feature argument: %s{reset}\n", argv[i + 1]);
                                free(argument->value);
                                argument->value = NULL;
                            }
                        } else {
                            fossil_io_fprintf(FOSSIL_STDERR, "{red}Missing value for feature argument: %s{reset}\n", arg_value);
                        }
                        break;
                    default:
                        fossil_io_fprintf(FOSSIL_STDERR, "{red}Unknown argument type for: %s{reset}\n", arg_value);
                        break;
                }
                break;
            }
            argument = argument->next;
        }
    }
}

void fossil_io_parser_free(fossil_io_parser_palette_t *palette) {
    fossil_io_parser_command_t *command = palette->commands;
    while (command) {
        fossil_io_parser_argument_t *argument = command->arguments;
        while (argument) {
            free(argument->name);
            if (argument->value && argument->value != (char *)argument->combo_options) {
                free(argument->value);
            }
            argument = argument->next;
        }
        free(command->name);
        free(command->description);
        command = command->next;
    }
    free(palette->name);
    free(palette->description);
    free(palette);
}
