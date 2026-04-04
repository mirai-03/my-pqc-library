#include <stdio.h>
#include <string.h>
#include "include/pqc_lib.h"

int main() {
    uint8_t public_key[PK_SIZE];
    uint8_t secret_key[SK_SIZE];
    uint8_t ciphertext[CT_SIZE];
    uint8_t secret_client[SHARED_SECRET_SIZE];
    uint8_t secret_server[SHARED_SECRET_SIZE];

    printf("Generating Quantum-Safe Keys...\n");
    generate_pqc_keys(public_key, secret_key);

    printf("Client: Creating shared secret...\n");
    pqc_encrypt(ciphertext, secret_client, public_key);

    printf("Server: Recovering shared secret...\n");
    pqc_decrypt(secret_server, ciphertext, secret_key);

    if (memcmp(secret_client, secret_server, SHARED_SECRET_SIZE) == 0) {
        printf("SUCCESS! Both sides have the same 32-byte secret.\n");
    } else {
        printf("ERROR: Secrets do not match!\n");
    }

    return 0;
}
