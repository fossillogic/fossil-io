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
#ifndef FOSSIL_IO_ERROR_H
#define FOSSIL_IO_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

/*
    ===============================================================================
     Fossil IO — Unified Comedy Error Code Catalogue
    ===============================================================================
    
    OVERVIEW
    --------
    This table defines the complete set of symbolic error identifiers used by
    Fossil IO and all dependent subsystems. Each entry is a stable, human-readable
    string of the form:
    
        <category>.<subcode>
    
    Examples:
        "system.ok"
        "io.read"
        "memory.alloc"
        "network.timeout"
    
    These strings serve as the *canonical identity* of an error condition.
    They are not localized, abbreviated, or user-facing by default. Instead,
    they act as durable keys for:
    
      - internal dispatch and switch-case logic
      - structured logging and diagnostics
      - error classification (retryable, fatal, user-visible, etc.)
      - mapping to human-readable messages
      - ABI-stable error interchange across modules and plugins
    
    
    STABILITY GUARANTEES
    -------------------
    • Entries in this table MUST NEVER be reordered or removed once released.
    • New error codes may be appended at any time.
    • String contents are ABI-stable identifiers; changing spelling or semantics
      is considered a breaking change.
    • Consumers must not rely on array indices directly unless generated enums
      are explicitly provided.
    
    
    RELATIONSHIP TO fossil_io_what()
    --------------------------------
    The function:
    
        const char *fossil_io_what(const char *error_code);
    
    accepts one of the symbolic strings defined in this table and returns a
    descriptive message suitable for logging or diagnostics.
    
    Important notes:
    • fossil_io_what() does NOT return the symbolic name itself.
    • It returns a *human-readable explanation* of the error condition.
    • The returned string is a constant literal and must not be freed.
    • Unknown or unrecognized codes return a generic fallback message.
    
    Typical usage:
    
        fossil_io_error("[%s] %s",
            error_code,
            fossil_io_what(error_code)
        );
    
    
    MULTIPLE RESPONSES PER ERROR CODE (RANDOMIZED MESSAGES)
    -------------------------------------------------------
    Each symbolic error code MAY map to multiple human-readable message variants.
    
    For example, the error code:
    
        "io.read"
    
    may internally map to a set of messages such as:
        1. "I/O read operation failed"
        2. "Failed while reading from input source"
        3. "Unable to read requested data"
        4. "Read operation encountered an error"
        5. "Input stream read failed"
    
    The implementation of fossil_io_what() MAY select one of these variants,
    optionally using:
      - a deterministic hash of the error code
      - a pseudo-random generator
      - a rotating index
      - a build-time or runtime policy
    
    This allows:
    • reduced log monotony
    • improved readability during debugging
    • richer diagnostics without changing error identity
    
    IMPORTANT:
    The symbolic error code itself remains the *single source of truth*.
    Randomized messages must never alter control flow or semantics.
    
    
    INTERNAL DISPATCH AND SWITCH-CASE LOGIC
    --------------------------------------
    Although error codes are strings, they are intentionally structured to allow
    efficient internal classification.
    
    Common patterns include:
    • prefix matching (e.g., "io.", "memory.", "network.")
    • category extraction for switch-case logic
    • mapping to severity or retry policies
    
    Example:
    
        if (strncmp(code, "memory.", 7) == 0) {
            // treat as fatal or near-fatal
        }
    
    This avoids large enums while retaining clarity and extensibility.
    
    
    RELATIONSHIP TO fossil_io_error()
    ---------------------------------
    The function:
    
        void fossil_io_error(const char *format, ...);
    
    is a formatting and reporting mechanism only. It does NOT define error identity.
    
    Recommended pattern:
    
        fossil_io_error(
            "[%s] %s",
            error_code,
            fossil_io_what(error_code)
        );
    
    The symbolic error code should always be included in logs to ensure:
    • machine readability
    • postmortem analysis
    • stable filtering and aggregation
    
    
    EXTENSIBILITY NOTES
    -------------------
    This table is designed to scale indefinitely.
    
    Future extensions may include:
    • auto-generated enums mirroring this table
    • bitmask traits (fatal, retryable, transient, user-visible)
    • localization layers keyed off symbolic codes
    • wire-format compression or hashing
    • structured error objects carrying both code and context
    
    None of these extensions require changing the existing error codes.
    
    
    SUMMARY
    -------
    • Symbolic error codes are stable identifiers, not messages
    • fossil_io_what() maps codes → descriptive text
    • Multiple randomized messages per code are supported and encouraged
    • Control flow must depend on the code, not the message
    • This table is the authoritative registry for all Fossil IO errors
    
    ===============================================================================
*/

