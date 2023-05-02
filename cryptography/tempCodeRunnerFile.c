unsigned char ciphertext2[MAX_MSG_LEN];
    // memcpy(ciphertext2, ciphertext, ciphertext_len);
    // unsigned long long ciphertext2_len = ciphertext_len;
    // unsigned char decrypted2_plaintext[MAX_MSG_LEN];
    // unsigned long long decrypted2_plaintext_len = 0;
    // int decrypted2 = crypto_aead_decrypt(decrypted2_plaintext, &decrypted2_plaintext_len, NULL, ciphertext2, ciphertext2_len, NULL, 0, iv, key);
    // if (decrypted2 == 0)
    // {
    //     // Print the decrypted plaintext
    //     printf("Decrypted plaintext 2(length %llu): %.*s\n", decrypted2_plaintext_len, decrypted2_plaintext_len, decrypted2_plaintext);

    //     // fwrite(decrypted_plaintext, 1, decrypted_plaintext_len, stdout);
    //     printf("\n");
    // }
    // else
    // {
    //     printf("Decryption failed!\n");
    // }