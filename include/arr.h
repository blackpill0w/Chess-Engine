#ifndef _GENERIC_ARRAY_H
#define _GENERIC_ARRAY_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*!
  Abort and write an error to stderr.
  @param str: the error message to display.
*/
#define abort_with_arr_err(str) ({      \
   fprintf(stderr,                      \
      "\nError: %s, aborting...\n", str \
   ); abort(); 0; })

#define out_of_bound_arr_err() abort_with_arr_err("Accessing out of bound memory")

#define memory_allocation_arr_err() abort_with_arr_err("Memory allocation failed")

/*!
  Define array for the given type, the produced type is arrNAME_SUFFIX.
  Note: suffix is required to allow passing `struct myStruct` as a type.
*/
#define arr_typedef(TYPE, NAME_SUFFIX)  \
   typedef struct {                     \
      size_t _len;                      \
      TYPE *a;                          \
   } arr##NAME_SUFFIX

/*!
  Set size of the array and initialize elements to 0.
*/
#define arr_init(arr, size)                        \
   arr._len = size;                                \
   arr.a = calloc(size, sizeof( *((arr).a) ));     \
   if (arr.a == NULL) memory_allocation_arr_err()

/*!
  Free allocated resources.
*/
#define arr_free(arr) free((arr).a)

/*!
  Get array's length.
  @return length of the array.
*/
#define arr_len(arr) (arr)._len

//! Get an item from the array using an index, abort if the index is out of bound.
#define arr_at(arr, i) (arr).a[ (i) >= (arr)._len ? out_of_bound_arr_err() : (i) ]

#endif // _GENERIC_ARRAY_H
