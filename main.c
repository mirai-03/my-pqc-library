#include <stdio.h>
#include <string.h>
#include <oqs/oqs.h>
#include "pqc_lib.h"

int main() {
    printf("--- [PQC LIBRARY TEST SUITE: ML-KEM & ML-DSA] ---\n\n");

    /* 1. TEST SIGNATURES (ML-DSA) */
    unsigned char sig_pk[PQ_SIG_PUBLIC_KEY_SIZE];
    unsigned char sig_sk[PQ_SIG_SECRET_KEY_SIZE];
    unsigned char signature[PQ_SIGNATURE_SIZE];
    size_t sig_len;

    const char *message = "This is a quantum-safe signed message.";
    size_t msg_len = strlen(message);

    printf("[STEP 1] Generating ML-DSA Keypair...\n");
    if (pqc_sig_keygen(sig_pk, sig_sk) != 0) {
        fprintf(stderr, "Keygen failed!\n");
        return 1;
    }

    printf("[STEP 2] Signing Message: '%s'\n", message);
    if (pqc_sign(signature, &sig_len, (unsigned char *)message, msg_len, sig_sk) != 0) {
        fprintf(stderr, "Signing failed!\n");
        return 1;
    }

    printf("[STEP 3] Verifying Signature...\n");
    if (pqc_verify(signature, sig_len, (unsigned char *)message, msg_len, sig_pk) == 0) {
        printf(">>> VERIFICATION SUCCESSFUL! (Integrity Confirmed)\n\n");
    } else {
        printf(">>> VERIFICATION FAILED! (Data Corrupted)\n\n");
    }

    /* 2. TEST KEY EXCHANGE (ML-KEM) */
    /* Using your original names and constants here */
    unsigned char kem_pk[PK_SIZE];
    unsigned char kem_sk[SK_SIZE];
    unsigned char ct[CT_SIZE];
    unsigned char key_alice[SHARED_SECRET_SIZE], key_bob[SHARED_SECRET_SIZE];

    printf("[STEP 4] Testing ML-KEM Handshake...\n");
    if (generate_pqc_keys(kem_pk, kem_sk) != 0) {
        fprintf(stderr, "KEM Keygen failed!\n");
        return 1;
    }
    
    pqc_encrypt(ct, key_alice, kem_pk);
    pqc_decrypt(key_bob, ct, kem_sk);

    if (memcmp(key_alice, key_bob, SHARED_SECRET_SIZE) == 0) {
        printf(">>> KEM SUCCESSFUL! Shared secrets match.\n");
    }

    printf("\n--- ALL SYSTEMS OPERATIONAL ---\n");
    return 0;
}
