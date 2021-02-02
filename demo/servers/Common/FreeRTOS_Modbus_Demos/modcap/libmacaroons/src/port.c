/* Copyright (c) 2014, Robert Escriva
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of this project nor the names of its contributors may
 *       be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* C */
#include <assert.h>
#include <string.h>

/* macaroons */
#include "port.h"
#include "sha256.h"
#include "tweetnacl.h"

#if crypto_secretbox_xsalsa20poly1305_KEYBYTES != MACAROON_SECRET_KEY_BYTES
#error set your constants right
#endif

#if crypto_secretbox_xsalsa20poly1305_NONCEBYTES != MACAROON_SECRET_NONCE_BYTES
#error set your constants right
#endif

#if crypto_secretbox_xsalsa20poly1305_ZEROBYTES != MACAROON_SECRET_TEXT_ZERO_BYTES
#error set your constants right
#endif

#if crypto_secretbox_xsalsa20poly1305_BOXZEROBYTES != MACAROON_SECRET_BOX_ZERO_BYTES
#error set your constants right
#endif

/* So why this port file?  Why add a level of indirection?  It makes the API
 * consistent with the coding style throughout the rest of the code.  A reader
 * familiar with the macaroons code can intuitively follow the primitives,
 * without needing to understand the sodium API.
 *
 * As a bonus, it makes it ridiculously easy to swap out sodium for something a
 * little more hipstery, like TweetNACL if that's your thing.
 */

void
explicit_bzero(void *buf, size_t len);

void
macaroon_memzero(void* data, size_t data_sz)
{
    explicit_bzero(data, data_sz);
}

int
timingsafe_bcmp(const void *b1, const void *b2, size_t n);

int
macaroon_memcmp(const void* data1, const void* data2, size_t data_sz)
{
    return timingsafe_bcmp(data1, data2, data_sz);
}

void
arc4random_buf(void *buf, size_t len);

int
macaroon_randombytes(void* data, const size_t data_sz)
{
    arc4random_buf(data, data_sz);
    return 0;
}

int
macaroon_hmac(const unsigned char* _key, size_t _key_sz,
              const unsigned char* text, size_t text_sz,
              unsigned char* hash)
{
    unsigned char key[MACAROON_HASH_BYTES];
    explicit_bzero(key, MACAROON_HASH_BYTES);
    memmove(key, _key, _key_sz < sizeof(key) ? _key_sz : sizeof(key));
    HMAC_SHA256_Buf(key, MACAROON_HASH_BYTES, text, text_sz, hash);
    return 0;
}

int
macaroon_secretbox(const unsigned char* enc_key,
                   const unsigned char* enc_nonce,
                   const unsigned char* plaintext, size_t plaintext_sz,
                   unsigned char* ciphertext)
{
    return crypto_secretbox_xsalsa20poly1305(ciphertext, plaintext, plaintext_sz, enc_nonce, enc_key);
}

int
macaroon_secretbox_open(const unsigned char* enc_key,
                        const unsigned char* enc_nonce,
                        const unsigned char* ciphertext, size_t ciphertext_sz,
                        unsigned char* plaintext)
{
    return crypto_secretbox_xsalsa20poly1305_open(plaintext, ciphertext, ciphertext_sz, enc_nonce, enc_key);
}

void
macaroon_bin2hex(const unsigned char* bin, size_t bin_sz, char* hex)
{
    static const char hexes[] = "0123456789abcdef";
    size_t i;

    for (i = 0; i < bin_sz; ++i)
    {
        hex[2 * i + 0] = hexes[(bin[i] >> 4) & 0xfu];
        hex[2 * i + 1] = hexes[bin[i] & 0xfU];
    }

    hex[2 * bin_sz] = '\0';
}

int
macaroon_hex2bin(const char* hex, size_t hex_sz, unsigned char* bin)
{
    size_t idx = 0;
    static const char bet[] = "0123456789abcdef";
    const char* tmp = NULL;
    unsigned byte;

    if(hex_sz & 1)
    {
        return -1;
    }

    for (idx = 0; idx < hex_sz; idx += 2)
    {
        byte = 0;
        tmp = strchr(bet, hex[idx]);

        if (!tmp)
        {
            return -1;
        }

        byte |= tmp - bet;
        byte <<= 4;
        tmp = strchr(bet, hex[idx + 1]);

        if (!tmp)
        {
            return -1;
        }

        byte |= tmp - bet;
        bin[idx >> 1] = byte & 0xffU;
    }

    return 0;
}
