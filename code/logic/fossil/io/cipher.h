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
 * Cipher Registry and API Documentation
 *
 * This module provides a registry of text ciphers and a unified API for encoding and decoding
 * using various cipher algorithms. Each cipher is identified by a string ID and may accept
 * additional options via a colon-separated parameter string (e.g., "caesar:shift=5,wrap=0").
 *
 * Available Cipher IDs and Options:
 *
 * 1. "caesar"
 *    - Options:
 *      - shift=<int>   : Number of positions to shift (default: 3)
 *      - wrap=<bool>   : Wrap alphabet (default: 1)
 *      - alpha=<bool>  : Only shift alphabetic chars (default: 1)
 *    - Usage: encode/decode with optional shift and wrapping.
 *
 * 2. "vigenere"
 *    - Options:
 *      - key=<string>  : Cipher key (required)
 *      - alpha=<bool>  : Only cipher alphabetic chars (default: 1)
 *      - preserve=<bool>: Preserve non-alpha chars (default: 1)
 *    - Usage: encode/decode with Vigenère cipher and key.
 *
 * 3. "base64"
 *    - Options:
 *      - url=<bool>    : Use URL-safe alphabet (default: 0)
 *      - pad=<bool>    : Add padding '=' (default: 1)
 *      - wrap=<int>    : Line wrap length (default: 0)
 *    - Usage: encode/decode using Base64.
 *
 * 4. "base32"
 *    - Options:
 *      - hex=<bool>    : Use base32hex alphabet (default: 0)
 *      - pad=<bool>    : Add padding '=' (default: 1)
 *      - upper=<bool>  : Output uppercase (default: 1)
 *    - Usage: encode/decode using Base32.
 *
 * 5. "binary"
 *    - Options:
 *      - bits=<int>    : Bits per character (default: 8)
 *      - sep=<char>    : Separator character (default: ' ')
 *      - signed=<bool> : Signed mode (default: 0)
 *    - Usage: encode/decode text as binary representation.
 *
 * 6. "morse"
 *    - Options:
 *      - dash=<char>   : Dash symbol (default: '-')
 *      - dot=<char>    : Dot symbol (default: '.')
 *      - charsep=<str> : Character separator (default: ' ')
 *      - wordsep=<str> : Word separator (default: '/')
 *      - strict=<bool> : Skip unknown chars (default: 0)
 *    - Usage: encode/decode using Morse code.
 *
 * 7. "baconian"
 *    - Options:
 *      - variant=<str> : Variant (default: "classic")
 *      - a=<char>      : Symbol for 'A' (default: 'A')
 *      - b=<char>      : Symbol for 'B' (default: 'B')
 *      - group=<int>   : Group size (default: 5)
 *    - Usage: encode/decode using Baconian cipher.
 *
 * 8. "railfence"
 *    - Options:
 *      - rails=<int>   : Number of rails (default: 2)
 *      - offset=<int>  : Offset (default: 0)
 *      - preserve=<bool>: Preserve non-alpha chars (default: 1)
 *    - Usage: encode/decode using Rail Fence cipher.
 *
 * 9. "haxor"
 *    - Options:
 *      - mode=<str>    : Leet mode (default: "1337")
 *      - case=<bool>   : Preserve case (default: 0)
 *      - reverse=<bool>: Enable reverse mapping (default: 0)
 *    - Usage: encode/decode using leet-speak substitutions.
 *
 * API Usage:
 *   - To encode:   char *fossil_io_cipher_encode(const char *text, const char *cipher_id);
 *   - To decode:   char *fossil_io_cipher_decode(const char *text, const char *cipher_id);
 *   - The cipher_id may include options, e.g. "caesar:shift=5,wrap=0".
 *   - Returns a newly allocated string (caller must free).
 *
 * Implementation Notes:
 *   - All ciphers are registered in the cipher_table[].
 *   - Helper functions parse options from the cipher_id string.
 *   - Encoding/decoding is performed by the corresponding cipher function.
 *   - If an unknown cipher_id is provided, the API returns NULL.
 */

