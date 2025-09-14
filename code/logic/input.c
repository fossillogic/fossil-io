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
#include "fossil/io/input.h"
#include "fossil/io/output.h"
#include "fossil/io/cstring.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <math.h>

#ifdef __WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

/* ============================================================
 * Helpers
 * ============================================================ */

static inline int is_allowed_generic(char c) {
    if (isalnum((unsigned char)c)) return 1;
    switch (c) {
        case ' ': case '_': case '-': case '.': case ',': case ':':
        case '/': case '\\': case '@': case '+': case '=': case '#':
        case '%': case '(': case ')': case '[': case ']':
            return 1;
        default:
            return 0;
    }
}

/* Allowed chars for specific contexts */
static inline int is_allowed_html(char c) {
    return (isalnum((unsigned char)c) || c==' ' || c=='-' || c=='_' || c=='.' || c==',' );
}

static inline int is_allowed_sql(char c) {
    return (isalnum((unsigned char)c) || c==' ' || c=='_' || c=='-' );
}

static inline int is_allowed_shell(char c) {
    return (isalnum((unsigned char)c) || c==' ' || c=='_' || c=='-' || c=='.' || c=='/' );
}

static inline int is_allowed_filename(char c) {
    return (isalnum((unsigned char)c) || c=='_' || c=='-' || c=='.');
}

/* Base64 heuristic */
static int long_base64_run(const char *s, size_t len, size_t threshold) {
    size_t run = 0;
    for (size_t i = 0; i < len; ++i) {
        unsigned char c = (unsigned char)s[i];
        if ((c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') ||
            c == '+' || c == '/' || c == '=') {
            run++;
            if (run >= threshold) return 1;
        } else {
            run = 0;
        }
    }
    return 0;
}

char *fossil_io_gets_from_stream_ex(char *buf, size_t size, fossil_fstream_t *input_stream, int *error_code) {
    if (buf == NULL || size == 0 || input_stream == NULL || error_code == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Invalid buffer, stream, or error code.\n");
        return NULL;
    }

    // Use fgets to get the input from the stream
    if (fgets(buf, size, input_stream->file) == NULL) {
        if (feof(input_stream->file)) {
            *error_code = EOF;
            return NULL; // End of file reached
        }
        *error_code = ferror(input_stream->file);
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Failed to read from input stream.\n");
        return NULL;
    }

    // Ensure the string is null-terminated
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0'; // Remove the newline character
    }

    // Trim any leading or trailing whitespace
    fossil_io_trim(buf);

    return buf;
}

/* Case-insensitive contains */
static int strncase_contains(const char *haystack, const char *needle, size_t len) {
    size_t nlen = strlen(needle);
    if (nlen == 0 || nlen > len) return 0;
    for (size_t i = 0; i + nlen <= len; i++) {
        size_t j;
        for (j = 0; j < nlen; j++) {
            char a = haystack[i+j];
            char b = needle[j];
            if (tolower((unsigned char)a) != tolower((unsigned char)b)) break;
        }
        if (j == nlen) return 1;
    }
    return 0;
}

// Function to trim leading and trailing spaces from a string
void fossil_io_trim(char *str) {
    if (str == NULL) return;

    // Trim leading spaces
    char *start = str;
    while (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r') {
        start++;
    }

    // Move the trimmed string to the beginning
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }

    // Trim trailing spaces
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[--len] = '\0';
    }
}

int fossil_io_display_menu(const char *prompt, const char *choices[], int num_choices) {
    if (prompt != NULL) {
        fossil_io_printf("%s\n", prompt);
    }

    for (int i = 0; i < num_choices; i++) {
        fossil_io_printf("%d. %s\n", i + 1, choices[i]);
    }

    int choice;
    do {
        fossil_io_printf("Please choose an option (1-%d): ", num_choices);
        if (fossil_io_scanf("%d", &choice) != 1 || choice < 1 || choice > num_choices) {
            fossil_io_printf("Invalid choice. Please try again.\n");
        }
    } while (choice < 1 || choice > num_choices);

    return choice - 1; // Return the index of the chosen option
}

void fossil_io_show_progress(int progress) {
    int width = 50; // Width of the progress bar
    int pos = (progress * width) / 100;
    fossil_io_printf("[");
    for (int i = 0; i < width; i++) {
        if (i < pos) {
            fossil_io_printf("=");
        } else if (i == pos) {
            fossil_io_printf(">");
        } else {
            fossil_io_printf(" ");
        }
    }
    fossil_io_printf("] %d%%\r", progress);
    fflush(stdout);
}

