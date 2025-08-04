/***********************************************************************
 *  File Name   : main.c
 *  Description : Entry point of the Steganography application.
 *                Handles command-line arguments and delegates control
 *                to encoding or decoding functionalities.
 *
 *                Supported Operations:
 *                - Encoding: Embeds a secret file into a BMP image.
 *                - Decoding: Extracts a hidden file from a stego BMP.
 *
 *                Usage:
 *                - Encoding:
 *                  ./a.out -e <source.bmp> <secret.ext> <output.bmp>
 *
 *                - Decoding:
 *                  ./a.out -d <stego.bmp> <output_file>
 *
 *  Author      : Pankaj Kumar
 *  Roll No     : 25008_018
 *  Date        : 30-Jul-2025
 ***********************************************************************/

#include <stdio.h>
#include <string.h>

#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        fprintf(stderr, "Correct Syntax: \n");
        fprintf(stderr, "For Encoding : %s -e <source_file.bmp> <secret_file(\".txt\", \".jpg\", \".sh\", \".c\")> <output_file.bmp>\n", argv[0]);
        fprintf(stderr, "For Decoding : %s -d <source_file.bmp> <output_file(\".txt\", \".jpg\", \".sh\", \".c\")> \n", argv[0]);   
        return -1;
    } 
    Status operation = check_operation_type(argv);
    // check opreation type
    if(operation == e_unsupported)
    {
        return -1;
    }
    // IF => e_encode
    if(operation == e_encode)
    {
        EncodeInfo encodeInfo;
        if(argc >= 4 && argc <= 5)
        {
            // Validate the input CLA
            if(read_and_validate_encode_args(argv, &encodeInfo) == e_failure)
                return e_failure;

            // Start the encoding
            do_encoding(&encodeInfo);
        }
        else
        {
            fprintf(stderr, "Correct Syntax for decoding: \n");
            fprintf(stderr, "%s -e <source_file.bmp> <secret_file(\".txt\", \".jpg\", \".sh\", \".c\")> <output_file.bmp>\n", argv[0]);
        }
    }
    // IF => e_decode
    if(operation == e_decode)
    {
        DecodeInfo decodeInfo = {0};
        if(argc >= 3 && argc <= 4)
        {
            // Validate the input CLA
            if(read_and_validate_decode_args(argv, &decodeInfo) == e_failure)
                return e_failure;

            // Start the Dencoding
            do_decoding(&decodeInfo);
        }
    }
    return 0; 
}