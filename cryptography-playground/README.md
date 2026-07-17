# Cryptography Playground

An interactive C99 command-line toolkit implementing classical and modern cryptographic algorithms from scratch. All implementations are educational — not production-grade.

## Building

```bash
cd cryptography-playground
gcc -std=c99 -Wall -Wextra -O2 src/*.c -o build/cryptography.exe -lm
```

Run: `build\cryptography.exe`

## Algorithms

### 1. Caesar Cipher

A **substitution cipher** where each letter is shifted by a fixed number of positions in the alphabet.

- **Encryption**: `c = (p + shift) mod 26`
- **Decryption**: `p = (c - shift) mod 26`
- **Attack**: Brute force — try all 26 shifts and inspect output. Frequency analysis can also break it.

> **Key space**: 26 keys only. Trivially breakable by modern standards.

### 2. Vigenère Cipher

A **polyalphabetic substitution** that uses a keyword to determine varying shift values.

- **Encryption**: For each position `i`, shift by `key[i mod len(key)]`.
- **Decryption**: Reverse shift using same key.
- **Attack**: Kasiski examination or Friedman test to find key length, then frequency analysis per column.

> **Key space**: `26^L` where `L` is key length. More secure than Caesar but still vulnerable to statistical analysis.

### 3. AES-128 (Simplified)

A **block cipher** encrypting 16-byte blocks with a 16-byte key using 10 rounds of substitution-permutation network.

**Operations** (per round):
- **SubBytes**: Each byte replaced via fixed S-box (nonlinear substitution).
- **ShiftRows**: Rows of state matrix shifted cyclically (diffusion).
- **MixColumns**: Each column multiplied by fixed matrix in GF(2⁸) (diffusion). Omitted in final round.
- **AddRoundKey**: State XOR'd with round key derived from cipher key.

**Key Expansion**: 4-word cipher key expanded to 44 words (11 round keys) using RotWord, SubWord, and Rcon constants.

**Decryption**: Inverse operations using inverse S-box, inverse ShiftRows, inverse MixColumns.

> **Security**: 128-bit key → `2¹²⁸` brute-force complexity. No practical attack exists.

### 4. RSA

An **asymmetric (public-key) cryptosystem** based on the difficulty of factoring the product of two large primes.

**Key Generation**:
1. Choose primes `p`, `q` (small primes via trial division for demo).
2. Compute `n = p × q` and `φ(n) = (p-1)(q-1)`.
3. Choose public exponent `e` (typically 3 or 65537) coprime with `φ(n)`.
4. Compute private exponent `d = e⁻¹ mod φ(n)` (extended Euclidean algorithm).

**Operations**:
- **Encrypt**: `c = m^e mod n`
- **Decrypt**: `m = c^d mod n`
- **Sign**: `s = m^d mod n`
- **Verify**: `m = s^e mod n` — if result matches original, signature is valid.

> **Security**: Based on integer factorization. With small primes (demo), trivially breakable. Production RSA uses 2048–4096-bit primes.

### 5. SHA-256 (Secure Hash Algorithm)

A **cryptographic hash function** producing a 256-bit (32-byte) digest from arbitrary-length input.

**Properties**:
- **Preimage resistance**: given hash `h`, infeasible to find `m` with `SHA256(m) = h`.
- **Second preimage resistance**: given `m₁`, infeasible to find `m₂ ≠ m₁` with same hash.
- **Collision resistance**: infeasible to find any `m₁ ≠ m₂` with same hash.

**Algorithm**:
1. **Preprocessing**: Pad message to multiple of 512 bits. Append `1` bit, then zeros, then 64-bit length.
2. **Hash computation**: Process each 512-bit block through 64 rounds using:
   - 64 constant words `K[0..63]`
   - 8 working variables `(a,b,c,d,e,f,g,h)` initialized to specific hash values
   - σ₀, σ₁, Σ₀, Σ₁, `Ch`, `Maj` functions (bitwise operations)
3. **Output**: Concatenate final `H[0..7]` as 32 bytes.

> **Security**: Output space = `2²⁵⁶`. No known collision attacks better than birthday bound (`2¹²⁸`).

## File Structure

```
cryptography-playground/
├── README.md
├── header.h              # Common declarations and includes
├── src/
│   ├── main.c            # Menu-driven interactive CLI
│   ├── 01_caesar.c       # Caesar cipher (encrypt/decrypt/bruteforce)
│   ├── 02_vigenere.c     # Vigenère cipher
│   ├── 03_aes.c          # AES-128 single block (S-box, key expansion, all rounds)
│   ├── 04_rsa.c          # RSA key generation, encrypt/decrypt, sign/verify
│   ├── 05_hash.c         # SHA-256 from scratch
│   └── 06_utils.c        # Hex printing, alloc helpers
└── build/
    └── cryptography.exe
```

## References

- FIPS 197 (AES Standard)
- FIPS 180-4 (SHA Standard)
- Rivest, Shamir, Adleman, *A Method for Obtaining Digital Signatures and Public-Key Cryptosystems* (1978)
- Stallings, *Cryptography and Network Security*, 7th ed.
