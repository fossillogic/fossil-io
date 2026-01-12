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
#ifndef FOSSIL_IO_CIPHER_H
#define FOSSIL_IO_CIPHER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cipher Registry and Internal Logic
 *
 * This module provides unified encode/decode APIs for a set of supported text ciphers.
 * Each cipher is identified by a string ID and registered in a static lookup table.
 *
 * Available cipher string IDs:
 *   - "caesar"
 *   - "vigenere"
 *   - "base64"
 *   - "base32"
 *   - "binary"
 *   - "morse"
 *   - "baconian"
 *   - "railfence"
 *   - "haxor"
 *   - "leet"
 *   - "rot13"
 *   - "atbash"
 *
 * Internal logic:
 *   - Ciphers are registered in a static table: static const cipher_entry cipher_table[].
 *   - Lookup is performed by string ID (case-insensitive) using a linear search (O(n) time).
 *   - Each cipher_entry contains the cipher's string ID and a function pointer.
 *   - The encode/decode API calls the cipher function with the input text and mode.
 *   - Cipher functions return a newly allocated string (caller must free).
 *   - If an unknown cipher ID is provided, the API returns NULL.
 *
 * Example usage:
 *   char *encoded = fossil_io_cipher_encode("Hello", "caesar");
 *   char *decoded = fossil_io_cipher_decode(encoded, "caesar");
 *   free(encoded);
 *   free(decoded);
 *
 * Notes:
 *   - All cipher functions are stateless and thread-safe.
 *   - The registry is static and not extensible at runtime.
 *   - The lookup is O(n) (linear in number of ciphers).
 *   - Input validation is performed; NULL is returned for invalid arguments.
 *   - The encode/decode APIs are symmetric for reversible ciphers.
 *   - Some ciphers (e.g., vigenere, railfence) use fixed parameters in this implementation.
 *   - The caller is responsible for freeing returned buffers.
 */

/**
 * Encode text using a named cipher.
 *
 * Internal logic:
 *   - Ciphers are registered in cipher_table[].
 *   - Lookup is performed by string ID (case-insensitive).
 *   - Encode API selects the cipher and calls its function.
 *   - Returns a newly allocated string (caller must free).
 *   - If an unknown cipher ID is provided, returns NULL.
 *
 * Available cipher string IDs:
 *   - "caesar"
 *   - "vigenere"
 *   - "base64"
 *   - "base32"
 *   - "binary"
 *   - "morse"
 *   - "baconian"
 *   - "railfence"
 *   - "haxor"
 *   - "leet"
 *   - "rot13"
 *   - "atbash"
 */
char *fossil_io_cipher_encode(const char *text, const char *cipher_id);

/**
 * Decode text using a named cipher.
 *
 * Internal logic:
 *   - Ciphers are registered in cipher_table[].
 *   - Lookup is performed by string ID (case-insensitive).
 *   - Decode API selects the cipher and calls its function.
 *   - Returns a newly allocated string (caller must free).
 *   - If an unknown cipher ID is provided, returns NULL.
 *
 * Available cipher string IDs:
 *   - "caesar"
 *   - "vigenere"
 *   - "base64"
 *   - "base32"
 *   - "binary"
 *   - "morse"
 *   - "baconian"
 *   - "railfence"
 *   - "haxor"
 *   - "leet"
 *   - "rot13"
 *   - "atbash"
 */
char *fossil_io_cipher_decode(const char *text, const char *cipher_id);

#ifdef __cplusplus
}

#include <string>

namespace fossil {
    namespace io {
    
        /**
         * Cipher
         *
         * Thin C++ wrapper around Fossil IO cipher encode/decode API.
         *
         * This class:
         *  - Owns returned buffers safely
         *  - Does not throw
         *  - Does not allocate unless the C layer does
         *  - Is header-only and ABI-neutral
         */
        class Cipher {
        public:
            /**
             * Encode text using a named cipher.
             *
             * Internal logic:
             *   - Ciphers are registered in cipher_table[].
             *   - Lookup is performed by string ID (case-insensitive).
             *   - The encode API selects the cipher and calls its function.
             *   - Returns a newly allocated string (caller must free).
             *   - If an unknown cipher ID is provided, returns an empty string.
             *
             * Available cipher string IDs:
             *   - "caesar"
             *   - "vigenere"
             *   - "base64"
             *   - "base32"
             *   - "binary"
             *   - "morse"
             *   - "baconian"
             *   - "railfence"
             *   - "haxor"
             *   - "leet"
             *   - "rot13"
             *   - "atbash"
             */
            static std::string encode(const std::string &text,
                                      const std::string &cipher_id) {
                return fossil_io_cipher_encode(text.c_str(), cipher_id.c_str());
            }

            /**
             * Decode text using a named cipher.
             *
             * Internal logic:
             *   - Ciphers are registered in cipher_table[].
             *   - Lookup is performed by string ID (case-insensitive).
             *   - The decode API selects the cipher and calls its function.
             *   - Returns a newly allocated string (caller must free).
             *   - If an unknown cipher ID is provided, returns an empty string.
             *
             * Available cipher string IDs:
             *   - "caesar"
             *   - "vigenere"
             *   - "base64"
             *   - "base32"
             *   - "binary"
             *   - "morse"
             *   - "baconian"
             *   - "railfence"
             *   - "haxor"
             *   - "leet"
             *   - "rot13"
             *   - "atbash"
             */
            static std::string decode(const std::string &text,
                                      const std::string &cipher_id) {
                return fossil_io_cipher_decode(text.c_str(), cipher_id.c_str());
            }
        
        };
    
    } // namespace io
} // namespace fossil

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