/**
 * Encode text using a named cipher.
 *
 * cipher_id format:
 *   "<name>"
 *   "<name>:<option1>=<value1>,<option2>=<value2>"
 *
 * Available Cipher IDs and Options:
 *   1. "caesar"
 *      - shift=<int>   : Number of positions to shift (default: 3)
 *      - wrap=<bool>   : Wrap alphabet (default: 1)
 *      - alpha=<bool>  : Only shift alphabetic chars (default: 1)
 *   2. "vigenere"
 *      - key=<string>  : Cipher key (required)
 *      - alpha=<bool>  : Only cipher alphabetic chars (default: 1)
 *      - preserve=<bool>: Preserve non-alpha chars (default: 1)
 *   3. "base64"
 *      - url=<bool>    : Use URL-safe alphabet (default: 0)
 *      - pad=<bool>    : Add padding '=' (default: 1)
 *      - wrap=<int>    : Line wrap length (default: 0)
 *   4. "base32"
 *      - hex=<bool>    : Use base32hex alphabet (default: 0)
 *      - pad=<bool>    : Add padding '=' (default: 1)
 *      - upper=<bool>  : Output uppercase (default: 1)
 *   5. "binary"
 *      - bits=<int>    : Bits per character (default: 8)
 *      - sep=<char>    : Separator character (default: ' ')
 *      - signed=<bool> : Signed mode (default: 0)
 *   6. "morse"
 *      - dash=<char>   : Dash symbol (default: '-')
 *      - dot=<char>    : Dot symbol (default: '.')
 *      - charsep=<str> : Character separator (default: ' ')
 *      - wordsep=<str> : Word separator (default: '/')
 *      - strict=<bool> : Skip unknown chars (default: 0)
 *   7. "baconian"
 *      - variant=<str> : Variant (default: "classic")
 *      - a=<char>      : Symbol for 'A' (default: 'A')
 *      - b=<char>      : Symbol for 'B' (default: 'B')
 *      - group=<int>   : Group size (default: 5)
 *   8. "railfence"
 *      - rails=<int>   : Number of rails (default: 2)
 *      - offset=<int>  : Offset (default: 0)
 *      - preserve=<bool>: Preserve non-alpha chars (default: 1)
 *   9. "haxor"
 *      - mode=<str>    : Leet mode (default: "1337")
 *      - case=<bool>   : Preserve case (default: 0)
 *      - reverse=<bool>: Enable reverse mapping (default: 0)
 *
 * Usage example:
 *   - "caesar:shift=5,wrap=0"
 *   - "vigenere:key=SECRET,alpha=1"
 *
 * Returns a newly allocated string (caller must free).
 * If cipher_id is invalid or unsupported, returns NULL.
 */
char *fossil_io_cipher_encode(const char *text, const char *cipher_id);

