#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Wire.h>
#include "api.h"
#include "encrypt.h"

#define KEY_LEN 16
#define IV_LEN 12
#define MAX_MSG_LEN 1024

void setup() {
  Serial.begin(115200);
}

void loop() {
  unsigned char key[KEY_LEN] = "mysecretkey1234";
  unsigned char iv[IV_LEN] = "myniceiv78";
  unsigned char ciphertext[] = {
    0x78, 0x3A, 0x9E, 0xFC, 0xE7, 0xF3, 0xE5, 0x2E, 0x09, 0x86, 0x74, 0xB5,
    0xCE, 0x6E, 0xFB, 0x34, 0x7D, 0x11, 0x67, 0x97, 0x03, 0x8F, 0xBA, 0x71,
    0xE1, 0xE3, 0x70, 0xE6, 0x3F, 0x09, 0xC1, 0xE2, 0x36, 0xC8, 0xE9, 0xD2,
    0xF3, 0x19, 0xEB, 0x43
  };
  unsigned long long ciphertext_len = sizeof(ciphertext);

  unsigned char decrypted_plaintext[MAX_MSG_LEN] = {0};
  unsigned long long decrypted_plaintext_len = MAX_MSG_LEN;

  unsigned long startTime = micros(); // Waktu awal

  int decrypted = crypto_aead_decrypt(decrypted_plaintext, &decrypted_plaintext_len, NULL, ciphertext, ciphertext_len, NULL, 0, iv, key);

  unsigned long endTime = micros(); // Waktu akhir
  unsigned long executionTime = endTime - startTime; // Selisih waktu eksekusi

  if (decrypted == 0)
  {
      Serial.print("Decrypted plaintext : ");
      Serial.print((char*)decrypted_plaintext);
      Serial.println("");
  }
  else
  {
      Serial.println("Decryption failed!");
  }

  Serial.print("Encryption execution time: ");
  Serial.print(executionTime);
  Serial.println(" us");

  uint32_t freeHeapBytes = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
  uint32_t totalHeapBytes = heap_caps_get_total_size(MALLOC_CAP_DEFAULT);
  float percentageHeapFree = freeHeapBytes * 100.0f / (float)totalHeapBytes;

  Serial.printf("[Memory] %.1f%% free - %d of %d bytes free\n", percentageHeapFree, freeHeapBytes, totalHeapBytes);

  // Menambahkan delay agar program tidak berjalan terus-menerus
  delay(2000);
}