int fossil_io_getc(fossil_fstream_t *input_stream) {
    if (input_stream == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Invalid input stream.\n");
        return EOF;
    }
    
    int c = fgetc(input_stream->file);
    if (c == EOF && ferror(input_stream->file)) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Failed to read from input stream.\n");
    }
    
    return c;
}

// Function to get a sanitized line of input from a provided stream (or stdin by default)
char *fossil_io_gets_from_stream(char *buf, size_t size, fossil_fstream_t *input_stream) {
    if (buf == NULL || size == 0 || input_stream == NULL) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Invalid buffer or stream.\n");
        return NULL;
    }

    // Use fgets to get the input from the stream
    if (fgets(buf, size, input_stream->file) == NULL) {
        if (feof(input_stream->file)) {
            return NULL; // End of file reached
        }
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Failed to read from input stream.\n");
        return NULL;
    }

    // Ensure the string is null-terminated
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0'; // Remove the newline character
    }

    // Trim any leading or trailing whitespace
    fossil_io_trim(buf);

    return buf;
}

/* --- sanitizer --- */
int fossil_io_validate_sanitize_string(const char *input,
                                           char *output,
                                           size_t output_size,
                                           fossil_context_t ctx) {
    if (!input || !output || output_size == 0) {
        if (output && output_size > 0) output[0] = '\0';
        return FOSSIL_SAN_MODIFIED;
    }

    size_t in_len = fossil_io_cstring_length_safe(input, 4096); /* cap scanning to 4k */
    size_t out_i = 0;
    int flags = FOSSIL_SAN_OK;

    /* Context-specific allowed char filter */
    int (*is_allowed)(char) = is_allowed_generic;
    switch (ctx) {
        case FOSSIL_CTX_HTML:     is_allowed = is_allowed_html; break;
        case FOSSIL_CTX_SQL:      is_allowed = is_allowed_sql; break;
        case FOSSIL_CTX_SHELL:    is_allowed = is_allowed_shell; break;
        case FOSSIL_CTX_FILENAME: is_allowed = is_allowed_filename; break;
        default:                  is_allowed = is_allowed_generic; break;
    }

    /* Suspicious patterns */
    const char *script_patterns[] = {
        "<script", "javascript:", "onerror=", "onload=", "onclick=",
        "eval(", "document.cookie", "alert(", "src=", "iframe", "onmouseover=",
        "onfocus=", "onblur=", "onchange=", "oninput=", "onreset=", "onsubmit=",
        "onselect=", "onkeydown=", "onkeyup=", "onkeypress=", "onmousedown=",
        "onmouseup=", "onmousemove=", "onmouseenter=", "onmouseleave=", "onwheel=",
        "oncontextmenu=", "oncopy=", "oncut=", "onpaste=", "location.href",
        "window.open", "window.location", NULL
    };
    const char *sql_patterns[] = {
        "select ", "insert ", "update ", "delete ", "drop ", "union ",
        "--", ";--", "/*", "*/", "0x", "xp_", "exec ", "sp_", "information_schema",
        "truncate ", "alter ", "create ", "rename ", "grant ", "revoke ", "cast(",
        "convert(", "declare ", "fetch ", "open ", "close ", "rollback ", "commit ",
        "savepoint ", "release ", "begin ", "end ", NULL
    };
    const char *shell_patterns[] = {
        "curl ", "wget ", "rm -rf", "powershell", "cmd.exe",
        "exec(", "system(", "|", "&&", "||", "bash", "sh", "zsh", "fish", "scp ",
        "ssh ", "ftp ", "tftp ", "nc ", "netcat ", "nmap ", "chmod ", "chown ",
        "sudo ", "kill ", "pkill ", "ps ", "ls ", "cat ", "dd ", "mkfs ", "mount ",
        "umount ", "service ", "systemctl ", "init ", "reboot ", "shutdown ",
        "start ", "stop ", "restart ", NULL
    };
    const char *bot_patterns[] = {
        "bot", "crawler", "spider", "curl/", "python-requests", "scrapy", "httpclient",
        "libwww", "wget", "java", "go-http-client", "phantomjs", "selenium", "headless",
        "robot", "checker", "monitor", "scan", "probe", "harvest", "grabber", "fetcher",
        "indexer", "parser", "api-client", "node-fetch", "axios", NULL
    };
    const char *spam_patterns[] = {
        "viagra", "free money", "winner", "prize", "click here",
        "http://", "https://", "meta refresh", "casino", "loan", "credit", "bitcoin",
        "crypto", "forex", "investment", "guaranteed", "risk-free", "unsubscribe",
        "buy now", "limited offer", "act now", "earn cash", "work from home", "miracle",
        "weight loss", "no prescription", "cheap", "discount", "deal", "promo", "bonus",
        "gift", "exclusive", "urgent", "clearance", "bargain", "order now", "trial",
        "winner!", "congratulations", "selected", "luxury", "get rich", "easy money",
        NULL
    };
    const char *path_patterns[] = {
        "../", "..\\", "/etc/passwd", "C:\\", "/proc/self/environ", "/proc/version",
        "/proc/cpuinfo", "/proc/meminfo", "/boot.ini", "/windows/", "/winnt/", "/system32/",
        "/sys/", "/dev/", "/bin/", "/sbin/", "/usr/", "/var/", "/tmp/", "/root/", "/home/",
        "/Users/", "/Documents/", "/AppData/", "/Local/", "/Roaming/", "/Program Files/",
        "/ProgramData/", "/Desktop/", "/Downloads/", NULL
    };

    /* Scan categories */
    for (const char **p = script_patterns; *p; ++p)
        if (strncase_contains(input, *p, in_len)) flags |= FOSSIL_SAN_SCRIPT;

    for (const char **p = sql_patterns; *p; ++p)
        if (strncase_contains(input, *p, in_len)) flags |= FOSSIL_SAN_SQL;

    for (const char **p = shell_patterns; *p; ++p)
        if (strncase_contains(input, *p, in_len)) flags |= FOSSIL_SAN_SHELL;

    for (const char **p = bot_patterns; *p; ++p)
        if (strncase_contains(input, *p, in_len)) flags |= FOSSIL_SAN_BOT;

    for (const char **p = spam_patterns; *p; ++p)
        if (strncase_contains(input, *p, in_len)) flags |= FOSSIL_SAN_SPAM;

    for (const char **p = path_patterns; *p; ++p)
        if (strncase_contains(input, *p, in_len)) flags |= FOSSIL_SAN_PATH;

    if (long_base64_run(input, in_len, 80))
        flags |= FOSSIL_SAN_BASE64;

    /* Sanitization pass */
    int modified = 0;
    for (size_t i = 0; i < in_len && out_i < output_size - 1; i++) {
        char c = input[i];
        if (is_allowed(c)) {
            output[out_i++] = c;
        } else {
            output[out_i++] = '_'; /* neutralize */
            modified = 1;
        }
    }
    output[out_i] = '\0';

    /* Edge case: SQL context, but input contains SQL keywords with only allowed chars */
    if ((ctx == FOSSIL_CTX_SQL) && (flags & FOSSIL_SAN_SQL) && !modified) {
        /* Force MODIFIED if SQL pattern detected but no chars were replaced */
        flags |= FOSSIL_SAN_MODIFIED;
    } else if (modified) {
        flags |= FOSSIL_SAN_MODIFIED;
    }

    return flags == 0 ? FOSSIL_SAN_OK : flags;
}

