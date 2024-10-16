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
  jaat_start();   // start point
  
  jaat_load_programm(&prg);
  parse_instruction();
  jaat_loop();
  jaat_load_programm(&prg2);
  parse_instruction();
  jaat_loop();
 
  jaat_free();    // end point 
  return 0;
}


