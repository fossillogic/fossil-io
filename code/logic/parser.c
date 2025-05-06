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
                       "unknown", 
                       arg->value ? arg->value : "No default value");
                arg = arg->next;
            }
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
            fossil_io_printf("\n");
            return;
        }
        command = command->next;
    }

    // If the command is not found
    fossil_io_fprintf(FOSSIL_STDERR, "{red}Unknown command '%s'. Use '--help' to see available commands.{reset}\n", command_name);
}

fossil_io_parser_palette_t *fossil_io_parser_create_palette(const char *name, const char *description) {
    fossil_io_parser_palette_t *palette = malloc(sizeof(fossil_io_parser_palette_t));
    palette->name = _custom_strdup(name);
    palette->description = _custom_strdup(description);
    palette->commands = NULL;
    return palette;
}

fossil_io_parser_command_t *fossil_io_parser_add_command(fossil_io_parser_palette_t *palette, const char *command_name, const char *description) {
    fossil_io_parser_command_t *command = malloc(sizeof(fossil_io_parser_command_t));
    command->name = _custom_strdup(command_name);
    command->description = _custom_strdup(description);
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
    fossil_io_parser_argument_t *argument = malloc(sizeof(fossil_io_parser_argument_t));
    argument->name = _custom_strdup(arg_name);
    argument->type = arg_type;
    argument->value = NULL;
    argument->combo_options = combo_options;
    argument->combo_count = combo_count;
    argument->next = command->arguments;
    command->arguments = argument;
    return argument;
}

// Updated parse function
void fossil_io_parser_parse(fossil_io_parser_palette_t *palette, int argc, char **argv) {
    if (argc < 2) {
        fossil_io_fprintf(FOSSIL_STDERR, "{red}No command provided.{reset}\n");
        return;
    }

    const char *command_name = argv[1];

    // Check for --help and --usage flags
    if (strcmp(argv[1], "--help") == 0) {
        if (argc == 3) {
            show_help(argv[2], palette); // Show help for a specific command
        } else {
            show_help(NULL, palette);   // Show general help
        }
        return;
    }

    if (strcmp(argv[1], "--usage") == 0) {
        if (argc == 3) {
            show_usage(argv[2], palette); // Show usage for a specific command
        } else {
            fossil_io_fprintf(FOSSIL_STDERR, "{blue}Usage:{cuan} --usage <command>{reset}\n");
        }
        return;
    }

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
    for (int i = 2; i < argc; i++) {
        const char *arg_value = argv[i];
        fossil_io_parser_argument_t *argument = command->arguments;
        while (argument) {
            if (strcmp(argument->name, arg_value) == 0) {
                switch (argument->type) {
                    case FOSSIL_IO_PARSER_BOOL:
                        argument->value = malloc(sizeof(int));
                        if (strcmp(arg_value, "true") == 0 || strcmp(arg_value, "yes") == 0) {
                            *(int *)argument->value = 1; // Enable
                        } else if (strcmp(arg_value, "false") == 0|| strcmp(arg_value, "no") == 0) {
                            *(int *)argument->value = 0; // Disable
                        } else {
                            fossil_io_fprintf(FOSSIL_STDERR, "{red}Invalid value for boolean argument: %s{reset}\n", arg_value);
                            argument->value = NULL;
                        }
                        break;
                    case FOSSIL_IO_PARSER_STRING:
                        argument->value = _custom_strdup(arg_value);
                        break;
                    case FOSSIL_IO_PARSER_INT:
                        argument->value = malloc(sizeof(int));
                        *(int *)argument->value = atoi(arg_value);
                        break;
                    case FOSSIL_IO_PARSER_FLOAT:
                        argument->value = malloc(sizeof(float));
                        *(float *)argument->value = atof(arg_value);
                        break;
                    case FOSSIL_IO_PARSER_DATE:
                        argument->value = _custom_strdup(arg_value); // Assume valid date format
                        break;
                    case FOSSIL_IO_PARSER_ARRAY: {
                        char *array_copy = _custom_strdup(arg_value);
                        char *token = strtok(array_copy, ",");
                        char **array_values = NULL;
                        int count = 0;

                        while (token) {
                            array_values = realloc(array_values, sizeof(char *) * (count + 1));
                            array_values[count++] = _custom_strdup(token);
                            token = strtok(NULL, ",");
                        }

                        argument->value = (char *)array_values;
                        break;
                    }
                    case FOSSIL_IO_PARSER_FEATURE:
                        argument->value = malloc(sizeof(int));
                        if (strcmp(arg_value, "enable") == 0) {
                            *(int *)argument->value = 1; // Enable
                        } else if (strcmp(arg_value, "disable") == 0) {
                            *(int *)argument->value = 0; // Disable
                        } else if (strcmp(arg_value, "auto") == 0) {
                            *(int *)argument->value = 2; // Auto
                        } else {
                            fossil_io_fprintf(FOSSIL_STDERR, "{red}Invalid value for feature argument: %s{reset}\n", arg_value);
                            argument->value = NULL;
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
