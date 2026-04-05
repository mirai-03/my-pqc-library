#ifndef PQC_LIB_H
#define PQC_LIB_H

#include <stdint.h>
#include <stddef.h>

/* Your Original KEM Constants */
#define PK_SIZE 800
#define SK_SIZE 1632
#define CT_SIZE 768
#define SHARED_SECRET_SIZE 32

/* New Signature Constants */
#define PQ_SIG_PUBLIC_KEY_SIZE 1952
#define PQ_SIG_SECRET_KEY_SIZE 4032
#define PQ_SIGNATURE_SIZE      3309

/* Your Original Functions */
int generate_pqc_keys(uint8_t *public_key, uint8_t *secret_key);
int pqc_encrypt(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
int pqc_decrypt(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key);

/* New Signature Functions */
int pqc_sig_keygen(unsigned char *pk, unsigned char *sk);
int pqc_sign(unsigned char *sig, size_t *sig_len, const unsigned char *msg, size_t msg_len, const unsigned char *sk);
int pqc_verify(const unsigned char *sig, size_t sig_len, const unsigned char *msg, size_t msg_len, const unsigned char *pk);

#endif
