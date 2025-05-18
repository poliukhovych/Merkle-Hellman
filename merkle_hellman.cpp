#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include "merkle_hellman.h"

int main() {
    std::ofstream logFile("mh_log.txt", std::ios::app);
    logFile << "=== New Session ===\n";

    std::cout << "==============================\n"
              << " Merkle-Hellman Cryptosystem \n"
              << "==============================\n";

    const int n = 8;
    std::cout << "Generating keys for " << n << "-bit blocks...\n";
    MHKey key = generate_key(n);
    logFile << "[Key Generation]\n";
    logFile << "w=[";
    for (size_t i = 0; i < key.w.size(); ++i) logFile << key.w[i] << (i+1<key.w.size()?",":"");
    logFile << "] m=" << key.m << " a=" << key.a << " a_inv=" << key.a_inv << "\n";
    logFile << "Public b=[";
    for (size_t i = 0; i < key.b.size(); ++i) logFile << key.b[i] << (i+1<key.b.size()?",":"");
    logFile << "]\n";

    std::cout << "Public key (b): ";
    for (auto bi : key.b) std::cout << bi << " ";
    std::cout << "\n";

    std::vector<ll> last_cipher;
    while (true) {
        std::cout << "\nMenu:\n"
                  << "1) Encrypt\n"
                  << "2) Decrypt\n"
                  << "3) Exit\n"
                  << "Enter choice: ";
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter 1, 2, or 3.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            std::cout << "Enter plaintext: ";
            std::string message;
            std::getline(std::cin, message);
            last_cipher = encrypt(message, key, logFile);
            std::cout << "Ciphertext: ";
            for (auto c : last_cipher) std::cout << c << " ";
            std::cout << "\n";
        } else if (choice == 2) {
            if (last_cipher.empty()) {
                std::cout << "No ciphertext. Perform encryption first.\n";
            } else {
                std::string decrypted = decrypt(last_cipher, key, logFile);
                std::cout << "Decrypted: " << decrypted << "\n";
                logFile << "[Ciphertext cleared after decryption]\n";
                last_cipher.clear();
            }
        } else if (choice == 3) {
            std::cout << "Goodbye!\n";
            logFile << "=== Session End ===\n";
            break;
        } else {
            std::cout << "Invalid choice. Please select 1-3.\n";
        }
    }
    return 0;
}
