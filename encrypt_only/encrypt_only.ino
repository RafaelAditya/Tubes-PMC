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
  uint32_t freeHeapBytes = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
  uint32_t totalHeapBytes = heap_caps_get_total_size(MALLOC_CAP_DEFAULT);
  float percentageHeapFree = freeHeapBytes * 100.0f / (float)totalHeapBytes;

  static int messageCounter = 1;

  unsigned char key[KEY_LEN] = "mysecretkey1234";
  unsigned char iv[IV_LEN] = "myniceiv78";
  unsigned char plaintext[MAX_MSG_LEN] = {0};
  unsigned char ciphertext[MAX_MSG_LEN] = {0};

  unsigned long long ciphertext_len = 0;
  unsigned long long plaintext_len = 0;

  // Serial.println("Enter plaintext (max 1023 bytes): ");
  // while (Serial.available() == 0) {
  //   ; // wait for user input
  // }
  // plaintext_len = Serial.readBytesUntil('\n', plaintext, MAX_MSG_LEN);

  // // Serial.readBytesUntil('\n', plaintext, MAX_MSG_LEN);
  // // // Hitung panjang plaintext
  // // plaintext_len = strlen((char*)plaintext);
  // // Tambahkan null-terminator ke plaintext
  // plaintext[plaintext_len] = '\0';
  // // Cetak plaintext dan panjangnya
  // Serial.print("Plaintext: ");
  // Serial.println((char*)plaintext);
  // Serial.print("Plaintext length: ");
  // Serial.println(plaintext_len);

  snprintf((char*)plaintext, MAX_MSG_LEN, "altitude_%d;speed_%d;orientation_%d", messageCounter, messageCounter, messageCounter);
  plaintext_len = strlen((char*)plaintext);

  unsigned long startTime = micros(); // Waktu awal

  int encrypted = crypto_aead_encrypt(ciphertext, &ciphertext_len, plaintext, plaintext_len, NULL, 0, NULL, iv, key);
  if (encrypted == 0)
  {
      Serial.print("Ciphertext : ");
      for (unsigned long long i = 0; i < ciphertext_len; i++)
      {
          Serial.printf("%02X", ciphertext[i]);
      }
      Serial.println("");
  }
  else
  {
      Serial.println("Encryption failed!");
      return;
  }

  unsigned long endTime = micros(); // Waktu akhir
  unsigned long executionTime = endTime - startTime; // Selisih waktu eksekusi

  Serial.print("Encryption execution time: ");
  Serial.print(executionTime);
  Serial.println(" us");

  Serial.printf("[Memory] %.1f%% free - %d of %d bytes free\n", percentageHeapFree, freeHeapBytes, totalHeapBytes);

  messageCounter++;
  delay(2000);
}