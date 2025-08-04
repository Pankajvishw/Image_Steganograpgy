/***********************************************************************
 *  File Name   : encode.c
 *  Description : Source file for the Steganography Encoding Module.
 *                Implements functions for embedding secret data into
 *                a BMP image using Least Significant Bit (LSB) technique.
 *
 *                Functions:
 *                - get_image_size_for_bmp()
 *                - open_files()
 *                - check_operation_type()
 *                - read_and_validate_encode_args()
 *                - do_encoding()
 *                - check_capacity()
 *                - get_file_size()
 *                - copy_bmp_header()
 *                - encode_magic_string()
 *                - encode_data_to_image()
 *                - encode_byte_to_lsb()
 *                - encode_secret_file_extn_size()
 *                - encode_secret_file_extn()
 *                - encode_secret_file_size()
 *                - encode_secret_file_data()
 *                - encode_int_to_lsb()
 *                - copy_remaining_img_data()
 *
 *  Author      : Pankaj Kumar
 *  Roll No     : 25008_018
 *  Date        : 30-Jul-2025
 ***********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BM P Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	fprintf(stderr, "ERROR: No Source file found with name \"%s\"\n", encInfo->src_image_fname);
    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	fprintf(stderr, "ERROR: No Secrat file found with name \"%s\"\n", encInfo->secret_fname);
    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to Create output file with name \"%s\"\n", encInfo->stego_image_fname);
    	return e_failure;
    }
    printf("Output File Created Successfully with Name \"%s\"\n", encInfo->stego_image_fname);
    // No failure return e_success
    return e_success;
}

OperationType check_operation_type(char *argv[])
{
    // -e for enoding
    if(strcmp(argv[1], "-e") == 0)
        return e_encode;
    // -d for decoding
    if(strcmp(argv[1], "-d") == 0) 
        return e_decode;
    fprintf(stderr, "Error: Invalid Operation => %s\n", argv[1]);
    return e_unsupported;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // If argv[2] is .bmp file
    encInfo->src_image_fname = malloc(50);
    char *ext = strstr(argv[2], ".bmp");
    if (ext != NULL && strcmp(ext, ".bmp") == 0)
    {
        strcpy(encInfo->src_image_fname, argv[2]);
    }
    else
    {
        fprintf(stderr, "Error: Source_Image Should be \".bmp\" File\n");
        return e_failure;
    }
    // If argv[3] has valid extension
    ext = strrchr(argv[3], '.');
    if(ext == NULL)
    {
        fprintf(stderr, "Error: No file extension found in secret file name : \"%s\"\nRecommended File extension : \".txt\" or \".jpg\" or \".c\" or \".sh\"\n", argv[3]);
            return e_failure;
    }
    encInfo->secret_fname = malloc(50);
    strcpy(encInfo->secret_fname, argv[3]);
    if (strcmp(ext, ".c") == 0)
        strcpy(encInfo->extn_secret_file, ".c");
    else if (strcmp(ext, ".sh") == 0)
        strcpy(encInfo->extn_secret_file, ".sh");
    else if(strcmp(ext, ".txt") == 0)
        strcpy(encInfo->extn_secret_file, ".txt");
    else if(strcmp(ext, ".jpg") == 0)
        strcpy(encInfo->extn_secret_file, ".jpg");
    else
    {
        fprintf(stderr, "Error: Secret_file Should be \".txt\" or \".c\" or \".sh\" File\n");
        return e_failure;
    }
    // If argv[4] is present or not
    encInfo->stego_image_fname = malloc(50);
    if(argv[4] == NULL)
    {
        // Creating default file name argv[4] is not present
        strcpy(encInfo->stego_image_fname, "stego.bmp");
    }
    else
    {
        // If argv[4] is .bmp file 
        if(strcmp(strstr(argv[4], ".bmp"), ".bmp") == 0)
            strcpy(encInfo->stego_image_fname, argv[4]);
        else
        {
            fprintf(stderr, "Error: Destination_Image Should be \".bmp\" File\n");
            return e_failure;
        }
    }
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_success)
    {
        // Checking the Image and secret file capacity is valid or not
        if(check_capacity(encInfo) == e_failure)
        {
            fprintf(stderr, "Error: File Size is incompatible to encode\n");
            return e_failure;
        }
        if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure) 
            return e_failure;

        // Taking magic string from user to match with the encoded magic string
        char *magic_string = malloc(50);
        printf("Enter the Magic string keys : ");
        scanf(" %s", magic_string);
        if(encode_magic_string(magic_string, encInfo) == e_failure) return e_failure;
        if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_failure) return e_failure;
        if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_failure) return e_failure;
        if(encode_secret_file_size(encInfo->secret_size, encInfo) == e_failure) return e_failure;
        if(encode_secret_file_data(encInfo) == e_failure) return e_failure;
        if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure) return e_failure;
        // Freeing allocated memory for magic string
        free(magic_string);
    }
    else
        return e_failure; 



    // Freeing the allocated memory for file name
    free(encInfo->src_image_fname);
    free(encInfo->secret_fname);
    free(encInfo->stego_image_fname);
    // closing the open files
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_secret);
    fclose(encInfo->fptr_stego_image);
    printf("Encoding Completed Successfully\n");
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->secret_size = get_file_size(encInfo->fptr_secret);
    if(get_image_size_for_bmp(encInfo->fptr_src_image) < 54 + 8 * (strlen(MAGIC_STRING) + 8 + strlen(encInfo->extn_secret_file) + encInfo->secret_size))
        return e_failure;
    return e_success;
}

uint get_file_size(FILE *fptr)
{
    // set the file pointer to end
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    // Creating a buffer to store header
    unsigned char header[54];
    // Setting the file pointer to beginning
    fseek(fptr_src_image, 0, SEEK_SET);
    // Reading the BMP header
    if(fread(header, 54, 1, fptr_src_image) != 1)
    {
        fprintf(stderr, "Error: Failed to read Header\n");
        return e_failure;
    }
    // Writing the BMP header 
    if(fwrite(header, 54, 1, fptr_dest_image) != 1)
    {
        fprintf(stderr, "Error: Failed to write Header\n");
        return e_failure;
    }
    printf("Header Copied Successfully\n");
    return e_success;
}

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    if(encode_data_to_image(magic_string, strlen(magic_string), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        fprintf(stderr, "Error: Failed to encode Magic String\n");
        return e_failure;
    }
    printf("Magic String \"%s\" Encoded Successfully\n", magic_string);
    return e_success;
}

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    // Creating buffer for each 8 bits
    unsigned char image_buffer[MAX_IMAGE_BUF_SIZE];
    if (!data || !fptr_src_image || !fptr_stego_image)
        return e_failure;
    
    for(int i = 0; i < size; i++)
    {
        // Reading 8 bytes from the src image
        if(fread(image_buffer, MAX_IMAGE_BUF_SIZE, 1, fptr_src_image) != 1)
            return e_failure;
        // Encoding the 1 byte data into the 8 bytes
        if(encode_byte_to_lsb(data[i], image_buffer) == e_failure)
            return e_failure;
        // Writing the 8 bytes to the stego image
        if(fwrite(image_buffer, MAX_IMAGE_BUF_SIZE, 1, fptr_stego_image) != 1)
            return e_failure;
    }
    return e_success;
}

Status encode_byte_to_lsb(char data,  char *image_buffer)
{
    // Checking if the buffer is NULL
    if(image_buffer == NULL)
        return e_failure;
    for(int j = 0 ; j < MAX_IMAGE_BUF_SIZE; j++)
    {
        // Clearing the LSB bit from te buffer
        image_buffer[j] = image_buffer[j] & ~1; 
        // Getting the nth bit from the data
        int mask = (data >> (7 - j)) & 1;
        // Setting the nth bit from the data into the LSB of buffer
        image_buffer[j] = image_buffer[j] | mask;
    }
    return e_success;
}

Status encode_secret_file_extn_size(uint file_extn_size, EncodeInfo *encInfo)
{
    // Calling the encode data fns to encode file ext size
    if(encode_int_to_lsb(file_extn_size, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        fprintf(stderr, "Error: Failed to encode File extension Size\n");
        return e_failure;
    }
    printf("File ext Size %d Encoded Successfully\n", file_extn_size);
    return e_success;
}

Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    // Calling the enocode data fns to encode secret file ext
    if(encode_data_to_image(file_extn, strlen(file_extn), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        fprintf(stderr, "Error: Failed to encode File extension Name\n");
        return e_failure;
    }
    printf("File extenion \"%s\" Encoded Successfully\n", file_extn);
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    // Calling the encode data fns to encode secret file size
    uint size = file_size;
    if(encode_int_to_lsb(size, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        fprintf(stderr, "Error: Failed to encode Secret File Size\n");
        return e_failure;
    } 
    printf("Secret File Size %d Encoded Successfully\n", size);
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    // creating buffer character pass copy one by one character
    unsigned char secret_data[encInfo->secret_size];
    rewind(encInfo->fptr_secret);
    // Reading 1 byte from the secret file one by one 
    if(fread(secret_data, encInfo->secret_size, 1, encInfo->fptr_secret) != 1)
    {
        fprintf(stderr, "Error:failed to read secret file data\n");
        return e_failure;
    }
    // Calling the encode data fns to encode each character 
    if (encode_data_to_image(secret_data, encInfo->secret_size, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        fprintf(stderr, "Error: Failed to encode Secret File data\n");
        return e_failure;
    }
    printf("Secret File Data Encoded Successfully\n");
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    unsigned char ch;
    // Reading character from the src image file one by one
    while(fread(&ch, 1, 1, fptr_src) == 1)
    {
        // Writing the charcter into the des image file one by one
        if(fwrite(&ch, 1, 1, fptr_dest) != 1)
        {
            fprintf(stderr, "Error: Failed to Copy Remaining Source Image Data\n");
            return e_failure;
        }
    }
    printf("Remaining Data copied Successfully\n");
    return e_success;
}

Status encode_int_to_lsb(uint size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char buffer[32];
    for(int i = 0; i < 32; i++)
    {
        if(fread(&buffer[i], 1, 1, fptr_src_image) != 1)
            return e_failure;
        buffer[i] = (buffer[i] & ~1) | ((size >> (31 - i)) & 1); 

        if(fwrite(&buffer[i], 1, 1, fptr_stego_image) != 1)
            return e_failure;
    }
    return e_success;
}