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
#include "fossil/io/error.h"
#include "fossil/io/soap.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define FOSSIL_IO_BUFFER_SIZE 1024

static const char *fossil_error_codes[] = {
    /* ======================================================================= */
    /* SYSTEM / META                                                            */
    /* ======================================================================= */
    "system.ok",
    "system.unknown",
    "system.internal",
    "system.fatal",
    "system.panic",
    "system.abort",
    "system.assertion",
    "system.invariant",
    "system.contract",
    "system.recoverable",
    "system.unrecoverable",
    "system.transient",
    "system.permanent",
    "system.unsupported",
    "system.unimplemented",
    "system.deprecated",
    "system.experimental",
    "system.misconfigured",
    "system.corrupt",
    "system.bootstrap",
    "system.shutdown",
    "system.restart",
    "system.upgrade",
    "system.downgrade",
    "system.permission",
    "system.capability",
    
    /* ======================================================================= */
    /* IO                                                                       */
    /* ======================================================================= */
    "io.read",
    "io.write",
    "io.seek",
    "io.flush",
    "io.sync",
    "io.fsync",
    "io.truncate",
    "io.append",
    "io.scatter",
    "io.gather",
    "io.closed",
    "io.eof",
    "io.partial",
    "io.short",
    "io.blocked",
    "io.nonblocking",
    "io.timeout",
    "io.interrupt",
    "io.retry",
    "io.corrupt",
    "io.checksum",
    "io.buffer",
    "io.alignment",
    "io.direct",
    "io.stream",
    "io.pipe",
    
    /* ======================================================================= */
    /* MEMORY                                                                   */
    /* ======================================================================= */
    "memory.alloc",
    "memory.realloc",
    "memory.free",
    "memory.map",
    "memory.unmap",
    "memory.remap",
    "memory.lock",
    "memory.unlock",
    "memory.protect",
    "memory.unprotect",
    "memory.leak",
    "memory.overrun",
    "memory.underrun",
    "memory.use_after_free",
    "memory.double_free",
    "memory.fragmented",
    "memory.exhausted",
    "memory.alignment",
    "memory.page_fault",
    "memory.segmentation",
    "memory.guard",
    "memory.poison",
    "memory.swap",
    "memory.numa",
    
    /* ======================================================================= */
    /* CPU / EXECUTION                                                          */
    /* ======================================================================= */
    "cpu.illegal_instruction",
    "cpu.privilege_violation",
    "cpu.div_zero",
    "cpu.overflow",
    "cpu.underflow",
    "cpu.fpu",
    "cpu.simd",
    "cpu.cache",
    "cpu.pipeline",
    "cpu.affinity",
    "cpu.throttle",
    
    /* ======================================================================= */
    /* MATH / NUMERIC                                                           */
    /* ======================================================================= */
    "math.overflow",
    "math.underflow",
    "math.div_zero",
    "math.nan",
    "math.infinity",
    "math.domain",
    "math.range",
    "math.precision",
    "math.rounding",
    "math.convergence",
    "math.divergence",
    "math.iteration",
    "math.singularity",
    "math.condition",
    "math.approximation",
    
    /* ======================================================================= */
    /* PARSING / LEXING / GRAMMAR                                               */
    /* ======================================================================= */
    "parse.invalid",
    "parse.syntax",
    "parse.semantic",
    "parse.context",
    "parse.state",
    "parse.encoding",
    "parse.unexpected_token",
    "parse.missing_token",
    "parse.extra_token",
    "parse.ambiguous",
    "parse.incomplete",
    "parse.recursion",
    "parse.depth",
    "parse.stack",
    "parse.overflow",
    
    "lexer.invalid",
    "lexer.token",
    "lexer.state",
    "lexer.encoding",
    "lexer.buffer",
    "lexer.escape",
    
    /* ======================================================================= */
    /* TYPE SYSTEM                                                              */
    /* ======================================================================= */
    "type.invalid",
    "type.mismatch",
    "type.cast",
    "type.coercion",
    "type.size",
    "type.range",
    "type.signedness",
    "type.alignment",
    "type.qualifier",
    "type.generic",
    "type.polymorphic",
    "type.variance",
    
    /* ======================================================================= */
    /* FORMAT / ENCODING                                                        */
    /* ======================================================================= */
    "format.invalid",
    "format.unsupported",
    "format.truncated",
    "format.version",
    "format.magic",
    "format.header",
    "format.footer",
    "format.padding",
    "format.layout",
    
    "encoding.invalid",
    "encoding.unsupported",
    "encoding.incomplete",
    "encoding.locale",
    "encoding.endianness",
    "encoding.normalization",
    "encoding.compression",
    "encoding.decompression",
    
    /* ======================================================================= */
    /* DATA / CONTENT                                                           */
    /* ======================================================================= */
    "data.invalid",
    "data.corrupt",
    "data.missing",
    "data.extra",
    "data.duplicate",
    "data.inconsistent",
    "data.constraint",
    "data.integrity",
    "data.reference",
    "data.circular",
    "data.order",
    "data.range",
    "data.null",
    "data.schema",
    "data.version",
    "data.migration",
    
    /* ======================================================================= */
    /* FILESYSTEM / STORAGE                                                     */
    /* ======================================================================= */
    "fs.not_found",
    "fs.exists",
    "fs.permission",
    "fs.read_only",
    "fs.locked",
    "fs.busy",
    "fs.mount",
    "fs.unmount",
    "fs.remount",
    "fs.quota",
    "fs.corrupt",
    "fs.journal",
    "fs.snapshot",
    "fs.backup",
    "fs.restore",
    "fs.path",
    "fs.symlink",
    "fs.hardlink",
    "fs.inode",
    "fs.filesystem",
    
    /* ======================================================================= */
    /* PROCESS / SIGNAL                                                         */
    /* ======================================================================= */
    "process.spawn",
    "process.exec",
    "process.exit",
    "process.crash",
    "process.signal",
    "process.kill",
    "process.zombie",
    "process.orphan",
    "process.permission",
    "process.limit",
    "process.priority",
    "process.affinity",
    
    /* ======================================================================= */
    /* THREAD / CONCURRENCY                                                     */
    /* ======================================================================= */
    "thread.create",
    "thread.join",
    "thread.detach",
    "thread.cancel",
    "thread.signal",
    
    "concurrency.race",
    "concurrency.deadlock",
    "concurrency.livelock",
    "concurrency.starvation",
    "concurrency.atomicity",
    "concurrency.lock",
    "concurrency.unlock",
    "concurrency.condition",
    "concurrency.barrier",
    "concurrency.scheduler",
    "concurrency.preemption",
    
    /* ======================================================================= */
    /* RESOURCE                                                                 */
    /* ======================================================================= */
    "resource.exhausted",
    "resource.leak",
    "resource.locked",
    "resource.starvation",
    "resource.handle",
    "resource.descriptor",
    "resource.pool",
    "resource.cache",
    "resource.bandwidth",
    "resource.quota",
    "resource.limit",
    
    /* ======================================================================= */
    /* TIME / CLOCK                                                             */
    /* ======================================================================= */
    "time.timeout",
    "time.expired",
    "time.schedule",
    "time.clock",
    "time.drift",
    "time.skew",
    "time.monotonic",
    "time.realtime",
    "time.resolution",
    "time.wrap",
    
    /* ======================================================================= */
    /* CONFIG / ENVIRONMENT                                                     */
    /* ======================================================================= */
    "config.missing",
    "config.invalid",
    "config.conflict",
    "config.version",
    "config.env",
    "config.profile",
    "config.permission",
    "config.schema",
    "config.override",
    "config.default",
    "config.locked",
    
    /* ======================================================================= */
    /* API / ABI                                                                */
    /* ======================================================================= */
    "api.invalid_call",
    "api.contract",
    "api.precondition",
    "api.postcondition",
    "api.version",
    "api.mismatch",
    "api.deprecated",
    "api.timeout",
    "api.limit",
    "api.state",
    "api.sequence",
    "api.serialization",
    "api.deserialization",
    "abi.mismatch",
    "abi.incompatible",
    
    /* ======================================================================= */
    /* PROTOCOL / IPC                                                           */
    /* ======================================================================= */
    "protocol.invalid",
    "protocol.version",
    "protocol.handshake",
    "protocol.negotiation",
    "protocol.sequence",
    "protocol.frame",
    "protocol.fragment",
    "protocol.checksum",
    "protocol.timeout",
    "protocol.reset",
    "protocol.flow_control",
    
    /* ======================================================================= */
    /* NETWORK                                                                  */
    /* ======================================================================= */
    "network.unreachable",
    "network.timeout",
    "network.reset",
    "network.refused",
    "network.aborted",
    "network.dns",
    "network.routing",
    "network.latency",
    "network.bandwidth",
    "network.congestion",
    "network.proxy",
    "network.firewall",
    "network.nat",
    "network.session",
    "network.stream",
    "network.packet",
    
    /* ======================================================================= */
    /* SECURITY / CRYPTO                                                        */
    /* ======================================================================= */
    "security.violation",
    "security.auth_failed",
    "security.authz_failed",
    "security.identity",
    "security.credential",
    "security.token",
    "security.session",
    "security.encryption",
    "security.decryption",
    "security.certificate",
    "security.key",
    "security.keystore",
    "security.revocation",
    "security.sandbox",
    "security.trust",
    "security.integrity",
    "security.tamper",
    "security.replay",
    
    /* ======================================================================= */
    /* DATABASE / STORAGE ENGINE                                                */
    /* ======================================================================= */
    "database.connect",
    "database.disconnect",
    "database.query",
    "database.prepare",
    "database.execute",
    "database.transaction",
    "database.commit",
    "database.rollback",
    "database.deadlock",
    "database.lock",
    "database.constraint",
    "database.schema",
    "database.migration",
    "database.index",
    "database.cursor",
    "database.replication",
    "database.consistency",
    "database.timeout",
    
    /* ======================================================================= */
    /* AI / ML                                                                  */
    /* ======================================================================= */
    "ai.model",
    "ai.version",
    "ai.load",
    "ai.unload",
    "ai.inference",
    "ai.training",
    "ai.finetune",
    "ai.dataset",
    "ai.validation",
    "ai.bias",
    "ai.drift",
    "ai.hallucination",
    "ai.alignment",
    "ai.confidence",
    "ai.explainability",
    "ai.prompt",
    "ai.token_limit",
    "ai.context_overflow",
    
    /* ======================================================================= */
    /* UI / UX                                                                  */
    /* ======================================================================= */
    "ui.render",
    "ui.layout",
    "ui.paint",
    "ui.refresh",
    "ui.input",
    "ui.focus",
    "ui.gesture",
    "ui.accessibility",
    "ui.localization",
    "ui.theme",
    "ui.font",
    "ui.image",
    
    /* ======================================================================= */
    /* OBSERVABILITY / OPS                                                      */
    /* ======================================================================= */
    "log.write",
    "log.read",
    "log.format",
    "log.rotate",
    "log.truncate",
    "metrics.collect",
    "metrics.aggregate",
    "metrics.export",
    "trace.emit",
    "trace.flush",
    "trace.sample",
    "monitor.unavailable",
    "diagnostics.collect",
    "diagnostics.dump",
    "profile.sample",
    
    /* ======================================================================= */
    /* BUILD / DEPLOY                                                           */
    /* ======================================================================= */
    "build.configure",
    "build.compile",
    "build.link",
    "build.package",
    "deploy.install",
    "deploy.remove",
    "deploy.upgrade",
    "deploy.rollback",
    "deploy.migration",
    "deploy.orchestration",
    "deploy.container",
    "deploy.image",
    
    /* ======================================================================= */
    /* USER                                                                     */
    /* ======================================================================= */
    "user.input",
    "user.permission",
    "user.quota",
    "user.cancelled",
    "user.timeout",
    "user.conflict",
    "user.invalid_state",
    "user.rate_limit",
    
    /* ======================================================================= */
    /* LEGAL / POLICY                                                           */
    /* ======================================================================= */
    "policy.violation",
    "policy.denied",
    "policy.expired",
    "policy.restricted",
    "license.invalid",
    "license.expired",
    "license.restricted",
    "privacy.violation",
    "privacy.redaction",
    "compliance.failure",
    "audit.failure",
    
    /* ======================================================================= */
    /* FALLBACK / GUARANTEES                                                    */
    /* ======================================================================= */
    "meta.unreachable",
    "meta.assumption",
    "meta.placeholder",
    "meta.future",
    
    NULL
};

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

