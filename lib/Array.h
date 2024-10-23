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


#ifndef ARRAY_IMPLEMENTATION

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
    TYPE* buffer = (TYPE*)malloc(sizeof(TYPE)*SIZE);\
    for(int i=0;i<SIZE;i++){\
      buffer[i] = Array_h-arr[POINTER+i];\
    }\
    DATA = buffer;\
  }

#else
#include "./Array.h"

void array_new(Array* Array_h);
void array_push(Array* Array_h, TYPE DATA);
void array_pop(Array*Array_h,TYPE DATA);
void array_get(Array* Array_h, int POINTER,TYPE DATA);
void array_free(Array*Array_h);
void array_cpy(Array*Array_h, int POINTER, int SIZE,TYPE* DATA);
void array_realloc(Array*Array_h);


void array_new(Array* Array_h){
  Array_h = (Array*)malloc(sizeof(Array));
  Array_h->arr = (TYPE*)malloc(sizeof(TYPE)*START_DIM);
  Array_h->size = START_DIM;
  Array_h->pointer = 0;
  Array_h->nelem = 0;
}

void array_push(Array* Array_h, TYPE DATA){
  if(Array_h->pointer+1 > Array_h->size){
    array_realloc(Array_h);
  }
  Array_h->arr[Array_h->pointer] = DATA;
  Array_h->pointer += 1;
  Array_h->nelem += 1;
}

void array_pop(Array*Array_h,TYPE DATA){
  Array_h->pointer -= 1;
  if(Array_h->pointer < 0){
    Array_h->pointer = 0;
    Array_h->nelem = 0;
  }
  DATA = Array_h->arr[Array_h->pointer];
  Array_h->arr[Array_h->pointer] = 0;
  Array_h->nelem -= 1;
}

void array_get(Array* Array_h, int POINTER,TYPE DATA){
  if(POINTER >= 0 && POINTER <= Array_h->size){
    DATA = Array_h->arr[POINTER];
  }else{
    DATA = (TYPE)-1;
  }
}
void array_realloc(Array*Array_h){
  TYPE* new_arr = (TYPE*)malloc(sizeof(TYPE)*Array_h->size*2);
  for(int i=0;i<Array_h->nelem;i++){
    new_arr[i] = Array_h->arr[i];
  }
  free(Array_h->arr);
  Array_h->arr = new_arr;
  Array_h->size = Array_h->size *2;
}

void array_free(Array*Array_h){
  free(Array_h->arr);
  Array_h->arr = 0;
  free(Array_h);
  Array_h = 0;
}


void array_cpy(Array*Array_h, int POINTER, int SIZE,TYPE* DATA){
  if(POINTER >= 0 && POINTER <= Array_h->size){
    TYPE* buffer = (TYPE*)malloc(sizeof(TYPE)*SIZE);
    for(int i=0;i<SIZE;i++){
      buffer[i] = Array_h->arr[POINTER+i];
    }
    DATA = buffer;
  }
}


#endif


#endif
