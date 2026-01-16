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

// SYSTEM / META error code tests
FOSSIL_TEST(cpp_test_error_code_system_meta) {
    const char *codes[] = {
        "system.ok", "system.unknown", "system.internal", "system.fatal", "system.panic",
        "system.abort", "system.assertion", "system.invariant", "system.contract",
        "system.recoverable", "system.unrecoverable", "system.transient", "system.permanent",
        "system.unsupported", "system.unimplemented", "system.deprecated", "system.experimental",
        "system.misconfigured", "system.corrupt", "system.bootstrap", "system.shutdown",
        "system.restart", "system.upgrade", "system.downgrade", "system.permission",
        "system.capability"
    };
    for (size_t i = 0; i < sizeof(codes)/sizeof(*codes); ++i) {
        int id = fossil::io::Error::code(codes[i]);
        ASSUME_ITS_TRUE(id >= 0);
        const char *msg = fossil::io::Error::what(codes[i]);
        ASSUME_ITS_TRUE(msg && strcmp(msg, "unknown") != 0);
    }
}

// IO error code tests
FOSSIL_TEST(cpp_test_error_code_io) {
    const char *codes[] = {
        "io.read", "io.write", "io.seek", "io.flush", "io.sync", "io.fsync", "io.truncate",
        "io.append", "io.scatter", "io.gather", "io.closed", "io.eof", "io.partial",
        "io.short", "io.blocked", "io.nonblocking", "io.timeout", "io.interrupt", "io.retry",
        "io.corrupt", "io.checksum", "io.buffer", "io.alignment", "io.direct", "io.stream",
        "io.pipe"
    };
    for (size_t i = 0; i < sizeof(codes)/sizeof(*codes); ++i) {
        int id = fossil::io::Error::code(codes[i]);
        ASSUME_ITS_TRUE(id >= 0);
        const char *msg = fossil::io::Error::what(codes[i]);
        ASSUME_ITS_TRUE(msg && strcmp(msg, "unknown") != 0);
    }
}

// MEMORY error code tests
FOSSIL_TEST(cpp_test_error_code_memory) {
    const char *codes[] = {
        "memory.alloc", "memory.realloc", "memory.free", "memory.map", "memory.unmap",
        "memory.remap", "memory.lock", "memory.unlock", "memory.protect", "memory.unprotect",
        "memory.leak", "memory.overrun", "memory.underrun", "memory.use_after_free",
        "memory.double_free", "memory.fragmented", "memory.exhausted", "memory.alignment",
        "memory.page_fault", "memory.segmentation", "memory.guard", "memory.poison",
        "memory.swap", "memory.numa"
    };
    for (size_t i = 0; i < sizeof(codes)/sizeof(*codes); ++i) {
        int id = fossil::io::Error::code(codes[i]);
        ASSUME_ITS_TRUE(id >= 0);
        const char *msg = fossil::io::Error::what(codes[i]);
        ASSUME_ITS_TRUE(msg && strcmp(msg, "unknown") != 0);
    }
}

// CPU / EXECUTION error code tests
FOSSIL_TEST(cpp_test_error_code_cpu) {
    const char *codes[] = {
        "cpu.illegal_instruction", "cpu.privilege_violation", "cpu.div_zero", "cpu.overflow",
        "cpu.underflow", "cpu.fpu", "cpu.simd", "cpu.cache", "cpu.pipeline", "cpu.affinity",
        "cpu.throttle"
    };
    for (size_t i = 0; i < sizeof(codes)/sizeof(*codes); ++i) {
        int id = fossil::io::Error::code(codes[i]);
        ASSUME_ITS_TRUE(id >= 0);
        const char *msg = fossil::io::Error::what(codes[i]);
        ASSUME_ITS_TRUE(msg && strcmp(msg, "unknown") != 0);
    }
}

// MATH / NUMERIC error code tests
FOSSIL_TEST(cpp_test_error_code_math) {
    const char *codes[] = {
        "math.overflow", "math.underflow", "math.div_zero", "math.nan", "math.infinity",
        "math.domain", "math.range", "math.precision", "math.rounding", "math.convergence",
        "math.divergence", "math.iteration", "math.singularity", "math.condition",
        "math.approximation"
    };
    for (size_t i = 0; i < sizeof(codes)/sizeof(*codes); ++i) {
        int id = fossil::io::Error::code(codes[i]);
        ASSUME_ITS_TRUE(id >= 0);
        const char *msg = fossil::io::Error::what(codes[i]);
        ASSUME_ITS_TRUE(msg && strcmp(msg, "unknown") != 0);
    }
}

