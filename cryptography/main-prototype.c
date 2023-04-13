#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#include "encrypt.h"

#define KEY_LEN 16
#define IV_LEN 12
#define MAX_MSG_LEN 1024

int main()
{
    unsigned char key[KEY_LEN] = "mysecretkey12345";
    unsigned char iv[IV_LEN] = "myniceiv789";
    unsigned char plaintext[MAX_MSG_LEN];
    unsigned char ciphertext[MAX_MSG_LEN];

    unsigned long long ciphertext_len = 0;
    unsigned long long plaintext_len = 0;

    // Prompt user for plaintext
    printf("Enter plaintext (max %d bytes): ", MAX_MSG_LEN);
    scanf("%1023s", plaintext);
    plaintext_len = strlen(plaintext);

    // Encrypt the plaintext
    int encrypted = crypto_aead_encrypt(ciphertext, &ciphertext_len, plaintext, plaintext_len, NULL, 0, NULL, iv, key);
    if (encrypted == 0)
    {
        // Print the ciphertext
        printf("Ciphertext (length %llu): ", ciphertext_len);
        fwrite(ciphertext, 1, ciphertext_len, stdout);
        printf("\n");
    }
    else
    {
        printf("Encryption failed!\n");
        return 1;
    }

    // Prompt user for ciphertext2
    unsigned char ciphertext2[MAX_MSG_LEN];

    unsigned long long ciphertext2_len = 0;
    printf("Enter ciphertext2 (max %d bytes): ", MAX_MSG_LEN);
    scanf("%1023s", ciphertext2);
    ciphertext2_len = strlen(ciphertext2);

    // Decrypt the ciphertext
    unsigned char decrypted_plaintext[MAX_MSG_LEN];
    unsigned long long decrypted_plaintext_len = 0;
    int decrypted = crypto_aead_decrypt(decrypted_plaintext, &decrypted_plaintext_len, NULL, ciphertext, ciphertext_len, NULL, 0, iv, key);

    if (decrypted == 0)
    {
        // Print the decrypted plaintext
        printf("Decrypted plaintext (length %llu): %s ", decrypted_plaintext_len, decrypted_plaintext);
        printf("\n");
    }
    else
    {
        printf("Decryption failed!\n");
        return 1;
    }

    return 0;
}
