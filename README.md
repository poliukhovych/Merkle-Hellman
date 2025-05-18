# Merkle-Hellman Knapsack Cryptosystem

This repository provides a simple C++ implementation of the classic Merkle-Hellman knapsack cryptosystem for educational purposes. It supports:

* **Key Generation**: Superincreasing private sequence, modulus *m*, multiplier *a* (with *gcd(a, m) = 1*), and public sequence *b*.
* **Encryption**: Byte-by-byte encryption of ASCII text into integer sums using the public key.
* **Decryption**: Greedy subset-sum approach with the private key to recover the original plaintext.

## Limitations

* **Fixed Block Size**: Designed for 8-bit blocks (ASCII). Extending to arbitrary message sizes or Unicode requires modifying block size and using a big-integer library.
* **Inferior RNG**: Uses `std::mt19937_64` for simplicity. Not suitable for production cryptography — replace with a cryptographically secure RNG (e.g., OS-provided CSPRNG).
* **Security**: The original Merkle-Hellman scheme is cryptographically broken. This implementation is for learning only — do **not** use for real security.

## Building

```bash
make all
```

## Usage

```bash
./mh_knapsack
```

Follow on-screen menu to encrypt or decrypt messages. Logs (bit expansion, sums, modular inverses) are appended to `mh_log.txt`.

## License

This code is released under the MIT License. Feel free to experiment and extend it for educational projects.
