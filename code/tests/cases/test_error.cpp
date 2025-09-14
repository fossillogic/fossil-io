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
#include <fossil/pizza/framework.h>

#include "fossil/io/framework.h"

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilites
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Define the test suite and add test cases
FOSSIL_SUITE(cpp_error_suite);

// Setup function for the test suite
FOSSIL_SETUP(cpp_error_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_error_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(cpp_test_io_what_no_error) {
    const char *result = fossil_io_what(FOSSIL_ERROR_OK);
    ASSUME_ITS_EQUAL_CSTR("No error, operation successful.", result);
}

FOSSIL_TEST(cpp_test_io_what_CNULL_pointer) {
    const char *result = fossil_io_what(FOSSIL_ERROR_CNULL_POINTER);
    ASSUME_ITS_EQUAL_CSTR("Null pointer encountered.", result);
}

FOSSIL_TEST(cpp_test_io_what_invalid_argument) {
    const char *result = fossil_io_what(FOSSIL_ERROR_INVALID_ARGUMENT);
    ASSUME_ITS_EQUAL_CSTR("Invalid argument provided.", result);
}

FOSSIL_TEST(cpp_test_io_what_type_mismatch) {
    const char *result = fossil_io_what(FOSSIL_ERROR_TYPE_MISMATCH);
    ASSUME_ITS_EQUAL_CSTR("Type mismatch encountered.", result);
}

FOSSIL_TEST(cpp_test_io_what_invalid_operation) {
    const char *result = fossil_io_what(FOSSIL_ERROR_INVALID_OPERATION);
    ASSUME_ITS_EQUAL_CSTR("Invalid operation.", result);
}

FOSSIL_TEST(cpp_test_io_what_unknown) {
    const char *result = fossil_io_what(FOSSIL_ERROR_UNKNOWN);
    ASSUME_ITS_EQUAL_CSTR("Unknown error.", result);
}

FOSSIL_TEST(cpp_test_io_what_custom) {
    const char *result = fossil_io_what(FOSSIL_ERROR_CUSTOM);
    ASSUME_ITS_EQUAL_CSTR("Custom error occurred.", result);
}

FOSSIL_TEST(cpp_test_io_what_internal) {
    const char *result = fossil_io_what(FOSSIL_ERROR_INTERNAL);
    ASSUME_ITS_EQUAL_CSTR("Internal error.", result);
}

FOSSIL_TEST(cpp_test_io_what_unknown_error_code) {
    const char *result = fossil_io_what(FOSSIL_ERROR_UNKNOWN_ERROR_CODE);
    ASSUME_ITS_EQUAL_CSTR("Unknown error code.", result);
}

FOSSIL_TEST(cpp_test_io_what_overflow_int) {
    const char *result = fossil_io_what(FOSSIL_ERROR_OVERFLOW_INT);
    ASSUME_ITS_EQUAL_CSTR("Integer overflow.", result);
}

FOSSIL_TEST(cpp_test_io_what_underflow_int) {
    const char *result = fossil_io_what(FOSSIL_ERROR_UNDERFLOW_INT);
    ASSUME_ITS_EQUAL_CSTR("Integer underflow.", result);
}

FOSSIL_TEST(cpp_test_io_what_overflow_float) {
    const char *result = fossil_io_what(FOSSIL_ERROR_OVERFLOW_FLOAT);
    ASSUME_ITS_EQUAL_CSTR("Float overflow.", result);
}

FOSSIL_TEST(cpp_test_io_what_underflow_float) {
    const char *result = fossil_io_what(FOSSIL_ERROR_UNDERFLOW_FLOAT);
    ASSUME_ITS_EQUAL_CSTR("Float underflow.", result);
}

FOSSIL_TEST(cpp_test_io_what_division_by_zero) {
    const char *result = fossil_io_what(FOSSIL_ERROR_DIVISION_BY_ZERO);
    ASSUME_ITS_EQUAL_CSTR("Division by zero.", result);
}

FOSSIL_TEST(cpp_test_io_what_invalid_cast) {
    const char *result = fossil_io_what(FOSSIL_ERROR_INVALID_CAST);
    ASSUME_ITS_EQUAL_CSTR("Invalid type cast.", result);
}

FOSSIL_TEST(cpp_test_io_what_out_of_memory) {
    const char *result = fossil_io_what(FOSSIL_ERROR_OUT_OF_MEMORY);
    ASSUME_ITS_EQUAL_CSTR("Out of memory.", result);
}

FOSSIL_TEST(cpp_test_io_what_memory_corruption) {
    const char *result = fossil_io_what(FOSSIL_ERROR_MEMORY_CORRUPTION);
    ASSUME_ITS_EQUAL_CSTR("Memory corruption detected.", result);
}

FOSSIL_TEST(cpp_test_io_what_buffer_overflow) {
    const char *result = fossil_io_what(FOSSIL_ERROR_BUFFER_OVERFLOW);
    ASSUME_ITS_EQUAL_CSTR("Buffer overflow.", result);
}

FOSSIL_TEST(cpp_test_io_what_buffer_underflow) {
    const char *result = fossil_io_what(FOSSIL_ERROR_BUFFER_UNDERFLOW);
    ASSUME_ITS_EQUAL_CSTR("Buffer underflow.", result);
}

FOSSIL_TEST(cpp_test_io_what_file_not_found) {
    const char *result = fossil_io_what(FOSSIL_ERROR_FILE_NOT_FOUND);
    ASSUME_ITS_EQUAL_CSTR("File not found.", result);
}

FOSSIL_TEST(cpp_test_io_what_permission_denied) {
    const char *result = fossil_io_what(FOSSIL_ERROR_PERMISSION_DENIED);
    ASSUME_ITS_EQUAL_CSTR("Permission denied.", result);
}

FOSSIL_TEST(cpp_test_io_what_network_failure) {
    const char *result = fossil_io_what(FOSSIL_ERROR_NETWORK_FAILURE);
    ASSUME_ITS_EQUAL_CSTR("Network failure.", result);
}

FOSSIL_TEST(cpp_test_io_what_timeout) {
    const char *result = fossil_io_what(FOSSIL_ERROR_TIMEOUT);
    ASSUME_ITS_EQUAL_CSTR("Network timeout.", result);
}

FOSSIL_TEST(cpp_test_io_what_unknown_host) {
    const char *result = fossil_io_what(FOSSIL_ERROR_UNKNOWN_HOST);
    ASSUME_ITS_EQUAL_CSTR("Unknown host.", result);
}

FOSSIL_TEST(cpp_test_io_what_connection_refused) {
    const char *result = fossil_io_what(FOSSIL_ERROR_CONNECTION_REFUSED);
    ASSUME_ITS_EQUAL_CSTR("Connection refused.", result);
}

FOSSIL_TEST(cpp_test_io_what_sql_injection) {
    const char *result = fossil_io_what(FOSSIL_ERROR_SQL_INJECTION);
    ASSUME_ITS_EQUAL_CSTR("SQL injection attempt detected.", result);
}

FOSSIL_TEST(cpp_test_io_what_xss_attack) {
    const char *result = fossil_io_what(FOSSIL_ERROR_XSS_ATTACK);
    ASSUME_ITS_EQUAL_CSTR("Cross-site scripting attack detected.", result);
}

FOSSIL_TEST(cpp_test_io_what_csrf_attack) {
    const char *result = fossil_io_what(FOSSIL_ERROR_CSRF_ATTACK);
    ASSUME_ITS_EQUAL_CSTR("Cross-site request forgery attack detected.", result);
}

FOSSIL_TEST(cpp_test_io_what_user_abort) {
    const char *result = fossil_io_what(FOSSIL_ERROR_USER_ABORT);
    ASSUME_ITS_EQUAL_CSTR("User aborted operation.", result);
}

FOSSIL_TEST(cpp_test_io_what_database_connection_failed) {
    const char *result = fossil_io_what(FOSSIL_ERROR_DATABASE_CONNECTION_FAILED);
    ASSUME_ITS_EQUAL_CSTR("Database connection failed.", result);
}

FOSSIL_TEST(cpp_test_io_what_serialization_failed) {
    const char *result = fossil_io_what(FOSSIL_ERROR_SERIALIZATION_FAILED);
    ASSUME_ITS_EQUAL_CSTR("Serialization failed.", result);
}

FOSSIL_TEST(cpp_test_io_error_exception_no_error) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::OK);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("No error, operation successful.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::OK);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_CNULL_pointer) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::NULL_POINTER);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Null pointer encountered.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::NULL_POINTER);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_invalid_argument) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::INVALID_ARGUMENT);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Invalid argument provided.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::INVALID_ARGUMENT);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_type_mismatch) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::TYPE_MISMATCH);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Type mismatch encountered.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::TYPE_MISMATCH);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_invalid_operation) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::INVALID_OPERATION);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Invalid operation.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::INVALID_OPERATION);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_unknown) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::UNKNOWN);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Unknown error.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::UNKNOWN);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_custom) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::CUSTOM);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Custom error occurred.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::CUSTOM);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_internal) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::INTERNAL);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Internal error.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::INTERNAL);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_unknown_error_code) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::UNKNOWN_ERROR_CODE);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Unknown error code.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::UNKNOWN_ERROR_CODE);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_overflow_int) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::OVERFLOW_INT);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Integer overflow.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::OVERFLOW_INT);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_underflow_int) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::UNDERFLOW_INT);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Integer underflow.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::UNDERFLOW_INT);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_overflow_float) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::OVERFLOW_FLOAT);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Float overflow.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::OVERFLOW_FLOAT);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_underflow_float) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::UNDERFLOW_FLOAT);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Float underflow.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::UNDERFLOW_FLOAT);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_division_by_zero) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::DIVISION_BY_ZERO);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Division by zero.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::DIVISION_BY_ZERO);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_invalid_cast) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::INVALID_CAST);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Invalid type cast.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::INVALID_CAST);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_out_of_memory) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::OUT_OF_MEMORY);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Out of memory.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::OUT_OF_MEMORY);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_memory_corruption) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::MEMORY_CORRUPTION);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Memory corruption detected.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::MEMORY_CORRUPTION);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_buffer_overflow) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::BUFFER_OVERFLOW);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Buffer overflow.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::BUFFER_OVERFLOW);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_buffer_underflow) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::BUFFER_UNDERFLOW);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Buffer underflow.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::BUFFER_UNDERFLOW);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_file_not_found) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::FILE_NOT_FOUND);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("File not found.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::FILE_NOT_FOUND);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_permission_denied) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::PERMISSION_DENIED);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Permission denied.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::PERMISSION_DENIED);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_network_failure) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::NETWORK_FAILURE);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Network failure.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::NETWORK_FAILURE);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_timeout) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::TIMEOUT);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Network timeout.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::TIMEOUT);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_unknown_host) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::UNKNOWN_HOST);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Unknown host.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::UNKNOWN_HOST);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_connection_refused) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::CONNECTION_REFUSED);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Connection refused.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::CONNECTION_REFUSED);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_sql_injection) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::SQL_INJECTION);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("SQL injection attempt detected.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::SQL_INJECTION);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_xss_attack) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::XSS_ATTACK);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Cross-site scripting attack detected.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::XSS_ATTACK);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_csrf_attack) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::CSRF_ATTACK);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Cross-site request forgery attack detected.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::CSRF_ATTACK);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_user_abort) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::USER_ABORT);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("User aborted operation.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::USER_ABORT);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_database_connection_failed) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::DATABASE_CONNECTION_FAILED);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Database connection failed.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::DATABASE_CONNECTION_FAILED);
    }
}

