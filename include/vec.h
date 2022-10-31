#ifndef _GENERIC_VECTOR_H
#define _GENERIC_VECTOR_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*!
  Abort and write an error to stderr.
  @param str: the error message to display.
*/
#define _vec_abort_with_err(str) ({ fprintf(stderr, "\nError: %s, aborting...\n", str); abort(); 0; })

#define _vec_out_of_bound_err() _vec_abort_with_err("Accessing out of bound memory")

#define _vec_memory_allocation_err() _vec_abort_with_err("Memory allocation failed")

#define _vec_pop_empty_vec_err() _vec_abort_with_err("removing elements from an empty vector")

/*!
  Define vector for the given type, the produced type is vecNAME_SUFFIX.
  Note: suffix is required to allow passing struct SOMETHING as a type.
*/
#define vec_typedef(TYPE, NAME_SUFFIX)  \
   typedef struct {                     \
      size_t _len;                      \
      size_t _capacity;                 \
      TYPE *v;                          \
   } vec##NAME_SUFFIX

//! Initialize a vector to 0.
#define vec_init(vec) memset(&(vec), 0, sizeof(vec));   \
   (vec)._capacity = 1;                                 \
   (vec).v = malloc(sizeof( *((vec).v) ));              \
   if (vec.v == NULL) _vec_memory_allocation_err()

//! Free allocated resources.
#define vec_free(vec) free((vec).v)

//! Get vector's length.
#define vec_len(vec) (vec)._len

//! Get vector's capacity.
#define vec_capacity(vec) (vec)._capacity

//!  Check if a vector is empty.
#define vec_isempty(vec) ((vec)._len == 0)

//! Get an item from the vector using an index, abort if the index is out of bound.
#define vec_at(vec, i) vec.v[ (i) >= vec._len ? _vec_out_of_bound_err() : (i) ]

//! Insert an item into the vector.
#define vec_push(vec, item)                                 \
   {                                                        \
      if ((vec)._len == (vec)._capacity) {                  \
         (vec).v = realloc(                                 \
            (vec).v,                                        \
            ((vec)._capacity *= 2) * sizeof( *((vec).v) )   \
         );                                                 \
         if (vec.v == NULL) _vec_memory_allocation_err();    \
      }                                                     \
      (vec).v[(vec)._len++] = item;                         \
   }

//! Remove the last item, and return it.
#define vec_pop(vec) ((vec)._len == 0) ? _vec_pop_empty_vec_err() : (vec).v[ --((vec)._len) ]

//! Empty the vector.
#define vec_clear(vec) ((vec)._len = 0)

/*!
   Increase the capacity of the vector.
   If n is less than the vector's capacity, no changes are made.
*/
#define vec_reserve(vec, n)                                    \
   if ((vec)._capacity < n) {                                  \
      (vec).v = realloc((vec).v, n * sizeof((vec).v[0]) );     \
      if (vec.v == NULL) _vec_memory_allocation_err();          \
      (vec)._capacity = n;                                     \
   }                                                           \

#endif // _GENERIC_VECTOR_H
