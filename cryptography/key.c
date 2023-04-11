#include <stdio.h>
#include <string.h>

#define IV_SIZE 12
#define KEY_SIZE 16

int main()
{
    unsigned char iv[IV_SIZE];
    unsigned char key[KEY_SIZE];

    printf("Enter 12-byte IV (in hex format): ");
    scanf("%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx", &iv[0], &iv[1], &iv[2], &iv[3], &iv[4], &iv[5], &iv[6], &iv[7], &iv[8], &iv[9], &iv[10], &iv[11]);

    printf("Enter 16-byte key (in hex format): ");
    scanf("%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx", &key[0], &key[1], &key[2], &key[3], &key[4], &key[5], &key[6], &key[7], &key[8], &key[9], &key[10], &key[11], &key[12], &key[13], &key[14], &key[15]);

    printf("IV: ");
    for (int i = 0; i < IV_SIZE; i++)
    {
        printf("%02x", iv[i]);
    }
    printf("\n");

    printf("Key: ");
    for (int i = 0; i < KEY_SIZE; i++)
    {
        printf("%02x", key[i]);
    }
    printf("\n");

    return 0;
}