int fossil_io_scanf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = vscanf(format, args);
    va_end(args);
    return result;
}

int fossil_io_fscanf(fossil_fstream_t *input_stream, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = vfscanf(input_stream->file, format, args);
    va_end(args);
    return result;
}

int fossil_io_validate_input_buffer(const char *buf, size_t size) {
    if (buf == NULL || size == 0) {
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Invalid buffer or size.\n");
        return 0;
    }
    return 1;
}

char *fossil_io_gets_utf8(char *buf, size_t size, fossil_fstream_t *input_stream) {
    if (!fossil_io_validate_input_buffer(buf, size)) {
        return NULL;
    }

    // Use fgets to get the input from the stream
    if (fgets(buf, size, input_stream->file) == NULL) {
        if (feof(input_stream->file)) {
            return NULL; // End of file reached
        }
        fossil_io_fprintf(FOSSIL_STDERR, "Error: Failed to read from input stream.\n");
        return NULL;
    }

    // Ensure the string is null-terminated
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0'; // Remove the newline character
    }

    // Trim any leading or trailing whitespace
    fossil_io_trim(buf);

    return buf;
}

int fossil_io_validate_is_suspicious_user(const char *input) {
    if (input == NULL) return 0;

    size_t len = strlen(input);
    if (len == 0) return 0;

    // 1. Too long or too short
    if (len < 3 || len > 32) return 1;

    // 2. Count digits, letters, digit runs, and symbol runs
    int digit_run = 0, max_digit_run = 0, digit_count = 0, alpha_count = 0;
    int symbol_run = 0, max_symbol_run = 0, symbol_count = 0;
    for (size_t i = 0; i < len; i++) {
        if (isdigit((unsigned char)input[i])) {
            digit_run++;
            digit_count++;
            if (digit_run > max_digit_run) max_digit_run = digit_run;
            symbol_run = 0;
        } else if (isalpha((unsigned char)input[i])) {
            alpha_count++;
            digit_run = 0;
            symbol_run = 0;
        } else {
            symbol_run++;
            symbol_count++;
            digit_run = 0;
            if (symbol_run > max_symbol_run) max_symbol_run = symbol_run;
        }
    }

    // 3. Check for long digit/symbol runs or too few letters
    if (max_digit_run >= 5) return 1;                  // suspicious long digit tail
    if (max_symbol_run >= 4) return 1;                 // suspicious long symbol run
    if (digit_count >= 8) return 1;                    // many digits (new: covers user1234567890)
    if ((float)digit_count / len > 0.45) return 1;     // high digit ratio (new: covers a1b2c3d4e5f6g7h8i9j0)
    if ((float)alpha_count / len < 0.3) return 1;      // too few letters
    if ((float)symbol_count / len > 0.3) return 1;     // too many symbols

    // 4. Suspicious keywords and patterns
    const char *bad_keywords[] = {
        "bot", "test", "fake", "spam", "zzz", "null", "admin",
        "user", "guest", "demo", "temp", "unknown", "default", "root",
        "system", "anonymous", "trial", "sample", "password", "qwerty",
        "abc123", "123456", "login", "register", "support", "contact",
        "info", "webmaster", "help", "service", "account", "manager",
        "api", "sys", "operator", "mod", "moderator", "superuser",
        "owner", "master", "testuser", "tester", "dev", "developer",
        "backup", "restore", "error", "fail", "invalid", "void"
    };
    size_t nkeys = sizeof(bad_keywords) / sizeof(bad_keywords[0]);
    for (size_t i = 0; i < nkeys; i++) {
        if (fossil_io_cstring_case_search(input, bad_keywords[i]) != NULL) {
            return 1;
        }
    }

    // 5. Common suspicious patterns (repetitive, alternating, keyboard walks)
    int repetitive = 1, alternating = 1;
    for (size_t i = 1; i < len; i++) {
        if (input[i] != input[i - 1]) repetitive = 0;
        if (i > 1 && input[i] != input[i - 2]) alternating = 0;
    }
    if (repetitive || alternating) return 1;

    // 6. Keyboard walk detection (e.g., "qwerty", "asdf", "zxcv")
    const char *keyboard_walks[] = {
        "qwerty", "asdf", "zxcv", "12345", "67890", "poiuy", "lkjhg", "mnbvc"
    };
    size_t nwalks = sizeof(keyboard_walks) / sizeof(keyboard_walks[0]);
    for (size_t i = 0; i < nwalks; i++) {
        if (fossil_io_cstring_case_search(input, keyboard_walks[i]) != NULL) {
            return 1;
        }
    }

    // 7. Very high entropy (simple Shannon estimate)
    int freq[256] = {0};
    for (size_t i = 0; i < len; i++) freq[(unsigned char)input[i]]++;
    double entropy = 0.0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            double p = (double)freq[i] / len;
            entropy -= p * log2(p);
        }
    }
    if (entropy > 4.2) return 1; // slightly lower threshold for suspicious randomness

    // 8. Looks like an email or URL
    if (strchr(input, '@') || fossil_io_cstring_case_search(input, "http") != NULL) return 1;

    // 9. Looks like a UUID or hex string
    size_t hex_count = 0;
    for (size_t i = 0; i < len; i++) {
        if (isxdigit((unsigned char)input[i])) hex_count++;
    }
    if (hex_count == len && len >= 16) return 1;

    return 0; // not flagged
}

