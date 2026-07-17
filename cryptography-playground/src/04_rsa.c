#include "../header.h"

static int is_prime(int n)
{
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return 0;
    return 1;
}

static long long mod_pow(long long base, long long exp, long long mod)
{
    long long r = 1;
    base %= mod;
    while (exp > 0)
    {
        if (exp & 1) r = (r * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return r;
}

static long long mod_inv(long long a, long long m)
{
    long long t = 0, newt = 1;
    long long r = m, newr = a;
    while (newr != 0)
    {
        long long q = r / newr;
        long long tmp = t; t = newt; newt = tmp - q * newt;
        tmp = r; r = newr; newr = tmp - q * newr;
    }
    if (r > 1) return -1;
    if (t < 0) t += m;
    return t;
}

void rsa_generate_keys(RSAKey *pub, RSAKey *priv, int bitlen)
{
    (void)bitlen;
    srand((unsigned int)time(NULL));
    int p = 0, q = 0;
    while (!is_prime(p)) p = 100 + rand() % 200;
    while (!is_prime(q) || q == p) q = 100 + rand() % 200;
    long long n = (long long)p * q;
    long long phi = (long long)(p - 1) * (q - 1);
    long long e = 65537;
    if (e >= phi) e = 3;
    while (phi % e == 0) e += 2;
    long long d = mod_inv(e, phi);
    pub->n = n; pub->e = e; pub->d = 0;
    priv->n = n; priv->e = e; priv->d = d;
    printf("RSA keys generated (p=%d, q=%d, n=%lld, phi=%lld)\n", p, q, n, phi);
}

long long rsa_encrypt(long long m, long long e, long long n)
{
    return mod_pow(m, e, n);
}

long long rsa_decrypt(long long c, long long d, long long n)
{
    return mod_pow(c, d, n);
}

long long rsa_sign(long long m, long long d, long long n)
{
    return mod_pow(m, d, n);
}

long long rsa_verify(long long s, long long e, long long n)
{
    return mod_pow(s, e, n);
}
