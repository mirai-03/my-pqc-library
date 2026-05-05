# 🛡️ QuantumVault-PQC

> **A High-Level Hybrid Post-Quantum Cryptographic Library for C**

QuantumVault-PQC is a developer-friendly C library that wraps NIST-standardized Post-Quantum Cryptography (PQC) algorithms with industry-proven AES-256-GCM encryption to deliver end-to-end data security — built for the quantum era, usable today.
**Developed under the initiative of Corecys Private Limited.**

---

## 📋 Table of Contents

- [Why QuantumVault-PQC?](#why-quantumvault-pqc)
- [The Three Pillars](#the-three-pillars)
- [Technical Architecture](#technical-architecture)
- [Prerequisites](#prerequisites)
- [Installation & Build](#installation--build)

---

## ❓ Why QuantumVault-PQC?

Classical public-key cryptography — RSA, ECDH, and ECDSA — will be vulnerable to sufficiently powerful quantum computers running Shor's algorithm. The transition to post-quantum algorithms is no longer theoretical; NIST finalized its first PQC standards in 2024. However, integrating these algorithms directly into applications is often complex and error-prone.

QuantumVault-PQC eliminates that complexity. It provides a clean, high-level API on top of the [Open Quantum Safe (liboqs)](https://openquantumsafe.org/) engine, combined with OpenSSL's battle-tested AES-GCM for symmetric encryption — giving you a full cryptographic stack in just a few function calls.

---

## 🏛️ The Three Pillars

QuantumVault-PQC is built around three complementary security primitives that together solve the core problems of secure communication.

### 1. Identity & Authentication — ML-DSA (Dilithium)
Uses **ML-DSA-65** (formerly CRYSTALS-Dilithium), a NIST-standardized lattice-based signature scheme, to sign and verify messages. This ensures that the sender's identity is authenticated and that data has not been altered in transit.

### 2. Quantum-Safe Key Exchange — ML-KEM (Kyber)
Uses **ML-KEM-512/768** (formerly CRYSTALS-Kyber), a NIST-standardized key encapsulation mechanism, to establish a shared 32-byte secret between two parties. The exchange is protected against attacks from both classical and future quantum computers.

### 3. Authenticated File Encryption — AES-256-GCM
Uses the shared secret established by ML-KEM to encrypt physical files with **AES-256-GCM** (Galois/Counter Mode). GCM provides *authenticated encryption*: it not only scrambles your data but automatically detects tampering. Any modification to the ciphertext (even a single bit) will cause decryption to fail explicitly.

---

## ⚙️ Technical Architecture

| Property | Detail |
|---|---|
| **Language** | C (C99 Standard) |
| **PQC Engine** | [liboqs](https://github.com/open-quantum-safe/liboqs) — Open Quantum Safe |
| **Symmetric Engine** | OpenSSL (`libcrypto`) — AES-GCM, RAND_bytes |
| **Design Pattern** | Hybrid: PQC wraps the symmetric key; AES-GCM encrypts the bulk data |
| **Output Formats** | Static library (`libmypqc.a`), Shared library (`libmypqc.so`) |

**Hybrid Design Rationale:** PQC algorithms are significantly more computationally expensive than symmetric ciphers. QuantumVault-PQC uses PQC to securely encapsulate a 32-byte AES key (the "key wrapping" pattern) and then uses that key with AES-256-GCM for high-speed file encryption.

---

## 🛠️ Prerequisites

The following must be installed and accessible on your system before building:

* **Operating System:** Linux (Ubuntu/Debian recommended) or WSL2 on Windows.
* **Compiler:** `gcc` or `clang`
* **Build Tools:** `make`
* **Dependencies:**
    * **liboqs**: Must be compiled and installed (default headers at `/usr/local/include`, libraries at `/usr/local/lib`).
    * **OpenSSL**: Development headers are required. Install via:
        ```bash
        sudo apt-get install libssl-dev
        ```

---

## 📦 Installation & Build

Clone the repository and use the provided Makefile:

```bash
git clone [https://github.com/your-username/QuantumVault-PQC.git](https://github.com/your-username/QuantumVault-PQC.git)
cd QuantumVault-PQC
make