int fossil_io_validate_is_disposable_email(const char *input) {
    if (input == NULL) return 0;
    const char *at = strchr(input, '@');
    if (at == NULL) return 0;

    const char *disposable_domains[] = {
        "mailinator.com", "10minutemail.com", "guerrillamail.com",
        "tempmail.com", "trashmail.com", "yopmail.com"
    };
    size_t ndomains = sizeof(disposable_domains) / sizeof(disposable_domains[0]);

    for (size_t i = 0; i < ndomains; i++) {
        if (fossil_io_cstring_case_compare(at + 1, disposable_domains[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int fossil_io_validate_is_suspicious_bot(const char *input) {
    if (input == NULL) return 0;

    const char *bot_signatures[] = {
        "bot", "crawl", "spider", "scrape", "httpclient", "libwww",
        "wget", "curl", "python-requests", "java", "go-http-client"
    };
    size_t nsignatures = sizeof(bot_signatures) / sizeof(bot_signatures[0]);

    for (size_t i = 0; i < nsignatures; i++) {
        if (fossil_io_cstring_case_search(input, bot_signatures[i]) != NULL) {
            return 1;
        }
    }

    return 0;
}

int fossil_io_validate_is_weak_password(const char *password,
                                        const char *username,
                                        const char *email) {
    if (password == NULL) return 1;

    size_t len = strlen(password);

    // 1. Length check
    if (len < 8 || len > 64) {
        return 1; // too short or unreasonably long
    }

    // 2. Check character diversity
    int has_lower = 0, has_upper = 0, has_digit = 0, has_symbol = 0;
    for (size_t i = 0; i < len; i++) {
        if (islower((unsigned char)password[i])) has_lower = 1;
        else if (isupper((unsigned char)password[i])) has_upper = 1;
        else if (isdigit((unsigned char)password[i])) has_digit = 1;
        else has_symbol = 1;
    }
    int diversity = has_lower + has_upper + has_digit + has_symbol;
    if (diversity < 3) {
        return 1; // not diverse enough
    }

    // 3. Common weak passwords
    const char *weak_list[] = {
        "password", "123456", "123456789", "qwerty", "abc123",
        "letmein", "111111", "123123", "iloveyou", "admin"
    };
    size_t weak_count = sizeof(weak_list) / sizeof(weak_list[0]);
    for (size_t i = 0; i < weak_count; i++) {
        if (fossil_io_cstring_case_compare(password, weak_list[i]) == 0) {
            return 1;
        }
    }

    // 4. Sequential/repetitive patterns
    int seq_inc = 1, seq_dec = 1, same = 1;
    for (size_t i = 1; i < len; i++) {
        if (password[i] != password[i - 1]) same = 0;
        if ((unsigned char)password[i] != (unsigned char)password[i - 1] + 1) seq_inc = 0;
        if ((unsigned char)password[i] != (unsigned char)password[i - 1] - 1) seq_dec = 0;
    }
    if (same || seq_inc || seq_dec) {
        return 1;
    }

    // 5. Prevent reuse of username or email as password
    if (username && *username && fossil_io_cstring_case_compare(password, username) == 0) {
        return 1;
    }
    if (email && *email && fossil_io_cstring_case_compare(password, email) == 0) {
        return 1;
    }

    return 0; // password passed basic strength checks
}

int fossil_io_validate_is_int(const char *input, int *output) {
    if (input == NULL || output == NULL) {
        return 0;
    }

    char *endptr;
    long value = strtol(input, &endptr, 10);

    if (*endptr != '\0' || value < INT_MIN || value > INT_MAX) {
        return 0;
    }

    *output = (int)value;
    return 1;
}

int fossil_io_validate_is_float(const char *input, float *output) {
    if (input == NULL || output == NULL) {
        return 0;
    }

    char *endptr;
    float value = strtof(input, &endptr);

    if (*endptr != '\0') {
        return 0;
    }

    *output = value;
    return 1;
}

int fossil_io_validate_is_alnum(const char *input) {
    if (input == NULL) {
        return 0;
    }

    while (*input != '\0') {
        if (!isalnum(*input)) {
            return 0;
        }
        input++;
    }

    return 1;
}

int fossil_io_validate_is_email(const char *input) {
    if (input == NULL) {
        return 0;
    }

    // Check for the presence of an '@' character
    const char *at = strchr(input, '@');
    if (at == NULL || at == input) {
        // No '@' or no local part before '@'
        return 0;
    }

    // Check for the presence of a '.' character after the '@' character
    const char *dot = strchr(at, '.');
    if (dot == NULL || dot == at + 1) {
        // No '.' after '@' or nothing between '@' and '.'
        return 0;
    }

    // Validate the domain against a list of known mailing services
    const char *valid_services[] = {"gmail.com", "yahoo.com", "outlook.com", "hotmail.com", "icloud.com"};
    size_t num_services = sizeof(valid_services) / sizeof(valid_services[0]);

    for (size_t i = 0; i < num_services; i++) {
        if (strcmp(at + 1, valid_services[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

int fossil_io_validate_is_length(const char *input, size_t max_length) {
    if (input == NULL) {
        return 0;
    }

    return strlen(input) <= max_length;
}

/* ============================================================
 * Sanitizer with bitmask + context
 * ============================================================ */

int fossil_io_gets(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL) {
        return -1; // Error or EOF
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0'; // Remove newline character
    }
    return 0; // Success
}

#define MAX_KEYBINDINGS 256
static fossil_io_keybinding_t _keybindings[MAX_KEYBINDINGS];
static size_t _num_keybindings = 0;

int fossil_io_register_keybinding(int key_code, const char *action) {
    if (!action || _num_keybindings >= MAX_KEYBINDINGS) return 1;

    // Check for duplicate
    for (size_t i = 0; i < _num_keybindings; i++) {
        if (_keybindings[i].key_code == key_code) return 2; // Duplicate key
    }

    _keybindings[_num_keybindings].key_code = key_code;
    _keybindings[_num_keybindings].action = fossil_io_cstring_dup(action);
    if (!_keybindings[_num_keybindings].action) return 3; // Allocation failure

    _num_keybindings++;
    return 0;
}

int fossil_io_unregister_keybinding(int key_code) {
    for (size_t i = 0; i < _num_keybindings; i++) {
        if (_keybindings[i].key_code == key_code) {
            free((void*)_keybindings[i].action);
            // Shift remaining bindings
            for (size_t j = i; j < _num_keybindings - 1; j++) {
                _keybindings[j] = _keybindings[j + 1];
            }
            _num_keybindings--;
            return 0;
        }
    }
    return 1; // Key not found
}

const char* fossil_io_get_keybinding_action(int key_code) {
    for (size_t i = 0; i < _num_keybindings; i++) {
        if (_keybindings[i].key_code == key_code) {
            return _keybindings[i].action;
        }
    }
    return NULL;
}

/**
 * Registers a new key binding with optional callback.
 */
int fossil_io_register_keybinding_with_callback(int key_code, const char *action, fossil_io_action_callback_t cb) {
    if (!action || _num_keybindings >= MAX_KEYBINDINGS) return 1;

    // Check for duplicate key
    for (size_t i = 0; i < _num_keybindings; i++) {
        if (_keybindings[i].key_code == key_code) return 2; // Duplicate
    }

    _keybindings[_num_keybindings].key_code = key_code;
    _keybindings[_num_keybindings].action = fossil_io_cstring_dup(action);
    _keybindings[_num_keybindings].callback = cb;
    if (!_keybindings[_num_keybindings].action) return 3; // Allocation failed

    _num_keybindings++;
    return 0;
}

/**
 * Enhanced key processing that calls registered callback.
 */
int fossil_io_process_keybinding(int key_code) {
    for (size_t i = 0; i < _num_keybindings; i++) {
        if (_keybindings[i].key_code == key_code) {
            if (_keybindings[i].callback) {
                _keybindings[i].callback();  // Execute callback
            } else {
                printf("Action triggered: %s\n", _keybindings[i].action);
            }
            return 1; // Action processed
        }
    }
    return 0; // No binding
}

size_t fossil_io_list_keybindings(fossil_io_keybinding_t *bindings, size_t max_bindings) {
    size_t count = (max_bindings < _num_keybindings) ? max_bindings : _num_keybindings;
    for (size_t i = 0; i < count; i++) {
        bindings[i].key_code = _keybindings[i].key_code;
        bindings[i].action = _keybindings[i].action;
    }
    return count;
}

void fossil_io_clear_keybindings(void) {
    for (size_t i = 0; i < _num_keybindings; i++) {
        free((void*)_keybindings[i].action);
    }
    _num_keybindings = 0;
}
