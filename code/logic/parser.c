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

#define DP(i, j) dp[(i) * (len2 + 1) + (j)]

int FOSSIL_CLI_TOGGLE_DRY_RUN = 0;
int FOSSIL_CLI_TOGGLE_VERBOSE = 0;

char *FOSSIL_CLI_TOOL_NAME = "fossil-cli";
char *FOSSIL_CLI_TOOL_VERSION = "1.0.0";


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
// TI Reasoning Metadata (lightweight struct for audit/debug)
// ==================================================================
typedef struct {
    const char *input;
    const char *suggested;
    int edit_distance;
    float confidence_score; // 0.0 to 1.0
    const char *reason;
} fossil_ti_reason_t;

// ==================================================================
// Levenshtein Distance (Unchanged Core)
// ==================================================================
int levenshtein_distance(const char *s1, const char *s2) {
    int len1 = strlen(s1), len2 = strlen(s2);
    int i, j;

    int *dp = (int *)calloc((len1 + 1) * (len2 + 1), sizeof(int));
    if (!dp) return INT_MAX;

    // helper macro for 2D indexing
    #define DP(i, j) dp[(i) * (len2 + 1) + (j)]

    for (i = 0; i <= len1; i++) DP(i, 0) = i;
    for (j = 0; j <= len2; j++) DP(0, j) = j;

    for (i = 1; i <= len1; i++) {
        for (j = 1; j <= len2; j++) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            int del = DP(i - 1, j) + 1;
            int ins = DP(i, j - 1) + 1;
            int sub = DP(i - 1, j - 1) + cost;
            int min = del < ins ? del : ins;
            min = min < sub ? min : sub;
            DP(i, j) = min;
        }
    }

    int result = DP(len1, len2);
    free(dp);
    return result;
}

// ==================================================================
// TI-Aware Command Suggestion (with traceable reasoning)
// ==================================================================
const char* suggest_command_ti(const char *input, fossil_io_parser_palette_t *palette, fossil_ti_reason_t *out_reason) {
    fossil_io_parser_command_t *current = palette->commands;
    const char *best_match = NULL;
    int min_distance = INT_MAX;
    int best_length = 1; // Avoid divide-by-zero

    while (current) {
        int distance = levenshtein_distance(input, current->name);
        if (distance < min_distance) {
            min_distance = distance;
            best_match = current->name;
            best_length = strlen(current->name);
        }
        current = current->next;
    }

    // Compute confidence score based on edit distance
    float confidence = 1.0f - ((float)min_distance / (float)best_length);
    confidence = (confidence < 0.0f) ? 0.0f : (confidence > 1.0f) ? 1.0f : confidence;

    // Optional reasoning trace
    if (out_reason) {
        out_reason->input = input;
        out_reason->suggested = best_match;
        out_reason->edit_distance = min_distance;
        out_reason->confidence_score = confidence;
        out_reason->reason = (confidence >= 0.7f) ? "Close semantic match" : "Low confidence match";
    }

    // Only return suggestions above a minimum confidence threshold
    return (confidence >= 0.7f) ? best_match : NULL;
}

// ==================================================================
// Functions
// ==================================================================

void show_version(void) {
    fossil_io_printf("{blue}Version:{reset} %s\n", FOSSIL_CLI_TOOL_VERSION);
}