/*
    ===============================================================================
     Fossil IO Error System — Usage Options & Best Practices
    ===============================================================================
    
    This comment documents *all supported usage patterns* for the Fossil IO error
    code system, including when to rely on high-level categories versus specific
    error code identifiers, and how to integrate error handling cleanly across
    layers.
    
    ------------------------------------------------------------------------------
     1. ERROR CODE STRUCTURE
    ------------------------------------------------------------------------------
    Each error condition is identified by a stable string of the form:
    
        <category>.<subcode>
    
    Examples:
        "io.read"
        "memory.alloc"
        "network.timeout"
        "parse.syntax"
    
    The category expresses the *origin domain* of the error.
    The subcode expresses the *specific failure condition*.
    
    These identifiers are ABI-stable and must be treated as canonical keys.
    
    
    ------------------------------------------------------------------------------
     2. CATEGORY-LEVEL USAGE (COARSE-GRAINED)
    ------------------------------------------------------------------------------
    Category-level handling is appropriate when:
    
    • The response is the same for all errors in a domain
    • You are implementing policy or recovery logic
    • You want to avoid brittle dependency on specific subcodes
    • You are writing infrastructure, not application logic
    
    Typical use cases:
    • Determining retry behavior
    • Determining severity (fatal vs recoverable)
    • Selecting logging channels
    • Enforcing security or audit policies
    • Resource cleanup decisions
    
    Examples:
    • "memory.*" → treat as fatal or near-fatal
    • "network.*" → possibly retryable
    • "user.*" → user-visible error
    • "security.*" → audit + deny
    • "system.*" → escalate or abort
    
    Best practice:
        Use category matching (e.g., prefix checks) in core control flow.
    
    
    ------------------------------------------------------------------------------
     3. FULL ERROR CODE USAGE (FINE-GRAINED)
    ------------------------------------------------------------------------------
    Full error code identifiers should be used when:
    
    • You need to distinguish between closely related failures
    • The handling differs materially between subcases
    • You are emitting diagnostics or logs
    • You are writing tests or assertions
    • You are communicating errors across module boundaries
    
    Typical use cases:
    • Logging and telemetry
    • Error message selection (via fossil_io_what)
    • API error reporting
    • Debugging and postmortem analysis
    • Protocol or wire-level error reporting
    
    Examples:
    • "io.read" vs "io.timeout"
    • "memory.alloc" vs "memory.use_after_free"
    • "database.deadlock" vs "database.timeout"
    
    Best practice:
        Always log or report the *full* error code string.
    
    
    ------------------------------------------------------------------------------
     4. HUMAN-READABLE MESSAGES
    ------------------------------------------------------------------------------
    Human-readable error messages are derived from error codes, never the reverse.
    
    Key rules:
    • Messages are descriptive, not authoritative
    • Messages may change without breaking ABI
    • Messages may be randomized or rotated
    • Control flow must never depend on message text
    
    fossil_io_what() maps:
        error code → descriptive message
    
    Best practice:
        Treat messages as diagnostics only.
    
    
    ------------------------------------------------------------------------------
     5. MULTIPLE MESSAGE VARIANTS PER ERROR CODE
    ------------------------------------------------------------------------------
    Each error code may map to multiple descriptive messages.
    
    Allowed strategies:
    • Fixed message
    • Rotating messages
    • Pseudo-random selection
    • Hash-based deterministic selection
    
    Purpose:
    • Reduce log fatigue
    • Improve readability
    • Preserve human attention
    
    Restrictions:
    • Message variation must not alter semantics
    • The error code remains the single source of truth
    
    
    ------------------------------------------------------------------------------
     6. INTERNAL DISPATCH STRATEGY
    ------------------------------------------------------------------------------
    Recommended hierarchy for decision-making:
    
        1. Category
        2. Error code
        3. Contextual data (if available)
        4. Message (never)
    
    Example:
        • Category → retry or abort
        • Error code → specific recovery path
        • Message → logging only
    
    Best practice:
        Use categories for policy, codes for precision.
    
    
    ------------------------------------------------------------------------------
     7. ERROR PROPAGATION ACROSS LAYERS
    ------------------------------------------------------------------------------
    When propagating errors upward:
    
    • Preserve the original error code whenever possible
    • Avoid collapsing distinct errors into generic ones
    • Do not stringify errors prematurely
    • Do not translate codes unless crossing an abstraction boundary
    
    If translation is required:
    • Map to the closest semantic equivalent
    • Document the translation explicitly
    
    
    ------------------------------------------------------------------------------
     8. PUBLIC API VS INTERNAL ERRORS
    ------------------------------------------------------------------------------
    Public-facing APIs:
    • May expose symbolic error codes
    • Should not expose internal-only subcodes
    • Should document the possible codes
    
    Internal APIs:
    • May use the full catalog freely
    • May introduce internal-only codes (prefixed appropriately)
    
    Best practice:
        Maintain a clear boundary between public and internal error domains.
    
    
    ------------------------------------------------------------------------------
     9. LOGGING AND TELEMETRY
    ------------------------------------------------------------------------------
    Required fields in logs:
    • Full error code string
    • Human-readable message
    • Contextual metadata (file, line, resource, etc.)
    
    Optional fields:
    • Category
    • Retry count
    • Severity
    
    Best practice:
        Error codes should be indexable and filterable in logs.
    
    
    ------------------------------------------------------------------------------
    10. TESTING AND ASSERTIONS
    ------------------------------------------------------------------------------
    Use error codes in tests when:
    
    • Verifying specific failure modes
    • Ensuring correct error propagation
    • Preventing regression in error semantics
    
    Avoid testing against:
    • Message text
    • Message ordering
    • Randomized message selection
    
    
    ------------------------------------------------------------------------------
    11. WHAT NOT TO DO (ANTI-PATTERNS)
    ------------------------------------------------------------------------------
    ✗ Do not branch logic on message strings
    ✗ Do not invent ad-hoc error strings
    ✗ Do not overload one error code with unrelated meanings
    ✗ Do not remove or rename existing codes
    ✗ Do not expose raw internal messages to users
    
    
    ------------------------------------------------------------------------------
    12. LONG-TERM MAINTENANCE GUIDELINES
    ------------------------------------------------------------------------------
    • Append new error codes; never reorder
    • Prefer adding a new subcode over changing semantics
    • Keep categories broad and stable
    • Let subcodes evolve
    • Treat error codes as part of the public contract
    
    
    ------------------------------------------------------------------------------
    SUMMARY
    ------------------------------------------------------------------------------
    • Categories are for policy and control flow
    • Error code IDs are for precision and diagnostics
    • Messages are for humans, not machines
    • Stability beats cleverness
    • This system is designed to last for decades
    
    ===============================================================================
*/

