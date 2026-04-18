#include "pqc_lib.h"
#include <oqs/oqs.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

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

/* File Encryption using AES-256-GCM */
int pqc_encrypt_file(const char *input_path, const char *output_path, const uint8_t *shared_secret) {
    FILE *ifp = fopen(input_path, "rb");
    FILE *ofp = fopen(output_path, "wb");
    if (!ifp || !ofp) return -1;

    unsigned char iv[AES_IV_SIZE];
    RAND_bytes(iv, AES_IV_SIZE); // Generate a random IV
    fwrite(iv, 1, AES_IV_SIZE, ofp); // Store IV at the start of the file

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, shared_secret, iv);

    unsigned char in_buf[4096], out_buf[4096 + 16];
    int in_len, out_len;

    while ((in_len = fread(in_buf, 1, 4096, ifp)) > 0) {
        EVP_EncryptUpdate(ctx, out_buf, &out_len, in_buf, in_len);
        fwrite(out_buf, 1, out_len, ofp);
    }

    EVP_EncryptFinal_ex(ctx, out_buf, &out_len);
    fwrite(out_buf, 1, out_len, ofp);

    unsigned char tag[AES_TAG_SIZE];
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, AES_TAG_SIZE, tag);
    fwrite(tag, 1, AES_TAG_SIZE, ofp); // Store the Auth Tag at the end

    EVP_CIPHER_CTX_free(ctx);
    fclose(ifp); fclose(ofp);
    return 0;
}

/* File Decryption using AES-256-GCM */
int pqc_decrypt_file(const char *input_path, const char *output_path, const uint8_t *shared_secret) {
    FILE *ifp = fopen(input_path, "rb");
    FILE *ofp = fopen(output_path, "wb");
    if (!ifp || !ofp) return -1;

    unsigned char iv[AES_IV_SIZE];
    fread(iv, 1, AES_IV_SIZE, ifp); // Read the IV from the start

    // Seek to the end to get the tag, then back to data
    fseek(ifp, -AES_TAG_SIZE, SEEK_END);
    unsigned char tag[AES_TAG_SIZE];
    fread(tag, 1, AES_TAG_SIZE, ifp);
    long file_size = ftell(ifp) - AES_IV_SIZE - AES_TAG_SIZE;
    fseek(ifp, AES_IV_SIZE, SEEK_SET);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, shared_secret, iv);

    unsigned char in_buf[4096], out_buf[4096 + 16];
    int in_len, out_len;
    long total_read = 0;

    while (total_read < file_size) {
        int to_read = (file_size - total_read > 4096) ? 4096 : (file_size - total_read);
        in_len = fread(in_buf, 1, to_read, ifp);
        EVP_DecryptUpdate(ctx, out_buf, &out_len, in_buf, in_len);
        fwrite(out_buf, 1, out_len, ofp);
        total_read += in_len;
    }

    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, AES_TAG_SIZE, tag);
    int rc = EVP_DecryptFinal_ex(ctx, out_buf, &out_len);
    if (rc > 0) fwrite(out_buf, 1, out_len, ofp);

    EVP_CIPHER_CTX_free(ctx);
    fclose(ifp); fclose(ofp);
    return (rc > 0) ? 0 : -1; // -1 means the file was tampered with!
}
