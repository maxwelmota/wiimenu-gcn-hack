#!/usr/bin/env bash
# Rebuild the Wii Menu GameCube controller patch with Clang/LLD.
set -euo pipefail
cd "$(dirname "$0")"
for bin in clang ld.lld gcc g++ python3; do command -v "$bin" >/dev/null || { echo "Missing dependency: $bin" >&2; exit 1; }; done
for reg in US EU JP; do
  case "$reg" in
    US) text=0x8135A19C; rodata=0x81359E98; opt=() ;;
    EU) text=0x8135A244; rodata=0x81359F40; opt=(-DREGION_EU) ;;
    JP) text=0x8135965C; rodata=0x81359358; opt=(-DREGION_JP) ;;
  esac
  sed -e "s|@TEXT@|$text|" -e "s|@RODATA@|$rodata|" linker-template.ld > "${reg}.ld"
  clang -target powerpc-unknown-eabi -mcpu=750 -O1 -ffreestanding -fno-builtin -fno-stack-protector "${opt[@]}" -c gcnhook.c -o "hook_${reg}.o"
  ld.lld -m elf32ppc --no-undefined -T "${reg}.ld" -o "hook_${reg}.elf" "hook_${reg}.o"
done
gcc -O2 -c bytearray.c -o bytearray.o
g++ -std=c++20 -O2 -Wall builder.cc bytearray.o -o hack-builder
./hack-builder
python3 validate.py
