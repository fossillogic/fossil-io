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
#include "fossil/io/error.h"
#include "fossil/io/soap.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define FOSSIL_IO_BUFFER_SIZE 1024

// Function to print sanitized error messages
void fossil_io_error(const char *format, ...) {
    va_list args;
    va_start(args, format);

    // Create a buffer to hold the formatted error message
    char buffer[FOSSIL_IO_BUFFER_SIZE];
    vsnprintf(buffer, sizeof(buffer), format, args);

    // Print the sanitized error message
    fprintf(stderr, "ERROR: %s\n", buffer);

    va_end(args);
}

const char *fossil_io_what(fossil_status_t error_code) {
    switch (error_code) {
        // Success and General Errors
        case FOSSIL_ERROR_OK: return "No error, operation successful.";
        case FOSSIL_ERROR_CNULL_POINTER: return "Null pointer encountered.";
        case FOSSIL_ERROR_INVALID_ARGUMENT: return "Invalid argument provided.";
        case FOSSIL_ERROR_TYPE_MISMATCH: return "Type mismatch encountered.";
        case FOSSIL_ERROR_INVALID_OPERATION: return "Invalid operation.";
        case FOSSIL_ERROR_UNKNOWN: return "Unknown error.";
        case FOSSIL_ERROR_CUSTOM: return "Custom error occurred.";
        case FOSSIL_ERROR_INTERNAL: return "Internal error.";
        case FOSSIL_ERROR_UNKNOWN_ERROR_CODE: return "Unknown error code.";

        // Arithmetic Errors
        case FOSSIL_ERROR_OVERFLOW_INT: return "Integer overflow.";
        case FOSSIL_ERROR_UNDERFLOW_INT: return "Integer underflow.";
        case FOSSIL_ERROR_OVERFLOW_FLOAT: return "Float overflow.";
        case FOSSIL_ERROR_UNDERFLOW_FLOAT: return "Float underflow.";
        case FOSSIL_ERROR_DIVISION_BY_ZERO: return "Division by zero.";
        case FOSSIL_ERROR_INVALID_CAST: return "Invalid type cast.";

        // Memory Errors
        case FOSSIL_ERROR_OUT_OF_MEMORY: return "Out of memory.";
        case FOSSIL_ERROR_MEMORY_CORRUPTION: return "Memory corruption detected.";
        case FOSSIL_ERROR_BUFFER_OVERFLOW: return "Buffer overflow.";
        case FOSSIL_ERROR_BUFFER_UNDERFLOW: return "Buffer underflow.";
        case FOSSIL_ERROR_BUFFER_OVERFLOW_STR: return "String buffer overflow.";
        case FOSSIL_ERROR_BUFFER_OVERFLOW_FMT: return "Format buffer overflow.";
        case FOSSIL_ERROR_BUFFER_OVERFLOW_FILE: return "File buffer overflow.";
        case FOSSIL_ERROR_BUFFER_OVERFLOW_NET: return "Network buffer overflow.";
        case FOSSIL_ERROR_BUFFER_OVERFLOW_CMD: return "Command buffer overflow.";
        case FOSSIL_ERROR_BUFFER_OVERFLOW_ENV: return "Environment buffer overflow.";
        case FOSSIL_ERROR_MEMORY_LEAK: return "Memory leak detected.";
        case FOSSIL_ERROR_MEMORY_ALLOCATION_FAILURE: return "Memory allocation failure.";
        case FOSSIL_ERROR_MEMORY_ALIGNMENT_ISSUE: return "Memory alignment issue.";

        // File and I/O Errors
        case FOSSIL_ERROR_FILE_CORRUPTION: return "File corruption detected.";
        case FOSSIL_ERROR_FILE_NOT_FOUND: return "File not found.";
        case FOSSIL_ERROR_DIRECTORY_NOT_FOUND: return "Directory not found.";
        case FOSSIL_ERROR_TOO_MANY_OPEN_FILES: return "Too many open files.";
        case FOSSIL_ERROR_IO: return "Input/output error.";
        case FOSSIL_ERROR_UNSUPPORTED_OPERATION: return "Operation not supported.";
        case FOSSIL_ERROR_PIPE: return "Pipe error.";
        case FOSSIL_ERROR_SYSTEM_CALL_FAILED: return "System call failed.";
        case FOSSIL_ERROR_PERMISSION_DENIED: return "Permission denied.";
        case FOSSIL_ERROR_READ_ONLY_FILESYSTEM: return "Filesystem is read-only.";
        case FOSSIL_ERROR_FILE_LOCK_FAILED: return "File lock failed.";
        case FOSSIL_ERROR_FILE_WRITE_FAILED: return "File write failed.";
        case FOSSIL_ERROR_FILE_READ_FAILED: return "File read failed.";
        case FOSSIL_ERROR_FILE_SEEK_FAILED: return "File seek failed.";
        case FOSSIL_ERROR_FILE_CLOSE_FAILED: return "File close failed.";
        case FOSSIL_ERROR_FILE_TRUNCATE_FAILED: return "File truncate failed.";
        case FOSSIL_ERROR_FILE_TOO_LARGE: return "File is too large.";
        case FOSSIL_ERROR_DEVICE_NOT_READY: return "Device not ready.";
        case FOSSIL_ERROR_DEVICE_REMOVED: return "Device was removed.";

        // Resource and Process Errors
        case FOSSIL_ERROR_RESOURCE_UNAVAILABLE: return "Resource unavailable.";
        case FOSSIL_ERROR_BUSY: return "Resource is busy.";
        case FOSSIL_ERROR_DEADLOCK: return "Deadlock detected.";
        case FOSSIL_ERROR_INTERRUPTED: return "Operation interrupted.";
        case FOSSIL_ERROR_LIMIT_REACHED: return "Resource limit reached.";
        case FOSSIL_ERROR_PROCESS: return "Process-related error.";
        case FOSSIL_ERROR_PROCESS_CREATION_FAILED: return "Process creation failed.";
        case FOSSIL_ERROR_PROCESS_EXITED_WITH_ERROR: return "Process exited with an error.";
        case FOSSIL_ERROR_PROCESS_TIMEOUT: return "Process timeout.";
        case FOSSIL_ERROR_RESOURCE_DEPLETION: return "Resource depletion.";
        case FOSSIL_ERROR_THREAD_CREATION_FAILED: return "Thread creation failed.";
        case FOSSIL_ERROR_THREAD_TERMINATION_FAILED: return "Thread termination failed.";

        // Hardware Errors
        case FOSSIL_ERROR_HARDWARE_FAILURE: return "Hardware failure.";
        case FOSSIL_ERROR_HARD_DISK_FAILURE: return "Hard disk failure.";
        case FOSSIL_ERROR_CPU_OVERHEAT: return "CPU overheat detected.";
        case FOSSIL_ERROR_MEMORY_FAILURE: return "Memory failure.";
        case FOSSIL_ERROR_HARDWARE_OVERLOAD: return "Hardware overload.";
        case FOSSIL_ERROR_DEVICE_TIMEOUT: return "Device timeout.";
        case FOSSIL_ERROR_POWER_SUPPLY_FAILURE: return "Power supply failure.";
        case FOSSIL_ERROR_MOTHERBOARD_FAILURE: return "Motherboard failure.";
        case FOSSIL_ERROR_CPU_FAILURE: return "CPU failure.";
        case FOSSIL_ERROR_RAM_FAILURE: return "RAM failure.";
        case FOSSIL_ERROR_SSD_FAILURE: return "SSD failure.";
        case FOSSIL_ERROR_HDD_FAILURE: return "HDD failure.";
        case FOSSIL_ERROR_FAN_FAILURE: return "Fan failure.";
        case FOSSIL_ERROR_GPU_FAILURE: return "GPU failure.";
        case FOSSIL_ERROR_BATTERY_FAILURE: return "Battery failure.";
        case FOSSIL_ERROR_SENSOR_FAILURE: return "Sensor failure.";
        case FOSSIL_ERROR_USB_CONTROLLER_FAILURE: return "USB controller failure.";
        case FOSSIL_ERROR_PCI_DEVICE_FAILURE: return "PCI device failure.";
        case FOSSIL_ERROR_NETWORK_CARD_FAILURE: return "Network card failure.";
        case FOSSIL_ERROR_SOUND_CARD_FAILURE: return "Sound card failure.";
        case FOSSIL_ERROR_DISPLAY_ADAPTER_FAILURE: return "Display adapter failure.";
        case FOSSIL_ERROR_POWER_SUPPLY_OVERVOLTAGE: return "Power supply overvoltage.";
        case FOSSIL_ERROR_POWER_SUPPLY_UNDERVOLTAGE: return "Power supply undervoltage.";
        case FOSSIL_ERROR_THERMAL_SHUTDOWN: return "Thermal shutdown.";
        case FOSSIL_ERROR_FIRMWARE_CORRUPTION: return "Firmware corruption.";
        case FOSSIL_ERROR_HARDWARE_INCOMPATIBILITY: return "Hardware incompatibility.";
        case FOSSIL_ERROR_HARDWARE_NOT_DETECTED: return "Hardware not detected.";
        case FOSSIL_ERROR_HARDWARE_DRIVER_MISSING: return "Hardware driver missing.";
        case FOSSIL_ERROR_HARDWARE_DRIVER_CORRUPTED: return "Hardware driver corrupted.";

        // Network Errors
        case FOSSIL_ERROR_NETWORK_FAILURE: return "Network failure.";
        case FOSSIL_ERROR_TIMEOUT: return "Network timeout.";
        case FOSSIL_ERROR_UNKNOWN_HOST: return "Unknown host.";
        case FOSSIL_ERROR_CONNECTION_REFUSED: return "Connection refused.";
        case FOSSIL_ERROR_NETWORK_UNREACHABLE: return "Network unreachable.";
        case FOSSIL_ERROR_PROTOCOL: return "Protocol error.";
        case FOSSIL_ERROR_CONNECTION_RESET: return "Connection reset by peer.";
        case FOSSIL_ERROR_DNS_RESOLUTION_FAILED: return "DNS resolution failed.";
        case FOSSIL_ERROR_NETWORK_CONGESTION: return "Network congestion.";
        case FOSSIL_ERROR_BANDWIDTH_EXCEEDED: return "Bandwidth exceeded.";
        case FOSSIL_ERROR_FIREWALL_BLOCKED: return "Connection blocked by firewall.";
        case FOSSIL_ERROR_SSL_HANDSHAKE_FAILED: return "SSL handshake failed.";
        case FOSSIL_ERROR_CERTIFICATE_EXPIRED: return "SSL certificate expired.";
        case FOSSIL_ERROR_CERTIFICATE_REVOKED: return "SSL certificate revoked.";

        // Security Errors
        case FOSSIL_ERROR_SQL_INJECTION: return "SQL injection attempt detected.";
        case FOSSIL_ERROR_XSS_ATTACK: return "Cross-site scripting attack detected.";
        case FOSSIL_ERROR_CSRF_ATTACK: return "Cross-site request forgery attack detected.";
        case FOSSIL_ERROR_FORMAT_STRING_ATTACK: return "Format string attack detected.";
        case FOSSIL_ERROR_CRYPTOGRAPHIC_WEAKNESS: return "Cryptographic weakness detected.";
        case FOSSIL_ERROR_INSECURE_RANDOMNESS: return "Insecure randomness detected.";
        case FOSSIL_ERROR_INSECURE_CONFIGURATION: return "Insecure configuration.";
        case FOSSIL_ERROR_INSECURE_DESERIALIZATION: return "Insecure deserialization detected.";
        case FOSSIL_ERROR_INSECURE_FILE_HANDLING: return "Insecure file handling detected.";
        case FOSSIL_ERROR_INSECURE_TEMPORARY_FILES: return "Insecure temporary files detected.";
        case FOSSIL_ERROR_INSECURE_COMMUNICATION: return "Insecure communication detected.";
        case FOSSIL_ERROR_INSECURE_AUTHENTICATION: return "Insecure authentication detected.";
        case FOSSIL_ERROR_INSECURE_ACCESS_CONTROL: return "Insecure access control detected.";
        case FOSSIL_ERROR_SESSION_HIJACKING: return "Session hijacking attempt detected.";
        case FOSSIL_ERROR_DATA_TAMPERING: return "Data tampering detected.";
        case FOSSIL_ERROR_MAN_IN_THE_MIDDLE_ATTACK: return "Man-in-the-middle attack detected.";
        case FOSSIL_ERROR_PRIVILEGE_ESCALATION: return "Privilege escalation detected.";

        // User-Related and External Errors
        case FOSSIL_ERROR_USER_ABORT: return "User aborted operation.";
        case FOSSIL_ERROR_SIGNAL: return "Signal error.";
        case FOSSIL_ERROR_USER_INPUT_TOO_LARGE: return "User input is too large.";
        case FOSSIL_ERROR_USER_INPUT_INVALID_FORMAT: return "User input has an invalid format.";
        case FOSSIL_ERROR_USER_PERMISSION_REJECTED: return "User permission rejected.";

        // Database Errors
        case FOSSIL_ERROR_DATABASE_CONNECTION_FAILED: return "Database connection failed.";
        case FOSSIL_ERROR_DATABASE_QUERY_FAILED: return "Database query failed.";
        case FOSSIL_ERROR_DATABASE_TIMEOUT: return "Database operation timed out.";
        case FOSSIL_ERROR_DATABASE_CORRUPTION: return "Database corruption detected.";
        case FOSSIL_ERROR_DATABASE_DEADLOCK: return "Database deadlock detected.";
        case FOSSIL_ERROR_DATABASE_LOCK_FAILED: return "Database lock failed.";

        // Serialization/Deserialization Errors
        case FOSSIL_ERROR_SERIALIZATION_FAILED: return "Serialization failed.";
        case FOSSIL_ERROR_DESERIALIZATION_FAILED: return "Deserialization failed.";
        case FOSSIL_ERROR_INVALID_SERIALIZATION_FORMAT: return "Invalid serialization format.";
        case FOSSIL_ERROR_DATA_CORRUPTION: return "Data corruption detected.";

        // Miscellaneous Errors
        case FOSSIL_ERROR_INDEX_OUT_OF_BOUNDS: return "Index out of bounds.";
        case FOSSIL_ERROR_FORMAT: return "Format error.";
        case FOSSIL_ERROR_INCOMPATIBLE_VERSION: return "Incompatible version.";
        case FOSSIL_ERROR_JSON_PARSING_FAILED: return "JSON parsing failed.";
        case FOSSIL_ERROR_XML_PARSING_FAILED: return "XML parsing failed.";
        case FOSSIL_ERROR_YAML_PARSING_FAILED: return "YAML parsing failed.";
        case FOSSIL_ERROR_INVALID_CHECKSUM: return "Invalid checksum.";
        case FOSSIL_ERROR_TIMER_EXPIRED: return "Timer expired.";

        // Truthful Intelligence Errors (TI)
        case FOSSIL_ERROR_TI_MODEL_NOT_FOUND: return "TI: Model not found.";
        case FOSSIL_ERROR_TI_INFERENCE_FAILED: return "TI: Inference failed.";
        case FOSSIL_ERROR_TI_TRAINING_FAILED: return "TI: Training failed.";
        case FOSSIL_ERROR_TI_INVALID_INPUT: return "TI: Invalid input.";
        case FOSSIL_ERROR_TI_UNSUPPORTED_OPERATION: return "TI: Unsupported operation.";
        case FOSSIL_ERROR_TI_TIMEOUT: return "TI: Operation timed out.";
        case FOSSIL_ERROR_TI_MODEL_CORRUPTION: return "TI: Model corruption detected.";
        case FOSSIL_ERROR_TI_INSUFFICIENT_RESOURCES: return "TI: Insufficient resources.";
        case FOSSIL_ERROR_TI_INVALID_CONFIGURATION: return "TI: Invalid configuration.";
        case FOSSIL_ERROR_TI_DATASET_NOT_FOUND: return "TI: Dataset not found.";
        case FOSSIL_ERROR_TI_DATASET_CORRUPTION: return "TI: Dataset corruption detected.";
        case FOSSIL_ERROR_TI_EVALUATION_FAILED: return "TI: Evaluation failed.";
        case FOSSIL_ERROR_TI_UNSUPPORTED_MODEL_TYPE: return "TI: Unsupported model type.";
        case FOSSIL_ERROR_TI_INCOMPATIBLE_MODEL_VERSION: return "TI: Incompatible model version.";
        case FOSSIL_ERROR_TI_INVALID_PARAMETER: return "TI: Invalid parameter.";
        case FOSSIL_ERROR_TI_UNEXPECTED_OUTPUT: return "TI: Unexpected output.";

        default: return "Unknown error code.";
    }
}
