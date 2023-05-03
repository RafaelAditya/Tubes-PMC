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
  unsigned char plaintext[MAX_MSG_LEN] = {0};
  unsigned char ciphertext[MAX_MSG_LEN] = {0};

  unsigned long long ciphertext_len = 0;
  unsigned long long plaintext_len = 0;

  Serial.println("Enter plaintext (max 1023 bytes): ");
  while (Serial.available() == 0) {
    ; // wait for user input
  }
  plaintext_len = Serial.readBytesUntil('\n', plaintext, MAX_MSG_LEN);

  // Serial.readBytesUntil('\n', plaintext, MAX_MSG_LEN);
  // // Hitung panjang plaintext
  // plaintext_len = strlen((char*)plaintext);
  // Tambahkan null-terminator ke plaintext
  plaintext[plaintext_len] = '\0';
  // Cetak plaintext dan panjangnya
  Serial.print("Plaintext: ");
  Serial.println((char*)plaintext);
  Serial.print("Plaintext length: ");
  Serial.println(plaintext_len);

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

  unsigned char ciphertext2[MAX_MSG_LEN] = {0};
  unsigned long long ciphertext2_len = 0;
  char input[MAX_MSG_LEN * 2 + 1] = {0};
  Serial.println("Enter ciphertext2 (max 1023 bytes): ");
  while (Serial.available() == 0) {
    ; // wait for user input
  }
  Serial.readBytesUntil('\n', input, MAX_MSG_LEN * 2 + 1);
  ciphertext2_len = strlen(input) / 2;
  for (unsigned long long i = 0; i < ciphertext2_len; i++)
  {
      sscanf(&input[i * 2], "%2hhx", &ciphertext2[i]);
  }
  input[ciphertext2_len * 2] = '\0';

  unsigned char decrypted_plaintext[MAX_MSG_LEN] = {0};
  unsigned long long decrypted_plaintext_len = plaintext_len;
  int decrypted = crypto_aead_decrypt(decrypted_plaintext, &decrypted_plaintext_len, NULL, ciphertext2, ciphertext2_len, NULL, 0, iv, key);
  input[ciphertext2_len * 2] = '\0';
  Serial.print("Ciphertext2 input: ");
  Serial.println(input);

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
}