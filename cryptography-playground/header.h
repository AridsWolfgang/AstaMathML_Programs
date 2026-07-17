#ifndef CRYPTO_HEADER_H
#define CRYPTO_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

/* 01_caesar.c */
void caesar_encrypt(const char *plain, char *cipher, int shift);
void caesar_decrypt(const char *cipher, char *plain, int shift);
void caesar_bruteforce(const char *cipher);

/* 02_vigenere.c */
void vigenere_encrypt(const char *plain, char *cipher, const char *key);
void vigenere_decrypt(const char *cipher, char *plain, const char *key);

/* 03_aes.c */
void aes_encrypt_block(const uint8_t in[16], uint8_t out[16], const uint8_t key[16]);
void aes_decrypt_block(const uint8_t in[16], uint8_t out[16], const uint8_t key[16]);

/* 04_rsa.c */
typedef struct { long long n, e, d; } RSAKey;
void rsa_generate_keys(RSAKey *pub, RSAKey *priv, int bitlen);
long long rsa_encrypt(long long m, long long e, long long n);
long long rsa_decrypt(long long c, long long d, long long n);
long long rsa_sign(long long m, long long d, long long n);
long long rsa_verify(long long s, long long e, long long n);

/* 05_hash.c */
void sha256_hash(const uint8_t *data, size_t len, uint8_t out[32]);

/* 06_utils.c */
void print_hex(const uint8_t *data, size_t len);
void *xmalloc(size_t sz);

/* helpers */
void clear_input(void);

#endif
