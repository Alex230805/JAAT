#ifndef DEVICES_H
#define DEVICES_H

#include <stdio.h>
#include <stdlib.h>

#ifdef DEV_1

// device header file

  #include "dev1.h"

#endif

#ifdef DEV_2

// device header file

  #include "dev2.h"

#endif

// call convention for a standard device call 


typedef struct{
  int arg_0;
  int arg_1;
  int arg_2;
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



/** 
 *    BASIC STRUCTURE:
 *
 *
 *    By including a device header with a dedicated preprocessor flag you importing the definition of the dedicated function of the device. 
 *    After that at the bottom of this file you can implement those methods. 
 *    One type of function is the "initialize_device" that take 0 arguments and return a "dev" struct with all the call reference to the device's 
 *    functions already setted up. 
 *    Next the array of devices will be initialized with all the device ready. 
 *    All of that is done by usign some preprocessing power of C 
 *
 *
 * **/


#ifdef DEVICES_IMP


#include "devices.h"

#ifdef DEV_1 

// here put the implementation for the methods inside dev1.h

#endif 

#ifdef DEV_1 

// here put the implementation for the methods inside dev2.h

#endif


#endif // devices imp

#endif
