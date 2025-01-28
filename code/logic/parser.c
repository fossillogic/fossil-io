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
        printf("Available commands:\n");
        while (command) {
            printf("  %s: %s\n", command->name, command->description);
            command = command->next;
        }
        printf("\nUse '--help <command>' for details on a specific command.\n");
        return;
    }

    // Search for the specific command
    while (command) {
        if (strcmp(command->name, command_name) == 0) {
            printf("Command: %s\nDescription: %s\n", command->name, command->description);
            printf("Arguments:\n");
            fossil_io_parser_argument_t *arg = command->arguments;
            while (arg) {
                printf("  --%s (%s): %s\n", 
                       arg->name, 
                       arg->type == FOSSIL_io_PARSER_BOOL ? "bool" :
                       arg->type == FOSSIL_io_PARSER_STRING ? "string" :
                       arg->type == FOSSIL_io_PARSER_INT ? "int" :
                       "combo", 
                       arg->value ? arg->value : "No default value");
                if (arg->type == FOSSIL_io_PARSER_COMBO) {
                    printf("    Options: ");
                    for (int i = 0; i < arg->combo_count; i++) {
                        printf("%s%s", arg->combo_options[i], i == arg->combo_count - 1 ? "" : ", ");
                    }
                    printf("\n");
                }
                arg = arg->next;
            }
            return;
        }
        command = command->next;
    }

    // If the command is not found
    fprintf(stderr, "Unknown command '%s'. Use '--help' to see available commands.\n", command_name);
}


void show_usage(const char *command_name, const fossil_io_parser_palette_t *palette) {
    fossil_io_parser_command_t *command = palette->commands;

    // Search for the specific command
    while (command) {
        if (strcmp(command->name, command_name) == 0) {
            printf("Usage example for '%s':\n", command->name);
            printf("  %s", command->name);

            fossil_io_parser_argument_t *arg = command->arguments;
            while (arg) {
                printf(" --%s ", arg->name);
                if (arg->type == FOSSIL_io_PARSER_STRING) {
                    printf("<string>");
                } else if (arg->type == FOSSIL_io_PARSER_INT) {
                    printf("<int>");
                } else if (arg->type == FOSSIL_io_PARSER_BOOL) {
                    printf("<true/false>");
                } else if (arg->type == FOSSIL_io_PARSER_COMBO) {
                    printf("<%s>", arg->combo_options[0]); // Show first combo option
                }
                arg = arg->next;
            }
            printf("\n");
            return;
        }
        command = command->next;
    }

    // If the command is not found
    fprintf(stderr, "Unknown command '%s'. Use '--help' to see available commands.\n", command_name);
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
        fprintf(stderr, "No command provided.\n");
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
            fprintf(stderr, "Usage: --usage <command>\n");
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
            fprintf(stderr, "Unknown command: '%s'. Did you mean '%s'?\n", command_name, suggestion);
        } else {
            fprintf(stderr, "Unknown command: '%s'. Type '--help' to see available commands.\n", command_name);
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
                    case FOSSIL_io_PARSER_BOOL:
                        argument->value = malloc(sizeof(int));
                        if (strcmp(arg_value, "enable") == 0) {
                            *(int *)argument->value = 1; // Enable
                        } else if (strcmp(arg_value, "disable") == 0) {
                            *(int *)argument->value = 0; // Disable
                        } else {
                            fprintf(stderr, "Invalid value for boolean argument: %s\n", arg_value);
                            free(argument->value);
                            argument->value = NULL;
                        }
                        break;
                    case FOSSIL_io_PARSER_STRING:
                        argument->value = _custom_strdup(arg_value); // Custom _custom_strdup
                        break;
                    case FOSSIL_io_PARSER_INT:
                        argument->value = malloc(sizeof(int));
                        *(int *)argument->value = atoi(arg_value);
                        break;
                    case FOSSIL_io_PARSER_COMBO:
                        for (int j = 0; j < argument->combo_count; j++) {
                            if (strcmp(arg_value, argument->combo_options[j]) == 0) {
                                argument->value = _custom_strdup(arg_value);
                                break;
                            }
                        }
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
            if (argument->type == FOSSIL_io_PARSER_COMBO) {
                free(argument->combo_options);
            }
            free(argument->name);
            free(argument->value);
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
