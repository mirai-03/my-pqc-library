#include "pqc_lib.h"
#include <oqs/oqs.h>

/* KEM Functions (Keeping your original names) */
int generate_pqc_keys(uint8_t *public_key, uint8_t *secret_key) {
    OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_kyber_512);
    if (!kem) return -1;
    OQS_KEM_keypair(kem, public_key, secret_key);
    OQS_KEM_free(kem);
    return 0;
}

int pqc_encrypt(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
    OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_kyber_512);
    if (!kem) return -1;
    OQS_KEM_encaps(kem, ciphertext, shared_secret, public_key);
    OQS_KEM_free(kem);
    return 0;
}

int pqc_decrypt(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key) {
    OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_kyber_512);
    if (!kem) return -1;
    OQS_KEM_decaps(kem, shared_secret, ciphertext, secret_key);
    OQS_KEM_free(kem);
    return 0;
}

/* Signature Functions (Adding the new logic) */
int pqc_sig_keygen(unsigned char *pk, unsigned char *sk) {
    OQS_SIG *sig = OQS_SIG_new(OQS_SIG_alg_ml_dsa_65);
    if (!sig) return -1;
    OQS_SIG_keypair(sig, pk, sk);
    OQS_SIG_free(sig);
    return 0;
}

int pqc_sign(unsigned char *sig, size_t *sig_len, const unsigned char *msg, size_t msg_len, const unsigned char *sk) {
    OQS_SIG *sig_obj = OQS_SIG_new(OQS_SIG_alg_ml_dsa_65);
    if (!sig_obj) return -1;
    OQS_SIG_sign(sig_obj, sig, sig_len, msg, msg_len, sk);
    OQS_SIG_free(sig_obj);
    return 0;
}

int pqc_verify(const unsigned char *sig, size_t sig_len, const unsigned char *msg, size_t msg_len, const unsigned char *pk) {
    OQS_SIG *sig_obj = OQS_SIG_new(OQS_SIG_alg_ml_dsa_65);
    if (!sig_obj) return -1;
    int rc = OQS_SIG_verify(sig_obj, msg, msg_len, sig, sig_len, pk);
    OQS_SIG_free(sig_obj);
    return (rc == OQS_SUCCESS) ? 0 : -1;
}
