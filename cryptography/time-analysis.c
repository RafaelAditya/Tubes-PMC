#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#include "encrypt.h"
#include <time.h>
#define KEY_LEN 16
#define IV_LEN 12
#define MAX_MSG_LEN 1024

struct timespec {
    time_t tv_sec;      // seconds
    long   tv_nsec;     // nanoseconds
};

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

    // Measure encryption time
    clock_t start = clock();

    // Encrypt the plaintext
    int encrypted = crypto_aead_encrypt(ciphertext, &ciphertext_len, plaintext, plaintext_len, NULL, 0, NULL, iv, key);

    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Waktunya berapa lama: %f\n", time_taken);

    if (encrypted == 0)
    {
        // Print the ciphertext as a string
        printf("Ciphertext : ");
        for (unsigned long long i = 0; i < ciphertext_len; i++)
        {
            printf("%02X", ciphertext[i]);
        }
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
    char input[MAX_MSG_LEN * 2 + 1];
    printf("Enter ciphertext2 (max %d bytes): ", MAX_MSG_LEN);
    scanf("%s", input);
    ciphertext2_len = strlen(input) / 2;
    for (unsigned long long i = 0; i < ciphertext2_len; i++)
    {
        sscanf(&input[i * 2], "%2hhx", &ciphertext2[i]);
    }

    // Decrypt the ciphertext
    unsigned char decrypted_plaintext[MAX_MSG_LEN];
    unsigned long long decrypted_plaintext_len = plaintext_len;
    int decrypted = crypto_aead_decrypt(decrypted_plaintext, &decrypted_plaintext_len, NULL, ciphertext2, ciphertext2_len, NULL, 0, iv, key);

    if (decrypted == 0)
    {
        // Add null character to the decrypted plaintext
        decrypted_plaintext[decrypted_plaintext_len] = '\0';

        // Print the decrypted plaintext
        printf("Decrypted plaintext : %s\n", decrypted_plaintext);
    }
    else if (decrypted == -1)
    {
        printf("Decryption failed!\n");
    }

    return 0;
}