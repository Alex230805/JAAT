#include <stdio.h>
#include <stdlib.h>

#define JAAT_IMPLEMENTATION

#include "./lib/JAAT.h"


int main(void){

  instruction_pool = (BYTE_LENGHT*)malloc(sizeof(BYTE_LENGHT)*3*pool_size);
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
  jaat_load_programm(&prg);
  parse_instruction();
  jaat_loop();
  free(instruction_pool);
  return 0;
}


