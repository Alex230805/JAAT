#ifndef DEVICES_H
#define DEVICES_H

#include <stdio.h>
#include <stdlib.h>

#define DEV_1
#define DEV_2

#include "dev1.h"
#include "dev2.h"


typedef struct{
  int arg_0;
  int arg_1;
  int arg_2;
  static char* str;
}ex_d_int;


typedef void (*call)(ex_d_int* data);

typedef struct{
  call c0;  // function call reference
  int pos;  // device enumeration
}call_ref;

typedef struct{
  char* name;      // dev name
  call_ref* calls; // array of ports for device 
  int calls_length; // array length
}dev;


static dev* devices = NULL;       // array of devices
static int devices_length = 0;    // array of devices length 


void compose_devices();

#ifdef DEVICES_IMP

#include "devices.h"

void compose_devices(){
  
}

#endif
