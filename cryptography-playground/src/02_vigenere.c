#include "../header.h"

void vigenere_encrypt(const char *plain, char *cipher, const char *key)
{
    int klen = (int)strlen(key);
    if (klen == 0) return;
    for (int i = 0; plain[i]; i++)
    {
        char c = plain[i];
        int k = key[i % klen];
        int shift = (k >= 'a' && k <= 'z') ? (k - 'a') : (k >= 'A' && k <= 'Z') ? (k - 'A') : 0;
        if (c >= 'a' && c <= 'z')
            cipher[i] = 'a' + (c - 'a' + shift) % 26;
        else if (c >= 'A' && c <= 'Z')
            cipher[i] = 'A' + (c - 'A' + shift) % 26;
        else
            cipher[i] = c;
    }
}

void vigenere_decrypt(const char *cipher, char *plain, const char *key)
{
    int klen = (int)strlen(key);
    if (klen == 0) return;
    for (int i = 0; cipher[i]; i++)
    {
        char c = cipher[i];
        int k = key[i % klen];
        int shift = (k >= 'a' && k <= 'z') ? (k - 'a') : (k >= 'A' && k <= 'Z') ? (k - 'A') : 0;
        if (c >= 'a' && c <= 'z')
            plain[i] = 'a' + (c - 'a' - shift + 26) % 26;
        else if (c >= 'A' && c <= 'Z')
            plain[i] = 'A' + (c - 'A' - shift + 26) % 26;
        else
            plain[i] = c;
    }
}