// FILESYSTEM / STORAGE error code tests
FOSSIL_TEST(cpp_test_error_code_fs) {
    const char *codes[] = {
        "fs.not_found", "fs.exists", "fs.permission", "fs.read_only", "fs.locked",
        "fs.busy", "fs.mount", "fs.unmount", "fs.remount", "fs.quota", "fs.corrupt",
        "fs.journal", "fs.snapshot", "fs.backup", "fs.restore", "fs.path", "fs.symlink",
        "fs.hardlink", "fs.inode", "fs.filesystem"
    };
    for (size_t i = 0; i < sizeof(codes)/sizeof(*codes); ++i) {
        int id = fossil::io::Error::code(codes[i]);
        ASSUME_ITS_TRUE(id >= 0);
        const char *msg = fossil::io::Error::what(codes[i]);
        ASSUME_ITS_TRUE(msg && strcmp(msg, "unknown") != 0);
    }
}

// NETWORK error code tests
FOSSIL_TEST(cpp_test_error_code_network) {
    const char *codes[] = {
        "network.unreachable", "network.timeout", "network.reset", "network.refused",
        "network.aborted", "network.dns", "network.routing", "network.latency",
        "network.bandwidth", "network.congestion", "network.proxy", "network.firewall",
        "network.nat", "network.session", "network.stream", "network.packet"
    };
    for (size_t i = 0; i < sizeof(codes)/sizeof(*codes); ++i) {
        int id = fossil::io::Error::code(codes[i]);
        ASSUME_ITS_TRUE(id >= 0);
        const char *msg = fossil::io::Error::what(codes[i]);
        ASSUME_ITS_TRUE(msg && strcmp(msg, "unknown") != 0);
    }
}

// SECURITY / CRYPTO error code tests
FOSSIL_TEST(cpp_test_error_code_security) {
    const char *codes[] = {
        "security.violation", "security.auth_failed", "security.authz_failed",
        "security.identity", "security.credential", "security.token", "security.session",
        "security.encryption", "security.decryption", "security.certificate", "security.key",
        "security.keystore", "security.revocation", "security.sandbox", "security.trust",
        "security.integrity", "security.tamper", "security.replay"
    };
    for (size_t i = 0; i < sizeof(codes)/sizeof(*codes); ++i) {
        int id = fossil::io::Error::code(codes[i]);
        ASSUME_ITS_TRUE(id >= 0);
        const char *msg = fossil::io::Error::what(codes[i]);
        ASSUME_ITS_TRUE(msg && strcmp(msg, "unknown") != 0);
    }
}

// DATABASE / STORAGE ENGINE error code tests
FOSSIL_TEST(cpp_test_error_code_database) {
    const char *codes[] = {
        "database.connect", "database.disconnect", "database.query", "database.prepare",
        "database.execute", "database.transaction", "database.commit", "database.rollback",
        "database.deadlock", "database.lock", "database.constraint", "database.schema",
        "database.migration", "database.index", "database.cursor", "database.replication",
        "database.consistency", "database.timeout"
    };
    for (size_t i = 0; i < sizeof(codes)/sizeof(*codes); ++i) {
        int id = fossil::io::Error::code(codes[i]);
        ASSUME_ITS_TRUE(id >= 0);
        const char *msg = fossil::io::Error::what(codes[i]);
        ASSUME_ITS_TRUE(msg && strcmp(msg, "unknown") != 0);
    }
}

// AI / ML error code tests
FOSSIL_TEST(cpp_test_error_code_ai) {
    const char *codes[] = {
        "ai.model", "ai.version", "ai.load", "ai.unload", "ai.inference", "ai.training",
        "ai.finetune", "ai.dataset", "ai.validation", "ai.bias", "ai.drift",
        "ai.hallucination", "ai.alignment", "ai.confidence", "ai.explainability",
        "ai.prompt", "ai.token_limit", "ai.context_overflow"
    };
    for (size_t i = 0; i < sizeof(codes)/sizeof(*codes); ++i) {
        int id = fossil::io::Error::code(codes[i]);
        ASSUME_ITS_TRUE(id >= 0);
        const char *msg = fossil::io::Error::what(codes[i]);
        ASSUME_ITS_TRUE(msg && strcmp(msg, "unknown") != 0);
    }
}