/**
 * This function is used to report an error message with a formatted string.
 *
 * @param format The format string for the error message.
 * @param ... The additional arguments to be formatted.
 */
void fossil_io_error(const char *format, ...);

/**
 * This function returns a string literal representing the symbolic name
 * of the given error code, such as "system.ok", "io.read", or "memory.alloc".
 * The returned string is intended to be a stable, human-readable identifier
 * for the error code, suitable for logging, debugging, or diagnostics.
 *
 * @param error_code The error code whose symbolic name is to be retrieved.
 * @return A constant string literal describing the symbolic name of the error code,
 *         or "unknown" if the code is not recognized.
 */
const char *fossil_io_what(const char *error_code);

#ifdef __cplusplus
}

#include <string>

namespace fossil {

    namespace io {
    
        class Error final {
        public:
            Error() = delete; // static-only utility class
        
            /**
             * Report an error using printf-style formatting.
             */
            static void report(const char *format, ...) noexcept {
                va_list args;
                va_start(args, format);
                fossil_io_error(format, args);
                va_end(args);
            }
        
            /**
             * Retrieve the symbolic name for an error code.
             *
             * Example: "system.ok", "io.read", "memory.alloc"
             */
            static const char *what(const char *error_code) noexcept {
                return fossil_io_what(error_code);
            }
        
            /**
             * Convenience overload for std::string.
             */
            static const char *what(const std::string &error_code) noexcept {
                return fossil_io_what(error_code.c_str());
            }
        };
    
    } // namespace io

} // namespace fossil

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
