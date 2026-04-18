# 🛡️ QuantumVault-PQC Library
A simplified C library for Post-Quantum Cryptography using NIST-standardized algorithms.

## 🚀 Features
- **Key Encapsulation (ML-KEM/Kyber-512):** Securely establish shared secrets.
- **Digital Signatures (ML-DSA/Dilithium3):** Verify identity and data integrity.
- **Simplified API:** Easy-to-use wrappers over the complex `liboqs` engine.

## 🛠️ Requirements
- **Linux/WSL2** (Ubuntu recommended)
- **liboqs:** Must be installed in `/usr/local/`
- **OpenSSL:** For underlying cryptographic support

## 📦 How to Build
To compile the static (`.a`) and shared (`.so`) libraries along with the test suite:
```bash
make clean
make