void show_help(const char *command_name, const fossil_io_parser_palette_t *palette) {
    if (!palette) return;

    fossil_io_parser_command_t *command = palette->commands;

    // Show all commands if no specific command is requested
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
                // Print long and short names
                if (arg->short_name != '\0') {
                    fossil_io_printf("{cyan}  -%c, --%s (%s): ", arg->short_name, arg->name,
                        arg->type == FOSSIL_IO_PARSER_BOOL ? "bool" :
                        arg->type == FOSSIL_IO_PARSER_FLAG ? "flag" :
                        arg->type == FOSSIL_IO_PARSER_TOGGLE ? "toggle" :
                        arg->type == FOSSIL_IO_PARSER_INT ? "int" :
                        arg->type == FOSSIL_IO_PARSER_UINT ? "uint" :
                        arg->type == FOSSIL_IO_PARSER_HEX ? "hex" :
                        arg->type == FOSSIL_IO_PARSER_OCT ? "oct" :
                        arg->type == FOSSIL_IO_PARSER_BIN ? "bin" :
                        arg->type == FOSSIL_IO_PARSER_FLOAT ? "float" :
                        arg->type == FOSSIL_IO_PARSER_DOUBLE ? "double" :
                        arg->type == FOSSIL_IO_PARSER_STRING ? "string" :
                        arg->type == FOSSIL_IO_PARSER_FILE ? "file" :
                        arg->type == FOSSIL_IO_PARSER_DIR ? "dir" :
                        arg->type == FOSSIL_IO_PARSER_DATE ? "date" :
                        arg->type == FOSSIL_IO_PARSER_TIMESTAMP ? "timestamp" :
                        arg->type == FOSSIL_IO_PARSER_DURATION ? "duration" :
                        arg->type == FOSSIL_IO_PARSER_REGEX ? "regex" :
                        arg->type == FOSSIL_IO_PARSER_JSON ? "json" :
                        arg->type == FOSSIL_IO_PARSER_BASE64 ? "base64" :
                        arg->type == FOSSIL_IO_PARSER_URL ? "url" :
                        arg->type == FOSSIL_IO_PARSER_IP ? "ip" :
                        arg->type == FOSSIL_IO_PARSER_SIZE ? "size" :
                        arg->type == FOSSIL_IO_PARSER_PERCENT ? "percent" :
                        arg->type == FOSSIL_IO_PARSER_ARRAY ? "array" :
                        arg->type == FOSSIL_IO_PARSER_LIST ? "list" :
                        arg->type == FOSSIL_IO_PARSER_MAP ? "map" :
                        arg->type == FOSSIL_IO_PARSER_FEATURE ? "feature" :
                        arg->type == FOSSIL_IO_PARSER_COMBO ? "combo" :
                        "unknown");

                } else {
                    fossil_io_printf("{cyan}  --%s (%s): ", arg->name,
                        arg->type == FOSSIL_IO_PARSER_BOOL ? "bool" :
                        arg->type == FOSSIL_IO_PARSER_FLAG ? "flag" :
                        arg->type == FOSSIL_IO_PARSER_TOGGLE ? "toggle" :
                        arg->type == FOSSIL_IO_PARSER_INT ? "int" :
                        arg->type == FOSSIL_IO_PARSER_UINT ? "uint" :
                        arg->type == FOSSIL_IO_PARSER_HEX ? "hex" :
                        arg->type == FOSSIL_IO_PARSER_OCT ? "oct" :
                        arg->type == FOSSIL_IO_PARSER_BIN ? "bin" :
                        arg->type == FOSSIL_IO_PARSER_FLOAT ? "float" :
                        arg->type == FOSSIL_IO_PARSER_DOUBLE ? "double" :
                        arg->type == FOSSIL_IO_PARSER_STRING ? "string" :
                        arg->type == FOSSIL_IO_PARSER_FILE ? "file" :
                        arg->type == FOSSIL_IO_PARSER_DIR ? "dir" :
                        arg->type == FOSSIL_IO_PARSER_DATE ? "date" :
                        arg->type == FOSSIL_IO_PARSER_TIMESTAMP ? "timestamp" :
                        arg->type == FOSSIL_IO_PARSER_DURATION ? "duration" :
                        arg->type == FOSSIL_IO_PARSER_REGEX ? "regex" :
                        arg->type == FOSSIL_IO_PARSER_JSON ? "json" :
                        arg->type == FOSSIL_IO_PARSER_BASE64 ? "base64" :
                        arg->type == FOSSIL_IO_PARSER_URL ? "url" :
                        arg->type == FOSSIL_IO_PARSER_IP ? "ip" :
                        arg->type == FOSSIL_IO_PARSER_SIZE ? "size" :
                        arg->type == FOSSIL_IO_PARSER_PERCENT ? "percent" :
                        arg->type == FOSSIL_IO_PARSER_ARRAY ? "array" :
                        arg->type == FOSSIL_IO_PARSER_LIST ? "list" :
                        arg->type == FOSSIL_IO_PARSER_MAP ? "map" :
                        arg->type == FOSSIL_IO_PARSER_FEATURE ? "feature" :
                        arg->type == FOSSIL_IO_PARSER_COMBO ? "combo" :
                        "unknown");
                }

                // Print argument value if set
                switch(arg->type) {
                    case FOSSIL_IO_PARSER_BOOL:
                    case FOSSIL_IO_PARSER_FLAG:
                    case FOSSIL_IO_PARSER_TOGGLE:
                    case FOSSIL_IO_PARSER_FEATURE:
                        fossil_io_printf("%s", arg->value.i ? "true" : "false");
                        break;
                    case FOSSIL_IO_PARSER_INT: fossil_io_printf("%d", arg->value.i); break;
                    case FOSSIL_IO_PARSER_UINT: fossil_io_printf("%u", arg->value.u); break;
                    case FOSSIL_IO_PARSER_HEX: fossil_io_printf("0x%lX", arg->value.l); break;
                    case FOSSIL_IO_PARSER_OCT: fossil_io_printf("0%lo", arg->value.l); break;
                    case FOSSIL_IO_PARSER_BIN: {
                        long val = arg->value.l;
                        char buf[65] = {0};
                        for(int i=63;i>=0;i--) buf[63-i] = (val & (1L<<i)) ? '1' : '0';
                        fossil_io_printf("0b%s", buf);
                        break;
                    }
                    case FOSSIL_IO_PARSER_FLOAT: fossil_io_printf("%f", arg->value.f); break;
                    case FOSSIL_IO_PARSER_DOUBLE: fossil_io_printf("%lf", arg->value.d); break;
                    case FOSSIL_IO_PARSER_SIZE:
                    case FOSSIL_IO_PARSER_PERCENT:
                    case FOSSIL_IO_PARSER_STRING:
                    case FOSSIL_IO_PARSER_FILE:
                    case FOSSIL_IO_PARSER_DIR:
                    case FOSSIL_IO_PARSER_DATE:
                    case FOSSIL_IO_PARSER_TIMESTAMP:
                    case FOSSIL_IO_PARSER_DURATION:
                    case FOSSIL_IO_PARSER_REGEX:
                    case FOSSIL_IO_PARSER_JSON:
                    case FOSSIL_IO_PARSER_BASE64:
                    case FOSSIL_IO_PARSER_URL:
                    case FOSSIL_IO_PARSER_IP:
                    case FOSSIL_IO_PARSER_COMBO:
                        fossil_io_printf("%s", arg->value.s ? arg->value.s : "No default");
                        break;
                    case FOSSIL_IO_PARSER_ARRAY:
                    case FOSSIL_IO_PARSER_LIST:
                        if (arg->value.arr) {
                            fossil_io_printf("[");
                            for (int j=0; arg->value.arr[j]; j++) {
                                if (j>0) fossil_io_printf(", ");
                                fossil_io_printf("%s", arg->value.arr[j]);
                            }
                            fossil_io_printf("]");
                        } else fossil_io_printf("No default");
                        break;
                    case FOSSIL_IO_PARSER_MAP:
                        fossil_io_printf("{...}"); // placeholder
                        break;
                    default:
                        fossil_io_printf("Unknown");
                        break;
                }

                fossil_io_printf("{reset}\n");
                arg = arg->next;
            }

            // Built-in flags
            fossil_io_printf("{blue}Built-in:{reset}\n");
            fossil_io_printf("{cyan}  --help: Show help information{reset}\n");
            fossil_io_printf("{cyan}  --version: Show version{reset}\n");
            fossil_io_printf("{cyan}  --dry-run: Simulate operation{reset}\n");
            fossil_io_printf("{cyan}  --verbose: Verbose output{reset}\n");
            return;
        }
        command = command->next;
    }

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
                // Print both long and short names if available
                if (arg->short_name) {
                    fossil_io_printf("{cyan} [--%s | -%c]{reset}", arg->name, arg->short_name);
                } else {
                    fossil_io_printf("{cyan} --%s{reset}", arg->name);
                }

                // Show expected type/value
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
                    case FOSSIL_IO_PARSER_COMBO:
                        fossil_io_printf("{cyan}<");
                        for (int i = 0; i < arg->combo_count; i++) {
                            fossil_io_printf("%s%s", i > 0 ? "|" : "", arg->combo_options[i]);
                        }
                        fossil_io_printf(">{reset}");
                        break;
                    default:
                        fossil_io_printf("{cyan}<unknown>{reset}");
                        break;
                }

                arg = arg->next;
            }

            // Add built-in flags to the usage
            fossil_io_printf("{cyan} [--dry-run] [--verbose] [--version] [--help]{reset}");
            fossil_io_printf("\n");
            return;
        }
        command = command->next;
    }

    // If the command is not found
    fossil_io_fprintf(FOSSIL_STDERR,
        "{red}Unknown command '%s'. Use '--help' to see available commands.{reset}\n",
        command_name);
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
        if (palette->name) free(palette->name);
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
    // Initialize all fields to zero/NULL
    command->name = NULL;
    command->description = NULL;
    command->arguments = NULL;
    command->prev = NULL;
    command->next = NULL;

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

