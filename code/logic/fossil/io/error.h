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

/**
 * ==============================================================================
 * Fossil IO Error Code System — Internal Logic, Error Code Catalogue, and Usage
 * ==============================================================================
 *
 * OVERVIEW
 * --------
 * The Fossil IO error code system provides a unified, extensible, and ABI-stable
 * catalogue of symbolic error identifiers for all Fossil IO and dependent subsystems.
 * Each error condition is represented by a canonical string of the form:
 *
 *     <category>.<subcode>
 *
 * Example error codes:
 *     "system.ok"
 *     "io.read"
 *     "memory.alloc"
 *     "network.timeout"
 *
 * These symbolic codes are the authoritative identity for error conditions.
 * They are used for:
 *   - Internal dispatch and switch-case logic
 *   - Structured logging and diagnostics
 *   - Error classification (retryable, fatal, user-visible, etc.)
 *   - Mapping to human-readable messages
 *   - ABI-stable error interchange across modules and plugins
 *
 * ==============================================================================
 * INTERNAL LOGIC
 * ==============================================================================
 *
 * 1. Error Code Table
 * -------------------
 * All valid error codes are listed in a single, ordered array:
 *
 *   static const char *fossil_error_codes[] = { ... };
 *
 * Each entry is a stable string identifier. The array is never reordered or
 * truncated; new codes are appended only.
 *
 * 2. Error Code Lookup
 * --------------------
 * - fossil_io_code(const char *error_code):
 *     Returns the integer index of the error code in fossil_error_codes[], or -1
 *     if not found. This index is stable and can be used for switch-case logic,
 *     compact storage, or serialization.
 *
 * - fossil_io_what(const char *error_code):
 *     Returns a human-readable message describing the error condition. Each code
 *     maps to a set of message variants (see below). The returned string is a
 *     constant literal and must not be freed.
 *
 * 3. Message Variants
 * -------------------
 * Each error code may have multiple descriptive message variants. The implementation
 * may select a variant deterministically or randomly to reduce log monotony and
 * improve diagnostics. The symbolic error code remains the single source of truth.
 *
 * 4. Error Reporting
 * ------------------
 * - fossil_io_error(const char *format, ...):
 *     Formats and prints an error message to stderr. Recommended usage is to
 *     include both the symbolic code and the human-readable message:
 *
 *         fossil_io_error("[%s] %s", error_code, fossil_io_what(error_code));
 *
 * ==============================================================================
 * ERROR CODE CATALOGUE
 * ==============================================================================
 *
 * The following error code strings are available. Each is stable and should be
 * used as the canonical identifier for the corresponding error condition.
 *
 * SYSTEM / META
 *   "system.ok", "system.unknown", "system.internal", "system.fatal", "system.panic",
 *   "system.abort", "system.assertion", "system.invariant", "system.contract",
 *   "system.recoverable", "system.unrecoverable", "system.transient", "system.permanent",
 *   "system.unsupported", "system.unimplemented", "system.deprecated", "system.experimental",
 *   "system.misconfigured", "system.corrupt", "system.bootstrap", "system.shutdown",
 *   "system.restart", "system.upgrade", "system.downgrade", "system.permission",
 *   "system.capability",
 *
 * IO
 *   "io.read", "io.write", "io.seek", "io.flush", "io.sync", "io.fsync", "io.truncate",
 *   "io.append", "io.scatter", "io.gather", "io.closed", "io.eof", "io.partial",
 *   "io.short", "io.blocked", "io.nonblocking", "io.timeout", "io.interrupt", "io.retry",
 *   "io.corrupt", "io.checksum", "io.buffer", "io.alignment", "io.direct", "io.stream",
 *   "io.pipe",
 *
 * MEMORY
 *   "memory.alloc", "memory.realloc", "memory.free", "memory.map", "memory.unmap",
 *   "memory.remap", "memory.lock", "memory.unlock", "memory.protect", "memory.unprotect",
 *   "memory.leak", "memory.overrun", "memory.underrun", "memory.use_after_free",
 *   "memory.double_free", "memory.fragmented", "memory.exhausted", "memory.alignment",
 *   "memory.page_fault", "memory.segmentation", "memory.guard", "memory.poison",
 *   "memory.swap", "memory.numa",
 *
 * CPU / EXECUTION
 *   "cpu.illegal_instruction", "cpu.privilege_violation", "cpu.div_zero", "cpu.overflow",
 *   "cpu.underflow", "cpu.fpu", "cpu.simd", "cpu.cache", "cpu.pipeline", "cpu.affinity",
 *   "cpu.throttle",
 *
 * MATH / NUMERIC
 *   "math.overflow", "math.underflow", "math.div_zero", "math.nan", "math.infinity",
 *   "math.domain", "math.range", "math.precision", "math.rounding", "math.convergence",
 *   "math.divergence", "math.iteration", "math.singularity", "math.condition",
 *   "math.approximation",
 *
 * PARSING / LEXING / GRAMMAR
 *   "parse.invalid", "parse.syntax", "parse.semantic", "parse.context", "parse.state",
 *   "parse.encoding", "parse.unexpected_token", "parse.missing_token", "parse.extra_token",
 *   "parse.ambiguous", "parse.incomplete", "parse.recursion", "parse.depth", "parse.stack",
 *   "parse.overflow", "lexer.invalid", "lexer.token", "lexer.state", "lexer.encoding",
 *   "lexer.buffer", "lexer.escape",
 *
 * TYPE SYSTEM
 *   "type.invalid", "type.mismatch", "type.cast", "type.coercion", "type.size",
 *   "type.range", "type.signedness", "type.alignment", "type.qualifier", "type.generic",
 *   "type.polymorphic", "type.variance",
 *
 * FORMAT / ENCODING
 *   "format.invalid", "format.unsupported", "format.truncated", "format.version",
 *   "format.magic", "format.header", "format.footer", "format.padding", "format.layout",
 *   "encoding.invalid", "encoding.unsupported", "encoding.incomplete", "encoding.locale",
 *   "encoding.endianness", "encoding.normalization", "encoding.compression",
 *   "encoding.decompression",
 *
 * DATA / CONTENT
 *   "data.invalid", "data.corrupt", "data.missing", "data.extra", "data.duplicate",
 *   "data.inconsistent", "data.constraint", "data.integrity", "data.reference",
 *   "data.circular", "data.order", "data.range", "data.null", "data.schema",
 *   "data.version", "data.migration",
 *
 * FILESYSTEM / STORAGE
 *   "fs.not_found", "fs.exists", "fs.permission", "fs.read_only", "fs.locked",
 *   "fs.busy", "fs.mount", "fs.unmount", "fs.remount", "fs.quota", "fs.corrupt",
 *   "fs.journal", "fs.snapshot", "fs.backup", "fs.restore", "fs.path", "fs.symlink",
 *   "fs.hardlink", "fs.inode", "fs.filesystem",
 *
 * PROCESS / SIGNAL
 *   "process.spawn", "process.exec", "process.exit", "process.crash", "process.signal",
 *   "process.kill", "process.zombie", "process.orphan", "process.permission",
 *   "process.limit", "process.priority", "process.affinity",
 *
 * THREAD / CONCURRENCY
 *   "thread.create", "thread.join", "thread.detach", "thread.cancel", "thread.signal",
 *   "concurrency.race", "concurrency.deadlock", "concurrency.livelock",
 *   "concurrency.starvation", "concurrency.atomicity", "concurrency.lock",
 *   "concurrency.unlock", "concurrency.condition", "concurrency.barrier",
 *   "concurrency.scheduler", "concurrency.preemption",
 *
 * RESOURCE
 *   "resource.exhausted", "resource.leak", "resource.locked", "resource.starvation",
 *   "resource.handle", "resource.descriptor", "resource.pool", "resource.cache",
 *   "resource.bandwidth", "resource.quota", "resource.limit",
 *
 * TIME / CLOCK
 *   "time.timeout", "time.expired", "time.schedule", "time.clock", "time.drift",
 *   "time.skew", "time.monotonic", "time.realtime", "time.resolution", "time.wrap",
 *
 * CONFIG / ENVIRONMENT
 *   "config.missing", "config.invalid", "config.conflict", "config.version",
 *   "config.env", "config.profile", "config.permission", "config.schema",
 *   "config.override", "config.default", "config.locked",
 *
 * API / ABI
 *   "api.invalid_call", "api.contract", "api.precondition", "api.postcondition",
 *   "api.version", "api.mismatch", "api.deprecated", "api.timeout", "api.limit",
 *   "api.state", "api.sequence", "api.serialization", "api.deserialization",
 *   "abi.mismatch", "abi.incompatible",
 *
 * PROTOCOL / IPC
 *   "protocol.invalid", "protocol.version", "protocol.handshake", "protocol.negotiation",
 *   "protocol.sequence", "protocol.frame", "protocol.fragment", "protocol.checksum",
 *   "protocol.timeout", "protocol.reset", "protocol.flow_control",
 *
 * NETWORK
 *   "network.unreachable", "network.timeout", "network.reset", "network.refused",
 *   "network.aborted", "network.dns", "network.routing", "network.latency",
 *   "network.bandwidth", "network.congestion", "network.proxy", "network.firewall",
 *   "network.nat", "network.session", "network.stream", "network.packet",
 *
 * SECURITY / CRYPTO
 *   "security.violation", "security.auth_failed", "security.authz_failed",
 *   "security.identity", "security.credential", "security.token", "security.session",
 *   "security.encryption", "security.decryption", "security.certificate", "security.key",
 *   "security.keystore", "security.revocation", "security.sandbox", "security.trust",
 *   "security.integrity", "security.tamper", "security.replay",
 *
 * DATABASE / STORAGE ENGINE
 *   "database.connect", "database.disconnect", "database.query", "database.prepare",
 *   "database.execute", "database.transaction", "database.commit", "database.rollback",
 *   "database.deadlock", "database.lock", "database.constraint", "database.schema",
 *   "database.migration", "database.index", "database.cursor", "database.replication",
 *   "database.consistency", "database.timeout",
 *
 * AI / ML
 *   "ai.model", "ai.version", "ai.load", "ai.unload", "ai.inference", "ai.training",
 *   "ai.finetune", "ai.dataset", "ai.validation", "ai.bias", "ai.drift",
 *   "ai.hallucination", "ai.alignment", "ai.confidence", "ai.explainability",
 *   "ai.prompt", "ai.token_limit", "ai.context_overflow",
 *
 * UI / UX
 *   "ui.render", "ui.layout", "ui.paint", "ui.refresh", "ui.input", "ui.focus",
 *   "ui.gesture", "ui.accessibility", "ui.localization", "ui.theme", "ui.font",
 *   "ui.image",
 *
 * OBSERVABILITY / OPS
 *   "log.write", "log.read", "log.format", "log.rotate", "log.truncate",
 *   "metrics.collect", "metrics.aggregate", "metrics.export", "trace.emit",
 *   "trace.flush", "trace.sample", "monitor.unavailable", "diagnostics.collect",
 *   "diagnostics.dump", "profile.sample",
 *
 * BUILD / DEPLOY
 *   "build.configure", "build.compile", "build.link", "build.package",
 *   "deploy.install", "deploy.remove", "deploy.upgrade", "deploy.rollback",
 *   "deploy.migration", "deploy.orchestration", "deploy.container", "deploy.image",
 *
 * USER
 *   "user.input", "user.permission", "user.quota", "user.cancelled", "user.timeout",
 *   "user.conflict", "user.invalid_state", "user.rate_limit",
 *
 * LEGAL / POLICY
 *   "policy.violation", "policy.denied", "policy.expired", "policy.restricted",
 *   "license.invalid", "license.expired", "license.restricted", "privacy.violation",
 *   "privacy.redaction", "compliance.failure", "audit.failure",
 *
 * FALLBACK / GUARANTEES
 *   "meta.unreachable", "meta.assumption", "meta.placeholder", "meta.future"
 *
 * ==============================================================================
 * BEST PRACTICES FOR ERROR CODE USAGE
 * ==============================================================================
 *
 * 1. Use category-level codes (e.g., "memory.*", "network.*") for broad policy,
 *    retry, or severity logic.
 * 2. Use full error codes (e.g., "io.read", "memory.alloc") for precise diagnostics,
 *    logging, and error propagation.
 * 3. Always log both the symbolic error code and the human-readable message.
 * 4. Never branch control flow on message strings; always use the symbolic code.
 * 5. Never remove or rename existing codes; only append new codes.
 * 6. Do not expose internal-only codes in public APIs unless documented.
 * 7. When propagating errors, preserve the original code whenever possible.
 * 8. Use fossil_io_code() for efficient switch-case or serialization.
 * 9. Use fossil_io_what() for user-facing or log messages, not for control flow.
 * 10. Treat error codes as part of the public contract and maintain stability.
 *
 * ==============================================================================
 * EXTENSIBILITY
 * ==============================================================================
 * - New error codes may be appended at any time.
 * - Future extensions may include enums, bitmask traits, localization, or structured
 *   error objects, all keyed off the symbolic code.
 * - The symbolic code is the single source of truth for error identity.
 *
 * ==============================================================================
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

/**
 * Returns the numeric error code ID associated with a symbolic error code.
 *
 * The returned value is a stable integer identifier derived from the
 * fossil_error_codes[] table. This value is suitable for switch-case logic,
 * compact storage, and serialization.
 *
 * @param error_code The symbolic error code string (e.g., "io.read").
 * @return A non-negative integer error ID on success,
 *         or -1 if the error code is NULL or unrecognized.
 *
 * Stability guarantees:
 * • IDs are stable as long as fossil_error_codes[] ordering is preserved.
 * • New error codes must be appended, never reordered.
 */
int fossil_io_code(const char *error_code);

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

            /**
             * Retrieve the numeric error code ID for a symbolic error code.
             *
             * @param error_code The symbolic error code string (e.g., "io.read").
             * @return A non-negative integer error ID on success,
             *         or -1 if the error code is NULL or unrecognized.
             */
            static int code(const char *error_code) noexcept {
                return fossil_io_code(error_code);
            }

            /**
             * Convenience overload for std::string.
             */
            static int code(const std::string &error_code) noexcept {
                return fossil_io_code(error_code.c_str());
            }
        };
    
    } // namespace io

} // namespace fossil

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
