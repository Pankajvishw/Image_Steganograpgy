# Steganography in C (BMP LSB Encoding)

This project implements a simple **steganography tool** using the **Least Significant Bit (LSB)** technique to hide secret data inside `.bmp` image files.

## 🔧 Features

- Encode secret `.txt`, `.c`, `.jpg`, etc. files inside 24-bit BMP images.
- Decode and retrieve secret files from stego images.
- Works with CLI arguments for flexible usage.

## 📂 File Structure

- `main.c` – Entry point; handles encoding/decoding mode selection.
- `encode.c / encode.h` – Logic for encoding secret data into images.
- `decode.c / decode.h` – Logic for decoding secret data from images.
- `types.h` – Custom type definitions.
- `common.h` – Global macros (like MAGIC_STRING).

## ⚙️ Compilation

```bash
gcc -o stego main.c encode.c decode.c
```

## Encoding
```bash
./stego -e <source.bmp> <secret.txt/.sh/.c/.jpg> <output.bmp>
```

## Decoding
```bash
./stego -d <output.bmp> <recovered_filename>
```