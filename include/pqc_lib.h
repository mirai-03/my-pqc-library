#ifndef PQC_LIB_H
#define PQC_LIB_H

#include <stdint.h>
#include <stddef.h>

// We use Kyber-512 (Standard PQC for replacing RSA-2048)
#define PK_SIZE 800
#define SK_SIZE 1632
#define CT_SIZE 768
#define SHARED_SECRET_SIZE 32

// Your simplified functions
int generate_pqc_keys(uint8_t *public_key, uint8_t *secret_key);
int pqc_encrypt(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
int pqc_decrypt(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

#endif
