#include "pqc_lib.h"
#include <oqs/oqs.h>

int generate_pqc_keys(uint8_t *public_key, uint8_t *secret_key) {
    OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_kyber_512);
    if (!kem) return -1;
    OQS_KEM_keypair(kem, public_key, secret_key);
    OQS_KEM_free(kem);
    return 0;
}

int pqc_encrypt(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
    OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_kyber_512);
    OQS_KEM_encaps(kem, ciphertext, shared_secret, public_key);
    OQS_KEM_free(kem);
    return 0;
}

int pqc_decrypt(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key) {
    OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_kyber_512);
    OQS_KEM_decaps(kem, shared_secret, ciphertext, secret_key);
    OQS_KEM_free(kem);
    return 0;
}
