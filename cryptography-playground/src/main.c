#include "../header.h"

void demo_caesar(void)
{
    printf("\n=== CAESAR CIPHER ===\n");
    char text[256], buf[256];
    printf("Enter text: ");
    clear_input();
    if (!fgets(text, 256, stdin)) return;
    size_t len = strlen(text);
    if (len > 0 && text[len-1] == '\n') text[len-1] = '\0';
    int shift;
    printf("Shift: "); scanf("%d", &shift);
    caesar_encrypt(text, buf, shift);
    printf("Encrypted: %s\n", buf);
    caesar_decrypt(buf, buf, shift);
    printf("Decrypted: %s\n", buf);
    printf("Brute force:\n");
    caesar_bruteforce(buf);
}

void demo_vigenere(void)
{
    printf("\n=== VIGENERE CIPHER ===\n");
    char text[256], key[256], buf[256];
    clear_input();
    printf("Enter text: ");
    if (!fgets(text, 256, stdin)) return;
    size_t len = strlen(text);
    if (len > 0 && text[len-1] == '\n') text[len-1] = '\0';
    printf("Key: ");
    if (!fgets(key, 256, stdin)) return;
    len = strlen(key);
    if (len > 0 && key[len-1] == '\n') key[len-1] = '\0';
    vigenere_encrypt(text, buf, key);
    printf("Encrypted: %s\n", buf);
    vigenere_decrypt(buf, buf, key);
    printf("Decrypted: %s\n", buf);
}

void demo_aes(void)
{
    printf("\n=== AES-128 (single block) ===\n");
    uint8_t plain[16] = "Hello AES128!!!";
    uint8_t key[16] = "MySecretKey12345";
    uint8_t cipher[16], decrypted[16];

    printf("Plain:  "); print_hex(plain, 16);
    printf("Key:    "); print_hex(key, 16);
    aes_encrypt_block(plain, cipher, key);
    printf("Cipher: "); print_hex(cipher, 16);
    aes_decrypt_block(cipher, decrypted, key);
    printf("Decr:   "); print_hex(decrypted, 16);
}

void demo_rsa(void)
{
    printf("\n=== RSA ===\n");
    RSAKey pub, priv;
    rsa_generate_keys(&pub, &priv, 16);
    long long msg = 42;
    printf("Original message: %lld\n", msg);
    long long c = rsa_encrypt(msg, pub.e, pub.n);
    printf("Encrypted:        %lld\n", c);
    long long m = rsa_decrypt(c, priv.d, priv.n);
    printf("Decrypted:        %lld\n", m);
    long long sig = rsa_sign(msg, priv.d, priv.n);
    printf("Signature:        %lld\n", sig);
    long long v = rsa_verify(sig, pub.e, pub.n);
    printf("Verified:         %lld\n", v);
    printf("Signature valid:  %s\n", (v == msg) ? "YES" : "NO");
}

void demo_hash(void)
{
    printf("\n=== SHA-256 ===\n");
    const char *test = "Hello, Cryptography!";
    uint8_t hash[32];
    sha256_hash((const uint8_t*)test, strlen(test), hash);
    printf("Input:  %s\n", test);
    printf("SHA-256: "); print_hex(hash, 32);
    const char *test2 = "The quick brown fox jumps over the lazy dog";
    sha256_hash((const uint8_t*)test2, strlen(test2), hash);
    printf("Input:  %s\n", test2);
    printf("SHA-256: "); print_hex(hash, 32);
}

void all_demos(void)
{
    demo_caesar();
    demo_vigenere();
    demo_aes();
    demo_rsa();
    demo_hash();
}

int main(void)
{
    int choice;
    do {
        printf("\n===== CRYPTOGRAPHY PLAYGROUND =====\n");
        printf("1) Caesar Cipher\n");
        printf("2) Vigenere Cipher\n");
        printf("3) AES-128\n");
        printf("4) RSA\n");
        printf("5) SHA-256\n");
        printf("6) All Demos\n");
        printf("0) Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { choice = -1; clear_input(); }
        switch (choice) {
            case 1: demo_caesar(); break;
            case 2: demo_vigenere(); break;
            case 3: demo_aes(); break;
            case 4: demo_rsa(); break;
            case 5: demo_hash(); break;
            case 6: all_demos(); break;
            case 0: printf("Goodbye!\n"); break;
            default: printf("Invalid option.\n"); clear_input();
        }
    } while (choice != 0);
    return 0;
}
