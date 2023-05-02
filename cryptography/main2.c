#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#include "encrypt.h"
// The TinyJAMBU-128 encryption function and parameters

#define KEY_LEN 16
#define IV_LEN 12
#define MAX_MSG_LEN 1024

int main()
{
    unsigned char key[KEY_LEN] = "mysecretkey1234";
    unsigned char iv[IV_LEN] = "myniceiv789";
    unsigned char plaintext[MAX_MSG_LEN];
    unsigned char ciphertext[MAX_MSG_LEN];
    unsigned long long ciphertext_len = 0;
    unsigned long long plaintext_len = 0;

    // Prompt user for key and IV
    // printf("Enter 16-byte key: ");
    // scanf("%s", &key);
    // mysecretkey12345

    // printf("Enter 12-byte IV: ");
    // scanf("%s", &iv);
    // myniceiv789

    // Prompt user for plaintext
    printf("Enter plaintext (max %d bytes): ", MAX_MSG_LEN);
    scanf("%s", &plaintext);
    plaintext_len = strlen(plaintext);
    // abcdefghij

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
        printf("encryption failed!\n");
    }

    // Decrypt the ciphertext
    unsigned char decrypted_plaintext[MAX_MSG_LEN];
    unsigned long long decrypted_plaintext_len = 0;
    int decrypted = crypto_aead_decrypt(decrypted_plaintext, &decrypted_plaintext_len, NULL, ciphertext, ciphertext_len, NULL, 0, iv, key);

    if (decrypted == 0)
    {
        // Print the decrypted plaintext
        printf("Decrypted plaintext (length %llu): ", decrypted_plaintext_len);
        fwrite(decrypted_plaintext, 1, decrypted_plaintext_len, stdout);
        printf("\n");
    }
    else
    {
        printf("Decryption failed!\n");
    }

    return 0;
}