/**
 * Decode text using a named cipher.
 *
 * cipher_id format:
 *   "<name>"
 *   "<name>:<option1>=<value1>,<option2>=<value2>"
 *
 * Available Cipher IDs and Options:
 *   1. "caesar"
 *      - shift=<int>   : Number of positions to shift (default: 3)
 *      - wrap=<bool>   : Wrap alphabet (default: 1)
 *      - alpha=<bool>  : Only shift alphabetic chars (default: 1)
 *   2. "vigenere"
 *      - key=<string>  : Cipher key (required)
 *      - alpha=<bool>  : Only cipher alphabetic chars (default: 1)
 *      - preserve=<bool>: Preserve non-alpha chars (default: 1)
 *   3. "base64"
 *      - url=<bool>    : Use URL-safe alphabet (default: 0)
 *      - pad=<bool>    : Add padding '=' (default: 1)
 *      - wrap=<int>    : Line wrap length (default: 0)
 *   4. "base32"
 *      - hex=<bool>    : Use base32hex alphabet (default: 0)
 *      - pad=<bool>    : Add padding '=' (default: 1)
 *      - upper=<bool>  : Output uppercase (default: 1)
 *   5. "binary"
 *      - bits=<int>    : Bits per character (default: 8)
 *      - sep=<char>    : Separator character (default: ' ')
 *      - signed=<bool> : Signed mode (default: 0)
 *   6. "morse"
 *      - dash=<char>   : Dash symbol (default: '-')
 *      - dot=<char>    : Dot symbol (default: '.')
 *      - charsep=<str> : Character separator (default: ' ')
 *      - wordsep=<str> : Word separator (default: '/')
 *      - strict=<bool> : Skip unknown chars (default: 0)
 *   7. "baconian"
 *      - variant=<str> : Variant (default: "classic")
 *      - a=<char>      : Symbol for 'A' (default: 'A')
 *      - b=<char>      : Symbol for 'B' (default: 'B')
 *      - group=<int>   : Group size (default: 5)
 *   8. "railfence"
 *      - rails=<int>   : Number of rails (default: 2)
 *      - offset=<int>  : Offset (default: 0)
 *      - preserve=<bool>: Preserve non-alpha chars (default: 1)
 *   9. "haxor"
 *      - mode=<str>    : Leet mode (default: "1337")
 *      - case=<bool>   : Preserve case (default: 0)
 *      - reverse=<bool>: Enable reverse mapping (default: 0)
 *
 * Usage example:
 *   - "caesar:shift=5,wrap=0"
 *   - "vigenere:key=SECRET,alpha=1"
 *
 * Returns a newly allocated string (caller must free).
 * If cipher_id is invalid or unsupported, returns NULL.
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
             * cipher_id format:
             *   "<name>"
             *   "<name>:<option1>=<value1>,<option2>=<value2>"
             *
             * Available Cipher IDs and Options:
             *   1. "caesar"
             *      - shift=<int>   : Number of positions to shift (default: 3)
             *      - wrap=<bool>   : Wrap alphabet (default: 1)
             *      - alpha=<bool>  : Only shift alphabetic chars (default: 1)
             *   2. "vigenere"
             *      - key=<string>  : Cipher key (required)
             *      - alpha=<bool>  : Only cipher alphabetic chars (default: 1)
             *      - preserve=<bool>: Preserve non-alpha chars (default: 1)
             *   3. "base64"
             *      - url=<bool>    : Use URL-safe alphabet (default: 0)
             *      - pad=<bool>    : Add padding '=' (default: 1)
             *      - wrap=<int>    : Line wrap length (default: 0)
             *   4. "base32"
             *      - hex=<bool>    : Use base32hex alphabet (default: 0)
             *      - pad=<bool>    : Add padding '=' (default: 1)
             *      - upper=<bool>  : Output uppercase (default: 1)
             *   5. "binary"
             *      - bits=<int>    : Bits per character (default: 8)
             *      - sep=<char>    : Separator character (default: ' ')
             *      - signed=<bool> : Signed mode (default: 0)
             *   6. "morse"
             *      - dash=<char>   : Dash symbol (default: '-')
             *      - dot=<char>    : Dot symbol (default: '.')
             *      - charsep=<str> : Character separator (default: ' ')
             *      - wordsep=<str> : Word separator (default: '/')
             *      - strict=<bool> : Skip unknown chars (default: 0)
             *   7. "baconian"
             *      - variant=<str> : Variant (default: "classic")
             *      - a=<char>      : Symbol for 'A' (default: 'A')
             *      - b=<char>      : Symbol for 'B' (default: 'B')
             *      - group=<int>   : Group size (default: 5)
             *   8. "railfence"
             *      - rails=<int>   : Number of rails (default: 2)
             *      - offset=<int>  : Offset (default: 0)
             *      - preserve=<bool>: Preserve non-alpha chars (default: 1)
             *   9. "haxor"
             *      - mode=<str>    : Leet mode (default: "1337")
             *      - case=<bool>   : Preserve case (default: 0)
             *      - reverse=<bool>: Enable reverse mapping (default: 0)
             *
             * Usage example:
             *   - "caesar:shift=5,wrap=0"
             *   - "vigenere:key=SECRET,alpha=1"
             *
             * Returns a newly allocated string (caller must free).
             * If cipher_id is invalid or unsupported, returns an empty string.
             */
            static std::string encode(const std::string &text,
                                      const std::string &cipher_id) {
                return fossil_io_cipher_encode(text.c_str(), cipher_id.c_str());
            }

            /**
             * Decode text using a named cipher.
             *
             * cipher_id format:
             *   "<name>"
             *   "<name>:<option1>=<value1>,<option2>=<value2>"
             *
             * Available Cipher IDs and Options:
             *   1. "caesar"
             *      - shift=<int>   : Number of positions to shift (default: 3)
             *      - wrap=<bool>   : Wrap alphabet (default: 1)
             *      - alpha=<bool>  : Only shift alphabetic chars (default: 1)
             *   2. "vigenere"
             *      - key=<string>  : Cipher key (required)
             *      - alpha=<bool>  : Only cipher alphabetic chars (default: 1)
             *      - preserve=<bool>: Preserve non-alpha chars (default: 1)
             *   3. "base64"
             *      - url=<bool>    : Use URL-safe alphabet (default: 0)
             *      - pad=<bool>    : Add padding '=' (default: 1)
             *      - wrap=<int>    : Line wrap length (default: 0)
             *   4. "base32"
             *      - hex=<bool>    : Use base32hex alphabet (default: 0)
             *      - pad=<bool>    : Add padding '=' (default: 1)
             *      - upper=<bool>  : Output uppercase (default: 1)
             *   5. "binary"
             *      - bits=<int>    : Bits per character (default: 8)
             *      - sep=<char>    : Separator character (default: ' ')
             *      - signed=<bool> : Signed mode (default: 0)
             *   6. "morse"
             *      - dash=<char>   : Dash symbol (default: '-')
             *      - dot=<char>    : Dot symbol (default: '.')
             *      - charsep=<str> : Character separator (default: ' ')
             *      - wordsep=<str> : Word separator (default: '/')
             *      - strict=<bool> : Skip unknown chars (default: 0)
             *   7. "baconian"
             *      - variant=<str> : Variant (default: "classic")
             *      - a=<char>      : Symbol for 'A' (default: 'A')
             *      - b=<char>      : Symbol for 'B' (default: 'B')
             *      - group=<int>   : Group size (default: 5)
             *   8. "railfence"
             *      - rails=<int>   : Number of rails (default: 2)
             *      - offset=<int>  : Offset (default: 0)
             *      - preserve=<bool>: Preserve non-alpha chars (default: 1)
             *   9. "haxor"
             *      - mode=<str>    : Leet mode (default: "1337")
             *      - case=<bool>   : Preserve case (default: 0)
             *      - reverse=<bool>: Enable reverse mapping (default: 0)
             *
             * Usage example:
             *   - "caesar:shift=5,wrap=0"
             *   - "vigenere:key=SECRET,alpha=1"
             *
             * Returns a newly allocated string (caller must free).
             * If cipher_id is invalid or unsupported, returns an empty string.
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
