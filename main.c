#include <stdio.h>
#include <stdlib.h>

#define JAAT_IMPLEMENTATION

#include "./lib/JAAT.h"


int main(void){
 
  // put 68 and add 1 until it reach 420
  vm_programm prg = {
    .programm_lenght = 9,
    .inst_array = {
      "PUT(69)",
      "POP()",
      "ADC(0,1)",
      "PRT()",
      "CMP(0,420)",
      "JEQ(8)",
      "NXT()",
      "JMP(1)",
      "HLT()"
    }
  };

  // fibonacci sequence
  vm_programm prg2 = {
    .programm_lenght = 9,
    .inst_array = {
      "PUT(1)",
      "PUT(1)",
      "ADC#(0,1)",
      "SWP(0,1)",
      "PRT(1)", 
      "CMP(1, 46368)",
      "JEQ(8)",
      "JMP(2)",
      "HLT()"
    }
  };

  vm_programm prg3 = {
    .programm_lenght = 10,
    .inst_array = {
      "PUT(1)",
      "PUT(1)",
      "PUT(0)",
      "ADC#(0,1)",
      "SWP(0,1)",
      "INC(2)",
      "CMP(2, 17)",
      "JNZ(3)",
      "PRT(1)",
      "HLT()"      
    }
  };
  // print a string with a variation of PRT
  vm_programm prg4 = {
    .programm_lenght = 2,
    .inst_array = {
      "PRT('Hello World')",
      "HLT()"
    }
  };
  jaat_start();   // start point
  
  printf("Programm 1\n");
  jaat_load_programm(&prg);
  jaat_loop();

  printf("Programm 2\n");
  jaat_load_programm(&prg2);
  jaat_loop();

  printf("Programm 3\n");
  jaat_load_programm(&prg3);
  jaat_loop();

  printf("Programm 4\n");
  jaat_load_programm(&prg4);
  jaat_loop();

  jaat_free();    // end point 
  return 0;
}


