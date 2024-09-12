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
#ifndef FOSSIL_IO_INPUT_H
#define FOSSIL_IO_INPUT_H

#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reads a line from the input stream and stores it into the buffer pointed to by 'buf'.
 * 
 * @param buf           Pointer to the buffer where the line will be stored.
 * @param size          Maximum number of characters to be read, including the null terminator.
 * @param input_stream  Pointer to the input stream to read from.
 * @return              On success, the function returns 'buf'. If the end-of-file is reached or an error occurs, it returns NULL.
 */
char *fossil_io_gets_from_stream(char *buf, size_t size, FILE *input_stream);

/**
 * Reads a line from the input stream and stores it into the buffer pointed to by 'buf'.
 * 
 * @param buf   Pointer to the buffer where the line will be stored.
 * @param size  Maximum number of characters to be read, including the null terminator.
 * @return      On success, the function returns 'buf'. If the end-of-file is reached or an error occurs, it returns NULL.
 */
char *fossil_io_gets(char *buf, size_t size);

/**
 * Reads a line from the input stream and stores it into the buffer pointed to by 'buf',
 * while displaying a dialog message to the user.
 * 
 * @param buf       Pointer to the buffer where the line will be stored.
 * @param size      Maximum number of characters to be read, including the null terminator.
 * @param dialog    Dialog message to be displayed to the user.
 * @return          On success, the function returns 'buf'. If the end-of-file is reached or an error occurs, it returns NULL.
 */
char *fossil_io_gets_with_dialog(char *buf, size_t size, const char *dialog);

#ifdef __cplusplus
}
#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