FOSSIL_TEST(cpp_test_io_error_exception_serialization_failed) {
    try {
        throw fossil::io::Error(fossil::io::ErrorCode::SERIALIZATION_FAILED);
    } catch (const fossil::io::Error& e) {
        ASSUME_ITS_EQUAL_CSTR("Serialization failed.", e.what());
        ASSUME_ITS_EQUAL_I32(e.code(), fossil::io::ErrorCode::SERIALIZATION_FAILED);
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_error_tests) {
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_no_error);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_CNULL_pointer);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_invalid_argument);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_type_mismatch);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_invalid_operation);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_unknown);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_custom);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_internal);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_unknown_error_code);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_overflow_int);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_underflow_int);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_overflow_float);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_underflow_float);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_division_by_zero);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_invalid_cast);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_out_of_memory);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_memory_corruption);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_buffer_overflow);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_buffer_underflow);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_file_not_found);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_permission_denied);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_network_failure);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_timeout);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_unknown_host);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_connection_refused);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_sql_injection);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_xss_attack);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_csrf_attack);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_user_abort);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_database_connection_failed);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_what_serialization_failed);

    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_no_error);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_CNULL_pointer);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_invalid_argument);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_type_mismatch);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_invalid_operation);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_unknown);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_custom);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_internal);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_unknown_error_code);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_overflow_int);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_underflow_int);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_overflow_float);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_underflow_float);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_division_by_zero);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_invalid_cast);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_out_of_memory);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_memory_corruption);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_buffer_overflow);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_buffer_underflow);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_file_not_found);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_permission_denied);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_network_failure);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_timeout);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_unknown_host);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_connection_refused);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_sql_injection);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_xss_attack);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_csrf_attack);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_user_abort);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_database_connection_failed);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_io_error_exception_serialization_failed);

    FOSSIL_TEST_REGISTER(cpp_error_suite);
}
