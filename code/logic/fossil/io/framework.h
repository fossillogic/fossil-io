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
#ifndef FOSSIL_IO_FRAMEWORK_H
#define FOSSIL_IO_FRAMEWORK_H

// Include the necessary headers
#include "serialize.h"
#include "network.h"
#include "output.h"
#include "input.h"
#include "error.h"
#include "cryptic.h"
#include "cstring.h"
#include "stream.h"
#include "soap.h"
#include "parser.h"

enum {
    FOSSIL_IO_SUCCESS = 0,
    FOSSIL_IO_FAILURE = 1,
    FOSSIL_IO_ERROR   = -1,
};

#endif /* FOSSIL_IO_FRAMEWORK_H */
