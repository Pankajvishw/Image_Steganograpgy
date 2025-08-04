/***********************************************************************
 *  File Name   : decode.c
 *  Description : Source file for the Steganography Decoding Module.
 *                Contains the implementation of functions to extract
 *                hidden data from a BMP image using the Least Significant
 *                Bit (LSB) technique.
 *
 *                Functions:
 *                - open_image_file()
 *                - open_secret_file()
 *                - read_and_validate_decode_args()
 *                - do_decoding()
 *                - decode_data_from_image()
 *                - decode_byte_from_lsb()
 *                - get_magic_string()
 *                - decode_magic_string()
 *                - decode_secret_file_extn_size()
 *                - decode_secret_file_extn()
 *                - decode_secret_file_size()
 *                - decode_secret_file_data()
 *                - decode_int_from_lsb()
 *
 *  Author      : Pankaj Kumar
 *  Roll No     : 25008_018
 *  Date        : 30-Jul-2025
 ***********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "common.h"
#include "decode.h"

Status open_image_file(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if(decInfo->fptr_stego_image == NULL)
    {
    	fprintf(stderr, "ERROR: No source File found with name \"%s\"\n", decInfo->stego_image_fname);
    	return e_failure;
    }
    return e_success;
}

Status open_secret_file(DecodeInfo *decInfo)
{
    char *ptr = strchr(decInfo->secret_fname, '.');
    // Replacing the file extension after the dot to the encoded extension if exists
    if(ptr != NULL)
        strcpy(ptr, decInfo->extn_secret_file);
    else
        strcpy(decInfo->secret_fname + strlen(decInfo->secret_fname), decInfo->extn_secret_file);
    // opening the secret file in binary write mode
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "wb");
    // Do Error handling
    if(decInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_fname);
    	return e_failure;
    }
    printf("Secret file with name \"%s\" created Successfully\n", decInfo->secret_fname);
    // No failure return e_success
    return e_success;
}

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    // If argv[2] is .bmp file
    decInfo->stego_image_fname = malloc(50);
    char *ext = strstr(argv[2], ".bmp");
    if (ext != NULL && strcmp(ext, ".bmp") == 0)
        strcpy(decInfo->stego_image_fname, argv[2]);
    else
    {
        fprintf(stderr, "Error: Source_Image Should be \".bmp\" File\n");
        return e_failure;
    }
    decInfo->secret_fname = malloc(50);
    // If argv[3] Exists 
    if(argv[3] != NULL)
        strcpy(decInfo->secret_fname, argv[3]);
    else
    {
        // Default secret file name
        strcpy(decInfo->secret_fname, "my_secret");
    }
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    // opening the image in binary read mode
    if(open_image_file(decInfo) == e_failure)
        return e_failure;
    // setting the file pointer after the header
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);

    // To get the magic string from the user
    if(get_magic_string(decInfo) == e_failure) return e_failure;
    if(decode_magic_string(decInfo->magic_string, decInfo) == e_failure) return e_failure;
    if(decode_secret_file_extn_size(&decInfo->extn_size, decInfo) == e_failure) return e_failure;
    if(decode_secret_file_extn(decInfo->extn_secret_file, decInfo) == e_failure) return e_failure;
    // opening the secrat file
    if(open_secret_file(decInfo) == e_failure) return e_failure;
    if(decode_secret_file_size(&decInfo->secret_size, decInfo) == e_failure) return e_failure;
    if(decode_secret_file_data(decInfo) == e_failure) return e_failure;
    // Freeing the allocated memory for file names
    free(decInfo->secret_fname);  
    free(decInfo->stego_image_fname);
    free(decInfo->magic_string);
    // closing the open files
    fclose(decInfo->fptr_secret);
    fclose(decInfo->fptr_stego_image);
    printf("Decoding Completed Successfully\n");
    return e_success;
}

Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image)
{
    // creating buffer to read 8 butes from the encoded image   
    unsigned char image_buffer[MAX_IMAGE_BUF_SIZE];
    if (!data || !fptr_stego_image)
        return e_failure;
    
    for(int i = 0; i < size; i++)
    {
        // reading 8 bytes from the encoded mage for each 1 byte data
        if(fread(image_buffer, MAX_IMAGE_BUF_SIZE, 1, fptr_stego_image) != 1)
            return e_failure;
        // calling the function to decode lsb bit into the data
        if(decode_byte_from_lsb(&data[i], image_buffer) == e_failure)
            return e_failure;
    }
    return e_success;
}

Status decode_byte_from_lsb(char *data,  char *image_buffer)
{
    *data = 0;
    // Checking if the image buffer is NULL
    if(image_buffer == NULL)
        return e_failure;
    for(int j = 0 ; j < MAX_IMAGE_BUF_SIZE; j++)
    {
        // getting the lsb bit from the buffer and setting it to nth position of the data 
        *data = *data | ((image_buffer[j] & 1) << (7 - j));
    }
    return e_success;
}

Status get_magic_string(DecodeInfo * decInfo)
{
    decInfo->magic_string = malloc(50);
    memset(decInfo->magic_string, 0, 50);
    printf("Enter the magic string kays : ");
    if(scanf(" %s", decInfo->magic_string) != 1) 
        return e_failure;
    return e_success;
}

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    int len = strlen(magic_string);
    // creating magic string buffer based on the original magic string length
    char temp_ms[len + 1];
    temp_ms[len] = '\0';
    // calling the decode fns to decode magic string
    if(decode_data_from_image(temp_ms, strlen(magic_string),decInfo->fptr_stego_image) == e_failure)
    {
        fprintf(stderr, "Error: Failed to decode Magic String\n");
        return e_failure;
    }
    // checking if the decoded string matches with the original magic string
    if(strcmp(temp_ms, magic_string) != 0)
    {
        fprintf(stderr, "The entered Magic string \"%s\" Not found\n", magic_string);
        return e_failure;
    }

    printf("Magic String Decoded Successfully\n");
    return e_success;
}

Status decode_secret_file_extn_size(uint *file_extn_size, DecodeInfo *decInfo)
{
    // Creating buffer for to store ext size;
    if(decode_int_from_lsb(file_extn_size, decInfo->fptr_stego_image) == e_failure)
        return e_failure;

    printf("File extion Size %d Decoded Successfully\n", *file_extn_size);
    return e_success;
}

Status decode_secret_file_extn(char *file_extn, DecodeInfo *decInfo)
{
    // Creating extension buffer to store the decoded extension
    char extn[decInfo->extn_size];
    // calling the decode fns to decode the file ext
    if(decode_data_from_image(extn, decInfo->extn_size, decInfo->fptr_stego_image) == e_failure)
    {
        fprintf(stderr, "Error: Failed to decode File extension Name\n");
        return e_failure;
    }
    if(strstr(extn, ".txt"))
        strncpy(decInfo->extn_secret_file, extn, 4);
    else if(strstr(extn, ".jpg"))
        strncpy(decInfo->extn_secret_file, extn, 4);
    else if(strstr(extn, ".c"))
        strncpy(decInfo->extn_secret_file, extn, 2);
    else if(strstr(extn, ".sh"))
        strncpy(decInfo->extn_secret_file, extn, 3);

    printf("File extention \"%s\" Decoded Successfully\n", decInfo->extn_secret_file);
    return e_success;
}

Status decode_secret_file_size(uint* file_size, DecodeInfo *decInfo)
{
    // Creating the buffer to store secret file size
    if(decode_int_from_lsb(file_size, decInfo->fptr_stego_image) == e_failure)
        return e_failure;

    printf("Secret File Size %d Decoded Successfully\n", *file_size);
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    // creating buffer character to store decoded characters into the secret file
    unsigned char secret_data[decInfo->secret_size];
    // calling the decode fns to decode each enoded character from the encoded image
    if(decode_data_from_image(secret_data, decInfo->secret_size , decInfo->fptr_stego_image) == e_failure)
    {
        fprintf(stderr, "Error: Failed to decode Secret File Data frome %s\n", decInfo->stego_image_fname);
        return e_failure;
    }
    // writing the decode character into the secret file
    if(fwrite(secret_data, decInfo->secret_size, 1, decInfo->fptr_secret) != 1)
    {
        fprintf(stderr, "Error: Failed to write secrat file data into the file %s\n", decInfo->secret_fname);
        return e_failure;
    }
    printf("Secret File Data Decoded Successfully\n");
    return e_success;
}

Status decode_int_from_lsb(uint * size,  FILE *fptr_stego_image)
{
    *size = 0;
    unsigned char buffer[32];
    for(int i = 0; i < 32; i++)
    {
        if(fread(&buffer[i], 1, 1, fptr_stego_image) != 1)
            return e_failure;
        
        *size = ((buffer[i] & 1) << (31 - i)) | *size;
    }
    return e_success;
}