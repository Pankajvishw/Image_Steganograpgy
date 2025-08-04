/***********************************************************************
 *  File Name   : decode.h
 *  Description : Header file for the Steganography Decoding Module.
 *                Contains structure definition and function declarations
 *                used for extracting hidden secret data from a BMP image
 *                using Least Significant Bit (LSB) technique.
 *
 *                Structures:
 *                - DecodeInfo
 *
 *                Functions:
 *                - read_and_validate_decode_args()
 *                - do_decoding()
 *                - open_image_file()
 *                - open_secret_file()
 *                - get_magic_string()
 *                - decode_magic_string()
 *                - decode_secret_file_extn_size()
 *                - decode_secret_file_extn()
 *                - decode_secret_file_size()
 *                - decode_secret_file_data()
 *                - decode_data_from_image()
 *                - decode_byte_from_lsb()
 *                - decode_int_from_lsb()
 *
 *  Author      : Pankaj Kumar
 *  Roll No     : 25008_018
 *  Date        : 30-Jul-2025
 ***********************************************************************/

#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h" 

/* 
 * Structure to store information required for
 * decoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Secret File Info */
    char *secret_fname;        // => Store the Secret file name
    FILE *fptr_secret;          // => Store the Secret file pointer
    char extn_secret_file[MAX_FILE_SUFFIX + 1]; // => Store the Secret file extension
    uint extn_size;              // => store the extn Size
    uint secret_size;            // => Store secret file size
    char *magic_string;          // => Store the Magic String
    /* Stego Image Info */
    char *stego_image_fname;    // => Store the Stego Image file name
    FILE *fptr_stego_image;     // => Store the Stego Image file pointer

} DecodeInfo;

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p file */
Status open_image_file(DecodeInfo *decInfo);

/* Get File pointers for o/p file */
Status open_secret_file(DecodeInfo *decInfo);

/* Decode Magic String */
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

/* Get Magic string from user*/
Status get_magic_string(DecodeInfo * decInfo);

/* Dencode secret file extenstion */
Status decode_secret_file_extn(char *file_extn, DecodeInfo *decInfo);

/* Dencode secret file extenstion size */
Status decode_secret_file_extn_size(uint *file_extn_size, DecodeInfo *decInfo);

/* Dencode secret file size */
Status decode_secret_file_size(uint *file_size, DecodeInfo *decInfo);

/* Dencode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Dencode function, which does the real encoding */
Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image);

/* Dencode a byte into LSB of image data array */
Status decode_byte_from_lsb(char *data,  char *image_buffer);

Status decode_int_from_lsb(uint * size,  FILE *fptr_stego_image);

#endif