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
#include "fossil/io/input.h"
#include "fossil/io/soap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to get a sanitized line of input
char *fossil_io_gets(char *buf, size_t size) {
    if (buf == NULL || size == 0) {
        return NULL;
    }

    if (fgets(buf, size, stdin) == NULL) {
        return NULL;
    }

    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    }

    // Sanitize the input buffer
    fossil_soap_sanitize(buf);

    return buf;
}

// Function to get a sanitized line of input with a dialog prompt
char *fossil_io_gets_with_dialog(char *buf, size_t size, const char *dialog)
{
    if (dialog != NULL) {
        fossil_io_puts(dialog); // Print the dialog before input
    }

    return fossil_io_gets(buf, size);
}
