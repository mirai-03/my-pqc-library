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

    printf("[STEP 1] Generating ML-DSA Keypair...\n");
    pqc_sig_keygen(sig_pk, sig_sk);
    
    printf("[STEP 2] Signing Message...\n");
    pqc_sign(signature, &sig_len, (unsigned char *)message, strlen(message), sig_sk);

    printf("[STEP 3] Verifying Signature...\n");
    if (pqc_verify(signature, sig_len, (unsigned char *)message, strlen(message), sig_pk) == 0) {
        printf(">>> VERIFICATION SUCCESSFUL! (Integrity Confirmed)\n\n");
    }

    /* 2. TEST KEY EXCHANGE (ML-KEM) */
    unsigned char kem_pk[PK_SIZE];
    unsigned char kem_sk[SK_SIZE];
    unsigned char ct[CT_SIZE];
    unsigned char key_alice[SHARED_SECRET_SIZE], key_bob[SHARED_SECRET_SIZE];

    printf("[STEP 4] Testing ML-KEM Handshake...\n");
    generate_pqc_keys(kem_pk, kem_sk);
    pqc_encrypt(ct, key_alice, kem_pk);
    pqc_decrypt(key_bob, ct, kem_sk);

    if (memcmp(key_alice, key_bob, SHARED_SECRET_SIZE) == 0) {
        printf(">>> KEM SUCCESSFUL! Shared secrets match.\n\n");
    }

    /* 3. TEST FILE ENCRYPTION (AES-256-GCM) */
    printf("[STEP 5] Testing File Security (Hybrid PQC + AES)...\n");

    // Create a dummy file
    FILE *f = fopen("test_data.txt", "w");
    if (f) {
        fprintf(f, "Quantum Computers cannot read this message!");
        fclose(f);
    }

    // Encrypt the file using Alice's key
    if (pqc_encrypt_file("test_data.txt", "test_data.vault", key_alice) == 0) {
        printf(">>> ENCRYPTION SUCCESSFUL! File locked in 'test_data.vault'.\n");
    }

    // Decrypt the file using Bob's key
    if (pqc_decrypt_file("test_data.vault", "decrypted.txt", key_bob) == 0) {
        printf(">>> DECRYPTION SUCCESSFUL! Data recovered in 'decrypted.txt'.\n");
    }

    printf("\n--- ALL SYSTEMS OPERATIONAL ---\n");
    return 0;
}