const char *fossil_io_what(const char *error_code) {
    // Table of error messages: 5 variants per error code, indexed by [error_index][variant]
    static const char *fossil_error_messages[][5] = {
        // SYSTEM / META
        {"Operation completed successfully.", "No error occurred.", "Everything is OK.", "Success.", "Operation succeeded."},
        {"Unknown system error.", "An unspecified error occurred.", "System error: unknown.", "Error: unknown cause.", "Unrecognized error."},
        {"Internal system error.", "A bug was detected.", "Unexpected internal failure.", "Internal logic error.", "System malfunction."},
        {"Fatal system error.", "Critical failure encountered.", "System halted due to fatal error.", "Unrecoverable fatal error.", "Fatal: operation stopped."},
        {"System panic.", "Panic: unrecoverable error.", "System entered panic state.", "Panic triggered.", "Critical panic error."},
        {"Operation aborted.", "Process was aborted.", "Aborted by system.", "Aborted: operation stopped.", "Aborted unexpectedly."},
        {"Assertion failed.", "System assertion triggered.", "Assertion error.", "Failed assertion detected.", "Assertion violation."},
        {"Invariant violation.", "System invariant broken.", "Invariant error.", "Invariant not maintained.", "Invariant check failed."},
        {"Contract violation.", "System contract broken.", "Contract error.", "Contract not fulfilled.", "Contract check failed."},
        {"Recoverable system error.", "Error can be recovered.", "Recoverable: try again.", "Temporary recoverable error.", "Recoverable failure."},
        {"Unrecoverable system error.", "Cannot recover from error.", "Unrecoverable: operation failed.", "Permanent failure.", "Unrecoverable condition."},
        {"Transient system error.", "Temporary error occurred.", "Transient: try again.", "Short-lived error.", "Transient failure."},
        {"Permanent system error.", "Permanent failure.", "Error will not resolve.", "Permanent condition.", "Irrecoverable error."},
        {"Unsupported operation.", "Operation not supported.", "Feature unsupported.", "Unsupported by system.", "Not supported."},
        {"Unimplemented feature.", "Feature not implemented.", "Unimplemented operation.", "Not yet implemented.", "Missing implementation."},
        {"Deprecated feature.", "Feature is deprecated.", "Deprecated operation.", "Use of deprecated feature.", "Deprecated: avoid use."},
        {"Experimental feature.", "Feature is experimental.", "Experimental: may be unstable.", "Unstable experimental feature.", "Experimental use only."},
        {"System misconfigured.", "Configuration error.", "Misconfiguration detected.", "System not configured properly.", "Configuration problem."},
        {"System corruption detected.", "Corrupt system state.", "Corruption error.", "System data corrupted.", "Corruption found."},
        {"System bootstrap error.", "Bootstrap failed.", "Error during bootstrap.", "Bootstrap process error.", "Bootstrap not completed."},
        {"System shutdown.", "Shutdown in progress.", "System is shutting down.", "Shutdown error.", "Shutdown initiated."},
        {"System restart.", "Restart in progress.", "System is restarting.", "Restart error.", "Restart initiated."},
        {"System upgrade error.", "Upgrade failed.", "Error during upgrade.", "Upgrade process error.", "Upgrade not completed."},
        {"System downgrade error.", "Downgrade failed.", "Error during downgrade.", "Downgrade process error.", "Downgrade not completed."},
        {"Permission denied.", "Insufficient permissions.", "Access denied.", "Permission error.", "No permission for operation."},
        {"Capability error.", "Insufficient capability.", "Capability not present.", "Missing required capability.", "Capability denied."},

        // IO
        {"Read error.", "Failed to read data.", "Unable to perform read operation.", "Read operation failed.", "Error reading input."},
        {"Write error.", "Failed to write data.", "Unable to perform write operation.", "Write operation failed.", "Error writing output."},
        {"Seek error.", "Failed to seek.", "Unable to move file pointer.", "Seek operation failed.", "Error seeking position."},
        {"Flush error.", "Failed to flush data.", "Unable to flush buffer.", "Flush operation failed.", "Error flushing output."},
        {"Sync error.", "Failed to synchronize.", "Unable to sync data.", "Sync operation failed.", "Error syncing data."},
        {"FSync error.", "Failed to fsync.", "Unable to flush to disk.", "FSync operation failed.", "Error syncing file."},
        {"Truncate error.", "Failed to truncate.", "Unable to truncate file.", "Truncate operation failed.", "Error truncating file."},
        {"Append error.", "Failed to append data.", "Unable to append.", "Append operation failed.", "Error appending data."},
        {"Scatter error.", "Failed to scatter write.", "Unable to scatter data.", "Scatter operation failed.", "Error in scatter operation."},
        {"Gather error.", "Failed to gather read.", "Unable to gather data.", "Gather operation failed.", "Error in gather operation."},
        {"File closed.", "File is already closed.", "Operation on closed file.", "Closed file error.", "File not open."},
        {"End of file.", "Reached end of file.", "EOF encountered.", "No more data to read.", "End of input."},
        {"Partial I/O.", "Partial read/write.", "Incomplete I/O operation.", "Partial operation performed.", "Partial data processed."},
        {"Short I/O.", "Short read/write.", "Less data processed than expected.", "Short operation.", "Short data transfer."},
        {"I/O blocked.", "Operation blocked.", "I/O is currently blocked.", "Blocked I/O operation.", "I/O blocking error."},
        {"Non-blocking I/O error.", "Non-blocking operation failed.", "Non-blocking mode error.", "Error in non-blocking I/O.", "Non-blocking I/O not possible."},
        {"I/O timeout.", "Operation timed out.", "Timeout during I/O.", "I/O operation timeout.", "Timeout error."},
        {"I/O interrupted.", "Operation interrupted.", "I/O was interrupted.", "Interrupted I/O operation.", "I/O interruption error."},
        {"I/O retry needed.", "Retry the operation.", "I/O needs to be retried.", "Retry required.", "I/O retry error."},
        {"I/O corruption detected.", "Corrupt I/O data.", "I/O data corrupted.", "Corruption in I/O.", "I/O corruption error."},
        {"Checksum error.", "Checksum mismatch.", "Invalid checksum.", "Checksum verification failed.", "Checksum error detected."},
        {"Buffer error.", "Buffer overflow/underflow.", "Buffer operation failed.", "Buffer error detected.", "Buffer issue."},
        {"Alignment error.", "Data not aligned.", "Alignment requirement not met.", "Alignment error detected.", "Misaligned data."},
        {"Direct I/O error.", "Direct I/O failed.", "Unable to perform direct I/O.", "Direct I/O operation error.", "Direct I/O not supported."},
        {"Stream error.", "Stream operation failed.", "Stream error detected.", "Error in stream.", "Stream failure."},
        {"Pipe error.", "Pipe operation failed.", "Pipe error detected.", "Error in pipe.", "Pipe failure."},

        // MEMORY
        {"Memory allocation failed.", "Unable to allocate memory.", "Allocation error.", "Out of memory.", "Memory alloc error."},
        {"Memory reallocation failed.", "Unable to reallocate memory.", "Reallocation error.", "Memory realloc error.", "Realloc failed."},
        {"Memory free error.", "Unable to free memory.", "Free operation failed.", "Memory free failed.", "Free error."},
        {"Memory map error.", "Unable to map memory.", "Memory mapping failed.", "Map operation failed.", "Memory map failed."},
        {"Memory unmap error.", "Unable to unmap memory.", "Unmap operation failed.", "Memory unmap failed.", "Unmap error."},
        {"Memory remap error.", "Unable to remap memory.", "Remap operation failed.", "Memory remap failed.", "Remap error."},
        {"Memory lock error.", "Unable to lock memory.", "Lock operation failed.", "Memory lock failed.", "Lock error."},
        {"Memory unlock error.", "Unable to unlock memory.", "Unlock operation failed.", "Memory unlock failed.", "Unlock error."},
        {"Memory protection error.", "Unable to protect memory.", "Protection operation failed.", "Memory protect failed.", "Protect error."},
        {"Memory unprotect error.", "Unable to unprotect memory.", "Unprotect operation failed.", "Memory unprotect failed.", "Unprotect error."},
        {"Memory leak detected.", "Leaked memory found.", "Memory leak error.", "Memory not freed.", "Leak error."},
        {"Memory overrun detected.", "Buffer overrun.", "Overrun error.", "Memory overrun error.", "Overrun detected."},
        {"Memory underrun detected.", "Buffer underrun.", "Underrun error.", "Memory underrun error.", "Underrun detected."},
        {"Use after free detected.", "Memory used after free.", "Use-after-free error.", "Dangling pointer used.", "Use after free error."},
        {"Double free detected.", "Memory freed twice.", "Double free error.", "Double free operation.", "Double free issue."},
        {"Memory fragmentation.", "Fragmented memory detected.", "Memory fragmentation error.", "Fragmentation issue.", "Fragmented memory error."},
        {"Memory exhausted.", "No memory available.", "Memory exhausted error.", "Out of memory.", "Memory exhaustion."},
        {"Memory alignment error.", "Alignment requirement not met.", "Misaligned memory.", "Memory alignment issue.", "Alignment error."},
        {"Page fault occurred.", "Memory page fault.", "Page fault error.", "Page not present.", "Page fault detected."},
        {"Segmentation fault.", "Invalid memory access.", "Segmentation violation.", "Segfault error.", "Segmentation error."},
        {"Memory guard error.", "Guard page triggered.", "Memory guard violation.", "Guard error.", "Guard page error."},
        {"Memory poison error.", "Poisoned memory accessed.", "Memory poison detected.", "Poison error.", "Poisoned memory error."},
        {"Memory swap error.", "Swap operation failed.", "Memory swap failed.", "Swap error.", "Swap operation error."},
        {"NUMA error.", "NUMA node error.", "NUMA operation failed.", "NUMA error detected.", "NUMA failure."},

        // CPU / EXECUTION
        {"Illegal instruction.", "Invalid CPU instruction.", "Illegal instruction error.", "CPU illegal instruction.", "Illegal instruction detected."},
        {"Privilege violation.", "Insufficient CPU privilege.", "Privilege error.", "CPU privilege violation.", "Privilege violation detected."},
        {"Division by zero.", "Divide by zero error.", "CPU division by zero.", "Division error.", "Zero division detected."},
        {"CPU overflow.", "Overflow error.", "CPU overflow detected.", "Overflow occurred.", "Overflow error."},
        {"CPU underflow.", "Underflow error.", "CPU underflow detected.", "Underflow occurred.", "Underflow error."},
        {"FPU error.", "Floating point unit error.", "FPU operation failed.", "FPU error detected.", "FPU failure."},
        {"SIMD error.", "SIMD operation failed.", "SIMD error detected.", "SIMD failure.", "SIMD instruction error."},
        {"CPU cache error.", "Cache operation failed.", "CPU cache failure.", "Cache error detected.", "Cache error."},
        {"CPU pipeline error.", "Pipeline operation failed.", "Pipeline error detected.", "Pipeline failure.", "Pipeline error."},
        {"CPU affinity error.", "Affinity operation failed.", "CPU affinity failure.", "Affinity error detected.", "Affinity error."},
        {"CPU throttle error.", "CPU throttling detected.", "Throttle error.", "CPU throttle failure.", "Throttling error."},

        // MATH / NUMERIC
        {"Math overflow.", "Numeric overflow error.", "Overflow in math operation.", "Overflow detected.", "Math operation overflow."},
        {"Math underflow.", "Numeric underflow error.", "Underflow in math operation.", "Underflow detected.", "Math operation underflow."},
        {"Division by zero.", "Divide by zero in math.", "Math division by zero.", "Division error.", "Zero division detected."},
        {"NaN encountered.", "Not a Number (NaN) error.", "NaN value detected.", "NaN error.", "NaN in math operation."},
        {"Infinity encountered.", "Infinite value error.", "Infinity in math operation.", "Infinity error.", "Infinite result detected."},
        {"Math domain error.", "Invalid input for math function.", "Domain error detected.", "Math domain violation.", "Domain error."},
        {"Math range error.", "Value out of range.", "Range error detected.", "Math range violation.", "Range error."},
        {"Precision error.", "Insufficient precision.", "Precision loss detected.", "Precision error.", "Precision issue."},
        {"Rounding error.", "Rounding issue detected.", "Rounding error occurred.", "Rounding error.", "Rounding problem."},
        {"Convergence error.", "Failed to converge.", "Convergence not achieved.", "Convergence error detected.", "No convergence."},
        {"Divergence error.", "Divergence detected.", "Failed to converge: divergence.", "Divergence error.", "Divergence occurred."},
        {"Iteration error.", "Iteration failed.", "Iteration error detected.", "Iteration did not complete.", "Iteration problem."},
        {"Singularity encountered.", "Singularity error.", "Singular point detected.", "Singularity in math.", "Singularity error."},
        {"Condition error.", "Condition number too high.", "Condition error detected.", "Ill-conditioned problem.", "Conditioning error."},
        {"Approximation error.", "Approximation not accurate.", "Approximation error detected.", "Approximation failed.", "Approximation issue."},

        // PARSING / LEXING / GRAMMAR
        {"Invalid parse.", "Parsing failed.", "Parse error detected.", "Invalid input for parser.", "Parsing error."},
        {"Syntax error.", "Syntax violation detected.", "Syntax error in input.", "Syntax error occurred.", "Syntax problem."},
        {"Semantic error.", "Semantic violation detected.", "Semantic error in input.", "Semantic error occurred.", "Semantic problem."},
        {"Parsing context error.", "Context error in parsing.", "Context violation detected.", "Parsing context failed.", "Context error."},
        {"Parsing state error.", "State error in parsing.", "Parsing state invalid.", "State violation detected.", "State error."},
        {"Encoding error in parsing.", "Encoding error detected.", "Invalid encoding in parse.", "Encoding violation.", "Encoding error."},
        {"Unexpected token.", "Unexpected token found.", "Unexpected token error.", "Token not expected.", "Unexpected token detected."},
        {"Missing token.", "Token missing in input.", "Missing token error.", "Token not found.", "Token missing error."},
        {"Extra token.", "Extra token in input.", "Extra token error.", "Unexpected extra token.", "Extra token detected."},
        {"Ambiguous parse.", "Ambiguity detected.", "Ambiguous parsing error.", "Ambiguous input.", "Ambiguity error."},
        {"Incomplete parse.", "Parsing incomplete.", "Incomplete input.", "Incomplete parsing error.", "Incomplete error."},
        {"Recursive parse error.", "Recursion limit reached.", "Recursive parsing error.", "Recursion error.", "Recursion detected."},
        {"Parse depth error.", "Parsing depth exceeded.", "Depth error in parsing.", "Parse depth limit reached.", "Depth error."},
        {"Parse stack error.", "Parsing stack overflow.", "Stack error in parsing.", "Stack overflow detected.", "Stack error."},
        {"Parse overflow error.", "Overflow in parsing.", "Parsing overflow detected.", "Overflow error.", "Overflow in parse."},

        {"Invalid lexer input.", "Lexer error detected.", "Invalid input for lexer.", "Lexer invalid error.", "Lexer error."},
        {"Lexer token error.", "Token error in lexer.", "Lexer token invalid.", "Lexer token problem.", "Lexer token issue."},
        {"Lexer state error.", "State error in lexer.", "Lexer state invalid.", "Lexer state problem.", "Lexer state issue."},
        {"Lexer encoding error.", "Encoding error in lexer.", "Lexer encoding invalid.", "Lexer encoding problem.", "Lexer encoding issue."},
        {"Lexer buffer error.", "Buffer error in lexer.", "Lexer buffer invalid.", "Lexer buffer problem.", "Lexer buffer issue."},
        {"Lexer escape error.", "Escape sequence error.", "Lexer escape invalid.", "Lexer escape problem.", "Lexer escape issue."},

        // TYPE SYSTEM
        {"Invalid type.", "Type error detected.", "Invalid type encountered.", "Type invalid error.", "Type error."},
        {"Type mismatch.", "Mismatched types.", "Type mismatch error.", "Type mismatch detected.", "Type mismatch issue."},
        {"Type cast error.", "Invalid type cast.", "Type casting error.", "Type cast failed.", "Type cast issue."},
        {"Type coercion error.", "Invalid type coercion.", "Type coercion failed.", "Type coercion error.", "Type coercion issue."},
        {"Type size error.", "Invalid type size.", "Type size mismatch.", "Type size error detected.", "Type size issue."},
        {"Type range error.", "Type value out of range.", "Type range error detected.", "Type range violation.", "Type range issue."},
        {"Type signedness error.", "Signedness mismatch.", "Type signedness error detected.", "Signedness error.", "Signedness issue."},
        {"Type alignment error.", "Type alignment mismatch.", "Type alignment error detected.", "Alignment error.", "Alignment issue."},
        {"Type qualifier error.", "Invalid type qualifier.", "Type qualifier error detected.", "Qualifier error.", "Qualifier issue."},
        {"Generic type error.", "Generic type error detected.", "Generic type invalid.", "Generic type problem.", "Generic type issue."},
        {"Polymorphic type error.", "Polymorphic type error detected.", "Polymorphic type invalid.", "Polymorphic type problem.", "Polymorphic type issue."},
        {"Type variance error.", "Variance error detected.", "Type variance invalid.", "Variance problem.", "Variance issue."},

        // FORMAT / ENCODING
        {"Invalid format.", "Format error detected.", "Invalid data format.", "Format invalid error.", "Format error."},
        {"Unsupported format.", "Format not supported.", "Unsupported data format.", "Format unsupported error.", "Format not supported error."},
        {"Truncated format.", "Format truncated.", "Truncated data format.", "Format truncation error.", "Truncation error."},
        {"Format version error.", "Invalid format version.", "Format version mismatch.", "Format version error detected.", "Version error."},
        {"Format magic error.", "Invalid format magic.", "Format magic mismatch.", "Format magic error detected.", "Magic error."},
        {"Format header error.", "Invalid format header.", "Format header mismatch.", "Format header error detected.", "Header error."},
        {"Format footer error.", "Invalid format footer.", "Format footer mismatch.", "Format footer error detected.", "Footer error."},
        {"Format padding error.", "Invalid format padding.", "Format padding mismatch.", "Format padding error detected.", "Padding error."},
        {"Format layout error.", "Invalid format layout.", "Format layout mismatch.", "Format layout error detected.", "Layout error."},

        {"Invalid encoding.", "Encoding error detected.", "Invalid data encoding.", "Encoding invalid error.", "Encoding error."},
        {"Unsupported encoding.", "Encoding not supported.", "Unsupported data encoding.", "Encoding unsupported error.", "Encoding not supported error."},
        {"Incomplete encoding.", "Encoding incomplete.", "Incomplete data encoding.", "Encoding incomplete error.", "Incomplete encoding error."},
        {"Encoding locale error.", "Invalid encoding locale.", "Encoding locale mismatch.", "Encoding locale error detected.", "Locale error."},
        {"Encoding endianness error.", "Invalid encoding endianness.", "Encoding endianness mismatch.", "Encoding endianness error detected.", "Endianness error."},
        {"Encoding normalization error.", "Invalid encoding normalization.", "Encoding normalization mismatch.", "Encoding normalization error detected.", "Normalization error."},
        {"Encoding compression error.", "Compression error in encoding.", "Encoding compression failed.", "Compression error detected.", "Compression error."},
        {"Encoding decompression error.", "Decompression error in encoding.", "Encoding decompression failed.", "Decompression error detected.", "Decompression error."},

        // DATA / CONTENT
        {"Invalid data.", "Data error detected.", "Invalid content.", "Data invalid error.", "Data error."},
        {"Corrupt data.", "Data corruption detected.", "Corrupt content.", "Data corrupt error.", "Corruption error."},
        {"Missing data.", "Data missing.", "Missing content.", "Data missing error.", "Missing error."},
        {"Extra data.", "Extra data detected.", "Extra content.", "Data extra error.", "Extra error."},
        {"Duplicate data.", "Duplicate data detected.", "Duplicate content.", "Data duplicate error.", "Duplicate error."},
        {"Inconsistent data.", "Data inconsistency detected.", "Inconsistent content.", "Data inconsistent error.", "Inconsistency error."},
        {"Data constraint error.", "Constraint violation detected.", "Data constraint error.", "Constraint error.", "Constraint issue."},
        {"Data integrity error.", "Integrity violation detected.", "Data integrity error.", "Integrity error.", "Integrity issue."},
        {"Data reference error.", "Reference error detected.", "Data reference error.", "Reference error.", "Reference issue."},
        {"Circular data reference.", "Circular reference detected.", "Circular data error.", "Circular reference error.", "Circular error."},
        {"Data order error.", "Order error detected.", "Data order error.", "Order error.", "Order issue."},
        {"Data range error.", "Range error detected.", "Data range error.", "Range error.", "Range issue."},
        {"Null data error.", "Null data detected.", "Null content.", "Null data error.", "Null error."},
        {"Data schema error.", "Schema error detected.", "Data schema error.", "Schema error.", "Schema issue."},
        {"Data version error.", "Version error detected.", "Data version error.", "Version error.", "Version issue."},
        {"Data migration error.", "Migration error detected.", "Data migration error.", "Migration error.", "Migration issue."},

        // FILESYSTEM / STORAGE
        {"File not found.", "File does not exist.", "File missing.", "Not found error.", "File not found error."},
        {"File already exists.", "File exists error.", "File exists.", "Exists error.", "File already exists error."},
        {"Filesystem permission denied.", "Permission error.", "Filesystem access denied.", "Permission denied error.", "Filesystem permission error."},
        {"Filesystem is read-only.", "Read-only error.", "Filesystem read-only.", "Read-only filesystem error.", "Read-only error."},
        {"Filesystem locked.", "Locked error.", "Filesystem is locked.", "Locked filesystem error.", "Locked error."},
        {"Filesystem busy.", "Busy error.", "Filesystem is busy.", "Busy filesystem error.", "Busy error."},
        {"Mount error.", "Mount operation failed.", "Mount failed.", "Mount error detected.", "Mount failure."},
        {"Unmount error.", "Unmount operation failed.", "Unmount failed.", "Unmount error detected.", "Unmount failure."},
        {"Remount error.", "Remount operation failed.", "Remount failed.", "Remount error detected.", "Remount failure."},
        {"Quota error.", "Quota exceeded.", "Quota error detected.", "Quota exceeded error.", "Quota issue."},
        {"Filesystem corruption detected.", "Corrupt filesystem.", "Filesystem corrupt error.", "Corruption error.", "Filesystem corruption."},
        {"Journal error.", "Journal operation failed.", "Journal error detected.", "Journal failure.", "Journal error."},
        {"Snapshot error.", "Snapshot operation failed.", "Snapshot error detected.", "Snapshot failure.", "Snapshot error."},
        {"Backup error.", "Backup operation failed.", "Backup error detected.", "Backup failure.", "Backup error."},
        {"Restore error.", "Restore operation failed.", "Restore error detected.", "Restore failure.", "Restore error."},
        {"Path error.", "Invalid path.", "Path error detected.", "Path failure.", "Path error."},
        {"Symlink error.", "Symlink operation failed.", "Symlink error detected.", "Symlink failure.", "Symlink error."},
        {"Hardlink error.", "Hardlink operation failed.", "Hardlink error detected.", "Hardlink failure.", "Hardlink error."},
        {"Inode error.", "Inode operation failed.", "Inode error detected.", "Inode failure.", "Inode error."},
        {"Filesystem error.", "Filesystem operation failed.", "Filesystem error detected.", "Filesystem failure.", "Filesystem error."},

        // PROCESS / SIGNAL
        {"Process spawn error.", "Failed to spawn process.", "Process spawn failed.", "Spawn error detected.", "Spawn error."},
        {"Process exec error.", "Failed to exec process.", "Process exec failed.", "Exec error detected.", "Exec error."},
        {"Process exit error.", "Process exited with error.", "Exit error detected.", "Process exit failed.", "Exit error."},
        {"Process crash.", "Process crashed.", "Crash error detected.", "Process crash error.", "Crash error."},
        {"Process signal error.", "Signal error detected.", "Process signal failed.", "Signal error.", "Signal issue."},
        {"Process kill error.", "Failed to kill process.", "Kill error detected.", "Process kill failed.", "Kill error."},
        {"Zombie process detected.", "Zombie process error.", "Zombie process found.", "Zombie error.", "Zombie issue."},
        {"Orphan process detected.", "Orphan process error.", "Orphan process found.", "Orphan error.", "Orphan issue."},
        {"Process permission denied.", "Permission error.", "Process access denied.", "Permission denied error.", "Process permission error."},
        {"Process limit error.", "Process limit exceeded.", "Limit error detected.", "Process limit error.", "Limit issue."},
        {"Process priority error.", "Priority error detected.", "Process priority error.", "Priority error.", "Priority issue."},
        {"Process affinity error.", "Affinity error detected.", "Process affinity error.", "Affinity error.", "Affinity issue."},

        // THREAD / CONCURRENCY
        {"Thread creation error.", "Failed to create thread.", "Thread creation failed.", "Thread create error.", "Thread error."},
        {"Thread join error.", "Failed to join thread.", "Thread join failed.", "Thread join error.", "Join error."},
        {"Thread detach error.", "Failed to detach thread.", "Thread detach failed.", "Thread detach error.", "Detach error."},
        {"Thread cancel error.", "Failed to cancel thread.", "Thread cancel failed.", "Thread cancel error.", "Cancel error."},
        {"Thread signal error.", "Thread signal failed.", "Signal error in thread.", "Thread signal error.", "Thread signal issue."},

        {"Race condition detected.", "Race condition error.", "Race detected.", "Race error.", "Race issue."},
        {"Deadlock detected.", "Deadlock error.", "Deadlock occurred.", "Deadlock error detected.", "Deadlock issue."},
        {"Livelock detected.", "Livelock error.", "Livelock occurred.", "Livelock error detected.", "Livelock issue."},
        {"Starvation detected.", "Starvation error.", "Starvation occurred.", "Starvation error detected.", "Starvation issue."},
        {"Atomicity error.", "Atomicity violation detected.", "Atomicity error occurred.", "Atomicity error.", "Atomicity issue."},
        {"Lock error.", "Lock operation failed.", "Lock error detected.", "Lock failure.", "Lock issue."},
        {"Unlock error.", "Unlock operation failed.", "Unlock error detected.", "Unlock failure.", "Unlock issue."},
        {"Condition variable error.", "Condition error detected.", "Condition variable failed.", "Condition error.", "Condition issue."},
        {"Barrier error.", "Barrier operation failed.", "Barrier error detected.", "Barrier failure.", "Barrier issue."},
        {"Scheduler error.", "Scheduler operation failed.", "Scheduler error detected.", "Scheduler failure.", "Scheduler issue."},
        {"Preemption error.", "Preemption operation failed.", "Preemption error detected.", "Preemption failure.", "Preemption issue."},

        // RESOURCE
        {"Resource exhausted.", "No resources available.", "Resource exhaustion error.", "Resource exhausted error.", "Exhaustion error."},
        {"Resource leak detected.", "Resource leak error.", "Resource leak found.", "Leak error.", "Resource leak issue."},
        {"Resource locked.", "Resource lock error.", "Resource is locked.", "Locked resource error.", "Locked error."},
        {"Resource starvation.", "Resource starvation error.", "Starvation detected.", "Resource starvation detected.", "Starvation error."},
        {"Resource handle error.", "Invalid resource handle.", "Handle error detected.", "Resource handle error.", "Handle issue."},
        {"Resource descriptor error.", "Invalid resource descriptor.", "Descriptor error detected.", "Resource descriptor error.", "Descriptor issue."},
        {"Resource pool error.", "Resource pool operation failed.", "Pool error detected.", "Resource pool error.", "Pool issue."},
        {"Resource cache error.", "Resource cache operation failed.", "Cache error detected.", "Resource cache error.", "Cache issue."},
        {"Resource bandwidth error.", "Bandwidth error detected.", "Resource bandwidth error.", "Bandwidth error.", "Bandwidth issue."},
        {"Resource quota error.", "Quota error detected.", "Resource quota error.", "Quota error.", "Quota issue."},
        {"Resource limit error.", "Limit error detected.", "Resource limit error.", "Limit error.", "Limit issue."},

        // TIME / CLOCK
        {"Timeout occurred.", "Operation timed out.", "Timeout error detected.", "Timeout error.", "Timeout issue."},
        {"Time expired.", "Time expired error.", "Expired error detected.", "Time expired.", "Expired issue."},
        {"Schedule error.", "Scheduling failed.", "Schedule error detected.", "Schedule failure.", "Schedule issue."},
        {"Clock error.", "Clock operation failed.", "Clock error detected.", "Clock failure.", "Clock issue."},
        {"Time drift detected.", "Time drift error.", "Drift error detected.", "Time drift.", "Drift issue."},
        {"Time skew detected.", "Time skew error.", "Skew error detected.", "Time skew.", "Skew issue."},
        {"Monotonic time error.", "Monotonic clock error.", "Monotonic error detected.", "Monotonic error.", "Monotonic issue."},
        {"Realtime clock error.", "Realtime error detected.", "Realtime clock error.", "Realtime error.", "Realtime issue."},
        {"Time resolution error.", "Resolution error detected.", "Time resolution error.", "Resolution error.", "Resolution issue."},
        {"Time wrap error.", "Time wrap detected.", "Wrap error detected.", "Time wrap error.", "Wrap issue."},

        // CONFIG / ENVIRONMENT
        {"Missing configuration.", "Configuration missing.", "Config missing error.", "Missing config error.", "Missing configuration error."},
        {"Invalid configuration.", "Configuration error detected.", "Config invalid error.", "Invalid config error.", "Invalid configuration error."},
        {"Configuration conflict.", "Config conflict detected.", "Conflict error.", "Configuration conflict error.", "Conflict issue."},
        {"Configuration version error.", "Config version error detected.", "Version error.", "Configuration version error.", "Version issue."},
        {"Environment error.", "Environment variable error.", "Environment error detected.", "Env error.", "Environment issue."},
        {"Profile error.", "Profile error detected.", "Profile error.", "Profile issue.", "Profile error."},
        {"Configuration permission denied.", "Permission error.", "Config access denied.", "Permission denied error.", "Config permission error."},
        {"Configuration schema error.", "Schema error detected.", "Config schema error.", "Schema error.", "Schema issue."},
        {"Configuration override error.", "Override error detected.", "Config override error.", "Override error.", "Override issue."},
        {"Default configuration error.", "Default error detected.", "Default config error.", "Default error.", "Default issue."},
        {"Configuration locked.", "Config locked error.", "Locked configuration.", "Locked config error.", "Locked error."},

        // API / ABI
        {"Invalid API call.", "API call error detected.", "Invalid call to API.", "API invalid call error.", "API call error."},
        {"API contract error.", "Contract error detected.", "API contract violation.", "Contract error.", "Contract issue."},
        {"API precondition failed.", "Precondition error detected.", "API precondition error.", "Precondition error.", "Precondition issue."},
        {"API postcondition failed.", "Postcondition error detected.", "API postcondition error.", "Postcondition error.", "Postcondition issue."},
        {"API version error.", "Version error detected.", "API version error.", "Version error.", "Version issue."},
        {"API mismatch error.", "Mismatch error detected.", "API mismatch error.", "Mismatch error.", "Mismatch issue."},
        {"API deprecated error.", "Deprecated API used.", "API deprecated error.", "Deprecated error.", "Deprecated issue."},
        {"API timeout error.", "API timeout detected.", "Timeout error.", "API timeout error.", "Timeout issue."},
        {"API limit error.", "Limit error detected.", "API limit error.", "Limit error.", "Limit issue."},
        {"API state error.", "State error detected.", "API state error.", "State error.", "State issue."},
        {"API sequence error.", "Sequence error detected.", "API sequence error.", "Sequence error.", "Sequence issue."},
        {"API serialization error.", "Serialization error detected.", "API serialization error.", "Serialization error.", "Serialization issue."},
        {"API deserialization error.", "Deserialization error detected.", "API deserialization error.", "Deserialization error.", "Deserialization issue."},
        {"ABI mismatch error.", "ABI mismatch detected.", "ABI mismatch error.", "Mismatch error.", "Mismatch issue."},
        {"ABI incompatible error.", "ABI incompatible detected.", "ABI incompatible error.", "Incompatible error.", "Incompatible issue."},

        // PROTOCOL / IPC
        {"Invalid protocol.", "Protocol error detected.", "Invalid protocol error.", "Protocol invalid error.", "Protocol error."},
        {"Protocol version error.", "Version error detected.", "Protocol version error.", "Version error.", "Version issue."},
        {"Protocol handshake error.", "Handshake error detected.", "Protocol handshake error.", "Handshake error.", "Handshake issue."},
        {"Protocol negotiation error.", "Negotiation error detected.", "Protocol negotiation error.", "Negotiation error.", "Negotiation issue."},
        {"Protocol sequence error.", "Sequence error detected.", "Protocol sequence error.", "Sequence error.", "Sequence issue."},
        {"Protocol frame error.", "Frame error detected.", "Protocol frame error.", "Frame error.", "Frame issue."},
        {"Protocol fragment error.", "Fragment error detected.", "Protocol fragment error.", "Fragment error.", "Fragment issue."},
        {"Protocol checksum error.", "Checksum error detected.", "Protocol checksum error.", "Checksum error.", "Checksum issue."},
        {"Protocol timeout error.", "Timeout error detected.", "Protocol timeout error.", "Timeout error.", "Timeout issue."},
        {"Protocol reset error.", "Reset error detected.", "Protocol reset error.", "Reset error.", "Reset issue."},
        {"Protocol flow control error.", "Flow control error detected.", "Protocol flow control error.", "Flow control error.", "Flow control issue."},

        // NETWORK
        {"Network unreachable.", "Network not reachable.", "Unreachable network error.", "Network unreachable error.", "Unreachable error."},
        {"Network timeout.", "Network operation timed out.", "Network timeout error.", "Timeout error.", "Timeout issue."},
        {"Network reset.", "Network reset error.", "Reset error detected.", "Network reset detected.", "Reset issue."},
        {"Network connection refused.", "Connection refused error.", "Network refused error.", "Refused error.", "Refused issue."},
        {"Network aborted.", "Network operation aborted.", "Aborted error detected.", "Network aborted error.", "Aborted issue."},
        {"DNS error.", "DNS resolution failed.", "DNS error detected.", "DNS failure.", "DNS issue."},
        {"Network routing error.", "Routing error detected.", "Network routing error.", "Routing error.", "Routing issue."},
        {"Network latency error.", "Latency error detected.", "Network latency error.", "Latency error.", "Latency issue."},
        {"Network bandwidth error.", "Bandwidth error detected.", "Network bandwidth error.", "Bandwidth error.", "Bandwidth issue."},
        {"Network congestion error.", "Congestion error detected.", "Network congestion error.", "Congestion error.", "Congestion issue."},
        {"Network proxy error.", "Proxy error detected.", "Network proxy error.", "Proxy error.", "Proxy issue."},
        {"Network firewall error.", "Firewall error detected.", "Network firewall error.", "Firewall error.", "Firewall issue."},
        {"Network NAT error.", "NAT error detected.", "Network NAT error.", "NAT error.", "NAT issue."},
        {"Network session error.", "Session error detected.", "Network session error.", "Session error.", "Session issue."},
        {"Network stream error.", "Stream error detected.", "Network stream error.", "Stream error.", "Stream issue."},
        {"Network packet error.", "Packet error detected.", "Network packet error.", "Packet error.", "Packet issue."},

        // SECURITY / CRYPTO
        {"Security violation.", "Security error detected.", "Security violation error.", "Violation error.", "Security issue."},
        {"Authentication failed.", "Authentication error detected.", "Auth failed error.", "Authentication error.", "Auth issue."},
        {"Authorization failed.", "Authorization error detected.", "Authz failed error.", "Authorization error.", "Authz issue."},
        {"Identity error.", "Identity error detected.", "Identity error.", "Identity issue.", "Identity error."},
        {"Credential error.", "Credential error detected.", "Credential error.", "Credential issue.", "Credential error."},
        {"Token error.", "Token error detected.", "Token error.", "Token issue.", "Token error."},
        {"Session error.", "Session error detected.", "Session error.", "Session issue.", "Session error."},
        {"Encryption error.", "Encryption error detected.", "Encryption error.", "Encryption issue.", "Encryption error."},
        {"Decryption error.", "Decryption error detected.", "Decryption error.", "Decryption issue.", "Decryption error."},
        {"Certificate error.", "Certificate error detected.", "Certificate error.", "Certificate issue.", "Certificate error."},
        {"Key error.", "Key error detected.", "Key error.", "Key issue.", "Key error."},
        {"Keystore error.", "Keystore error detected.", "Keystore error.", "Keystore issue.", "Keystore error."},
        {"Revocation error.", "Revocation error detected.", "Revocation error.", "Revocation issue.", "Revocation error."},
        {"Sandbox error.", "Sandbox error detected.", "Sandbox error.", "Sandbox issue.", "Sandbox error."},
        {"Trust error.", "Trust error detected.", "Trust error.", "Trust issue.", "Trust error."},
        {"Integrity error.", "Integrity error detected.", "Integrity error.", "Integrity issue.", "Integrity error."},
        {"Tamper detected.", "Tamper error detected.", "Tamper error.", "Tamper issue.", "Tamper error."},
        {"Replay attack detected.", "Replay error detected.", "Replay error.", "Replay issue.", "Replay error."},

        // DATABASE / STORAGE ENGINE
        {"Database connection error.", "Failed to connect to database.", "Database connect error.", "Connection error.", "Database error."},
        {"Database disconnect error.", "Failed to disconnect from database.", "Database disconnect error.", "Disconnect error.", "Database error."},
        {"Database query error.", "Query error detected.", "Database query failed.", "Query error.", "Query issue."},
        {"Database prepare error.", "Prepare error detected.", "Database prepare failed.", "Prepare error.", "Prepare issue."},
        {"Database execute error.", "Execute error detected.", "Database execute failed.", "Execute error.", "Execute issue."},
        {"Database transaction error.", "Transaction error detected.", "Database transaction failed.", "Transaction error.", "Transaction issue."},
        {"Database commit error.", "Commit error detected.", "Database commit failed.", "Commit error.", "Commit issue."},
        {"Database rollback error.", "Rollback error detected.", "Database rollback failed.", "Rollback error.", "Rollback issue."},
        {"Database deadlock detected.", "Deadlock error detected.", "Database deadlock error.", "Deadlock error.", "Deadlock issue."},
        {"Database lock error.", "Lock error detected.", "Database lock error.", "Lock error.", "Lock issue."},
        {"Database constraint error.", "Constraint error detected.", "Database constraint error.", "Constraint error.", "Constraint issue."},
        {"Database schema error.", "Schema error detected.", "Database schema error.", "Schema error.", "Schema issue."},
        {"Database migration error.", "Migration error detected.", "Database migration error.", "Migration error.", "Migration issue."},
        {"Database index error.", "Index error detected.", "Database index error.", "Index error.", "Index issue."},
        {"Database cursor error.", "Cursor error detected.", "Database cursor error.", "Cursor error.", "Cursor issue."},
        {"Database replication error.", "Replication error detected.", "Database replication error.", "Replication error.", "Replication issue."},
        {"Database consistency error.", "Consistency error detected.", "Database consistency error.", "Consistency error.", "Consistency issue."},
        {"Database timeout error.", "Timeout error detected.", "Database timeout error.", "Timeout error.", "Timeout issue."},

        // AI / ML
        {"AI model error.", "Model error detected.", "AI model failure.", "Model error.", "Model issue."},
        {"AI version error.", "Version error detected.", "AI version error.", "Version error.", "Version issue."},
        {"AI load error.", "Load error detected.", "AI load error.", "Load error.", "Load issue."},
        {"AI unload error.", "Unload error detected.", "AI unload error.", "Unload error.", "Unload issue."},
        {"AI inference error.", "Inference error detected.", "AI inference error.", "Inference error.", "Inference issue."},
        {"AI training error.", "Training error detected.", "AI training error.", "Training error.", "Training issue."},
        {"AI finetune error.", "Finetune error detected.", "AI finetune error.", "Finetune error.", "Finetune issue."},
        {"AI dataset error.", "Dataset error detected.", "AI dataset error.", "Dataset error.", "Dataset issue."},
        {"AI validation error.", "Validation error detected.", "AI validation error.", "Validation error.", "Validation issue."},
        {"AI bias error.", "Bias error detected.", "AI bias error.", "Bias error.", "Bias issue."},
        {"AI drift error.", "Drift error detected.", "AI drift error.", "Drift error.", "Drift issue."},
        {"AI hallucination error.", "Hallucination error detected.", "AI hallucination error.", "Hallucination error.", "Hallucination issue."},
        {"AI alignment error.", "Alignment error detected.", "AI alignment error.", "Alignment error.", "Alignment issue."},
        {"AI confidence error.", "Confidence error detected.", "AI confidence error.", "Confidence error.", "Confidence issue."},
        {"AI explainability error.", "Explainability error detected.", "AI explainability error.", "Explainability error.", "Explainability issue."},
        {"AI prompt error.", "Prompt error detected.", "AI prompt error.", "Prompt error.", "Prompt issue."},
        {"AI token limit error.", "Token limit error detected.", "AI token limit error.", "Token limit error.", "Token limit issue."},
        {"AI context overflow error.", "Context overflow error detected.", "AI context overflow error.", "Context overflow error.", "Context overflow issue."},

        // UI / UX
        {"UI render error.", "Render error detected.", "UI render failed.", "Render error.", "Render issue."},
        {"UI layout error.", "Layout error detected.", "UI layout failed.", "Layout error.", "Layout issue."},
        {"UI paint error.", "Paint error detected.", "UI paint failed.", "Paint error.", "Paint issue."},
        {"UI refresh error.", "Refresh error detected.", "UI refresh failed.", "Refresh error.", "Refresh issue."},
        {"UI input error.", "Input error detected.", "UI input failed.", "Input error.", "Input issue."},
        {"UI focus error.", "Focus error detected.", "UI focus failed.", "Focus error.", "Focus issue."},
        {"UI gesture error.", "Gesture error detected.", "UI gesture failed.", "Gesture error.", "Gesture issue."},
        {"UI accessibility error.", "Accessibility error detected.", "UI accessibility failed.", "Accessibility error.", "Accessibility issue."},
        {"UI localization error.", "Localization error detected.", "UI localization failed.", "Localization error.", "Localization issue."},
        {"UI theme error.", "Theme error detected.", "UI theme failed.", "Theme error.", "Theme issue."},
        {"UI font error.", "Font error detected.", "UI font failed.", "Font error.", "Font issue."},
        {"UI image error.", "Image error detected.", "UI image failed.", "Image error.", "Image issue."},

        // OBSERVABILITY / OPS
        {"Log write error.", "Write error detected in log.", "Log write failed.", "Write error.", "Log write issue."},
        {"Log read error.", "Read error detected in log.", "Log read failed.", "Read error.", "Log read issue."},
        {"Log format error.", "Format error detected in log.", "Log format failed.", "Format error.", "Log format issue."},
        {"Log rotate error.", "Rotate error detected in log.", "Log rotate failed.", "Rotate error.", "Log rotate issue."},
        {"Log truncate error.", "Truncate error detected in log.", "Log truncate failed.", "Truncate error.", "Log truncate issue."},
        {"Metrics collect error.", "Metrics collection failed.", "Metrics collect error detected.", "Collect error.", "Metrics collect issue."},
        {"Metrics aggregate error.", "Metrics aggregation failed.", "Metrics aggregate error detected.", "Aggregate error.", "Metrics aggregate issue."},
        {"Metrics export error.", "Metrics export failed.", "Metrics export error detected.", "Export error.", "Metrics export issue."},
        {"Trace emit error.", "Trace emit failed.", "Trace emit error detected.", "Emit error.", "Trace emit issue."},
        {"Trace flush error.", "Trace flush failed.", "Trace flush error detected.", "Flush error.", "Trace flush issue."},
        {"Trace sample error.", "Trace sample failed.", "Trace sample error detected.", "Sample error.", "Trace sample issue."},
        {"Monitor unavailable.", "Monitor unavailable error.", "Monitor not available.", "Unavailable error.", "Monitor unavailable issue."},
        {"Diagnostics collect error.", "Diagnostics collection failed.", "Diagnostics collect error detected.", "Collect error.", "Diagnostics collect issue."},
        {"Diagnostics dump error.", "Diagnostics dump failed.", "Diagnostics dump error detected.", "Dump error.", "Diagnostics dump issue."},
        {"Profile sample error.", "Profile sample failed.", "Profile sample error detected.", "Sample error.", "Profile sample issue."},

        // BUILD / DEPLOY
        {"Build configure error.", "Configure error detected.", "Build configure failed.", "Configure error.", "Configure issue."},
        {"Build compile error.", "Compile error detected.", "Build compile failed.", "Compile error.", "Compile issue."},
        {"Build link error.", "Link error detected.", "Build link failed.", "Link error.", "Link issue."},
        {"Build package error.", "Package error detected.", "Build package failed.", "Package error.", "Package issue."},
        {"Deploy install error.", "Install error detected.", "Deploy install failed.", "Install error.", "Install issue."},
        {"Deploy remove error.", "Remove error detected.", "Deploy remove failed.", "Remove error.", "Remove issue."},
        {"Deploy upgrade error.", "Upgrade error detected.", "Deploy upgrade failed.", "Upgrade error.", "Upgrade issue."},
        {"Deploy rollback error.", "Rollback error detected.", "Deploy rollback failed.", "Rollback error.", "Rollback issue."},
        {"Deploy migration error.", "Migration error detected.", "Deploy migration failed.", "Migration error.", "Migration issue."},
        {"Deploy orchestration error.", "Orchestration error detected.", "Deploy orchestration failed.", "Orchestration error.", "Orchestration issue."},
        {"Deploy container error.", "Container error detected.", "Deploy container failed.", "Container error.", "Container issue."},
        {"Deploy image error.", "Image error detected.", "Deploy image failed.", "Image error.", "Image issue."},

        // USER
        {"User input error.", "Input error detected.", "User input failed.", "Input error.", "Input issue."},
        {"User permission denied.", "Permission error detected.", "User permission denied.", "Permission denied error.", "Permission issue."},
        {"User quota error.", "Quota error detected.", "User quota exceeded.", "Quota error.", "Quota issue."},
        {"User cancelled operation.", "Operation cancelled by user.", "User cancelled error.", "Cancelled error.", "Cancelled issue."},
        {"User timeout error.", "Timeout error detected.", "User timeout occurred.", "Timeout error.", "Timeout issue."},
        {"User conflict error.", "Conflict error detected.", "User conflict occurred.", "Conflict error.", "Conflict issue."},
        {"User invalid state error.", "Invalid state detected.", "User invalid state.", "Invalid state error.", "Invalid state issue."},
        {"User rate limit error.", "Rate limit error detected.", "User rate limit exceeded.", "Rate limit error.", "Rate limit issue."},

        // LEGAL / POLICY
        {"Policy violation.", "Policy error detected.", "Policy violation error.", "Violation error.", "Policy issue."},
        {"Policy denied.", "Policy denied error.", "Denied error detected.", "Policy denied.", "Denied issue."},
        {"Policy expired.", "Policy expired error.", "Expired error detected.", "Policy expired.", "Expired issue."},
        {"Policy restricted.", "Policy restricted error.", "Restricted error detected.", "Policy restricted.", "Restricted issue."},
        {"License invalid.", "Invalid license detected.", "License invalid error.", "Invalid license error.", "License issue."},
        {"License expired.", "Expired license detected.", "License expired error.", "Expired license error.", "License issue."},
        {"License restricted.", "Restricted license detected.", "License restricted error.", "Restricted license error.", "License issue."},
        {"Privacy violation.", "Privacy error detected.", "Privacy violation error.", "Violation error.", "Privacy issue."},
        {"Privacy redaction error.", "Redaction error detected.", "Privacy redaction error.", "Redaction error.", "Redaction issue."},
        {"Compliance failure.", "Compliance error detected.", "Compliance failure error.", "Failure error.", "Compliance issue."},
        {"Audit failure.", "Audit error detected.", "Audit failure error.", "Failure error.", "Audit issue."},

        // FALLBACK / GUARANTEES
        {"Meta unreachable.", "Unreachable error detected.", "Meta unreachable error.", "Unreachable error.", "Unreachable issue."},
        {"Meta assumption error.", "Assumption error detected.", "Meta assumption error.", "Assumption error.", "Assumption issue."},
        {"Meta placeholder error.", "Placeholder error detected.", "Meta placeholder error.", "Placeholder error.", "Placeholder issue."},
        {"Meta future error.", "Future error detected.", "Meta future error.", "Future error.", "Future issue."}
    };

    // Find the index of the error_code in fossil_error_codes
    int error_index = -1;
    for (int i = 0; fossil_error_codes[i] != NULL; ++i) {
        if (strcmp(error_code, fossil_error_codes[i]) == 0) {
            error_index = i;
            break;
        }
    }

    if (error_index >= 0) {
        // Pick a variant (for example, always 0, or random if you want)
        int variant = 0;
        return fossil_error_messages[error_index][variant];
    } else {
        return "Unknown error code.";
    }
    }
}
