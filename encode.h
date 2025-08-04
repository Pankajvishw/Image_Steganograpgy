/***********************************************************************
 *  File Name   : encode.h
 *  Description : Header file for the Steganography Encoding Module.
 *                Contains structure definition and function declarations
 *                used for embedding secret data into a BMP image file.
 *
 *                Structures:
 *                - EncodeInfo
 *
 *                Functions:
 *                - check_operation_type()
 *                - read_and_validate_encode_args()
 *                - do_encoding()
 *                - open_files()
 *                - check_capacity()
 *                - get_image_size_for_bmp()
 *                - get_file_size()
 *                - copy_bmp_header()
 *                - encode_magic_string()
 *                - encode_secret_file_extn()
 *                - encode_secret_file_extn_size()
 *                - encode_secret_file_size()
 *                - encode_secret_file_data()
 *                - encode_data_to_image()
 *                - encode_byte_to_lsb()
 *                - encode_int_to_lsb()
 *                - copy_remaining_img_data()
 *
 *  Author      : Pankaj Kumar
 *  Roll No     : 25008_018
 *  Date        : 30-Jul-2025
 ***********************************************************************/

#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _EncodeInfo
{
    /* Source Image info */
    char *src_image_fname;      // => Store the Src Image file name 
    FILE *fptr_src_image;       // => Store the Src Image file pointer
    uint image_capacity;         // => Store the image capacity in bytes

    /* Secret File Info */
    char *secret_fname;        // => Store the Secret file name
    FILE *fptr_secret;          // => Store the Secret file pointer
    char extn_secret_file[MAX_FILE_SUFFIX]; // => Store the Secret file extension
    int secret_size;            // => Store the Secret file size
    int extn_size;              // => Store the Secret file extn Size

    /* Stego Image Info */
    char *stego_image_fname;    // => Store the Stego Image file name
    FILE *fptr_stego_image;     // => Store the Stego Image file pointer

} EncodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo);

/* Encode secret file extenstion */
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo);

/* Encode secret file extenstion size */
Status encode_secret_file_extn_size(uint file_extn_size, EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data,  char *image_buffer);

Status encode_int_to_lsb(uint size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

#endif
