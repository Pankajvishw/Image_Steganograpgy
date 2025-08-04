/***********************************************************************
 *  File Name   : types.h
 *  Description : Header file for user-defined types used across the
 *                Steganography project.
 *
 *                Type Definitions:
 *                - uint          : Unsigned integer alias for brevity.
 *                - Status        : Enum for function return statuses 
 *                                   (e_success, e_failure).
 *                - OperationType : Enum for operation mode (encoding,
 *                                   decoding, or unsupported).
 *
 *  Author      : Pankaj Kumar
 *  Roll No     : 25008_018
 *  Date        : 30-Jul-2025
 ***********************************************************************/

#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    e_encode,
    e_decode,
    e_unsupported
} OperationType;
#endif
