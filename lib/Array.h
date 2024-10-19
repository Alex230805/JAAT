#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


// single file library for dynamic array in c


#define u8t uint8_t
#define u16t uint16_t
#define u32t uint32_t
#define u64t uint64_t

#define START_DIM 8

#ifndef TYPE

#define TYPE u8t

#endif

typedef struct{
  TYPE* arr;
  size_t size;
  size_t pointer;
  size_t nelem;
}Array;

#define array_new(Array_h)  \
  Array_h = (Array*)malloc(sizeof(Array));\
  Array_h->arr = (TYPE*)malloc(sizeof(TYPE)*START_DIM);\
  Array_h->size = START_DIM;\
  Array_h->pointer = 0;\
  Array_h->nelem = 0;

#define array_push(Array_h, DATA) \
  if(Array_h->pointer+1 > Array_h->size){\
    array_realloc(Array_h);\
  } \
  Array_h->arr[Array_h->pointer] = DATA;\
  Array_h->pointer += 1;\
  Array_h->nelem += 1;


#define array_pop(Array_h, DATA) \
  Array_h->pointer -= 1;\
  if(Array_h->pointer < 0){\
    Array_h->pointer = 0;\
    Array_h->nelem = 0;\
  }\
  DATA = Array_h->arr[Array_h->pointer];\
  Array_h->arr[Array_h->pointer] = 0;\
  Array->nelem -= 1;

#define array_get(Array_h, POINTER,DATA)\
  if(POINTER >= 0 && POINTER <= Array_h->size){\
    DATA = Array_h->arr[POINTER];\
  }else{\
    DATA = NULL;\
  }

#define array_realloc(Array_h) \
  TYPE* new_arr = (TYPE*)malloc(sizeof(TYPE)*Array_h->size*2);\
  for(int i=0;i<Array_h->nelem;i++){\
    new_arr[i] = Array_h->arr[i];\
  }\
  free(Array_h->arr);\
  Array_h->arr = new_arr;\
  Array_h->size = Array_h->size *2;

#define array_free(Array_h) \
  free(Array_h->arr);\
  Array_h->arr = 0;\
  free(Array_h);\
  Array_h = 0;

#define array_cpy(Array_h, POINTER, SIZE, DATA) \
  if(POINTER >= 0 && POINTER <= Array_h->size){\
    memcpy(DATA, &Array_h->arr[POINTER], SIZE);\
  }

#endif
