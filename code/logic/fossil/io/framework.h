/**
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * This file is part of the Fossil Logic project, which aims to develop
 * high-performance, cross-platform applications and libraries. The code
 * contained herein is licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * Author: Michael Gene Brockus (Dreamer)
 * Date: 04/05/2014
 *
 * Copyright (C) 2014-2025 Fossil Logic. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#ifndef FOSSIL_IO_FRAMEWORK_H
#define FOSSIL_IO_FRAMEWORK_H

// Include the necessary headers
#include "regex.h"
#include "output.h"
#include "input.h"
#include "error.h"
#include "archive.h"
#include "cstring.h"
#include "cipher.h"
#include "file.h"
#include "soap.h"
#include "dir.h"

enum {
    FOSSIL_IO_SUCCESS = 0,
    FOSSIL_IO_FAILURE = 1,
    FOSSIL_IO_ERROR   = -1,
};

#endif /* FOSSIL_IO_FRAMEWORK_H */
