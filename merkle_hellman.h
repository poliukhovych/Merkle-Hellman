#pragma once

#include <vector>
#include <string>
#include <random>
#include <numeric>

using ll = long long;

inline ll gcd_ll(ll a, ll b) {
    return b == 0 ? a : gcd_ll(b, a % b);
}

struct MHKey {
    std::vector<ll> w;    // private superincreasing sequence
    std::vector<ll> b;    // public sequence
    ll m, a, a_inv;       // modulus, multiplier, inverse of a mod m
};

inline ll modinv(ll a, ll m) {
    ll m0 = m, x0 = 0, x1 = 1;
    if (m == 1) return 0;
    while (a > 1) {
        ll q = a / m;
        ll t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}

inline std::vector<ll> generate_superincreasing(int n) {
    std::vector<ll> w(n);
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<ll> dist(1, 10);
    ll sum = 0;
    for (int i = 0; i < n; ++i) {
        ll next = sum + dist(gen);
        w[i] = next;
        sum += next;
    }
    return w;
}

inline MHKey generate_key(int n) {
    MHKey key;
    key.w = generate_superincreasing(n);
    ll sum = std::accumulate(key.w.begin(), key.w.end(), 0LL);

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<ll> dist_m(sum + 1, sum + 20);
    key.m = dist_m(gen);

    std::uniform_int_distribution<ll> dist_a(2, key.m - 1);
    do {
        key.a = dist_a(gen);
    } while (gcd_ll(key.a, key.m) != 1);

    key.a_inv = modinv(key.a, key.m);
    key.b.resize(n);
    for (int i = 0; i < n; ++i) {
        key.b[i] = (key.a * key.w[i]) % key.m;
    }
    return key;
}

inline std::vector<ll> encrypt(const std::string &msg, const MHKey &key, std::ofstream &logFile) {
    int n = static_cast<int>(key.b.size());
    std::vector<ll> cipher;
    cipher.reserve(msg.size());
    logFile << "[Encryption Details]\n";
    for (char ch : msg) {
        ll sum = 0;
        std::vector<int> bits(n);
        for (int i = 0; i < n; ++i) {
            bits[i] = (ch >> i) & 1;
            if (bits[i]) sum += key.b[i];
        }
        cipher.push_back(sum);
        logFile << "Char '" << ch << "' (" << int(ch) << "): bits=[";
        for (int i = 0; i < n; ++i) logFile << bits[i] << (i+1<n?",":"");
        logFile << "] sum=" << sum << "\n";
    }
    logFile << "[End Encryption]\n";
    return cipher;
}

inline std::string decrypt(const std::vector<ll> &cipher, const MHKey &key, std::ofstream &logFile) {
    int n = static_cast<int>(key.w.size());
    std::string out;
    out.reserve(cipher.size());
    logFile << "[Decryption Details]\n";
    for (ll c : cipher) {
        logFile << "Cipher component c=" << c;
        ll c_prime = (key.a_inv * c) % key.m;
        logFile << ", after inv*a_inv mod m: " << c_prime << "\n";
        ll temp = c_prime;
        std::vector<int> bits(n);
        for (int i = n - 1; i >= 0; --i) {
            bits[i] = (temp >= key.w[i]);
            if (bits[i]) temp -= key.w[i];
        }
        char ch = 0;
        for (int i = 0; i < n; ++i) if (bits[i]) ch |= (1 << i);
        out.push_back(ch);
        logFile << "Recovered Char '" << ch << "' (" << int(ch) << "): bits=[";
        for (int i = 0; i < n; ++i) logFile << bits[i] << (i+1<n?",":"");
        logFile << "]\n";
    }
    logFile << "[End Decryption]\n";
    return out;
}
