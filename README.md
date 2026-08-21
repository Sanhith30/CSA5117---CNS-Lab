# CSA5117 — Cryptography and Network Security Lab

This repository contains coursework, lab experiments, assessments, assignments, and a capstone project for the **CSA5117 (Cryptography and Network Security)** subject.

## About

The repo is organized as a complete CNS lab record, including:
- Classic cipher implementation experiments in C
- Assignment and assessment artifacts
- A capstone project on phishing website detection using ML, FastAPI, React, and a Chrome extension

## Repository Structure

- `experiments/` — 40 CNS lab experiment programs in C (Classical Ciphers, Block Ciphers, Public Key Cryptography, MACs & Signatures, Cryptanalysis)
- `Assignment/` — course assignment submissions
- `Assessment Tools/` — CO-based assessment documents (CO1, CO2, CO3)
- `activity/` — activity evidence files
- `Capstone/` — Intelligent Phishing Website Detection Platform (full-stack + ML)

## Experiments Index (1 – 40)

| Exp # | Experiment Title |
| :--- | :--- |
| **01** | Caesar Cipher (Substitution) |
| **02** | Monoalphabetic Substitution Cipher |
| **03** | Playfair Cipher Algorithm |
| **04** | Polyalphabetic Substitution Cipher (Vigenère Cipher) |
| **05** | Affine Caesar Cipher |
| **06** | Brute-force Attack on Affine Cipher |
| **07** | Simple Substitution Cipher |
| **08** | Monoalphabetic Substitution Cipher Analysis |
| **09** | Playfair Cipher Decryption |
| **10** | Playfair Matrix Generation |
| **11** | Playfair Cipher Key Space Analysis |
| **12** | Hill Cipher Encryption & Decryption |
| **13** | Chosen-Plaintext Attack on Hill Cipher |
| **14** | One-Time Pad (OTP) using Vigenère Cipher |
| **15** | Letter Frequency Attack on Additive (Caesar) Cipher |
| **16** | Letter Frequency Attack on Monoalphabetic Substitution Cipher (Top-10 Plaintexts) |
| **17** | DES Decryption Reverse Key Schedule Design |
| **18** | DES Subkey Generation with Disjoint 28-bit Halves (PC-2 Verification) |
| **19** | 3DES (Triple DES) in Cipher Block Chaining (CBC) Mode |
| **20** | Error Propagation Analysis in ECB and CBC Modes |
| **21** | Block Cipher 10* Padding Mechanism in ECB, CBC, and CFB Modes |
| **22** | S-DES (Simplified DES) in CBC Mode |
| **23** | S-DES (Simplified DES) in Counter (CTR) Mode |
| **24** | RSA Private Key Calculation (Trial Factoring & Extended Euclidean Algorithm) |
| **25** | RSA Common Factor Vulnerability Attack |
| **26** | RSA Leaked Private Key & Modulus Reuse Security Analysis |
| **27** | RSA Character-by-Character Encryption Codebook Attack |
| **28** | Diffie-Hellman Protocol Analysis & Flawed Variant Cryptanalysis |
| **29** | SHA-3 / Keccak Sponge State Matrix Lane Diffusion Simulation |
| **30** | CBC-MAC Extension & Forgery Attack on Variable Length Messages |
| **31** | CMAC Subkey Generation Algorithm (64-bit and 128-bit Rb Reduction) |
| **32** | DSA vs RSA Signatures Nonce Implications & Nonce Reuse Vulnerability |
| **33** | Full Data Encryption Standard (DES) Implementation (64-bit block, 56-bit key) |
| **34** | Block Cipher Mandatory Padding Motivation Analysis |
| **35** | One-Time Pad (OTP) Vigenère Cipher with Random Key Stream |
| **36** | Affine Caesar Cipher Bijectivity & Invertibility Analysis |
| **37** | Automated Letter Frequency Attack on Monoalphabetic Cipher |
| **38** | Known-Plaintext & Chosen-Plaintext Attack on Hill Cipher Matrix |
| **39** | Automated Frequency Cryptanalysis on Additive Cipher with Top-N Selection |
| **40** | Monoalphabetic Substitution Frequency Attack with Top-10 Likelihood Output |

## Capstone Snapshot

The capstone includes:
- **Backend:** FastAPI API with phishing risk prediction
- **Frontend:** React (Vite) dashboard
- **ML pipeline:** model training notebooks and deployment artifacts
- **Extension:** Chrome extension for quick phishing checks

See `/Capstone/README.md` for full setup and execution steps.


