#include "../header.h"

void caesar_encrypt(const char *plain, char *cipher, int shift)
{
    shift = ((shift % 26) + 26) % 26;
    for (int i = 0; plain[i]; i++)
    {
        char c = plain[i];
        if (c >= 'a' && c <= 'z')
            cipher[i] = 'a' + (c - 'a' + shift) % 26;
        else if (c >= 'A' && c <= 'Z')
            cipher[i] = 'A' + (c - 'A' + shift) % 26;
        else
            cipher[i] = c;
    }
}

void caesar_decrypt(const char *cipher, char *plain, int shift)
{
    caesar_encrypt(cipher, plain, -shift);
}

void caesar_bruteforce(const char *cipher)
{
    printf("Brute-force attacks:\n");
    char buf[256];
    for (int s = 0; s < 26; s++)
    {
        caesar_decrypt(cipher, buf, s);
        printf("  shift %2d: %s\n", s, buf);
    }
}