// UI / UX error code tests
FOSSIL_TEST(cpp_test_error_code_ui) {
    const char *codes[] = {
        "ui.render", "ui.layout", "ui.paint", "ui.refresh", "ui.input", "ui.focus",
        "ui.gesture", "ui.accessibility", "ui.localization", "ui.theme", "ui.font",
        "ui.image"
    };
    for (size_t i = 0; i < sizeof(codes)/sizeof(*codes); ++i) {
        int id = fossil::io::Error::code(codes[i]);
        ASSUME_ITS_TRUE(id >= 0);
        const char *msg = fossil::io::Error::what(codes[i]);
        ASSUME_ITS_TRUE(msg && strcmp(msg, "unknown") != 0);
    }
}

// LEGAL / POLICY error code tests
FOSSIL_TEST(cpp_test_error_code_policy) {
    const char *codes[] = {
        "policy.violation", "policy.denied", "policy.expired", "policy.restricted",
        "license.invalid", "license.expired", "license.restricted", "privacy.violation",
        "privacy.redaction", "compliance.failure", "audit.failure"
    };
    for (size_t i = 0; i < sizeof(codes)/sizeof(*codes); ++i) {
        int id = fossil::io::Error::code(codes[i]);
        ASSUME_ITS_TRUE(id >= 0);
        const char *msg = fossil::io::Error::what(codes[i]);
        ASSUME_ITS_TRUE(msg && strcmp(msg, "unknown") != 0);
    }
}

// Fallback / meta error code tests
FOSSIL_TEST(cpp_test_error_code_meta) {
    const char *codes[] = {
        "meta.unreachable", "meta.assumption", "meta.placeholder", "meta.future"
    };
    for (size_t i = 0; i < sizeof(codes)/sizeof(*codes); ++i) {
        int id = fossil::io::Error::code(codes[i]);
        ASSUME_ITS_TRUE(id >= 0);
        const char *msg = fossil::io::Error::what(codes[i]);
        ASSUME_ITS_TRUE(msg && strcmp(msg, "unknown") != 0);
    }
}

// Test that all codes are unique and stable
FOSSIL_TEST(cpp_test_error_code_uniqueness_and_stability) {
    const char *codes[] = {
        "system.ok", "io.read", "memory.alloc", "cpu.div_zero", "math.nan", "parse.syntax",
        "type.mismatch", "format.invalid", "data.corrupt", "fs.not_found", "process.spawn",
        "thread.create", "resource.exhausted", "time.timeout", "config.missing",
        "api.invalid_call", "protocol.invalid", "network.unreachable", "security.violation",
        "database.connect", "ai.model", "ui.render", "log.write", "build.compile",
        "user.input", "policy.violation", "meta.unreachable"
    };
    int ids[sizeof(codes)/sizeof(*codes)];
    for (size_t i = 0; i < sizeof(codes)/sizeof(*codes); ++i) {
        ids[i] = fossil::io::Error::code(codes[i]);
        ASSUME_ITS_TRUE(ids[i] >= 0);
    }
    for (size_t i = 0; i < sizeof(codes)/sizeof(*codes); ++i) {
        for (size_t j = i + 1; j < sizeof(codes)/sizeof(*codes); ++j) {
            ASSUME_ITS_TRUE(ids[i] != ids[j]);
        }
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_error_tests) {
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_error_code_system_meta);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_error_code_io);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_error_code_memory);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_error_code_cpu);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_error_code_math);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_error_code_fs);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_error_code_network);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_error_code_security);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_error_code_database);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_error_code_ai);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_error_code_ui);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_error_code_policy);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_error_code_meta);
    FOSSIL_TEST_ADD(cpp_error_suite, cpp_test_error_code_uniqueness_and_stability);

    FOSSIL_TEST_REGISTER(cpp_error_suite);
}
