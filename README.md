# Reverse Engineering Textbook Labs

The programs behind *esreveR Engineering for Beginners*, a free textbook for reading compiled software with IDA Free, Ghidra and dnSpy. Twenty chapters and 49 labs. Every lab that needs a binary has its source here and a compiled copy in `release/`.

Book and walkthrough: [Learn Reverse Engineering from Scratch](https://dboudreau.dev/2026/10/19/learn-reverse-engineering-from-scratch/)

**The book itself:** [`esreveR-Engineering-for-Beginners.pdf`](esreveR-Engineering-for-Beginners.pdf) (316 pages, 7 × 10 in, 6.7 MB).

> **In your VM environment only.** These are harmless training samples: none of them carries a payload or contacts a live host. They still behave like the things they imitate (a kill switch, a domain generator, a renamed system tool, packed and virtualized code), and antivirus products flag them. Run them in a virtual machine you can revert, never on your everyday PC.

## Get the labs

- **Clone this repo.** `release/` holds every compiled program, built from `src/`.
- **Or download the archive.** The same files as `release/`, in one AES-256 zip, from the [latest release](https://github.com/dboudreau00/Reverse-Engineering-Textbook-Labs/releases/latest) or from the book's page. The password is `dboudreau.dev`. It only keeps scanners from opening the file in transit, so it is not a secret.

## What is where

| Path | What it holds |
|---|---|
| `esreveR-Engineering-for-Beginners.pdf` | the textbook (7 × 10 in edition) |
| `src/c/` | C sources, resource files and `make_config.py`, one file per program |
| `src/dotnet/` | the four C# labs, one project each |
| `src/dataset/` | the Lab 12.5 dataset generator, feature extractor and trainer |
| `release/` | every compiled program, exactly as in the archive |
| `release.sha256` | SHA-256 of every file in `release/` |
| `build.sh` | rebuilds `release/` from `src/` |

## Which program belongs to which lab

| Lab | Programs | Source |
|---|---|---|
| 1 | `lab1.exe`, `lab1s.exe` | `src/c/lab1.c` |
| 5 | `lab5.exe` | `src/c/lab5.c` |
| 7 | `dotnet/Lab7/` | `src/dotnet/Lab7/` |
| 9a | `lab9.exe`, `lab9s.exe` | `src/c/lab9.c`, `lab9.rc`, `make_config.py` |
| 9b | `dotnet/Lab9Net/` | `src/dotnet/Lab9Net/` |
| 9c | `lab9c.exe` | `src/c/lab9c.c` |
| 10.2 | `lab9_upx.exe`, `lab9_unpacked.exe` | `lab9s.exe` packed with UPX, then unpacked |
| 10.3 | `labtool.exe`, `svch0st.exe` | `src/c/labtool.c`, `labtool.rc` (`svch0st.exe` is a renamed copy) |
| 10.6 | `killswitch.exe` | `src/c/killswitch.c` |
| 11.2 | `rotor.exe` | `src/c/rotor.c` |
| 11.3 | `dotnet/LabVault/` | `src/dotnet/LabVault/` |
| 11.4 | `curves.exe` | `src/c/curves.c` |
| 11.5 | `lab1_signed.exe` | `lab1s.exe` signed with a throwaway certificate |
| 11.7 | `apihash.exe`, `verify.exe` | `src/c/apihash.c`, `src/c/verify.c` |
| 12.4 | `corpus_O0.o` to `corpus_Os.o` | `src/c/corpus.c` |
| 12.5 | `dataset/` | `src/dataset/`, `src/c/corpus.c` |
| 12.6 | `killswitch_upx.exe` | `killswitch.exe` packed with UPX |
| 12.7 | `flat.exe` | `src/c/flat.c` |
| 13.1 | `greet.exe`, `greet_safe.exe`, `labhelper_tools.dll`, `labhelper_app.dll` | `src/c/greet.c`, `greet_safe.c`, `labhelper.c` |
| 13.4 | `labhelper_modified.dll` | `src/c/labhelper_modified.c` |
| 14.2 | `vmcheck.exe` | `src/c/vmcheck.c` |
| 14.3 | `dotnet/Lab14Obf/` | `src/dotnet/Lab14Obf/` |
| 16.2 | `notes.exe` | `src/c/notes.c` |
| 18.1 | `dga.exe` | `src/c/dga.c` |

The C# labs are framework-dependent (net8.0): install the .NET 8 runtime, then run `Lab7.exe` or `dotnet Lab7.dll`.

## Rebuild

```
sudo apt install gcc-mingw-w64-x86-64 binutils-mingw-w64-x86-64 upx-ucl osslsigncode openssl dotnet-sdk-8.0 python3 python3-venv
python3 -m venv .venv && . .venv/bin/activate && pip install pefile numpy
bash build.sh
```

`build.sh` writes the same tree as `release/` into `out/`. The toolchain is the one the book prints figures from: MinGW-w64 GCC 13.2 (msvcrt), UPX 4.2.2, osslsigncode 2.8, OpenSSL 3.0.13 and the .NET 8 SDK, so file sizes match (`lab9_upx.exe` is 8,192 bytes, `lab1s.exe` is 14,848). Hashes do not: every build stamps the time into the PE header and the signed copy gets a new certificate, so the packed files' entropy in `dataset.csv` moves in the fourth decimal. The book's tables, at two decimals, hold.

## Not included

The MSVC variants the book mentions (they need Visual Studio) and the Python scripts the labs ask you to write, which are printed in the book.
