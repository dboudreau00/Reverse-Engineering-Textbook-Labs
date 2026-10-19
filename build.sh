#!/usr/bin/env bash
# Rebuilds every program in release/ from src/, into ./out (or the folder you name).
#
# Ubuntu 24.04 or WSL:
#   sudo apt install gcc-mingw-w64-x86-64 binutils-mingw-w64-x86-64 upx-ucl osslsigncode openssl dotnet-sdk-8.0 python3 python3-venv
#   python3 -m venv .venv && . .venv/bin/activate && pip install pefile numpy
#
#   bash build.sh [output folder]
#
# These toolchain versions give the sizes the book prints (lab9_upx.exe is 8,192 bytes, lab1s.exe is 14,848 bytes):
# MinGW-w64 GCC 13.2 (msvcrt), UPX 4.2.2, osslsigncode 2.8, OpenSSL 3.0.13, .NET SDK 8.
# Build timestamps and the throwaway signing certificate differ per run, so hashes will not match release/; sizes will.
set -euo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
mkdir -p "${1:-$HERE/out}"
OUT="$(cd "${1:-$HERE/out}" && pwd)"
CC=x86_64-w64-mingw32-gcc
RC=x86_64-w64-mingw32-windres
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

for tool in "$CC" "$RC" gcc upx osslsigncode openssl dotnet python3; do
  command -v "$tool" >/dev/null || { echo "missing tool: $tool" >&2; exit 1; }
done
python3 -c "import pefile, numpy" 2>/dev/null || { echo "missing Python packages: pip install pefile numpy" >&2; exit 1; }

cp "$HERE"/src/c/* "$WORK"/
cd "$WORK"

# the Lab 9a configuration, embedded as a resource
python3 make_config.py
$RC lab9.rc -O coff -o lab9.res
$RC labtool.rc -O coff -o labtool.res

# C programs, with the flags the labs give
$CC -O2 -o lab1.exe lab1.c
$CC -O2 -s -o lab1s.exe lab1.c
$CC -O2 -s -o lab5.exe lab5.c
$CC -O2 -o lab9.exe lab9.c lab9.res
$CC -O2 -s -o lab9s.exe lab9.c lab9.res
$CC -O2 -s -o lab9c.exe lab9c.c
$CC -O2 -s -o labtool.exe labtool.c labtool.res
cp labtool.exe svch0st.exe
$CC -O2 -s -o killswitch.exe killswitch.c -lwininet
for n in rotor curves apihash verify vmcheck notes dga greet greet_safe; do
  $CC -O2 -s -o "$n.exe" "$n.c"
done
$CC -O2 -o flat.exe flat.c
$CC -O2 -s -shared -o labhelper_tools.dll labhelper.c
$CC -O2 -s -shared -DWHERE='"application folder"' -o labhelper_app.dll labhelper.c
$CC -O2 -s -shared -o labhelper_modified.dll labhelper_modified.c

# Lab 10.2 and Lab 12.6: packed and unpacked copies
upx -q --best -o lab9_upx.exe lab9s.exe
upx -q -d -o lab9_unpacked.exe lab9_upx.exe
upx -q --best -o killswitch_upx.exe killswitch.exe

# Lab 11.5: a signed copy of lab1s.exe, with a certificate made for this run and thrown away
openssl req -x509 -newkey rsa:2048 -keyout key.pem -out cert.pem -days 365 -nodes \
  -subj "/CN=esreveR Lab Code Signing" -addext "extendedKeyUsage=codeSigning" 2>/dev/null
osslsigncode sign -certs cert.pem -key key.pem -n "Lab 1" -h sha256 -in lab1s.exe -out lab1_signed.exe >/dev/null
osslsigncode verify -in lab1_signed.exe -CAfile cert.pem >/dev/null
rm -f key.pem

# Lab 12.4: the corpus at five optimisation levels (native ELF objects, as the lab says)
for level in O0 O1 O2 O3 Os; do
  gcc "-$level" -c -fno-asynchronous-unwind-tables -fcf-protection=none -o "corpus_$level.o" corpus.c
done

mkdir -p "$OUT"
cp ./*.exe ./*.dll ./corpus_O*.o "$OUT"/

# Lab 12.5: 80 programs, 40 of them packed, and the model trained on them
mkdir -p "$WORK/ds"
cp corpus.c "$HERE"/src/dataset/*.py "$WORK/ds"/
(cd "$WORK/ds" && python3 gen_dataset.py && python3 train.py)
mkdir -p "$OUT/dataset"
cp -r "$WORK/ds/samples" "$WORK/ds/dataset.csv" "$WORK/ds/model.json" "$OUT/dataset"/
cp "$WORK/ds/corpus.c" "$WORK/ds"/*.py "$OUT/dataset"/

# the four C# labs: net8.0, with a Windows apphost next to each dll
for p in Lab7 Lab9Net LabVault Lab14Obf; do
  mkdir -p "$WORK/dn/$p"
  cp "$HERE/src/dotnet/$p"/* "$WORK/dn/$p"/
  (cd "$WORK/dn/$p" && dotnet build -c Release -r win-x64 --self-contained false -o bin -nologo -v q >/dev/null)
  mkdir -p "$OUT/dotnet/$p"
  cp "$WORK/dn/$p/bin/$p.exe" "$WORK/dn/$p/bin/$p.dll" "$WORK/dn/$p/bin/$p.runtimeconfig.json" "$OUT/dotnet/$p"/
done

echo "built $(find "$OUT" -type f | wc -l) files into $OUT"
