#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#include "encrypt.h"
#include "esp_system.h"
#include "esp_console.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define KEY_LEN 16
#define IV_LEN 12
#define MAX_MSG_LEN 1024

unsigned char key[KEY_LEN] = "mysecretkey1234";
unsigned char iv[IV_LEN] = "myniceiv789";
unsigned char plaintext[MAX_MSG_LEN];
unsigned char ciphertext[MAX_MSG_LEN];
unsigned char ciphertext2[MAX_MSG_LEN];

unsigned long long ciphertext_len = 0;
unsigned long long plaintext_len = 0;
unsigned long long ciphertext2_len = 0;
unsigned char decrypted_plaintext[MAX_MSG_LEN];
unsigned long long decrypted_plaintext_len = 0;

void setup() {
    Serial.begin(115200);
    vTaskDelay(portMAX_DELAY); // Prevent loop() from being called
}

void loop() {
    // Prompt user for plaintext
    Serial.print("Enter plaintext (max ");
    Serial.print(MAX_MSG_LEN);
    Serial.println(" bytes): ");
    while (!Serial.available()); // Wait until a character is received
      Serial.readBytesUntil('\n', (char*)plaintext, MAX_MSG_LEN);
//    plaintext = Serial.readString();
    plaintext_len = strlen((char*)plaintext);

    // Encrypt the plaintext
    int encrypted = crypto_aead_encrypt(ciphertext, &ciphertext_len, plaintext, plaintext_len, NULL, 0, NULL, iv, key);
    if (encrypted == 0)
    {
        // Print the ciphertext
        Serial.print("Ciphertext (length ");
        Serial.print(ciphertext_len);
        Serial.print("): ");
        for(int i = 0; i < ciphertext_len; i++)
        {
            Serial.print(ciphertext[i], HEX);
        }
        Serial.println("");
    }
    else
    {
        Serial.println("Encryption failed!");
        return;
    }

    // Prompt user for ciphertext2
    Serial.print("Enter ciphertext2 (max ");
    Serial.print(MAX_MSG_LEN);
    Serial.println(" bytes): ");
    while (!Serial.available()); // Wait until a character is received
      Serial.readBytesUntil('\n', (char*)ciphertext2, MAX_MSG_LEN);
//    ciphertext2 = Serial.readString();
    ciphertext2_len = strlen((char*)ciphertext2);

    // Decrypt the ciphertext
    int decrypted = crypto_aead_decrypt(decrypted_plaintext, &decrypted_plaintext_len, NULL, ciphertext2, ciphertext2_len, NULL, 0, iv, key);

    if (decrypted == 0)
    {
        // Print the decrypted plaintext
        Serial.print("Decrypted plaintext (length ");
        Serial.print(decrypted_plaintext_len);
        Serial.print("): ");
        Serial.println((char*)decrypted_plaintext);
    }
    else
    {
        Serial.println("Decryption failed!");
        return;
    }
}

