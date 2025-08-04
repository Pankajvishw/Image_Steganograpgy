# 🖼️ Image Steganography in C (LSB on BMP Files)

This project is a simple yet powerful **Image Steganography** tool developed in **C**. It uses the **Least Significant Bit (LSB)** technique to hide a secret file inside a 24-bit BMP image. It also allows the use of a **custom magic string**, making it flexible and secure.

---

## 🚀 Features

- 🔐 **Hide any secret file** (`.txt`, `.c`, `.sh`, `.jpg`) inside a `.bmp` image
- 🧠 Uses **Least Significant Bit (LSB)** encoding on image pixel data
- 🔍 Validates encoded files using a **magic string** (default or user-defined)
- 🔓 Supports decoding of hidden files accurately
- 💻 Written in **pure C**, works on Linux/Windows (GCC)

---

## 🗂️ Project Structure



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

🧪 Supported File Types for Encoding
.txt
.sh
.c
.jpg
(You can try others too — as long as file size fits in the BMP image)

Magic String Support
During encoding, you can enter a custom magic string from the command line when prompted.
This string is embedded into the image and later used during decoding to validate if hidden data exists.

✅ User-defined at runtime (No need to hardcode in common.h)

🛡️ Acts like a secret key — decoding won't work if the magic string doesn’t match