fossil_io_parser_argument_t *fossil_io_parser_add_argument(
    fossil_io_parser_command_t *command,
    const char *arg_name,
    char short_name,   // NEW
    fossil_io_parser_arg_type_t arg_type,
    char **combo_options,
    int combo_count
) {
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
    argument->short_name = short_name ? short_name : '\0';
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
        // Use TI-aware suggestion system
        fossil_ti_reason_t ti_reason = {0};
        const char *suggestion = suggest_command_ti(command_name, palette, &ti_reason);
    
        if (suggestion) {
            fossil_io_fprintf(FOSSIL_STDERR,
                "{red}Unknown command: '%s'. Did you mean '%s'?{reset}\n"
                "{yellow}[TI] Suggestion confidence: %.2f | Distance: %d | Reason: %s{reset}\n",
                command_name,
                suggestion,
                ti_reason.confidence_score,
                ti_reason.edit_distance,
                ti_reason.reason
            );
        } else {
            fossil_io_fprintf(FOSSIL_STDERR,
                "{red}Unknown command: '%s'. Type '--help' to see available commands.{reset}\n",
                command_name
            );
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
                    case FOSSIL_IO_PARSER_COMBO:
                        if (i + 1 < argc) {
                            char *candidate = argv[++i];
                            int valid = 0;
                            for (int j = 0; j < argument->combo_count; j++) {
                                if (strcmp(candidate, argument->combo_options[j]) == 0) {
                                    valid = 1;
                                    break;
                                }
                            }
                            if (valid) {
                                argument->value = _custom_strdup(candidate);
                            } else {
                                fossil_io_fprintf(FOSSIL_STDERR,
                                    "{red}Invalid value '%s' for combo argument '%s'. Valid options: ",
                                    candidate, argument->name);
                                for (int j = 0; j < argument->combo_count; j++) {
                                    fossil_io_printf("%s%s", j > 0 ? ", " : "", argument->combo_options[j]);
                                }
                                fossil_io_printf("{reset}\n");
                            }
                        } else {
                            fossil_io_fprintf(FOSSIL_STDERR,
                                "{red}Missing value for combo argument: %s{reset}\n", arg_value);
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
